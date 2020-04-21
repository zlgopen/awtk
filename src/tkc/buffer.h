/**
 * File:   buffer.h
 * Author: AWTK Develop Team
 * Brief:  buffer
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
 * 2018-02-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BUFFER_H
#define TK_BUFFER_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class wbuffer_t
 * Write Buffer。用于数据打包。
 *
 * 示例：
 *
 * ```c
 *  uint8_t buff[128];
 *  wbuffer_t wbuffer;
 *  rbuffer_t rbuffer;
 *  const char* str = NULL;
 *  wbuffer_init(&wbuffer, buff, sizeof(buff));
 *
 *  wbuffer_write_string(&wbuffer, "hello awtk");
 *
 *  rbuffer_init(&rbuffer, wbuffer.data, wbuffer.cursor);
 *  rbuffer_read_string(&rbuffer, &str);
 * ```
 *
 * ```c
 *  wbuffer_t wbuffer;
 *  wbuffer_init_extendable(&wbuffer);
 *
 *  wbuffer_write_string(&wbuffer, "hello awtk");
 *
 *  wbuffer_deinit(&wbuffer);
 * ```
 * > 如果初始化为extendable，则最后需要调用wbuffer\_deinit释放资源。
 *
 *
 */
typedef struct _wbuffer_t {
  /**
   * @property {uint8_t*} data
   * @annotation ["readable"]
   * 数据缓冲区。用于保存写入的数据。
   */
  uint8_t* data;
  /**
   * @property {uint32_t} cursor
   * @annotation ["readable"]
   * 当前写入位置。
   */
  uint32_t cursor;
  /**
   * @property {uint32_t} capacity
   * @annotation ["readable"]
   * 数据缓冲区最大容量。
   */
  uint32_t capacity;
  /**
   * @property {bool_t} extendable
   * @annotation ["readable"]
   * 容量不够时是否支持自动扩展。
   */
  bool_t extendable;
} wbuffer_t;

/**
 * @method wbuffer_init
 * 初始wbuffer对象。
 * @annotation ["constructor"]
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {uint8_t*} data 缓冲区。
 * @param {uint16_t} capacity 缓冲区的容量。
 *
 * @return {wbuffer_t*} wbuffer对象本身。
 */
wbuffer_t* wbuffer_init(wbuffer_t* wbuffer, uint8_t* data, uint32_t capacity);

/**
 * @method wbuffer_init_extendable
 * 初始wbuffer对象，容量不够时是否支持自动扩展，使用完成后需要调用wbuffer\_deinit释放资源。
 *
 * @annotation ["constructor"]
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 *
 * @return {wbuffer_t*} wbuffer对象本身。
 */
wbuffer_t* wbuffer_init_extendable(wbuffer_t* wbuffer);

/**
 * @method wbuffer_extend_capacity
 * 扩展缓冲区至指定的大小。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {uint32_t} capacity 缓冲区的容量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_extend_capacity(wbuffer_t* wbuffer, uint32_t capacity);

/**
 * @method wbuffer_deinit
 * 释放资源。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_deinit(wbuffer_t* wbuffer);

/**
 * @method wbuffer_skip
 * 跳过指定的长度。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {int32_t} delta 跳过的偏移量，正数往前负数往回跳。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_skip(wbuffer_t* wbuffer, int32_t delta);

/**
 * @method wbuffer_write_uint8
 * 写入uint8数据。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {uint8_t} value 要写入的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_write_uint8(wbuffer_t* wbuffer, uint8_t value);

/**
 * @method wbuffer_write_uint16
 * 写入uint16数据。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {uint16_t} value 要写入的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_write_uint16(wbuffer_t* wbuffer, uint16_t value);

/**
 * @method wbuffer_write_uint32
 * 写入uint32数据。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {uint32_t} value 要写入的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_write_uint32(wbuffer_t* wbuffer, uint32_t value);

/**
 * @method wbuffer_write_float
 * 写入float数据。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {float_t} value 要写入的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_write_float(wbuffer_t* wbuffer, float_t value);

/**
 * @method wbuffer_write_binary
 * 写入指定长度的二进制数据。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {void*} data 要写入的数据。
 * @param {uint32_t} size 要写入的数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_write_binary(wbuffer_t* wbuffer, const void* data, uint32_t size);

/**
 * @method wbuffer_write_string
 * 写入字符串。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {char*} data 要写入的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_write_string(wbuffer_t* wbuffer, const char* data);

/**
 * @method wbuffer_has_room
 * 是否还有空间。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {uint32_t} size 空间大小。
 *
 * @return {bool_t} 返回FALSE表示无，否则表示有。
 */
bool_t wbuffer_has_room(wbuffer_t* wbuffer, uint32_t size);

/**
 * @class rbuffer_t
 * Read Buffer。用于数据解包。
 *
 * 示例：
 *
 * ```c
 *  uint8_t buff[128];
 *  wbuffer_t wbuffer;
 *  rbuffer_t rbuffer;
 *  const char* str = NULL;
 *  wbuffer_init(&wbuffer, buff, sizeof(buff));
 *
 *  wbuffer_write_string(&wbuffer, "hello awtk");
 *
 *  rbuffer_init(&rbuffer, wbuffer.data, wbuffer.cursor);
 *  rbuffer_read_string(&rbuffer, &str);
 * ```
 */
typedef struct _rbuffer_t {
  /**
   * @property {uint8_t*} data
   * @annotation ["readable"]
   * 数据缓冲区。
   */
  const uint8_t* data;
  /**
   * @property {uint32_t} cursor
   * @annotation ["readable"]
   * 当前读取位置。
   */
  uint32_t cursor;
  /**
   * @property {uint32_t} capacity
   * @annotation ["readable"]
   * 缓存区的容量。
   */
  uint32_t capacity;
} rbuffer_t;

/**
 * @method rbuffer_init
 * 初始rbuffer对象。
 * @annotation ["constructor"]
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint8_t*} data 缓冲区。
 * @param {uint16_t} capacity 缓冲区的容量。
 *
 * @return {rbuffer_t*} rbuffer对象本身。
 */
rbuffer_t* rbuffer_init(rbuffer_t* rbuffer, const uint8_t* data, uint32_t capacity);

/**
 * @method rbuffer_has_more
 * 判断是否还有数据可读。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 *
 * @return {bool_t} 返回TRUE表示还有数据可读，否则表示无数据可读。
 */
bool_t rbuffer_has_more(rbuffer_t* rbuffer);

/**
 * @method rbuffer_skip
 * 跳过指定的长度。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {int32_t} offset 跳过的偏移量，正数往前负数往回跳。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_skip(rbuffer_t* rbuffer, int32_t offset);

/**
 * @method rbuffer_read_uint8
 * 读取uint8数据。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint8_t*} value 返回读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_read_uint8(rbuffer_t* rbuffer, uint8_t* value);

/**
 * @method rbuffer_read_uint16
 * 读取uint16数据。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint16_t*} value 读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_read_uint16(rbuffer_t* rbuffer, uint16_t* value);

/**
 * @method rbuffer_read_uint32
 * 读取uint32数据。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint32_t*} value 返回读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_read_uint32(rbuffer_t* rbuffer, uint32_t* value);

/**
 * @method rbuffer_read_float
 * 读取float数据。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {float_t*} value 返回读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_read_float(rbuffer_t* rbuffer, float_t* value);

/**
 * @method rbuffer_read_binary
 * 读取指定长度的二进制数据。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {void*} data 返回读取的数据。
 * @param {uint32_t} size 读取的数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_read_binary(rbuffer_t* rbuffer, void* data, uint32_t size);

/**
 * @method rbuffer_read_string
 * 读取字符串。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {char**} str 返回字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_read_string(rbuffer_t* rbuffer, const char** str);

/**
 * @method rbuffer_peek_uint8
 * 读取uint8数据，但不改变cursor的位置。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint8_t*} value 返回读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_peek_uint8(rbuffer_t* rbuffer, uint8_t* value);

/**
 * @method rbuffer_peek_uint16
 * 读取uint16数据，但不改变cursor的位置。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint16_t*} value 返回读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_peek_uint16(rbuffer_t* rbuffer, uint16_t* value);

/**
 * @method rbuffer_peek_uint32
 * 读取uint32数据，但不改变cursor的位置。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint32_t*} value 读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_peek_uint32(rbuffer_t* rbuffer, uint32_t* value);

#define load_uint32(p, v)                                   \
  (v) = (p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24)); \
  p += 4;
#define save_uint32(p, v)  \
  p[0] = (v & 0xff);       \
  p[1] = (v >> 8) & 0xff;  \
  p[2] = (v >> 16) & 0xff; \
  p[3] = (v >> 24) & 0xff; \
  p += 4;

#define load_uint16(p, v)     \
  (v) = (p[0] | (p[1] << 8)); \
  p += 2;
#define save_uint16(p, v) \
  p[0] = (v & 0xff);      \
  p[1] = (v >> 8) & 0xff; \
  p += 2;

#define load_uint8(p, v) \
  (v) = p[0];            \
  p += 1;
#define save_uint8(p, v) \
  p[0] = v;              \
  p += 1;

END_C_DECLS

#endif /*TK_BUFFER_H*/
