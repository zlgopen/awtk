/**
 * File:   array.h
 * Author: AWTK Develop Team
 * Brief:  dynamic array.
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_ARRAY_H
#define TK_ARRAY_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @class array_t
 * 动态数组，根据元素个数动态调整数组的容量。
 */
typedef struct _array_t {
  /**
   * @property {uint16_t} size
   * @annotation ["readable"]
   * 数组中元素的个数。
   */
  uint16_t size;
  /**
   * @property {uint16_t} capacity
   * @annotation ["readable"]
   * 数组的容量大小。
   */
  uint16_t capacity;
  /**
   * @property {void**} elms
   * @annotation ["readable"]
   * 数组中的元素。
   */
  void** elms;
} array_t;

/**
 * @method array_create
 * @annotation ["constructor"]
 * 创建array对象
 * @param {uint16_t*} capacity 数组的初始容量。
 *
 * @return {array_t*} 数组对象。
 */
array_t* array_create(uint16_t capacity);

/**
 * @method array_init
 * 初始化array对象
 * @param {array_t*} array 数组对象。
 * @param {uint16_t*} capacity 数组的初始容量。
 *
 * @return {array_t*} 数组对象。
 */
array_t* array_init(array_t* array, uint16_t capacity);

/**
 * @method array_find
 * 查找第一个满足条件的元素。
 * @param {array_t*} array 数组对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL时直接比较指针。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {void*} 如果找到，返回满足条件的对象，否则返回NULL。
 */
void* array_find(array_t* array, tk_compare_t cmp, void* ctx);

/**
 * @method array_find_index
 * 查找第一个满足条件的元素，并返回位置。
 * @param {array_t*} array 数组对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL时直接比较指针。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {int} 如果找到，返回满足条件的对象的位置，否则返回-1。
 */
int array_find_index(array_t* array, tk_compare_t cmp, void* ctx);

/**
 * @method array_remove
 * 删除第一个满足条件的元素。
 * @param {array_t*} array 数组对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL时直接比较指针。
 * @param {void*} ctx 比较函数的上下文。
 * @param {tk_destroy_t} destroy 销毁元素的回调函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t array_remove(array_t* array, tk_compare_t cmp, void* ctx, tk_destroy_t destroy);

/**
 * @method array_remove_all
 * 删除全部满足条件的元素。
 * @param {array_t*} array 数组对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL时直接比较指针。
 * @param {void*} ctx 比较函数的上下文。
 * @param {tk_destroy_t} destroy 销毁元素的回调函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t array_remove_all(array_t* array, tk_compare_t cmp, void* ctx, tk_destroy_t destroy);

/**
 * @method array_pop
 * 弹出最后一个元素。
 * @param {array_t*} array 数组对象。
 *
 * @return {void*} 成功返回最后一个元素，失败返回NULL。
 */
void* array_pop(array_t* array);

/**
 * @method array_push
 * 在尾巴追加一个元素。
 * @param {array_t*} array 数组对象。
 * @param {void*} data 待追加的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t array_push(array_t* array, void* data);

/**
 * @method array_deinit
 * 清除数组中的元素。
 * @param {array_t*} array 数组对象。
 *
 * @return {void} 无。
 */
void array_deinit(array_t* array);

/**
 * @method array_destroy
 * 清除数组中的元素，并释放数组对象。
 * @param {array_t*} array 数组对象。
 *
 * @return {void} 无。
 */
void array_destroy(array_t* array);

END_C_DECLS

#endif /*TK_ARRAY_H*/
