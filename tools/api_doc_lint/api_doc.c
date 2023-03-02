#include "api_doc.h"
#include "awtk.h"

#if defined(WIN32)
    const char* g_line_end = "\r\n";
#else
    const char* g_line_end = "\n";
#endif

typedef struct _mparam_t {
  char* type;
  char* name;
} mparam_t;

typedef struct _auto_fix_t {
  // 初始的时候存储源码 每一行的内容, 包含行结束符
  // 为了保证 lines 的行号 和 原先代码辅助解析出来的行号 始终一致, 自动修正过程中:
  // 插入一行时, 不会扩充 lines的size, 只是在相关的 line上追加
  // 删除一行时, 也不会缩减 lines的size, 只是把 line 内容设置为 空串
  darray_t* lines;
  uint32_t filesize;

  ca_symbol_t* sym;
  // [begin, end)
  int comment_line_begin;
  int comment_line_end;
  int32_t param_n;

  bool_t dirty;
} auto_fix_t;

typedef struct _check_ctx_t {
  code_assist_t* ca;
  const char* path;
  ca_symbols_t* syms;
  
  log_hook_t log_hook;
  void* log_ctx;

  char* prefix; // 保存enum的 prefix

  auto_fix_t auto_fix;
  int32_t cur_line; // 下标开始为1
  ca_symbol_t* last_enum;

  bool_t enable_auto_fix;
  auto_fix_hook_t fix_hook;
} check_ctx_t;

// ---------------------------- utils ---------------------------------

static const char* key_method = "@method";
static const char* key_annotation = "@annotation";
static const char* key_param = "@param";
static const char* key_return = "@return";
static const char* key_enum = "@enum";
static const char* key_prefix = "@prefix";
static const char* key_const = "@const";
static const char* key_class = "@class";
static const char* key_property = "@property";

static inline bool_t is_id_char (char c) { return isalnum(c) || c=='_' || c=='$'; }

static bool_t is_name_end(const char* s) {
  return *s == ' ' || *s == '\r' || *s == '\n' || *s == '\t' || *s == 0;
}

static inline bool_t is_space(char c) { return c == ' ' || c == '\t'; }


// ---------------------------- log_hook ---------------------------------


#if defined (WIN32) && defined (_MSC_VER)
#define app_err(fmt, ...) app_log(ctx, LOG_LEVEL_ERROR, fmt, __VA_ARGS__)
#define app_warn(fmt, ...) app_log(ctx, LOG_LEVEL_WARN, fmt, __VA_ARGS__)
#else
#define app_err(fmt, args...) app_log(ctx, LOG_LEVEL_ERROR, fmt, ##args)
#define app_warn(fmt, args...) app_log(ctx, LOG_LEVEL_WARN, fmt, ##args)
#endif

/*
 LOG_LEVEL_WARN
 LOG_LEVEL_ERROR
 */
static void default_log_hook(void* ctx, log_level_t level, const char* s) {
  if (level == LOG_LEVEL_ERROR) {
    log_error("%s", s);
  } else if (level == LOG_LEVEL_WARN) {
    log_warn("%s", s);
  } else {
    log_info("%s", s);
  }
}

static void app_log(check_ctx_t* ctx, log_level_t level, const char* fmt, ...) {
  va_list va;
  char buf[2048+1] = {0};
  return_if_fail(fmt != NULL);
  
  va_start(va, fmt);
  vsnprintf(buf, sizeof(buf), fmt, va);
  va_end(va);

  log_hook_t hook = ctx->log_hook ? ctx->log_hook : default_log_hook;
  hook(ctx->log_ctx, level, buf);
}

// ---------------------------- 自动修正相关代码 ---------------------------------

static str_t* dump_lines(check_ctx_t* ctx) {
  darray_t* lines = ctx->auto_fix.lines;
  str_t* str = str_create(ctx->auto_fix.filesize + 1024);

  str_t** arr = (str_t**)lines->elms;
  for (uint32_t i = 0; i < lines->size; ++i) {
    str_t* l = arr[i];
    str_append_with_len(str, l->str, l->size);
  }
  
  return str;
}

// push_back(lines, [beg, end])
static void append_line(darray_t* lines, const char* beg, const char* end) {
  // 稍微多预留一点
  uint32_t len = end-beg+1;
  str_t* str = str_create(64+len); 
  str_append_with_len(str, beg, len);
  darray_push(lines, str);
}

static void auto_fix_init(check_ctx_t* ctx) {
  darray_t* lines = darray_create(1024, (tk_destroy_t)str_destroy, NULL);
  ctx->auto_fix.lines = lines;

  uint32_t len;
  char* buffer = (char*)file_read(ctx->path, &len);
  ctx->auto_fix.filesize = len;

  const char *beg = buffer, *end = buffer;
  for (; *end; ++end) {
    if (*end == '\n') {
      append_line(lines, beg, end);
      beg = end+1;
    }
  }

  if (end != beg) {
    append_line(lines, beg, end-1);
  }

#if 0
  // 自动测试
  // 把lines再拼成文件内容, 看看 是否完全一致
  str_t* file_content = dump_lines(ctx);
  if (0 != strcmp(file_content->str, buffer)) {
    log_error("internal bug! %s\n", ctx->path);
  }
  str_destroy(file_content);
#endif

  TKMEM_FREE(buffer);
}

static void auto_fix_finit(check_ctx_t* ctx) {
  darray_destroy(ctx->auto_fix.lines);
}

static void auto_fix_init_sym(check_ctx_t* ctx, ca_symbol_t* sym) {
  auto_fix_t* af = &ctx->auto_fix;
  af->sym = sym;

  af->comment_line_begin = 0;
  af->comment_line_end = 0;

  // 我们的注释必须是 /** 开头的， 否则就不检测合法性了
  if ( !strstr(sym->comment, "/**") ) {
    return;
  }
  
  // 向前找注释, 先找结束符 再找开始符号
  // sym->line 是1开始的序号, lines中是0开始的  要减一
  // sym->line 对应的是 sym 的行号, 注释本来就至少在前面一行  要再减一
  for (int32_t i = sym->line-2; i>=0; --i) {
    str_t* line = darray_get(af->lines, i);
    if (!line) {
      log_error("%s(%d): fatal error code_assist line invalid\n", sym->file, i);
      break;
    }
    if (strstr(line->str, "/**")) {
      af->comment_line_begin = i;
      break;
    } else if (strstr(line->str, "*/")) {
      af->comment_line_end = i+1; // 区间惯例 [begin, end) , end 要大一格
    }
  }
}

static void auto_fix_type_impl(check_ctx_t* ctx, const char* new_type) {
  if (!ctx->enable_auto_fix) {
    return;
  }

  auto_fix_t* af = &ctx->auto_fix;
  str_t* line = darray_get(af->lines, ctx->cur_line-1);
  if (!line) {
    log_error("%s(%d) internal error, invalid cur_line \n", ctx->path, ctx->cur_line);
    return;
  }

  const char* beg = strstr(line->str, "{");;
  const char* end = strstr(beg, "}");
  if (end > beg) {
    str_t old, new_;
    str_init(&old, 128);
    str_init(&new_, 128);

    str_append_with_len(&old, beg, end-beg+1);
    str_append_more(&new_, "{", new_type, "}", NULL);
    str_replace(line, old.str, new_.str);

    af->dirty = TRUE;
    
    str_reset(&old);
    str_reset(&new_);
  }
}

static void auto_fix_return_type(check_ctx_t* ctx, const char* old_type, const char* new_type) {
  auto_fix_type_impl(ctx, new_type);
}

static void auto_fix_param_name_missing(check_ctx_t* ctx, mparam_t* param) {
  if (!ctx->enable_auto_fix) {
    return;
  }

  auto_fix_t* af = &ctx->auto_fix;
  str_t* line = darray_get(af->lines, ctx->cur_line-1);
  if (!line) {
    log_error("%s(%d) internal error, invalid cur_line \n", ctx->path, ctx->cur_line);
    return;
  }

  const char* beg = strstr(line->str, "{");;
  const char* end = strstr(beg, "}");
  if (end > beg) {
    str_t old, new_;
    str_init(&old, 128);
    str_init(&new_, 128);

    str_append_with_len(&old, beg, end-beg+1);
    str_append_more(&new_, old.str, " ", param->name, NULL);
    str_replace(line, old.str, new_.str);

    af->dirty = TRUE;
    
    str_reset(&old);
    str_reset(&new_);
  }
}

static void auto_fix_property_name_missing(check_ctx_t* ctx, mparam_t* param) {
  auto_fix_param_name_missing(ctx, param);
}

static void auto_fix_param_name(check_ctx_t* ctx, const char* new_name, const char* old_name) {
  if (!ctx->enable_auto_fix) {
    return;
  }

  auto_fix_t* af = &ctx->auto_fix;
  str_t* line = darray_get(af->lines, ctx->cur_line-1);
  if (!line) {
    log_error("%s(%d) internal error, invalid cur_line \n", ctx->path, ctx->cur_line);
    return;
  }

  // 过滤掉前面的 type , 防止干扰
  const char* s = strstr(line->str, "} ");
  // "} name" 防止被干扰
  const char* name_beg = s;
  const char* name_end = s;
  if (s) {
    s = strstr(s, old_name);
    name_end = s+strlen(old_name);
  }
  
  if (s) {
    str_t old, new_;
    str_init(&old, 128);
    str_init(&new_, 128);

    str_append_with_len(&old, name_beg, name_end-name_beg);
    const char* end = name_end - strlen(old_name);
    str_append_with_len(&new_, name_beg, end-name_beg);
    str_append_more(&new_, new_name, NULL);
    str_replace(line, old.str, new_.str);
    af->dirty = TRUE;
    
    str_reset(&old);
    str_reset(&new_);
  }
}

static void auto_fix_param_type(check_ctx_t* ctx, mparam_t* param) {
  auto_fix_type_impl(ctx, param->type);
}

static void auto_fix_ename(check_ctx_t* ctx, ca_symbol_t* sym, const char* old_name) {
  if (!ctx->enable_auto_fix) {
    return;
  }

  auto_fix_t* af = &ctx->auto_fix;
  str_t* line = darray_get(af->lines, ctx->cur_line-1);
  str_replace(line, old_name, sym->name);
  af->dirty = TRUE;
}

static void auto_fix_const_missing(check_ctx_t* ctx, ca_symbol_t* sym) {
  if (!ctx->enable_auto_fix) {
    return;
  }

  auto_fix_t* af = &ctx->auto_fix;
  str_t* line = darray_get(af->lines, af->comment_line_begin);
  str_append_more(line, "   * @const ", sym->name, g_line_end, NULL);
  af->dirty = TRUE;
}

// ename 和 enum_name 是有区别的
// ename 是指 enum 里的各个常量
// enum_name 是指 enum 的名字
static void auto_fix_ename_missing(check_ctx_t* ctx, ca_symbol_t* sym) {
  if (!ctx->enable_auto_fix) {
    return;
  }

  auto_fix_t* af = &ctx->auto_fix;
  str_t* line = darray_get(af->lines, ctx->cur_line-1);
  str_clear(line);
  str_append_more(line, "   * @const ", sym->name, g_line_end, NULL);
  af->dirty = TRUE;
}

static void auto_fix_enum_name_missing(check_ctx_t* ctx, ca_symbol_t* sym, int32_t enum_line) {
  if (!ctx->enable_auto_fix) {
    return;
  }

  auto_fix_t* af = &ctx->auto_fix;
  str_t* line = darray_get(af->lines, enum_line);
  str_clear(line);
  str_append_more(line, " * @enum ", sym->name+1, "\n", NULL);
  af->dirty = TRUE;
}

static void auto_fix_name(check_ctx_t* ctx, const char* new_name, int32_t line_number, const char* old_name) {
  if (!ctx->enable_auto_fix) {
    return;
  }

  auto_fix_t* af = &ctx->auto_fix;
  str_t* line = darray_get(af->lines, line_number);
  str_replace(line, old_name, new_name);
  af->dirty = TRUE;
}

static void auto_fix_method_name(check_ctx_t* ctx, const char* new_name, int32_t line, const char* old_name) {
  auto_fix_name(ctx, new_name, line, old_name);
}

static void auto_fix_method_name_missing(check_ctx_t* ctx, ca_symbol_t* sym, int32_t ln) {
  if (!ctx->enable_auto_fix) {
    return;
  }

  auto_fix_t* af = &ctx->auto_fix;
  str_t* line = darray_get(af->lines, ln);
  str_clear(line);
  str_append_more(line, " * @method ", sym->name, "\n", NULL);
  af->dirty = TRUE;
}

static void auto_fix_enum_name(check_ctx_t* ctx, ca_symbol_t* sym, int32_t enum_line, const char* old_name) {
  // 目前的文档 enum 名字可以不匹配， 这里不能开启 这个
  return;
  auto_fix_name(ctx, sym->name+1, enum_line, old_name);
}

static void auto_fix_enum_prefix_missing(check_ctx_t* ctx, ca_symbol_t* sym, int32_t enum_line, bool_t replace_old_line) {
  if (!ctx->enable_auto_fix) {
    return;
  }

  char* guessed_prefix_name = NULL;
  ca_symbols_t* syms = ctx->syms;
  int32_t i = 1 + sym - syms->nodes;
  for (; i < syms->size; ++i) {
    ca_symbol_t* cs = syms->nodes+i;
    if (cs->scope && strcmp(cs->scope, sym->name)) {
      break;
    }

    const char* last_underscore = NULL;
    for (const char* s = cs->name; *s; ++s) {
      if (*s == '_') {
        last_underscore = s;
      }
    }
    if (last_underscore) {
      guessed_prefix_name = tk_strndup(cs->name, 1+last_underscore-cs->name);
    }
    break;
  }

  if (guessed_prefix_name) {
    auto_fix_t* af = &ctx->auto_fix;
    str_t* line = darray_get(af->lines, enum_line);
    if (replace_old_line) {
      str_clear(line);
    }
    str_append_more(line, " * @prefix ", guessed_prefix_name, g_line_end, NULL);
    
    af->dirty = TRUE;
    TKMEM_FREE(guessed_prefix_name);
  }
}

static void auto_fix_property_name(check_ctx_t* ctx, ca_symbol_t* sym, const char* old_name) {
  auto_fix_param_name(ctx, sym->name, old_name);
}

static void auto_fix_property_type(check_ctx_t* ctx, const char* type) {
  auto_fix_type_impl(ctx, type);
}

static void auto_fix_save(check_ctx_t* ctx) {
  if (!ctx->enable_auto_fix) {
    return;
  }

  auto_fix_t* af = &ctx->auto_fix;
  if (af->dirty) {
    str_t* file_content = dump_lines(ctx);
    if (ctx->fix_hook) {
      ctx->fix_hook(ctx->path, file_content->str, file_content->size);
    } else {
      file_write(ctx->path, file_content->str, file_content->size);
    }
    
    str_destroy(file_content);
  }
}

// ---------------------------- api错误检查相关  ---------------------------------

// 去除前面及末尾的空格
// 去除中间多余的空格
static void trim_space_of_type(char* src) {
  char* start = src;
  char* dest = src;
  bool_t pre_is_space = TRUE;
  // 去除前面及中间的空格
  while (*src) {
    if (is_space(*src) && pre_is_space) {
      ++src;
      continue;
    } 
    pre_is_space = is_space(*src);

    // widget_t * --> widget_t*
    if (*src == '*') {
      if (dest > start && is_space(*(dest-1))) {
        --dest;
      }
    }
    *dest++ = *src++;
  }
  *dest = 0;

  // 去除尾部空格
  for (--dest; dest >= start && is_space(*dest); --dest) {
    *dest = 0;
  }
}
 
// 提取 @method @class @enum 后面的字符串
// 会 去除两头的空格
static char* load_name(const char** s) {
  const char* start = *s;
  const char* end = *s;
  while (is_space(*start)) {
    ++start;
  }

  for (end = start; !is_name_end(end); ++end)
    ;

  if (end > start) {
    *s = end;
    return tk_strndup(start, end-start);
  }
  return NULL;
}

static char* load_line(const char* s) {
  const char* start = s;
  const char* end = s;
  while (*start == ' ') {
    ++start;
  }

  for (end = start; *end && *end != '\r' && *end != '\n'; ++end)
    ;

  if (end > start) {
    return tk_strndup(start, end-start);
  }
  return NULL;
}

static void ignore_to_line_end(const char** s) {
  const char* p = *s;
  while(*p && *p != '\n') {
    ++p;
  }
  *s = p;
}

static char* load_type(const char** s) {
  const char* start = *s;
  const char* end = *s;
  while (is_space(*start)) {
    ++start;
  }

  // type 头
  if ('{' != *start) {
    return NULL;
  }

  ++start;
  end = start;
  while (*end != '}' && *end && *end != '\n') {
    ++end;
  }

  // type 尾
  if (*end != '}') {
    return NULL;
  }

  if (end > start) {
    char* type = tk_strndup(start, end-start);
    char* te = type + strlen(type) - 1; // 去除尾部空格
    while (*te == ' ' && te != type) {
      *te = 0;
    }
    *s = end;
    trim_space_of_type(type);
    return type;
  }

  return NULL;
}

static bool_t load_param(const char* s, char** type, char** name, bool_t* exist_desc) {
  *type = load_type(&s);
  if (*type) {
    ++s;
    char* p = *name = load_name(&s);
    if (NULL == p) {
      return FALSE;
    }
    /* to_str注释的话 直接判错， 加空格去!
    for (; *p; ++p) {
      
      // name 出现了非法字符(含中文)， 一般是因为 name 缺失， 然后把后面的 desc 读上来了， 
      if (!is_id_char(*p)) {
        if (p == *name) {  // 第一个字符就非法， 把注释读成了 name
          TKMEM_FREE(p);
          *name = NULL;
        } else {
          s -= strlen(p); // 修正 s 的值

          *p = 0; // 前面是 id  后面是中文， 可能是忘了加空格， 比如: to_str注释
        }

        break;
      }
    }*/

    // 判断下是否只有空格
    for ( ; *s && (is_space(*s) || *s == '\r' || *s == '\n'); ++s);

    if (exist_desc) {
      *exist_desc = *s ? TRUE : FALSE;
    }
  }
  
  return *type && *name;
}

// 把函数返回类型的 static inline 去掉
static void trim_func_type(char* src) {
  char* start = src;
  char* dest = src;
  for (; *src; ++src) {
    if (0 == strncmp("inline", src, 6) ||
        0 == strncmp("static", src, 6)) {
      src += 6;
    } else if (0 == strncmp("BEGIN_C_DECLS", src, 13)) {
      src += 13;
    }

    *dest++ = *src;
  }
  *dest = 0;
  trim_space_of_type(start);
}

static char* sym_type(ca_symbol_t* sym) {
  str_t name;
  str_init(&name, 256);
  str_append_more(&name, sym->name, "(", NULL);
  const char* s = strstr(sym->line_content, name.str);
  str_reset(&name);
  if (s == NULL) {
    return NULL;
  }

  char* ret = tk_strndup(sym->line_content, s-sym->line_content);
  trim_func_type(ret);
  return ret;
}

static ret_t param_destroy(mparam_t* param) {
  if (param) {
    if (param->type) TKMEM_FREE(param->type);
    if (param->name) TKMEM_FREE(param->name);
  }
  return RET_OK;
}

static const char* last_space(const char* s) {
  const char* end = s + strlen(s) - 1;
  for (; end >= s; --end) {
    if (is_space(*end)) {
      return end;
    }
  }
  return NULL;
}

static const char* last_not_id(const char* s) {
  const char* end = s + strlen(s) - 1;
  for (; end >= s; --end) {
    if (!is_id_char(*end)) {
      return end;
    }
  }
  return NULL;
}

static mparam_t* parse_param(const char* param_str) {
  mparam_t* param = TKMEM_ZALLOC(mparam_t);
  // s = "wh_t w"   --> {type:"wh_t", name:"w"}
  // s = "w" --> {type:NULL, name:"w"}
  // s = "char path[MAX_LEN + 1]" --> {type:"char*", name:"path"}
  char* s = tk_strdup(param_str);
  char* square_bracket = strstr(s, "[");
  if (square_bracket) {
    *square_bracket = 0;
  }

  char* semicomma = strstr(s, ";");
  if (semicomma) {
    *semicomma = 0;
  }

  char* colon = strstr(s, ":");
  if (colon) {
    *colon = 0;
    // 吃掉末尾的空格
    for (--colon; colon > s && is_space(*colon); --colon) {
      *colon = 0;
    }
  }
  
  const char* last = last_not_id(s);
  if (last) {
    if (last-s > 0) {
      str_t type;
      str_init(&type, last-s+20);
      str_set_with_len(&type, s, last-s+1);
      if (square_bracket) {
        str_append_char(&type, '*');
      }
      param->type = type.str;
      trim_space_of_type(param->type);
    }
      
    param->name = tk_strdup(last+1);
  } else {
    param->name = tk_strdup(s);
  }
  TKMEM_FREE(s);
  return param;
}

// (wh_t w, wh_t h) -->  [{wh_t, w}, {wh_t, h}]
static void sym_params(const char* param_str, darray_t* o) {
  char* src = tk_strdup(param_str);
  uint32_t len = strlen(src);
  if (len < 2) {
    return;
  }

  // 去除 )
  src[len-1] = 0;
  for (int32_t i = len-2; i > 0 && is_space(src[i]); --i) {
    src[i] = 0; // 过滤掉空格
  }

  tokenizer_t tokenizer;
  tokenizer_t* t = NULL;
  
  // src+1 去除 (
  t = tokenizer_init(&tokenizer, src+1, 0xffffff, ",");
  while (tokenizer_has_more(t)) {
    const char* s = tokenizer_next_str(t);
    darray_push(o, parse_param(s));
  }

  if (o->size == 1) {
    mparam_t* param = darray_get(o, 0);
    if (0 == strcmp(param->name, "void")) {
      darray_clear(o);
    }
  }

  TKMEM_FREE(src);
  tokenizer_deinit(t);
}

static bool_t str_same(const char* s1, const char* s2) {
  s1 = s1 ? s1: "";
  s2 = s2 ? s2: "";
  return 0 == strcmp(s1, s2);
}

// 是否可以自动修正 param的name
static bool_t can_fix_param_name(auto_fix_t* af, darray_t* params, mparam_t* param, const char* type,const char* name) {
  // 参数名字的自动修正是很危险的， 所以满足下面的条件后，才进行自动修正

  // 参数个数必须一样多
  if (params->size != af->param_n) {
    return FALSE;
  } 

  // 类型名称必须相同
  if (!str_same(param->type, type)) {
    return FALSE;
  }

  // 万一是顺序错了呢， 所以 如果 type name 配对 在 params中存在过， 也不修正
  mparam_t** arr = (mparam_t**)params->elms;
  for (uint32_t i = 0; i < params->size; ++i) {
    mparam_t* p = arr[i];
    if (str_same(p->type, type) && str_same(p->name, name)) {
      return FALSE;
    }
  }

  return TRUE;
}

static bool_t check_param(check_ctx_t* ctx, ca_symbol_t* sym, darray_t* params, int32_t param_i,
                          const char* type, const char* name) {
  
  if (param_i >= params->size) {
    app_err("%s(%d): error: too much @param\n", sym->file, ctx->cur_line);
    return FALSE;
  }

  mparam_t* param = darray_get(params, param_i);
  if (!str_same(param->name, name)) {
    auto_fix_t* af = &ctx->auto_fix;

    app_err("%s(%d): error: @param_%d's name invalid: %s -- %s\n", 
      sym->file, ctx->cur_line, param_i, name, param->name);

    // 参数个数完全对的， 却类型也对的上, 再进行 名字修正
    if (can_fix_param_name(af, params, param, type, name)) {
      auto_fix_param_name(ctx, param->name, name);
    }
    return FALSE;
  }

  if (!str_same(param->type, type)) {
    app_err("%s(%d): error: @param_%d's type invalid: %s -- %s\n", 
      sym->file, ctx->cur_line, param_i, type, param->type);

    // 名字正确, 类型错误, 可以尝试自动修正
    auto_fix_param_type(ctx, param);
    return FALSE;
  }

  return TRUE;
}

typedef enum _doc_type_t {
  DOC_ANNOTATION,
  DOC_CLASS,
  DOC_METHOD,
  DOC_PARAM,
  DOC_ENUM,
  DOC_RETURN,
  DOC_PREFIX,
  DOC_CONST,
  DOC_PROPERTY,
  DOC_UNKNOWN,
} doc_type_t;

static const char* handle_one_line(const char* line, check_ctx_t* ctx, doc_type_t* kind) {
  const char* s = line;
  *kind = DOC_UNKNOWN;
  if (strlen(s) > 3 && s[1] == '*' && s[3] == '@') {
    s += 3;
  } else if (strlen(s) > 5 && s[3] == '*' && s[5] == '@') {
    s += 5;
  } else {
    return s;
  }

#define SAME(s1, s2) 0 == strncmp(s1, s2, strlen(s2))
  
  if (SAME(s, key_method)) {
    *kind = DOC_METHOD;
  } else if (SAME(s, key_annotation)) {
    *kind = DOC_ANNOTATION;
  } else if (SAME(s, key_return)) {
    *kind = DOC_RETURN;
  } else if (SAME(s, key_param)) {
    *kind = DOC_PARAM;
  } else if (SAME(s, key_enum)) {
    *kind = DOC_ENUM;
  } else if (SAME(s, key_prefix)) {
    *kind = DOC_PREFIX;
  } else if (SAME(s, key_const)) {
    *kind = DOC_CONST;
  } else if (SAME(s, key_class)) {
    *kind = DOC_CLASS;
  } else if (SAME(s, key_property)) {
    *kind = DOC_PROPERTY;
  } 
#undef SAME

  return s;
}

static int32_t count_param(check_ctx_t* ctx) {
  auto_fix_t* af = &ctx->auto_fix;
  int32_t param_n = 0;
  for (int32_t i = af->comment_line_begin; i < af->comment_line_end; ++i) {
    doc_type_t type;
    str_t* line = darray_get(af->lines, i);
    handle_one_line(line->str, ctx, &type);
    if (type == DOC_PARAM) {
      ++param_n;
    }
  }
  return af->param_n = param_n;
}

static void check_comment_between_param(check_ctx_t* ctx) {
  auto_fix_t* af = &ctx->auto_fix;
  int32_t line_first_param = -1;
  int32_t line_last_param = -1;
  bool_t got_method = FALSE;
  for (int32_t i = af->comment_line_begin; i < af->comment_line_end; ++i) {
    str_t* line = darray_get(af->lines, i);
    doc_type_t type;
    handle_one_line(line->str, ctx, &type);
    if (type == DOC_PARAM) {
      if (line_first_param == -1) { 
        line_first_param = i;
      }
      line_last_param = i;
    } else if (type == DOC_METHOD) {
      got_method = TRUE;
    }
  }

  if (!got_method || line_last_param == -1) {
    return;
  }

  // param之间的注释 报错
  for (int32_t i = line_first_param; i < line_last_param; ++i) {
    str_t* line = darray_get(af->lines, i);
    doc_type_t type;
    handle_one_line(line->str, ctx, &type);
    if (type != DOC_PARAM) {
      app_err("%s(%d): error: nothing can be written between @param\n",
              ctx->path, i+1);
    } 
  }
}

static bool_t is_valid_anno(const char* anno) {
  return 
    0 == strcmp(anno, "\"macro\"")          ||
    0 == strcmp(anno, "\"constructor\"")    ||
    0 == strcmp(anno, "\"gc\"")             ||
    0 == strcmp(anno, "\"deconstructor\"")  ||
    0 == strcmp(anno, "\"design\"")         ||
    0 == strcmp(anno, "\"widget\"")         ||
    0 == strcmp(anno, "\"window\"")         ||
    0 == strcmp(anno, "\"scriptable\"")     ||
    0 == strcmp(anno, "\"scriptable:custom\"")     ||
    0 == strcmp(anno, "\"cast\"")           ||
    0 == strcmp(anno, "\"fake\"")           ||
    0 == strcmp(anno, "\"static\"")         ||
    0 == strcmp(anno, "\"readable\"")       ||
    0 == strcmp(anno, "\"writable\"")       ||
    0 == strcmp(anno, "\"set_prop\"")       ||
    0 == strcmp(anno, "\"get_prop\"")       ||
    0 == strcmp(anno, "\"persitent\"")      ||
    0 == strcmp(anno, "\"private\"")        ||
    0 == strcmp(anno, "\"string\"")         ||
    0 == strcmp(anno, "\"global\"");
}

static bool_t check_anno(check_ctx_t* ctx, str_t* str) {
  str_trim(str, " ");
  if (!is_valid_anno(str->str)) {
    app_err("%s(%d): error: unknown annotation %s\n", ctx->path, ctx->cur_line, str->str);
    return FALSE;
  }
  return TRUE;
}

// TODO： class method 等用各自的规则判断 annotation
static bool_t check_annotation(check_ctx_t* ctx, doc_type_t type, const char* rest) {
  char* annos = load_line(rest + strlen(key_annotation) + 1);
  bool_t ret = TRUE;
  if (!annos) {
    return ret;
  }
  // 去除 []
  uint32_t len = strlen(annos);
  if (len < 2) {
    TKMEM_FREE(annos);
    return TRUE;
  }
  annos[len - 1] = 0;

  str_t str;
  str_init(&str, 32);

  for (const char* s = annos+1; *s; ++s) {
    if (*s == ',') {
      if (!check_anno(ctx, &str)) {
        ret = FALSE;
      }
      str_clear(&str);
    } else {
      str_append_char(&str, *s);
    }
  }
  if (str.size) {
    if (!check_anno(ctx, &str)) {
      ret = FALSE;
    }
  }
  str_reset(&str);

  TKMEM_FREE(annos);
  return ret;
}

bool_t check_func(check_ctx_t* ctx, ca_symbol_t* sym) {
  bool_t ret = TRUE;

  if (strstr(sym->line_content, "#define")) {
    // TODO: 宏函数的检测
    return TRUE;
  }

  bool_t got_method = FALSE;
  bool_t got_anno = FALSE;
  bool_t got_param = FALSE;
  bool_t got_ret = FALSE;

  auto_fix_t* af = &ctx->auto_fix;
  int param_i = 0;
  darray_t* params = darray_create(8, (tk_destroy_t)param_destroy, NULL);
  sym_params(sym->param, params);

  count_param(ctx);
  check_comment_between_param(ctx);

  for (int32_t i = af->comment_line_begin; i < af->comment_line_end; ++i) {
    doc_type_t type;
    ctx->cur_line = i+1;
    str_t* line = darray_get(af->lines, i);
    const char* rest = handle_one_line(line->str, ctx, &type);

    if (type == DOC_METHOD) {
      rest += strlen(key_method) + 1;
      char* method_name = load_name(&rest);

      got_method = TRUE;
      if (method_name) {
        if (strcmp(method_name, sym->name)) {
          app_err("%s(%d): error: @method name invalid, expect(%s) -- actual(%s)\n",
                  sym->file, ctx->cur_line, sym->name, method_name);
          ret = FALSE;

          auto_fix_method_name(ctx, sym->name, i, method_name);
        }
        TKMEM_FREE(method_name);
      } else {
        app_err("%s(%d): error: @method name missing %s\n", sym->file, ctx->cur_line, sym->name);
        ret = FALSE;

        auto_fix_method_name_missing(ctx, sym, i);
      }
    } else if (type == DOC_ANNOTATION) {
      got_anno = TRUE;
      check_annotation(ctx, DOC_METHOD, rest);
    } else if (type == DOC_PARAM) {
      char* type = NULL;
      char* name = NULL;
      bool_t exist_desc = FALSE;
      got_param = TRUE;
      if (!load_param(rest + strlen(key_param) + 1, &type, &name, &exist_desc)) {
        if (type == NULL) {
          app_err("%s(%d): error: @parem(%d) missing type\n",
                  sym->file, ctx->cur_line, param_i);
        } else if (name == NULL) {
          app_err("%s(%d): error: @parem(%d) missing name\n",
                  sym->file, ctx->cur_line, param_i);
          mparam_t* param = darray_get(params, param_i);
          if (str_same(param->type, type)) {
            auto_fix_param_name_missing(ctx, param);
          }
        }
      
        ret = FALSE;
      } else {
        if (!exist_desc) {
          app_err("%s(%d): error: @parem_%d %s missing comment\n",
                  sym->file, ctx->cur_line, param_i, name?name:"");
        }
        check_param(ctx, sym, params, param_i, type, name);
      }

      if (type) {
        TKMEM_FREE(type);
      }
      if (name) { 
        TKMEM_FREE(name);
      }

      ++param_i;
    } else if (type == DOC_RETURN) {
      got_ret = TRUE;
      rest += strlen(key_return) + 1;
      char* type = load_type(&rest);
      char* exp_type = sym_type(sym);

      if (type == NULL) {
        app_err("%s(%d): error: @return type missing\n", sym->file, ctx->cur_line);
        ret = FALSE;
      } else if (exp_type == NULL) {
        app_err("%s(%d): error: internal error: return info missing\n",
                sym->file, ctx->cur_line);
        // 代码辅助内部错误， 不是 api doc 错误
      } else if (strcmp(type, exp_type)) {
        app_err("%s(%d): error: @return type invalid %s -- %s\n",
                sym->file, ctx->cur_line, type, exp_type);
        auto_fix_return_type(ctx, type, exp_type);
        ret = FALSE;
      }

      if (type) {
        TKMEM_FREE(type);
      }
      if (exp_type) {
        TKMEM_FREE(exp_type);
      }
    }
  }
         
  if (param_i < params->size) {
    mparam_t* p = darray_tail(params);
    bool_t is_variable = (p->type && 0 == strcmp("...", p->type));

    if (is_variable && param_i+1 == params->size) {
      // 可变参数, 可变部分不计入 @param
    } else if (got_method) {
      uint32_t param_n = is_variable ? params->size -1 : params->size;
      str_t info;
      str_init(&info, 4096);
      str_format(&info, 4096, "%s(%d): error: not enough @param (%d / %d)\n",
                 sym->file, sym->line, param_i, params->size);
      str_append(&info, "suggest:\n");
      for (int32_t i = param_i; i < param_n; ++i) {
        p = darray_get(params, i);
        str_append_format(&info, 1024, " * @param {%s} %s \n", p->type, p->name);
      }
      app_err("%s", info.str);
      str_reset(&info);
    }
  }
  darray_destroy(params);

  if (!got_method) {
    // 没有 @method 
    if (got_ret || got_param || got_anno) {
      app_err("%s(%d): error: @method missing\n", sym->file, ctx->cur_line);
      ret = FALSE;
    }
  }
  return ret;
}

static bool_t check_enum_impl(check_ctx_t* ctx, ca_symbol_t* sym) {
  // typedef enum _foo_t {} foo_t;
  bool_t ret = TRUE;
  bool_t got_enum = FALSE;
  bool_t got_anno = FALSE;
  bool_t got_prefix = FALSE;
  int32_t enum_line = 0;
  auto_fix_t* af = &ctx->auto_fix;
  ctx->last_enum = sym;
  for (int32_t i = af->comment_line_begin; i < af->comment_line_end; ++i) {
    doc_type_t type;
    str_t* line = darray_get(af->lines, i);
    const char* rest = handle_one_line(line->str, ctx, &type);
    ctx->cur_line = i + 1;
    if (type == DOC_ENUM) {
      rest += strlen(key_enum) + 1;
      char* enum_name = load_name(&rest);
      got_enum = TRUE;
      enum_line = i;

      if (sym->name[0] != '_') {
        app_err("%s(%d): error: enum must begin with _\n", sym->file, sym->line);
        ret = FALSE;
      } else if (NULL == enum_name) {
        app_err("%s(%d): error: @enum name missing\n", sym->file, enum_line + 1);
        auto_fix_enum_name_missing(ctx, sym, enum_line);
        ret = FALSE;
      } else if (strcmp(sym->name + 1, enum_name)) {
        app_err("%s(%d): error: @enum name invalid %s -- %s\n", sym->file, enum_line + 1, enum_name,
                sym->name + 1);
        auto_fix_enum_name(ctx, sym, enum_line, enum_name);
        ret = FALSE;
      }

      if (enum_name) {
        TKMEM_FREE(enum_name);
      }
    } else if (type == DOC_ANNOTATION) {
      got_anno = TRUE;
      check_annotation(ctx, DOC_ENUM, rest);
    } else if (type == DOC_PREFIX) {
      got_prefix = TRUE;
      rest += strlen(key_prefix) + 1;
      if (ctx->prefix) {
        TKMEM_FREE(ctx->prefix);
      }
      ctx->prefix = load_name(&rest);
      if (NULL == ctx->prefix) {
        app_err("%s(%d): error: @prefix name missing\n", sym->file, ctx->cur_line);
        ret = FALSE;

        auto_fix_enum_prefix_missing(ctx, sym, i, TRUE);
      }
    }
  }

  if (got_enum) {
    if (!got_prefix) {
      app_warn("%s(%d): warning: @prefix missing\n", sym->file, enum_line + 1);
      ret = FALSE;

      if (ctx->prefix) {
        TKMEM_FREE(ctx->prefix);
      }

      auto_fix_enum_prefix_missing(ctx, sym, enum_line, FALSE);
    }
  }

  if (!got_enum && (got_anno || got_prefix)) {
    app_err("%s(%d): error: missing @enum\n", sym->file, sym->line);
    ret = FALSE;
  }

  return ret;
}

static bool_t check_ename(check_ctx_t* ctx, ca_symbol_t* sym) {
  auto_fix_t* af = &ctx->auto_fix;
  bool_t ret = TRUE;
  bool_t got_const = FALSE;
  for (int32_t i = af->comment_line_begin; i < af->comment_line_end; ++i) {
    doc_type_t type;
    str_t* line = darray_get(af->lines, i);
    const char* rest = handle_one_line(line->str, ctx, &type);
    ctx->cur_line = i + 1;

    if (type == DOC_CONST) {
      got_const = TRUE;

      rest += strlen(key_const) + 1;
      char* ename = load_name(&rest);

      if (ename) {
        if (strcmp(ename, sym->name)) {
          app_err("%s(%d): error: @const name invalid %s -- %s\n", sym->file, ctx->cur_line, ename, sym->name);
          ret = FALSE;
          auto_fix_ename(ctx, sym, ename);
        } else if (ctx->prefix && !strstr(ename, ctx->prefix)) {
          app_err("%s(%d): error: @const %s check prefix failed\n", sym->file, ctx->cur_line, ename);
          ret = FALSE;
        }
        
        TKMEM_FREE(ename);
      } else {
        app_err("%s(%d): error: @const name missing\n", sym->file, ctx->cur_line);
        ret = FALSE;
        auto_fix_ename_missing(ctx, sym);
      }
    }
  }

  if (!got_const 
      && ctx->last_enum 
      && str_same(ctx->last_enum->name, sym->scope)) {
    str_t info;
    str_init(&info, 4096);
    str_format(&info, 4096, "%s(%d): @const %s missing\n", sym->file, sym->line, sym->name);
    str_append(&info, "suggest:\n");
    str_append_more(&info, "   * @const ", sym->name, " \n", NULL);
    app_err("%s", info.str);
    str_reset(&info);
    auto_fix_const_missing(ctx, sym);
    ret = FALSE;
  }

  return ret;
}

static bool_t check_enum(check_ctx_t* ctx, ca_symbol_t* sym) {
  bool_t is_ename = sym->scope && *sym->scope;
  
  if (is_ename) {
    return check_ename(ctx, sym);
  } else { 
    return check_enum_impl(ctx, sym);
  }
}

static bool_t check_property(check_ctx_t* ctx, ca_symbol_t* sym, const char* type, const char* name) {
  if (!str_same(sym->name, name)) {
    app_err("%s(%d): error: @property's name invalid: %s -- %s\n", 
      sym->file, ctx->cur_line, name, sym->name);

    auto_fix_property_name(ctx, sym, name);
    return FALSE;
  }

  mparam_t* param = parse_param(sym->line_content);
  if (!str_same(param->type, type)) {
    bool_t report_error = TRUE;
    const char* struct_ = "struct _";
    const char* union_ = "union _";

    // uint32_t alt : 1; 这样的话， 类型可以是 bool_t
    if (strstr(sym->line_content, ":")) {
      if (str_same(type, "bool_t")) {
        report_error = FALSE;
      }
    } else if (strstr(param->type, struct_)) {
      // struct _idl_tree_node_t* -- > _idl_tree_node_t*
      if (str_same(param->type+strlen(struct_), type)) {
        report_error = FALSE;
      }
    } else if (strstr(param->type, union_)) {
      // union _idl_tree_node_t* -- > _idl_tree_node_t*
      if (str_same(param->type+strlen(union_), type)) {
        report_error = FALSE;
      }
    }

    if (report_error) {
      app_err("%s(%d): error: @property's type invalid: %s -- %s\n", 
        sym->file, ctx->cur_line, type, param->type);

      auto_fix_property_type(ctx, param->type);
      return FALSE;
    }
  }
  TKMEM_FREE(param);

  return TRUE;
}

static bool_t check_class_member(check_ctx_t* ctx, ca_symbol_t* sym) {
  bool_t ret = TRUE;
  bool_t got_anno = FALSE;
  bool_t got_property = FALSE;
  auto_fix_t* af = &ctx->auto_fix;
  for (int32_t i = af->comment_line_begin; i < af->comment_line_end; ++i) {
    doc_type_t type;
    str_t* line = darray_get(af->lines, i);
    const char* rest = handle_one_line(line->str, ctx, &type);
    ctx->cur_line = i + 1;

    if (type == DOC_PROPERTY) {
      char* type = NULL;
      char* name = NULL;
      got_property = TRUE;
      if (!load_param(rest + strlen(key_property) + 1, &type, &name, NULL)) {
        if (type == NULL) {
          app_err("%s(%d):error: @property missing type\n", sym->file, ctx->cur_line);
        } else if (name == NULL) {
          app_err("%s(%d):error: @property missing name\n", sym->file, ctx->cur_line);
          if (sym->line_content) {
            mparam_t* param = parse_param(sym->line_content);
            if (str_same(param->type, type)) {
              auto_fix_property_name_missing(ctx, param);
            }
          }
        }
      
        ret = FALSE;
      } else {
        check_property(ctx, sym, type, name);
      }

      if (type) {
        TKMEM_FREE(type);
      }
      if (name) {
        TKMEM_FREE(name);
      }
    } else if (type == DOC_ANNOTATION) {
      got_anno = TRUE;
      check_annotation(ctx, DOC_PROPERTY, rest);
    } 
  }

  if (got_anno && !got_property) {
    app_err("%s(%d): error: @property missing\n", sym->file, sym->line);
    ret = FALSE;
  }

  return ret;
}

static bool_t check_class(check_ctx_t* ctx, ca_symbol_t* sym) {
  bool_t ret = TRUE;
  bool_t got_anno = FALSE;
  bool_t got_class = FALSE;
  auto_fix_t* af = &ctx->auto_fix;
  for (int32_t i = af->comment_line_begin; i < af->comment_line_end; ++i) {
    doc_type_t type;
    str_t* line = darray_get(af->lines, i);
    const char* rest = handle_one_line(line->str, ctx, &type);
    ctx->cur_line = i + 1;
    if (type == DOC_CLASS) {
      rest += strlen(key_class) + 1;
      char* class_name = load_name(&rest);
      got_class = TRUE;

      if (class_name) {
        /* 关闭这个校验 -- 李先静 觉得这个可以不校验
        const char* expect_name = sym->name+1;
        if (sym->typeref) {
          expect_name = sym->name;
        }
        if (strcmp(class_name, expect_name)) {
          app_err("%s(%d): error: @class name invalid, expect(%s) -- actual(%s)\n",
                  sym->file, ctx->cur_line, expect_name, class_name);
          ret = FALSE;
        }
        */
        TKMEM_FREE(class_name);
      } else {
        app_err("%s(%d): error: @class name missing\n", sym->file, ctx->cur_line);
        ret = FALSE;
      }
    } else if (type == DOC_ANNOTATION) {
      got_anno = TRUE;
      check_annotation(ctx, DOC_CLASS, rest);
    } 
  }

  if (got_anno && !got_class) {
    app_err("%s(%d): error: @class missing\n", sym->file, sym->line);
    ret = FALSE;
  }
  return ret;
}

bool_t is_empty_line(const char* s) {
  for(; *s; ++s) {
    if (is_space(*s) || *s == '\r' || *s == '\n') {
      
    } else {
      return FALSE;
    }
  }
  return TRUE;
}

// 当遇到 #define CODE_EDIT_PROP_LANG "lang" 且 prefix 为空， 或不匹配时，
// 往前面行找找， 看看 是不是重新定义了 @prefix
static void try_update_prefix(check_ctx_t* ctx, ca_symbol_t* sym) {
  int32_t enum_begin = 0, enum_end = 0;
  auto_fix_t* af = &ctx->auto_fix;
  bool_t in_comment = FALSE;
  int32_t i = 0;
  for (i = af->comment_line_begin - 1; i >= 0; --i) {
    str_t* line = darray_get(af->lines, i);

    if (strstr(line->str, "/**")) {
      enum_begin = i;
      break;
    } else if (strstr(line->str, "*/")) {
      enum_end = i+1; // 区间惯例 [begin, end) , end 要大一格
      in_comment = TRUE;
    }

    if (in_comment || is_empty_line(line->str)) {
      continue;
    } else {
      return;
    }
  }

  bool_t got_enum = FALSE;
  bool_t got_prefix = FALSE;
  int32_t enum_line = 0;
  for (i = enum_begin; i < enum_end; ++i) {
    str_t* line = darray_get(af->lines, i);
    doc_type_t type;
    const char* rest = handle_one_line(line->str, ctx, &type);
    ctx->cur_line = i+1;

    if (type == DOC_ENUM) {
      got_enum = TRUE;
      enum_line = i;
    } else if (type == DOC_PREFIX) {
      got_prefix = TRUE;
      rest += strlen(key_prefix) + 1;
      if (ctx->prefix) {
        TKMEM_FREE(ctx->prefix);
      }
      ctx->prefix = load_name(&rest);
      if (NULL == ctx->prefix) {
        app_err("%s(%d): error: @prefix name missing\n", sym->file, ctx->cur_line);
        
        auto_fix_enum_prefix_missing(ctx, sym, i, TRUE);
      }
    } else if (type == DOC_ANNOTATION) {
      check_annotation(ctx, DOC_ENUM, rest);
    } 
  }
  if (got_enum && !got_prefix) {
    app_warn("%s(%d): warning: @prefix missing\n", sym->file, enum_line + 1);
    auto_fix_enum_prefix_missing(ctx, sym, enum_line, FALSE);
  }
}

/**

 * @const CODE_EDIT_PROP_LANG

#define CODE_EDIT_PROP_LANG "lang"
 */
static bool_t check_define(check_ctx_t* ctx, ca_symbol_t* sym) {
  try_update_prefix(ctx, sym);

  bool_t ret = TRUE;
  auto_fix_t* af = &ctx->auto_fix;
  for (int32_t i = af->comment_line_begin; i < af->comment_line_end; ++i) {
    doc_type_t type;
    str_t* line = darray_get(af->lines, i);
    const char* rest = handle_one_line(line->str, ctx, &type);
    ctx->cur_line = i + 1;
    if (type == DOC_CONST) {
      rest += strlen(key_const) + 1;
      char* ename = load_name(&rest);

      if (ename) {
        if (strcmp(ename, sym->name)) {
          app_err("%s(%d): error: @const name invalid %s -- %s\n", sym->file, ctx->cur_line, ename, sym->name);
          ret = FALSE;
          auto_fix_ename(ctx, sym, ename);
        } else if (ctx->prefix && !strstr(ename, ctx->prefix)) {
          app_err("%s(%d): error: @const %s check prefix failed\n", sym->file, ctx->cur_line, ename);
          ret = FALSE;
        }
        
        TKMEM_FREE(ename);
      } else {
        app_err("%s(%d): error: @const name missing\n", sym->file, ctx->cur_line);
        ret = FALSE;
        auto_fix_ename_missing(ctx, sym);
      }
    }
  }

  return ret;
}

static bool_t check_api_doc_impl(check_ctx_t* ctx) {
  bool_t ret = TRUE;
  ca_symbols_t* syms = ctx->syms;
  for (uint32_t i = 0; i < syms->size; ++i) {
    ca_symbol_t* sym = syms->nodes+i;
    // 有注释的才需要验证
    if (sym->comment == NULL ||
        sym->comment[0] == 0 ||
        NULL == strstr(sym->comment, "/**")) {
      continue;
    }

    if (sym->type == CA_FUNC_DEF || sym->type == CA_FUNC_PROTOTYPE) {
      auto_fix_init_sym(ctx, sym);
      if (!check_func(ctx, sym)) {
        ret = FALSE;
      }
    } else if (sym->type == CA_ENUM) {
      auto_fix_init_sym(ctx, sym);
      if (!check_enum(ctx, sym)) {
        ret = FALSE;
      }
    } else if (sym->type == CA_DEF) {
      auto_fix_init_sym(ctx, sym);
      if (!check_define(ctx, sym)) {
        ret = FALSE;
      }
    } else if (sym->type == CA_TYPE) {
      auto_fix_init_sym(ctx, sym);
      if (!sym->scope && !check_class(ctx, sym)) {
        ret = FALSE;
      }
    } else if (sym->type == CA_GENERAL && sym->scope) {
      auto_fix_init_sym(ctx, sym);
      if (!check_class_member(ctx, sym)) {
        ret = FALSE;
      }
    }
  }
  return ret;
}

bool_t check_api_doc2(code_assist_t* ca, const char* filename, log_hook_t hook, void* log_ctx, 
                      bool_t auto_fix, auto_fix_hook_t fix_hook) {
  code_assist_visit_file2(ca, filename);
  ca_symbols_t* syms = code_assist_symbols_from_file(ca, filename, FALSE);
  if (NULL == syms) {
    return FALSE;
  }

  check_ctx_t ctx = { .ca = ca, .path = filename, .log_hook = hook, .log_ctx = log_ctx, 
    .enable_auto_fix = auto_fix, .fix_hook = fix_hook };
  auto_fix_init(&ctx);
  ctx.syms = syms;
  bool_t ret = check_api_doc_impl(&ctx);
  syms->destroy(syms);

  auto_fix_save(&ctx);
  auto_fix_finit(&ctx);
  if (ctx.prefix) {
    TKMEM_FREE(ctx.prefix);
  }
  return ret;
}

bool_t check_api_doc(code_assist_t* ca, const char* filename, log_hook_t hook, void* log_ctx, bool_t auto_fix) {
  return check_api_doc2(ca, filename, hook, log_ctx, auto_fix, NULL);
}
