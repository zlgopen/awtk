/**
 * File:   dlist.h
 * Author: AWTK Develop Team
 * Brief:  doubly linked list
 *
 * Copyright (c) 2024 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-09-30 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_DLIST_H
#define TK_DLIST_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef struct _dlist_node_t dlist_node_t;

struct _dlist_node_t {
  dlist_node_t* prev;
  dlist_node_t* next;
  void* data;
};

/**
 * @class dlist_t
 * 双向链表
 *
 * 用dlist\_init初始化时，用dlist\_deinit释放。如：
 *
 * ```c
 * dlist_t dlist;
 * dlist_init(&dlist, destroy, compare);
 * ...
 * dlist_deinit(&dlist);
 * ```
 *
 * 用dlist\_create创建时，用dlist\_destroy销毁。如：
 *
 * ```c
 * dlist_t* dlist = dlist_create(destroy, compare);
 * ...
 * dlist_destroy(dlist);
 * ```
 *
 */
typedef struct _dlist_t {
  /**
   * @property {dlist_node_t*} first
   * @annotation ["readable"]
   * 首节点。
   */
  dlist_node_t* first;

  /**
   * @property {dlist_node_t*} last
   * @annotation ["readable"]
   * 尾节点。
   */
  dlist_node_t* last;

  /**
   * @property {int32_t} size
   * @annotation ["readable"]
   * 元素个数。
   */
  int32_t size;

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
} dlist_t;

/**
 * @method dlist_create
 * @annotation ["constructor"]
 * 创建dlist对象
 * @param {tk_destroy_t} destroy 元素销毁函数。
 * @param {tk_compare_t} compare 元素比较函数。
 *
 * @return {dlist_t*} 双向链表对象。
 */
dlist_t* dlist_create(tk_destroy_t destroy, tk_compare_t compare);

/**
 * @method dlist_init
 * 初始化dlist对象
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {tk_destroy_t} destroy 元素销毁函数。
 * @param {tk_compare_t} compare 元素比较函数。
 *
 * @return {dlist_t*} 双向链表对象。
 */
dlist_t* dlist_init(dlist_t* dlist, tk_destroy_t destroy, tk_compare_t compare);

/**
 * @method dlist_find
 * 查找第一个满足条件的元素。
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {void*} 如果找到，返回满足条件的对象，否则返回NULL。
 */
void* dlist_find(dlist_t* dlist, void* ctx);

/**
 * @method dlist_find_last
 * 查找最后一个满足条件的元素。
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {void*} 如果找到，返回满足条件的对象，否则返回NULL。
 */
void* dlist_find_last(dlist_t* dlist, void* ctx);

/**
 * @method dlist_find_ex
 * 查找第一个满足条件的元素。
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {tk_compare_t} compare 元素比较函数。
 * @param {void*} ctx 比较函数的上下文。
 * @param {bool_t} reverse 逆序查找。
 *
 * @return {void*} 返回节点。
 */
void* dlist_find_ex(dlist_t* dlist, tk_compare_t compare, void* ctx, bool_t reverse);

/**
 * @method dlist_remove
 * 删除第一个满足条件的元素。
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dlist_remove(dlist_t* dlist, void* ctx);

/**
 * @method dlist_remove_last
 * 删除最后一个满足条件的元素。
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dlist_remove_last(dlist_t* dlist, void* ctx);

/**
 * @method dlist_remove_ex
 * 删除满足条件的元素。
 * 备注：
 * 如果队列中符合条件的元素不足 remove_size，移除最后一个符合条件的元素后返回 RET_OK。
 * 如果队列中符合条件的元素大于 remove_size，在队列中移除 remove_size 个元素后返回 RET_OK。
 * remove_size 为负数则会移除所有符合条件的元素。
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {tk_compare_t} compare 元素比较函数。
 * @param {void*} ctx 比较函数的上下文。
 * @param {int32_t} remove_size 删除个数。
 * @param {bool_t} reverse 逆序删除。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dlist_remove_ex(dlist_t* dlist, tk_compare_t compare, void* ctx, int32_t remove_size,
                      bool_t reverse);

/**
 * @method dlist_remove_all
 * 删除全部元素。
 * @param {dlist_t*} dlist 双向链表对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dlist_remove_all(dlist_t* dlist);

/**
 * @method dlist_append
 * 在尾巴追加一个元素。
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {void*} data 待追加的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dlist_append(dlist_t* dlist, void* data);

/**
 * @method dlist_insert
 * 插入一个元素。
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {uint32_t} index 位置序数。
 * @param {void*} data 待追加的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dlist_insert(dlist_t* dlist, uint32_t index, void* data);

/**
 * @method dlist_prepend
 * 在头部追加一个元素。
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {void*} data 待追加的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dlist_prepend(dlist_t* dlist, void* data);

/**
 * @method dlist_foreach
 * 遍历元素。
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {tk_visit_t} visit 遍历函数。
 * @param {void*} ctx 遍历函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dlist_foreach(dlist_t* dlist, tk_visit_t visit, void* ctx);

/**
 * @method dlist_foreach_reverse
 * 反向遍历元素。
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {tk_visit_t} visit 遍历函数。
 * @param {void*} ctx 遍历函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dlist_foreach_reverse(dlist_t* dlist, tk_visit_t visit, void* ctx);

/**
 * @method dlist_tail_pop
 * 弹出最后一个元素。
 * @param {dlist_t*} dlist 双向链表对象。
 *
 * @return {void*} 成功返回最后一个元素，失败返回NULL。
 */
void* dlist_tail_pop(dlist_t* dlist);

/**
 * @method dlist_head_pop
 * 弹出第一个元素。
 * @param {dlist_t*} dlist 双向链表对象。
 *
 * @return {void*} 成功返回最后一个元素，失败返回NULL。
 */
void* dlist_head_pop(dlist_t* dlist);

/**
 * @method dlist_tail
 * 返回最后一个元素。
 * @param {dlist_t*} dlist 双向链表对象。
 *
 * @return {void*} 成功返回最后一个元素，失败返回NULL。
 */
void* dlist_tail(dlist_t* dlist);

/**
 * @method dlist_head
 * 返回第一个元素。
 * @param {dlist_t*} dlist 双向链表对象。
 *
 * @return {void*} 成功返回最后一个元素，失败返回NULL。
 */
void* dlist_head(dlist_t* dlist);

/**
 * @method dlist_is_empty
 * 列表是否为空。
 * @param {dlist_t*} dlist 双向链表对象。
 *
 * @return {bool_t} 返回 TRUE 表示空列表，返回 FALSE 表示列表有数据。
 */
bool_t dlist_is_empty(dlist_t* dlist);

/**
 * @method dlist_size
 * 返回元素个数。
 * @param {dlist_t*} dlist 双向链表对象。
 *
 * @return {int32_t} 返回元素个数。
 */
int32_t dlist_size(dlist_t* dlist);

/**
 * @method dlist_count
 * 返回满足条件元素的个数。
 * @param {dlist_t*} dlist 双向链表对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {int32_t} 返回元素个数。
 */
int32_t dlist_count(dlist_t* dlist, void* ctx);

/**
 * @method dlist_reverse
 * 反转链表。
 * @param {dlist_t*} dlist 双向链表对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dlist_reverse(dlist_t* dlist);

/**
 * @method dlist_deinit
 * 清除双向链表中的元素。
 * @param {dlist_t*} dlist 双向链表对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dlist_deinit(dlist_t* dlist);

/**
 * @method dlist_destroy
 * 清除双向链表中的元素，并释放双向链表对象。
 * @param {dlist_t*} dlist 双向链表对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dlist_destroy(dlist_t* dlist);

END_C_DECLS

#endif /*TK_DLIST_H*/
