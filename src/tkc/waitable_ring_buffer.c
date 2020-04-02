/**
 * File:   waitable_ring_buffer.c
 * Author: AWTK Develop Team
 * Brief:  waitable_ring_buffer
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-04-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/waitable_ring_buffer.h"

waitable_ring_buffer_t* waitable_ring_buffer_create(uint32_t capacity, uint32_t block_size) {
  waitable_ring_buffer_t* rb = TKMEM_ZALLOC(waitable_ring_buffer_t);
  return_value_if_fail(rb != NULL, NULL);
  rb->ring_buffer = ring_buffer_create(capacity * block_size, capacity * block_size);
  goto_error_if_fail(rb->ring_buffer != NULL);

  rb->block_size = block_size;
  rb->mutex = tk_mutex_create();
  goto_error_if_fail(rb->mutex != NULL);

  rb->sema_read = tk_semaphore_create(0, NULL);
  goto_error_if_fail(rb->sema_read != NULL);

  rb->sema_write = tk_semaphore_create(capacity, NULL);
  goto_error_if_fail(rb->sema_write != NULL);

  return rb;
error:
  if (rb != NULL) {
    if (rb->ring_buffer != NULL) {
      ring_buffer_destroy(rb->ring_buffer);
    }

    if (rb->mutex != NULL) {
      tk_mutex_destroy(rb->mutex);
    }

    if (rb->sema_read != NULL) {
      tk_semaphore_destroy(rb->sema_read);
    }

    if (rb->sema_write != NULL) {
      tk_semaphore_destroy(rb->sema_write);
    }

    TKMEM_FREE(rb);
  }

  return NULL;
}

ret_t waitable_ring_buffer_read(waitable_ring_buffer_t* rb, void* buff, uint32_t size,
                                uint32_t timeout_ms) {
  ret_t ret = RET_FAIL;
  return_value_if_fail(rb != NULL && buff != NULL, RET_BAD_PARAMS);
  return_value_if_fail(size == rb->block_size, RET_BAD_PARAMS);

  if (tk_semaphore_wait(rb->sema_read, timeout_ms) == RET_OK) {
    if (tk_mutex_lock(rb->mutex) == RET_OK) {
      ret = ring_buffer_read_len(rb->ring_buffer, buff, size);
      if (ret == RET_OK) {
        ENSURE(tk_semaphore_post(rb->sema_write) == RET_OK);
      }
      ENSURE(tk_mutex_unlock(rb->mutex) == RET_OK);
    }
  }

  return ret;
}

ret_t waitable_ring_buffer_write(waitable_ring_buffer_t* rb, const void* buff, uint32_t size,
                                 uint32_t timeout_ms) {
  ret_t ret = RET_FAIL;
  return_value_if_fail(rb != NULL && buff != NULL, RET_BAD_PARAMS);
  return_value_if_fail(size == rb->block_size, RET_BAD_PARAMS);

  if (tk_semaphore_wait(rb->sema_write, timeout_ms) == RET_OK) {
    if (tk_mutex_lock(rb->mutex) == RET_OK) {
      ret = ring_buffer_write_len(rb->ring_buffer, buff, size);
      assert(ret == RET_OK);
      ENSURE(tk_semaphore_post(rb->sema_read) == RET_OK);
      ENSURE(tk_mutex_unlock(rb->mutex) == RET_OK);
    }
  }

  return ret;
}

ret_t waitable_ring_buffer_destroy(waitable_ring_buffer_t* rb) {
  return_value_if_fail(rb != NULL, RET_BAD_PARAMS);

  ENSURE(ring_buffer_destroy(rb->ring_buffer) == RET_OK);
  ENSURE(tk_mutex_destroy(rb->mutex) == RET_OK);
  ENSURE(tk_semaphore_destroy(rb->sema_read) == RET_OK);
  ENSURE(tk_semaphore_destroy(rb->sema_write) == RET_OK);
  TKMEM_FREE(rb);

  return RET_OK;
}
