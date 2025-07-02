/**
 * File:   tree.h
 * Author: AWTK Develop Team
 * Brief:  树结构
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
 * 2025-03-26 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_TREE_H
#define TK_TREE_H

#include "tkc/types_def.h"
#include "tkc/mem_allocator.h"
#include "tkc/str.h"

BEGIN_C_DECLS

/**
 * @class tree_node_t
 * 树结点
 */
typedef struct _tree_node_t tree_node_t;

struct _tree_node_t {
  tree_node_t* parent;
  tree_node_t* child;
  tree_node_t* prev_sibling;
  tree_node_t* next_sibling;
  void* data;
};

/**
 * @method tree_node_degree
 * 获取树结点的度（子节点数量）
 * @param {tree_node_t*} node 树结点对象。
 *
 * @return {int32_t} 返回树结点的度。
 */
int32_t tree_node_degree(tree_node_t* node);

/**
 * @method tree_node_is_ancestor
 * 判断树结点是否为祖先结点
 * @param {const tree_node_t*} node 树结点对象。
 * @param {const tree_node_t*} ancestor 祖先结点对象。
 *
 * @return {bool_t} 返回是否为祖先结点。
 */
bool_t tree_node_is_ancestor(const tree_node_t* node, const tree_node_t* ancestor);

/**
 * @method tree_node_get_lowest_common_ancestor
 * 获取两个树节点的最近公共祖先(LCA)结点。
 * @param {tree_node_t*} node1 树结点对象。
 * @param {tree_node_t*} node2 树结点对象。
 *
 * @return {tree_node_t*} 返回最近公共祖先节点。
 */
tree_node_t* tree_node_get_lowest_common_ancestor(tree_node_t* node1, tree_node_t* node2);

/**
 * @method tree_node_get_sibling
 * 获取指定位置的兄弟节点。
 * @param {tree_node_t*} node 树结点对象。
 * @param {uint32_t} index 索引。
 *
 * @return {tree_node_t*} 返回兄弟节点。
 */
tree_node_t* tree_node_get_sibling(tree_node_t* node, uint32_t index);

/**
 * @method tree_node_get_child
 * 获取指定位置的子节点。
 * @param {tree_node_t*} node 树结点对象。
 * @param {uint32_t} index 索引。
 *
 * @return {tree_node_t*} 返回子节点。
 */
tree_node_t* tree_node_get_child(tree_node_t* node, uint32_t index);

typedef ret_t (*tree_to_string_on_node_t)(const tree_node_t* node, str_t* result, void* ctx);
typedef ret_t (*tree_to_string_on_next_t)(const tree_node_t* node, str_t* result, void* ctx);
typedef ret_t (*tree_to_string_on_empty_t)(const tree_node_t* node, str_t* result, void* ctx);
typedef ret_t (*tree_to_string_on_connect_t)(const tree_node_t* node, str_t* result, void* ctx);
typedef ret_t (*tree_to_string_on_connect_extend_t)(const tree_node_t* node, str_t* result,
                                                    void* ctx);

/**
 * @struct tree_to_string_handler_t
 * 树结构转字符串处理。
 */
typedef struct _tree_to_string_handler_t {
  tree_to_string_on_node_t on_node;
  tree_to_string_on_next_t on_next;
  tree_to_string_on_empty_t on_empty;
  tree_to_string_on_connect_t on_connect;
  tree_to_string_on_connect_extend_t on_connect_extend;
  void* ctx;
} tree_to_string_handler_t;

/**
 * @enum tree_foreach_type_t
 * @prefix TREE_FOREACH_TYPE_
 * 树结构遍历类型。
 */
typedef enum _tree_foreach_type_t {
  /**
   * @const TREE_FOREACH_TYPE_BREADTH_FIRST
   * 广度优先遍历（层级遍历）
   */
  TREE_FOREACH_TYPE_BREADTH_FIRST = 0,
  /**
   * @const TREE_FOREACH_TYPE_PREORDER
   * 前序遍历（根-左-右）
   */
  TREE_FOREACH_TYPE_PREORDER,
  /**
   * @const TREE_FOREACH_TYPE_POSTORDER
   * 后序遍历（左-右-根）
   */
  TREE_FOREACH_TYPE_POSTORDER,
} tree_foreach_type_t;

/**
 * @class tree_t
 * 树结构
 *
 * 用tree\_init初始化时，用tree\_deinit释放。如：
 *
 * ```c
 * tree_t tree;
 * tree_init(&tree, destroy, compare);
 * ...
 * tree_deinit(&tree);
 * ```
 *
 * 用tree\_create创建时，用tree\_destroy销毁。如：
 *
 * ```c
 * tree_t* tree = tree_create(destroy, compare);
 * ...
 * tree_destroy(tree);
 * ```
 *
 */
typedef struct _tree_t {
  /**
   * @property {tree_node_t*} root
   * @annotation ["readable"]
   * 根节点。
   */
  tree_node_t* root;

  /**
   * @property {tk_destroy_t} destroy
   * @annotation ["readable"]
   * 元素销毁函数。
   */
  tk_destroy_t destroy;

  /**
   * @property {tk_compare_t} compare
   * @annotation ["readable"]
   * 元素比较函数。
   */
  tk_compare_t compare;

  /**
   * @property {mem_allocator_t*} node_allocator
   * 节点内存分配器。
   */
  mem_allocator_t* node_allocator;

  /**
   * @property {mem_allocator_t*} node_allocator_is_shared
   * 节点内存分配器是否共享。
   */
  bool_t node_allocator_is_shared;
} tree_t;

/**
 * @method tree_create
 * @annotation ["constructor"]
 * 创建tree对象
 * @param {tk_destroy_t} destroy 元素销毁函数。
 * @param {tk_compare_t} compare 元素比较函数。
 *
 * @return {tree_t*} 树结构对象。
 */
tree_t* tree_create(tk_destroy_t destroy, tk_compare_t compare);

/**
 * @method tree_init
 * 初始化tree对象
 * @param {tree_t*} tree 树结构对象。
 * @param {tk_destroy_t} destroy 元素销毁函数。
 * @param {tk_compare_t} compare 元素比较函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_init(tree_t* tree, tk_destroy_t destroy, tk_compare_t compare);

/**
 * @method tree_find
 * 查找第一个满足条件的元素。
 * @param {tree_t*} tree 树结构对象。
 * @param {tree_node_t*} node 节点（为NULL时，遍历整个树）。
 * @param {tree_foreach_type_t} foreach_type 遍历类型。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {tree_node_t*} 如果找到，返回满足条件的对象，否则返回NULL。
 */
tree_node_t* tree_find(tree_t* tree, tree_node_t* node, tree_foreach_type_t foreach_type,
                       void* ctx);

/**
 * @method tree_find_ex
 * 查找第一个满足条件的元素。
 * @param {tree_t*} tree 树结构对象。
 * @param {tree_node_t*} node 节点（为NULL时，遍历整个树）。
 * @param {tree_foreach_type_t} foreach_type 遍历类型。
 * @param {tk_compare_t} compare 元素比较函数。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {tree_node_t*} 返回节点。
 */
tree_node_t* tree_find_ex(tree_t* tree, tree_node_t* node, tree_foreach_type_t foreach_type,
                          tk_compare_t compare, void* ctx);

/**
 * @method tree_remove
 * 删除第一个满足条件的元素。
 * @param {tree_t*} tree 树结构对象。
 * @param {tree_node_t*} node 节点（为NULL时，遍历整个树）。
 * @param {tree_foreach_type_t} foreach_type 遍历类型。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_remove(tree_t* tree, tree_node_t* node, tree_foreach_type_t foreach_type, void* ctx);

/**
 * @method tree_remove_ex
 * 删除满足条件的元素。
 * 备注：
 * 如果树结构中符合条件的元素不足 remove_size，移除最后一个符合条件的元素后返回 RET_OK。
 * 如果树结构中符合条件的元素大于 remove_size，在树结构中移除 remove_size 个元素后返回 RET_OK。
 * remove_size 为负数则会移除所有符合条件的元素。
 * @param {tree_t*} tree 树结构对象。
 * @param {tree_node_t*} node 节点（为NULL时，遍历整个树）。
 * @param {tree_foreach_type_t} foreach_type 遍历类型。
 * @param {tk_compare_t} compare 元素比较函数。
 * @param {void*} ctx 比较函数的上下文。
 * @param {int32_t} remove_size 删除个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_remove_ex(tree_t* tree, tree_node_t* node, tree_foreach_type_t foreach_type,
                     tk_compare_t compare, void* ctx, int32_t remove_size);

/**
 * @method tree_create_node
 * 创建树结点对象。
 * @param {tree_t*} tree 树结构对象。
 * @param {void*} data 节点数据。
 *
 * @return {tree_node_t*} 树结点对象。
 */
tree_node_t* tree_create_node(tree_t* tree, void* data);

/**
 * @method tree_remove_node
 * 删除指定节点。
 * @param {tree_t*} tree 树结构对象。
 * @param {tree_node_t*} node 节点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_remove_node(tree_t* tree, tree_node_t* node);

/**
 * @method tree_unlink_node
 * 解绑节点（保持子树结构，仅解除与父节点和兄弟节点的链接）
 * @param {tree_t*} tree 树结构对象
 * @param {tree_node_t*} node 待解绑节点
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_unlink_node(tree_t* tree, tree_node_t* node);

/**
 * @method tree_clear
 * 清空树结构。
 * @param {tree_t*} tree 树结构对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_clear(tree_t* tree);

/**
 * @method tree_set_root
 * 设置根节点。
 * @param {tree_t*} tree 树结构对象
 * @param {tree_node_t*} root 根节点
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_set_root(tree_t* tree, tree_node_t* root);

/**
 * @method tree_insert_child_node
 * 插入子节点到指定位置
 * @param {tree_t*} tree 树结构对象
 * @param {tree_node_t*} node 节点 （为NULL时，插入到根节点）
 * @param {uint32_t} index 插入位置
 * @param {tree_node_t*} child 子节点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_insert_child_node(tree_t* tree, tree_node_t* node, uint32_t index, tree_node_t* child);

/**
 * @method tree_insert_sibling_node
 * 插入兄弟节点到指定位置
 * @param {tree_t*} tree 树结构对象
 * @param {tree_node_t*} node 节点
 * @param {uint32_t} index 插入位置
 * @param {tree_node_t*} sibling 兄弟节点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_insert_sibling_node(tree_t* tree, tree_node_t* node, uint32_t index,
                               tree_node_t* sibling);

/**
 * @method tree_append_child_node
 * 追加子节点
 * @param {tree_t*} tree 树结构对象
 * @param {tree_node_t*} node 节点 （为NULL时，插入到根节点）
 * @param {tree_node_t*} child 子节点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_append_child_node(tree_t* tree, tree_node_t* node, tree_node_t* child);

/**
 * @method tree_append_sibling_node
 * 追加兄弟节点
 * @param {tree_t*} tree 树结构对象
 * @param {tree_node_t*} node 节点
 * @param {tree_node_t*} sibling 兄弟节点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_append_sibling_node(tree_t* tree, tree_node_t* node, tree_node_t* sibling);

/**
 * @method tree_prepend_child_node
 * 头部追加子节点
 * @param {tree_t*} tree 树结构对象
 * @param {tree_node_t*} node 节点 （为NULL时，插入到根节点）
 * @param {tree_node_t*} child 子节点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_prepend_child_node(tree_t* tree, tree_node_t* node, tree_node_t* child);

/**
 * @method tree_prepend_sibling_node
 * 头部追加兄弟节点
 * @param {tree_t*} tree 树结构对象
 * @param {tree_node_t*} node 节点
 * @param {tree_node_t*} sibling 兄弟节点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_prepend_sibling_node(tree_t* tree, tree_node_t* node, tree_node_t* sibling);

/**
 * @method tree_foreach
 * 遍历元素。
 * @param {tree_t*} tree 树结构对象。
 * @param {tree_node_t*} node 节点（为NULL时，遍历整个树）。
 * @param {tree_foreach_type_t} foreach_type 遍历类型。
 * @param {tk_visit_t} visit 遍历函数。
 * @param {void*} ctx 遍历函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_foreach(tree_t* tree, tree_node_t* node, tree_foreach_type_t foreach_type,
                   tk_visit_t visit, void* ctx);

/**
 * @method tree_is_empty
 * 树节点是否为空。
 * @param {tree_t*} tree 树结构对象。
 * @param {tree_node_t*} node 节点（为NULL时，返回树是否为空）。
 *
 * @return {bool_t} 返回 TRUE 表示空树，返回 FALSE 表示树有数据。
 */
bool_t tree_is_empty(tree_t* tree, tree_node_t* node);

/**
 * @method tree_size
 * 返回树节点子节点个数。
 * @param {tree_t*} tree 树结构对象。
 * @param {tree_node_t*} node 节点（为NULL时，返回树的节点个数）。
 *
 * @return {int32_t} 返回树节点子节点个数。
 */
int32_t tree_size(tree_t* tree, tree_node_t* node);

/**
 * @method tree_depth
 * 获取树节点深度。
 * @param {tree_t*} tree 树结构对象。
 * @param {tree_node_t*} node 节点（为NULL时，为根节点）。
 *
 * @return {int32_t} 返回树节点深度。
 */
int32_t tree_depth(tree_t* tree, tree_node_t* node);

/**
 * @method tree_level
 * 获取树节点的层级。
 * @param {tree_t*} tree 树结构对象。
 * @param {tree_node_t*} node 节点（为NULL时，为根节点）。
 *
 * @return {int32_t} 返回树节点层级。
 */
int32_t tree_level(tree_t* tree, tree_node_t* node);

/**
 * @method tree_height
 * 获取树节点高度。
 * @param {tree_t*} tree 树结构对象。
 * @param {tree_node_t*} node 节点（为NULL时，返回树的高度）。
 *
 * @return {int32_t} 返回树节点高度。
 */
int32_t tree_height(tree_t* tree, tree_node_t* node);

/**
 * @method tree_degree
 * 获取树的度（树中所有节点的度的最大值）。
 * @param {tree_t*} tree 树结构对象。
 *
 * @return {int32_t} 返回树的宽度。
 */
int32_t tree_degree(tree_t* tree);

/**
 * @method tree_to_string
 * 树结构转换为字符串。
 * @param {tree_t*} tree 树结构对象。
 * @param {tree_node_t*} node 节点（为NULL时，遍历整个树）。
 * @param {str_t*} result 字符串对象。
 * @param {tree_to_string_handler_t*} handler 树结构转字符串处理。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_to_string(tree_t* tree, tree_node_t* node, str_t* result,
                     tree_to_string_handler_t* handler);

/**
 * @method tree_set_node_allocator
 * 设置节点内存分配器。
 * @param {tree_t*} tree 树结构对象。
 * @param {mem_allocator_t*} allocator 内存分配器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_set_node_allocator(tree_t* tree, mem_allocator_t* allocator);

/**
 * @method tree_set_shared_node_allocator
 * 设置共享节点内存分配器。
 * @param {tree_t*} tree 树结构对象。
 * @param {mem_allocator_t*} allocator 内存分配器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_set_shared_node_allocator(tree_t* tree, mem_allocator_t* allocator);

/**
 * @method tree_deinit
 * 清除树结构中的元素。
 * @param {tree_t*} tree 树结构对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_deinit(tree_t* tree);

/**
 * @method tree_destroy
 * @annotation ["deconstructor"]
 * 清除树结构中的元素，并释放树结构对象。
 * @param {tree_t*} tree 树结构对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tree_destroy(tree_t* tree);

END_C_DECLS

#endif /*TK_TREE_H*/
