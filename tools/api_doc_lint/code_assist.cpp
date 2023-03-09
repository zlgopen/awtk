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

#define SAFE_FREE(x) if (x) { TKMEM_FREE(x); }
#define CLONE_STR(dest, src) dest = tk_str_copy((char*)dest, src);

struct compile_unit {
  vector<ca_symbol_t> symbols;
  void clear() {
    for (auto sym : symbols) {
      SAFE_FREE(sym.name);
      SAFE_FREE(sym.line_content);
      SAFE_FREE(sym.file);
      SAFE_FREE(sym.param);
      SAFE_FREE(sym.scope);
      SAFE_FREE(sym.typeref);
      SAFE_FREE(sym.comment);
    }
    symbols.clear();
  }

  ~compile_unit() { clear(); }
};

ca_symbol_type_t sym_type_from_ctags_kind(char kind) {
  switch (kind) {
    case 'd': return CA_DEF;
    case 'f': return CA_FUNC_DEF;
    case 'p': return CA_FUNC_PROTOTYPE;
    case 'c': return CA_TYPE;
    case 's': return CA_TYPE;
    case 'u': return CA_TYPE;
    case 'i': return CA_INCLUDE;
    case 'e': return CA_ENUM;
    case 't': return CA_TYPE;
    case 'g': return CA_ENUM;
  }
  return CA_GENERAL;
}

// node 到 comment 不能有其它字符
static bool is_valid_comment(const char* buffer, int sym_pos, int comment_pos, int diff_line) {
  if (buffer) {
    int line = 0;
    const char* s = buffer + sym_pos - 1;
    const char* begin = buffer + comment_pos;
    for (; s >= begin && strchr("\n\r\t ", *s); --s) {
      if (*s == '\n') {
        ++line;
        if (line >= diff_line) {
          return true;
        }
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

static void find_comment(const char* buffer, ca_symbol_t& node) {
  if (!buffer) return;

  const char* s = buffer + node.pos - 1;
  for (; s >= buffer; --s) {
    if (*s == '/' && s > buffer && *(s-1) == '*') {
      string comment;
      reverse_get_comment(comment, s, buffer);
      CLONE_STR(node.comment, comment.c_str());
      return;
    }

    if (!strchr("\n\r\t ", *s)) {
      break;
    }
  }
}

struct ca_impl {
  ctags_export_t *ctagsp;

  const char* current_buffer;

  compile_unit* current_unit;
  map<string, compile_unit*> units;
  
  struct ctags_comment_t {
    int32_t line;
    string name;
    int32_t pos;
  } last_comment;

  ca_impl() : current_buffer(NULL), current_unit(NULL) {
    ctagsp = get_ctags(ca_impl::tcb_ctags_callback, this);
  }

  ~ca_impl() {
    ctagsp->close_ctags();
    for (auto i : units) {
      delete i.second;
    }
  }

  compile_unit* get_unit(const char* full_path) const {
    auto i = units.find(full_path);
    return (i != units.end()) ? i->second : NULL;
  }

  void visit_file(const char* full_path) {
    auto i = units.find(full_path);
    if (i == units.end()) {
      current_unit = units[full_path] = new compile_unit;
    } else {
      current_unit = i->second;
    }

    last_comment_clear();
    current_unit->clear();

    uint32_t len;
    current_buffer = (char*)file_read(full_path, &len); 
    ctagsp->parse_buffer(full_path, current_buffer, len);
    TKMEM_FREE(current_buffer);
  }

  void last_comment_clear() {
    last_comment.name = "";
    last_comment.line = -2;
    last_comment.pos = 0;
  }

  bool append_file_symbols(vector<ca_symbol_t> &nodes, const string &file) {
    if (!units.count(file)) 
      return false;
    compile_unit* punit = units[file];
    if (!punit) 
      return false;

    nodes.insert(nodes.end(), punit->symbols.begin(), punit->symbols.end());
    return true;
  }

  void set_node_comment(ca_symbol_t& node) {
    int line_diff = node.line - last_comment.line;
    if (node.typeref) {
      /*
      typedef struct _widget_t {

        // comment here does not belong to widget_t.
      } widget_t;

      typedef struct _widget_t {
      } 
      // comment here belongs to widget_t
      widget_t;
      */
      if (node.line_content && node.line_content[0] == '}') {
        return;
      }
    }

    if (line_diff < 0) {
      find_comment(current_buffer, node);
    } else if (line_diff < 4 && !last_comment.name.empty() 
      && is_valid_comment(current_buffer, node.pos, last_comment.pos, line_diff)) {
      CLONE_STR(node.comment, last_comment.name.c_str());
    } 
  }

  static void tcb_ctags_callback(const tagEntryInfo* const tag, char* const line, void* param) {
    ca_impl* impl = (ca_impl*)param;
    impl->ctags_callback(tag, line);
  }

  void ctags_callback(const tagEntryInfo* const tag, char* const line) {
    if (tag->kind == 'z') {
      last_comment.line = tag->lineNumber;
      last_comment.name = tag->name;
      last_comment.pos = tag->filePosition;
      return;
    }

    ca_symbol_t node = {CA_GENERAL, 0};
    CLONE_STR(node.name, tag->name);
    CLONE_STR(node.file, tag->sourceFileName);
    CLONE_STR(node.param, tag->extensionFields.signature);
    CLONE_STR(node.scope, tag->extensionFields.scope[1]);
    node.type = sym_type_from_ctags_kind(tag->kind);
    node.pos = tag->filePosition;
    node.line = tag->lineNumber;

    if (tag->functionPreffix && node.param && tag->kind != 't') {
      string snapshot = tag->functionPreffix;
      if (tag->kind == 'm')
        snapshot = snapshot + " (*" + tag->name + ")" + node.param;
      else
        snapshot = snapshot + " " + tag->name + node.param;
      CLONE_STR(node.line_content, snapshot.c_str());
    } else {
      string s = line ? line : "";
      /* ctags 已经把 \r 过滤掉了 */
      if (!s.empty() && *s.rbegin() == '\n') {
        s.pop_back();
      }
      CLONE_STR(node.line_content, s.c_str());
    }

    if (tag->kind == 'i') {
      CLONE_STR(node.name, tag->name + 1);
    } else if (tag->kind == 't') {
      CLONE_STR(node.typeref, tag->extensionFields.typeRef[1]);
    } else if (tag->kind == 'd' && !tag->extensionFields.signature) {
      if (tag->extensionFields.typeRef[1])
        CLONE_STR(node.typeref, tag->extensionFields.typeRef[1]);
    }

    set_node_comment(node);
    last_comment_clear();

    current_unit->symbols.push_back(node);
  }
};

void code_assist_symbols_destroy(ca_symbols_t* symbols) {
  SAFE_FREE(symbols);
}

static ca_symbols_t* nodes_from_node_vector(const vector<ca_symbol_t>& nodes) {
  ca_symbols_t* ret = NULL;
  size_t len = nodes.size();
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
    memcpy(&ret->nodes[i++], &node, sizeof(node));
    if (i == len) {
      break;
    }
  }
  return ret;
}

code_assist_t* code_assist_create() {
  code_assist_t* ca = TKMEM_ZALLOC(code_assist_t);
  return_value_if_fail(ca != NULL, NULL);

  ca->impl = new ca_impl();
  return ca;
}

ret_t code_assist_destroy(code_assist_t* ca) {
  return_value_if_fail(ca != NULL, RET_BAD_PARAMS);

  delete (ca_impl*)ca->impl;
  TKMEM_FREE(ca);
  return RET_OK;
}

ret_t code_assist_visit_file2(code_assist_t* ca, const char* full_path) {
  return_value_if_fail(ca != NULL && full_path != NULL, RET_BAD_PARAMS);

  if (file_get_size(full_path) > 1024 * 1024) {
    return RET_BAD_PARAMS;
  }

  ca_impl* impl = (ca_impl*)ca->impl;
  impl->visit_file(full_path);
  return RET_OK;
}

ca_symbols_t* code_assist_symbols_from_file(code_assist_t* ca, const char* full_path, bool_t sort_) {
  return_value_if_fail(ca != NULL && full_path != NULL, NULL);

  ca_impl* impl = (ca_impl*)ca->impl;
  auto unit = impl->get_unit(full_path);
  return nodes_from_node_vector(unit->symbols);
}
