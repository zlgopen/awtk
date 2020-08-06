/**
 * File:   slist.h
 * Author: AWTK Develop Team
 * Brief:  single link list
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SLIST_H
#define TK_SLIST_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef struct _slist_node_t {
  struct _slist_node_t* next;
  void* data;
} slist_node_t;

/**
 * @class slist_t
 * 单向链表
 *
 * 用slist\_init初始化时，用slist\_deinit释放。如：
 *
 * ```c
 * slist_t slist;
 * slist_init(&slist, destroy, compare);
 * ...
 * slist_deinit(&slist);
 * ```
 *
 * 用slist\_create创建时，用slist\_destroy销毁。如：
 *
 * ```c
 * slist_t* slist = slist_create(destroy, compare);
 * ...
 * slist_destroy(slist);
 * ```
 *
 */
typedef struct _slist_t {
  /**
   * @property {slist_node_t*} first
   * @annotation ["readable"]
   * 首节点。
   */
  slist_node_t* first;

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
} slist_t;

/**
 * @method slist_create
 * @annotation ["constructor"]
 * 创建slist对象
 * @param {tk_destroy_t} destroy 元素销毁函数。
 * @param {tk_compare_t} compare 元素比较函数。
 *
 * @return {slist_t*} 单向链表对象。
 */
slist_t* slist_create(tk_destroy_t destroy, tk_compare_t compare);

/**
 * @method slist_init
 * 初始化slist对象
 * @param {slist_t*} slist 单向链表对象。
 * @param {tk_destroy_t} destroy 元素销毁函数。
 * @param {tk_compare_t} compare 元素比较函数。
 *
 * @return {slist_t*} 单向链表对象。
 */
slist_t* slist_init(slist_t* slist, tk_destroy_t destroy, tk_compare_t compare);

/**
 * @method slist_find
 * 查找第一个满足条件的元素。
 * @param {slist_t*} slist 单向链表对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {void*} 如果找到，返回满足条件的对象，否则返回NULL。
 */
void* slist_find(slist_t* slist, void* ctx);

/**
 * @method slist_remove
 * 删除第一个满足条件的元素。
 * @param {slist_t*} slist 单向链表对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slist_remove(slist_t* slist, void* ctx);

/**
 * @method slist_remove_all
 * 删除全部元素。
 * @param {slist_t*} slist 单向链表对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slist_remove_all(slist_t* slist);

/**
 * @method slist_append
 * 在尾巴追加一个元素。
 * @param {slist_t*} slist 单向链表对象。
 * @param {void*} data 待追加的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slist_append(slist_t* slist, void* data);

/**
 * @method slist_prepend
 * 在头部追加一个元素。
 * @param {slist_t*} slist 单向链表对象。
 * @param {void*} data 待追加的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slist_prepend(slist_t* slist, void* data);

/**
 * @method slist_foreach
 * 遍历元素。
 * @param {slist_t*} slist 单向链表对象。
 * @param {tk_visit_t} visit 遍历函数。
 * @param {void*} ctx 遍历函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slist_foreach(slist_t* slist, tk_visit_t visit, void* ctx);

/**
 * @method slist_tail_pop
 * 弹出最后一个元素。
 * @param {slist_t*} slist 单向链表对象。
 *
 * @return {ret_t} 成功返回最后一个元素，失败返回NULL。
 */
void* slist_tail_pop(slist_t* slist);

/**
 * @method slist_head_pop
 * 弹出第一个元素。
 * @param {slist_t*} slist 单向链表对象。
 *
 * @return {ret_t} 成功返回最后一个元素，失败返回NULL。
 */
void* slist_head_pop(slist_t* slist);

/**
 * @method slist_size
 * 返回元素个数。
 * @param {slist_t*} slist 单向链表对象。
 *
 * @return {int32_t} 返回元素个数。
 */
int32_t slist_size(slist_t* slist);

/**
 * @method slist_count
 * 返回满足条件元素的个数。
 * @param {slist_t*} slist 单向链表对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {int32_t} 返回元素个数。
 */
int32_t slist_count(slist_t* slist, void* ctx);

/**
 * @method slist_deinit
 * 清除单向链表中的元素。
 * @param {slist_t*} slist 单向链表对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slist_deinit(slist_t* slist);

/**
 * @method slist_destroy
 * 清除单向链表中的元素，并释放单向链表对象。
 * @param {slist_t*} slist 单向链表对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t slist_destroy(slist_t* slist);

END_C_DECLS

#endif /*TK_SLIST_H*/
