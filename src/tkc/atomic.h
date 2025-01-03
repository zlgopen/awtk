/**
 * File:   atomic.h
 * Author: AWTK Develop Team
 * Brief:  原子操作
 *
 * Copyright (c) 2024 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-11-17 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_ATOMIC_H
#define TK_ATOMIC_H

#include "atomic_typedef.h"

#include "tkc/value.h"
#include "tkc/mem.h"

/**
 * @method tk_atomic_support_value_type
 * @annotation ["static"]
 * @export none
 * 判断原子操作是否支持该类型。
 *
 * @param {value_type_t} type 类型。
 *
 * @return {bool_t} 返回 TRUE 表示支持，FALSE 表示不支持。
 */
static inline bool_t tk_atomic_support_value_type(value_type_t type) {
  switch (type) {
    case VALUE_TYPE_BOOL:
    case VALUE_TYPE_INT8:
    case VALUE_TYPE_UINT8:
    case VALUE_TYPE_INT16:
    case VALUE_TYPE_UINT16:
    case VALUE_TYPE_INT32:
    case VALUE_TYPE_UINT32:
    case VALUE_TYPE_INT64:
    case VALUE_TYPE_UINT64:
      return TRUE;
    default:
      return FALSE;
  }
}

/**
 * @method tk_atomic_deinit
 * @export none
 * 释放原子操作类对象。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static ret_t tk_atomic_deinit(tk_atomic_t* atomic);

/**
 * @method tk_atomic_init
 * @export none
 * 初始化原子操作类对象。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {const value_t*} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static ret_t tk_atomic_init(tk_atomic_t* atomic, const value_t* v);

/**
 * @method tk_atomic_exchange
 * @export none
 * 原子交换操作。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {value_t*} v 交换值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static ret_t tk_atomic_exchange(tk_atomic_t* atomic, value_t* v);

/**
 * @method tk_atomic_compare_exchange_weak
 * @export none
 * 原子比较交换操作(Compare And Swap)。
 * > 如果 atomic 等于 expect，则将 atomic 改为 desire, 否则将 expect 改为 atomic。
 * 
 * weak版本的CAS允许偶然出乎意料的返回（比如在字段值和期待值一样的时候却返回了FALSE），
 * 不过在一些循环算法中，这是可以接受的。通常它比起strong有更高的性能。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {value_t*} expect 期望值。
 * @param {value_t*} desire 设定值。
 *
 * @return {bool_t} 返回TRUE表示成功，否则表示失败。
 */
static bool_t tk_atomic_compare_exchange_weak(tk_atomic_t* atomic, value_t* expect,
                                              value_t* desire);

/**
 * @method tk_atomic_compare_exchange_strong
 * @export none
 * 原子比较交换操作(Compare And Swap)。
 * > 如果 atomic 等于 expect，则将 atomic 改为 desire, 否则将 expect 改为 atomic。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {value_t*} expect 期望值。
 * @param {value_t*} desire 设定值。
 *
 * @return {bool_t} 返回TRUE表示成功，否则表示失败。
 */
static bool_t tk_atomic_compare_exchange_strong(tk_atomic_t* atomic, value_t* expect,
                                                value_t* desire);

/**
 * @method tk_atomic_store
 * @export none
 * 原子写操作。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {const value_t*} v 写入值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static ret_t tk_atomic_store(tk_atomic_t* atomic, const value_t* v);

/**
 * @method tk_atomic_load
 * @export none
 * 原子读操作。
 *
 * @param {const tk_atomic_t*} atomic 原子操作类对象。
 * @param {value_t*} v 用于返回读取值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static ret_t tk_atomic_load(const tk_atomic_t* atomic, value_t* v);

/**
 * @method tk_atomic_fetch_add
 * @export none
 * 原子加操作。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {value_t*} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static ret_t tk_atomic_fetch_add(tk_atomic_t* atomic, value_t* v);

/**
 * @method tk_atomic_fetch_sub
 * @export none
 * 原子减操作。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {value_t*} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static ret_t tk_atomic_fetch_sub(tk_atomic_t* atomic, value_t* v);

#if defined(WIN32) && !defined(MINGW)
#include <winnt.h>

struct _tk_atomic_t {
  volatile value_t value;
};

static inline ret_t tk_atomic_deinit(tk_atomic_t* atomic) {
  return_value_if_fail(atomic != NULL, RET_BAD_PARAMS);
  memset(atomic, 0, sizeof(tk_atomic_t));
  return RET_OK;
}

static inline ret_t tk_atomic_init(tk_atomic_t* atomic, const value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(tk_atomic_support_value_type(v->type), RET_BAD_PARAMS);

  ret = tk_atomic_deinit(atomic);
  return_value_if_fail(RET_OK == ret, ret);

  ret = value_copy((value_t*)(&atomic->value), v);
  return_value_if_fail(RET_OK == ret, ret);

  return ret;
}

static ret_t tk_atomic_exchange(tk_atomic_t* atomic, value_t* v) {
  value_t tmp;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  switch (atomic->value.type) {
    case VALUE_TYPE_BOOL: {
      value_set_bool(&tmp,
                     InterlockedExchange8((CHAR*)(&atomic->value.value.b), (CHAR)value_bool(v)));
    } break;
    case VALUE_TYPE_INT8: {
      value_set_int8(&tmp,
                     InterlockedExchange8((CHAR*)(&atomic->value.value.i8), (CHAR)value_int8(v)));
    } break;
    case VALUE_TYPE_UINT8: {
      value_set_uint8(&tmp,
                      InterlockedExchange8((CHAR*)(&atomic->value.value.u8), (CHAR)value_uint8(v)));
    } break;
    case VALUE_TYPE_INT16: {
      value_set_int16(
          &tmp, InterlockedExchange16((SHORT*)(&atomic->value.value.i16), (SHORT)value_int16(v)));
    } break;
    case VALUE_TYPE_UINT16: {
      value_set_uint16(
          &tmp, InterlockedExchange16((SHORT*)(&atomic->value.value.u16), (SHORT)value_uint16(v)));
    } break;
    case VALUE_TYPE_INT32: {
      value_set_int32(&tmp,
                      InterlockedExchange((LONG*)(&atomic->value.value.i32), (LONG)value_int32(v)));
    } break;
    case VALUE_TYPE_UINT32: {
      value_set_uint32(
          &tmp, InterlockedExchange((LONG*)(&atomic->value.value.u32), (LONG)value_uint32(v)));
    } break;
    case VALUE_TYPE_INT64: {
      value_set_int64(
          &tmp, InterlockedExchange64((LONG64*)(&atomic->value.value.i64), (LONG64)value_int64(v)));
    } break;
    case VALUE_TYPE_UINT64: {
      value_set_uint64(&tmp, InterlockedExchange64((LONG64*)(&atomic->value.value.u64),
                                                   (LONG64)value_uint64(v)));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return value_copy(v, &tmp);
}

static inline bool_t tk_atomic_compare_exchange_weak(tk_atomic_t* atomic, value_t* expect,
                                                     value_t* desire) {
  return tk_atomic_compare_exchange_strong(atomic, expect, desire);
}

static bool_t tk_atomic_compare_exchange_strong(tk_atomic_t* atomic, value_t* expect,
                                                value_t* desire) {
  bool_t ret = TRUE;
  value_t tmp;
  return_value_if_fail(atomic != NULL && expect != NULL && desire != NULL, FALSE);

  switch (atomic->value.type) {
    case VALUE_TYPE_BOOL: {
      value_set_bool(
          &tmp, InterlockedCompareExchange16((SHORT*)(&atomic->value.value.b),
                                             (SHORT)value_bool(desire), (SHORT)value_bool(expect)));
    } break;
    case VALUE_TYPE_INT8: {
      value_set_int8(
          &tmp, InterlockedCompareExchange16((SHORT*)(&atomic->value.value.i8),
                                             (SHORT)value_int8(desire), (SHORT)value_int8(expect)));
    } break;
    case VALUE_TYPE_UINT8: {
      value_set_uint8(&tmp, InterlockedCompareExchange16((SHORT*)(&atomic->value.value.u8),
                                                         (SHORT)value_uint8(desire),
                                                         (SHORT)value_uint8(expect)));
    } break;
    case VALUE_TYPE_INT16: {
      value_set_int16(&tmp, InterlockedCompareExchange16((SHORT*)(&atomic->value.value.i16),
                                                         (SHORT)value_int16(desire),
                                                         (SHORT)value_int16(expect)));
    } break;
    case VALUE_TYPE_UINT16: {
      value_set_uint16(&tmp, InterlockedCompareExchange16((SHORT*)(&atomic->value.value.u16),
                                                          (SHORT)value_uint16(desire),
                                                          (SHORT)value_uint16(expect)));
    } break;
    case VALUE_TYPE_INT32: {
      value_set_int32(
          &tmp, InterlockedCompareExchange((LONG*)(&atomic->value.value.i32),
                                           (LONG)value_int32(desire), (LONG)value_int32(expect)));
    } break;
    case VALUE_TYPE_UINT32: {
      value_set_uint32(
          &tmp, InterlockedCompareExchange((LONG*)(&atomic->value.value.u32),
                                           (LONG)value_uint32(desire), (LONG)value_uint32(expect)));
    } break;
    case VALUE_TYPE_INT64: {
      value_set_int64(&tmp, InterlockedCompareExchange64((LONG64*)(&atomic->value.value.i64),
                                                         (LONG64)value_int64(desire),
                                                         (LONG64)value_int64(expect)));
    } break;
    case VALUE_TYPE_UINT64: {
      value_set_uint64(&tmp, InterlockedCompareExchange64((LONG64*)(&atomic->value.value.u64),
                                                          (LONG64)value_uint64(desire),
                                                          (LONG64)value_uint64(expect)));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return FALSE;
    } break;
  }

  ret = value_equal(expect, &tmp);
  if (!ret) {
    value_copy(expect, &tmp);
  }

  return ret;
}

static inline ret_t tk_atomic_store(tk_atomic_t* atomic, const value_t* v) {
  value_t tmp;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  value_copy(&tmp, v);
  return tk_atomic_exchange(atomic, &tmp);
}

static ret_t tk_atomic_load(const tk_atomic_t* atomic, value_t* v) {
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  value_set_int(v, 0);

  switch (atomic->value.type) {
    case VALUE_TYPE_BOOL: {
      v->value.b = atomic->value.value.b;
    } break;
    case VALUE_TYPE_INT8: {
      v->value.i8 = atomic->value.value.i8;
    } break;
    case VALUE_TYPE_UINT8: {
      v->value.u8 = atomic->value.value.u8;
    } break;
    case VALUE_TYPE_INT16: {
      v->value.i16 = atomic->value.value.i16;
    } break;
    case VALUE_TYPE_UINT16: {
      v->value.u16 = atomic->value.value.u16;
    } break;
    case VALUE_TYPE_INT32: {
      v->value.i32 = atomic->value.value.i32;
    } break;
    case VALUE_TYPE_UINT32: {
      v->value.u32 = atomic->value.value.u32;
    } break;
    case VALUE_TYPE_INT64: {
      v->value.i64 = atomic->value.value.i64;
    } break;
    case VALUE_TYPE_UINT64: {
      v->value.u64 = atomic->value.value.u64;
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  v->type = atomic->value.type;

  return RET_OK;
}

static ret_t tk_atomic_fetch_add(tk_atomic_t* atomic, value_t* v) {
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  switch (atomic->value.type) {
    case VALUE_TYPE_BOOL: {
      if (value_bool(v)) {
        tk_atomic_store(atomic, v);
      }
    } break;
    case VALUE_TYPE_INT8: {
      if (1 == value_int(v)) {
        InterlockedIncrement16((SHORT*)(&atomic->value.value.i8));
      } else {
        InterlockedAdd((LONG*)(&atomic->value.value.i8), (LONG)value_int8(v));
      }
    } break;
    case VALUE_TYPE_UINT8: {
      if (1 == value_int(v)) {
        InterlockedIncrement16((SHORT*)(&atomic->value.value.u8));
      } else {
        InterlockedAdd((LONG*)(&atomic->value.value.u8), (LONG)value_uint8(v));
      }
    } break;
    case VALUE_TYPE_INT16: {
      if (1 == value_int(v)) {
        InterlockedIncrement16((SHORT*)(&atomic->value.value.i16));
      } else {
        InterlockedAdd((LONG*)(&atomic->value.value.i16), (LONG)value_int16(v));
      }
    } break;
    case VALUE_TYPE_UINT16: {
      if (1 == value_int(v)) {
        InterlockedIncrement16((SHORT*)(&atomic->value.value.u16));
      } else {
        InterlockedAdd((LONG*)(&atomic->value.value.u16), (LONG)value_uint16(v));
      }
    } break;
    case VALUE_TYPE_INT32: {
      if (1 == value_int(v)) {
        InterlockedIncrement((LONG*)(&atomic->value.value.i32));
      } else {
        InterlockedAdd((LONG*)(&atomic->value.value.i32), (LONG)value_int32(v));
      }
    } break;
    case VALUE_TYPE_UINT32: {
      if (1 == value_int(v)) {
        InterlockedIncrement((LONG*)(&atomic->value.value.u32));
      } else {
        InterlockedAdd((LONG*)(&atomic->value.value.u32), (LONG)value_uint32(v));
      }
    } break;
    case VALUE_TYPE_INT64: {
      if (1 == value_int(v)) {
        InterlockedIncrement64((LONG64*)(&atomic->value.value.i64));
      } else {
        InterlockedAdd64((LONG64*)(&atomic->value.value.i64), (LONG64)value_int64(v));
      }
    } break;
    case VALUE_TYPE_UINT64: {
      if (1 == value_int(v)) {
        InterlockedIncrement64((LONG64*)(&atomic->value.value.u64));
      } else {
        InterlockedAdd64((LONG64*)(&atomic->value.value.u64), (LONG64)value_uint64(v));
      }
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return RET_OK;
}

static ret_t tk_atomic_fetch_sub(tk_atomic_t* atomic, value_t* v) {
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  switch (atomic->value.type) {
    case VALUE_TYPE_BOOL: {
      if (value_bool(v)) {
        value_t tmp;
        tk_atomic_store(atomic, value_set_bool(&tmp, FALSE));
      }
    } break;
    case VALUE_TYPE_INT8: {
      if (1 == value_int(v)) {
        InterlockedDecrement16((SHORT*)(&atomic->value.value.i8));
      } else {
        InterlockedAdd((LONG*)(&atomic->value.value.i8), -(LONG)value_int8(v));
      }
    } break;
    case VALUE_TYPE_UINT8: {
      if (1 == value_int(v)) {
        InterlockedDecrement16((SHORT*)(&atomic->value.value.u8));
      } else {
        InterlockedAdd((LONG*)(&atomic->value.value.u8), -(LONG)value_uint8(v));
      }
    } break;
    case VALUE_TYPE_INT16: {
      if (1 == value_int(v)) {
        InterlockedDecrement16((SHORT*)(&atomic->value.value.i16));
      } else {
        InterlockedAdd((LONG*)(&atomic->value.value.i16), -(LONG)value_int16(v));
      }
    } break;
    case VALUE_TYPE_UINT16: {
      if (1 == value_int(v)) {
        InterlockedDecrement16((SHORT*)(&atomic->value.value.u16));
      } else {
        InterlockedAdd((LONG*)(&atomic->value.value.u16), -(LONG)value_uint16(v));
      }
    } break;
    case VALUE_TYPE_INT32: {
      if (1 == value_int(v)) {
        InterlockedDecrement((LONG*)(&atomic->value.value.i32));
      } else {
        InterlockedAdd((LONG*)(&atomic->value.value.i32), -(LONG)value_int32(v));
      }
    } break;
    case VALUE_TYPE_UINT32: {
      if (1 == value_int(v)) {
        InterlockedDecrement((LONG*)(&atomic->value.value.u32));
      } else {
        InterlockedAdd((LONG*)(&atomic->value.value.u32), -(LONG)value_uint32(v));
      }
    } break;
    case VALUE_TYPE_INT64: {
      if (1 == value_int(v)) {
        InterlockedDecrement64((LONG64*)(&atomic->value.value.i64));
      } else {
        InterlockedAdd64((LONG64*)(&atomic->value.value.i64), -(LONG64)value_int64(v));
      }
    } break;
    case VALUE_TYPE_UINT64: {
      if (1 == value_int(v)) {
        InterlockedDecrement64((LONG64*)(&atomic->value.value.u64));
      } else {
        InterlockedAdd64((LONG64*)(&atomic->value.value.u64), -(LONG64)value_uint64(v));
      }
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return RET_OK;
}
#elif ((__cplusplus >= 201103L) || (__STDC_VERSION__ >= 201112L)) && !defined(__STDC_NO_ATOMICS__)
#ifdef __cplusplus
#include <atomic>
#define _Atomic(X) std::atomic<X>
#else
#include <stdatomic.h>
#endif /* __cplusplus */

struct _tk_atomic_t {
  value_type_t type : 8;
  union _value {
    _Atomic(bool_t) b;
    _Atomic(int8_t) i8;
    _Atomic(uint8_t) u8;
    _Atomic(int16_t) i16;
    _Atomic(uint16_t) u16;
    _Atomic(int32_t) i32;
    _Atomic(uint32_t) u32;
    _Atomic(int64_t) i64;
    _Atomic(uint64_t) u64;
  } value;
};

static inline ret_t tk_atomic_deinit(tk_atomic_t* atomic) {
  return_value_if_fail(atomic != NULL, RET_BAD_PARAMS);
  memset(atomic, 0, sizeof(tk_atomic_t));
  return RET_OK;
}

static ret_t tk_atomic_init(tk_atomic_t* atomic, const value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(tk_atomic_support_value_type(v->type), RET_BAD_PARAMS);

  ret = tk_atomic_deinit(atomic);
  return_value_if_fail(RET_OK == ret, ret);

  atomic->type = v->type;

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      atomic_init(&atomic->value.b, value_bool(v));
    } break;
    case VALUE_TYPE_INT8: {
      atomic_init(&atomic->value.i8, value_int8(v));
    } break;
    case VALUE_TYPE_UINT8: {
      atomic_init(&atomic->value.u8, value_uint8(v));
    } break;
    case VALUE_TYPE_INT16: {
      atomic_init(&atomic->value.i16, value_int16(v));
    } break;
    case VALUE_TYPE_UINT16: {
      atomic_init(&atomic->value.u16, value_uint16(v));
    } break;
    case VALUE_TYPE_INT32: {
      atomic_init(&atomic->value.i32, value_int32(v));
    } break;
    case VALUE_TYPE_UINT32: {
      atomic_init(&atomic->value.u32, value_uint32(v));
    } break;
    case VALUE_TYPE_INT64: {
      atomic_init(&atomic->value.i64, value_int64(v));
    } break;
    case VALUE_TYPE_UINT64: {
      atomic_init(&atomic->value.u64, value_uint64(v));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return ret;
}

static ret_t tk_atomic_exchange(tk_atomic_t* atomic, value_t* v) {
  value_t tmp;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      value_set_bool(&tmp, atomic_exchange(&atomic->value.b, value_bool(v)));
    } break;
    case VALUE_TYPE_INT8: {
      value_set_int8(&tmp, atomic_exchange(&atomic->value.i8, value_int8(v)));
    } break;
    case VALUE_TYPE_UINT8: {
      value_set_uint8(&tmp, atomic_exchange(&atomic->value.u8, value_uint8(v)));
    } break;
    case VALUE_TYPE_INT16: {
      value_set_int16(&tmp, atomic_exchange(&atomic->value.i16, value_int16(v)));
    } break;
    case VALUE_TYPE_UINT16: {
      value_set_uint16(&tmp, atomic_exchange(&atomic->value.u16, value_uint16(v)));
    } break;
    case VALUE_TYPE_INT32: {
      value_set_int32(&tmp, atomic_exchange(&atomic->value.i32, value_int32(v)));
    } break;
    case VALUE_TYPE_UINT32: {
      value_set_uint32(&tmp, atomic_exchange(&atomic->value.u32, value_uint32(v)));
    } break;
    case VALUE_TYPE_INT64: {
      value_set_int64(&tmp, atomic_exchange(&atomic->value.i64, value_int64(v)));
    } break;
    case VALUE_TYPE_UINT64: {
      value_set_uint64(&tmp, atomic_exchange(&atomic->value.u64, value_uint64(v)));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return value_copy(v, &tmp);
}

static bool_t tk_atomic_compare_exchange_weak(tk_atomic_t* atomic, value_t* expect,
                                              value_t* desire) {
  bool_t ret = TRUE;
  return_value_if_fail(atomic != NULL && expect != NULL && desire != NULL, FALSE);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      bool_t tmp = value_bool(expect);
      ret = atomic_compare_exchange_weak(&atomic->value.b, &tmp, value_bool(desire));
      value_set_bool(expect, tmp);
    } break;
    case VALUE_TYPE_INT8: {
      int8_t tmp = value_int8(expect);
      ret = atomic_compare_exchange_weak(&atomic->value.i8, &tmp, value_int8(desire));
      value_set_int8(expect, tmp);
    } break;
    case VALUE_TYPE_UINT8: {
      uint8_t tmp = value_uint8(expect);
      ret = atomic_compare_exchange_weak(&atomic->value.u8, &tmp, value_uint8(desire));
      value_set_uint8(expect, tmp);
    } break;
    case VALUE_TYPE_INT16: {
      int16_t tmp = value_int16(expect);
      ret = atomic_compare_exchange_weak(&atomic->value.i16, &tmp, value_int16(desire));
      value_set_int16(expect, tmp);
    } break;
    case VALUE_TYPE_UINT16: {
      uint16_t tmp = value_uint16(expect);
      ret = atomic_compare_exchange_weak(&atomic->value.u16, &tmp, value_uint16(desire));
      value_set_uint16(expect, tmp);
    } break;
    case VALUE_TYPE_INT32: {
      int32_t tmp = value_int32(expect);
      ret = atomic_compare_exchange_weak(&atomic->value.i32, &tmp, value_int32(desire));
      value_set_int32(expect, tmp);
    } break;
    case VALUE_TYPE_UINT32: {
      uint32_t tmp = value_uint32(expect);
      ret = atomic_compare_exchange_weak(&atomic->value.u32, &tmp, value_uint32(desire));
      value_set_uint32(expect, tmp);
    } break;
    case VALUE_TYPE_INT64: {
      int64_t tmp = value_int64(expect);
      ret = atomic_compare_exchange_weak(&atomic->value.i64, &tmp, value_int64(desire));
      value_set_int64(expect, tmp);
    } break;
    case VALUE_TYPE_UINT64: {
      uint64_t tmp = value_uint64(expect);
      ret = atomic_compare_exchange_weak(&atomic->value.u64, &tmp, value_uint64(desire));
      value_set_uint64(expect, tmp);
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return FALSE;
    } break;
  }

  return ret;
}

static bool_t tk_atomic_compare_exchange_strong(tk_atomic_t* atomic, value_t* expect,
                                                value_t* desire) {
  bool_t ret = TRUE;
  return_value_if_fail(atomic != NULL && expect != NULL && desire != NULL, FALSE);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      bool_t tmp = value_bool(expect);
      ret = atomic_compare_exchange_strong(&atomic->value.b, &tmp, value_bool(desire));
      value_set_bool(expect, tmp);
    } break;
    case VALUE_TYPE_INT8: {
      int8_t tmp = value_int8(expect);
      ret = atomic_compare_exchange_strong(&atomic->value.i8, &tmp, value_int8(desire));
      value_set_int8(expect, tmp);
    } break;
    case VALUE_TYPE_UINT8: {
      uint8_t tmp = value_uint8(expect);
      ret = atomic_compare_exchange_strong(&atomic->value.u8, &tmp, value_uint8(desire));
      value_set_uint8(expect, tmp);
    } break;
    case VALUE_TYPE_INT16: {
      int16_t tmp = value_int16(expect);
      ret = atomic_compare_exchange_strong(&atomic->value.i16, &tmp, value_int16(desire));
      value_set_int16(expect, tmp);
    } break;
    case VALUE_TYPE_UINT16: {
      uint16_t tmp = value_uint16(expect);
      ret = atomic_compare_exchange_strong(&atomic->value.u16, &tmp, value_uint16(desire));
      value_set_uint16(expect, tmp);
    } break;
    case VALUE_TYPE_INT32: {
      int32_t tmp = value_int32(expect);
      ret = atomic_compare_exchange_strong(&atomic->value.i32, &tmp, value_int32(desire));
      value_set_int32(expect, tmp);
    } break;
    case VALUE_TYPE_UINT32: {
      uint32_t tmp = value_uint32(expect);
      ret = atomic_compare_exchange_strong(&atomic->value.u32, &tmp, value_uint32(desire));
      value_set_uint32(expect, tmp);
    } break;
    case VALUE_TYPE_INT64: {
      int64_t tmp = value_int64(expect);
      ret = atomic_compare_exchange_strong(&atomic->value.i64, &tmp, value_int64(desire));
      value_set_int64(expect, tmp);
    } break;
    case VALUE_TYPE_UINT64: {
      uint64_t tmp = value_uint64(expect);
      ret = atomic_compare_exchange_strong(&atomic->value.u64, &tmp, value_uint64(desire));
      value_set_uint64(expect, tmp);
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return FALSE;
    } break;
  }

  return ret;
}

static ret_t tk_atomic_store(tk_atomic_t* atomic, const value_t* v) {
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      atomic_store(&atomic->value.b, value_bool(v));
    } break;
    case VALUE_TYPE_INT8: {
      atomic_store(&atomic->value.i8, value_int8(v));
    } break;
    case VALUE_TYPE_UINT8: {
      atomic_store(&atomic->value.u8, value_uint8(v));
    } break;
    case VALUE_TYPE_INT16: {
      atomic_store(&atomic->value.i16, value_int16(v));
    } break;
    case VALUE_TYPE_UINT16: {
      atomic_store(&atomic->value.u16, value_uint16(v));
    } break;
    case VALUE_TYPE_INT32: {
      atomic_store(&atomic->value.i32, value_int32(v));
    } break;
    case VALUE_TYPE_UINT32: {
      atomic_store(&atomic->value.u32, value_uint32(v));
    } break;
    case VALUE_TYPE_INT64: {
      atomic_store(&atomic->value.i64, value_int64(v));
    } break;
    case VALUE_TYPE_UINT64: {
      atomic_store(&atomic->value.u64, value_uint64(v));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return RET_OK;
}

static ret_t tk_atomic_load(const tk_atomic_t* atomic, value_t* v) {
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      value_set_bool(v, atomic_load(&atomic->value.b));
    } break;
    case VALUE_TYPE_INT8: {
      value_set_int8(v, atomic_load(&atomic->value.i8));
    } break;
    case VALUE_TYPE_UINT8: {
      value_set_uint8(v, atomic_load(&atomic->value.u8));
    } break;
    case VALUE_TYPE_INT16: {
      value_set_int16(v, atomic_load(&atomic->value.i16));
    } break;
    case VALUE_TYPE_UINT16: {
      value_set_uint16(v, atomic_load(&atomic->value.u16));
    } break;
    case VALUE_TYPE_INT32: {
      value_set_int32(v, atomic_load(&atomic->value.i32));
    } break;
    case VALUE_TYPE_UINT32: {
      value_set_uint32(v, atomic_load(&atomic->value.u32));
    } break;
    case VALUE_TYPE_INT64: {
      value_set_int64(v, atomic_load(&atomic->value.i64));
    } break;
    case VALUE_TYPE_UINT64: {
      value_set_uint64(v, atomic_load(&atomic->value.u64));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return RET_OK;
}

static ret_t tk_atomic_fetch_add(tk_atomic_t* atomic, value_t* v) {
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      if (value_bool(v)) {
        tk_atomic_store(atomic, v);
      }
    } break;
    case VALUE_TYPE_INT8: {
      atomic_fetch_add(&atomic->value.i8, value_int8(v));
    } break;
    case VALUE_TYPE_UINT8: {
      atomic_fetch_add(&atomic->value.u8, value_uint8(v));
    } break;
    case VALUE_TYPE_INT16: {
      atomic_fetch_add(&atomic->value.i16, value_int16(v));
    } break;
    case VALUE_TYPE_UINT16: {
      atomic_fetch_add(&atomic->value.u16, value_uint16(v));
    } break;
    case VALUE_TYPE_INT32: {
      atomic_fetch_add(&atomic->value.i32, value_int32(v));
    } break;
    case VALUE_TYPE_UINT32: {
      atomic_fetch_add(&atomic->value.u32, value_uint32(v));
    } break;
    case VALUE_TYPE_INT64: {
      atomic_fetch_add(&atomic->value.i64, value_int64(v));
    } break;
    case VALUE_TYPE_UINT64: {
      atomic_fetch_add(&atomic->value.u64, value_uint64(v));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return RET_OK;
}

static ret_t tk_atomic_fetch_sub(tk_atomic_t* atomic, value_t* v) {
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      if (value_bool(v)) {
        value_t tmp;
        tk_atomic_store(atomic, value_set_bool(&tmp, FALSE));
      }
    } break;
    case VALUE_TYPE_INT8: {
      atomic_fetch_sub(&atomic->value.i8, value_int8(v));
    } break;
    case VALUE_TYPE_UINT8: {
      atomic_fetch_sub(&atomic->value.u8, value_uint8(v));
    } break;
    case VALUE_TYPE_INT16: {
      atomic_fetch_sub(&atomic->value.i16, value_int16(v));
    } break;
    case VALUE_TYPE_UINT16: {
      atomic_fetch_sub(&atomic->value.u16, value_uint16(v));
    } break;
    case VALUE_TYPE_INT32: {
      atomic_fetch_sub(&atomic->value.i32, value_int32(v));
    } break;
    case VALUE_TYPE_UINT32: {
      atomic_fetch_sub(&atomic->value.u32, value_uint32(v));
    } break;
    case VALUE_TYPE_INT64: {
      atomic_fetch_sub(&atomic->value.i64, value_int64(v));
    } break;
    case VALUE_TYPE_UINT64: {
      atomic_fetch_sub(&atomic->value.u64, value_uint64(v));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return RET_OK;
}
#else
/* 不支持原子操作，已退化为互斥锁，编译时请指定C11以上的C语言标准，以确保支持原子操作！ */
#pragma message(__FILE__ "(" TK_STRINGIZE( \
    __LINE__) "): "                        \
              "Warning : Atomic operation is not supported and have degenerated into mutex, please specify the C language standard above C11 when compiling to ensure that atomic operation is supported!")
#ifdef __cplusplus
#pragma message("__cplusplus = " TK_STRINGIZE(__cplusplus))
#else
#pragma message("__STDC_VERSION__ = " TK_STRINGIZE(__STDC_VERSION__))
#endif /* __cplusplus */
#ifdef __STDC_NO_ATOMICS__
#pragma message("__STDC_NO_ATOMICS__ = TRUE")
#else
#pragma message("__STDC_NO_ATOMICS__ = FALSE")
#endif /* __STDC_NO_ATOMICS__ */

#include "tkc/mutex.h"

struct _tk_atomic_t {
  value_t value;
  tk_mutex_t* lock;
};

static inline ret_t tk_atomic_deinit(tk_atomic_t* atomic) {
  ret_t ret = RET_OK;
  return_value_if_fail(atomic != NULL, RET_BAD_PARAMS);
  if (atomic->lock != NULL) {
    ret = tk_mutex_destroy(atomic->lock);
  }
  if (RET_OK == ret) {
    memset(atomic, 0, sizeof(tk_atomic_t));
  }
  return ret;
}

static inline ret_t tk_atomic_init(tk_atomic_t* atomic, const value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(tk_atomic_support_value_type(v->type), RET_BAD_PARAMS);

  ret = tk_atomic_deinit(atomic);
  return_value_if_fail(RET_OK == ret, ret);

  ret = value_copy(&atomic->value, v);
  return_value_if_fail(RET_OK == ret, ret);

  atomic->lock = tk_mutex_create();
  return_value_if_fail(atomic->lock != NULL, RET_OOM);

  return ret;
}

static inline ret_t tk_atomic_exchange(tk_atomic_t* atomic, value_t* v) {
  ret_t ret = RET_OK;
  value_t tmp;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  ret = tk_mutex_lock(atomic->lock);
  return_value_if_fail(RET_OK == ret, ret);

  /* 读-修改-写 */
  value_copy(&tmp, &atomic->value);
  ret = value_copy(&atomic->value, v);
  value_copy(v, &tmp);

  tk_mutex_unlock(atomic->lock);
  return ret;
}

static inline bool_t tk_atomic_compare_exchange_weak(tk_atomic_t* atomic, value_t* expect,
                                                     value_t* desire) {
  bool_t ret = TRUE;
  return_value_if_fail(atomic != NULL && expect != NULL && desire != NULL, FALSE);

  ret = (RET_OK == tk_mutex_try_lock(atomic->lock));
  if (ret) {
    ret = value_equal(&atomic->value, expect);
    if (ret) {
      value_copy(&atomic->value, desire);
    } else {
      value_copy(expect, &atomic->value);
    }

    tk_mutex_unlock(atomic->lock);
  }

  return ret;
}

static inline bool_t tk_atomic_compare_exchange_strong(tk_atomic_t* atomic, value_t* expect,
                                                       value_t* desire) {
  bool_t ret = TRUE;
  return_value_if_fail(atomic != NULL && expect != NULL && desire != NULL, FALSE);

  return_value_if_fail(RET_OK == tk_mutex_lock(atomic->lock), FALSE);

  ret = value_equal(&atomic->value, expect);
  if (ret) {
    value_copy(&atomic->value, desire);
  } else {
    value_copy(expect, &atomic->value);
  }

  tk_mutex_unlock(atomic->lock);
  return ret;
}

static inline ret_t tk_atomic_store(tk_atomic_t* atomic, const value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  ret = tk_mutex_lock(atomic->lock);
  return_value_if_fail(RET_OK == ret, ret);

  ret = value_copy(&atomic->value, v);

  tk_mutex_unlock(atomic->lock);
  return ret;
}

static inline ret_t tk_atomic_load(const tk_atomic_t* atomic, value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  ret = tk_mutex_lock(atomic->lock);
  return_value_if_fail(RET_OK == ret, ret);

  ret = value_copy(v, &atomic->value);

  tk_mutex_unlock(atomic->lock);
  return ret;
}

static inline ret_t tk_atomic_fetch_add(tk_atomic_t* atomic, value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  ret = tk_mutex_lock(atomic->lock);
  return_value_if_fail(RET_OK == ret, ret);

  ret = value_add(&atomic->value, v, &atomic->value);

  tk_mutex_unlock(atomic->lock);
  return ret;
}

static inline ret_t tk_atomic_fetch_sub(tk_atomic_t* atomic, value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  ret = tk_mutex_lock(atomic->lock);
  return_value_if_fail(RET_OK == ret, ret);

  ret = value_sub(&atomic->value, v, &atomic->value);

  tk_mutex_unlock(atomic->lock);
  return ret;
}
#endif

/**
 * @method tk_atomic_destroy
 * @annotation ["deconstructor"]
 * @export none
 * 销毁原子操作类对象。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t tk_atomic_destroy(tk_atomic_t* atomic) {
  ret_t ret = RET_OK;
  return_value_if_fail(atomic != NULL, RET_BAD_PARAMS);

  ret = tk_atomic_deinit(atomic);
  if (RET_OK == ret) {
    TKMEM_FREE(atomic);
  }

  return ret;
}

/**
 * @method tk_atomic_create
 * @annotation ["constructor"]
 * @export none
 * 创建原子操作类对象。
 *
 * @param {const value_t*} v 值。
 *
 * @return {tk_atomic_t*} 返回原子操作类对象。
 */
static inline tk_atomic_t* tk_atomic_create(const value_t* v) {
  tk_atomic_t* ret = NULL;
  return_value_if_fail(v != NULL, NULL);
  return_value_if_fail(tk_atomic_support_value_type(v->type), NULL);

  ret = TKMEM_ZALLOC(tk_atomic_t);
  return_value_if_fail(ret != NULL, NULL);

  goto_error_if_fail(RET_OK == tk_atomic_init(ret, v));

  return ret;
error:
  tk_atomic_destroy(ret);
  return NULL;
}

#endif /*TK_ATOMIC_H*/
