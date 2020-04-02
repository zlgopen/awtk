/**
 * File:   darray.h
 * Author: AWTK Develop Team
 * Brief:  dynamic darray.
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DARRAY_H
#define TK_DARRAY_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class darray_t
 * 动态数组，根据元素个数动态调整数组的容量。
 *
 * 用darray\_init初始化时，用darray\_deinit释放。如：
 *
 * ```c
 * darray_t darray;
 * darray_init(&darray, 10, destroy, compare);
 * ...
 * darray_deinit(&darray);
 * ```
 *
 * 用darray\_create创建时，用darray\_destroy销毁。如：
 *
 * ```c
 * darray_t* darray = darray_create(10, destroy, compare);
 * ...
 * darray_destroy(darray);
 * ```
 *
 */
typedef struct _darray_t {
  /**
   * @property {uint32_t} size
   * @annotation ["readable"]
   * 数组中元素的个数。
   */
  uint32_t size;
  /**
   * @property {uint32_t} capacity
   * @annotation ["readable"]
   * 数组的容量大小。
   */
  uint32_t capacity;
  /**
   * @property {void**} elms
   * @annotation ["readable"]
   * 数组中的元素。
   */
  void** elms;
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
} darray_t;

/**
 * @method darray_create
 * @annotation ["constructor"]
 * 创建darray对象。
 *
 * @param {uint32_t} capacity 数组的初始容量。
 * @param {tk_destroy_t} destroy 元素销毁函数。
 * @param {tk_compare_t} compare 元素比较函数。
 *
 * @return {darray_t*} 数组对象。
 */
darray_t* darray_create(uint32_t capacity, tk_destroy_t destroy, tk_compare_t compare);

/**
 * @method darray_init
 * 初始化darray对象。
 *
 * @param {darray_t*} darray 数组对象。
 * @param {uint32_t*} capacity 数组的初始容量。
 * @param {tk_destroy_t} destroy 元素销毁函数。
 * @param {tk_compare_t} compare 元素比较函数。
 *
 * @return {darray_t*} 数组对象。
 */
darray_t* darray_init(darray_t* darray, uint32_t capacity, tk_destroy_t destroy,
                      tk_compare_t compare);

/**
 * @method darray_find
 * 查找第一个满足条件的元素。
 * @param {darray_t*} darray 数组对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {void*} 如果找到，返回满足条件的对象，否则返回NULL。
 */
void* darray_find(darray_t* darray, void* ctx);

/**
 * @method darray_bsearch_index
 * 二分查找(确保数组有序)。
 * 
 * @param {darray_t*} darray 数组对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {int} 如果找到，返回满足条件的对象的位置，否则返回-1。
 */
int darray_bsearch_index(darray_t* darray, tk_compare_t cmp, void* ctx);

/**
 * @method darray_bsearch
 * 二分查找(确保数组有序)。
 * 
 * @param {darray_t*} darray 数组对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {void*} 如果找到，返回满足条件的对象，否则返回NULL。
 */
void* darray_bsearch(darray_t* darray, tk_compare_t cmp, void* ctx);

/**
 * @method darray_get
 * 获取指定序数的元素。
 * @param {darray_t*} darray 数组对象。
 * @param {uint32_t} index 序数。
 *
 * @return {void*} 返回满足条件的对象，否则返回NULL。
 */
void* darray_get(darray_t* darray, uint32_t index);

/**
 * @method darray_find_index
 * 查找第一个满足条件的元素，并返回位置。
 * @param {darray_t*} darray 数组对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {int} 如果找到，返回满足条件的对象的位置，否则返回-1。
 */
int darray_find_index(darray_t* darray, void* ctx);

/**
 * @method darray_remove
 * 删除第一个满足条件的元素。
 * @param {darray_t*} darray 数组对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t darray_remove(darray_t* darray, void* ctx);

/**
 * @method darray_remove_index
 * 删除指定位置的元素。
 * @param {darray_t*} darray 数组对象。
 * @param {uint32_t} index 位置序数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t darray_remove_index(darray_t* darray, uint32_t index);

/**
 * @method darray_remove_all
 * 删除全部满足条件的元素。
 * @param {darray_t*} darray 数组对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t darray_remove_all(darray_t* darray, tk_compare_t cmp, void* ctx);

/**
 * @method darray_sort
 * 排序。
 * @param {darray_t*} darray 数组对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t darray_sort(darray_t* darray, tk_compare_t cmp);

/**
 * @method darray_find_all
 * 查找全部满足条件的元素。
 *
 * ```
 * darray_t matched;
 * darray_init(&matched, 0, NULL, NULL);
 * darray_find_all(darray, mycmp, myctx, &matched);
 * ...
 * darray_deinit(&matched);
 *
 * ```
 * @param {darray_t*} darray 数组对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 * @param {void*} ctx 比较函数的上下文。
 * @param {darray_t*} matched 返回满足条件的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t darray_find_all(darray_t* darray, tk_compare_t cmp, void* ctx, darray_t* matched);

/**
 * @method darray_pop
 * 弹出最后一个元素。
 * @param {darray_t*} darray 数组对象。
 *
 * @return {void*} 成功返回最后一个元素，失败返回NULL。
 */
void* darray_pop(darray_t* darray);

/**
 * @method darray_tail
 * 返回最后一个元素。
 * @param {darray_t*} darray 数组对象。
 *
 * @return {void*} 成功返回最后一个元素，失败返回NULL。
 */
void* darray_tail(darray_t* darray);

/**
 * @method darray_head
 * 返回第一个元素。
 * @param {darray_t*} darray 数组对象。
 *
 * @return {void*} 成功返回最后一个元素，失败返回NULL。
 */
void* darray_head(darray_t* darray);

/**
 * @method darray_push
 * 在尾巴追加一个元素。
 * @param {darray_t*} darray 数组对象。
 * @param {void*} data 待追加的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t darray_push(darray_t* darray, void* data);

/**
 * @method darray_count
 * 返回满足条件元素的个数。
 * @param {darray_t*} darray 单向链表对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {int32_t} 返回元素个数。
 */
int32_t darray_count(darray_t* darray, void* ctx);

/**
 * @method darray_clear
 * 清除全部元素。
 * @param {darray_t*} darray 数组对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t darray_clear(darray_t* darray);

/**
 * @method darray_foreach
 * 遍历元素。
 * @param {darray_t*} darray 数组对象。
 * @param {tk_visit_t} visit 遍历函数。
 * @param {void*} ctx 遍历函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t darray_foreach(darray_t* darray, tk_visit_t visit, void* ctx);

/**
 * @method darray_deinit
 * 清除全部元素，并释放elms。
 * @param {darray_t*} darray 数组对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t darray_deinit(darray_t* darray);

/**
 * @method darray_destroy
 * 销毁darray对象。
 * @param {darray_t*} darray 数组对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t darray_destroy(darray_t* darray);

END_C_DECLS

#endif /*TK_DARRAY_H*/
