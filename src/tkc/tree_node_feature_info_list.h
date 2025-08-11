/**
 * File:   tree_node_feature_info_list.h
 * Author: AWTK Develop Team
 * Brief:  树结点特征信息列表
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
 * 2025-08-08 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_TREE_NODE_FEATURE_INFO_LIST_H
#define TK_TREE_NODE_FEATURE_INFO_LIST_H

#include "tkc/types_def.h"
#include "tkc/slist.h"

BEGIN_C_DECLS

typedef ret_t (*tree_node_feature_init_t)(void* feature);
typedef ret_t (*tree_node_feature_deinit_t)(void* feature);

/**
 * @struct tree_node_feature_info_t
 * 树结点特征信息
 */
typedef struct _tree_node_feature_info_t {
  uint32_t size;
  tree_node_feature_init_t init;
  tree_node_feature_deinit_t deinit;
} tree_node_feature_info_t;

/**
 * @struct tree_node_feature_info_list_item_t
 * 树结点特征信息列表项
 */
typedef struct _tree_node_feature_info_list_item_t {
  const tree_node_feature_info_t* info;
  uint32_t offset;
} tree_node_feature_info_list_item_t;

/**
 * @class tree_node_feature_info_list_t
 * @parent slist_t
 * 树结点特征信息列表
 */
typedef struct _tree_node_feature_info_list_t {
  slist_t base;
  uint32_t features_size;
  void* owner; /* tree_t */
} tree_node_feature_info_list_t;

/**
 * @method tree_node_feature_info_list_create
 * @annotation ["constructor"]
 * 创建树结点特征信息列表
 * @return {tree_node_feature_info_list_t*} 返回树结点特征信息列表对象
 */
tree_node_feature_info_list_t* tree_node_feature_info_list_create(void);

/**
 * @method tree_node_feature_info_list_find
 * 查找树结点特征信息
 * @param {tree_node_feature_info_list_t*} list 树结点特征信息列表对象
 * @param {const tree_node_feature_info_t*} info 树结点特征信息
 *
 * @return {tree_node_feature_info_list_item_t*} 返回树结点特征信息列表项对象
 */
tree_node_feature_info_list_item_t* tree_node_feature_info_list_find(
    tree_node_feature_info_list_t* list, const tree_node_feature_info_t* info);

/**
 * @method tree_node_feature_info_list_append
 * 追加树结点特征信息
 * @param {tree_node_feature_info_list_t*} list 树结点特征信息列表对象
 * @param {const tree_node_feature_info_t*} info 树结点特征信息
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_node_feature_info_list_append(tree_node_feature_info_list_t* list,
                                         const tree_node_feature_info_t* info);
/**
 * @method tree_node_feature_info_list_remove
 * 移除树结点特征信息
 * @param {tree_node_feature_info_list_t*} list 树结点特征信息列表对象
 * @param {const tree_node_feature_info_t*} info 树结点特征信息
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_node_feature_info_list_remove(tree_node_feature_info_list_t* list,
                                         const tree_node_feature_info_t* info);

/**
 * @method tree_node_feature_info_list_clear
 * 清空树结点特征信息列表
 * @param {tree_node_feature_info_list_t*} list 树结点特征信息列表对象
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_node_feature_info_list_clear(tree_node_feature_info_list_t* list);

/**
 * @method tree_node_feature_info_list_destroy
 * @annotation ["deconstructor"]
 * 销毁树结点特征信息列表
 * @param {tree_node_feature_info_list_t*} list 树结点特征信息列表对象
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_node_feature_info_list_destroy(tree_node_feature_info_list_t* list);

/**
 * @method tree_node_feature_info_list_attach
 * @export none
 * 绑定树结点特征信息列表
 * @param {tree_node_feature_info_list_t*} list 树结点特征信息列表对象
 * @param {void*} owner 拥有者
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_node_feature_info_list_attach(tree_node_feature_info_list_t* list, void* owner);

/**
 * @method tree_node_feature_info_list_detach
 * @export none
 * 解绑树结点特征信息列表
 * @param {tree_node_feature_info_list_t*} list 树结点特征信息列表对象
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_node_feature_info_list_detach(tree_node_feature_info_list_t* list);

END_C_DECLS

#endif /*TK_TREE_NODE_FEATURE_INFO_LIST_H*/
