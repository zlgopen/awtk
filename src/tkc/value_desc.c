/**
 * File:   value_desc.c
 * Author: AWTK Develop Team
 * Brief:  value_desc
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
 * 2019-09-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "tkc/value_desc.h"

ret_t value_desc_validate(value_desc_t* schema, value_t* v) {
  return_value_if_fail(v != NULL && schema != NULL, RET_BAD_PARAMS);

  switch (schema->type) {
    case VALUE_DESC_TYPE_INT8: {
      int32_t value = value_int(v);
      const value_desc_int8_t* desc = (const value_desc_int8_t*)schema;
      return_value_if_fail(value >= desc->min && value <= desc->max, RET_FAIL);

      return RET_OK;
    }

    case VALUE_DESC_TYPE_INT16: {
      int32_t value = value_int(v);
      const value_desc_int16_t* desc = (const value_desc_int16_t*)schema;
      return_value_if_fail(value >= desc->min && value <= desc->max, RET_FAIL);
      return RET_OK;
    }
    case VALUE_DESC_TYPE_INT32: {
      int32_t value = value_int(v);
      const value_desc_int32_t* desc = (const value_desc_int32_t*)schema;
      return_value_if_fail(value >= desc->min && value <= desc->max, RET_FAIL);
      return RET_OK;
    }
    case VALUE_DESC_TYPE_INT64: {
      int64_t value = value_int64(v);
      const value_desc_int64_t* desc = (const value_desc_int64_t*)schema;
      return_value_if_fail(value >= desc->min && value <= desc->max, RET_FAIL);
      return RET_OK;
    }

    case VALUE_DESC_TYPE_UINT8: {
      int32_t value = value_int(v);
      const value_desc_uint8_t* desc = (const value_desc_uint8_t*)schema;
      return_value_if_fail(value >= desc->min && value <= desc->max, RET_FAIL);
      return RET_OK;
    }

    case VALUE_DESC_TYPE_UINT16: {
      int32_t value = value_int(v);
      const value_desc_uint16_t* desc = (const value_desc_uint16_t*)schema;
      return_value_if_fail(value >= desc->min && value <= desc->max, RET_FAIL);
      return RET_OK;
    }
    case VALUE_DESC_TYPE_UINT32: {
      uint32_t value = value_uint32(v);
      const value_desc_uint32_t* desc = (const value_desc_uint32_t*)schema;
      return_value_if_fail(value >= desc->min && value <= desc->max, RET_FAIL);
      return RET_OK;
    }
    case VALUE_DESC_TYPE_UINT64: {
      uint64_t value = value_uint64(v);
      const value_desc_uint64_t* desc = (const value_desc_uint64_t*)schema;
      return_value_if_fail(value >= desc->min && value <= desc->max, RET_FAIL);
      return RET_OK;
    }
    case VALUE_DESC_TYPE_STRING: {
      uint32_t len = 0;
      const char* value = value_str(v);
      const value_desc_string_t* desc = (const value_desc_string_t*)schema;
      return_value_if_fail(value != NULL, RET_FAIL);
      len = strlen(value);
      return_value_if_fail(len >= desc->min && len <= desc->max, RET_FAIL);
      return RET_OK;
    }
    case VALUE_DESC_TYPE_FLOAT: {
      float value = value_float32(v);
      const value_desc_float_t* desc = (const value_desc_float_t*)schema;
      return_value_if_fail(value >= desc->min && value <= desc->max, RET_FAIL);
      return RET_OK;
    }
    case VALUE_DESC_TYPE_DOUBLE: {
      double value = value_double(v);
      const value_desc_double_t* desc = (const value_desc_double_t*)schema;
      return_value_if_fail(value >= desc->min && value <= desc->max, RET_FAIL);
      return RET_OK;
    }
    case VALUE_DESC_TYPE_STRING_ENUMS: {
      uint32_t i = 0;
      const char* value = value_str(v);
      const value_desc_string_enums_t* desc = (const value_desc_string_enums_t*)schema;
      return_value_if_fail(desc->enums != NULL && value != NULL, RET_FAIL);
      for (i = 0; desc->enums[i] != NULL; i++) {
        if (tk_str_eq(desc->enums[i], value)) {
          return RET_OK;
        }
      }
      break;
    }

    case VALUE_DESC_TYPE_INT_ENUMS: {
      uint32_t i = 0;
      int32_t value = value_int(v);
      const value_desc_int_enums_t* desc = (const value_desc_int_enums_t*)schema;
      return_value_if_fail(desc->enums != NULL, RET_FAIL);

      for (i = 0; desc->enums[i] != NULL; i++) {
        if (tk_atoi(desc->enums[i]) == value) {
          return RET_OK;
        }
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}
