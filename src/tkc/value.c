/**
 * File:   value.h
 * Author: AWTK Develop Team
 * Brief:  generic value type
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
 * 2018-01-27 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "tkc/mem.h"
#include "tkc/value.h"
#include "tkc/utils.h"
#include "tkc/object.h"

bool_t value_bool(const value_t* v) {
  return_value_if_fail(v->type != VALUE_TYPE_INVALID, 0);

  switch (v->type) {
    case VALUE_TYPE_BOOL: {
      return v->value.b;
    }
    case VALUE_TYPE_STRING: {
      return tk_atob(v->value.str);
    }
    case VALUE_TYPE_WSTRING: {
      return tk_watob(v->value.wstr);
    }
    default: { return value_int(v) ? TRUE : FALSE; }
  }
}

value_t* value_init(value_t* v, uint32_t type) {
  v->type = type;
  v->free_handle = FALSE;

  return v;
}

value_t* value_set_bool(value_t* v, bool_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.b = value;
  return value_init(v, VALUE_TYPE_BOOL);
}

value_t* value_set_int8(value_t* v, int8_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.i8 = value;
  return value_init(v, VALUE_TYPE_INT8);
}

int8_t value_int8(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_INT8) {
    return v->value.i8;
  } else {
    return (int8_t)value_int(v);
  }
}

value_t* value_set_uint8(value_t* v, uint8_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.u8 = value;

  return value_init(v, VALUE_TYPE_UINT8);
}

uint8_t value_uint8(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_UINT8) {
    return v->value.u8;
  } else {
    return (uint8_t)value_int(v);
  }
}

value_t* value_set_int16(value_t* v, int16_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.i16 = value;

  return value_init(v, VALUE_TYPE_INT16);
}

int16_t value_int16(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_INT16) {
    return v->value.i16;
  } else {
    return (int16_t)value_int(v);
  }
}

value_t* value_set_uint16(value_t* v, uint16_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.u16 = value;

  return value_init(v, VALUE_TYPE_UINT16);
}

uint16_t value_uint16(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_UINT16) {
    return v->value.u16;
  } else {
    return (uint16_t)value_int(v);
  }
}

value_t* value_set_int32(value_t* v, int32_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.i32 = value;
  return value_init(v, VALUE_TYPE_INT32);
}

int32_t value_int32(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_INT32) {
    return v->value.i32;
  } else {
    return (int32_t)value_int(v);
  }
}

value_t* value_set_uint32(value_t* v, uint32_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.u32 = value;
  return value_init(v, VALUE_TYPE_UINT32);
}

uint32_t value_uint32(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_UINT32) {
    return v->value.u32;
  } else {
    return (uint32_t)value_int(v);
  }
}

value_t* value_set_int64(value_t* v, int64_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.i64 = value;
  return value_init(v, VALUE_TYPE_INT64);
}

int64_t value_int64(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_INT64) {
    return v->value.i64;
  } else {
    return (int64_t)value_int(v);
  }
}

value_t* value_set_uint64(value_t* v, uint64_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.u64 = value;

  return value_init(v, VALUE_TYPE_UINT64);
}

uint64_t value_uint64(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_UINT64) {
    return v->value.u64;
  } else {
    return (uint64_t)value_int(v);
  }
}

value_t* value_set_pointer(value_t* v, void* value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.ptr = value;
  return value_init(v, VALUE_TYPE_POINTER);
}

void* value_pointer(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  switch (v->type) {
    case VALUE_TYPE_STRING: {
      return (void*)(v->value.str);
    }
    case VALUE_TYPE_WSTRING: {
      return (void*)(v->value.wstr);
    }
    case VALUE_TYPE_OBJECT: {
      return (v->value.object);
    }
    case VALUE_TYPE_POINTER: {
      return (v->value.ptr);
    }
    default:
      break;
  }

  return NULL;
}

value_t* value_set_float(value_t* v, float_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.f = value;

  return value_init(v, VALUE_TYPE_FLOAT);
}

float_t value_float(const value_t* v) {
  return_value_if_fail(v != NULL, 0);
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
    case VALUE_TYPE_WSTRING: {
      return (float_t)tk_watof(v->value.wstr);
    }
    default:
      break;
  }

  return 0;
}

value_t* value_set_float32(value_t* v, float value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.f32 = value;

  return value_init(v, VALUE_TYPE_FLOAT32);
}

float value_float32(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_FLOAT32) {
    return v->value.f32;
  } else {
    return (float)value_float(v);
  }
}

value_t* value_set_double(value_t* v, double value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.f64 = value;

  return value_init(v, VALUE_TYPE_DOUBLE);
}

double value_double(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  return_value_if_fail(v->type != VALUE_TYPE_INVALID, 0);

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      return (double)v->value.i8;
    }
    case VALUE_TYPE_UINT8: {
      return (double)v->value.u8;
    }
    case VALUE_TYPE_INT16: {
      return (double)v->value.i16;
    }
    case VALUE_TYPE_UINT16: {
      return (double)v->value.u16;
    }
    case VALUE_TYPE_INT32: {
      return (double)v->value.i32;
    }
    case VALUE_TYPE_UINT32: {
      return (double)v->value.u32;
    }
    case VALUE_TYPE_INT64: {
      return (double)v->value.i64;
    }
    case VALUE_TYPE_UINT64: {
      return (double)v->value.u64;
    }
    case VALUE_TYPE_FLOAT: {
      return (double)v->value.f;
    }
    case VALUE_TYPE_FLOAT32: {
      return (double)v->value.f32;
    }
    case VALUE_TYPE_DOUBLE: {
      return v->value.f64;
    }
    case VALUE_TYPE_STRING: {
      return (double)tk_atof(v->value.str);
    }
    case VALUE_TYPE_WSTRING: {
      return (double)tk_watof(v->value.wstr);
    }
    case VALUE_TYPE_BOOL: {
      return value_bool(v) ? 1 : 0;
    }
    default:
      break;
  }

  return 0;
}

value_t* value_set_str(value_t* v, const char* value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.str = value;
  return value_init(v, VALUE_TYPE_STRING);
}

value_t* value_dup_str(value_t* v, const char* value) {
  return_value_if_fail(v != NULL, NULL);

  value_init(v, VALUE_TYPE_STRING);
  v->value.str = tk_strdup(value);
  v->free_handle = TRUE;

  return v;
}

value_t* value_set_wstr(value_t* v, const wchar_t* value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.wstr = value;
  return value_init(v, VALUE_TYPE_WSTRING);
}

const char* value_str(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  return_value_if_fail(v->type == VALUE_TYPE_STRING, NULL);

  return v->value.str;
}

const wchar_t* value_wstr(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  return_value_if_fail(v->type == VALUE_TYPE_WSTRING, NULL);

  return v->value.wstr;
}

ret_t value_copy(value_t* dst, const value_t* src) {
  return_value_if_fail(dst != NULL && src != NULL, RET_BAD_PARAMS);

  if (dst == src) {
    return RET_OK;
  }

  memcpy(dst, src, sizeof(value_t));
  dst->free_handle = FALSE;

  return RET_OK;
}

ret_t value_deep_copy(value_t* dst, const value_t* src) {
  return_value_if_fail(dst != NULL && src != NULL, RET_BAD_PARAMS);

  value_copy(dst, src);

  switch (dst->type) {
    case VALUE_TYPE_STRING: {
      dst->value.str = tk_strdup(src->value.str);
      dst->free_handle = dst->value.str != NULL;
      break;
    }
    case VALUE_TYPE_BINARY:
    case VALUE_TYPE_UBJSON: {
      if (src->value.binary_data.data != NULL) {
        dst->value.binary_data.data = TKMEM_ALLOC(src->value.binary_data.size);
        return_value_if_fail(dst->value.binary_data.data != NULL, RET_OOM);
        memcpy(dst->value.binary_data.data, src->value.binary_data.data,
               src->value.binary_data.size);
        dst->free_handle = TRUE;
      } else {
        dst->free_handle = FALSE;
      }
      break;
    }
    case VALUE_TYPE_WSTRING: {
      dst->value.wstr = tk_wstrdup(src->value.wstr);
      dst->free_handle = dst->value.wstr != NULL;
      break;
    }
    case VALUE_TYPE_OBJECT: {
      object_ref(dst->value.object);
      dst->free_handle = dst->value.object != NULL;
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

bool_t value_is_null(value_t* v) {
  return v == NULL || v->type == VALUE_TYPE_INVALID;
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
    case VALUE_TYPE_WSTRING: {
      return tk_watoi(v->value.wstr);
    }
    default: { assert(!"not supported type"); }
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
      return tk_lfequal(v->value.f64, other->value.f64);
    }
    case VALUE_TYPE_STRING: {
      return (v->value.str == other->value.str) || tk_str_eq(v->value.str, other->value.str);
    }
    case VALUE_TYPE_WSTRING: {
      return (v->value.wstr == other->value.wstr) || tk_wstr_eq(v->value.wstr, other->value.wstr);
    }
    case VALUE_TYPE_OBJECT: {
      return object_compare(v->value.object, other->value.object) == 0;
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

ret_t value_reset(value_t* v) {
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);

  if (v->free_handle) {
    switch (v->type) {
      case VALUE_TYPE_SIZED_STRING: {
        TKMEM_FREE(v->value.sized_str.str);
        break;
      }
      case VALUE_TYPE_BINARY: {
        TKMEM_FREE(v->value.binary_data.data);
        break;
      }
      case VALUE_TYPE_STRING: {
        TKMEM_FREE(v->value.str);
        break;
      }
      case VALUE_TYPE_WSTRING: {
        TKMEM_FREE(v->value.wstr);
        break;
      }
      case VALUE_TYPE_OBJECT: {
        object_unref(v->value.object);
        break;
      }
      default:
        break;
    }
  }

  memset(v, 0x00, sizeof(value_t));

  return RET_OK;
}

ret_t value_destroy(value_t* v) {
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);

  value_reset(v);
  TKMEM_FREE(v);

  return RET_OK;
}

value_t* value_set_object(value_t* v, object_t* value) {
  return_value_if_fail(v != NULL && value != NULL, NULL);

  v->type = VALUE_TYPE_OBJECT;
  v->value.object = value;

  return v;
}

object_t* value_object(const value_t* v) {
  return_value_if_fail(v != NULL && v->type == VALUE_TYPE_OBJECT, NULL);

  return v->value.object;
}

value_t* value_cast(value_t* value) {
  return_value_if_fail(value != NULL, NULL);

  return value;
}

value_t* value_set_token(value_t* v, uint32_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.token = value;

  return value_init(v, VALUE_TYPE_TOKEN);
}

uint32_t value_token(const value_t* v) {
  return_value_if_fail(v != NULL, 0);
  return_value_if_fail(v->type == VALUE_TYPE_TOKEN, 0);

  return v->value.token;
}

value_t* value_set_sized_str(value_t* v, char* str, uint32_t size) {
  return_value_if_fail(v != NULL, NULL);

  v->value.sized_str.str = str;
  v->value.sized_str.size = size;

  return value_init(v, VALUE_TYPE_SIZED_STRING);
}

sized_str_t* value_sized_str(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  return_value_if_fail(v->type == VALUE_TYPE_SIZED_STRING, NULL);

  return (sized_str_t*)&(v->value.sized_str);
}

value_t* value_set_binary_data(value_t* v, void* data, uint32_t size) {
  return_value_if_fail(v != NULL, NULL);

  v->value.binary_data.data = data;
  v->value.binary_data.size = size;

  return value_init(v, VALUE_TYPE_BINARY);
}

binary_data_t* value_binary_data(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  return_value_if_fail(v->type == VALUE_TYPE_BINARY, NULL);

  return (binary_data_t*)&(v->value.binary_data);
}

value_t* value_set_ubjson(value_t* v, void* data, uint32_t size) {
  return_value_if_fail(v != NULL, NULL);

  v->value.binary_data.data = data;
  v->value.binary_data.size = size;

  return value_init(v, VALUE_TYPE_UBJSON);
}

binary_data_t* value_ubjson(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  return_value_if_fail(v->type == VALUE_TYPE_UBJSON, NULL);

  return (binary_data_t*)&(v->value.binary_data);
}
