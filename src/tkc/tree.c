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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/tree.h"
#include "tkc/slist.h"

inline static tree_node_t* tree_node_get_first_sibling(tree_node_t* node) {
  while (node != NULL && node->prev_sibling != NULL) {
    node = node->prev_sibling;
  }
  return node;
}

inline static int32_t tree_node_sibling_size(const tree_node_t* first_sibling) {
  int32_t ret = 0;
  const tree_node_t* node = first_sibling;
  for (; node != NULL; node = node->next_sibling) {
    ret++;
  }
  return ret;
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

ret_t tree_node_destroy(tree_node_t* node, tk_destroy_t destroy) {
  return_value_if_fail(node != NULL && destroy != NULL, RET_BAD_PARAMS);

  destroy(node->data);
  TKMEM_FREE(node);

  return RET_OK;
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
  tree_find_on_visit_ctx_t actx = {.tree = tree, .data = ctx, .compare = compare};
  return_value_if_fail(tree != NULL && compare != NULL, NULL);

  tree_foreach(tree, node, foreach_type, tree_find_on_visit, &actx);

  return actx.result;
}

typedef struct _tree_remove_on_visit_ctx_t {
  tree_t* tree;
  void* data;
  tk_compare_t compare;
  int32_t remove_size;
  slist_t* remove_list;
} tree_remove_on_visit_ctx_t;

static ret_t tree_remove_on_visit(void* ctx, const void* data) {
  tree_remove_on_visit_ctx_t* actx = (tree_remove_on_visit_ctx_t*)(ctx);
  const tree_node_t* node = (const tree_node_t*)(data);

  if (actx->remove_size == 0) {
    return RET_STOP;
  }

  if (0 == actx->compare(node->data, actx->data)) {
    slist_append(actx->remove_list, (tree_node_t*)node);
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
  slist_t remove_list;
  tree_node_t* iter = NULL;
  tree_remove_on_visit_ctx_t actx = {
      .tree = tree,
      .data = ctx,
      .compare = compare,
      .remove_size = remove_size,
      .remove_list = &remove_list,
  };
  return_value_if_fail(tree != NULL && compare != NULL, RET_BAD_PARAMS);

  slist_init(&remove_list, NULL, NULL);

  ret = tree_foreach(tree, node, foreach_type, tree_remove_on_visit, &actx);

  while ((iter = slist_head_pop(&remove_list)) != NULL) {
    tree_remove_node(tree, iter);
  }

  slist_deinit(&remove_list);

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
  tree_node_t* ret = NULL;
  return_value_if_fail(tree != NULL && node != NULL, NULL);

  if (node->child != NULL) {
    ret = tree_get_sibling_node(tree, node->child, index);
  }

  return ret;
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
  uint32_t i = 0;
  int32_t size = 0;
  tree_node_t* iter = NULL;
  tree_node_t* ret = NULL;
  return_value_if_fail(tree != NULL && node != NULL, NULL);

  iter = tree_node_get_first_sibling(node);
  size = tree_node_sibling_size(iter);
  index = tk_min(index, size);

  for (i = 0; iter != NULL; iter = iter->next_sibling, i++) {
    if (i == index) {
      ret = iter;
      break;
    }
  }

  return ret;
}

ret_t tree_insert_sibling_node(tree_t* tree, tree_node_t* node, uint32_t index,
                               tree_node_t* sibling) {
  ret_t ret = RET_FAIL;
  tree_node_t *iter = NULL, *prev = NULL;
  int32_t size = 0;
  uint32_t i = 0;
  return_value_if_fail(tree != NULL && node != NULL && sibling != NULL, RET_BAD_PARAMS);
  return_value_if_fail(node != tree->root, RET_FAIL);

  iter = tree_node_get_first_sibling(node);
  size = tree_node_sibling_size(iter);
  index = tk_min(index, size);

  for (i = 0; iter != NULL || prev != NULL; i++) {
    if (i == index) {
      ret = tree_link_sibling(tree, prev, iter, sibling);
      break;
    }

    prev = iter;
    if (iter != NULL) {
      iter = iter->next_sibling;
    }
  }

  return ret;
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

static ret_t tree_foreach_breadth_first(tree_t* tree, tree_node_t* node, tk_visit_t visit,
                                        void* ctx) {
  return_value_if_fail(tree != NULL && visit != NULL, RET_BAD_PARAMS);

  if (node == NULL) {
    node = tree->root;
  }

  if (node != NULL) {
    slist_t queue;
    slist_init(&queue, NULL, NULL);
    slist_append(&queue, node);

    while (!slist_is_empty(&queue)) {
      tree_node_t* child = NULL;
      tree_node_t* iter = (tree_node_t*)slist_head_pop(&queue);
      ret_t ret = visit(ctx, iter);

      if (ret != RET_OK) {
        break;
      }

      for (child = tree_node_get_first_sibling(iter->child); child != NULL;
           child = child->next_sibling) {
        slist_append(&queue, child);
      }
    }

    slist_deinit(&queue);
  }

  return RET_OK;
}

static ret_t tree_foreach_preorder(tree_t* tree, tree_node_t* node, tk_visit_t visit, void* ctx) {
  return_value_if_fail(tree != NULL && visit != NULL, RET_BAD_PARAMS);

  if (node == NULL) {
    node = tree->root;
  }

  if (node != NULL) {
    slist_t stack;
    slist_init(&stack, NULL, NULL);
    slist_prepend(&stack, node);

    while (!slist_is_empty(&stack)) {
      tree_node_t* child = NULL;
      tree_node_t* iter = (tree_node_t*)slist_head_pop(&stack);
      ret_t ret = visit(ctx, iter);

      if (ret != RET_OK) {
        break;
      }

      if (iter->next_sibling != NULL) {
        slist_prepend(&stack, iter->next_sibling);
      }

      child = tree_node_get_first_sibling(iter->child);
      if (child != NULL) {
        slist_prepend(&stack, child);
      }
    }

    slist_deinit(&stack);
  }

  return RET_OK;
}

static ret_t tree_foreach_postorder(tree_t* tree, tree_node_t* node, tk_visit_t visit, void* ctx) {
  return_value_if_fail(tree != NULL && visit != NULL, RET_BAD_PARAMS);

  if (node == NULL) {
    node = tree->root;
  }

  if (node != NULL) {
    slist_t stack;
    slist_t result;
    slist_init(&stack, NULL, NULL);
    slist_init(&result, NULL, NULL);
    slist_prepend(&stack, node);

    while (!slist_is_empty(&stack)) {
      tree_node_t* child = NULL;
      tree_node_t* iter = (tree_node_t*)slist_head_pop(&stack);
      slist_prepend(&result, iter);

      for (child = tree_node_get_first_sibling(iter->child); child != NULL;
           child = child->next_sibling) {
        slist_prepend(&stack, child);
      }
    }

    slist_foreach(&result, visit, ctx);

    slist_deinit(&stack);
    slist_deinit(&result);
  }

  return RET_OK;
}

ret_t tree_foreach(tree_t* tree, tree_node_t* node, tree_foreach_type_t foreach_type,
                   tk_visit_t visit, void* ctx) {
  switch (foreach_type) {
    case TREE_FOREACH_TYPE_BREADTH_FIRST:
      return tree_foreach_breadth_first(tree, node, visit, ctx);
    case TREE_FOREACH_TYPE_PREORDER:
      return tree_foreach_preorder(tree, node, visit, ctx);
    case TREE_FOREACH_TYPE_POSTORDER:
      return tree_foreach_postorder(tree, node, visit, ctx);
    default:
      ENSURE(!"Not support foreach type!");
      return RET_NOT_IMPL;
  }
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
  tree_foreach(tree, node, TREE_FOREACH_TYPE_BREADTH_FIRST, tree_size_on_visit, &ret);
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

  return depth >= 0 ? depth : -1;
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

  tree_foreach(tree, node, TREE_FOREACH_TYPE_BREADTH_FIRST, tree_height_on_visit, &ctx);

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

  tree_foreach(tree, tree->root, TREE_FOREACH_TYPE_BREADTH_FIRST, tree_degree_on_visit, &ret);

  return ret;
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
