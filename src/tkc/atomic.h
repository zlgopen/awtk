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
 * @enum tk_atomic_memory_order_t
 * @prefix TK_ATOMIC_MEMORY_ORDER_
 * 内存顺序。
 */
typedef enum _tk_atomic_memory_order_t {
  /**
   * @value TK_ATOMIC_MEMORY_ORDER_SEQ_CST
   * 顺序一致性。
   */
  TK_ATOMIC_MEMORY_ORDER_SEQ_CST = 0,
  /**
   * @value TK_ATOMIC_MEMORY_ORDER_ACQUIRE
   * 获取顺序。
   */
  TK_ATOMIC_MEMORY_ORDER_ACQUIRE,
  /**
   * @value TK_ATOMIC_MEMORY_ORDER_RELEASE
   * 释放顺序。
   */
  TK_ATOMIC_MEMORY_ORDER_RELEASE,
  /**
   * @value TK_ATOMIC_MEMORY_ORDER_ACQ_REL
   * 获得+释放顺序。
   */
  TK_ATOMIC_MEMORY_ORDER_ACQ_REL,
  /**
   * @value TK_ATOMIC_MEMORY_ORDER_RELAXED
   * 松散顺序。
   */
  TK_ATOMIC_MEMORY_ORDER_RELAXED,
} tk_atomic_memory_order_t;

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
inline static bool_t tk_atomic_support_value_type(value_type_t type) {
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
 * @return {value_t} 返回操作前的旧值。
 */
static value_t tk_atomic_fetch_add(tk_atomic_t* atomic, value_t* v);

/**
 * @method tk_atomic_fetch_sub
 * @export none
 * 原子减操作。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {value_t*} v 值。
 *
 * @return {value_t} 返回操作前的旧值。
 */
static value_t tk_atomic_fetch_sub(tk_atomic_t* atomic, value_t* v);

/**
 * @method tk_atomic_thread_fence
 * @export none
 * 线程栅栏。
 *
 * @param {tk_atomic_memory_order_t} mem_order 内存顺序。
 *
 * @return {void}
 */
static void tk_atomic_thread_fence(tk_atomic_memory_order_t mem_order);

/**
 * @method tk_atomic_exchange_explicit
 * @export none
 * 原子交换操作。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {value_t*} v 交换值。
 * @param {tk_atomic_memory_order_t} mem_order 内存顺序。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static ret_t tk_atomic_exchange_explicit(tk_atomic_t* atomic, value_t* v,
                                         tk_atomic_memory_order_t mem_order);

/**
 * @method tk_atomic_compare_exchange_weak_explicit
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
 * @param {tk_atomic_memory_order_t} success_mem_order 成功时的内存顺序。
 * @param {tk_atomic_memory_order_t} failure_mem_order 失败时的内存顺序。
 *
 * @return {bool_t} 返回TRUE表示成功，否则表示失败。
 */
static bool_t tk_atomic_compare_exchange_weak_explicit(tk_atomic_t* atomic, value_t* expect,
                                                       value_t* desire,
                                                       tk_atomic_memory_order_t success_mem_order,
                                                       tk_atomic_memory_order_t failure_mem_order);

/**
 * @method tk_atomic_compare_exchange_strong_explicit
 * @export none
 * 原子比较交换操作(Compare And Swap)。
 * > 如果 atomic 等于 expect，则将 atomic 改为 desire, 否则将 expect 改为 atomic。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {value_t*} expect 期望值。
 * @param {value_t*} desire 设定值。
 * @param {tk_atomic_memory_order_t} success_mem_order 成功时的内存顺序。
 * @param {tk_atomic_memory_order_t} failure_mem_order 失败时的内存顺序。
 *
 * @return {bool_t} 返回TRUE表示成功，否则表示失败。
 */
static bool_t tk_atomic_compare_exchange_strong_explicit(
    tk_atomic_t* atomic, value_t* expect, value_t* desire,
    tk_atomic_memory_order_t success_mem_order, tk_atomic_memory_order_t failure_mem_order);

/**
 * @method tk_atomic_store_explicit
 * @export none
 * 原子写操作。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {const value_t*} v 写入值。
 * @param {tk_atomic_memory_order_t} mem_order 内存顺序。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static ret_t tk_atomic_store_explicit(tk_atomic_t* atomic, const value_t* v,
                                      tk_atomic_memory_order_t mem_order);

/**
 * @method tk_atomic_load_explicit
 * @export none
 * 原子读操作。
 *
 * @param {const tk_atomic_t*} atomic 原子操作类对象。
 * @param {value_t*} v 用于返回读取值。
 * @param {tk_atomic_memory_order_t} mem_order 内存顺序。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static ret_t tk_atomic_load_explicit(const tk_atomic_t* atomic, value_t* v,
                                     tk_atomic_memory_order_t mem_order);

/**
 * @method tk_atomic_fetch_add_explicit
 * @export none
 * 原子加操作。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {value_t*} v 值。
 * @param {tk_atomic_memory_order_t} mem_order 内存顺序。
 *
 * @return {value_t} 返回操作前的旧值。
 */
static value_t tk_atomic_fetch_add_explicit(tk_atomic_t* atomic, value_t* v,
                                            tk_atomic_memory_order_t mem_order);

/**
 * @method tk_atomic_fetch_sub_explicit
 * @export none
 * 原子减操作。
 *
 * @param {tk_atomic_t*} atomic 原子操作类对象。
 * @param {value_t*} v 值。
 * @param {tk_atomic_memory_order_t} mem_order 内存顺序。
 *
 * @return {value_t} 返回操作前的旧值。
 */
static value_t tk_atomic_fetch_sub_explicit(tk_atomic_t* atomic, value_t* v,
                                            tk_atomic_memory_order_t mem_order);

#if defined(WIN32) && !defined(MINGW)
#include <winnt.h>

struct _tk_atomic_t {
  volatile value_t value;
};

inline static ret_t tk_atomic_deinit(tk_atomic_t* atomic) {
  return_value_if_fail(atomic != NULL, RET_BAD_PARAMS);
  memset(atomic, 0, sizeof(tk_atomic_t));
  return RET_OK;
}

inline static ret_t tk_atomic_init(tk_atomic_t* atomic, const value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(tk_atomic_support_value_type(v->type), RET_BAD_PARAMS);

  ret = tk_atomic_deinit(atomic);
  return_value_if_fail(RET_OK == ret, ret);

  ret = value_copy((value_t*)(&atomic->value), v);
  return_value_if_fail(RET_OK == ret, ret);

  return ret;
}

inline static void tk_atomic_thread_fence(tk_atomic_memory_order_t mem_order) {
  (void)mem_order;
  return MemoryBarrier();
}

TK_MAYBE_UNUSED static ret_t tk_atomic_exchange_explicit(tk_atomic_t* atomic, value_t* v,
                                                         tk_atomic_memory_order_t mem_order) {
  value_t tmp;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  memset(&tmp, 0, sizeof(value_t));
  tmp.type = atomic->value.type;

  switch (value_type_size(atomic->value.type)) {
    case sizeof(int8_t): {
      tmp.value.i8 = InterlockedExchange8((CHAR*)(&atomic->value.value), (CHAR)(v->value.i8));
    } break;
    case sizeof(int16_t): {
      tmp.value.i16 = InterlockedExchange16((SHORT*)(&atomic->value.value), (SHORT)(v->value.i16));
    } break;
    case sizeof(int32_t): {
      switch (mem_order) {
        case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
          tmp.value.i32 =
              InterlockedExchangeAcquire((LONG*)(&atomic->value.value), (LONG)(v->value.i32));
        } break;
        case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
          tmp.value.i32 =
              InterlockedExchangeNoFence((LONG*)(&atomic->value.value), (LONG)(v->value.i32));
        } break;
        default: {
          tmp.value.i32 = InterlockedExchange((LONG*)(&atomic->value.value), (LONG)(v->value.i32));
        } break;
      }
    } break;
    case sizeof(int64_t): {
      switch (mem_order) {
        case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
          tmp.value.i64 =
              InterlockedExchangeAcquire64((LONG64*)(&atomic->value.value), (LONG64)(v->value.i64));
        } break;
        case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
          tmp.value.i64 =
              InterlockedExchangeNoFence64((LONG64*)(&atomic->value.value), (LONG64)(v->value.i64));
        } break;
        default: {
          tmp.value.i64 =
              InterlockedExchange64((LONG64*)(&atomic->value.value), (LONG64)(v->value.i64));
        } break;
      }
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return value_copy(v, &tmp);
}

inline static bool_t tk_atomic_compare_exchange_weak_explicit(
    tk_atomic_t* atomic, value_t* expect, value_t* desire,
    tk_atomic_memory_order_t success_mem_order, tk_atomic_memory_order_t failure_mem_order) {
  return tk_atomic_compare_exchange_strong_explicit(atomic, expect, desire, success_mem_order,
                                                    failure_mem_order);
}

TK_MAYBE_UNUSED static bool_t tk_atomic_compare_exchange_strong_explicit(
    tk_atomic_t* atomic, value_t* expect, value_t* desire,
    tk_atomic_memory_order_t success_mem_order, tk_atomic_memory_order_t failure_mem_order) {
  bool_t ret = TRUE;
  value_t tmp;
  (void)failure_mem_order;
  return_value_if_fail(atomic != NULL && expect != NULL && desire != NULL, FALSE);

  memset(&tmp, 0, sizeof(value_t));
  tmp.type = atomic->value.type;

  switch (value_type_size(atomic->value.type)) {
    case sizeof(int8_t):
    case sizeof(int16_t): {
      switch (success_mem_order) {
        case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
          tmp.value.i16 = InterlockedCompareExchangeAcquire16((SHORT*)(&atomic->value.value),
                                                              (SHORT)(desire->value.i16),
                                                              (SHORT)(expect->value.i16));
        } break;
        case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
          tmp.value.i16 = InterlockedCompareExchangeRelease16((SHORT*)(&atomic->value.value),
                                                              (SHORT)(desire->value.i16),
                                                              (SHORT)(expect->value.i16));
        } break;
        case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
          tmp.value.i16 = InterlockedCompareExchangeNoFence16((SHORT*)(&atomic->value.value),
                                                              (SHORT)(desire->value.i16),
                                                              (SHORT)(expect->value.i16));
        } break;
        default: {
          tmp.value.i16 =
              InterlockedCompareExchange16((SHORT*)(&atomic->value.value),
                                           (SHORT)(desire->value.i16), (SHORT)(expect->value.i16));
        } break;
      }
    } break;
    case sizeof(int32_t): {
      switch (success_mem_order) {
        case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
          tmp.value.i32 = InterlockedCompareExchangeAcquire(
              (LONG*)(&atomic->value.value), (LONG)(desire->value.i32), (LONG)(expect->value.i32));
        } break;
        case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
          tmp.value.i32 = InterlockedCompareExchangeRelease(
              (LONG*)(&atomic->value.value), (LONG)(desire->value.i32), (LONG)(expect->value.i32));
        } break;
        case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
          tmp.value.i32 = InterlockedCompareExchangeNoFence(
              (LONG*)(&atomic->value.value), (LONG)(desire->value.i32), (LONG)(expect->value.i32));
        } break;
        default: {
          tmp.value.i32 = InterlockedCompareExchange(
              (LONG*)(&atomic->value.value), (LONG)(desire->value.i32), (LONG)(expect->value.i32));
        } break;
      }
    } break;
    case sizeof(int64_t): {
      switch (success_mem_order) {
        case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
          tmp.value.i64 = InterlockedCompareExchangeAcquire64((LONG64*)(&atomic->value.value),
                                                              (LONG64)(desire->value.i64),
                                                              (LONG64)(expect->value.i64));
        } break;
        case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
          tmp.value.i64 = InterlockedCompareExchangeRelease64((LONG64*)(&atomic->value.value),
                                                              (LONG64)(desire->value.i64),
                                                              (LONG64)(expect->value.i64));
        } break;
        case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
          tmp.value.i64 = InterlockedCompareExchangeNoFence64((LONG64*)(&atomic->value.value),
                                                              (LONG64)(desire->value.i64),
                                                              (LONG64)(expect->value.i64));
        } break;
        default: {
          tmp.value.i64 = InterlockedCompareExchange64((LONG64*)(&atomic->value.value),
                                                       (LONG64)(desire->value.i64),
                                                       (LONG64)(expect->value.i64));
        } break;
      }
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

inline static ret_t tk_atomic_store_explicit(tk_atomic_t* atomic, const value_t* v,
                                             tk_atomic_memory_order_t mem_order) {
  value_t tmp;
  (void)mem_order;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  value_copy(&tmp, v);
  return tk_atomic_exchange(atomic, &tmp);
}

TK_MAYBE_UNUSED static ret_t tk_atomic_load_explicit(const tk_atomic_t* atomic, value_t* v,
                                                     tk_atomic_memory_order_t mem_order) {
  (void)mem_order;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  memset(v, 0, sizeof(value_t));
  v->type = atomic->value.type;

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

  return RET_OK;
}

TK_MAYBE_UNUSED static value_t tk_atomic_fetch_add_explicit(tk_atomic_t* atomic, value_t* v,
                                                            tk_atomic_memory_order_t mem_order) {
  value_t ret;
  memset(&ret, 0, sizeof(value_t));
  return_value_if_fail(atomic != NULL && v != NULL, ret);

  ret.type = atomic->value.type;

  switch (value_type_size(atomic->value.type)) {
    case sizeof(int8_t):
    case sizeof(int16_t): {
      if (1 == value_int(v)) {
        switch (mem_order) {
          case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
            ret.value.i16 = InterlockedIncrementAcquire16((SHORT*)(&atomic->value.value));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
            ret.value.i16 = InterlockedIncrementRelease16((SHORT*)(&atomic->value.value));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
            ret.value.i16 = InterlockedIncrementNoFence16((SHORT*)(&atomic->value.value));
          } break;
          default: {
            ret.value.i16 = InterlockedIncrement16((SHORT*)(&atomic->value.value));
          } break;
        }
      } else {
        switch (mem_order) {
          case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
            ret.value.i16 =
                InterlockedAddAcquire((LONG*)(&atomic->value.value), (LONG)(v->value.i16));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
            ret.value.i16 =
                InterlockedAddRelease((LONG*)(&atomic->value.value), (LONG)(v->value.i16));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
            ret.value.i16 =
                InterlockedAddNoFence((LONG*)(&atomic->value.value), (LONG)(v->value.i16));
          } break;
          default: {
            ret.value.i16 = InterlockedAdd((LONG*)(&atomic->value.value), (LONG)(v->value.i16));
          } break;
        }
      }
      /* InterlockedIncrement 和 InterlockedAdd 的返回值都是新值，需要减去增加的值得到旧值 */
      value_sub(&ret, v, &ret);
    } break;
    case sizeof(int32_t): {
      if (1 == value_int(v)) {
        switch (mem_order) {
          case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
            ret.value.i32 = InterlockedIncrementAcquire((LONG*)(&atomic->value.value));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
            ret.value.i32 = InterlockedIncrementRelease((LONG*)(&atomic->value.value));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
            ret.value.i32 = InterlockedIncrementNoFence((LONG*)(&atomic->value.value));
          } break;
          default: {
            ret.value.i32 = InterlockedIncrement((LONG*)(&atomic->value.value));
          } break;
        }
      } else {
        switch (mem_order) {
          case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
            ret.value.i32 =
                InterlockedAddAcquire((LONG*)(&atomic->value.value), (LONG)(v->value.i32));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
            ret.value.i32 =
                InterlockedAddRelease((LONG*)(&atomic->value.value), (LONG)(v->value.i32));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
            ret.value.i32 =
                InterlockedAddNoFence((LONG*)(&atomic->value.value), (LONG)(v->value.i32));
          } break;
          default: {
            ret.value.i32 = InterlockedAdd((LONG*)(&atomic->value.value), (LONG)(v->value.i32));
          } break;
        }
      }
      value_sub(&ret, v, &ret);
    } break;
    case sizeof(int64_t): {
      if (1 == value_int(v)) {
        switch (mem_order) {
          case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
            ret.value.i64 = InterlockedIncrementAcquire64((LONG64*)(&atomic->value.value));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
            ret.value.i64 = InterlockedIncrementRelease64((LONG64*)(&atomic->value.value));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
            ret.value.i64 = InterlockedIncrementNoFence64((LONG64*)(&atomic->value.value));
          } break;
          default: {
            ret.value.i64 = InterlockedIncrement64((LONG64*)(&atomic->value.value));
          } break;
        }
      } else {
        switch (mem_order) {
          case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
            ret.value.i64 =
                InterlockedAddAcquire64((LONG64*)(&atomic->value.value), (LONG64)(v->value.i64));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
            ret.value.i64 =
                InterlockedAddRelease64((LONG64*)(&atomic->value.value), (LONG64)(v->value.i64));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
            ret.value.i64 =
                InterlockedAddNoFence64((LONG64*)(&atomic->value.value), (LONG64)(v->value.i64));
          } break;
          default: {
            ret.value.i64 =
                InterlockedAdd64((LONG64*)(&atomic->value.value), (LONG64)(v->value.i64));
          } break;
        }
      }
      value_sub(&ret, v, &ret);
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
    } break;
  }

  return ret;
}

TK_MAYBE_UNUSED static value_t tk_atomic_fetch_sub_explicit(tk_atomic_t* atomic, value_t* v,
                                                            tk_atomic_memory_order_t mem_order) {
  value_t ret;
  memset(&ret, 0, sizeof(value_t));
  return_value_if_fail(atomic != NULL && v != NULL, ret);

  ret.type = atomic->value.type;

  switch (value_type_size(atomic->value.type)) {
    case sizeof(int8_t):
    case sizeof(int16_t): {
      if (1 == value_int(v)) {
        switch (mem_order) {
          case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
            ret.value.i16 = InterlockedDecrementAcquire16((SHORT*)(&atomic->value.value));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
            ret.value.i16 = InterlockedDecrementRelease16((SHORT*)(&atomic->value.value));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
            ret.value.i16 = InterlockedDecrementNoFence16((SHORT*)(&atomic->value.value));
          } break;
          default: {
            ret.value.i16 = InterlockedDecrement16((SHORT*)(&atomic->value.value));
          } break;
        }
      } else {
        switch (mem_order) {
          case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
            ret.value.i16 =
                InterlockedAddAcquire((LONG*)(&atomic->value.value), -(LONG)(v->value.i16));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
            ret.value.i16 =
                InterlockedAddRelease((LONG*)(&atomic->value.value), -(LONG)(v->value.i16));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
            ret.value.i16 =
                InterlockedAddNoFence((LONG*)(&atomic->value.value), -(LONG)(v->value.i16));
          } break;
          default: {
            ret.value.i16 = InterlockedAdd((LONG*)(&atomic->value.value), -(LONG)(v->value.i16));
          } break;
        }
      }
      /* InterlockedDecrement 和 InterlockedAdd 的返回值都是新值，需要增加减去的值得到旧值 */
      value_add(&ret, v, &ret);
    } break;
    case sizeof(int32_t): {
      if (1 == value_int(v)) {
        switch (mem_order) {
          case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
            ret.value.i32 = InterlockedDecrementAcquire((LONG*)(&atomic->value.value));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
            ret.value.i32 = InterlockedDecrementRelease((LONG*)(&atomic->value.value));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
            ret.value.i32 = InterlockedDecrementNoFence((LONG*)(&atomic->value.value));
          } break;
          default: {
            ret.value.i32 = InterlockedDecrement((LONG*)(&atomic->value.value));
          } break;
        }
      } else {
        switch (mem_order) {
          case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
            ret.value.i32 =
                InterlockedAddAcquire((LONG*)(&atomic->value.value), -(LONG)(v->value.i32));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
            ret.value.i32 =
                InterlockedAddRelease((LONG*)(&atomic->value.value), -(LONG)(v->value.i32));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
            ret.value.i32 =
                InterlockedAddNoFence((LONG*)(&atomic->value.value), -(LONG)(v->value.i32));
          } break;
          default: {
            ret.value.i32 = InterlockedAdd((LONG*)(&atomic->value.value), -(LONG)(v->value.i32));
          } break;
        }
      }
      value_add(&ret, v, &ret);
    } break;
    case sizeof(int64_t): {
      if (1 == value_int(v)) {
        switch (mem_order) {
          case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
            ret.value.i64 = InterlockedDecrementAcquire64((LONG64*)(&atomic->value.value));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
            ret.value.i64 = InterlockedDecrementRelease64((LONG64*)(&atomic->value.value));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
            ret.value.i64 = InterlockedDecrementNoFence64((LONG64*)(&atomic->value.value));
          } break;
          default: {
            ret.value.i64 = InterlockedDecrement64((LONG64*)(&atomic->value.value));
          } break;
        }
      } else {
        switch (mem_order) {
          case TK_ATOMIC_MEMORY_ORDER_ACQUIRE: {
            ret.value.i64 =
                InterlockedAddAcquire64((LONG64*)(&atomic->value.value), -(LONG64)(v->value.i64));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELEASE: {
            ret.value.i64 =
                InterlockedAddRelease64((LONG64*)(&atomic->value.value), -(LONG64)(v->value.i64));
          } break;
          case TK_ATOMIC_MEMORY_ORDER_RELAXED: {
            ret.value.i64 =
                InterlockedAddNoFence64((LONG64*)(&atomic->value.value), -(LONG64)(v->value.i64));
          } break;
          default: {
            ret.value.i64 =
                InterlockedAdd64((LONG64*)(&atomic->value.value), -(LONG64)(v->value.i64));
          } break;
        }
      }
      value_add(&ret, v, &ret);
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
    } break;
  }

  return ret;
}

inline static ret_t tk_atomic_exchange(tk_atomic_t* atomic, value_t* v) {
  return tk_atomic_exchange_explicit(atomic, v, TK_ATOMIC_MEMORY_ORDER_SEQ_CST);
}

inline static bool_t tk_atomic_compare_exchange_weak(tk_atomic_t* atomic, value_t* expect,
                                                     value_t* desire) {
  return tk_atomic_compare_exchange_weak_explicit(
      atomic, expect, desire, TK_ATOMIC_MEMORY_ORDER_SEQ_CST, TK_ATOMIC_MEMORY_ORDER_SEQ_CST);
}

inline static bool_t tk_atomic_compare_exchange_strong(tk_atomic_t* atomic, value_t* expect,
                                                       value_t* desire) {
  return tk_atomic_compare_exchange_strong_explicit(
      atomic, expect, desire, TK_ATOMIC_MEMORY_ORDER_SEQ_CST, TK_ATOMIC_MEMORY_ORDER_SEQ_CST);
}

inline static ret_t tk_atomic_store(tk_atomic_t* atomic, const value_t* v) {
  return tk_atomic_store_explicit(atomic, v, TK_ATOMIC_MEMORY_ORDER_SEQ_CST);
}

inline static ret_t tk_atomic_load(const tk_atomic_t* atomic, value_t* v) {
  return tk_atomic_load_explicit(atomic, v, TK_ATOMIC_MEMORY_ORDER_SEQ_CST);
}

inline static value_t tk_atomic_fetch_add(tk_atomic_t* atomic, value_t* v) {
  return tk_atomic_fetch_add_explicit(atomic, v, TK_ATOMIC_MEMORY_ORDER_SEQ_CST);
}

inline static value_t tk_atomic_fetch_sub(tk_atomic_t* atomic, value_t* v) {
  return tk_atomic_fetch_sub_explicit(atomic, v, TK_ATOMIC_MEMORY_ORDER_SEQ_CST);
}
#elif ((__cplusplus >= 201103L) || (__STDC_VERSION__ >= 201112L)) && !defined(__STDC_NO_ATOMICS__)
#ifdef __cplusplus
#include <atomic>
#define _Atomic(X) std::atomic<X>
#define _Std(X) std::X
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

inline static ret_t tk_atomic_deinit(tk_atomic_t* atomic) {
  return_value_if_fail(atomic != NULL, RET_BAD_PARAMS);
  memset(atomic, 0, sizeof(tk_atomic_t));
  return RET_OK;
}

TK_MAYBE_UNUSED static ret_t tk_atomic_init(tk_atomic_t* atomic, const value_t* v) {
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

TK_MAYBE_UNUSED static ret_t tk_atomic_exchange(tk_atomic_t* atomic, value_t* v) {
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

TK_MAYBE_UNUSED static bool_t tk_atomic_compare_exchange_weak(tk_atomic_t* atomic, value_t* expect,
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

TK_MAYBE_UNUSED static bool_t tk_atomic_compare_exchange_strong(tk_atomic_t* atomic,
                                                                value_t* expect, value_t* desire) {
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

TK_MAYBE_UNUSED static ret_t tk_atomic_store(tk_atomic_t* atomic, const value_t* v) {
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

TK_MAYBE_UNUSED static ret_t tk_atomic_load(const tk_atomic_t* atomic, value_t* v) {
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

TK_MAYBE_UNUSED static value_t tk_atomic_fetch_add(tk_atomic_t* atomic, value_t* v) {
  value_t ret;
  memset(&ret, 0, sizeof(value_t));
  return_value_if_fail(atomic != NULL && v != NULL, ret);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      value_set_bool(&ret, atomic_fetch_add(&atomic->value.b, value_bool(v)));
    } break;
    case VALUE_TYPE_INT8: {
      value_set_int8(&ret, atomic_fetch_add(&atomic->value.i8, value_int8(v)));
    } break;
    case VALUE_TYPE_UINT8: {
      value_set_uint8(&ret, atomic_fetch_add(&atomic->value.u8, value_uint8(v)));
    } break;
    case VALUE_TYPE_INT16: {
      value_set_int16(&ret, atomic_fetch_add(&atomic->value.i16, value_int16(v)));
    } break;
    case VALUE_TYPE_UINT16: {
      value_set_uint16(&ret, atomic_fetch_add(&atomic->value.u16, value_uint16(v)));
    } break;
    case VALUE_TYPE_INT32: {
      value_set_int32(&ret, atomic_fetch_add(&atomic->value.i32, value_int32(v)));
    } break;
    case VALUE_TYPE_UINT32: {
      value_set_uint32(&ret, atomic_fetch_add(&atomic->value.u32, value_uint32(v)));
    } break;
    case VALUE_TYPE_INT64: {
      value_set_int64(&ret, atomic_fetch_add(&atomic->value.i64, value_int64(v)));
    } break;
    case VALUE_TYPE_UINT64: {
      value_set_uint64(&ret, atomic_fetch_add(&atomic->value.u64, value_uint64(v)));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
    } break;
  }

  return ret;
}

TK_MAYBE_UNUSED static value_t tk_atomic_fetch_sub(tk_atomic_t* atomic, value_t* v) {
  value_t ret;
  memset(&ret, 0, sizeof(value_t));
  return_value_if_fail(atomic != NULL && v != NULL, ret);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      value_set_bool(&ret, atomic_fetch_sub(&atomic->value.b, value_bool(v)));
    } break;
    case VALUE_TYPE_INT8: {
      value_set_int8(&ret, atomic_fetch_sub(&atomic->value.i8, value_int8(v)));
    } break;
    case VALUE_TYPE_UINT8: {
      value_set_uint8(&ret, atomic_fetch_sub(&atomic->value.u8, value_uint8(v)));
    } break;
    case VALUE_TYPE_INT16: {
      value_set_int16(&ret, atomic_fetch_sub(&atomic->value.i16, value_int16(v)));
    } break;
    case VALUE_TYPE_UINT16: {
      value_set_uint16(&ret, atomic_fetch_sub(&atomic->value.u16, value_uint16(v)));
    } break;
    case VALUE_TYPE_INT32: {
      value_set_int32(&ret, atomic_fetch_sub(&atomic->value.i32, value_int32(v)));
    } break;
    case VALUE_TYPE_UINT32: {
      value_set_uint32(&ret, atomic_fetch_sub(&atomic->value.u32, value_uint32(v)));
    } break;
    case VALUE_TYPE_INT64: {
      value_set_int64(&ret, atomic_fetch_sub(&atomic->value.i64, value_int64(v)));
    } break;
    case VALUE_TYPE_UINT64: {
      value_set_uint64(&ret, atomic_fetch_sub(&atomic->value.u64, value_uint64(v)));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
    } break;
  }

  return ret;
}

inline static _Std(memory_order) tk_atomic_memory_order_to_std(tk_atomic_memory_order_t mem_order) {
  switch (mem_order) {
    case TK_ATOMIC_MEMORY_ORDER_SEQ_CST:
      return _Std(memory_order_seq_cst);
    case TK_ATOMIC_MEMORY_ORDER_ACQUIRE:
      return _Std(memory_order_acquire);
    case TK_ATOMIC_MEMORY_ORDER_RELEASE:
      return _Std(memory_order_release);
    case TK_ATOMIC_MEMORY_ORDER_ACQ_REL:
      return _Std(memory_order_acq_rel);
    case TK_ATOMIC_MEMORY_ORDER_RELAXED:
      return _Std(memory_order_relaxed);
    default: {
      assert(!"Not support memory order!");
      return _Std(memory_order_seq_cst);
    }
  }
}

inline static void tk_atomic_thread_fence(tk_atomic_memory_order_t mem_order) {
  return atomic_thread_fence(tk_atomic_memory_order_to_std(mem_order));
}

TK_MAYBE_UNUSED static ret_t tk_atomic_exchange_explicit(tk_atomic_t* atomic, value_t* v,
                                                         tk_atomic_memory_order_t mem_order) {
  value_t tmp;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      value_set_bool(&tmp, atomic_exchange_explicit(&atomic->value.b, value_bool(v),
                                                    tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT8: {
      value_set_int8(&tmp, atomic_exchange_explicit(&atomic->value.i8, value_int8(v),
                                                    tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT8: {
      value_set_uint8(&tmp, atomic_exchange_explicit(&atomic->value.u8, value_uint8(v),
                                                     tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT16: {
      value_set_int16(&tmp, atomic_exchange_explicit(&atomic->value.i16, value_int16(v),
                                                     tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT16: {
      value_set_uint16(&tmp, atomic_exchange_explicit(&atomic->value.u16, value_uint16(v),
                                                      tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT32: {
      value_set_int32(&tmp, atomic_exchange_explicit(&atomic->value.i32, value_int32(v),
                                                     tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT32: {
      value_set_uint32(&tmp, atomic_exchange_explicit(&atomic->value.u32, value_uint32(v),
                                                      tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT64: {
      value_set_int64(&tmp, atomic_exchange_explicit(&atomic->value.i64, value_int64(v),
                                                     tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT64: {
      value_set_uint64(&tmp, atomic_exchange_explicit(&atomic->value.u64, value_uint64(v),
                                                      tk_atomic_memory_order_to_std(mem_order)));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return value_copy(v, &tmp);
}

TK_MAYBE_UNUSED static bool_t tk_atomic_compare_exchange_weak_explicit(
    tk_atomic_t* atomic, value_t* expect, value_t* desire,
    tk_atomic_memory_order_t success_mem_order, tk_atomic_memory_order_t failure_mem_order) {
  bool_t ret = TRUE;
  return_value_if_fail(atomic != NULL && expect != NULL && desire != NULL, FALSE);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      bool_t tmp = value_bool(expect);
      ret = atomic_compare_exchange_weak_explicit(&atomic->value.b, &tmp, value_bool(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_bool(expect, tmp);
    } break;
    case VALUE_TYPE_INT8: {
      int8_t tmp = value_int8(expect);
      ret = atomic_compare_exchange_weak_explicit(&atomic->value.i8, &tmp, value_int8(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_int8(expect, tmp);
    } break;
    case VALUE_TYPE_UINT8: {
      uint8_t tmp = value_uint8(expect);
      ret = atomic_compare_exchange_weak_explicit(&atomic->value.u8, &tmp, value_uint8(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_uint8(expect, tmp);
    } break;
    case VALUE_TYPE_INT16: {
      int16_t tmp = value_int16(expect);
      ret = atomic_compare_exchange_weak_explicit(&atomic->value.i16, &tmp, value_int16(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_int16(expect, tmp);
    } break;
    case VALUE_TYPE_UINT16: {
      uint16_t tmp = value_uint16(expect);
      ret = atomic_compare_exchange_weak_explicit(&atomic->value.u16, &tmp, value_uint16(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_uint16(expect, tmp);
    } break;
    case VALUE_TYPE_INT32: {
      int32_t tmp = value_int32(expect);
      ret = atomic_compare_exchange_weak_explicit(&atomic->value.i32, &tmp, value_int32(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_int32(expect, tmp);
    } break;
    case VALUE_TYPE_UINT32: {
      uint32_t tmp = value_uint32(expect);
      ret = atomic_compare_exchange_weak_explicit(&atomic->value.u32, &tmp, value_uint32(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_uint32(expect, tmp);
    } break;
    case VALUE_TYPE_INT64: {
      int64_t tmp = value_int64(expect);
      ret = atomic_compare_exchange_weak_explicit(&atomic->value.i64, &tmp, value_int64(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_int64(expect, tmp);
    } break;
    case VALUE_TYPE_UINT64: {
      uint64_t tmp = value_uint64(expect);
      ret = atomic_compare_exchange_weak_explicit(&atomic->value.u64, &tmp, value_uint64(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
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

TK_MAYBE_UNUSED static bool_t tk_atomic_compare_exchange_strong_explicit(
    tk_atomic_t* atomic, value_t* expect, value_t* desire,
    tk_atomic_memory_order_t success_mem_order, tk_atomic_memory_order_t failure_mem_order) {
  bool_t ret = TRUE;
  return_value_if_fail(atomic != NULL && expect != NULL && desire != NULL, FALSE);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      bool_t tmp = value_bool(expect);
      ret =
          atomic_compare_exchange_strong_explicit(&atomic->value.b, &tmp, value_bool(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_bool(expect, tmp);
    } break;
    case VALUE_TYPE_INT8: {
      int8_t tmp = value_int8(expect);
      ret =
          atomic_compare_exchange_strong_explicit(&atomic->value.i8, &tmp, value_int8(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_int8(expect, tmp);
    } break;
    case VALUE_TYPE_UINT8: {
      uint8_t tmp = value_uint8(expect);
      ret =
          atomic_compare_exchange_strong_explicit(&atomic->value.u8, &tmp, value_uint8(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_uint8(expect, tmp);
    } break;
    case VALUE_TYPE_INT16: {
      int16_t tmp = value_int16(expect);
      ret =
          atomic_compare_exchange_strong_explicit(&atomic->value.i16, &tmp, value_int16(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_int16(expect, tmp);
    } break;
    case VALUE_TYPE_UINT16: {
      uint16_t tmp = value_uint16(expect);
      ret =
          atomic_compare_exchange_strong_explicit(&atomic->value.u16, &tmp, value_uint16(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_uint16(expect, tmp);
    } break;
    case VALUE_TYPE_INT32: {
      int32_t tmp = value_int32(expect);
      ret =
          atomic_compare_exchange_strong_explicit(&atomic->value.i32, &tmp, value_int32(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_int32(expect, tmp);
    } break;
    case VALUE_TYPE_UINT32: {
      uint32_t tmp = value_uint32(expect);
      ret =
          atomic_compare_exchange_strong_explicit(&atomic->value.u32, &tmp, value_uint32(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_uint32(expect, tmp);
    } break;
    case VALUE_TYPE_INT64: {
      int64_t tmp = value_int64(expect);
      ret =
          atomic_compare_exchange_strong_explicit(&atomic->value.i64, &tmp, value_int64(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
      value_set_int64(expect, tmp);
    } break;
    case VALUE_TYPE_UINT64: {
      uint64_t tmp = value_uint64(expect);
      ret =
          atomic_compare_exchange_strong_explicit(&atomic->value.u64, &tmp, value_uint64(desire),
                                                  tk_atomic_memory_order_to_std(success_mem_order),
                                                  tk_atomic_memory_order_to_std(failure_mem_order));
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

TK_MAYBE_UNUSED static ret_t tk_atomic_store_explicit(tk_atomic_t* atomic, const value_t* v,
                                                      tk_atomic_memory_order_t mem_order) {
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      atomic_store_explicit(&atomic->value.b, value_bool(v),
                            tk_atomic_memory_order_to_std(mem_order));
    } break;
    case VALUE_TYPE_INT8: {
      atomic_store_explicit(&atomic->value.i8, value_int8(v),
                            tk_atomic_memory_order_to_std(mem_order));
    } break;
    case VALUE_TYPE_UINT8: {
      atomic_store_explicit(&atomic->value.u8, value_uint8(v),
                            tk_atomic_memory_order_to_std(mem_order));
    } break;
    case VALUE_TYPE_INT16: {
      atomic_store_explicit(&atomic->value.i16, value_int16(v),
                            tk_atomic_memory_order_to_std(mem_order));
    } break;
    case VALUE_TYPE_UINT16: {
      atomic_store_explicit(&atomic->value.u16, value_uint16(v),
                            tk_atomic_memory_order_to_std(mem_order));
    } break;
    case VALUE_TYPE_INT32: {
      atomic_store_explicit(&atomic->value.i32, value_int32(v),
                            tk_atomic_memory_order_to_std(mem_order));
    } break;
    case VALUE_TYPE_UINT32: {
      atomic_store_explicit(&atomic->value.u32, value_uint32(v),
                            tk_atomic_memory_order_to_std(mem_order));
    } break;
    case VALUE_TYPE_INT64: {
      atomic_store_explicit(&atomic->value.i64, value_int64(v),
                            tk_atomic_memory_order_to_std(mem_order));
    } break;
    case VALUE_TYPE_UINT64: {
      atomic_store_explicit(&atomic->value.u64, value_uint64(v),
                            tk_atomic_memory_order_to_std(mem_order));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return RET_OK;
}

TK_MAYBE_UNUSED static ret_t tk_atomic_load_explicit(const tk_atomic_t* atomic, value_t* v,
                                                     tk_atomic_memory_order_t mem_order) {
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      value_set_bool(
          v, atomic_load_explicit(&atomic->value.b, tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT8: {
      value_set_int8(
          v, atomic_load_explicit(&atomic->value.i8, tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT8: {
      value_set_uint8(
          v, atomic_load_explicit(&atomic->value.u8, tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT16: {
      value_set_int16(
          v, atomic_load_explicit(&atomic->value.i16, tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT16: {
      value_set_uint16(
          v, atomic_load_explicit(&atomic->value.u16, tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT32: {
      value_set_int32(
          v, atomic_load_explicit(&atomic->value.i32, tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT32: {
      value_set_uint32(
          v, atomic_load_explicit(&atomic->value.u32, tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT64: {
      value_set_int64(
          v, atomic_load_explicit(&atomic->value.i64, tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT64: {
      value_set_uint64(
          v, atomic_load_explicit(&atomic->value.u64, tk_atomic_memory_order_to_std(mem_order)));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
      return RET_BAD_PARAMS;
    } break;
  }

  return RET_OK;
}

TK_MAYBE_UNUSED static value_t tk_atomic_fetch_add_explicit(tk_atomic_t* atomic, value_t* v,
                                                            tk_atomic_memory_order_t mem_order) {
  value_t ret;
  memset(&ret, 0, sizeof(value_t));
  return_value_if_fail(atomic != NULL && v != NULL, ret);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      value_set_bool(&ret, atomic_fetch_add_explicit(&atomic->value.b, value_bool(v),
                                                     tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT8: {
      value_set_int8(&ret, atomic_fetch_add_explicit(&atomic->value.i8, value_int8(v),
                                                     tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT8: {
      value_set_uint8(&ret, atomic_fetch_add_explicit(&atomic->value.u8, value_uint8(v),
                                                      tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT16: {
      value_set_int16(&ret, atomic_fetch_add_explicit(&atomic->value.i16, value_int16(v),
                                                      tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT16: {
      value_set_uint16(&ret, atomic_fetch_add_explicit(&atomic->value.u16, value_uint16(v),
                                                       tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT32: {
      value_set_int32(&ret, atomic_fetch_add_explicit(&atomic->value.i32, value_int32(v),
                                                      tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT32: {
      value_set_uint32(&ret, atomic_fetch_add_explicit(&atomic->value.u32, value_uint32(v),
                                                       tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT64: {
      value_set_int64(&ret, atomic_fetch_add_explicit(&atomic->value.i64, value_int64(v),
                                                      tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT64: {
      value_set_uint64(&ret, atomic_fetch_add_explicit(&atomic->value.u64, value_uint64(v),
                                                       tk_atomic_memory_order_to_std(mem_order)));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
    } break;
  }

  return ret;
}

TK_MAYBE_UNUSED static value_t tk_atomic_fetch_sub_explicit(tk_atomic_t* atomic, value_t* v,
                                                            tk_atomic_memory_order_t mem_order) {
  value_t ret;
  memset(&ret, 0, sizeof(value_t));
  return_value_if_fail(atomic != NULL && v != NULL, ret);

  switch (atomic->type) {
    case VALUE_TYPE_BOOL: {
      value_set_bool(&ret, atomic_fetch_sub_explicit(&atomic->value.b, value_bool(v),
                                                     tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT8: {
      value_set_int8(&ret, atomic_fetch_sub_explicit(&atomic->value.i8, value_int8(v),
                                                     tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT8: {
      value_set_uint8(&ret, atomic_fetch_sub_explicit(&atomic->value.u8, value_uint8(v),
                                                      tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT16: {
      value_set_int16(&ret, atomic_fetch_sub_explicit(&atomic->value.i16, value_int16(v),
                                                      tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT16: {
      value_set_uint16(&ret, atomic_fetch_sub_explicit(&atomic->value.u16, value_uint16(v),
                                                       tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT32: {
      value_set_int32(&ret, atomic_fetch_sub_explicit(&atomic->value.i32, value_int32(v),
                                                      tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT32: {
      value_set_uint32(&ret, atomic_fetch_sub_explicit(&atomic->value.u32, value_uint32(v),
                                                       tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_INT64: {
      value_set_int64(&ret, atomic_fetch_sub_explicit(&atomic->value.i64, value_int64(v),
                                                      tk_atomic_memory_order_to_std(mem_order)));
    } break;
    case VALUE_TYPE_UINT64: {
      value_set_uint64(&ret, atomic_fetch_sub_explicit(&atomic->value.u64, value_uint64(v),
                                                       tk_atomic_memory_order_to_std(mem_order)));
    } break;
    default: {
      /* tk_atomic_support_value_type() 已经判断过了，正常不可能会跑到这里 */
      assert(!"Not support type!");
    } break;
  }

  return ret;
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

inline static ret_t tk_atomic_deinit(tk_atomic_t* atomic) {
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

inline static ret_t tk_atomic_init(tk_atomic_t* atomic, const value_t* v) {
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

inline static ret_t tk_atomic_exchange(tk_atomic_t* atomic, value_t* v) {
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

inline static bool_t tk_atomic_compare_exchange_weak(tk_atomic_t* atomic, value_t* expect,
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

inline static bool_t tk_atomic_compare_exchange_strong(tk_atomic_t* atomic, value_t* expect,
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

inline static ret_t tk_atomic_store(tk_atomic_t* atomic, const value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  ret = tk_mutex_lock(atomic->lock);
  return_value_if_fail(RET_OK == ret, ret);

  ret = value_copy(&atomic->value, v);

  tk_mutex_unlock(atomic->lock);
  return ret;
}

inline static ret_t tk_atomic_load(const tk_atomic_t* atomic, value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(atomic != NULL && v != NULL, RET_BAD_PARAMS);

  ret = tk_mutex_lock(atomic->lock);
  return_value_if_fail(RET_OK == ret, ret);

  ret = value_copy(v, &atomic->value);

  tk_mutex_unlock(atomic->lock);
  return ret;
}

inline static value_t tk_atomic_fetch_add(tk_atomic_t* atomic, value_t* v) {
  value_t ret;
  memset(&ret, 0, sizeof(value_t));
  return_value_if_fail(atomic != NULL && v != NULL, ret);

  return_value_if_fail(RET_OK == tk_mutex_lock(atomic->lock), ret);

  value_copy(&ret, &atomic->value);
  value_add(&atomic->value, v, &atomic->value);

  tk_mutex_unlock(atomic->lock);

  return ret;
}

inline static value_t tk_atomic_fetch_sub(tk_atomic_t* atomic, value_t* v) {
  value_t ret;
  memset(&ret, 0, sizeof(value_t));
  return_value_if_fail(atomic != NULL && v != NULL, ret);

  return_value_if_fail(RET_OK == tk_mutex_lock(atomic->lock), ret);

  value_copy(&ret, &atomic->value);
  value_sub(&atomic->value, v, &atomic->value);

  tk_mutex_unlock(atomic->lock);

  return ret;
}

inline static void tk_atomic_thread_fence(tk_atomic_memory_order_t mem_order) {
  (void)mem_order;
  static tk_mutex_t* s_dummy = NULL;
  if (NULL == s_dummy) {
    s_dummy = tk_mutex_create();
    return_if_fail(s_dummy != NULL);
  }

  return_if_fail(RET_OK == tk_mutex_lock(s_dummy));
  tk_mutex_unlock(s_dummy);
}

inline static ret_t tk_atomic_exchange_explicit(tk_atomic_t* atomic, value_t* v,
                                                tk_atomic_memory_order_t mem_order) {
  (void)mem_order;
  return tk_atomic_exchange(atomic, v);
}

inline static bool_t tk_atomic_compare_exchange_weak_explicit(
    tk_atomic_t* atomic, value_t* expect, value_t* desire,
    tk_atomic_memory_order_t success_mem_order, tk_atomic_memory_order_t failure_mem_order) {
  (void)success_mem_order, (void)failure_mem_order;
  return tk_atomic_compare_exchange_weak(atomic, expect, desire);
}

inline static bool_t tk_atomic_compare_exchange_strong_explicit(
    tk_atomic_t* atomic, value_t* expect, value_t* desire,
    tk_atomic_memory_order_t success_mem_order, tk_atomic_memory_order_t failure_mem_order) {
  (void)success_mem_order, (void)failure_mem_order;
  return tk_atomic_compare_exchange_strong(atomic, expect, desire);
}

inline static ret_t tk_atomic_store_explicit(tk_atomic_t* atomic, const value_t* v,
                                             tk_atomic_memory_order_t mem_order) {
  (void)mem_order;
  return tk_atomic_store(atomic, v);
}

inline static ret_t tk_atomic_load_explicit(const tk_atomic_t* atomic, value_t* v,
                                            tk_atomic_memory_order_t mem_order) {
  (void)mem_order;
  return tk_atomic_load(atomic, v);
}

inline static value_t tk_atomic_fetch_add_explicit(tk_atomic_t* atomic, value_t* v,
                                                   tk_atomic_memory_order_t mem_order) {
  (void)mem_order;
  return tk_atomic_fetch_add(atomic, v);
}

inline static value_t tk_atomic_fetch_sub_explicit(tk_atomic_t* atomic, value_t* v,
                                                   tk_atomic_memory_order_t mem_order) {
  (void)mem_order;
  return tk_atomic_fetch_sub(atomic, v);
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
inline static ret_t tk_atomic_destroy(tk_atomic_t* atomic) {
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
inline static tk_atomic_t* tk_atomic_create(const value_t* v) {
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
