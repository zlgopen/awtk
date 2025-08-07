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

bool_t segment_tree_node_is_ancestor(tree_t* tree, const tree_node_t* node,
                                     const tree_node_t* ancestor) {
  uint32_t offset = 0;
  tree_node_feature_segment_t *node_feature = NULL, *ancestor_feature = NULL;
  return_value_if_fail(TREE_HAS_NODE_FEATURE(tree, tree_node_feature_segment_t), FALSE);
  return_value_if_fail(node != NULL && ancestor != NULL, FALSE);

  if (node == ancestor) {
    return FALSE;
  }

  offset = TREE_GET_NODE_FEATURE_OFFSET(tree, tree_node_feature_segment_t);
  node_feature = TREE_NODE_GET_SEGMENT_FEATURE(node, offset);
  ancestor_feature = TREE_NODE_GET_SEGMENT_FEATURE(ancestor, offset);

  return ancestor_feature->start <= node_feature->start &&
         node_feature->end <= ancestor_feature->end;
}

typedef struct _segment_tree_update_range_by_order_ctx_t {
  uint32_t node_feature_offset;
  int32_t index;
} segment_tree_update_range_by_order_ctx_t;

static ret_t segment_tree_update_range_by_order_on_visit(void* ctx, const void* data) {
  segment_tree_update_range_by_order_ctx_t* actx = (segment_tree_update_range_by_order_ctx_t*)ctx;
  const tree_node_t* node = (const tree_node_t*)data;
  tree_node_feature_segment_t* node_feature =
      TREE_NODE_GET_SEGMENT_FEATURE(node, actx->node_feature_offset);

  if (node->child == NULL) {
    node_feature->start = node_feature->end = actx->index;
    actx->index++;
  } else {
    const tree_node_t* iter = NULL;
    node_feature->start = INT32_MAX;
    node_feature->end = INT32_MIN;
    for (iter = tree_node_get_child((tree_node_t*)node, 0); iter != NULL;
         iter = iter->next_sibling) {
      tree_node_feature_segment_t* iter_feature =
          TREE_NODE_GET_SEGMENT_FEATURE(iter, actx->node_feature_offset);
      node_feature->start = tk_min(iter_feature->start, node_feature->start);
      node_feature->end = tk_max(iter_feature->end, node_feature->end);
    }
  }

  return RET_OK;
}

ret_t segment_tree_update_range_by_order(tree_t* tree) {
  segment_tree_update_range_by_order_ctx_t ctx = {.index = 0};
  return_value_if_fail(TREE_HAS_NODE_FEATURE(tree, tree_node_feature_segment_t), RET_BAD_PARAMS);

  ctx.node_feature_offset = TREE_GET_NODE_FEATURE_OFFSET(tree, tree_node_feature_segment_t);

  return tree_foreach(tree, NULL, TREE_FOREACH_TYPE_POSTORDER,
                      segment_tree_update_range_by_order_on_visit, &ctx);
}
