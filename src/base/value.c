/**
 * File:   value.h
 * Author: AWTK Develop Team
 * Brief:  generic value type
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
 * 2018-01-27 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "base/mem.h"
#include "base/value.h"
#include "base/utils.h"

bool_t value_bool(const value_t* v) {
  return_value_if_fail(v->type != VALUE_TYPE_INVALID, 0);

  switch (v->type) {
    case VALUE_TYPE_BOOL: {
      return v->value.b;
    }
    case VALUE_TYPE_INT8: {
      return v->value.i8 ? TRUE : FALSE;
    }
    case VALUE_TYPE_UINT8: {
      return v->value.u8 ? TRUE : FALSE;
    }
    case VALUE_TYPE_INT16: {
      return v->value.i16 ? TRUE : FALSE;
    }
    case VALUE_TYPE_UINT16: {
      return v->value.u16 ? TRUE : FALSE;
    }
    case VALUE_TYPE_INT32: {
      return v->value.i32 ? TRUE : FALSE;
    }
    case VALUE_TYPE_UINT32: {
      return v->value.u32 ? TRUE : FALSE;
    }
    case VALUE_TYPE_INT64: {
      return v->value.i64 ? TRUE : FALSE;
    }
    case VALUE_TYPE_UINT64: {
      return v->value.u64 ? TRUE : FALSE;
    }
    case VALUE_TYPE_FLOAT: {
      return v->value.f ? TRUE : FALSE;
    }
    case VALUE_TYPE_FLOAT32: {
      return v->value.f32 ? TRUE : FALSE;
    }
    case VALUE_TYPE_DOUBLE: {
      return v->value.f64 ? TRUE : FALSE;
    }
    case VALUE_TYPE_STRING: {
      const char* str = v->value.str;
      return (str == NULL || *str == '0' || *str == 'f' || *str == 'F') ? FALSE : TRUE;
    }
    default:
      break;
  }

  return FALSE;
}

value_t* value_set_bool(value_t* v, bool_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_BOOL;
  v->value.b = value;

  return v;
}

value_t* value_set_int8(value_t* v, int8_t value) {
  return_value_if_fail(v != NULL, NULL);
  v->type = VALUE_TYPE_INT8;
  v->value.i8 = value;

  return v;
}

int8_t value_int8(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_INT8, 0);

  return v->value.i8;
}

value_t* value_set_uint8(value_t* v, uint8_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_UINT8;
  v->value.u8 = value;

  return v;
}

uint8_t value_uint8(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_UINT8, 0);

  return v->value.u8;
}

value_t* value_set_int16(value_t* v, int16_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_INT16;
  v->value.i16 = value;

  return v;
}

int16_t value_int16(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_INT16, 0);

  return v->value.i16;
}

value_t* value_set_uint16(value_t* v, uint16_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_UINT16;
  v->value.u16 = value;

  return v;
}

uint16_t value_uint16(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_UINT16, 0);

  return v->value.u16;
}

value_t* value_set_int32(value_t* v, int32_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_INT32;
  v->value.i32 = value;

  return v;
}

int32_t value_int32(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_INT32, value_int(v));

  return v->value.i32;
}

value_t* value_set_uint32(value_t* v, uint32_t value) {
  return_value_if_fail(v != NULL, NULL);
  v->type = VALUE_TYPE_UINT32;
  v->value.u32 = value;

  return v;
}

uint32_t value_uint32(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_UINT32, value_int(v));

  return v->value.u32;
}

value_t* value_set_int64(value_t* v, int64_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_INT64;
  v->value.i64 = value;

  return v;
}

int64_t value_int64(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_INT64, 0);

  return v->value.i64;
}

value_t* value_set_uint64(value_t* v, uint64_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_UINT64;
  v->value.u64 = value;

  return v;
}

uint64_t value_uint64(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_UINT64, 0);

  return v->value.u64;
}

value_t* value_set_pointer(value_t* v, pointer_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_POINTER;
  v->value.ptr = value;

  return v;
}

pointer_t value_pointer(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_POINTER, NULL);

  return v->value.ptr;
}

value_t* value_set_float(value_t* v, float value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_FLOAT;
  v->value.f = value;

  return v;
}

float_t value_float(const value_t* v) {
  return_value_if_fail(v->type != VALUE_TYPE_INVALID, 0);

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      return (float_t)v->value.i8;
    }
    case VALUE_TYPE_UINT8: {
      return (float_t)v->value.u8;
    }
    case VALUE_TYPE_INT16: {
      return (float_t)v->value.i16;
    }
    case VALUE_TYPE_UINT16: {
      return (float_t)v->value.u16;
    }
    case VALUE_TYPE_INT32: {
      return (float_t)v->value.i32;
    }
    case VALUE_TYPE_UINT32: {
      return (float_t)v->value.u32;
    }
    case VALUE_TYPE_INT64: {
      return (float_t)v->value.i64;
    }
    case VALUE_TYPE_UINT64: {
      return (float_t)v->value.u64;
    }
    case VALUE_TYPE_FLOAT: {
      return (float_t)v->value.f;
    }
    case VALUE_TYPE_FLOAT32: {
      return (float_t)v->value.f32;
    }
    case VALUE_TYPE_DOUBLE: {
      return (float_t)v->value.f64;
    }
    case VALUE_TYPE_STRING: {
      return (float_t)tk_atof(v->value.str);
    }
    default:
      break;
  }

  return 0;
}

value_t* value_set_float32(value_t* v, float value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_FLOAT;
  v->value.f32 = value;

  return v;
}

float value_float32(const value_t* v) {
  return_value_if_fail(v->type != VALUE_TYPE_INVALID, 0);

  return v->value.f32;
}

value_t* value_set_double(value_t* v, double value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_DOUBLE;
  v->value.f64 = value;

  return v;
}

double value_double(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_DOUBLE, 0);

  return v->value.f64;
}

value_t* value_set_str(value_t* v, const char* value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_STRING;
  v->value.str = value;

  return v;
}

value_t* value_set_wstr(value_t* v, const wchar_t* value) {
  return_value_if_fail(v != NULL, NULL);

  v->type = VALUE_TYPE_WSTRING;
  v->value.wstr = value;

  return v;
}

const char* value_str(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_STRING, NULL);

  return v->value.str;
}
const wchar_t* value_wstr(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_WSTRING, NULL);

  return v->value.wstr;
}

void value_copy(value_t* dst, const value_t* src) {
  return_if_fail(dst != NULL && src != NULL);
  if (dst == src) {
    return;
  }

  memcpy(dst, src, sizeof(value_t));
}

bool_t value_is_null(value_t* v) {
  return v->type == VALUE_TYPE_INVALID;
}

int value_int(const value_t* v) {
  return_value_if_fail(v->type != VALUE_TYPE_INVALID, 0);

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      return (int)v->value.i8;
    }
    case VALUE_TYPE_UINT8: {
      return (int)v->value.u8;
    }
    case VALUE_TYPE_INT16: {
      return (int)v->value.i16;
    }
    case VALUE_TYPE_UINT16: {
      return (int)v->value.u16;
    }
    case VALUE_TYPE_INT32: {
      return (int)v->value.i32;
    }
    case VALUE_TYPE_UINT32: {
      return (int)v->value.u32;
    }
    case VALUE_TYPE_INT64: {
      return (int)v->value.i64;
    }
    case VALUE_TYPE_UINT64: {
      return (int)v->value.u64;
    }
    case VALUE_TYPE_FLOAT: {
      return (int)v->value.f;
    }
    case VALUE_TYPE_FLOAT32: {
      return (int)v->value.f32;
    }
    case VALUE_TYPE_DOUBLE: {
      return (int)v->value.f64;
    }
    case VALUE_TYPE_BOOL: {
      return (int)v->value.b;
    }
    case VALUE_TYPE_STRING: {
      return tk_atoi(v->value.str);
    }
    default:
      break;
  }

  return 0;
}

bool_t value_equal(const value_t* v, const value_t* other) {
  return_value_if_fail(v != NULL && other != NULL, FALSE);
  if (v->type != other->type) {
    return FALSE;
  }

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      return v->value.i8 == other->value.i8;
    }
    case VALUE_TYPE_UINT8: {
      return v->value.u8 == other->value.u8;
    }
    case VALUE_TYPE_INT16: {
      return v->value.i16 == other->value.i16;
    }
    case VALUE_TYPE_UINT16: {
      return v->value.u16 == other->value.u16;
    }
    case VALUE_TYPE_INT32: {
      return v->value.i32 == other->value.i32;
    }
    case VALUE_TYPE_UINT32: {
      return v->value.u32 == other->value.u32;
    }
    case VALUE_TYPE_INT64: {
      return v->value.i64 == other->value.i64;
    }
    case VALUE_TYPE_BOOL: {
      return v->value.b == other->value.b;
    }
    case VALUE_TYPE_POINTER: {
      return v->value.ptr == other->value.ptr;
    }
    case VALUE_TYPE_UINT64: {
      return v->value.u64 == other->value.u64;
    }
    case VALUE_TYPE_FLOAT: {
      return tk_fequal(v->value.f, other->value.f32);
    }
    case VALUE_TYPE_FLOAT32: {
      return tk_fequal(v->value.f32, other->value.f32);
    }
    case VALUE_TYPE_DOUBLE: {
      return tk_fequal(v->value.f64, other->value.f64);
    }
    case VALUE_TYPE_STRING: {
      return (v->value.str == other->value.str) || tk_str_eq(v->value.str, other->value.str);
    }
    case VALUE_TYPE_WSTRING: {
      return (v->value.wstr == other->value.wstr) || tk_wstr_eq(v->value.wstr, other->value.wstr);
    }
    default:
      break;
  }

  return FALSE;
}

value_t* value_set_int(value_t* v, int32_t value) {
  return value_set_int32(v, value);
}

value_t* value_create() {
  return TKMEM_ZALLOC(value_t);
}

void value_destroy(value_t* v) {
  TKMEM_FREE(v);
}
