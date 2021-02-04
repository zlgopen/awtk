/**
 * File:   fscript_bits.c
 * Author: AWTK Develop Team
 * Brief:  bits functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-03 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/fscript.h"

typedef enum _func_type_t {
  FUNC_LSHIFT = 1,
  FUNC_RSHIFT = 2,
  FUNC_BIT_CLEAR = 3,
} func_type_t;

static ret_t func_lshift(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t n = 0;
  value_t* value = NULL;
  ret_t ret = RET_OK;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value = args->args;
  n = value_uint32(args->args + 1);

  switch (value->type) {
    case VALUE_TYPE_INT8: {
      value_set_int8(result, value_int8(value) << n);
      break;
    }
    case VALUE_TYPE_UINT8: {
      value_set_uint8(result, value_uint8(value) << n);
      break;
    }
    case VALUE_TYPE_INT16: {
      value_set_int16(result, value_int16(value) << n);
      break;
    }
    case VALUE_TYPE_UINT16: {
      value_set_uint16(result, value_uint16(value) << n);
      break;
    }
    case VALUE_TYPE_INT32: {
      value_set_int32(result, value_int32(value) << n);
      break;
    }
    case VALUE_TYPE_UINT32: {
      value_set_uint32(result, value_uint32(value) << n);
      break;
    }
    case VALUE_TYPE_INT64: {
      value_set_int64(result, value_int64(value) << n);
      break;
    }
    case VALUE_TYPE_UINT64: {
      value_set_uint64(result, value_uint64(value) << n);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      break;
    }
  }

  return ret;
}

static ret_t func_rshift(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t n = 0;
  value_t* value = NULL;
  ret_t ret = RET_OK;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value = args->args;
  n = value_uint32(args->args + 1);

  switch (value->type) {
    case VALUE_TYPE_INT8: {
      value_set_int8(result, value_int8(value) >> n);
      break;
    }
    case VALUE_TYPE_UINT8: {
      value_set_uint8(result, value_uint8(value) >> n);
      break;
    }
    case VALUE_TYPE_INT16: {
      value_set_int16(result, value_int16(value) >> n);
      break;
    }
    case VALUE_TYPE_UINT16: {
      value_set_uint16(result, value_uint16(value) >> n);
      break;
    }
    case VALUE_TYPE_INT32: {
      value_set_int32(result, value_int32(value) >> n);
      break;
    }
    case VALUE_TYPE_UINT32: {
      value_set_uint32(result, value_uint32(value) >> n);
      break;
    }
    case VALUE_TYPE_INT64: {
      value_set_int64(result, value_int64(value) >> n);
      break;
    }
    case VALUE_TYPE_UINT64: {
      value_set_uint64(result, value_uint64(value) >> n);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      break;
    }
  }

  return ret;
}

static ret_t func_bit_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t n = 0;
  value_t* value = NULL;
  ret_t ret = RET_OK;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value = args->args;
  n = value_uint32(args->args + 1);

  switch (value->type) {
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(value);
      TK_SET_BIT(vv, n);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(value);
      TK_SET_BIT(vv, n);
      value_set_uint8(result, vv);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(value);
      TK_SET_BIT(vv, n);
      value_set_int16(result, vv);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(value);
      TK_SET_BIT(vv, n);
      value_set_uint16(result, vv);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(value);
      TK_SET_BIT(vv, n);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(value);
      TK_SET_BIT(vv, n);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(value);
      TK_SET_BIT(vv, n);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(value);
      TK_SET_BIT(vv, n);
      value_set_uint64(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      break;
    }
  }

  return ret;
}

static ret_t func_bit_clear(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t n = 0;
  value_t* value = NULL;
  ret_t ret = RET_OK;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value = args->args;
  n = value_uint32(args->args + 1);

  switch (value->type) {
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(value);
      TK_CLEAR_BIT(vv, n);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(value);
      TK_CLEAR_BIT(vv, n);
      value_set_uint8(result, vv);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(value);
      TK_CLEAR_BIT(vv, n);
      value_set_int16(result, vv);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(value);
      TK_CLEAR_BIT(vv, n);
      value_set_uint16(result, vv);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(value);
      TK_CLEAR_BIT(vv, n);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(value);
      TK_CLEAR_BIT(vv, n);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(value);
      TK_CLEAR_BIT(vv, n);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(value);
      TK_CLEAR_BIT(vv, n);
      value_set_uint64(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      break;
    }
  }

  return ret;
}

static ret_t func_bit_toggle(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t n = 0;
  value_t* value = NULL;
  ret_t ret = RET_OK;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value = args->args;
  n = value_uint32(args->args + 1);

  switch (value->type) {
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(value);
      TK_TOGGLE_BIT(vv, n);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(value);
      TK_TOGGLE_BIT(vv, n);
      value_set_uint8(result, vv);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(value);
      TK_TOGGLE_BIT(vv, n);
      value_set_int16(result, vv);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(value);
      TK_TOGGLE_BIT(vv, n);
      value_set_uint16(result, vv);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(value);
      TK_TOGGLE_BIT(vv, n);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(value);
      TK_TOGGLE_BIT(vv, n);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(value);
      TK_TOGGLE_BIT(vv, n);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(value);
      TK_TOGGLE_BIT(vv, n);
      value_set_uint64(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      break;
    }
  }

  return ret;
}

static ret_t func_bit_get(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t n = 0;
  value_t* value = NULL;
  ret_t ret = RET_OK;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value = args->args;
  n = value_uint32(args->args + 1);

  switch (value->type) {
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(value);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(value);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(value);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(value);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(value);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(value);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(value);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(value);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      break;
    }
  }

  return ret;
}

static ret_t func_bit_and(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_uint32(result, value_uint32(args->args) & value_uint32(args->args + 1));

  return RET_OK;
}

static ret_t func_bit_or(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_uint32(result, value_uint32(args->args) | value_uint32(args->args + 1));

  return RET_OK;
}

static ret_t func_bit_nor(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_uint32(result, value_uint32(args->args) ^ value_uint32(args->args + 1));

  return RET_OK;
}

static ret_t func_bit_not(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_uint32(result, ~value_uint32(args->args));

  return RET_OK;
}

ret_t fscript_bits_register(void) {
  ENSURE(fscript_register_func("&", func_bit_and) == RET_OK);
  ENSURE(fscript_register_func("^", func_bit_nor) == RET_OK);
  ENSURE(fscript_register_func("~", func_bit_not) == RET_OK);
  ENSURE(fscript_register_func("|", func_bit_or) == RET_OK);
  ENSURE(fscript_register_func("<<", func_lshift) == RET_OK);
  ENSURE(fscript_register_func(">>", func_rshift) == RET_OK);
  ENSURE(fscript_register_func("bit_get", func_bit_get) == RET_OK);
  ENSURE(fscript_register_func("bit_set", func_bit_set) == RET_OK);
  ENSURE(fscript_register_func("bit_clear", func_bit_clear) == RET_OK);
  ENSURE(fscript_register_func("bit_toggle", func_bit_toggle) == RET_OK);

  return RET_OK;
}
