/**
 * File:   lf_bp_buffer.h
 * Author: AWTK Develop Team
 * Brief:  Lock Free Bipartite Buffer
 *
 * Copyright (c) 2025 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-12-2 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 * adapted from: https://github.com/DNedic/lfbb
 */

/**************************************************************
 * Copyright (c) 2022 - 2026 Djordje Nedic
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
 * KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of LFBB - Lock Free Bipartite Buffer
 *
 * Author:          Djordje Nedic <nedic.djordje2@gmail.com>
 * Version:         1.3.7
 **************************************************************/

#ifndef TK_LF_BP_BUFFER_H
#define TK_LF_BP_BUFFER_H

#include "tkc/atomic.h"
#include "tkc/mem.h"

#ifdef _MSC_VER
#define TK_ALIGN(v, len) __declspec(align(len)) v
#else
#define TK_ALIGN(v, len) v __attribute__((aligned(len)))
#endif /*_MSC_VER*/

#ifndef TK_LF_BP_BUFFER_MULTICORE_HOSTED
#define TK_LF_BP_BUFFER_MULTICORE_HOSTED TK_IS_PC
#endif /*TK_LF_BP_BUFFER_MULTICORE_HOSTED*/

#ifndef TK_LF_BP_BUFFER_CACHELINE_LENGTH
#define TK_LF_BP_BUFFER_CACHELINE_LENGTH 64
#endif /*TK_LF_BP_BUFFER_CACHELINE_LENGTH*/

/**
 * @class tk_lf_bp_buffer_t
 * @export none
 * 无锁双分区缓冲区。
 * > 只适用于单生产者-单消费者（SPSC）场景。
 */
typedef struct _tk_lf_bp_buffer_t {
  /**
   * 避免伪共享：确保生产者和消费者线程的状态变量在不同缓存行中。
   * 在多核 CPU 上，当多个线程频繁修改同一缓存行（cache line）中的不同变量时，会导致以下问题：
   * 缓存一致性协议开销：CPU 需要不断同步缓存行，导致额外的总线流量
   * 性能下降：即使线程修改的是不同变量，也会因为缓存行失效而降低性能
   */
#if TK_LF_BP_BUFFER_MULTICORE_HOSTED
  TK_ALIGN(tk_atomic_t r, TK_LF_BP_BUFFER_CACHELINE_LENGTH);
  TK_ALIGN(tk_atomic_t w, TK_LF_BP_BUFFER_CACHELINE_LENGTH);
  TK_ALIGN(tk_atomic_t i, TK_LF_BP_BUFFER_CACHELINE_LENGTH);
#else
  /** @property {tk_atomic_t} r
   * @annotation ["readable"]
   * 读取索引。
   */
  tk_atomic_t r;
  /** @property {tk_atomic_t} i
   * @annotation ["readable"]
   * 写入索引。
   */
  tk_atomic_t w;
  /** @property {tk_atomic_t} i
   * @annotation ["readable"]
   * 无效空间索引。
   */
  tk_atomic_t i;
#endif /*TK_LF_BP_BUFFER_MULTICORE_HOSTED*/

  /** 
   * @property {bool_t} write_wrapped
   * @annotation ["readable"]
   * 写环绕标志，仅在生产者中使用。
   */
  bool_t write_wrapped;

  /** 
   * @property {bool_t} read_wrapped
   * @annotation ["readable"]
   * 读环绕标志，仅在消费者中使用。
   */
  bool_t read_wrapped;

  /**
   * @property {uint64_t} size
   * @annotation ["readable"]
   * 缓冲区的大小。
   */
  uint64_t size;

  /**
   * @property {uint8_t*} data
   * @annotation ["readable"]
   * 缓冲区数据指针。
   */
  uint8_t* data;
} tk_lf_bp_buffer_t;

/**
 * @method tk_lf_bp_buffer_deinit
 * @export none
 * 释放缓冲区。
 *
 * @param {tk_lf_bp_buffer_t*} buffer 缓冲区对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
inline static ret_t tk_lf_bp_buffer_deinit(tk_lf_bp_buffer_t* buffer) {
  return_value_if_fail(buffer != NULL, RET_BAD_PARAMS);

  buffer->size = 0;
  buffer->data = NULL;

  tk_atomic_deinit(&buffer->r);
  tk_atomic_deinit(&buffer->w);
  tk_atomic_deinit(&buffer->i);

  return RET_OK;
}

/**
 * @method tk_lf_bp_buffer_destroy
 * @annotation ["deconstructor"]
 * @export none
 * 销毁缓冲区。
 *
 * @param {tk_lf_bp_buffer_t*} buffer 缓冲区对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
inline static ret_t tk_lf_bp_buffer_destroy(tk_lf_bp_buffer_t* buffer) {
  ret_t ret = RET_OK;
  return_value_if_fail(buffer != NULL, RET_BAD_PARAMS);

  ret = tk_lf_bp_buffer_deinit(buffer);
  return_value_if_fail(RET_OK == ret, ret);

  TKMEM_FREE(buffer);

  return ret;
}

/**
 * @method tk_lf_bp_buffer_init
 * @export none
 * 初始化缓冲区。
 *
 * @param {tk_lf_bp_buffer_t*} buffer 缓冲区对象。
 * @param {uint8_t*} data 缓冲区数据指针。
 * @param {uint64_t} size 缓冲区大小。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
inline static ret_t tk_lf_bp_buffer_init(tk_lf_bp_buffer_t* buffer, uint8_t* data, uint64_t size) {
  ret_t ret = RET_OK;
  value_t v;
  return_value_if_fail(buffer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(data != NULL && size > 0, RET_BAD_PARAMS);

  memset(buffer, 0, sizeof(tk_lf_bp_buffer_t));

  value_set_uint64(&v, 0);

  ret = tk_atomic_init(&buffer->r, &v);
  return_value_if_fail(RET_OK == ret, ret);

  ret = tk_atomic_init(&buffer->w, &v);
  return_value_if_fail(RET_OK == ret, ret);

  ret = tk_atomic_init(&buffer->i, &v);
  return_value_if_fail(RET_OK == ret, ret);

  buffer->data = data;
  buffer->size = size;

  return ret;
}

/**
 * @method tk_lf_bp_buffer_create
 * @annotation ["constructor"]
 * @export none
 * 创建缓冲区。
 *
 * @param {uint8_t*} data 缓冲区数据指针。
 * @param {uint64_t} size 缓冲区大小。
 *
 * @return {tk_lf_bp_buffer_t*} 返回缓冲区对象。
 */
inline static tk_lf_bp_buffer_t* tk_lf_bp_buffer_create(uint8_t* data, uint64_t size) {
  tk_lf_bp_buffer_t* ret = NULL;
  return_value_if_fail(data != NULL && size > 0, NULL);

  ret = (tk_lf_bp_buffer_t*)TKMEM_ALLOC(sizeof(tk_lf_bp_buffer_t));
  return_value_if_fail(ret != NULL, NULL);

  goto_error_if_fail(RET_OK == tk_lf_bp_buffer_init(ret, data, size));

  return ret;
error:
  tk_lf_bp_buffer_destroy(ret);
  return NULL;
}

/**
 * @method tk_lf_bp_buffer_write_acquire
 * @export none
 * 申请写入缓冲区。
 *
 * @param {tk_lf_bp_buffer_t*} buffer 缓冲区对象。
 * @param {uint64_t*} free_required 所需的空闲空间大小，并返回实际申请写入缓冲区的大小。
 * @param {bool_t} accept_smaller 是否接受小于所需大小的空间。
 *
 * @return {uint8_t*} 返回写入缓冲区的指针。
 */
inline static uint8_t* tk_lf_bp_buffer_write_acquire(tk_lf_bp_buffer_t* buffer,
                                                     uint64_t* free_required,
                                                     bool_t accept_smaller) {
  uint64_t w;
  uint64_t r;
  uint64_t free;
  uint64_t linear_free;
  value_t v;
  return_value_if_fail(buffer != NULL && buffer->data != NULL, NULL);
  return_value_if_fail(free_required != NULL, NULL);
  return_value_if_fail(0 < *free_required && (accept_smaller || *free_required <= buffer->size),
                       NULL);

  /**
   * eg:
   * free_required = 3
   * □□□▨■■□□
   *     ↑ ↑
   *     r w
   * free = 5
   * linear_free = 2
   * □□□▨■■□□
   * ↑   ↑ ↑
   * ret r w
   */

  tk_atomic_load_explicit(&buffer->w, &v, TK_ATOMIC_MEMORY_ORDER_RELAXED);
  w = value_uint64(&v);

  tk_atomic_load_explicit(&buffer->r, &v, TK_ATOMIC_MEMORY_ORDER_ACQUIRE);
  r = value_uint64(&v);

  if (r > w) {
    free = (r - w) - 1;
  } else {
    free = (buffer->size - (w - r)) - 1;
  }

  linear_free = tk_min(free, buffer->size - w);

  if (*free_required <= linear_free) {
    return &buffer->data[w];
  } else if (*free_required <= free - linear_free) {
    buffer->write_wrapped = TRUE;
    return &buffer->data[0];
  }

  if (accept_smaller) {
    uint64_t available = 0;
    bool_t wrapped = FALSE;

    if (linear_free >= free - linear_free) {
      available = linear_free;
    } else {
      available = free - linear_free;
      wrapped = TRUE;
    }

    if (available > 0) {
      *free_required = available;

      buffer->write_wrapped = wrapped;
      if (!wrapped) {
        return &buffer->data[w];
      } else {
        return &buffer->data[0];
      }
    }
  }

  return NULL;
}

/**
 * @method tk_lf_bp_buffer_write_release
 * @export none
 * 释放写入缓冲区。
 *
 * @param {tk_lf_bp_buffer_t*} buffer 缓冲区对象。
 * @param {uint64_t} written 写入的字节数。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
inline static ret_t tk_lf_bp_buffer_write_release(tk_lf_bp_buffer_t* buffer, uint64_t written) {
  uint64_t w;
  uint64_t i;
  value_t v;
  return_value_if_fail(buffer != NULL && buffer->data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(0 < written && written <= buffer->size, RET_BAD_PARAMS);

  /**
   * eg:
   * written = 3
   * ◪◪◪▨■■□□
   *     ↑ ↑
   *     r w
   * ◪◪◪▨■■□□
   * ↑   ↑ ↑
   * w   r i
   * ◪◪◪▨■■□□
   *    ↑↑ ↑
   *    wr i
   */

  tk_atomic_load_explicit(&buffer->w, &v, TK_ATOMIC_MEMORY_ORDER_RELAXED);
  w = value_uint64(&v);

  if (buffer->write_wrapped) {
    buffer->write_wrapped = FALSE;
    i = w;
    w = 0;
  } else {
    tk_atomic_load_explicit(&buffer->i, &v, TK_ATOMIC_MEMORY_ORDER_RELAXED);
    i = value_uint64(&v);
  }

  w += written;
  ENSURE(w <= buffer->size);
  return_value_if_fail(w <= buffer->size, RET_FAIL);

  if (w > i) {
    i = w;
  }

  if (w == buffer->size) {
    w = 0;
  }

  tk_atomic_store_explicit(&buffer->i, value_set_uint64(&v, i), TK_ATOMIC_MEMORY_ORDER_RELAXED);
  tk_atomic_store_explicit(&buffer->w, value_set_uint64(&v, w), TK_ATOMIC_MEMORY_ORDER_RELEASE);

  return RET_OK;
}

/**
 * @method tk_lf_bp_buffer_read_acquire
 * @export none
 * 申请读取缓冲区。
 *
 * @param {tk_lf_bp_buffer_t*} buffer 缓冲区对象。
 * @param {uint64_t*} available 返回可用的字节数。
 *
 * @return {uint8_t*} 返回读取缓冲区的指针。
 */
inline static uint8_t* tk_lf_bp_buffer_read_acquire(tk_lf_bp_buffer_t* buffer,
                                                    uint64_t* available) {
  uint64_t w;
  uint64_t r;
  uint64_t i;
  value_t v;
  return_value_if_fail(buffer != NULL && buffer->data != NULL, NULL);
  return_value_if_fail(available != NULL, NULL);

  /**
   * eg:
   * ◪◪◪▨■■□□
   *    ↑↑ ↑
   *    wr i
   *     ↑
   *     ret
   * available = 2
   */

  tk_atomic_load_explicit(&buffer->r, &v, TK_ATOMIC_MEMORY_ORDER_RELAXED);
  r = value_uint64(&v);

  tk_atomic_load_explicit(&buffer->w, &v, TK_ATOMIC_MEMORY_ORDER_ACQUIRE);
  w = value_uint64(&v);

  if (r == w) {
    *available = 0;
    return NULL;
  } else if (r < w) {
    *available = w - r;
    return &buffer->data[r];
  }

  tk_atomic_load_explicit(&buffer->i, &v, TK_ATOMIC_MEMORY_ORDER_RELAXED);
  i = value_uint64(&v);

  if (r == i) {
    buffer->read_wrapped = TRUE;
    *available = w;
    return &buffer->data[0];
  }

  *available = i - r;
  return &buffer->data[r];
}

/**
 * @method tk_lf_bp_buffer_read_release
 * @export none
 * 释放读取缓冲区。
 *
 * @param {tk_lf_bp_buffer_t*} buffer 缓冲区对象。
 * @param {uint64_t} read 读取的字节数。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
inline static ret_t tk_lf_bp_buffer_read_release(tk_lf_bp_buffer_t* buffer, uint64_t read) {
  uint64_t r;
  value_t v;
  return_value_if_fail(buffer != NULL && buffer->data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(0 < read && read <= buffer->size, RET_BAD_PARAMS);

  /**
   * eg:
   * read = 2
   * ◪◪◪▨■■□□
   *     ↑ ↑
   *     r i
   * ◪◪◪□□▨□□
   *    ↑  ↑
   *    w  r=i
   * ■■■□□□□▨
   * ↑  ↑  ↑
   * r  w  i
   */

  if (buffer->read_wrapped) {
    buffer->read_wrapped = FALSE;
    r = 0;
  } else {
    tk_atomic_load_explicit(&buffer->r, &v, TK_ATOMIC_MEMORY_ORDER_RELAXED);
    r = value_uint64(&v);
  }

  r += read;
  ENSURE(r <= buffer->size);
  if (r == buffer->size) {
    r = 0;
  }

  tk_atomic_store_explicit(&buffer->r, value_set_uint64(&v, r), TK_ATOMIC_MEMORY_ORDER_RELEASE);

  return RET_OK;
}

/**
 * @method tk_lf_bp_buffer_write_simple
 * @export none
 * 写入缓冲区简化实现。
 *
 * @param {tk_lf_bp_buffer_t*} buffer 缓冲区对象。
 * @param {const uint8_t*} data 写入数据。
 * @param {uint64_t*} size 写入数据大小并返回实际写入的字节数。
 * @param {bool_t} accept_smaller 是否接受小于所需大小的空间。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
inline static ret_t tk_lf_bp_buffer_write_simple(tk_lf_bp_buffer_t* buffer, const uint8_t* data,
                                                 uint64_t* size, bool_t accept_smaller) {
  uint8_t* write_ptr;
  return_value_if_fail(buffer != NULL && buffer->data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(data != NULL, RET_BAD_PARAMS);

  write_ptr = tk_lf_bp_buffer_write_acquire(buffer, size, accept_smaller);
  if (NULL == write_ptr) {
    return RET_FAIL;
  }

  memcpy(write_ptr, data, *size);

  return tk_lf_bp_buffer_write_release(buffer, *size);
}

/**
 * @method tk_lf_bp_buffer_read_simple
 * @export none
 * 读取缓冲区简化实现。
 *
 * @param {tk_lf_bp_buffer_t*} buffer 缓冲区对象。
 * @param {uint8_t*} data 读取数据。
 * @param {uint64_t*} size 读取数据大小并返回实际读取的字节数。
 * @param {bool_t} accept_smaller 是否接受小于所需大小的空间。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
inline static ret_t tk_lf_bp_buffer_read_simple(tk_lf_bp_buffer_t* buffer, uint8_t* data,
                                                uint64_t* size, bool_t accept_smaller) {
  uint8_t* read_ptr;
  uint64_t available = 0;
  return_value_if_fail(buffer != NULL && buffer->data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(data != NULL, RET_BAD_PARAMS);

  read_ptr = tk_lf_bp_buffer_read_acquire(buffer, &available);
  if (NULL == read_ptr) {
    return RET_FAIL;
  }

  if (!accept_smaller && available < *size) {
    return RET_FAIL;
  } else {
    *size = available;
  }

  memcpy(data, read_ptr, *size);

  return tk_lf_bp_buffer_read_release(buffer, *size);
}

#endif /*TK_LF_BP_BUFFER_H*/
