/**
 * File:   code_assist.cpp
 * Author: AWTK Develop Team
 * Brief:  代码辅助模块接口实现。
 *
 * Copyright (c) 2022 - 2023 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-02-28 Wang JunSheng <wangjunsheng@zlg.cn> created
 *
 */

#include "code_assist.h"
#include "tkc/utils.h"
#include "tkc/mem.h"
#include "tkc/fs.h"
#include "ctags/ctags.h"

#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

// 出现顺序影响到补全列表排序
// 越小越靠前
enum TCB_TYPE {
  TCB_KEYWORD,
  TCB_Variable,
  TCB_Def,
  TCB_Class,
  TCB_Struct,
  TCB_Typeref,
  TCB_Prototype,
  TCB_Func,
  TCB_Xref,
  TCB_Enum,
  TCB_EName,
  TCB_Mem,
  TCB_UNKNOWN,
  TCB_Include,

  TCB_Type_num,
};

inline bool is_empty_s(const char* s) { return 0==s || 0==s[0]; }

// 析构时，并不会销毁 s, 
// 构造时申请 是为了使用方便
// comment 会统一登记， 销毁
struct comment_t {
  const char* s;
  int ref_cnt;

  comment_t(const string& str);

  void ref() { ++ref_cnt; }
  void unref();
};

/* 要频繁的对 node对象 进行 排序，所以 里面不要放 string 等， 会大幅降低性能
   如果实在要放， 那么 建议改成 tcb_symbol_node*, 不过这个改动很大
 */
struct tcb_symbol_node {
  int type;

  const char* name;
  const char* file;
  const char* line_content;
  const char* param;
  const char* scope;
  const char* typeref;
  comment_t* comment;
  int pos;
  int line;

  tcb_symbol_node();

  bool is_enum() const {
    return TCB_EName == type || TCB_Enum == type;
  }
  bool is_define() const {
    return TCB_Def == type;
  }
  bool is_macro_fun() const {
    return is_define() && !is_empty_s(param);
  }
  bool is_include() const {
    return TCB_Include == type;
  }
  bool is_struct_class() const {
    return TCB_Class == type || TCB_Struct == type;
  }
  bool is_type() const {
    return TCB_Class == type || TCB_Struct == type || TCB_Typeref == type;
  }
  bool is_function() const {
    return is_fun_define() || is_fun_prototype();
  }
  bool is_variable() const {
    return TCB_Variable == type || TCB_Xref == type;
  }
  bool is_variable_define() const {
    return TCB_Variable == type;
  }
  bool is_fun_define() const {
    return TCB_Func == type;
  }
  bool is_fun_prototype() const {
    return TCB_Prototype == type;
  }
  bool is_keyword() const {
    return TCB_KEYWORD == type;
  }

  ca_symbol_type_t to_ca_type() const;
};

struct compile_unit {
  vector<tcb_symbol_node> symbols;

  // 词素的注释字符串比较长，又很少会重复， 放在 atom 里是不合适的
  // 全部注册在 comments 中， 每次重新解析文件的时候 释放
  // tcb_symbol_node.comment new 出来后， 注册到 comments 中
  // comments 进行统一的资源释放
  vector<comment_t*> comments; 

  void clear() {
    symbols.clear();
    delete_comments();
  }
  void delete_comments();
  ~compile_unit();
};

inline bool tcb_strsame(const char* s1, const char* s2) {
  return s1 == s2;
}

char tcb_type_from_ctags_kind(char kind) {
  switch (kind) {
    case 'd':
      return TCB_Def;
    case 'f':
      return TCB_Func;
    case 'p':
      return TCB_Prototype;
    case 'c':
      return TCB_Class;
    case 's':
      return TCB_Struct;
    case 'u':
      return TCB_Struct;
    case 'v':
      return TCB_Variable;
    case 'x':
      return TCB_Xref;
    case 'i':
      return TCB_Include;
    case 'e':
      return TCB_EName;  // union names 和 enum names 暂时没区分
    case 't':
      return TCB_Typeref;
    case 'g':
      return TCB_Enum;
    case 'm':
      return TCB_Mem;

    default:
      return TCB_UNKNOWN;
  }
}

vector<string> split(const string& s_, const string& sep_) {
  vector<string> result;
  typedef string::const_iterator citer;
  citer sbeg = sep_.begin();
  citer send = sep_.end();
  for (citer beg = s_.begin(), end = s_.end(); beg != end;) {
    //citer tbeg = find_first_not_of(beg, end, sbeg, send);
    citer tbeg = beg;
    for (; tbeg != end; ++tbeg) {
      if (count(sbeg, send, *tbeg) == 0) {
        break;
      }
    }
    citer tend = find_first_of(tbeg, end, sbeg, send);
    if (tbeg != tend) result.push_back(string(tbeg, tend));
    beg = tend;
  }
  return result;
}

tcb_symbol_node::tcb_symbol_node()
    : type(TCB_UNKNOWN),
      name(NULL),
      file(NULL),
      line_content(NULL),
      param(NULL),
      scope(NULL),
      typeref(NULL),
      comment(NULL),
      pos(0),
      line(0) {
}

ca_symbol_type_t tcb_symbol_node::to_ca_type() const {
  if (is_fun_prototype())
    return CA_FUNC_PROTOTYPE;
  else if (is_fun_define() || is_macro_fun())
    return CA_FUNC_DEF;
  else if (is_define()) 
    return CA_DEF;
  else if (is_type())
    return CA_TYPE;
  else if (is_include())
    return CA_INCLUDE;
  else if (is_keyword())
    return CA_KEYWORD;
  else if (is_enum()) 
    return CA_ENUM;

  return CA_GENERAL;
}

void to_symbol_t(ca_symbol_t& to, tcb_symbol_node& from) {
  to.type = from.to_ca_type();
  to.file = from.file;
  to.pos = from.pos;
  to.line = from.line;
  to.name = from.name;
  to.param = from.param;
  to.line_content = from.line_content;
  to.scope = from.scope;
  to.typeref = from.typeref;
  if (from.comment) {
    to.comment = tk_str_copy((char*)to.comment, from.comment->s);
  }
}

void compile_unit::delete_comments() {
  for (auto c : comments) {
    c->unref();
  }
  comments.clear();
}

compile_unit::~compile_unit() {
  delete_comments();
}

comment_t::comment_t(const string& str) {
  s = tk_strndup(str.c_str(), str.length());
  ref_cnt = 1;
}

void comment_t::unref() {
  if (--ref_cnt == 0) {
    TKMEM_FREE(s);
    delete this;
  }
}

namespace {
class AtomsImpl;
class Atoms{
public:
    const char * intern(const string &s);
    const char * intern(const char *s);

    bool exist(const string& s) const;

    void dump(vector<const char*>& v) const;

    Atoms();
    ~Atoms();
private:
    AtomsImpl *pimpl;
};

struct AtomsImplNode {
  size_t len;
  size_t hash_val;
  char* s;
  AtomsImplNode* next;
};

class AtomsImpl {
 public:
  const char* intern(const string& s);
  const char* intern(const char* s);
  bool exist(const string& s) const;
  void dump(vector<const char*>& v) const;

  AtomsImpl();
  ~AtomsImpl();

 private:
  vector<AtomsImplNode*> buckets;
  size_t atom_count;
};

AtomsImpl::AtomsImpl() : buckets(256), atom_count(0) {
}

AtomsImpl::~AtomsImpl() {
  vector<AtomsImplNode*>::iterator beg = buckets.begin(), end = buckets.end();
  for (; beg != end; ++beg) {
    AtomsImplNode *head = *beg, *last;
    while (head) {
      last = head;
      head = head->next;
      delete[] last->s;
      delete last;
    }
  }
}

const char* AtomsImpl::intern(const string& s) {
  if (atom_count > buckets.size()) {  //rehash
    size_t new_buckets_size = buckets.size() * 2;
    vector<AtomsImplNode*> new_buckets(new_buckets_size);
    vector<AtomsImplNode*>::iterator beg = buckets.begin(), end = buckets.end();
    for (; beg != end; ++beg) {
      AtomsImplNode *head = *beg, *last;
      while (head) {
        last = head;
        head = head->next;
        size_t idx = last->hash_val % new_buckets_size;
        if (new_buckets[idx]) {
          AtomsImplNode* tail = new_buckets[idx];
          while (tail->next) tail = tail->next;
          tail->next = last;
          last->next = NULL;
        } else {
          new_buckets[idx] = last;
          last->next = NULL;
        }
      }
    }
    buckets.swap(new_buckets);
  }
  size_t hash_val = 7, len = s.size(), hidx = 0;
  while (hidx < len) {
    hash_val = hash_val * 31 + s[hidx++];
  }
  size_t idx = hash_val % buckets.size();
  AtomsImplNode* tail = NULL;  //tricky to reduce insert code
  if (buckets[idx]) {
    AtomsImplNode* head = buckets[idx];
    while (head) {
      if (head->len == len && head->hash_val == hash_val && !strcmp(s.c_str(), head->s))
        return head->s;
      tail = head;
      head = head->next;
    }
  }
  AtomsImplNode* newNode = new AtomsImplNode;
  newNode->len = len;
  newNode->hash_val = hash_val;
  newNode->next = NULL;
  newNode->s = new char[len + 1];
  strcpy(newNode->s, s.c_str());
  if (tail) {
    tail->next = newNode;
  } else {
    buckets[idx] = newNode;
  }
  ++atom_count;
  return newNode->s;
}

void AtomsImpl::dump(vector<const char*>& v) const {
  vector<AtomsImplNode*>::const_iterator beg = buckets.begin(), end = buckets.end();
  for (; beg != end; ++beg)
    for (AtomsImplNode* head = *beg; head; head = head->next) v.push_back(head->s);
}

bool AtomsImpl::exist(const string& s) const {
  size_t hash_val = 7, len = s.size(), hidx = 0;
  while (hidx < len) {
    hash_val = hash_val * 31 + s[hidx++];
  }
  size_t idx = hash_val % buckets.size();
  if (buckets[idx]) {
    AtomsImplNode* head = buckets[idx];
    while (head) {
      if (head->len == len && head->hash_val == hash_val && !strcmp(s.c_str(), head->s))
        return true;
      head = head->next;
    }
  }
  return false;
}

const char* AtomsImpl::intern(const char* s) {
  return intern(string(s));
}

Atoms::Atoms() {
  pimpl = new AtomsImpl;
}
Atoms::~Atoms() {
  delete pimpl;
}
const char* Atoms::intern(const string& s) {
  return pimpl->intern(s);
}
const char* Atoms::intern(const char* s) {
  if (!s) return NULL;
  return pimpl->intern(s);
}
bool Atoms::exist(const string& s) const {
  return pimpl->exist(s);
}

void Atoms::dump(vector<const char*>& v) const {
  pimpl->dump(v);
}
}

namespace ca_context{
  ctags_export_t *ctagsp;
  compile_unit * get_compile_unit(const string &full_path_name); //necessary initialization

  const char* current_buffer;

  compile_unit* current_unit;
  map<string, compile_unit*> units;

  Atoms fn_atoms;
  Atoms sy_atoms;
  Atoms pl_atoms;
  
  struct ctags_comment_t {
    int32_t line;
    string name;
    int32_t pos;
  } last_comment;

compile_unit* get_compile_unit(const string &full_path_name) {
  if (!units.count(full_path_name)) {
    return units[full_path_name] = new compile_unit;
  }
  return units[full_path_name];
}

static void last_comment_clear() {
  last_comment.name = "";
  last_comment.line = -2;
  last_comment.pos = 0;
}

// node 到 comment 不能有其它字符， 那么这个comment 是 node 的
static bool is_valid_comment(int sym_pos, int comment_pos, int diff_line) {
  if (current_buffer) {
    int line = 0;
    const char* s = current_buffer + sym_pos - 1;
    const char* begin = current_buffer + comment_pos;
    for (; s >= begin; --s) {
      if (*s == '\n') {
        ++line;
        if (line >= diff_line) {
          return true;
        }
      } else if (*s == '\r' || *s == ' ' || *s == '\t') {

      } else {
        return false;
      }
    }

    return false;
  }

  return true;
}

// 反向查找 并把注释填充到 comment 中
static void reverse_get_comment(string& comment, const char* end, const char* buffer_begin) {
  const char* s = end-2;
  comment = "/*";
  for (; s >= buffer_begin;--s) {
    if (*s == '*' && s > buffer_begin && *(s-1) == '/') {
      comment += "*/";
      break;
    }
    comment += *s;
  }
  reverse(comment.begin(), comment.end());
}

static void find_comment(tcb_symbol_node& node) {
  if (!current_buffer) return;

  string comment;
  int line = 0;
  const char* s = current_buffer + node.pos - 1;
  for (; s >= current_buffer; --s) {
    if (*s == '\n') {
      ++line;
      continue;
    }

    if (*s == '/' && s > current_buffer && *(s-1) == '*') {
      reverse_get_comment(comment, s, current_buffer);
      node.comment = new comment_t(comment);
      return;
    }

    if (*s == '\r' || *s == ' ' || *s == '\t') {
      continue;
    }

    break;
  }

  // 看看是不是行注释
  const char* line_beg = NULL;
  for (; s >= current_buffer; --s) {
    if (*s == '\n') {
      line_beg = s+1;
      break;
    }
  }
  if (line_beg) {
    for (s = line_beg; *s == ' ' || *s == '\t'; ++s);
    if (*s == '/' && *(s+1) == '/') {
      for (; *s && *s != '\n'; ++s) {
        comment += *s;
      }
      comment += '\n';
      node.comment = new comment_t(comment);
    }
  }
}

static void set_node_comment(tcb_symbol_node& node) {
  int line_diff = node.line - last_comment.line;
  // typedef 真是难处理
  if (node.typeref) {
    /*
    typedef struct _widget_t {

      这里有注释, 不算 widget_t
    } widget_t;

    typedef struct _widget_t {

      
    } 
    这里的注释算
    widget_t;
    */
    if (node.line_content && node.line_content[0] == '}') {
      return;
    }
  }

  if (line_diff < 0) {
    find_comment(node);
  } else if (line_diff < 4 && !last_comment.name.empty() 
    && is_valid_comment(node.pos, last_comment.pos, line_diff)) {
    node.comment = new comment_t(last_comment.name);
  } 
}

void on_ctags_symbol(tcb_symbol_node& node) {
  compile_unit* puint = current_unit;
  set_node_comment(node);
  if (node.comment) {
    puint->comments.push_back(node.comment);
  }
  last_comment_clear();

  puint->symbols.push_back(node);
}

bool append_file_symbols(vector<tcb_symbol_node> &nodes, const string &file) {
  if (!units.count(file)) 
    return false;
  compile_unit* punit = units[file];
  if (!punit) 
    return false;

  nodes.insert(nodes.end(), punit->symbols.begin(), punit->symbols.end());
  return true;
}

void code_assist_symbols_destroy(ca_symbols_t* symbols) {
  if (symbols == NULL) {
    return;
  }

  for (uint32_t i = 0; i < symbols->size; ++i) {
    const char* comment = symbols->nodes[i].comment;
    if (comment) {
      TKMEM_FREE(comment);
    }
  }
  TKMEM_FREE(symbols);
  return;
}

ca_symbols_t* nodes_from_node_vector_n(const vector<tcb_symbol_node>& nodes, size_t n) {
  ca_symbols_t* ret = NULL;
  size_t len = tk_min(nodes.size(), n);
  if (len == 0)
    return NULL;
  
  uint32_t size = len * sizeof(ca_symbol_t) + sizeof(*ret);
  ret = (ca_symbols_t*)TKMEM_ALLOC(size);
  if (ret == NULL) {
    return NULL;
  }
  memset(ret, 0x00, size);
  ret->size = len;
  ret->destroy = code_assist_symbols_destroy;
  size_t i = 0;
  for (auto node : nodes) {
    to_symbol_t(ret->nodes[i++], node);
    if (i == len) {
      break;
    }
  }
  return ret;
}

ca_symbols_t* nodes_from_node_vector(const vector<tcb_symbol_node>& nodes) {
  return nodes_from_node_vector_n(nodes, nodes.size());
}

template<typename M>
void map_delete(M& m) {
  for (auto i : m) {
    delete i.second;
  }
  m.clear();
}
};

using namespace ca_context;

void tcb_ctags_callback(const tagEntryInfo* const tag, char* const line, void* param) {
  // 注释
  if (tag->kind == 'z') {
    last_comment.line = tag->lineNumber;
    last_comment.name = tag->name;
    last_comment.pos = tag->filePosition;
    return;
  }

  tcb_symbol_node node;
  node.name = sy_atoms.intern(tag->name);
  node.file = fn_atoms.intern(tag->sourceFileName);
  node.param = pl_atoms.intern(tag->extensionFields.signature);
  node.scope = sy_atoms.intern(tag->extensionFields.scope[1]);
  node.type = tcb_type_from_ctags_kind(tag->kind);
  node.pos = tag->filePosition;
  node.line = tag->lineNumber;

  if (tag->functionPreffix && node.param && tag->kind != 't') {
    string snapshot = tag->functionPreffix;
    if (tag->kind == 'm')
      snapshot = snapshot + " (*" + tag->name + ")" + node.param;
    else
      snapshot = snapshot + " " + tag->name + node.param;
    node.line_content = fn_atoms.intern(snapshot);
  } else {
    string s = line ? line : "";
    if (!s.empty() && *s.rbegin() == '\n') {
      if (s[s.length()-2] == '\r') {
        s = s.substr(0, s.length()-2);
      } else {
        s = s.substr(0, s.length()-1);
      }
    }
    node.line_content = fn_atoms.intern(s);
  }

  if (tag->kind == 'i') {
    node.name = sy_atoms.intern(tag->name + 1);
  } else if (tag->kind == 't') {
    string typeref;
    if (tag->extensionFields.typeRef[1])
      typeref = tag->extensionFields.typeRef[1];

    node.typeref = sy_atoms.intern(typeref.c_str());
  } else if (tag->kind == 'd' && !tag->extensionFields.signature) {
    if (tag->extensionFields.typeRef[1])
      node.typeref = sy_atoms.intern(tag->extensionFields.typeRef[1]);
  }

  on_ctags_symbol(node);
}

code_assist_t* code_assist_create() {
  code_assist_t* ca = TKMEM_ZALLOC(code_assist_t);
  return_value_if_fail(ca != NULL, NULL);

  if (NULL == ctagsp) {
    // ctags 用了不少全局变量， 改成非全局的， 高投入， 低产出
    ctagsp = get_ctags(tcb_ctags_callback, NULL); //happy case no error
  }

  return ca;
}

ret_t code_assist_destroy(code_assist_t* ca) {
  return_value_if_fail(ca != NULL, RET_BAD_PARAMS);
  
  if (ctagsp) {
    ctagsp->close_ctags();
    ctagsp = NULL;
  }

  map_delete(units);
  TKMEM_FREE(ca);
  return RET_OK;
}

ret_t code_assist_visit_file2(code_assist_t* ca, const char* full_path) {
  return_value_if_fail(ca != NULL && full_path != NULL, RET_BAD_PARAMS);

  if (file_get_size(full_path) > 1024 * 1024) {
    return RET_BAD_PARAMS;
  }

  compile_unit* punit = get_compile_unit(full_path);
  current_unit = punit; // 保存起来，给 ctags 的回调用

  last_comment_clear();
  punit->clear();

  uint32_t len;
  current_buffer = (char*)file_read(full_path, &len); 
  ctagsp->parse_buffer(full_path, current_buffer, len);
  TKMEM_FREE(current_buffer);
  return RET_OK;
}

ca_symbols_t* code_assist_symbols_from_file(code_assist_t* ca, const char* full_path, bool_t sort_) {
  if (is_empty_s(full_path)) 
    return NULL;

  vector<tcb_symbol_node> nodes;
  append_file_symbols(nodes, full_path);
  return nodes_from_node_vector(nodes);
}
