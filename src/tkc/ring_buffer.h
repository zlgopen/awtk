/**
 * File:   ring_buffer.h
 * Author: AWTK Develop Team
 * Brief:  ring_buffer
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
 * 2019-10-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_RING_BUFFER_H
#define TK_RING_BUFFER_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class ring_buffer_t
 * 循环缓存区。
 */
typedef struct _ring_buffer_t {
  /**
   * @property {uint32_t} r
   * @annotation ["readable"]
   * 读取位置。
   */
  uint32_t r;
  /**
   * @property {uint32_t} w
   * @annotation ["readable"]
   * 写入位置。
   */
  uint32_t w;
  /**
   * @property {bool_t} full
   * @annotation ["readable"]
   * 是否满。
   */
  bool_t full;
  /**
   * @property {uint32_t} capacity
   * @annotation ["readable"]
   * 当前容量。
   */
  uint32_t capacity;
  /**
   * @property {uint32_t} max_capacity
   * @annotation ["readable"]
   * 最大容量。
   */
  uint32_t max_capacity;
  /**
   * @property {uint8_t*} data
   * @annotation ["readable"]
   * 数据。
   */
  uint8_t* data;

} ring_buffer_t;

/**
 * @method ring_buffer_create
 * @annotation ["constructor"]
 * 创建ring_buffer对象。
 *
 * @param {uint32_t} init_capacity 初始容量。
 * @param {uint32_t} max_capacity 最大容量。
 *
 * @return {ring_buffer_t*} ring_buffer对象。
 */
ring_buffer_t* ring_buffer_create(uint32_t init_capacity, uint32_t max_capacity);

/**
 * @method ring_buffer_is_full
 * 检查ring_buffer是否满。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 *
 * @return {bool_t} 是否满。
 */
bool_t ring_buffer_is_full(ring_buffer_t* ring_buffer);

/**
 * @method ring_buffer_is_empty
 * 检查ring_buffer是否空。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 *
 * @return {bool_t} 是否空。
 */
bool_t ring_buffer_is_empty(ring_buffer_t* ring_buffer);

/**
 * @method ring_buffer_size
 * 获取数据长度。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 *
 * @return {uint32_t} 数据长度。
 */
uint32_t ring_buffer_size(ring_buffer_t* ring_buffer);

/**
 * @method ring_buffer_free_size
 * 获取空闲空间的长度。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 *
 * @return {uint32_t} 空闲空间的长度。
 */
uint32_t ring_buffer_free_size(ring_buffer_t* ring_buffer);

/**
 * @method ring_buffer_capacity
 * 获取容量。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 *
 * @return {uint32_t} 容量。
 */
uint32_t ring_buffer_capacity(ring_buffer_t* ring_buffer);

/**
 * @method ring_buffer_read
 * 读取数据。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 * @param {void*} buff 缓冲区。
 * @param {uint32_t} size 最大长度。
 *
 * @return {uint32_t} 返回实际读取数据的长度。
 */
uint32_t ring_buffer_read(ring_buffer_t* ring_buffer, void* buff, uint32_t size);

/**
 * @method ring_buffer_peek
 * 读取数据(不修改读取位置)。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 * @param {void*} buff 缓冲区。
 * @param {uint32_t} size 最大长度。
 *
 * @return {uint32_t} 返回实际读取数据的长度。
 */
uint32_t ring_buffer_peek(ring_buffer_t* ring_buffer, void* buff, uint32_t size);

/**
 * @method ring_buffer_write
 * 写入数据。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 * @param {const void*} buff 缓冲区。
 * @param {uint32_t} size 最大长度。
 *
 * @return {uint32_t} 返回实际写入数据的长度。
 */
uint32_t ring_buffer_write(ring_buffer_t* ring_buffer, const void* buff, uint32_t size);

/**
 * @method ring_buffer_read_len
 * 读取指定长度数据，要么成功要么失败。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 * @param {void*} buff 缓冲区。
 * @param {uint32_t} size 最大长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ring_buffer_read_len(ring_buffer_t* ring_buffer, void* buff, uint32_t size);

/**
 * @method ring_buffer_skip
 * 跳过指定长度数据，要么成功要么失败。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 * @param {uint32_t} size 长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ring_buffer_skip(ring_buffer_t* ring_buffer, uint32_t size);

/**
 * @method ring_buffer_write_len
 * 写入指定长度数据，要么成功要么失败。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 * @param {const void*} buff 缓冲区。
 * @param {uint32_t} size 最大长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ring_buffer_write_len(ring_buffer_t* ring_buffer, const void* buff, uint32_t size);

/**
 * @method ring_buffer_reset
 * 重置ring_buffer为空。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ring_buffer_reset(ring_buffer_t* ring_buffer);

/**
 * @method ring_buffer_set_read_cursor
 * 设置读取光标的位置。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 * @param {uint32_t} r 读取光标的位置。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ring_buffer_set_read_cursor(ring_buffer_t* ring_buffer, uint32_t r);

/**
 * @method ring_buffer_set_read_cursor_delta
 * 设置读取光标的位置(delta)。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 * @param {uint32_t} r_delta 读取光标的位置(delta)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ring_buffer_set_read_cursor_delta(ring_buffer_t* ring_buffer, uint32_t r_delta);

/**
 * @method ring_buffer_set_write_cursor
 * 设置写入光标的位置。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 * @param {uint32_t} w 写入光标的位置。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ring_buffer_set_write_cursor(ring_buffer_t* ring_buffer, uint32_t w);

/**
 * @method ring_buffer_set_write_cursor_delta
 * 设置写入光标的位置(delta)。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 * @param {uint32_t} w_delta 写入光标的位置(delta)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ring_buffer_set_write_cursor_delta(ring_buffer_t* ring_buffer, uint32_t w_delta);

/**
 * @method ring_buffer_ensure_write_space
 * 扩展buffer。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 * @param {uint32_t} size 需要的大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ring_buffer_ensure_write_space(ring_buffer_t* ring_buffer, uint32_t size);

/**
 * @method ring_buffer_destroy
 * 销毁ring_buffer。
 *
 * @param {ring_buffer_t*} ring_buffer ring_buffer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ring_buffer_destroy(ring_buffer_t* ring_buffer);

END_C_DECLS

#endif /*TK_RING_BUFFER_H*/
