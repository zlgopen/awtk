/**
 * File:   tree.c
 * Author: AWTK Develop Team
 * Brief:  树结构
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY{} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2025-03-26 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "tkc/tree.h"

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/darray.h"

inline static tree_node_t* tree_node_get_first_sibling(tree_node_t* node) {
  while (node != NULL && node->prev_sibling != NULL) {
    node = node->prev_sibling;
  }
  return node;
}

inline static int32_t tree_node_sibling_size(const tree_node_t* first_sibling) {
  int32_t ret = 0;
  const tree_node_t* iter = first_sibling;
  for (; iter != NULL; iter = iter->next_sibling) {
    ret++;
  }
  return ret;
}

inline static tree_node_t* tree_node_get_sibling_by_index(tree_node_t* first_sibling,
                                                          uint32_t index) {
  uint32_t i = 0;
  tree_node_t* node = first_sibling;
  for (; node != NULL && node->next_sibling != NULL && i < index; i++) {
    node = node->next_sibling;
  }
  return node;
}

static ret_t tree_node_foreach_breadth_first(tree_node_t* node, tk_visit_t visit, void* ctx) {
  uint32_t i = 0;
  darray_t queue;
  darray_init(&queue, 64, NULL, NULL);
  darray_push(&queue, node);

  for (i = 0; i < queue.size; i++) {
    tree_node_t* iter = (tree_node_t*)darray_get(&queue, i);
    ret_t ret = visit(ctx, iter);

    if (ret != RET_OK) {
      break;
    }

    for (iter = tree_node_get_first_sibling(iter->child); iter != NULL; iter = iter->next_sibling) {
      darray_push(&queue, iter);
    }
  }

  darray_deinit(&queue);

  return RET_OK;
}

static ret_t tree_node_foreach_preorder(tree_node_t* node, tk_visit_t visit, void* ctx) {
  darray_t stack;
  darray_init(&stack, 16, NULL, NULL);
  darray_push(&stack, node);

  while (stack.size > 0) {
    tree_node_t* iter = (tree_node_t*)darray_pop(&stack);
    ret_t ret = visit(ctx, iter);

    if (ret != RET_OK) {
      break;
    }

    if (iter != node && iter->next_sibling != NULL) {
      darray_push(&stack, iter->next_sibling);
    }

    iter = tree_node_get_first_sibling(iter->child);
    if (iter != NULL) {
      darray_push(&stack, iter);
    }
  }

  darray_deinit(&stack);

  return RET_OK;
}

static ret_t tree_node_foreach_postorder(tree_node_t* node, tk_visit_t visit, void* ctx) {
  darray_t result_stack;
  darray_t process_stack;
  darray_init(&result_stack, 128, NULL, NULL);
  darray_init(&process_stack, 32, NULL, NULL);
  darray_push(&process_stack, node);

  while (process_stack.size > 0) {
    tree_node_t* iter = (tree_node_t*)darray_pop(&process_stack);
    darray_push(&result_stack, iter);

    for (iter = tree_node_get_first_sibling(iter->child); iter != NULL; iter = iter->next_sibling) {
      darray_push(&process_stack, iter);
    }
  }

  while (result_stack.size > 0) {
    tree_node_t* iter = (tree_node_t*)darray_pop(&result_stack);
    ret_t ret = visit(ctx, iter);

    if (ret != RET_OK) {
      break;
    }
  }

  darray_deinit(&process_stack);
  darray_deinit(&result_stack);

  return RET_OK;
}

inline static ret_t tree_node_foreach(tree_node_t* node, tree_foreach_type_t foreach_type,
                                      tk_visit_t visit, void* ctx) {
  if (node != NULL) {
    switch (foreach_type) {
      case TREE_FOREACH_TYPE_BREADTH_FIRST:
        return tree_node_foreach_breadth_first(node, visit, ctx);
      case TREE_FOREACH_TYPE_PREORDER:
        return tree_node_foreach_preorder(node, visit, ctx);
      case TREE_FOREACH_TYPE_POSTORDER:
        return tree_node_foreach_postorder(node, visit, ctx);
      default:
        ENSURE(!"Not support foreach type!");
        return RET_NOT_IMPL;
    }
  }

  return RET_OK;
}

tree_node_t* tree_node_create(void* data) {
  tree_node_t* ret = TKMEM_ZALLOC(tree_node_t);
  return_value_if_fail(ret != NULL, NULL);

  ret->data = data;

  return ret;
}

int32_t tree_node_degree(tree_node_t* node) {
  return_value_if_fail(node != NULL, -1);

  return tree_node_sibling_size(tree_node_get_first_sibling(node->child));
}

static ret_t tree_node_destroy_impl(tree_node_t* node, tk_destroy_t destroy) {
  return_value_if_fail(node != NULL && destroy != NULL, RET_BAD_PARAMS);

  destroy(node->data);
  TKMEM_FREE(node);

  return RET_OK;
}

static ret_t tree_node_destroy_on_visit(void* ctx, const void* data) {
  tk_destroy_t destroy = (tk_destroy_t)(ctx);
  tree_node_t* node = (tree_node_t*)(data);
  tree_node_destroy_impl(node, destroy);
  return RET_OK;
}

ret_t tree_node_destroy(tree_node_t* node, tk_destroy_t destroy) {
  return_value_if_fail(node != NULL && destroy != NULL, RET_BAD_PARAMS);

  return tree_node_foreach(node, TREE_FOREACH_TYPE_POSTORDER, tree_node_destroy_on_visit, destroy);
}

tree_t* tree_create(tk_destroy_t destroy, tk_compare_t compare) {
  tree_t* ret = TKMEM_ZALLOC(tree_t);
  return_value_if_fail(ret != NULL, NULL);

  goto_error_if_fail(RET_OK == tree_init(ret, destroy, compare));

  return ret;
error:
  tree_destroy(ret);
  return NULL;
}

ret_t tree_init(tree_t* tree, tk_destroy_t destroy, tk_compare_t compare) {
  return_value_if_fail(tree != NULL, RET_BAD_PARAMS);

  tree->root = NULL;
  tree->destroy = destroy != NULL ? destroy : dummy_destroy;
  tree->compare = compare != NULL ? compare : pointer_compare;

  return RET_OK;
}

typedef struct _tree_find_on_visit_ctx_t {
  tree_t* tree;
  void* data;
  tk_compare_t compare;
  tree_node_t* result;
} tree_find_on_visit_ctx_t;

static ret_t tree_find_on_visit(void* ctx, const void* data) {
  tree_find_on_visit_ctx_t* actx = (tree_find_on_visit_ctx_t*)(ctx);
  const tree_node_t* node = (const tree_node_t*)(data);

  if (0 == actx->compare(node->data, actx->data)) {
    actx->result = (tree_node_t*)node;
    return RET_FOUND;
  }

  return RET_OK;
}

tree_node_t* tree_find(tree_t* tree, tree_node_t* node, tree_foreach_type_t foreach_type,
                       void* ctx) {
  return tree_find_ex(tree, node, foreach_type, tree->compare, ctx);
}

tree_node_t* tree_find_ex(tree_t* tree, tree_node_t* node, tree_foreach_type_t foreach_type,
                          tk_compare_t compare, void* ctx) {
  tree_find_on_visit_ctx_t actx = {
      .tree = tree,
      .data = ctx,
      .compare = compare,
  };
  return_value_if_fail(tree != NULL && compare != NULL, NULL);

  tree_foreach(tree, node, foreach_type, tree_find_on_visit, &actx);

  return actx.result;
}

typedef struct _tree_remove_on_visit_ctx_t {
  tree_t* tree;
  void* data;
  tk_compare_t compare;
  int32_t remove_size;
  darray_t* remove_list;
} tree_remove_on_visit_ctx_t;

static ret_t tree_remove_on_visit(void* ctx, const void* data) {
  tree_remove_on_visit_ctx_t* actx = (tree_remove_on_visit_ctx_t*)(ctx);
  const tree_node_t* node = (const tree_node_t*)(data);

  if (actx->remove_size == 0) {
    return RET_STOP;
  }

  if (0 == actx->compare(node->data, actx->data)) {
    darray_push(actx->remove_list, (tree_node_t*)node);
    actx->remove_size--;
  }

  return RET_OK;
}

ret_t tree_remove(tree_t* tree, tree_node_t* node, tree_foreach_type_t foreach_type, void* ctx) {
  return tree_remove_ex(tree, node, foreach_type, tree->compare, ctx, 1);
}

ret_t tree_remove_ex(tree_t* tree, tree_node_t* node, tree_foreach_type_t foreach_type,
                     tk_compare_t compare, void* ctx, int32_t remove_size) {
  ret_t ret = RET_OK;
  darray_t remove_list;
  tree_remove_on_visit_ctx_t actx = {
      .tree = tree,
      .data = ctx,
      .compare = compare,
      .remove_size = remove_size,
      .remove_list = &remove_list,
  };
  return_value_if_fail(tree != NULL && compare != NULL, RET_BAD_PARAMS);

  darray_init(&remove_list, remove_size, NULL, NULL);

  ret = tree_foreach(tree, node, foreach_type, tree_remove_on_visit, &actx);

  while (remove_list.size > 0) {
    tree_node_t* iter = darray_pop(&remove_list);
    tree_remove_node(tree, iter);
  }

  darray_deinit(&remove_list);

  return ret;
}

ret_t tree_remove_node(tree_t* tree, tree_node_t* node) {
  return_value_if_fail(tree != NULL && node != NULL, RET_BAD_PARAMS);
  return_value_if_fail(RET_OK == tree_unlink_node(tree, node), RET_FAIL);

  return tree_node_destroy(node, tree->destroy);
}

ret_t tree_unlink_node(tree_t* tree, tree_node_t* node) {
  return_value_if_fail(tree != NULL && node != NULL, RET_BAD_PARAMS);

  if (node->parent != NULL) {
    if (node->parent->child == node) {
      if (node->prev_sibling != NULL) {
        node->parent->child = node->prev_sibling;
      } else {
        node->parent->child = node->next_sibling;
      }
    }
    node->parent = NULL;
  }

  if (node->prev_sibling != NULL) {
    node->prev_sibling->next_sibling = node->next_sibling;
  }

  if (node->next_sibling != NULL) {
    node->next_sibling->prev_sibling = node->prev_sibling;
  }

  node->prev_sibling = NULL;
  node->next_sibling = NULL;

  return RET_OK;
}

ret_t tree_clear(tree_t* tree) {
  return_value_if_fail(tree != NULL, RET_BAD_PARAMS);

  if (tree->root != NULL) {
    tree_remove_node(tree, tree->root);
    tree->root = NULL;
  }

  return RET_OK;
}

static ret_t tree_link_sibling(tree_t* tree, tree_node_t* prev_sibling, tree_node_t* next_sibling,
                               tree_node_t* node) {
  return_value_if_fail(
      tree != NULL && (prev_sibling != NULL || next_sibling != NULL) && node != NULL,
      RET_BAD_PARAMS);

  if (prev_sibling != NULL) {
    node->parent = prev_sibling->parent;

    node->prev_sibling = prev_sibling;
    node->next_sibling = prev_sibling->next_sibling;

    prev_sibling->next_sibling = node;
    if (node->next_sibling != NULL) {
      node->next_sibling->prev_sibling = node;
    }
  } else {
    node->parent = next_sibling->parent;

    node->next_sibling = next_sibling;
    node->prev_sibling = next_sibling->prev_sibling;

    next_sibling->prev_sibling = node;
    if (node->prev_sibling != NULL) {
      node->prev_sibling->next_sibling = node;
    }
  }

  return RET_OK;
}

tree_node_t* tree_get_child_node(tree_t* tree, tree_node_t* node, uint32_t index) {
  return_value_if_fail(tree != NULL && node != NULL, NULL);

  if (node->child != NULL) {
    return tree_get_sibling_node(tree, node->child, index);
  }

  return NULL;
}

ret_t tree_set_root(tree_t* tree, tree_node_t* root) {
  return_value_if_fail(tree != NULL && root != NULL, RET_BAD_PARAMS);

  tree_clear(tree);
  tree->root = root;

  return RET_OK;
}

ret_t tree_insert_child_node(tree_t* tree, tree_node_t* node, uint32_t index, tree_node_t* child) {
  return_value_if_fail(tree != NULL && child != NULL, RET_BAD_PARAMS);

  if (tree->root == NULL) {
    return tree_set_root(tree, child);
  }

  if (node == NULL) {
    node = tree->root;
  }

  if (node->child == NULL) {
    node->child = child;
    child->parent = node;
    return RET_OK;
  } else {
    return tree_insert_sibling_node(tree, node->child, index, child);
  }
}

tree_node_t* tree_get_sibling_node(tree_t* tree, tree_node_t* node, uint32_t index) {
  tree_node_t* first_sibling = NULL;
  return_value_if_fail(tree != NULL && node != NULL, NULL);

  first_sibling = tree_node_get_first_sibling(node);

  if (index == 0) {
    return first_sibling;
  } else {
    return tree_node_get_sibling_by_index(first_sibling, index);
  }
}

ret_t tree_insert_sibling_node(tree_t* tree, tree_node_t* node, uint32_t index,
                               tree_node_t* sibling) {
  tree_node_t* first_sibling = NULL;
  return_value_if_fail(tree != NULL && node != NULL && sibling != NULL, RET_BAD_PARAMS);
  return_value_if_fail(node != tree->root, RET_FAIL);

  first_sibling = tree_node_get_first_sibling(node);

  if (index == 0) {
    return tree_link_sibling(tree, NULL, first_sibling, sibling);
  } else {
    tree_node_t* pos_node = tree_node_get_sibling_by_index(first_sibling, index);
    int32_t size = tree_node_sibling_size(first_sibling);

    if (index < size) {
      return tree_link_sibling(tree, pos_node->prev_sibling, pos_node, sibling);
    } else {
      return tree_link_sibling(tree, pos_node, NULL, sibling);
    }
  }
}

ret_t tree_append_child_node(tree_t* tree, tree_node_t* node, tree_node_t* child) {
  return tree_insert_child_node(tree, node, -1, child);
}

ret_t tree_append_sibling_node(tree_t* tree, tree_node_t* node, tree_node_t* sibling) {
  return tree_insert_sibling_node(tree, node, -1, sibling);
}

ret_t tree_prepend_child_node(tree_t* tree, tree_node_t* node, tree_node_t* child) {
  return tree_insert_child_node(tree, node, 0, child);
}

ret_t tree_prepend_sibling_node(tree_t* tree, tree_node_t* node, tree_node_t* sibling) {
  return tree_insert_sibling_node(tree, node, 0, sibling);
}

ret_t tree_foreach(tree_t* tree, tree_node_t* node, tree_foreach_type_t foreach_type,
                   tk_visit_t visit, void* ctx) {
  return_value_if_fail(tree != NULL && visit != NULL, RET_BAD_PARAMS);

  if (node == NULL) {
    node = tree->root;
  }

  return tree_node_foreach(node, foreach_type, visit, ctx);
}

bool_t tree_is_empty(tree_t* tree, tree_node_t* node) {
  return_value_if_fail(tree != NULL, FALSE);

  if (node == NULL) {
    node = tree->root;
  }

  return node == NULL || node->child == NULL;
}

static ret_t tree_size_on_visit(void* ctx, const void* data) {
  int32_t* p_size = (int32_t*)(ctx);
  (void)data;
  (*p_size)++;
  return RET_OK;
}

int32_t tree_size(tree_t* tree, tree_node_t* node) {
  int32_t ret = 0;
  return_value_if_fail(tree != NULL, -1);

  tree_foreach(tree, node, TREE_FOREACH_TYPE_PREORDER, tree_size_on_visit, &ret);

  return ret;
}

int32_t tree_depth(tree_t* tree, tree_node_t* node) {
  int32_t depth = -1;
  const tree_node_t* iter = NULL;
  return_value_if_fail(tree != NULL, -1);

  if (node == NULL) {
    node = tree->root;
  }

  for (iter = node; iter != NULL; iter = iter->parent) {
    depth++;
  }

  return depth;
}

int32_t tree_level(tree_t* tree, tree_node_t* node) {
  return_value_if_fail(tree != NULL, -1);

  return tree_depth(tree, node) + 1;
}

typedef struct _tree_height_on_visit_ctx_t {
  const tree_node_t* root;
  int32_t max_height;
} tree_height_on_visit_ctx_t;

static ret_t tree_height_on_visit(void* ctx, const void* data) {
  tree_height_on_visit_ctx_t* actx = (tree_height_on_visit_ctx_t*)(ctx);
  const tree_node_t* node = (const tree_node_t*)(data);
  const tree_node_t* iter = NULL;
  int32_t height = 0;

  for (iter = node, height = 0; iter != actx->root; iter = iter->parent) {
    if (iter == NULL) {
      actx->max_height = -1;
      return RET_FAIL;
    }
    height++;
  }

  if (height > actx->max_height) {
    actx->max_height = height;
  }

  return RET_OK;
}

int32_t tree_height(tree_t* tree, tree_node_t* node) {
  tree_height_on_visit_ctx_t ctx;
  return_value_if_fail(tree != NULL, -1);

  if (node == NULL) {
    node = tree->root;
  }

  memset(&ctx, 0, sizeof(tree_height_on_visit_ctx_t));
  ctx.root = node;

  tree_foreach(tree, node, TREE_FOREACH_TYPE_PREORDER, tree_height_on_visit, &ctx);

  return ctx.max_height;
}

static ret_t tree_degree_on_visit(void* ctx, const void* data) {
  const tree_node_t* node = (const tree_node_t*)(data);
  int32_t* p_degree = (int32_t*)(ctx);
  int32_t degree = tree_node_degree((tree_node_t*)node);

  if (degree > *p_degree) {
    *p_degree = degree;
  }

  return RET_OK;
}

int32_t tree_degree(tree_t* tree) {
  int32_t ret = 0;
  return_value_if_fail(tree != NULL, -1);

  tree_foreach(tree, tree->root, TREE_FOREACH_TYPE_PREORDER, tree_degree_on_visit, &ret);

  return ret;
}

static ret_t tree_node_str_append_default(void* ctx, const void* data) {
  const tree_node_t* node = (const tree_node_t*)(data);
  str_t* str = (str_t*)(ctx);
  char buf[2 + 16 + 1] = {'\0'};
  tk_snprintf(buf, ARRAY_SIZE(buf) - 1, "%p", node->data);

  return str_append(str, buf);
}

typedef struct _tree_to_string_on_visit_ctx_t {
  tree_t* tree;
  tree_node_t* root;
  str_t* result;
  tk_visit_t node_str_append_func;
} tree_to_string_on_visit_ctx_t;

static ret_t tree_to_string_on_visit(void* ctx, const void* data) {
  ret_t ret = RET_OK;
  tree_to_string_on_visit_ctx_t* actx = (tree_to_string_on_visit_ctx_t*)ctx;
  const tree_node_t* node = (const tree_node_t*)data;

  if (actx->result->size > 0) {
    str_append(actx->result, "\n");
  }

  if (node != NULL && node != actx->root) {
    const tree_node_t* iter = NULL;
    darray_t stack;
    darray_init(&stack, 16, NULL, NULL);

    for (iter = node; iter != NULL && iter != actx->root; iter = iter->parent) {
      if (iter == node) {
        darray_push(&stack, iter->next_sibling != NULL ? "├── " : "└── ");
      } else if (iter->next_sibling != NULL) {
        darray_push(&stack, "│   ");
      } else {
        darray_push(&stack, "    ");
      }
    }
    while (stack.size > 0) {
      str_append(actx->result, (const char*)darray_pop(&stack));
    }

    darray_deinit(&stack);
  }

  ret = actx->node_str_append_func(actx->result, node);

  return ret;
}

ret_t tree_to_string(tree_t* tree, tree_node_t* node, str_t* result,
                     tk_visit_t node_str_append_func) {
  tree_to_string_on_visit_ctx_t ctx = {
      .tree = tree,
      .result = result,
      .node_str_append_func =
          (node_str_append_func != NULL) ? node_str_append_func : tree_node_str_append_default,
  };
  return_value_if_fail(tree != NULL && result != NULL, RET_BAD_PARAMS);

  if (node == NULL) {
    node = tree->root;
  }
  ctx.root = node;

  str_clear(result);

  return tree_foreach(tree, node, TREE_FOREACH_TYPE_PREORDER, tree_to_string_on_visit, &ctx);
}

ret_t tree_deinit(tree_t* tree) {
  return_value_if_fail(tree != NULL, RET_BAD_PARAMS);

  return tree_clear(tree);
}

ret_t tree_destroy(tree_t* tree) {
  return_value_if_fail(tree != NULL, RET_BAD_PARAMS);

  tree_deinit(tree);
  TKMEM_FREE(tree);

  return RET_OK;
}
