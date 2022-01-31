/**
 * File:   hash_table.h
 * Author: AWTK Develop Team
 * Brief:  hash table
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-11-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_HASH_TABLE_H
#define TK_HASH_TABLE_H

#include "tkc/darray.h"

BEGIN_C_DECLS

/**
 * @class hash_table_t
 * 哈希表。
 *
 * 用hash_table\_init初始化时，用hash_table\_deinit释放。如：
 *
 * ```c
 * hash_table_t hash_table;
 * hash_table_init(&hash_table, 10, destroy, compare, hash);
 * ...
 * hash_table_deinit(&hash_table);
 * ```
 *
 * 用hash_table\_create创建时，用hash_table\_destroy销毁。如：
 *
 * ```c
 * hash_table_t* hash_table = hash_table_create(10, destroy, compare, hash);
 * ...
 * hash_table_destroy(hash_table);
 * ```
 *
 */
typedef struct _hash_table_t {
  /**
   * @property {darray_t} buckets
   * @annotation ["readable"]
   * buckets。
   */
  darray_t buckets;

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
   * @property {tk_hash_t} hash
   * @annotation ["readable"]
   * 元素哈希函数。
   */
  tk_hash_t hash;
} hash_table_t;

/**
 * @method hash_table_create
 * @annotation ["constructor"]
 * 创建hash_table对象。
 *
 * @param {uint32_t} capacity 哈希表桶数。
 * @param {tk_destroy_t} destroy 元素销毁函数。
 * @param {tk_compare_t} compare 元素比较函数。
 * @param {tk_hash_t} hash 元素哈希函数。
 *
 * @return {hash_table_t*} 哈希表对象。
 */
hash_table_t* hash_table_create(uint32_t capacity, tk_destroy_t destroy, tk_compare_t compare,
                                tk_hash_t hash);

/**
 * @method hash_table_init
 * 初始化hash_table对象。
 *
 * @param {hash_table_t*} hash_table 哈希表对象。
 * @param {uint32_t} capacity 哈希表桶数。
 * @param {tk_destroy_t} destroy 元素销毁函数。
 * @param {tk_compare_t} compare 元素比较函数。
 * @param {tk_hash_t} hash 元素哈希函数。
 *
 * @return {hash_table_t*} 哈希表对象。
 */
hash_table_t* hash_table_init(hash_table_t* hash_table, uint32_t capacity, tk_destroy_t destroy,
                              tk_compare_t compare, tk_hash_t hash);

/**
 * @method hash_table_find
 * 查找第一个满足条件的元素。
 * @param {hash_table_t*} hash_table 哈希表对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {void*} 如果找到，返回满足条件的对象，否则返回NULL。
 */
void* hash_table_find(hash_table_t* hash_table, tk_compare_t cmp, void* ctx);

/**
 * @method hash_table_add
 * 加入一个元素。
 * @param {hash_table_t*} hash_table 哈希表对象。
 * @param {void*} data 数据。
 * @param {bool_t} replace_if_exist 如果存在是否替换。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hash_table_add(hash_table_t* hash_table, void* data, bool_t replace_if_exist);

/**
 * @method hash_table_remove
 * 删除第一个满足条件的元素。
 * @param {hash_table_t*} hash_table 哈希表对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hash_table_remove(hash_table_t* hash_table, tk_compare_t cmp, void* ctx);

/**
 * @method hash_table_remove_all
 * 删除全部满足条件的元素。
 * @param {hash_table_t*} hash_table 哈希表对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hash_table_remove_all(hash_table_t* hash_table, tk_compare_t cmp, void* ctx);

/**
 * @method hash_table_count
 * 返回满足条件元素的个数。
 * @param {hash_table_t*} hash_table 单向链表对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {int32_t} 返回元素个数。
 */
int32_t hash_table_count(hash_table_t* hash_table, tk_compare_t cmp, void* ctx);

/**
 * @method hash_table_clear
 * 清除全部元素。
 * @param {hash_table_t*} hash_table 哈希表对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hash_table_clear(hash_table_t* hash_table);

/**
 * @method hash_table_foreach
 * 遍历元素。
 * @param {hash_table_t*} hash_table 哈希表对象。
 * @param {tk_visit_t} visit 遍历函数。
 * @param {void*} ctx 遍历函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hash_table_foreach(hash_table_t* hash_table, tk_visit_t visit, void* ctx);

/**
 * @method hash_table_deinit
 * 清除全部元素，并释放elms。
 * @param {hash_table_t*} hash_table 哈希表对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hash_table_deinit(hash_table_t* hash_table);

/**
 * @method hash_table_destroy
 * 销毁hash_table对象。
 * @param {hash_table_t*} hash_table 哈希表对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hash_table_destroy(hash_table_t* hash_table);

/**
 * @method hash_table_hash_str
 * 计算字符串hash值。
 * @param {const void*} data 数据。
 *
 * @return {uint32_t} 返回hash值。
 */
uint32_t hash_table_hash_str(const void* data);

/**
 * @method hash_table_hash_int
 * 计算int的hash值。
 * @param {const void*} data 数据。
 *
 * @return {uint32_t} 返回hash值。
 */
uint32_t hash_table_hash_int(const void* data);

END_C_DECLS

#endif /*TK_HASH_TABLE_H*/
