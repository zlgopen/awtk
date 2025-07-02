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

static ret_t tree_node_destroy(tree_node_t* node, tk_destroy_t destroy, mem_allocator_t* allocator);

inline static int32_t tree_node_depth(tree_node_t* node) {
  int32_t depth = -1;
  const tree_node_t* iter = NULL;
  return_value_if_fail(node != NULL, -1);

  for (iter = node; iter != NULL; iter = iter->parent) {
    depth++;
  }

  return depth;
}

bool_t tree_node_is_ancestor(const tree_node_t* node, const tree_node_t* ancestor) {
  return_value_if_fail(node != NULL && ancestor != NULL, FALSE);

  for (node = node->parent; node != NULL; node = node->parent) {
    if (node == ancestor) {
      return TRUE;
    }
  }
  return FALSE;
}

tree_node_t* tree_node_get_lowest_common_ancestor(tree_node_t* node1, tree_node_t* node2) {
  return_value_if_fail(node1 != NULL && node2 != NULL, NULL);

  if (node1 == node2) {
    return node1;
  } else {
    tree_node_t* n1 = node1;
    tree_node_t* n2 = node2;
    int32_t depth_diff = tree_node_depth(n1) - tree_node_depth(n2);

    while (depth_diff > 0 && n1 != NULL) {
      n1 = n1->parent;
      depth_diff--;
    }
    while (depth_diff < 0 && n2 != NULL) {
      n2 = n2->parent;
      depth_diff++;
    }

    while (n1 != NULL && n2 != NULL) {
      if (n1 == n2) {
        return n1;
      }
      n1 = n1->parent;
      n2 = n2->parent;
    }
  }

  return NULL;
}

static int tree_node_is_ancestor_cmp(const void* iter, const void* ctx) {
  const tree_node_t* node = (const tree_node_t*)(iter);
  const tree_node_t* ancestor = (const tree_node_t*)(ctx);
  return tree_node_is_ancestor(node, ancestor) ? 0 : -1;
}

inline static bool_t tree_node_is_leaf(tree_node_t* node) {
  return_value_if_fail(node != NULL, FALSE);
  return node->child == NULL;
}

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

static ret_t tree_node_unlink(tree_node_t* node) {
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);

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

static ret_t tree_node_link_sibling(tree_node_t* node, tree_node_t* prev_sibling,
                                    tree_node_t* next_sibling) {
  return_value_if_fail(node != NULL && (prev_sibling != NULL || next_sibling != NULL),
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

static ret_t tree_node_foreach_breadth_first(tree_node_t* node, tk_visit_t visit,
                                             tk_destroy_t destroy, mem_allocator_t* allocator,
                                             void* ctx) {
  uint32_t i = 0;
  darray_t queue;
  darray_init(&queue, 64, NULL, NULL);
  darray_push(&queue, node);

  for (i = 0; i < queue.size; i++) {
    tree_node_t* iter = (tree_node_t*)darray_get(&queue, i);
    ret_t ret = visit(ctx, iter);

    if (ret == RET_REMOVE) {
      tree_node_destroy(node, destroy, allocator);
      continue;
    } else if (ret != RET_OK) {
      break;
    }

    for (iter = tree_node_get_first_sibling(iter->child); iter != NULL; iter = iter->next_sibling) {
      darray_push(&queue, iter);
    }
  }

  darray_deinit(&queue);

  return RET_OK;
}

static ret_t tree_node_foreach_preorder(tree_node_t* node, tk_visit_t visit, tk_destroy_t destroy,
                                        mem_allocator_t* allocator, void* ctx) {
  darray_t stack;
  darray_init(&stack, 16, NULL, NULL);
  darray_push(&stack, node);

  while (stack.size > 0) {
    tree_node_t* iter = (tree_node_t*)darray_pop(&stack);
    ret_t ret = visit(ctx, iter);

    if (iter != node && iter->next_sibling != NULL) {
      darray_push(&stack, iter->next_sibling);
    }

    if (ret == RET_REMOVE) {
      tree_node_destroy(node, destroy, allocator);
      continue;
    } else if (ret != RET_OK) {
      break;
    }

    iter = tree_node_get_first_sibling(iter->child);
    if (iter != NULL) {
      darray_push(&stack, iter);
    }
  }

  darray_deinit(&stack);

  return RET_OK;
}

static ret_t tree_node_foreach_postorder(tree_node_t* node, tk_visit_t visit, tk_destroy_t destroy,
                                         mem_allocator_t* allocator, void* ctx) {
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

    if (ret == RET_REMOVE) {
      tree_node_destroy(node, destroy, allocator);
      continue;
    } else if (ret != RET_OK) {
      break;
    }
  }

  darray_deinit(&process_stack);
  darray_deinit(&result_stack);

  return RET_OK;
}

inline static ret_t tree_node_foreach(tree_node_t* node, tree_foreach_type_t foreach_type,
                                      tk_visit_t visit, tk_destroy_t destroy,
                                      mem_allocator_t* allocator, void* ctx) {
  if (node != NULL) {
    switch (foreach_type) {
      case TREE_FOREACH_TYPE_BREADTH_FIRST:
        return tree_node_foreach_breadth_first(node, visit, destroy, allocator, ctx);
      case TREE_FOREACH_TYPE_PREORDER:
        return tree_node_foreach_preorder(node, visit, destroy, allocator, ctx);
      case TREE_FOREACH_TYPE_POSTORDER:
        return tree_node_foreach_postorder(node, visit, destroy, allocator, ctx);
      default:
        ENSURE(!"Not support foreach type!");
        return RET_NOT_IMPL;
    }
  }

  return RET_OK;
}

static inline tree_node_t* tree_node_create(void* data, mem_allocator_t* allocator) {
  tree_node_t* ret = NULL;
  if (allocator != NULL) {
    ret = MEM_ALLOCATOR_ALLOC(allocator, sizeof(tree_node_t));
  } else {
    ret = TKMEM_ALLOC(sizeof(tree_node_t));
  }
  return_value_if_fail(ret != NULL, NULL);

  memset(ret, 0, sizeof(*ret));

  ret->data = data;

  return ret;
}

tree_node_t* tree_create_node(tree_t* tree, void* data) {
  return_value_if_fail(tree != NULL, NULL);
  return tree_node_create(data, tree->node_allocator);
}

int32_t tree_node_degree(tree_node_t* node) {
  return_value_if_fail(node != NULL, -1);

  return tree_node_sibling_size(tree_node_get_first_sibling(node->child));
}

tree_node_t* tree_node_get_sibling(tree_node_t* node, uint32_t index) {
  tree_node_t* first_sibling = NULL;
  return_value_if_fail(node != NULL, NULL);

  first_sibling = tree_node_get_first_sibling(node);

  if (index == 0) {
    return first_sibling;
  } else {
    return tree_node_get_sibling_by_index(first_sibling, index);
  }
}

tree_node_t* tree_node_get_child(tree_node_t* node, uint32_t index) {
  return_value_if_fail(node != NULL, NULL);

  if (!tree_node_is_leaf(node)) {
    return tree_node_get_sibling(node->child, index);
  }

  return NULL;
}

static ret_t tree_node_destroy_impl(tree_node_t* node, tk_destroy_t destroy,
                                    mem_allocator_t* allocator) {
  return_value_if_fail(node != NULL && destroy != NULL, RET_BAD_PARAMS);

  if (node->data != NULL) {
    destroy(node->data);
  }
  if (allocator != NULL) {
    MEM_ALLOCATOR_FREE(allocator, node);
  } else {
    TKMEM_FREE(node);
  }

  return RET_OK;
}

typedef struct _tree_node_destroy_on_visit_ctx_t {
  tk_destroy_t destroy;
  mem_allocator_t* allocator;
} tree_node_destroy_on_visit_ctx_t;

static ret_t tree_node_destroy_on_visit(void* ctx, const void* data) {
  tree_node_destroy_on_visit_ctx_t* actx = (tree_node_destroy_on_visit_ctx_t*)(ctx);
  tree_node_t* node = (tree_node_t*)(data);
  tree_node_destroy_impl(node, actx->destroy, actx->allocator);
  return RET_OK;
}

inline static ret_t tree_node_destroy(tree_node_t* node, tk_destroy_t destroy,
                                      mem_allocator_t* allocator) {
  tree_node_destroy_on_visit_ctx_t ctx = {
      .destroy = destroy,
      .allocator = allocator,
  };
  return_value_if_fail(node != NULL && destroy != NULL, RET_BAD_PARAMS);
  return_value_if_fail(RET_OK == tree_node_unlink(node), RET_FAIL);

  return tree_node_foreach(node, TREE_FOREACH_TYPE_POSTORDER, tree_node_destroy_on_visit, destroy,
                           allocator, &ctx);
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

  memset(tree, 0, sizeof(tree_t));

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
      .remove_list = &remove_list,
  };
  return_value_if_fail(tree != NULL && compare != NULL, RET_BAD_PARAMS);

  if (remove_size == 0) {
    return RET_OK;
  } else if (remove_size < 0) {
    actx.remove_size = tree_size(tree, node);
  } else {
    int32_t size = tree_size(tree, node);
    actx.remove_size = tk_min(remove_size, size);
  }

  darray_init(&remove_list, actx.remove_size, NULL, NULL);

  ret = tree_foreach(tree, node, foreach_type, tree_remove_on_visit, &actx);

  while (remove_list.size > 0) {
    tree_node_t* iter = darray_pop(&remove_list);
    darray_remove_all(&remove_list, tree_node_is_ancestor_cmp, iter);
    tree_remove_node(tree, iter);
  }

  darray_deinit(&remove_list);

  return ret;
}

ret_t tree_remove_node(tree_t* tree, tree_node_t* node) {
  return_value_if_fail(tree != NULL && node != NULL, RET_BAD_PARAMS);

  return tree_node_destroy(node, tree->destroy, tree->node_allocator);
}

ret_t tree_unlink_node(tree_t* tree, tree_node_t* node) {
  return_value_if_fail(tree != NULL && node != NULL, RET_BAD_PARAMS);

  return tree_node_unlink(node);
}

ret_t tree_clear(tree_t* tree) {
  return_value_if_fail(tree != NULL, RET_BAD_PARAMS);

  if (tree->root != NULL) {
    tree_remove_node(tree, tree->root);
    tree->root = NULL;
  }

  return RET_OK;
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

  if (tree_node_is_leaf(node)) {
    node->child = child;
    child->parent = node;
    return RET_OK;
  } else {
    return tree_insert_sibling_node(tree, node->child, index, child);
  }
}

ret_t tree_insert_sibling_node(tree_t* tree, tree_node_t* node, uint32_t index,
                               tree_node_t* sibling) {
  tree_node_t* first_sibling = NULL;
  return_value_if_fail(tree != NULL && node != NULL && sibling != NULL, RET_BAD_PARAMS);
  return_value_if_fail(node != tree->root, RET_FAIL);

  first_sibling = tree_node_get_first_sibling(node);

  if (index == 0) {
    return tree_node_link_sibling(sibling, NULL, first_sibling);
  } else {
    tree_node_t* pos_node = tree_node_get_sibling_by_index(first_sibling, index);
    int32_t size = tree_node_sibling_size(first_sibling);

    if (index < size) {
      return tree_node_link_sibling(sibling, pos_node->prev_sibling, pos_node);
    } else {
      return tree_node_link_sibling(sibling, pos_node, NULL);
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

  return tree_node_foreach(node, foreach_type, visit, tree->destroy, tree->node_allocator, ctx);
}

bool_t tree_is_empty(tree_t* tree, tree_node_t* node) {
  return_value_if_fail(tree != NULL, FALSE);

  if (node == NULL) {
    node = tree->root;
  }

  return node == NULL || tree_node_is_leaf(node);
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
  return_value_if_fail(tree != NULL, -1);

  if (node == NULL) {
    node = tree->root;
  }

  return tree_node_depth(node);
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

inline static ret_t tree_to_string_on_append_node_default(const tree_node_t* node, str_t* result,
                                                          void* ctx) {
  (void)ctx;
  return str_append_format(result, 2 + 16 + 1, "%p", node->data);
}

inline static ret_t tree_to_string_on_swap_default(const tree_node_t* node, str_t* result,
                                                   void* ctx) {
  (void)node, (void)ctx;
  return str_append(result, "\n");
}

inline static ret_t tree_to_string_on_empty_default(const tree_node_t* node, str_t* result,
                                                    void* ctx) {
  (void)node, (void)ctx;
  return str_append(result, "    ");
}

inline static ret_t tree_to_string_on_connect_child_default(const tree_node_t* node, str_t* result,
                                                            void* ctx) {
  (void)ctx;
  return str_append(result, node->next_sibling != NULL ? "├── " : "└── ");
}

inline static ret_t tree_to_string_on_connect_sibling_default(const tree_node_t* node,
                                                              str_t* result, void* ctx) {
  (void)node, (void)ctx;
  return str_append(result, "│   ");
}

typedef struct _tree_to_string_on_visit_ctx_t {
  tree_t* tree;
  tree_node_t* root;
  str_t* result;
  const tree_to_string_handler_t* handler;
} tree_to_string_on_visit_ctx_t;

static ret_t tree_to_string_on_visit(void* ctx, const void* data) {
  ret_t ret = RET_OK;
  tree_to_string_on_visit_ctx_t* actx = (tree_to_string_on_visit_ctx_t*)ctx;
  const tree_node_t* node = (const tree_node_t*)data;

  if (actx->result->size > 0) {
    actx->handler->on_swap(node, actx->result, actx->handler->ctx);
  }

  if (node != NULL && node != actx->root) {
    const tree_node_t* iter = NULL;
    darray_t stack;
    darray_init(&stack, 16, (tk_destroy_t)str_destroy, NULL);

    for (iter = node; iter != NULL && iter != actx->root; iter = iter->parent) {
      str_t* iter_str = str_create(0);
      if (iter == node) {
        actx->handler->on_connect_child(node, iter_str, actx->handler->ctx);
      } else if (iter->next_sibling != NULL) {
        actx->handler->on_connect_sibling(node, iter_str, actx->handler->ctx);
      } else {
        actx->handler->on_empty(node, iter_str, actx->handler->ctx);
      }
      darray_push(&stack, iter_str);
    }
    while (stack.size > 0) {
      str_t* iter_str = (str_t*)darray_pop(&stack);
      str_append(actx->result, iter_str->str);
      stack.destroy(iter_str);
    }

    darray_deinit(&stack);
  }

  ret = actx->handler->on_append_node(node, actx->result, actx->handler->ctx);

  return ret;
}

ret_t tree_to_string(tree_t* tree, tree_node_t* node, str_t* result,
                     tree_to_string_handler_t* handler) {
  tree_to_string_handler_t real_handler = {
      .on_append_node = (handler != NULL && handler->on_append_node != NULL)
                            ? handler->on_append_node
                            : tree_to_string_on_append_node_default,
      .on_swap = (handler != NULL && handler->on_swap != NULL) ? handler->on_swap
                                                               : tree_to_string_on_swap_default,
      .on_empty = (handler != NULL && handler->on_empty != NULL) ? handler->on_empty
                                                                 : tree_to_string_on_empty_default,
      .on_connect_child = (handler != NULL && handler->on_connect_child != NULL)
                              ? handler->on_connect_child
                              : tree_to_string_on_connect_child_default,
      .on_connect_sibling = (handler != NULL && handler->on_connect_sibling != NULL)
                                ? handler->on_connect_sibling
                                : tree_to_string_on_connect_sibling_default,
      .ctx = (handler != NULL) ? handler->ctx : NULL,
  };
  tree_to_string_on_visit_ctx_t ctx = {
      .tree = tree,
      .result = result,
      .handler = &real_handler,
  };
  return_value_if_fail(tree != NULL && result != NULL, RET_BAD_PARAMS);

  if (node == NULL) {
    node = tree->root;
  }
  ctx.root = node;

  str_clear(result);

  return tree_foreach(tree, node, TREE_FOREACH_TYPE_PREORDER, tree_to_string_on_visit, &ctx);
}

inline static ret_t tree_set_node_allocator_impl(tree_t* tree, mem_allocator_t* allocator,
                                                 bool_t shared) {
  return_value_if_fail(tree != NULL, RET_BAD_PARAMS);

  if (tree->node_allocator != allocator) {
    return_value_if_fail(tree_is_empty(tree, NULL), RET_FAIL);

    if (tree->node_allocator != NULL && !tree->node_allocator_is_shared) {
      mem_allocator_destroy(tree->node_allocator);
    }
    tree->node_allocator = allocator;
  }
  tree->node_allocator_is_shared = shared;

  return RET_OK;
}

ret_t tree_set_node_allocator(tree_t* tree, mem_allocator_t* allocator) {
  return tree_set_node_allocator_impl(tree, allocator, FALSE);
}

ret_t tree_set_shared_node_allocator(tree_t* tree, mem_allocator_t* allocator) {
  return tree_set_node_allocator_impl(tree, allocator, TRUE);
}

ret_t tree_deinit(tree_t* tree) {
  ret_t ret = RET_OK;
  return_value_if_fail(tree != NULL, RET_BAD_PARAMS);

  ret = tree_clear(tree);
  if (RET_OK == ret) {
    tree_set_node_allocator(tree, NULL);
  }
  return ret;
}

ret_t tree_destroy(tree_t* tree) {
  return_value_if_fail(tree != NULL, RET_BAD_PARAMS);

  tree_deinit(tree);
  TKMEM_FREE(tree);

  return RET_OK;
}
