﻿/**
 * File:   conf_node.c
 * Author: AWTK Develop Team
 * Brief:  conf node
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
 * 2020-05-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_array.h"
#include "conf_io/conf_node.h"
#include "conf_node_obj.inc"

static ret_t conf_node_destroy(conf_doc_t* doc, conf_node_t* node);

conf_node_t* conf_node_get_first_child(conf_node_t* node) {
  return_value_if_fail(node != NULL, NULL);
  if (node->value_type != CONF_NODE_VALUE_NODE && node->value_type != CONF_NODE_VALUE_NONE) {
    return NULL;
  }

  return node->value.first_child;
}

ret_t conf_node_set_first_child(conf_node_t* node, conf_node_t* child) {
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);
  node->value_type = CONF_NODE_VALUE_NODE;
  node->value.first_child = child;

  if (child != NULL) {
    child->parent = node;
  }

  return RET_OK;
}

conf_doc_t* conf_doc_create(uint32_t prealloc_nodes_nr) {
  uint32_t size = prealloc_nodes_nr * sizeof(conf_node_t);
  conf_doc_t* doc = TKMEM_ZALLOC(conf_doc_t);
  return_value_if_fail(doc != NULL, NULL);

  if (prealloc_nodes_nr > 0) {
    doc->prealloc_nodes = TKMEM_ALLOC(size);
    if (doc->prealloc_nodes == NULL) {
      TKMEM_FREE(doc);
      return NULL;
    } else {
      memset(doc->prealloc_nodes, 0x00, size);
      doc->prealloc_nodes_nr = prealloc_nodes_nr;
    }
  }
  doc->max_deep_level = 0xffff;
  tokenizer_init(&(doc->tokenizer), "", 0, ".");

  return doc;
}

static conf_node_t* conf_doc_find_free_node(conf_doc_t* doc) {
  uint32_t i = 0;
  return_value_if_fail(doc != NULL, NULL);
  if (doc->prealloc_nodes_used >= doc->prealloc_nodes_nr) {
    return NULL;
  }

  for (i = doc->prealloc_nodes_index; i < doc->prealloc_nodes_nr; i++) {
    conf_node_t* iter = doc->prealloc_nodes + i;
    if (iter->node_type == CONF_NODE_NONE) {
      doc->prealloc_nodes_index = i;
      doc->prealloc_nodes_used++;
      return iter;
    }
  }

  for (i = 0; i < doc->prealloc_nodes_index; i++) {
    conf_node_t* iter = doc->prealloc_nodes + i;
    if (iter->node_type == CONF_NODE_NONE) {
      doc->prealloc_nodes_index = i;
      doc->prealloc_nodes_used++;
      return iter;
    }
  }

  return NULL;
}

conf_node_t* conf_doc_create_node(conf_doc_t* doc, const char* name) {
  conf_node_t* node = NULL;
  return_value_if_fail(doc != NULL, NULL);
  node = conf_doc_find_free_node(doc);

  if (node != NULL) {
    memset(node, 0x00, sizeof(conf_node_t));
  } else {
    node = TKMEM_ZALLOC(conf_node_t);
  }
  return_value_if_fail(node != NULL, NULL);

  node->node_type = CONF_NODE_OBJECT;
  if (name != NULL) {
    uint32_t size = strlen(name);
    if (size < sizeof(node->name)) {
      node->is_small_name = TRUE;
      strcpy(node->name.small_str, name);
    } else {
      node->is_small_name = FALSE;
      node->name.str = tk_strdup(name);
    }
  }

  return node;
}

static conf_node_t* conf_doc_dup_node_impl(conf_doc_t* doc, conf_node_t* node,
                                           const char* new_name) {
  conf_node_t* new_node = NULL;
  return_value_if_fail(doc != NULL && node != NULL, NULL);

  if (new_name == NULL) {
    new_name = conf_node_get_name(node);
  }
  new_node = conf_doc_create_node(doc, new_name);
  return_value_if_fail(new_node != NULL, NULL);

  new_node->value_type = node->value_type;
  new_node->node_type = node->node_type;
  if (node->value_type == CONF_NODE_VALUE_NODE) {
    conf_node_t* iter = conf_node_get_first_child(node);
    while (iter != NULL) {
      conf_node_t* new_iter = conf_doc_dup_node_impl(doc, iter, NULL);
      break_if_fail(new_iter != NULL);
      conf_doc_append_child(doc, new_node, new_iter);

      iter = iter->next;
    }
  } else if (node->value_type != CONF_NODE_VALUE_NONE) {
    value_t v;
    if (conf_node_get_value(node, &v) == RET_OK) {
      conf_node_set_value(new_node, &v);
    }
  }

  return new_node;
}

conf_node_t* conf_doc_dup_node(conf_doc_t* doc, conf_node_t* node, const char* new_name) {
  conf_node_t* new_node = NULL;
  return_value_if_fail(doc != NULL && node != NULL, NULL);
  new_node = conf_doc_dup_node_impl(doc, node, new_name);
  return_value_if_fail(new_node != NULL, NULL);

  new_node->next = node->next;
  node->next = new_node;
  new_node->parent = node->parent;

  return new_node;
}

ret_t conf_doc_set_node_prop(conf_doc_t* doc, conf_node_t* node, const char* name,
                             const value_t* v) {
  conf_node_t* child = NULL;
  return_value_if_fail(doc != NULL && node != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && v != NULL, RET_BAD_PARAMS);

  child = conf_node_find_child(node, name);
  if (child == NULL) {
    child = conf_doc_create_node(doc, name);
    return_value_if_fail(child != NULL, RET_BAD_PARAMS);

    conf_node_set_value(child, v);
    return conf_doc_append_child(doc, node, child);
  } else {
    return conf_node_set_value(child, v);
  }
}

static conf_node_obj_t* conf_doc_obj_array_find(conf_doc_t* doc, conf_node_t* node) {
  conf_node_obj_t* ret = NULL;
  uint32_t i = 0;
  return_value_if_fail(doc != NULL && node != NULL, NULL);
  return_value_if_fail(OBJECT_ARRAY(doc->obj_array) != NULL, NULL);

  for (i = 0; i < OBJECT_ARRAY(doc->obj_array)->size; i++) {
    value_t iter;
    if (RET_OK == object_array_get(doc->obj_array, i, &iter)) {
      conf_node_obj_t* iter_obj = CONF_NODE_OBJ(value_object(&iter));
      assert(iter_obj != NULL);
      if (iter_obj->node == node && iter_obj->doc == doc) {
        ret = iter_obj;
        break;
      }
    }
  }
  return ret;
}

ret_t conf_doc_destroy_node(conf_doc_t* doc, conf_node_t* node) {
  return_value_if_fail(doc != NULL && node != NULL, RET_BAD_PARAMS);

  if (doc->use_extend_type) {
    conf_node_obj_t* obj = conf_doc_obj_array_find(doc, node);
    if (obj != NULL) {
      value_t tmp;
      obj->doc = NULL;
      obj->node = NULL;
      object_array_remove_value(doc->obj_array, value_set_object(&tmp, TK_OBJECT(obj)));
    }
  }

  if (!node->is_small_name) {
    TKMEM_FREE(node->name.str);
  }

  if (node->value_type == CONF_NODE_VALUE_STRING) {
    TKMEM_FREE(node->value.str);
  } else if (node->value_type == CONF_NODE_VALUE_WSTRING) {
    TKMEM_FREE(node->value.wstr);
  } else if (node->value_type == CONF_NODE_VALUE_BINARY) {
    TKMEM_FREE(node->value.binary_data.data);
  }

  memset(node, 0x00, sizeof(*node));

  if (node >= doc->prealloc_nodes && node < (doc->prealloc_nodes + doc->prealloc_nodes_nr)) {
    node->node_type = CONF_NODE_NONE;
    doc->prealloc_nodes_used--;

    return RET_OK;
  }

  TKMEM_FREE(node);

  return RET_OK;
}

ret_t conf_doc_append_sibling(conf_doc_t* doc, conf_node_t* node, conf_node_t* sibling) {
  conf_node_t* iter = node;
  return_value_if_fail(doc != NULL && node != NULL && sibling != NULL, RET_BAD_PARAMS);

  sibling->parent = node->parent;
  while (iter->next != NULL) iter = iter->next;
  iter->next = sibling;

  return RET_OK;
}

ret_t conf_doc_append_child(conf_doc_t* doc, conf_node_t* node, conf_node_t* child) {
  conf_node_t* first_child = conf_node_get_first_child(node);
  return_value_if_fail(doc != NULL && node != NULL && child != NULL, RET_BAD_PARAMS);

  if (first_child == NULL) {
    conf_node_set_first_child(node, child);
    return RET_OK;
  } else {
    return conf_doc_append_sibling(doc, first_child, child);
  }
}

ret_t conf_doc_remove_sibling(conf_doc_t* doc, conf_node_t* node, conf_node_t* sibling) {
  conf_node_t* iter = node;
  return_value_if_fail(doc != NULL && node != NULL && sibling != NULL, RET_BAD_PARAMS);

  if (node == sibling) {
    return conf_doc_remove_child(doc, node->parent, node);
  } else {
    while (iter->next != NULL) {
      if (iter->next == sibling) {
        iter->next = sibling->next;
        sibling->next = NULL;
        conf_doc_destroy_node(doc, sibling);
        return RET_OK;
      }
      iter = iter->next;
    }
  }

  return RET_NOT_FOUND;
}

ret_t conf_doc_remove_child(conf_doc_t* doc, conf_node_t* node, conf_node_t* child) {
  conf_node_t* first_child = conf_node_get_first_child(node);
  return_value_if_fail(doc != NULL && node != NULL && child != NULL, RET_BAD_PARAMS);

  if (first_child == child) {
    conf_node_set_first_child(node, child->next);
    conf_doc_destroy_node(doc, child);

    return RET_OK;
  } else {
    return conf_doc_remove_sibling(doc, first_child, child);
  }
}

ret_t conf_doc_remove_child_by_name(conf_doc_t* doc, conf_node_t* node, const char* name) {
  conf_node_t* iter = NULL;
  conf_node_t* prev = NULL;
  conf_node_t* first_child = conf_node_get_first_child(node);
  return_value_if_fail(doc != NULL && node != NULL, RET_BAD_PARAMS);

  iter = first_child;
  prev = first_child;
  while (iter != NULL) {
    if (tk_str_eq(conf_node_get_name(iter), name)) {
      if (iter == first_child) {
        conf_node_set_first_child(node, iter->next);
      } else {
        prev->next = iter->next;
      }
      conf_doc_destroy_node(doc, iter);

      return RET_OK;
    }
    prev = iter;
    iter = iter->next;
  }

  return RET_NOT_FOUND;
}

ret_t conf_doc_remove_children(conf_doc_t* doc, conf_node_t* node) {
  conf_node_t* iter = NULL;
  conf_node_t* first_child = conf_node_get_first_child(node);
  return_value_if_fail(doc != NULL && node != NULL, RET_BAD_PARAMS);

  if (first_child == NULL) {
    return RET_OK;
  }

  iter = first_child;
  return_value_if_fail(iter != NULL, RET_BAD_PARAMS);
  conf_node_set_first_child(node, NULL);

  while (iter != NULL) {
    conf_node_t* next = iter->next;

    conf_doc_remove_children(doc, iter);
    conf_doc_destroy_node(doc, iter);
    iter = next;
  }

  return RET_OK;
}

static ret_t conf_node_destroy(conf_doc_t* doc, conf_node_t* node) {
  conf_node_t* iter = node;
  return_value_if_fail(doc != NULL && node != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    conf_node_t* next = iter->next;
    conf_doc_remove_children(doc, iter);
    conf_doc_destroy_node(doc, iter);
    iter = next;
  }

  return RET_OK;
}

const char* conf_node_get_name(conf_node_t* node) {
  return_value_if_fail(node != NULL, NULL);

  return node->is_small_name ? node->name.small_str : node->name.str;
}

uint32_t conf_node_count_children(conf_node_t* node) {
  uint32_t i = 0;
  conf_node_t* first = conf_node_get_first_child(node);
  return_value_if_fail(node != NULL, 0);

  while (first != NULL) {
    i++;
    first = first->next;
  }

  return i;
}

static int32_t conf_node_get_index(conf_node_t* node) {
  uint32_t i = 0;
  conf_node_t* iter = NULL;
  return_value_if_fail(node != NULL, 0);
  iter = conf_node_get_first_child(node->parent);

  while (iter != NULL) {
    if (iter == node) {
      return i;
    }
    i++;
    iter = iter->next;
  }
  assert(!"impossible");
  return -1;
}

conf_node_t* conf_node_find_sibling(conf_node_t* node, const char* name) {
  conf_node_t* iter = NULL;
  if (node == NULL) {
    return NULL;
  }

  return_value_if_fail(name != NULL, NULL);

  if (node->parent != NULL) {
    iter = conf_node_get_first_child(node->parent);
  } else {
    iter = node;
  }
  while (iter != NULL) {
    if (tk_str_eq(conf_node_get_name(iter), name)) {
      return iter;
    }

    iter = iter->next;
  }

  return NULL;
}

conf_node_t* conf_node_find_sibling_by_index(conf_node_t* node, int32_t index) {
  int32_t i = 0;
  conf_node_t* iter = NULL;
  return_value_if_fail(node != NULL && index >= 0, NULL);

  if (node->parent != NULL) {
    iter = conf_node_get_first_child(node->parent);
  } else {
    iter = node;
  }

  for (i = 0; i < index && iter != NULL; i++) {
    iter = iter->next;
  }

  return iter;
}

conf_node_t* conf_node_find_child(conf_node_t* node, const char* name) {
  conf_node_t* first_child = conf_node_get_first_child(node);
  return_value_if_fail(node != NULL && name != NULL, NULL);

  return conf_node_find_sibling(first_child, name);
}

conf_node_t* conf_node_find_child_by_index(conf_node_t* node, int32_t index) {
  conf_node_t* first_child = conf_node_get_first_child(node);
  return_value_if_fail(node != NULL, NULL);

  if (first_child == NULL) {
    return NULL;
  }

  return conf_node_find_sibling_by_index(first_child, index);
}

ret_t conf_doc_destroy(conf_doc_t* doc) {
  return_value_if_fail(doc != NULL, RET_BAD_PARAMS);

  if (doc->root != NULL) {
    conf_node_destroy(doc, doc->root);
    doc->root = NULL;
  }
  tokenizer_deinit(&(doc->tokenizer));
  TKMEM_FREE(doc->prealloc_nodes);
  TK_OBJECT_UNREF(doc->obj_array);

  TKMEM_FREE(doc);

  return RET_OK;
}

ret_t conf_node_set_value(conf_node_t* node, const value_t* v) {
  return_value_if_fail(node != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(node->value_type != CONF_NODE_VALUE_NODE, RET_BAD_PARAMS);

  if (node->value_type == CONF_NODE_VALUE_STRING) {
    if (v->type == VALUE_TYPE_STRING) {
      const char* p = value_str(v);
      if (node->value.str == p && p != NULL) {
        return RET_OK;
      }
    }
    TKMEM_FREE(node->value.str);
  } else if (node->value_type == CONF_NODE_VALUE_SMALL_STR) {
    if (v->type == VALUE_TYPE_STRING) {
      const char* p = value_str(v);
      if (node->value.small_str == p && p != NULL) {
        return RET_OK;
      }
    }
  } else if (node->value_type == CONF_NODE_VALUE_WSTRING) {
    if (v->type == VALUE_TYPE_WSTRING) {
      const wchar_t* p = value_wstr(v);
      if (node->value.wstr == p && p != NULL) {
        return RET_OK;
      }
    }
    TKMEM_FREE(node->value.wstr);
  } else if (node->value_type == CONF_NODE_VALUE_BINARY) {
    if (v->type == VALUE_TYPE_BINARY) {
      if (node->value.binary_data.data == v->value.binary_data.data &&
          node->value.binary_data.size == v->value.binary_data.size) {
        return RET_OK;
      }
    }
    TKMEM_FREE(node->value.binary_data.data);
  }

  switch (v->type) {
    case VALUE_TYPE_BOOL: {
      node->value_type = CONF_NODE_VALUE_BOOL;
      node->value.b = value_bool(v);
      break;
    }
    case VALUE_TYPE_INT8: {
      node->value_type = CONF_NODE_VALUE_INT8;
      node->value.i8 = value_int8(v);
      break;
    }
    case VALUE_TYPE_UINT8: {
      node->value_type = CONF_NODE_VALUE_UINT8;
      node->value.u8 = value_uint8(v);
      break;
    }
    case VALUE_TYPE_INT16: {
      node->value_type = CONF_NODE_VALUE_INT16;
      node->value.i16 = value_int16(v);
      break;
    }
    case VALUE_TYPE_UINT16: {
      node->value_type = CONF_NODE_VALUE_UINT16;
      node->value.u16 = value_uint16(v);
      break;
    }
    case VALUE_TYPE_INT32: {
      node->value_type = CONF_NODE_VALUE_INT32;
      node->value.i32 = value_int32(v);
      break;
    }
    case VALUE_TYPE_UINT32: {
      node->value_type = CONF_NODE_VALUE_UINT32;
      node->value.u32 = value_uint32(v);
      break;
    }
    case VALUE_TYPE_INT64: {
      node->value_type = CONF_NODE_VALUE_INT64;
      node->value.i64 = value_int64(v);
      break;
    }
    case VALUE_TYPE_UINT64: {
      node->value_type = CONF_NODE_VALUE_UINT64;
      node->value.u64 = value_uint64(v);
      break;
    }
    case VALUE_TYPE_POINTER: {
      node->value_type = CONF_NODE_VALUE_POINTER;
      node->value.u64 = tk_pointer_to_long(value_pointer(v));
      break;
    }
    case VALUE_TYPE_FLOAT:
    case VALUE_TYPE_FLOAT32: {
      node->value_type = CONF_NODE_VALUE_FLOAT32;
      node->value.f32 = value_float32(v);
      break;
    }
    case VALUE_TYPE_DOUBLE: {
      node->value_type = CONF_NODE_VALUE_DOUBLE;
      node->value.f64 = value_double(v);
      break;
    }
    case VALUE_TYPE_STRING: {
      const char* str = value_str(v);
      if (str == NULL) {
        node->value_type = CONF_NODE_VALUE_STRING;
        node->value.str = NULL;
      } else {
        if (strlen(str) < sizeof(node->value.small_str)) {
          node->value_type = CONF_NODE_VALUE_SMALL_STR;
          strcpy(node->value.small_str, str);
        } else {
          node->value_type = CONF_NODE_VALUE_STRING;
          node->value.str = tk_strdup(str);
          return_value_if_fail(node->value.str != NULL, RET_OOM);
        }
      }
      break;
    }
    case VALUE_TYPE_WSTRING: {
      const wchar_t* str = value_wstr(v);
      node->value_type = CONF_NODE_VALUE_WSTRING;
      if (str == NULL) {
        node->value.wstr = NULL;
      } else {
        node->value.wstr = tk_wstrdup(str);
        return_value_if_fail(node->value.wstr != NULL, RET_OOM);
      }
      break;
    }
    case VALUE_TYPE_BINARY: {
      binary_data_t* bdata = value_binary_data(v);
      node->value_type = CONF_NODE_VALUE_BINARY;
      if (bdata == NULL) {
        node->value.binary_data.data = NULL;
        node->value.binary_data.size = 0;
      } else {
        node->value.binary_data.data = tk_memdup(bdata->data, bdata->size);
        return_value_if_fail(node->value.binary_data.data != NULL, RET_OOM);
        node->value.binary_data.size = bdata->size;
      }

      node->node_type = CONF_NODE_ARRAY_UINT8;
      return RET_OK;
    }
    default: {
      return RET_NOT_IMPL;
    }
  }
  node->node_type = CONF_NODE_SIMPLE;

  return RET_OK;
}

ret_t conf_node_get_value(conf_node_t* node, value_t* v) {
  return_value_if_fail(node != NULL && v != NULL, RET_BAD_PARAMS);

  switch (node->value_type) {
    case CONF_NODE_VALUE_BOOL: {
      value_set_bool(v, node->value.b);
      break;
    }
    case CONF_NODE_VALUE_INT8: {
      value_set_int8(v, node->value.i8);
      break;
    }
    case CONF_NODE_VALUE_UINT8: {
      value_set_uint8(v, node->value.u8);
      break;
    }
    case CONF_NODE_VALUE_INT16: {
      value_set_int16(v, node->value.i16);
      break;
    }
    case CONF_NODE_VALUE_UINT16: {
      value_set_uint16(v, node->value.u16);
      break;
    }
    case CONF_NODE_VALUE_INT32: {
      value_set_int32(v, node->value.i32);
      break;
    }
    case CONF_NODE_VALUE_UINT32: {
      value_set_uint32(v, node->value.u32);
      break;
    }
    case CONF_NODE_VALUE_INT64: {
      value_set_int64(v, node->value.i64);
      break;
    }
    case CONF_NODE_VALUE_UINT64: {
      value_set_uint64(v, node->value.u64);
      break;
    }
    case CONF_NODE_VALUE_POINTER: {
      value_set_pointer(v, tk_pointer_from_long(node->value.u64));
      break;
    }
    case CONF_NODE_VALUE_FLOAT32: {
      value_set_float32(v, node->value.f32);
      break;
    }
    case CONF_NODE_VALUE_DOUBLE: {
      value_set_double(v, node->value.f64);
      break;
    }
    case CONF_NODE_VALUE_STRING: {
      value_set_str(v, node->value.str);
      break;
    }
    case CONF_NODE_VALUE_WSTRING: {
      value_set_wstr(v, node->value.wstr);
      break;
    }
    case CONF_NODE_VALUE_SMALL_STR: {
      value_set_str(v, node->value.small_str);
      break;
    }
    case CONF_NODE_VALUE_BINARY: {
      value_set_binary_data(v, node->value.binary_data.data, node->value.binary_data.size);
      break;
    }
    default: {
      return RET_NOT_IMPL;
    }
  }

  return RET_OK;
}

int32_t conf_node_get_value_int32(conf_node_t* node, int32_t defval) {
  value_t v;
  if (conf_node_get_value(node, &v) == RET_OK) {
    return value_int32(&v);
  } else {
    return defval;
  }
}

uint32_t conf_node_get_value_uint32(conf_node_t* node, uint32_t defval) {
  value_t v;
  if (conf_node_get_value(node, &v) == RET_OK) {
    return value_uint32(&v);
  } else {
    return defval;
  }
}

float conf_node_get_value_float(conf_node_t* node, float defval) {
  value_t v;
  if (conf_node_get_value(node, &v) == RET_OK) {
    return value_float(&v);
  } else {
    return defval;
  }
}

double conf_node_get_value_double(conf_node_t* node, double defval) {
  value_t v;
  if (conf_node_get_value(node, &v) == RET_OK) {
    return value_double(&v);
  } else {
    return defval;
  }
}

bool_t conf_node_get_value_bool(conf_node_t* node, bool_t defval) {
  value_t v;
  if (conf_node_get_value(node, &v) == RET_OK) {
    return value_bool(&v);
  } else {
    return defval;
  }
}

const char* conf_node_get_value_str(conf_node_t* node, const char* defval) {
  value_t v;
  if (conf_node_get_value(node, &v) == RET_OK) {
    return value_str(&v);
  } else {
    return defval;
  }
}

static tokenizer_t* conf_doc_get_tokenizer(conf_doc_t* doc, const char* path) {
  tokenizer_t* tokenizer = &(doc->tokenizer);
  tokenizer->str = path;
  tokenizer->cursor = 0;
  tokenizer->size = strlen(path);

  return tokenizer;
}

static conf_node_t* conf_doc_get_node(conf_doc_t* doc, const char* path,
                                      bool_t create_if_not_exist) {
  return_value_if_fail(doc != NULL && path != NULL, NULL);

  return conf_doc_find_node(doc, doc->root, path, create_if_not_exist);
}

conf_node_t* conf_doc_find_node(conf_doc_t* doc, conf_node_t* node, const char* path,
                                bool_t create_if_not_exist) {
  uint32_t deep = 1;
  const char* token = NULL;
  conf_node_t* iter = NULL;
  tokenizer_t* t = conf_doc_get_tokenizer(doc, path);
  return_value_if_fail(t != NULL, NULL);
  if (node == NULL) {
    node = doc->root;
  }
  return_value_if_fail(node != NULL, NULL);

  while (tokenizer_has_more(t)) {
    if (deep < doc->max_deep_level) {
      token = tokenizer_next(t);
    } else {
      token = t->str + t->cursor;
      t->cursor = t->size;
    }

    if (*token == '[') {
      int32_t index = tk_atoi(token + 1);
      iter = conf_node_find_child_by_index(node, index);
      if (iter == NULL) {
        if (index < 0) {
          /*append*/
          assert(node->node_type == CONF_NODE_ARRAY);
        } else if (index == conf_node_count_children(node)) {
          if (index == 0) {
            node->node_type = CONF_NODE_ARRAY;
          }
          /*append*/
        } else {
          /*node must be exist if find by index */
          return_value_if_fail(iter != NULL, NULL);
        }
      }
    } else if (*token == '#') {
      return node;
    } else {
      iter = conf_node_find_child(node, token);
    }

    if (iter == NULL) {
      if (create_if_not_exist) {
        iter = conf_doc_create_node(doc, token);
        return_value_if_fail(iter != NULL, NULL);
        conf_doc_append_child(doc, node, iter);
        if (!tokenizer_has_more(t)) {
          return iter;
        } else {
          node = iter;
        }
      } else {
        return NULL;
      }
    } else {
      if (tokenizer_has_more(t)) {
        node = iter;
      } else {
        return iter;
      }
    }

    deep++;
  }

  return NULL;
}

typedef struct _conf_doc_set_extend_type_object_prop_ctx_t {
  conf_doc_t* doc;
  const char* path;
} conf_doc_set_extend_type_object_prop_ctx_t;

static ret_t conf_doc_set_extend_type_object_prop(void* ctx, const void* data) {
  ret_t ret = RET_OK;
  conf_doc_set_extend_type_object_prop_ctx_t* actx = ctx;
  const named_value_t* nv = (const named_value_t*)data;
  str_t path;
  str_init(&path, TK_NAME_LEN + 1);
  str_append(&path, actx->path);
  str_append(&path, actx->doc->tokenizer.separtor);
  str_append(&path, nv->name);

  ret = conf_doc_set(actx->doc, path.str, &nv->value);

  str_reset(&path);
  return ret;
}

static ret_t conf_doc_set_extend_type(conf_doc_t* doc, conf_node_t* node, const char* path,
                                      const value_t* v) {
  ret_t ret = RET_NOT_IMPL;
  return_value_if_fail(doc != NULL && node != NULL && path != NULL && v != NULL, RET_BAD_PARAMS);

  if (v->type == VALUE_TYPE_OBJECT) {
    tk_object_t* obj = value_object(v);
    if (tk_object_is_collection(obj)) {
      node->node_type = CONF_NODE_ARRAY;
    } else {
      node->node_type = CONF_NODE_OBJECT;
    }
    node->value_type = CONF_NODE_VALUE_NODE;

    conf_doc_set_extend_type_object_prop_ctx_t ctx = {.doc = doc, .path = path};
    ret = tk_object_foreach_prop(obj, conf_doc_set_extend_type_object_prop, &ctx);
  }

  return ret;
}

ret_t conf_doc_set_ex(conf_doc_t* doc, conf_node_t* node, const char* path, const value_t* v) {
  return_value_if_fail(doc != NULL && path != NULL && v != NULL, RET_BAD_PARAMS);

  if (doc->root == NULL) {
    doc->root = conf_doc_create_node(doc, CONF_NODE_ROOT_NAME);
  }

  node = conf_doc_find_node(doc, node, path, TRUE);

  if (node != NULL) {
    ret_t ret = conf_node_set_value(node, v);
    if (RET_NOT_IMPL == ret && doc->use_extend_type) {
      ret = conf_doc_set_extend_type(doc, node, path, v);
    }
    return ret;
  } else {
    return RET_OOM;
  }
}

ret_t conf_doc_set(conf_doc_t* doc, const char* path, const value_t* v) {
  return_value_if_fail(doc != NULL && path != NULL && v != NULL, RET_BAD_PARAMS);

  return conf_doc_set_ex(doc, doc->root, path, v);
}

ret_t conf_doc_get(conf_doc_t* doc, const char* path, value_t* v) {
  return_value_if_fail(doc != NULL && path != NULL && v != NULL, RET_BAD_PARAMS);

  return conf_doc_get_ex(doc, doc->root, path, v);
}

ret_t conf_doc_get_value_extend_type(conf_doc_t* doc, conf_node_t* node, value_t* v) {
  return_value_if_fail(doc != NULL && node != NULL && v != NULL, RET_BAD_PARAMS);

  switch (node->value_type) {
    case CONF_NODE_VALUE_NODE: {
      switch (node->node_type) {
        case CONF_NODE_OBJECT:
        case CONF_NODE_ARRAY: {
          tk_object_t* obj = TK_OBJECT(conf_doc_obj_array_find(doc, node));
          if (obj == NULL) {
            value_t tmp;
            obj = conf_node_obj_create(doc, node);
            object_array_push(doc->obj_array, value_set_object(&tmp, obj));
            tk_object_unref(obj);
          }
          value_set_object(v, obj);
        } break;
        default: {
          return RET_NOT_IMPL;
        }
      }
      break;
    }
    default: {
      return RET_NOT_IMPL;
    }
  }

  return RET_OK;
}

ret_t conf_doc_get_ex(conf_doc_t* doc, conf_node_t* node, const char* path, value_t* v) {
  return_value_if_fail(doc != NULL && path != NULL && v != NULL, RET_BAD_PARAMS);

  node = conf_doc_find_node(doc, node, path, FALSE);

  if (node != NULL) {
    const char* special = strchr(path, '#');
    if (special == NULL) {
      ret_t ret = conf_node_get_value(node, v);
      if (RET_NOT_IMPL == ret && doc->use_extend_type) {
        ret = conf_doc_get_value_extend_type(doc, node, v);
      }
      return ret;
    } else if (tk_str_eq(special, CONF_SPECIAL_ATTR_NAME)) {
      value_set_str(v, conf_node_get_name(node));
      return RET_OK;
    } else if (tk_str_eq(special, CONF_SPECIAL_ATTR_SIZE)) {
      value_set_uint32(v, conf_node_count_children(node));
      return RET_OK;
    } else if (tk_str_eq(special, CONF_SPECIAL_ATTR_INDEX)) {
      value_set_uint32(v, conf_node_get_index(node));
      return RET_OK;
    }
  }
  value_set_int(v, 0);
  return RET_NOT_FOUND;
}

ret_t conf_doc_remove(conf_doc_t* doc, const char* path) {
  conf_node_t* node = NULL;
  return_value_if_fail(doc != NULL && path != NULL, RET_BAD_PARAMS);

  node = conf_doc_get_node(doc, path, FALSE);

  if (node != NULL) {
    conf_doc_remove_children(doc, node);
    return conf_doc_remove_child(doc, node->parent, node);
  } else {
    return RET_NOT_FOUND;
  }
}

ret_t conf_doc_clear(conf_doc_t* doc, const char* path) {
  conf_node_t* node = NULL;
  return_value_if_fail(doc != NULL && path != NULL, RET_BAD_PARAMS);

  node = conf_doc_get_node(doc, path, FALSE);

  if (node != NULL) {
    return conf_doc_remove_children(doc, node);
  } else {
    return RET_NOT_FOUND;
  }
}

static conf_node_t* conf_node_get_prev(conf_node_t* node) {
  conf_node_t* iter = NULL;
  return_value_if_fail(node != NULL && node->parent != NULL, NULL);
  iter = conf_node_get_first_child(node->parent);

  if (iter == node || iter == NULL) {
    return NULL;
  }

  while (iter->next != NULL) {
    if (iter->next == node) {
      return iter;
    }
    iter = iter->next;
  }

  return NULL;
}

bool_t conf_doc_is_first(conf_doc_t* doc, const char* path) {
  conf_node_t* node = NULL;
  return_value_if_fail(doc != NULL && path != NULL, FALSE);

  node = conf_doc_get_node(doc, path, FALSE);
  return_value_if_fail(node != NULL, FALSE);

  return conf_node_get_first_child(node->parent) == node;
}

bool_t conf_doc_is_last(conf_doc_t* doc, const char* path) {
  conf_node_t* node = NULL;
  return_value_if_fail(doc != NULL && path != NULL, FALSE);

  node = conf_doc_get_node(doc, path, FALSE);
  return_value_if_fail(node != NULL, FALSE);

  return node->next == NULL;
}

ret_t conf_doc_move_up(conf_doc_t* doc, const char* path) {
  conf_node_t* node = NULL;
  return_value_if_fail(doc != NULL && path != NULL, RET_BAD_PARAMS);

  node = conf_doc_get_node(doc, path, FALSE);
  if (node != NULL) {
    conf_node_t* pprev = NULL;
    conf_node_t* prev = conf_node_get_prev(node);

    if (prev == NULL) {
      return RET_FAIL;
    }

    pprev = conf_node_get_prev(prev);

    prev->next = node->next;
    node->next = prev;

    if (pprev != NULL) {
      pprev->next = node;
    } else {
      conf_node_set_first_child(node->parent, node);
    }

    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

ret_t conf_doc_move_down(conf_doc_t* doc, const char* path) {
  conf_node_t* node = NULL;
  return_value_if_fail(doc != NULL && path != NULL, RET_BAD_PARAMS);

  node = conf_doc_get_node(doc, path, FALSE);
  if (node != NULL) {
    conf_node_t* prev = conf_node_get_prev(node);
    conf_node_t* next = node->next;

    if (next == NULL) {
      return RET_FAIL;
    }

    node->next = next->next;
    next->next = node;

    if (prev != NULL) {
      prev->next = next;
    }

    if (conf_node_get_first_child(node->parent) == node) {
      conf_node_set_first_child(node->parent, next);
    }

    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

bool_t conf_doc_exists(conf_doc_t* doc, const char* path) {
  return_value_if_fail(doc != NULL && path != NULL, RET_BAD_PARAMS);

  return conf_doc_get_node(doc, path, FALSE) != NULL;
}

ret_t conf_doc_add_child(conf_doc_t* doc, const char* path) {
  conf_node_t* node = NULL;
  conf_node_t* new_node = NULL;
  char name[TK_NAME_LEN + 1];
  return_value_if_fail(doc != NULL && path != NULL, RET_BAD_PARAMS);

  node = conf_doc_get_node(doc, path, FALSE);
  return_value_if_fail(node != NULL, RET_NOT_FOUND);

  tk_snprintf(name, TK_NAME_LEN, "%u", conf_node_count_children(node));
  new_node = conf_doc_create_node(doc, name);
  return_value_if_fail(new_node != NULL, RET_OOM);

  return conf_doc_append_child(doc, node, new_node);
}

int32_t conf_doc_get_int(conf_doc_t* doc, const char* path, int32_t defval) {
  value_t vv;
  if (conf_doc_get(doc, path, &vv) == RET_OK) {
    return value_int32(&vv);
  } else {
    return defval;
  }
}

bool_t conf_doc_get_bool(conf_doc_t* doc, const char* path, bool_t defval) {
  value_t vv;
  if (conf_doc_get(doc, path, &vv) == RET_OK) {
    return value_bool(&vv);
  } else {
    return defval;
  }
}

float conf_doc_get_float(conf_doc_t* doc, const char* path, float defval) {
  value_t vv;
  if (conf_doc_get(doc, path, &vv) == RET_OK) {
    return value_float32(&vv);
  } else {
    return defval;
  }
}

const char* conf_doc_get_str(conf_doc_t* doc, const char* path, const char* defval) {
  value_t vv;
  if (conf_doc_get(doc, path, &vv) == RET_OK) {
    return value_str(&vv);
  } else {
    return defval;
  }
}

ret_t conf_doc_set_int(conf_doc_t* doc, const char* path, int32_t v) {
  value_t vv;
  return conf_doc_set(doc, path, value_set_int32(&vv, v));
}

ret_t conf_doc_set_bool(conf_doc_t* doc, const char* path, bool_t v) {
  value_t vv;
  return conf_doc_set(doc, path, value_set_bool(&vv, v));
}

ret_t conf_doc_set_float(conf_doc_t* doc, const char* path, float v) {
  value_t vv;
  return conf_doc_set(doc, path, value_set_float32(&vv, v));
}

ret_t conf_doc_set_str(conf_doc_t* doc, const char* path, const char* v) {
  value_t vv;
  return conf_doc_set(doc, path, value_set_str(&vv, v));
}

ret_t conf_doc_use_extend_type(conf_doc_t* doc, bool_t use) {
  ret_t ret = RET_OK;
  return_value_if_fail(doc != NULL, RET_BAD_PARAMS);

  if (doc->use_extend_type != use) {
    doc->use_extend_type = use;
    if (use) {
      doc->obj_array = object_array_create();
    } else {
      TK_OBJECT_UNREF(doc->obj_array);
    }
  }

  return ret;
}

ret_t conf_node_get_child_value(conf_node_t* node, const char* name, value_t* v) {
  conf_node_t* child = conf_node_find_child(node, name);
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);

  if (child == NULL) {
    value_set_int(v, 0);
    return RET_NOT_FOUND;
  }

  return conf_node_get_value(child, v);
}

ret_t conf_node_get_child_value_by_index(conf_node_t* node, uint32_t index, value_t* v) {
  conf_node_t* child = conf_node_find_child_by_index(node, index);
  return_value_if_fail(child != NULL && v != NULL, RET_BAD_PARAMS);

  return conf_node_get_value(child, v);
}

int32_t conf_node_get_child_value_int32(conf_node_t* node, const char* name, int32_t defval) {
  value_t v;
  return_value_if_fail(node != NULL && name != NULL, defval);

  if (conf_node_get_child_value(node, name, &v) == RET_OK) {
    return value_int32(&v);
  } else {
    return defval;
  }
}

uint32_t conf_node_get_child_value_uint32(conf_node_t* node, const char* name, uint32_t defval) {
  value_t v;
  return_value_if_fail(node != NULL && name != NULL, defval);

  if (conf_node_get_child_value(node, name, &v) == RET_OK) {
    return value_uint32(&v);
  } else {
    return defval;
  }
}

float conf_node_get_child_value_float(conf_node_t* node, const char* name, float defval) {
  value_t v;
  return_value_if_fail(node != NULL && name != NULL, defval);

  if (conf_node_get_child_value(node, name, &v) == RET_OK) {
    return value_float32(&v);
  } else {
    return defval;
  }
}

double conf_node_get_child_value_double(conf_node_t* node, const char* name, double defval) {
  value_t v;
  return_value_if_fail(node != NULL && name != NULL, defval);

  if (conf_node_get_child_value(node, name, &v) == RET_OK) {
    return value_double(&v);
  } else {
    return defval;
  }
}

bool_t conf_node_get_child_value_bool(conf_node_t* node, const char* name, bool_t defval) {
  value_t v;
  return_value_if_fail(node != NULL && name != NULL, defval);

  if (conf_node_get_child_value(node, name, &v) == RET_OK) {
    return value_bool(&v);
  } else {
    return defval;
  }
}

const char* conf_node_get_child_value_str(conf_node_t* node, const char* name, const char* defval) {
  value_t v;
  return_value_if_fail(node != NULL && name != NULL, defval);

  if (conf_node_get_child_value(node, name, &v) == RET_OK) {
    return value_str(&v);
  } else {
    return defval;
  }
}

static ret_t conf_node_foreach_sibling(const char* root, conf_node_t* iter,
                                       conf_doc_on_visit_t on_visit, void* ctx) {
  value_t v;
  bool_t is_array;
  int32_t index = 0;
  char path[MAX_PATH + 1];

  if (iter == NULL) {
    return RET_OK;
  }

  is_array = iter->parent->node_type == CONF_NODE_ARRAY;
  if (root == NULL) {
    root = "";
  }

  while (iter != NULL) {
    if (is_array) {
      tk_snprintf(path, MAX_PATH, "%s[%d]", root, index);
      index++;
    } else {
      tk_snprintf(path, MAX_PATH, "%s%s", root, conf_node_get_name(iter));
    }

    if (iter->node_type == CONF_NODE_SIMPLE) {
      return_value_if_fail(conf_node_get_value(iter, &v) == RET_OK, RET_BAD_PARAMS);

      on_visit(ctx, path, &v);
    } else {
      tk_str_append(path, MAX_PATH, ".");
      conf_node_foreach_sibling(path, conf_node_get_first_child(iter), on_visit, ctx);
    }

    iter = iter->next;
  }
  return RET_OK;
}

ret_t conf_doc_foreach(conf_doc_t* doc, conf_doc_on_visit_t on_visit, void* ctx) {
  return_value_if_fail(doc && on_visit, RET_BAD_PARAMS);
  return conf_node_foreach_sibling(NULL, conf_node_get_first_child(doc->root), on_visit, ctx);
}

static ret_t conf_doc_save_str(const char* p, str_t* str, char comment_char) {
  return_value_if_fail(p != NULL, RET_BAD_PARAMS);

  while (*p) {
    char c = str_escape_char(*p);
    if (c != *p || *p == comment_char || *p == '\\' || *p == '\n') {
      return_value_if_fail(str_append_char(str, '\\') == RET_OK, RET_OOM);
    }
    return_value_if_fail(str_append_char(str, c) == RET_OK, RET_OOM);
    p++;
  }

  return RET_OK;
}

ret_t conf_node_save_value(str_t* str, const value_t* v, char comment_char) {
  char buff[64] = {0};
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  switch (v->type) {
    case VALUE_TYPE_STRING: {
      const char* p = value_str(v);
      return_value_if_fail(p != NULL, RET_BAD_PARAMS);
      return conf_doc_save_str(p, str, comment_char);
    }
    case VALUE_TYPE_WSTRING: {
      str_t s;
      ret_t ret = RET_OK;

      str_init(&s, 0);
      str_from_wstr(&s, value_wstr(v));
      ret = conf_doc_save_str(s.str, str, comment_char);
      str_reset(&s);
      return ret;
    }
    case VALUE_TYPE_FLOAT32: {
      tk_snprintf(buff, sizeof(buff) - 1, "%f", value_float32(v));
      break;
    }
    case VALUE_TYPE_FLOAT:
    case VALUE_TYPE_DOUBLE: {
      tk_snprintf(buff, sizeof(buff) - 1, "%lf", value_double(v));
      break;
    }
    case VALUE_TYPE_INT64: {
      tk_snprintf(buff, sizeof(buff) - 1, "%lld", value_int64(v));
      break;
    }
    case VALUE_TYPE_UINT64: {
      tk_snprintf(buff, sizeof(buff) - 1, "%llu", value_uint64(v));
      break;
    }
    default: {
      tk_snprintf(buff, sizeof(buff) - 1, "%d", value_int(v));
      break;
    }
  }

  return str_append(str, buff);
}
