/**
 * File:   tree_node_feature_segment.h
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

#ifndef TK_TREE_NODE_FEATURE_SEGMENT_H
#define TK_TREE_NODE_FEATURE_SEGMENT_H

#include "tkc/tree.h"
#include "tkc/feature_info_list.h"

BEGIN_C_DECLS

/**
 * @class tree_node_feature_segment_t
 * 线段树结点特征
 */
typedef struct _tree_node_feature_segment_t {
  int32_t start;
  int32_t end;
} tree_node_feature_segment_t;

/**
 * @method tree_node_feature_segment_info
 * @annotation ["static"]
 * 线段树结点特征信息
 *
 * @return {const feature_info_list_feature_info_t*} 线段树结点特征信息。
 */
const feature_info_list_feature_info_t* tree_node_feature_segment_info(void);

/**
 * @class segment_tree_t
 * @parent tree_t
 * @annotation ["fake"]
 * 线段树结构
 */

/**
 * @method segment_tree_node_is_ancestor
 * 判断树结点是否为祖先结点
 * @param {tree_t*} tree 树结点对象。
 * @param {const tree_node_t*} node 树结点对象。
 * @param {const tree_node_t*} ancestor 祖先结点对象。
 *
 * @return {bool_t} 返回是否为祖先结点。
 */
bool_t segment_tree_node_is_ancestor(tree_t* tree, const tree_node_t* node,
                                     const tree_node_t* ancestor);

/**
 * @method segment_tree_update_range_by_order
 * 按遍历顺序更新范围
 * @param {tree_t*} tree 树结点对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t segment_tree_update_range_by_order(tree_t* tree);

END_C_DECLS

#endif /*TK_TREE_NODE_FEATURE_SEGMENT_H*/
