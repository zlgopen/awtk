/**
 * File:   endian.h
 * Author: AWTK Develop Team
 * Brief:  endian related functions.
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ENDIAN_H
#define TK_ENDIAN_H

#include "tkc/types_def.h"

BEGIN_C_DECLS
/**
 * @class endian_t
 * @annotation ["fake"]
 *
 * 字节顺序转换相关函数。
 *
 * 示例
 * 
 * ```c
 *  uint16_t a = 0x1234;
 *  ENSURE(is_little_endian());
 *  ENSURE(int16_endian_invert(a) == 0x3412);
 *  ENSURE(int16_from_big_endian(a) == 0x3412);
 *  ENSURE(int16_to_big_endian(a) == 0x3412);
 * 
 *  int32_t b = 0x12345678;
 *  ENSURE(int32_endian_invert(b) == 0x78563412);
 *  ENSURE(int32_from_big_endian(b) == 0x78563412);
 *  ENSURE(int32_to_big_endian(b) == 0x78563412);
 * 
 *  int64_t c = 0x1234567890ABCDEF;
 *  ENSURE(int64_endian_invert(c) == 0xEFCDAB9078563412);
 *  ENSURE(int64_from_big_endian(c) == 0xEFCDAB9078563412);
 *  ENSURE(int64_to_big_endian(c) == 0xEFCDAB9078563412);
 * 
 *  float f1 = 1.23;
 *  float f2 = float_endian_invert(f1);
 *  ENSURE(float_from_big_endian(f2) == f1);
 *  float f3 = float_from_big_endian(f2);
 *  ENSURE(float_to_big_endian(f3) == f2);
 * 
 *  double d1 = 1.2345678;
 *  double d2 = double_endian_invert(d1);
 *  ENSURE(double_from_big_endian(d2) == d1);
 *  double d3 = double_from_big_endian(d2);
 *  ENSURE(double_to_big_endian(d3) == d2);
 * ```
 */

/**
 * @method swap_uint8
 * @annotation ["static", "private"]
 * @export none
 * 交换两个uint8_t的值。
 * @param {uint8_t*} p1 数1.
 * @param {uint8_t*} p2 数2.
 *
 * @return {void} 无。
 */
static inline void swap_uint8(uint8_t* p1, uint8_t* p2) {
  uint8_t c = *p1;

  *p1 = *p2;
  *p2 = c;
}

/**
 * @method is_little_endian
 * @annotation ["static"]
 * @export none
 * 检查当前系统是否是小端模式。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示不是。
 */
static inline bool_t is_little_endian(void) {
  uint16_t val = 0x1122;

  return *((uint8_t*)&val) == 0x22;
}

/**
 * @method int16_endian_invert
 * @annotation ["static"]
 * @export none
 * 16 位整数大小端模式转换。
 * @param {int16_t} value 输入数据。
 *
 * @return {int16_t} 返回转换后的数据。
 */
static inline int16_t int16_endian_invert(int16_t value) {
  int16_t ret = value;
  uint8_t* p = (uint8_t*)&ret;

  swap_uint8(p, p + 1);

  return ret;
}

/**
 * @method int16_to_big_endian
 * @annotation ["static"]
 * @export none
 * 16 位整数转换为大端数据。
 * @param {int16_t} value 输入数据。
 *
 * @return {int16_t} 返回转换后的数据。
 */
static inline int16_t int16_to_big_endian(int16_t value) {
  return is_little_endian() ? int16_endian_invert(value) : value;
}

/**
 * @method int16_from_big_endian
 * @annotation ["static"]
 * @export none
 * 大端数据转换为 16 位整数。
 * @param {int16_t} value 输入数据。
 *
 * @return {int16_t} 返回转换后的数据。
 */
static inline int16_t int16_from_big_endian(int16_t value) {
  return is_little_endian() ? int16_endian_invert(value) : value;
}

/**
 * @method int16_to_little_endian
 * @annotation ["static"]
 * @export none
 * 16 位整数转换为小端数据。
 * @param {int16_t} value 输入数据。
 *
 * @return {int16_t} 返回转换后的数据。
 */
static inline int16_t int16_to_little_endian(int16_t value) {
  return is_little_endian() ? value : int16_endian_invert(value);
}

/**
 * @method int16_from_little_endian
 * @annotation ["static"]
 * @export none
 * 小端数据转换为 16 位整数。
 * @param {int16_t} value 输入数据。
 *
 * @return {int16_t} 返回转换后的数据。
 */
static inline int16_t int16_from_little_endian(int16_t value) {
  return is_little_endian() ? value : int16_endian_invert(value);
}

/**
 * @method uint16_endian_invert
 * @annotation ["static"]
 * @export none
 * 16 位无符号整数大小端模式转换。
 * @param {uint16_t} value 输入数据。
 *
 * @return {uint16_t} 返回转换后的数据。
 */
static inline uint16_t uint16_endian_invert(uint16_t value) {
  uint16_t ret = value;
  uint8_t* p = (uint8_t*)&ret;

  swap_uint8(p, p + 1);

  return ret;
}

/**
 * @method uint16_to_big_endian
 * @annotation ["static"]
 * @export none
 * 16 位无符号整数转换为大端数据。
 * @param {uint16_t} value 输入数据。
 *
 * @return {uint16_t} 返回转换后的数据。
 */
static inline uint16_t uint16_to_big_endian(uint16_t value) {
  return is_little_endian() ? uint16_endian_invert(value) : value;
}

/**
 * @method uint16_from_big_endian
 * @annotation ["static"]
 * @export none
 * 大端数据转换为 16 位无符号整数。
 * @param {uint16_t} value 输入数据。
 *
 * @return {uint16_t} 返回转换后的数据。
 */
static inline uint16_t uint16_from_big_endian(uint16_t value) {
  return is_little_endian() ? uint16_endian_invert(value) : value;
}

/**
 * @method uint16_to_little_endian
 * @annotation ["static"]
 * @export none
 * 16 位无符号整数转换为小端数据。
 * @param {uint16_t} value 输入数据。
 *
 * @return {uint16_t} 返回转换后的数据。
 */
static inline uint16_t uint16_to_little_endian(uint16_t value) {
  return is_little_endian() ? value : uint16_endian_invert(value);
}

/**
 * @method uint16_from_little_endian
 * @annotation ["static"]
 * @export none
 * 小端数据转换为 16 位无符号整数。
 * @param {uint16_t} value 输入数据。
 *
 * @return {uint16_t} 返回转换后的数据。
 */
static inline uint16_t uint16_from_little_endian(uint16_t value) {
  return is_little_endian() ? value : uint16_endian_invert(value);
}

/**
 * @method int32_endian_invert
 * @annotation ["static"]
 * @export none
 * 32 位整数大小端模式转换。
 * @param {int32_t} value 输入数据。
 *
 * @return {int32_t} 返回转换后的数据。
 */
static inline int32_t int32_endian_invert(int32_t value) {
  int32_t ret = value;
  uint8_t* p = (uint8_t*)&ret;

  swap_uint8(p, p + 3);
  swap_uint8(p + 1, p + 2);

  return ret;
}

/**
 * @method int32_to_big_endian
 * @annotation ["static"]
 * @export none
 * 32 位整数转换为大端数据。
 * @param {int32_t} value 输入数据。
 *
 * @return {int32_t} 返回转换后的数据。
 */
static inline int32_t int32_to_big_endian(int32_t value) {
  return is_little_endian() ? int32_endian_invert(value) : value;
}

/**
 * @method int32_from_big_endian
 * @annotation ["static"]
 * @export none
 * 大端数据转换为 32 位整数。
 * @param {int32_t} value 输入数据。
 *
 * @return {int32_t} 返回转换后的数据。
 */
static inline int32_t int32_from_big_endian(int32_t value) {
  return is_little_endian() ? int32_endian_invert(value) : value;
}

/**
 * @method int32_to_little_endian
 * @annotation ["static"]
 * @export none
 * 32 位整数转换为小端数据。
 * @param {int32_t} value 输入数据。
 *
 * @return {int32_t} 返回转换后的数据。
 */
static inline int32_t int32_to_little_endian(int32_t value) {
  return is_little_endian() ? value : int32_endian_invert(value);
}

/**
 * @method int32_from_little_endian
 * @annotation ["static"]
 * @export none
 * 小端数据转换为 32 位整数。
 * @param {int32_t} value 输入数据。
 *
 * @return {int32_t} 返回转换后的数据。
 */
static inline int32_t int32_from_little_endian(int32_t value) {
  return is_little_endian() ? value : int32_endian_invert(value);
}

/**
 * @method uint32_endian_invert
 * @annotation ["static"]
 * @export none
 * 32 位无符号整数大小端模式转换。
 * @param {uint32_t} value 输入数据。
 *
 * @return {uint32_t} 返回转换后的数据。
 */
static inline uint32_t uint32_endian_invert(uint32_t value) {
  uint32_t ret = value;
  uint8_t* p = (uint8_t*)&ret;

  swap_uint8(p, p + 3);
  swap_uint8(p + 1, p + 2);

  return ret;
}

/**
 * @method uint32_to_big_endian
 * @annotation ["static"]
 * @export none
 * 32 位无符号整数转换为大端数据。
 * @param {uint32_t} value 输入数据。
 *
 * @return {uint32_t} 返回转换后的数据。
 */
static inline uint32_t uint32_to_big_endian(uint32_t value) {
  return is_little_endian() ? uint32_endian_invert(value) : value;
}

/**
 * @method uint32_from_big_endian
 * @annotation ["static"]
 * @export none
 * 大端数据转换为 32 位无符号整数。
 * @param {uint32_t} value 输入数据。
 *
 * @return {uint32_t} 返回转换后的数据。
 */
static inline uint32_t uint32_from_big_endian(uint32_t value) {
  return is_little_endian() ? uint32_endian_invert(value) : value;
}

/**
 * @method uint32_to_little_endian
 * @annotation ["static"]
 * @export none
 * 32 位无符号整数转换为小端数据。
 * @param {uint32_t} value 输入数据。
 *
 * @return {uint32_t} 返回转换后的数据。
 */
static inline uint32_t uint32_to_little_endian(uint32_t value) {
  return is_little_endian() ? value : uint32_endian_invert(value);
}

/**
 * @method uint32_from_little_endian
 * @annotation ["static"]
 * @export none
 * 小端数据转换为 32 位无符号整数。
 * @param {uint32_t} value 输入数据。
 *
 * @return {uint32_t} 返回转换后的数据。
 */
static inline uint32_t uint32_from_little_endian(uint32_t value) {
  return is_little_endian() ? value : uint32_endian_invert(value);
}

/**
 * @method int64_endian_invert
 * @annotation ["static"]
 * @export none
 * 64 位整数大小端模式转换。
 * @param {int64_t} value 输入数据。
 *
 * @return {int64_t} 返回转换后的数据。
 */
static inline int64_t int64_endian_invert(int64_t value) {
  int64_t ret = value;
  uint8_t* p = (uint8_t*)&ret;

  swap_uint8(p, p + 7);
  swap_uint8(p + 1, p + 6);
  swap_uint8(p + 2, p + 5);
  swap_uint8(p + 3, p + 4);

  return ret;
}

/**
 * @method int64_to_big_endian
 * @annotation ["static"]
 * @export none
 * 64 位整数转换为大端数据。
 * @param {int64_t} value 输入数据。
 *
 * @return {int64_t} 返回转换后的数据。
 */
static inline int64_t int64_to_big_endian(int64_t value) {
  return is_little_endian() ? int64_endian_invert(value) : value;
}

/**
 * @method int64_from_big_endian
 * @annotation ["static"]
 * @export none
 * 大端数据转换为 64 位整数。
 * @param {int64_t} value 输入数据。
 *
 * @return {int64_t} 返回转换后的数据。
 */
static inline int64_t int64_from_big_endian(int64_t value) {
  return is_little_endian() ? int64_endian_invert(value) : value;
}

/**
 * @method int64_to_little_endian
 * @annotation ["static"]
 * @export none
 * 64 位整数转换为小端数据。
 * @param {int64_t} value 输入数据。
 *
 * @return {int64_t} 返回转换后的数据。
 */
static inline int64_t int64_to_little_endian(int64_t value) {
  return is_little_endian() ? value : int64_endian_invert(value);
}

/**
 * @method int64_from_little_endian
 * @annotation ["static"]
 * @export none
 * 小端数据转换为 64 位整数。
 * @param {int64_t} value 输入数据。
 *
 * @return {int64_t} 返回转换后的数据。
 */
static inline int64_t int64_from_little_endian(int64_t value) {
  return is_little_endian() ? value : int64_endian_invert(value);
}

/**
 * @method uint64_endian_invert
 * @annotation ["static"]
 * @export none
 * 64 位无符号整数大小端模式转换。
 * @param {uint64_t} value 输入数据。
 *
 * @return {uint64_t} 返回转换后的数据。
 */
static inline uint64_t uint64_endian_invert(uint64_t value) {
  uint64_t ret = value;
  uint8_t* p = (uint8_t*)&ret;

  swap_uint8(p, p + 7);
  swap_uint8(p + 1, p + 6);
  swap_uint8(p + 2, p + 5);
  swap_uint8(p + 3, p + 4);

  return ret;
}

/**
 * @method uint64_to_big_endian
 * @annotation ["static"]
 * @export none
 * 64 位无符号整数转换为大端数据。
 * @param {uint64_t} value 输入数据。
 *
 * @return {uint64_t} 返回转换后的数据。
 */
static inline uint64_t uint64_to_big_endian(uint64_t value) {
  return is_little_endian() ? uint64_endian_invert(value) : value;
}

/**
 * @method uint64_from_big_endian
 * @annotation ["static"]
 * @export none
 * 大端数据转换为 64 位无符号整数。
 * @param {uint64_t} value 输入数据。
 *
 * @return {uint64_t} 返回转换后的数据。
 */
static inline uint64_t uint64_from_big_endian(uint64_t value) {
  return is_little_endian() ? uint64_endian_invert(value) : value;
}

/**
 * @method uint64_to_little_endian
 * @annotation ["static"]
 * @export none
 * 64 位无符号整数转换为小端数据。
 * @param {uint64_t} value 输入数据。
 *
 * @return {uint64_t} 返回转换后的数据。
 */
static inline uint64_t uint64_to_little_endian(uint64_t value) {
  return is_little_endian() ? value : uint64_endian_invert(value);
}

/**
 * @method uint64_from_little_endian
 * @annotation ["static"]
 * @export none
 * 小端数据转换为 64 位无符号整数。
 * @param {uint64_t} value 输入数据。
 *
 * @return {uint64_t} 返回转换后的数据。
 */
static inline uint64_t uint64_from_little_endian(uint64_t value) {
  return is_little_endian() ? value : uint64_endian_invert(value);
}

/**
 * @method float_endian_invert
 * @annotation ["static"]
 * @export none
 * float浮点数大小端模式转换。
 * @param {float} value 输入数据。
 *
 * @return {float} 返回转换后的数据。
 */
static inline float float_endian_invert(float value) {
  float ret = value;
  uint8_t* p = (uint8_t*)&ret;

  swap_uint8(p, p + 3);
  swap_uint8(p + 1, p + 2);

  return ret;
}

/**
 * @method float_to_big_endian
 * @annotation ["static"]
 * @export none
 * float 浮点数转换为大端数据。
 * @param {float} value 输入数据。
 *
 * @return {float} 返回转换后的数据。
 */
static inline float float_to_big_endian(float value) {
  return is_little_endian() ? float_endian_invert(value) : value;
}

/**
 * @method float_from_big_endian
 * @annotation ["static"]
 * @export none
 * 大端数据转换为 float 浮点数。
 * @param {float} value 输入数据。
 *
 * @return {float} 返回转换后的数据。
 */
static inline float float_from_big_endian(float value) {
  return is_little_endian() ? float_endian_invert(value) : value;
}

/**
 * @method float_to_little_endian
 * @annotation ["static"]
 * @export none
 * float 浮点数转换为小端数据。
 * @param {float} value 输入数据。
 *
 * @return {float} 返回转换后的数据。
 */
static inline float float_to_little_endian(float value) {
  return is_little_endian() ? value : float_endian_invert(value);
}

/**
 * @method float_from_little_endian
 * @annotation ["static"]
 * @export none
 * 小端数据转换为 float 浮点数。
 * @param {float} value 输入数据。
 *
 * @return {float} 返回转换后的数据。
 */
static inline float float_from_little_endian(float value) {
  return is_little_endian() ? value : float_endian_invert(value);
}

/**
 * @method double_endian_invert
 * @annotation ["static"]
 * @export none
 * double浮点数大小端模式转换。
 * @param {double} value 输入数据。
 *
 * @return {double} 返回转换后的数据。
 */
static inline double double_endian_invert(double value) {
  double ret = value;
  uint8_t* p = (uint8_t*)&ret;
  assert(sizeof(value) == 8);

  swap_uint8(p, p + 7);
  swap_uint8(p + 1, p + 6);
  swap_uint8(p + 2, p + 5);
  swap_uint8(p + 3, p + 4);

  return ret;
}

/**
 * @method double_to_big_endian
 * @annotation ["static"]
 * @export none
 * double 浮点数转换为大端数据。
 * @param {double} value 输入数据。
 *
 * @return {double} 返回转换后的数据。
 */
static inline double double_to_big_endian(double value) {
  return is_little_endian() ? double_endian_invert(value) : value;
}

/**
 * @method double_from_big_endian
 * @annotation ["static"]
 * @export none
 * 大端数据转换为 double 浮点数。
 * @param {double} value 输入数据。
 *
 * @return {double} 返回转换后的数据。
 */
static inline double double_from_big_endian(double value) {
  return is_little_endian() ? double_endian_invert(value) : value;
}

/**
 * @method double_to_little_endian
 * @annotation ["static"]
 * @export none
 * double 浮点数转换为小端数据
 * @param {double} value 输入数据。
 *
 * @return {double} 返回转换后的数据。
 */
static inline double double_to_little_endian(double value) {
  return is_little_endian() ? value : double_endian_invert(value);
}

/**
 * @method double_from_little_endian
 * @annotation ["static"]
 * @export none
 * 小端数据转换为 double 浮点数。
 * @param {double} value 输入数据。
 *
 * @return {double} 返回转换后的数据。
 */
static inline double double_from_little_endian(double value) {
  return is_little_endian() ? value : double_endian_invert(value);
}

END_C_DECLS

#endif /*TK_ENDIAN_H*/
