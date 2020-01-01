/**
 * File:   endian.h
 * Author: AWTK Develop Team
 * Brief:  endian related functions.
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
 * 2019-09-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ENDIAN_H
#define TK_ENDIAN_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

static inline void swap_uint8(uint8_t* p1, uint8_t* p2) {
  uint8_t c = *p1;

  *p1 = *p2;
  *p2 = c;
}

static inline bool_t is_little_endian(void) {
  uint16_t val = 0x1122;

  return *((uint8_t*)&val) == 0x22;
}

static inline int16_t int16_endian_invert(int16_t value) {
  int16_t ret = value;
  uint8_t* p = (uint8_t*)&ret;

  p[0] = value >> 8;
  p[1] = value & 0xff;

  return ret;
}

static inline int16_t int16_to_big_endian(int16_t value) {
  return is_little_endian() ? int16_endian_invert(value) : value;
}

static inline int16_t int16_from_big_endian(int16_t value) {
  return is_little_endian() ? int16_endian_invert(value) : value;
}

static inline int32_t int32_endian_invert(int32_t value) {
  int32_t ret = value;
  uint8_t* p = (uint8_t*)&ret;

  swap_uint8(p, p + 3);
  swap_uint8(p + 1, p + 2);

  return ret;
}

static inline int32_t int32_to_big_endian(int32_t value) {
  return is_little_endian() ? int32_endian_invert(value) : value;
}

static inline int32_t int32_from_big_endian(int32_t value) {
  return is_little_endian() ? int32_endian_invert(value) : value;
}

static inline int64_t int64_endian_invert(int64_t value) {
  int64_t ret = value;
  uint8_t* p = (uint8_t*)&ret;

  swap_uint8(p, p + 7);
  swap_uint8(p + 1, p + 6);
  swap_uint8(p + 2, p + 5);
  swap_uint8(p + 3, p + 4);

  return ret;
}

static inline int64_t int64_to_big_endian(int64_t value) {
  return is_little_endian() ? int64_endian_invert(value) : value;
}

static inline int64_t int64_from_big_endian(int64_t value) {
  return is_little_endian() ? int64_endian_invert(value) : value;
}

static inline float float_endian_invert(float value) {
  float ret = value;
  uint8_t* p = (uint8_t*)&ret;

  swap_uint8(p, p + 3);
  swap_uint8(p + 1, p + 2);

  return ret;
}

static inline float float_to_big_endian(float value) {
  return is_little_endian() ? float_endian_invert(value) : value;
}

static inline float float_from_big_endian(float value) {
  return is_little_endian() ? float_endian_invert(value) : value;
}

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

static inline double double_to_big_endian(double value) {
  return is_little_endian() ? double_endian_invert(value) : value;
}

static inline double double_from_big_endian(double value) {
  return is_little_endian() ? double_endian_invert(value) : value;
}

END_C_DECLS

#endif /*TK_ENDIAN_H*/
