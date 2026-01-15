/**
 * File:   yaml.c
 * Author: AWTK Develop Team
 * Brief:  yaml 
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2025-04-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "conf_io/conf_yaml.h"
#include "tkc/data_reader_mem.h"
#include "tkc/data_writer_wbuffer.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"

/* YAML 标准使用2个空格作为缩进单位 */
#define YAML_SPACES_PER_LEVEL 2
/* root节点的leading_spaces为0，其他节点需要加上这个偏移量 */
#define YAML_LEADING_SPACE_OFFSET 2

#define YAML_SEP_CHAR ':'
#define YAML_COMMENT_CHAR '#'
#define YAML_LIST_START_CHAR '-'
#define YAML_SAVE_BUFFER_INIT_SIZE 1024

typedef struct _yaml_parser_t {
  str_t str;
  const char* data;
  const char* cursor;

  conf_doc_t* doc;
  conf_node_t* current_node;
} yaml_parser_t;

static ret_t yaml_parser_deinit(yaml_parser_t* parser);
static const char* yaml_parser_parse_name(yaml_parser_t* parser);
static ret_t yaml_parser_parse_flow_sequence(yaml_parser_t* parser, conf_node_t* node);
static ret_t yaml_parser_parse_flow_mapping(yaml_parser_t* parser, conf_node_t* node);
static ret_t yaml_parser_parse_flow_value(yaml_parser_t* parser, conf_node_t* node);
static const char* yaml_parser_parse_multiline_string(yaml_parser_t* parser, char block_char);

static ret_t yaml_parser_init(yaml_parser_t* parser, const char* data) {
  conf_doc_t* doc = NULL;
  return_value_if_fail(parser != NULL && data != NULL, RET_BAD_PARAMS);

  memset(parser, 0x00, sizeof(*parser));
  doc = conf_doc_create(100);
  return_value_if_fail(doc != NULL, RET_OOM);

  doc->max_deep_level = 20;
  doc->root = conf_doc_create_node(doc, CONF_NODE_ROOT_NAME);
  if (doc->root == NULL) {
    TKMEM_FREE(doc);
    return RET_OOM;
  }

  parser->doc = doc;
  goto_error_if_fail(str_init(&parser->str, 128) != NULL);

  parser->data = data;
  parser->cursor = data;
  parser->current_node = doc->root;

  return RET_OK;
error:
  yaml_parser_deinit(parser);

  return RET_FAIL;
}

static ret_t yaml_parser_deinit(yaml_parser_t* parser) {
  return_value_if_fail(parser != NULL, RET_BAD_PARAMS);

  if (parser->doc != NULL) {
    conf_doc_destroy(parser->doc);
    parser->doc = NULL;
  }

  str_reset(&parser->str);
  memset(parser, 0x00, sizeof(*parser));

  return RET_OK;
}

static uint32_t yaml_parser_skip_leading_space(yaml_parser_t* parser) {
  uint32_t leading_spaces = 0;
  const char* p = parser->cursor;

  while (*p) {
    if (*p == ' ') {
      leading_spaces++;
    } else if (*p == '\t') {
      leading_spaces += YAML_SPACES_PER_LEVEL;
    } else if (*p == '\r' || *p == '\n') {
      leading_spaces = 0;
    } else {
      break;
    }
    p++;
  }

  parser->cursor = p;

  return leading_spaces;
}

static ret_t yaml_parser_skip_to_line_end(yaml_parser_t* parser) {
  parser->cursor = tk_skip_to_chars(parser->cursor, "\r\n");
  parser->cursor = tk_skip_chars(parser->cursor, "\r\n");

  return RET_OK;
}

static void yaml_parser_skip_spaces(const char** p) {
  while (**p == ' ' || **p == '\t') {
    (*p)++;
  }
}

static const char* yaml_parser_parse_quoted_string_internal(yaml_parser_t* parser, char quote_char, bool_t skip_trailing) {
  char c = '\0';
  uint32_t n = 0;
  str_t* s = &(parser->str);
  const char* p = parser->cursor;
  bool_t escape = FALSE;
  bool_t closed = FALSE;
  bool_t is_single_quote = (quote_char == '\'');

  str_clear(s);
  p++; /* 跳过开始的引号 */
  
  while (*p != '\0') {
    if (is_single_quote) {
      /* 单引号字符串：不进行转义处理，只有 '' 表示一个单引号 */
      if (*p == '\'' && p[1] == '\'') {
        /* 两个连续的单引号表示一个单引号字符 */
        if (str_append_char(s, '\'') != RET_OK) {
          log_warn("yaml parse quoted string: out of memory\n");
          return NULL;
        }
        p += 2; /* 跳过两个单引号 */
      } else if (*p == '\'') {
        /* 单个单引号表示字符串结束 */
        p++; /* 跳过结束的引号 */
        closed = TRUE;
        break;
      } else if (*p == '\r' || *p == '\n') {
        /* 单引号字符串中遇到未转义的换行，这是错误，但为了容错，我们停止解析 */
        log_warn("yaml quoted string contains unescaped newline\n");
        /* 跳过换行符，避免后续内容被误认为是键名 */
        if (*p == '\r' && p[1] == '\n') {
          p += 2;
        } else {
          p++;
        }
        parser->cursor = p;
        break;
      } else {
        /* 普通字符，包括反斜杠，都按字面值处理 */
        c = *p++;
        if (str_append_char(s, c) != RET_OK) {
          log_warn("yaml parse quoted string: out of memory\n");
          return NULL;
        }
      }
    } else {
      /* 双引号字符串：支持转义序列 */
      if (escape) {
        if (*p == quote_char) {
          c = quote_char;
          p++;
        } else if (*p == '\\') {
          c = '\\';
          p++;
        } else {
          c = str_unescape_char(p, &n);
          p += n;
        }
        escape = FALSE;
        if (str_append_char(s, c) != RET_OK) {
          log_warn("yaml parse quoted string: out of memory\n");
          return NULL;
        }
      } else if (*p == '\\') {
        escape = TRUE;
        p++;
        continue;
      } else if (*p == quote_char) {
        p++; /* 跳过结束的引号 */
        closed = TRUE;
        break;
      } else if (*p == '\r' || *p == '\n') {
        /* 引号字符串中遇到未转义的换行，这是错误，但为了容错，我们停止解析 */
        log_warn("yaml quoted string contains unescaped newline\n");
        /* 跳过换行符，避免后续内容被误认为是键名 */
        if (*p == '\r' && p[1] == '\n') {
          p += 2;
        } else {
          p++;
        }
        parser->cursor = p;
        break;
      } else {
        c = *p++;
        if (str_append_char(s, c) != RET_OK) {
          log_warn("yaml parse quoted string: out of memory\n");
          return NULL;
        }
      }
    }
  }

  parser->cursor = p;
  
  /* 检查引号字符串是否正常闭合 */
  if (!closed && *p == '\0') {
    log_warn("yaml quoted string not closed before end of input\n");
  }
  
  if (skip_trailing) {
    /* 跳过行尾的空白和换行（用于值解析） */
    yaml_parser_skip_spaces(&parser->cursor);
    /* 跳过到行尾（包括注释） */
    yaml_parser_skip_to_line_end(parser);
  }
  
  return s->str;
}

static const char* yaml_parser_parse_quoted_string(yaml_parser_t* parser, char quote_char) {
  return yaml_parser_parse_quoted_string_internal(parser, quote_char, TRUE);
}

static ret_t yaml_parse_bool_value(const char* value, bool_t* result) {
  if (value == NULL || *value == '\0') {
    return RET_NOT_FOUND;
  }

  /* 转换为小写进行比较（不修改原字符串） */
  char c = *value;
  if (c == 't' || c == 'T') {
    /* true, True, TRUE */
    if (tk_str_ieq(value, "true")) {
      *result = TRUE;
      return RET_OK;
    }
  } else if (c == 'y' || c == 'Y') {
    /* yes, Yes, YES */
    if (tk_str_ieq(value, "yes")) {
      *result = TRUE;
      return RET_OK;
    }
  } else if (c == 'o' || c == 'O') {
    /* on, On, ON */
    if (tk_str_ieq(value, "on")) {
      *result = TRUE;
      return RET_OK;
    }
    /* off, Off, OFF */
    if (tk_str_ieq(value, "off")) {
      *result = FALSE;
      return RET_OK;
    }
  } else if (c == 'f' || c == 'F') {
    /* false, False, FALSE */
    if (tk_str_ieq(value, "false")) {
      *result = FALSE;
      return RET_OK;
    }
  } else if (c == 'n' || c == 'N') {
    /* no, No, NO */
    if (tk_str_ieq(value, "no")) {
      *result = FALSE;
      return RET_OK;
    }
  }
  /* 注意：不将 "1" 和 "0" 转换为布尔值，保持为字符串
   * 这样 conf_doc_get_str 可以正确获取 "1" 和 "0"
   * 而 conf_doc_get_bool 仍然可以通过 value_bool 正确转换 */

  /* 不是布尔值 */
  return RET_NOT_FOUND;
}

static bool_t yaml_is_null_value(const char* value) {
  if (value == NULL || *value == '\0') {
    return FALSE;
  }

  /* 检查是否是 null 值：null、Null、NULL、~ */
  char c = *value;
  if (c == 'n' || c == 'N') {
    /* null, Null, NULL */
    if (tk_str_ieq(value, "null")) {
      return TRUE;
    }
  } else if (c == '~') {
    /* ~ */
    if (tk_str_eq(value, "~")) {
      return TRUE;
    }
  }

  return FALSE;
}

/* 解析多行字符串（字面量块 | 或折叠块 >） */
static const char* yaml_parser_parse_multiline_string(yaml_parser_t* parser, char block_char) {
  const char* p = parser->cursor;
  str_t* s = &(parser->str);
  bool_t literal = (block_char == '|'); /* true=字面量块, false=折叠块 */
  bool_t keep_trailing = FALSE; /* + 修饰符：保留末尾换行 */
  bool_t strip_trailing = FALSE; /* - 修饰符：去除末尾换行 */
  int32_t indent = -1; /* 数字修饰符：指定缩进级别 */
  
  if (*p != '|' && *p != '>') {
    return NULL;
  }
  p++; /* 跳过 | 或 > */
  
  /* 解析修饰符 */
  if (*p == '+') {
    keep_trailing = TRUE;
    p++;
  } else if (*p == '-') {
    strip_trailing = TRUE;
    p++;
  } else if (tk_isdigit(*p)) {
    /* 解析缩进数字 */
    indent = tk_atoi(p);
    while (tk_isdigit(*p)) {
      p++;
    }
  }
  
  /* 跳过空格和注释，移动到下一行 */
  yaml_parser_skip_spaces(&p);
  if (*p == YAML_COMMENT_CHAR) {
    /* 跳过注释 */
    while (*p && *p != '\r' && *p != '\n') {
      p++;
    }
  }
  if (*p == '\r' || *p == '\n') {
    p++;
    if (*p == '\n' && *(p-1) == '\r') {
      p++;
    }
  }
  
  parser->cursor = p;
  
  /* 确定块的缩进级别（第一行非空内容的缩进） */
  uint32_t block_indent = 0;
  bool_t found_indent = FALSE;
  
  /* 读取多行内容 */
  str_clear(s);
  bool_t first_line = TRUE;
  bool_t last_was_newline = FALSE;
  
  while (*p != '\0') {
    /* 检查是否是空行（表示块结束） */
    const char* line_check = p;
    uint32_t line_indent = 0;
    
    /* 计算当前行的缩进 */
    while (*line_check == ' ') {
      line_indent++;
      line_check++;
    }
    
    /* 空行或注释行 */
    if (*line_check == '\r' || *line_check == '\n' || *line_check == '\0') {
      /* 空行：在字面量块中保留，在折叠块中转换为换行，但块继续 */
      if (!first_line) {
        /* 处理空行 */
        if (literal) {
          /* 字面量块：保留空行为换行（添加一个换行表示空行） */
          if (str_append_char(s, '\n') != RET_OK) {
            log_warn("yaml parse multiline: out of memory\n");
            return NULL;
          }
          last_was_newline = TRUE;
        } else {
          /* 折叠块：空行转换为单个换行 */
          if (s->size > 0 && s->str[s->size - 1] != '\n') {
            if (str_append_char(s, '\n') != RET_OK) {
              log_warn("yaml parse multiline: out of memory\n");
              return NULL;
            }
          }
          last_was_newline = TRUE;
        }
        
        /* 跳过空行 */
        while (*p == '\r' || *p == '\n') {
          if (*p == '\r' && p[1] == '\n') {
            p += 2;
          } else {
            p++;
          }
        }
        parser->cursor = p;
        first_line = FALSE;
        continue;
      } else {
        /* 第一行就是空行，跳过 */
        while (*p == '\r' || *p == '\n') {
          if (*p == '\r' && p[1] == '\n') {
            p += 2;
          } else {
            p++;
          }
        }
        parser->cursor = p;
        continue;
      }
    } else if (*line_check == YAML_COMMENT_CHAR) {
      /* 注释行：跳过，但块继续（如果已有内容） */
      if (!first_line) {
        /* 跳过注释行 */
        while (*p && *p != '\r' && *p != '\n') {
          p++;
        }
        while (*p == '\r' || *p == '\n') {
          if (*p == '\r' && p[1] == '\n') {
            p += 2;
          } else {
            p++;
          }
        }
        parser->cursor = p;
        continue;
      } else {
        /* 第一行是注释，跳过 */
        while (*p && *p != '\r' && *p != '\n') {
          p++;
        }
        while (*p == '\r' || *p == '\n') {
          if (*p == '\r' && p[1] == '\n') {
            p += 2;
          } else {
            p++;
          }
        }
        parser->cursor = p;
        continue;
      }
    }
    
    /* 检查缩进是否小于块的缩进（表示块结束） */
    if (found_indent && line_indent < block_indent) {
      break;
    }
    
    /* 确定块的缩进（第一行非空内容） */
    if (!found_indent && line_indent > 0) {
      block_indent = line_indent;
      found_indent = TRUE;
    }
    
    /* 如果指定了缩进，使用指定的缩进 */
    if (indent >= 0 && !found_indent) {
      block_indent = indent;
      found_indent = TRUE;
    }
    
    /* 读取当前行内容 */
    if (!first_line) {
      if (literal) {
        /* 字面量块：保留换行 */
        if (str_append_char(s, '\n') != RET_OK) {
          log_warn("yaml parse multiline: out of memory\n");
          return NULL;
        }
        last_was_newline = TRUE;
      } else {
        /* 折叠块：换行转换为空格（除非是连续的空行） */
        if (s->size > 0 && !last_was_newline) {
          /* 如果上一行有内容，添加空格 */
          if (str_append_char(s, ' ') != RET_OK) {
            log_warn("yaml parse multiline: out of memory\n");
            return NULL;
          }
        }
        last_was_newline = FALSE; /* 将在读取内容后更新 */
      }
    }
    
    /* 跳过行的缩进（去除块缩进） */
    uint32_t skip_indent = found_indent ? block_indent : 0;
    uint32_t skipped = 0;
    while (*p == ' ' && skipped < skip_indent) {
      p++;
      skipped++;
    }
    
    /* 读取行内容到换行 */
    bool_t line_has_content = FALSE;
    while (*p != '\r' && *p != '\n' && *p != '\0') {
      if (*p == YAML_COMMENT_CHAR) {
        /* 跳过注释 */
        break;
      }
      line_has_content = TRUE;
      if (str_append_char(s, *p) != RET_OK) {
        log_warn("yaml parse multiline: out of memory\n");
        return NULL;
      }
      p++;
    }
    
    if (line_has_content) {
      last_was_newline = FALSE;
    } else {
      /* 空行内容已经在前面处理过了，这里不需要再处理 */
      last_was_newline = TRUE;
    }
    
    first_line = FALSE;
    
    /* 跳过换行符 */
    if (*p == '\r') {
      p++;
      if (*p == '\n') {
        p++;
      }
    } else if (*p == '\n') {
      p++;
    }
  }
  
  parser->cursor = p;
  
  /* 处理末尾换行 */
  if (keep_trailing) {
    /* + 修饰符：保留末尾换行 */
    if (literal && s->size > 0 && s->str[s->size - 1] != '\n') {
      if (str_append_char(s, '\n') != RET_OK) {
        log_warn("yaml parse multiline: out of memory\n");
        return NULL;
      }
    }
  } else if (strip_trailing) {
    /* - 修饰符：去除末尾换行 */
    while (s->size > 0 && s->str[s->size - 1] == '\n') {
      s->size--;
      s->str[s->size] = '\0';
    }
  }
  
  /* 去除末尾空格（折叠块） */
  if (!literal && s->size > 0) {
    str_trim_right(s, " \t");
  }
  
  return s->str;
}

/* 解析流式集合中的值（可能是简单值、引号字符串或嵌套流式集合） */
static ret_t yaml_parser_parse_flow_value(yaml_parser_t* parser, conf_node_t* node) {
  const char* p = parser->cursor;
  const char* value = NULL;
  value_t v;
  bool_t bool_val = FALSE;

  /* 跳过空格 */
  yaml_parser_skip_spaces(&p);
  parser->cursor = p;

  /* 检查是否是流式集合 */
  if (*p == '[') {
    /* 流式序列 */
    return yaml_parser_parse_flow_sequence(parser, node);
  } else if (*p == '{') {
    /* 流式映射 */
    return yaml_parser_parse_flow_mapping(parser, node);
  } else if (*p == '"') {
    /* 双引号字符串（流式集合中，不跳过到行尾） */
    value = yaml_parser_parse_quoted_string_internal(parser, '"', FALSE);
    if (value != NULL) {
      value_set_str(&v, value);
      conf_node_set_value(node, &v);
      /* 跳过引号后的空格 */
      yaml_parser_skip_spaces(&parser->cursor);
    }
    return value != NULL ? RET_OK : RET_FAIL;
  } else if (*p == '\'') {
    /* 单引号字符串（流式集合中，不跳过到行尾） */
    value = yaml_parser_parse_quoted_string_internal(parser, '\'', FALSE);
    if (value != NULL) {
      value_set_str(&v, value);
      conf_node_set_value(node, &v);
      /* 跳过引号后的空格 */
      yaml_parser_skip_spaces(&parser->cursor);
    }
    return value != NULL ? RET_OK : RET_FAIL;
  } else {
    /* 普通值：解析到逗号、] 或 } 为止 */
    str_t* s = &(parser->str);
    str_clear(s);
    
    while (*p && *p != ',' && *p != ']' && *p != '}' && *p != '\r' && *p != '\n') {
      if (*p == '\\') {
        p++;
        uint32_t n = 0;
        char c = str_unescape_char(p, &n);
        if (str_append_char(s, c) != RET_OK) {
          log_warn("yaml parse flow value: out of memory\n");
          return RET_OOM;
        }
        p += n;
      } else {
        if (str_append_char(s, *p) != RET_OK) {
          log_warn("yaml parse flow value: out of memory\n");
          return RET_OOM;
        }
        p++;
      }
    }
    
    parser->cursor = p;
    str_trim(s, " \t");
    
    value = s->str;
    if (value == NULL || *value == '\0') {
      /* 空值 */
      value_set_str(&v, NULL);
    } else if (yaml_is_null_value(value)) {
      /* null 值 */
      value_set_str(&v, NULL);
    } else if (yaml_parse_bool_value(value, &bool_val) == RET_OK) {
      /* 布尔值 */
      value_set_bool(&v, bool_val);
    } else {
      /* 字符串值 */
      value_set_str(&v, value);
    }
    
    conf_node_set_value(node, &v);
    return RET_OK;
  }
}

/* 解析流式序列 [item1, item2, item3] */
static ret_t yaml_parser_parse_flow_sequence(yaml_parser_t* parser, conf_node_t* node) {
  const char* p = parser->cursor;
  uint32_t index = 0;
  char index_name[32] = {0};
  conf_node_t* child = NULL;

  if (*p != '[') {
    return RET_BAD_PARAMS;
  }
  p++; /* 跳过 '[' */
  parser->cursor = p;

  /* 设置节点类型为数组 */
  node->node_type = CONF_NODE_ARRAY;
  node->value_type = CONF_NODE_VALUE_NODE;

  /* 跳过空格 */
  yaml_parser_skip_spaces(&p);
  parser->cursor = p;

  /* 空数组 */
  if (*p == ']') {
    parser->cursor = p + 1;
    return RET_OK;
  }

  /* 解析数组元素 */
  while (*p && *p != ']') {
    /* 跳过空格和逗号 */
    yaml_parser_skip_spaces(&p);
    if (*p == ',') {
      p++;
      parser->cursor = p;
      yaml_parser_skip_spaces(&p);
      parser->cursor = p;
      continue;
    }
    if (*p == ']') {
      break;
    }

    /* 创建数组元素节点 */
    if (tk_snprintf(index_name, sizeof(index_name), "%u", index++) >= sizeof(index_name)) {
      log_warn("yaml flow sequence index too large\n");
      return RET_FAIL;
    }
    child = conf_doc_create_node(parser->doc, index_name);
    return_value_if_fail(child != NULL, RET_OOM);

    /* 解析元素值 */
    parser->cursor = p;
    ret_t ret = yaml_parser_parse_flow_value(parser, child);
    if (ret != RET_OK) {
      conf_doc_destroy_node(parser->doc, child);
      return ret;
    }

    conf_doc_append_child(parser->doc, node, child);
    p = parser->cursor;

    /* 跳过空格 */
    yaml_parser_skip_spaces(&p);
    parser->cursor = p;
  }

  if (*p == ']') {
    parser->cursor = p + 1;
  }

  return RET_OK;
}

/* 解析流式映射 {key1: value1, key2: value2} */
static ret_t yaml_parser_parse_flow_mapping(yaml_parser_t* parser, conf_node_t* node) {
  const char* p = parser->cursor;
  conf_node_t* child = NULL;
  const char* name = NULL;

  if (*p != '{') {
    return RET_BAD_PARAMS;
  }
  p++; /* 跳过 '{' */
  parser->cursor = p;

  /* 设置节点类型为对象 */
  node->node_type = CONF_NODE_OBJECT;
  node->value_type = CONF_NODE_VALUE_NODE;

  /* 跳过空格 */
  yaml_parser_skip_spaces(&p);
  parser->cursor = p;

  /* 空对象 */
  if (*p == '}') {
    parser->cursor = p + 1;
    return RET_OK;
  }

  /* 解析键值对 */
  while (*p && *p != '}') {
    /* 跳过空格和逗号 */
    yaml_parser_skip_spaces(&p);
    parser->cursor = p;
    if (*p == ',') {
      p++;
      parser->cursor = p;
      yaml_parser_skip_spaces(&p);
      parser->cursor = p;
      continue;
    }
    if (*p == '}') {
      break;
    }

    /* 解析键名 */
    name = yaml_parser_parse_name(parser);
    if (name == NULL || *name == '\0') {
      log_warn("yaml flow mapping: invalid key name\n");
      return RET_FAIL;
    }
    p = parser->cursor;

    /* 跳过键名后的空格和冒号 */
    yaml_parser_skip_spaces(&p);
    parser->cursor = p;
    if (*p != ':') {
      log_warn("yaml flow mapping: expected ':' after key\n");
      return RET_FAIL;
    }
    p++;
    parser->cursor = p;
    yaml_parser_skip_spaces(&p);
    parser->cursor = p;

    /* 创建子节点 */
    child = conf_doc_create_node(parser->doc, name);
    return_value_if_fail(child != NULL, RET_OOM);

    /* 解析值 */
    ret_t ret = yaml_parser_parse_flow_value(parser, child);
    if (ret != RET_OK) {
      conf_doc_destroy_node(parser->doc, child);
      return ret;
    }

    conf_doc_append_child(parser->doc, node, child);
    p = parser->cursor;

    /* 跳过空格 */
    yaml_parser_skip_spaces(&p);
    parser->cursor = p;
  }

  if (*p == '}') {
    parser->cursor = p + 1;
  }

  return RET_OK;
}

static const char* yaml_parser_parse_value(yaml_parser_t* parser, uint32_t csep) {
  char c = '\0';
  uint32_t n = 0;
  str_t* s = &(parser->str);
  const char* p = parser->cursor;

  if (*p != csep) {
    return NULL;
  } else {
    p++;
  }

  /* 跳过分隔符后的空格 */
  yaml_parser_skip_spaces(&p);

  /* 检查是否是流式集合 */
  if (*p == '[' || *p == '{') {
    /* 流式集合，返回特殊标记（调用者会处理） */
    parser->cursor = p;
    return (const char*)(uintptr_t)1; /* 使用非NULL指针作为标记 */
  }

  /* 检查是否是多行字符串 */
  if (*p == '|' || *p == '>') {
    parser->cursor = p;
    return yaml_parser_parse_multiline_string(parser, *p);
  }

  /* 检查是否是引号字符串 */
  if (*p == '"') {
    parser->cursor = p;
    return yaml_parser_parse_quoted_string(parser, '"');
  } else if (*p == '\'') {
    parser->cursor = p;
    return yaml_parser_parse_quoted_string(parser, '\'');
  }

  str_clear(s);
  do {
    if (*p == '\\') {
      p++;
      if (*p == YAML_COMMENT_CHAR) {
        c = *p;
        p++;
      } else {
        c = str_unescape_char(p, &n);
        p += n;
      }
    } else if (*p == YAML_COMMENT_CHAR) {
      break;
    } else if (*p == '\r' || *p == '\n' || *p == '\0') {
      break;
    } else {
      c = *p++;
    }

    if (str_append_char(s, c) != RET_OK) {
      log_warn("yaml parse value: out of memory\n");
      return NULL;
    }
  } while (1);

  parser->cursor = p;
  str_trim(s, " \t\r\n");
  yaml_parser_skip_to_line_end(parser);

  return s->str;
}

static const char* yaml_parser_parse_name(yaml_parser_t* parser) {
  const char* p = parser->cursor;
  str_t* s = &parser->str;

  /* 跳过前导空格 */
  yaml_parser_skip_spaces(&p);

  /* 检查是否是引号键名 */
  if (*p == '"' || *p == '\'') {
    char quote_char = *p;
    parser->cursor = p;
    const char* name = yaml_parser_parse_quoted_string_internal(parser, quote_char, FALSE);
    /* 跳过键名后的空格 */
    yaml_parser_skip_spaces(&parser->cursor);
    return name;
  }

  str_clear(s);
  do {
    char c = *p;
    if (c == YAML_SEP_CHAR) {
      break;
    } else if (c == '\0') {
      log_warn("yaml unexpected end\n");
      break;
    } else if (c == '\r' || c == '\n') {
      log_warn("yaml unexpected line end\n");
      break;
    } else if (c == YAML_COMMENT_CHAR) {
      /* 遇到注释字符，停止解析键名 */
      break;
    } else if (tk_isalpha(c) || tk_isdigit(c) || c == '_' || c == '-' || c == '.' || c == '/') {
      if (str_append_char(s, *p) != RET_OK) {
        log_warn("yaml parse name: out of memory\n");
        break;
      }
    } else if (c == ' ' || c == '\t') {
      /* 遇到空格，停止解析（除非是引号键名） */
      break;
    } else {
      log_warn("yaml invalid char '%c' for name\n", c);
      /* 遇到无效字符，停止解析 */
      break;
    }
    p++;
  } while (1);

  parser->cursor = p;
  str_trim(s, " \t\r\n");

  return s->str;
}

static conf_node_t* yaml_parser_get_parent_node(yaml_parser_t* parser, uint32_t leading_spaces) {
  conf_node_t* parent = parser->current_node;

  while (parent->leading_spaces >= leading_spaces) {
    parent = parent->parent;
  }

  return parent;
}

static ret_t yaml_parser_parse_line(yaml_parser_t* parser) {
  ret_t ret = RET_OK;
  conf_node_t* node = NULL;
  conf_node_t* parent = NULL;
  const char* name = NULL;
  const char* value = NULL;
  char index_name[32] = {0};
  bool_t is_list = FALSE;
  uint32_t leading_spaces = yaml_parser_skip_leading_space(parser) + YAML_LEADING_SPACE_OFFSET;

  parent = yaml_parser_get_parent_node(parser, leading_spaces);
  ENSURE(parent != NULL);

  if (parser->cursor[0] == YAML_COMMENT_CHAR) {
    return yaml_parser_skip_to_line_end(parser);
  } else if (parser->cursor[0] == '\r' || parser->cursor[0] == '\n') {
    return yaml_parser_skip_to_line_end(parser);
  } else if (parser->cursor[0] == '\0') {
    return RET_EOS;
  } else if (parser->cursor[0] == YAML_LIST_START_CHAR) {
    uint32_t index = conf_node_count_children(parent);
    if (tk_snprintf(index_name, sizeof(index_name), "%u", index) >= sizeof(index_name)) {
      log_warn("yaml array index too large\n");
      return RET_FAIL;
    }
    name = index_name;
    is_list = TRUE;
    parent->node_type = CONF_NODE_ARRAY;
  } else {
    name = yaml_parser_parse_name(parser);
  }

  node = conf_doc_create_node(parser->doc, name);
  return_value_if_fail(node != NULL, RET_OOM);
  node->leading_spaces = leading_spaces;

  value = yaml_parser_parse_value(parser, is_list ? YAML_LIST_START_CHAR : YAML_SEP_CHAR);
  if (value != NULL) {
    /* 检查是否是流式集合标记 */
    if (value == (const char*)(uintptr_t)1) {
      /* 流式集合，由 parse_value 内部处理 */
      ret_t flow_ret = yaml_parser_parse_flow_value(parser, node);
      if (flow_ret != RET_OK) {
        conf_doc_destroy_node(parser->doc, node);
        return flow_ret;
      }
    } else {
      value_t v;
      bool_t bool_val = FALSE;
      
      /* 检查是否是 null 值 */
      if (yaml_is_null_value(value)) {
        value_set_str(&v, NULL);
      } else if (*value) {
        /* 检查是否是布尔值 */
        if (yaml_parse_bool_value(value, &bool_val) == RET_OK) {
          value_set_bool(&v, bool_val);
        } else {
          value_set_str(&v, value);
        }
      } else {
        /* 空字符串 */
        value_set_str(&v, value);
      }
      conf_node_set_value(node, &v);
    }
  }

  ret = conf_doc_append_child(parser->doc, parent, node);
  if (ret == RET_OK) {
    parser->current_node = node;
  } else {
    conf_doc_destroy_node(parser->doc, node);
  }

  return ret;
}

static ret_t yaml_parser_parse(yaml_parser_t* parser) {
  do {
    ret_t ret = yaml_parser_parse_line(parser);
    if (ret == RET_EOS) {
      break;
    } else if (ret != RET_OK) {
      return ret;
    }
  } while (1);

  return RET_OK;
}

conf_doc_t* conf_doc_load_yaml(const char* data) {
  ret_t ret = RET_OK;
  yaml_parser_t parser;
  conf_doc_t* doc = NULL;

  if (data == NULL) {
    return NULL;
  }

  /* 空字符串应该返回空文档 */
  if (*data == '\0') {
    conf_doc_t* empty_doc = conf_doc_create(100);
    if (empty_doc != NULL) {
      empty_doc->root = conf_doc_create_node(empty_doc, CONF_NODE_ROOT_NAME);
      if (empty_doc->root == NULL) {
        conf_doc_destroy(empty_doc);
        return NULL;
      }
    }
    return empty_doc;
  }

  return_value_if_fail(yaml_parser_init(&parser, data) == RET_OK, NULL);

  ret = yaml_parser_parse(&parser);
  if (ret == RET_OK) {
    doc = parser.doc;
    parser.doc = NULL;
  }

  yaml_parser_deinit(&parser);

  return doc;
}

static bool_t yaml_need_quote_internal(const char* str, bool_t include_quotes) {
  const char* p = str;
  if (str == NULL || *str == '\0') {
    return FALSE;
  }

  /* 检查是否包含需要引号的字符 */
  while (*p != '\0') {
    char c = *p;
    if (c == ' ' || c == '\t' || c == ':' || c == '#' || c == '\n' || c == '\r' || 
        c == '[' || c == ']' || c == '{' || c == '}' || c == ',' || c == '&' || 
        c == '*' || c == '!' || c == '|' || c == '>' || c == '@' || c == '`') {
      return TRUE;
    }
    /* 键名包含点号需要引号，避免被解析为嵌套结构 */
    if (!include_quotes && c == '.') {
      return TRUE;
    }
    /* 值需要额外检查引号字符 */
    if (include_quotes && (c == '\\' || c == '"' || c == '\'')) {
      return TRUE;
    }
    p++;
  }

  return FALSE;
}

static bool_t yaml_need_quote_key(const char* key) {
  return yaml_need_quote_internal(key, FALSE);
}

static bool_t yaml_need_quote_value(const char* value) {
  return yaml_need_quote_internal(value, TRUE);
}

static ret_t yaml_save_quoted_string(str_t* str, const char* value, char quote_char) {
  const char* p = value;
  return_value_if_fail(str_append_char(str, quote_char) == RET_OK, RET_OOM);
  
  while (*p != '\0') {
    if (*p == quote_char) {
      /* 转义引号 */
      return_value_if_fail(str_append_char(str, '\\') == RET_OK, RET_OOM);
      return_value_if_fail(str_append_char(str, quote_char) == RET_OK, RET_OOM);
    } else if (*p == '\\') {
      /* 转义反斜杠 */
      return_value_if_fail(str_append_char(str, '\\') == RET_OK, RET_OOM);
      return_value_if_fail(str_append_char(str, '\\') == RET_OK, RET_OOM);
    } else {
      /* 引号字符串中，控制字符直接保存为实际字符 */
      return_value_if_fail(str_append_char(str, *p) == RET_OK, RET_OOM);
    }
    p++;
  }
  
  return_value_if_fail(str_append_char(str, quote_char) == RET_OK, RET_OOM);
  return RET_OK;
}

static ret_t yaml_save_value_with_quote(str_t* str, const value_t* v, char comment_char) {
  if (v->type == VALUE_TYPE_STRING) {
    const char* p = value_str(v);
    /* 检查是否是 NULL 值 */
    if (p == NULL) {
      return str_append(str, "null");
    }
    /* 空字符串：如果是从引号字符串解析来的，应该保存为 "" */
    if (*p == '\0') {
      /* 空字符串保存为空字符串（不加引号，因为不包含特殊字符） */
      return RET_OK;
    }
    if (yaml_need_quote_value(p)) {
      /* 使用双引号保存 */
      return yaml_save_quoted_string(str, p, '"');
    }
  } else if (v->type == VALUE_TYPE_WSTRING) {
    str_t s;
    ret_t ret = RET_OK;
    str_init(&s, 0);
    str_from_wstr(&s, value_wstr(v));
    if (s.str == NULL) {
      ret = str_append(str, "null");
    } else if (*s.str == '\0') {
      /* 空字符串 */
      ret = RET_OK;
    } else if (yaml_need_quote_value(s.str)) {
      ret = yaml_save_quoted_string(str, s.str, '"');
    } else {
      ret = conf_node_save_value(str, v, comment_char);
    }
    str_reset(&s);
    return ret;
  }
  
  return conf_node_save_value(str, v, comment_char);
}

static ret_t conf_doc_save_yaml_node_name_value(conf_node_t* node, str_t* str, uint32_t levels) {
  value_t v;
  ret_t ret = RET_OK;
  const char* key = conf_node_get_name(node);

  if (levels > 0) {
    ret = str_append_n_chars(str, ' ', levels * 2);
    return_value_if_fail(ret == RET_OK, ret);
  }

  if (node->parent->node_type == CONF_NODE_ARRAY) {
    return_value_if_fail(str_append_char(str, YAML_LIST_START_CHAR) == RET_OK, RET_OOM);
  } else {
    /* 如果键名需要引号，添加引号 */
    if (yaml_need_quote_key(key)) {
      return_value_if_fail(yaml_save_quoted_string(str, key, '"') == RET_OK, RET_OOM);
    } else {
      return_value_if_fail(str_append(str, key) == RET_OK, RET_OOM);
    }
    return_value_if_fail(str_append_char(str, YAML_SEP_CHAR) == RET_OK, RET_OOM);
  }

  ret = conf_node_get_value(node, &v);
  if (ret == RET_OK) {
    const char* p = value_str(&v);
    /* 如果是空字符串，不添加空格和值 */
    if (p != NULL && *p != '\0') {
      return_value_if_fail(str_append(str, " ") == RET_OK, RET_OOM);
      return_value_if_fail(yaml_save_value_with_quote(str, &v, YAML_COMMENT_CHAR) == RET_OK, RET_OOM);
    } else if (p == NULL) {
      /* NULL 值，添加空格和 null */
      return_value_if_fail(str_append(str, " ") == RET_OK, RET_OOM);
      return_value_if_fail(yaml_save_value_with_quote(str, &v, YAML_COMMENT_CHAR) == RET_OK, RET_OOM);
    }
    /* 空字符串不添加任何内容 */
  }
  return_value_if_fail(str_append(str, "\n") == RET_OK, RET_OOM);

  return RET_OK;
}

static ret_t conf_doc_save_yaml_node(conf_node_t* node, str_t* str, uint32_t levels) {
  return_value_if_fail(node != NULL && str != NULL, RET_BAD_PARAMS);

  while (node != NULL) {
    conf_doc_save_yaml_node_name_value(node, str, levels);

    if (node->value_type == CONF_NODE_VALUE_NODE) {
      conf_node_t* iter = conf_node_get_first_child(node);
      conf_doc_save_yaml_node(iter, str, levels + 1);
    }

    node = node->next;
  }

  return RET_OK;
}

ret_t conf_doc_save_yaml(conf_doc_t* doc, str_t* str) {
  conf_node_t* node = NULL;
  return_value_if_fail(doc != NULL && str != NULL, RET_BAD_PARAMS);
  str_clear(str);

  node = conf_node_get_first_child(doc->root);
  /* 空文档是合法的，直接返回成功 */
  if (node == NULL) {
    return RET_OK;
  }

  return conf_doc_save_yaml_node(node, str, 0);
}

static conf_doc_t* conf_doc_load_yaml_reader(data_reader_t* reader) {
  char* data = NULL;
  int32_t rsize = 0;
  conf_doc_t* doc = NULL;
  uint64_t size = 0;

  return_value_if_fail(reader != NULL, NULL);
  size = data_reader_get_size(reader);
  /* 防止整数溢出 */
  return_value_if_fail(size > 0 && size < UINT32_MAX, NULL);

  data = TKMEM_ALLOC(size + 1);
  return_value_if_fail(data != NULL, NULL);

  memset(data, 0x00, size + 1);
  rsize = data_reader_read(reader, 0, data, size);
  if (rsize > 0) {
    doc = conf_doc_load_yaml(data);
  }
  TKMEM_FREE(data);

  return doc;
}

static ret_t conf_doc_save_yaml_writer(conf_doc_t* doc, data_writer_t* writer) {
  str_t str;
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);

  goto_error_if_fail(str_init(&str, YAML_SAVE_BUFFER_INIT_SIZE) != NULL);
  goto_error_if_fail(conf_doc_save_yaml(doc, &str) == RET_OK);
  goto_error_if_fail(data_writer_write(writer, 0, str.str, str.size) == str.size);
  str_reset(&str);

  return RET_OK;

error:
  str_reset(&str);

  return RET_FAIL;
}

tk_object_t* conf_yaml_load(const char* url, bool_t create_if_not_exist) {
  return conf_obj_create(conf_doc_save_yaml_writer, conf_doc_load_yaml_reader, url,
                         create_if_not_exist);
}

tk_object_t* conf_yaml_load_ex(const char* url, bool_t create_if_not_exist,
                               bool_t use_extend_type) {
  return conf_obj_create_ex(conf_doc_save_yaml_writer, conf_doc_load_yaml_reader, url,
                            create_if_not_exist, use_extend_type);
}

ret_t conf_yaml_save_as(tk_object_t* obj, const char* url) {
  ret_t ret = RET_OK;
  data_writer_t* writer = NULL;
  conf_doc_t* doc = conf_obj_get_doc(obj);

  return_value_if_fail(doc != NULL && url != NULL, RET_BAD_PARAMS);
  writer = data_writer_factory_create_writer(data_writer_factory(), url);
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);

  ret = conf_doc_save_yaml_writer(doc, writer);
  data_writer_destroy(writer);

  return ret;
}

tk_object_t* conf_yaml_create(void) {
  return conf_yaml_load(NULL, TRUE);
}

tk_object_t* conf_yaml_load_from_buff(const void* buff, uint32_t size, bool_t create_if_not_exist) {
  char url[MAX_PATH + 1] = {0};
  return_value_if_fail(buff != NULL, NULL);
  data_reader_mem_build_url(buff, size, url);

  return conf_yaml_load(url, create_if_not_exist);
}

ret_t conf_yaml_save_to_buff(tk_object_t* obj, wbuffer_t* wb) {
  char url[MAX_PATH + 1] = {0};
  return_value_if_fail(obj != NULL && wb != NULL, RET_BAD_PARAMS);

  wbuffer_init_extendable(wb);
  data_writer_wbuffer_build_url(wb, url);

  return conf_yaml_save_as(obj, url);
}
