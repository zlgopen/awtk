/**
 * File:   ubjson_const.h
 * Author: AWTK Develop Team
 * Brief:  ubjson const
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

#ifndef TK_UBJSON_CONST_H
#define TK_UBJSON_CONST_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef enum _ubjson_marker_t {
  UBJSON_MARKER_NULL = 'Z',
  UBJSON_MARKER_NOOP = 'N',
  UBJSON_MARKER_TRUE = 'T',
  UBJSON_MARKER_FALSE = 'F',
  UBJSON_MARKER_INT8 = 'i',
  UBJSON_MARKER_UINT8 = 'U',
  UBJSON_MARKER_INT16 = 'I',
  UBJSON_MARKER_INT32 = 'l',
  UBJSON_MARKER_INT64 = 'L',
  UBJSON_MARKER_FLOAT32 = 'd',
  UBJSON_MARKER_FLOAT64 = 'D',
  UBJSON_MARKER_CHAR = 'C',
  UBJSON_MARKER_STRING = 'S',
  UBJSON_MARKER_ARRAY_BEGIN = '[',
  UBJSON_MARKER_ARRAY_END = ']',
  UBJSON_MARKER_OBJECT_BEGIN = '{',
  UBJSON_MARKER_OBJECT_END = '}'
} ubjson_marker_t;

END_C_DECLS

#endif /*TK_UBJSON_CONST_H*/
