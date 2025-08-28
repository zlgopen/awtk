/**
 * File:   tree_node_feature_segment.c
 * Author: AWTK Develop Team
 * Brief:  线段树结点特征
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-08-07 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "tkc/tree_node_feature_segment.h"

static const feature_info_list_feature_info_t s_tree_node_feature_segment_info = {
    .size = sizeof(tree_node_feature_segment_t),
};

const feature_info_list_feature_info_t* tree_node_feature_segment_info(void) {
  return &s_tree_node_feature_segment_info;
}

bool_t segment_tree_node_is_ancestor(tree_t* tree, const tree_node_t* node,
                                     const tree_node_t* ancestor) {
  tree_node_feature_segment_t *node_feature = NULL, *ancestor_feature = NULL;
  return_value_if_fail(tree_has_node_feature(tree, &s_tree_node_feature_segment_info), FALSE);
  return_value_if_fail(node != NULL && ancestor != NULL, FALSE);

  if (node == ancestor) {
    return FALSE;
  }

  node_feature = (tree_node_feature_segment_t*)tree_get_node_feature(
      tree, node, &s_tree_node_feature_segment_info);
  ancestor_feature = (tree_node_feature_segment_t*)tree_get_node_feature(
      tree, ancestor, &s_tree_node_feature_segment_info);

  return ancestor_feature->start <= node_feature->start &&
         node_feature->end <= ancestor_feature->end;
}

typedef struct _segment_tree_update_range_by_order_ctx_t {
  tree_t* tree;
  int32_t index;
} segment_tree_update_range_by_order_ctx_t;

static ret_t segment_tree_update_range_by_order_on_visit(void* ctx, const void* data) {
  segment_tree_update_range_by_order_ctx_t* actx = (segment_tree_update_range_by_order_ctx_t*)ctx;
  const tree_node_t* node = (const tree_node_t*)data;
  tree_node_feature_segment_t* node_feature = (tree_node_feature_segment_t*)tree_get_node_feature(
      actx->tree, node, &s_tree_node_feature_segment_info);

  if (node->child == NULL) {
    node_feature->start = node_feature->end = actx->index;
    actx->index++;
  } else {
    uint32_t i = 0;
    const tree_node_t* iter = NULL;
    node_feature->start = INT32_MAX;
    node_feature->end = INT32_MIN;
    for (iter = tree_node_get_child((tree_node_t*)node, 0), i = 0; iter != NULL;
         iter = iter->next_sibling, i++) {
      tree_node_feature_segment_t* iter_feature =
          (tree_node_feature_segment_t*)tree_get_node_feature(actx->tree, iter,
                                                              &s_tree_node_feature_segment_info);
      node_feature->start = tk_min(iter_feature->start, node_feature->start);
      node_feature->end = tk_max(iter_feature->end, node_feature->end);
    }

    /* 如果只有一个子结点，那么假定多一个假的子结点，让其保证结点的区间范围比子结点的大 */
    if (i < 2) {
      node_feature->end = actx->index;
      actx->index++;
    }
  }

  return RET_OK;
}

ret_t segment_tree_update_range_by_order(tree_t* tree) {
  segment_tree_update_range_by_order_ctx_t ctx = {.tree = tree};
  return_value_if_fail(tree_has_node_feature(tree, &s_tree_node_feature_segment_info), FALSE);

  return tree_foreach(tree, NULL, TREE_FOREACH_TYPE_POSTORDER,
                      segment_tree_update_range_by_order_on_visit, &ctx);
}
