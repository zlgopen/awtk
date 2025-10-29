/**
 * File:   revertible_str.h
 * Author: AWTK Develop Team
 * Brief:  revertible_str
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-9-02 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#ifndef TK_revertible_str_H
#define TK_revertible_str_H

#include "tkc/types_def.h"
#include "tkc/str.h"

BEGIN_C_DECLS

/**
 * @class revertible_str_t
 * @annotation ["fake"]
 * 类似栈效果的字符串类。
 *
 * 该类有把上一次添加的字符串给回退的功能。（类似于压栈和出栈功能）
 *
 * 示例：
 *
 * ```c
 *  revertible_str_t s;
 *  revertible_str_init(&s, 10, 10);
 *
 *  revertible_str_append(&s, "abc");
 *  revertible_str_append(&s, "123");
 *  log_debug("%s\n", s.str);
 * 
 *  revertible_str_revert(&s);
 *  log_debug("%s\n", s.str);
 * 
 *  revertible_str_revert(&s);
 *  log_debug("%s\n", s.str);
 *
 *  revertible_str_deinit(&s);
 * ```
 */
typedef struct _revertible_str_t {
  str_t base;
  /**
   * @property {uint32_t*} offsets
   * @annotation ["readable"]
   * 偏移表。
   * 
   * 该表记录了每次添加字符串的偏移量。
   */
  uint32_t* offsets;
  /**
   * @property {uint32_t} size
   * @annotation ["readable"]
   * 偏移表长度。
   *  
   */
  uint32_t size;
  /**
   * @property {uint32_t} capacity
   * @annotation ["readable"]
   * 偏移表容量。
   */
  uint32_t capacity;
} revertible_str_t;

/**
 * @method revertible_str_init
 * 初始化str对象。
 * @annotation ["constructor"]
 * @param {revertible_str_t*} str 对象。
 * @param {uint32_t} capacity 初始容量。
 * @param {uint32_t} offset_capacity 初始偏移表容量。
 *
 * @return {revertible_str_t*} str对象。
 */
revertible_str_t* revertible_str_init(revertible_str_t* str, uint32_t capacity,
                                      uint32_t offset_capacity);

/**
 * @method revertible_str_deinit
 * 反初始化str对象。
 * @annotation ["constructor"]
 * @param {revertible_str_t*} str 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t revertible_str_deinit(revertible_str_t* str);

/**
 * @method revertible_str_create
 * 创建str对象。
 * @annotation ["constructor"]
 * @param {uint32_t} capacity 初始容量。
 * @param {uint32_t} offset_capacity 初始偏移表容量。
 *
 * @return {revertible_str_t*} str对象。
 */
revertible_str_t* revertible_str_create(uint32_t capacity, uint32_t offset_capacity);

/**
 * @method revertible_str_destroy
 * 销毁str对象
 * @param {revertible_str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t revertible_str_destroy(revertible_str_t* str);

/**
 * @method revertible_str_revert
 * 回退到上一次添加的字符串。
 * @param {revertible_str_t*} str str对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t revertible_str_revert(revertible_str_t* str);

/*
 * @method revertible_str_get_str
 * 获取字符串。
 * @param {revertible_str_t*} str str对象。
 *
 * @return {const char*} 返回字符串。
 */
const char* revertible_str_get_str(const revertible_str_t* str);

/**
 * @method revertible_str_append
 * 追加字符串。
 * @param {revertible_str_t*} str str对象。
 * @param {const char*} text 要追加的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t revertible_str_append(revertible_str_t* str, const char* text);

/**
 * @method revertible_str_append_format
 * 通过格式追加字符串。
 * @param {revertible_str_t*} str str对象。
 * @param {uint32_t} size format生成的字符串的最大长度(用于预先分配内存)。
 * @param {const char*} format 格式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t revertible_str_append_format(revertible_str_t* str, uint32_t size, const char* format, ...);

END_C_DECLS

#endif /*TK_revertible_str_H*/
