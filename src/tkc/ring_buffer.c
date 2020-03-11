/**
 * File:   ring_buffer.c
 * Author: AWTK Develop Team
 * Brief:  ring_buffer
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
 * 2019-10-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/ring_buffer.h"

ring_buffer_t* ring_buffer_create(uint32_t init_capacity, uint32_t max_capacity) {
  ring_buffer_t* ring_buffer = NULL;
  return_value_if_fail(init_capacity >= 32, NULL);

  ring_buffer = TKMEM_ZALLOC(ring_buffer_t);
  return_value_if_fail(ring_buffer != NULL, NULL);

  ring_buffer->data = (uint8_t*)TKMEM_ALLOC(init_capacity);
  if (ring_buffer->data != NULL) {
    ring_buffer->capacity = init_capacity;
    memset(ring_buffer->data, 0x00, init_capacity);
    ring_buffer->max_capacity = tk_max(init_capacity, max_capacity);
  } else {
    TKMEM_FREE(ring_buffer);
    ring_buffer = NULL;
  }

  return ring_buffer;
}

bool_t ring_buffer_is_full(ring_buffer_t* ring_buffer) {
  return_value_if_fail(ring_buffer != NULL, FALSE);

  return ring_buffer->full;
}

bool_t ring_buffer_is_empty(ring_buffer_t* ring_buffer) {
  return_value_if_fail(ring_buffer != NULL, FALSE);

  return (ring_buffer->full == FALSE) && (ring_buffer->r == ring_buffer->w);
}

uint32_t ring_buffer_size(ring_buffer_t* ring_buffer) {
  return_value_if_fail(ring_buffer != NULL, 0);

  if (ring_buffer->full) {
    return ring_buffer->capacity;
  } else {
    if (ring_buffer->w >= ring_buffer->r) {
      return ring_buffer->w - ring_buffer->r;
    } else {
      return ring_buffer->capacity - (ring_buffer->r - ring_buffer->w);
    }
  }
}

uint32_t ring_buffer_free_size(ring_buffer_t* ring_buffer) {
  return_value_if_fail(ring_buffer != NULL, 0);

  return ring_buffer->capacity - ring_buffer_size(ring_buffer);
}

uint32_t ring_buffer_capacity(ring_buffer_t* ring_buffer) {
  return_value_if_fail(ring_buffer != NULL, 0);

  return ring_buffer->capacity;
}

ret_t ring_buffer_reset(ring_buffer_t* ring_buffer) {
  return_value_if_fail(ring_buffer != NULL, RET_BAD_PARAMS);

  ring_buffer->full = FALSE;
  ring_buffer->r = ring_buffer->w;
  memset(ring_buffer->data, 0x00, ring_buffer->capacity);

  return RET_OK;
}

ret_t ring_buffer_set_read_cursor(ring_buffer_t* ring_buffer, uint32_t r) {
  ring_buffer->r = r % ring_buffer->capacity;
  ring_buffer->full = FALSE;

  return RET_OK;
}

ret_t ring_buffer_set_read_cursor_delta(ring_buffer_t* ring_buffer, uint32_t r_delta) {
  return ring_buffer_set_read_cursor(ring_buffer, ring_buffer->r + r_delta);
}

ret_t ring_buffer_set_write_cursor(ring_buffer_t* ring_buffer, uint32_t w) {
  ring_buffer->w = w % ring_buffer->capacity;
  if (ring_buffer->r == ring_buffer->w) {
    ring_buffer->full = TRUE;
  }

  return RET_OK;
}

ret_t ring_buffer_set_write_cursor_delta(ring_buffer_t* ring_buffer, uint32_t w_delta) {
  return ring_buffer_set_write_cursor(ring_buffer, ring_buffer->w + w_delta);
}

uint32_t ring_buffer_read(ring_buffer_t* ring_buffer, void* buff, uint32_t size) {
  return_value_if_fail(ring_buffer != NULL && buff != NULL, 0);

  if (size == 0) {
    return 0;
  }

  if (ring_buffer->r != ring_buffer->w || ring_buffer->full) {
    uint32_t ret = 0;
    uint32_t rsize = 0;
    uint8_t* d = (uint8_t*)buff;
    uint8_t* s = ring_buffer->data + ring_buffer->r;

    ring_buffer->full = FALSE;
    if (ring_buffer->r < ring_buffer->w) {
      rsize = ring_buffer->w - ring_buffer->r;
      rsize = tk_min(rsize, size);

      ret = rsize;
      memcpy(d, s, rsize);
      ring_buffer_set_read_cursor_delta(ring_buffer, rsize);
    } else {
      rsize = ring_buffer->capacity - ring_buffer->r;
      rsize = tk_min(rsize, size);

      ret = rsize;
      memcpy(d, s, rsize);
      ring_buffer_set_read_cursor_delta(ring_buffer, rsize);

      if (rsize < size) {
        size -= rsize;
        d += rsize;
        s = ring_buffer->data;

        rsize = tk_min(ring_buffer->w, size);
        if (rsize > 0) {
          memcpy(d, s, rsize);
          ret += rsize;
        }

        ring_buffer_set_read_cursor(ring_buffer, rsize);
      }
    }

    return ret;
  }

  return 0;
}

uint32_t ring_buffer_peek(ring_buffer_t* ring_buffer, void* buff, uint32_t size) {
  uint32_t r = 0;
  uint32_t ret = 0;
  bool_t full = FALSE;
  return_value_if_fail(ring_buffer != NULL && buff != NULL, 0);

  r = ring_buffer->r;
  full = ring_buffer->full;
  ret = ring_buffer_read(ring_buffer, buff, size);
  ring_buffer->r = r;
  ring_buffer->full = full;

  return ret;
}

uint32_t ring_buffer_write(ring_buffer_t* ring_buffer, const void* buff, uint32_t size) {
  return_value_if_fail(ring_buffer != NULL && buff != NULL, 0);

  ring_buffer_ensure_write_space(ring_buffer, size);
  if (size == 0 || ring_buffer_free_size(ring_buffer) == 0) {
    return 0;
  }

  if (ring_buffer->r != ring_buffer->w || !ring_buffer->full) {
    uint32_t ret = 0;
    uint32_t rsize = 0;
    uint8_t* s = (uint8_t*)buff;
    uint8_t* d = ring_buffer->data + ring_buffer->w;

    if (ring_buffer->w < ring_buffer->r) {
      rsize = ring_buffer->r - ring_buffer->w;
      rsize = tk_min(rsize, size);

      ret = rsize;
      memcpy(d, s, rsize);
      ring_buffer_set_write_cursor_delta(ring_buffer, rsize);
    } else {
      rsize = ring_buffer->capacity - ring_buffer->w;
      rsize = tk_min(rsize, size);

      ret = rsize;
      memcpy(d, s, rsize);
      ring_buffer_set_write_cursor_delta(ring_buffer, rsize);

      if (rsize < size) {
        size -= rsize;
        s += rsize;
        d = ring_buffer->data;

        rsize = tk_min(ring_buffer->r, size);
        if (rsize > 0) {
          memcpy(d, s, rsize);
          ret += rsize;
        }

        ring_buffer_set_write_cursor(ring_buffer, rsize);
      }
    }

    return ret;
  }

  return 0;
}

ret_t ring_buffer_read_len(ring_buffer_t* ring_buffer, void* buff, uint32_t size) {
  return_value_if_fail(ring_buffer != NULL && buff != NULL, RET_BAD_PARAMS);

  if (ring_buffer_size(ring_buffer) >= size) {
    return ring_buffer_read(ring_buffer, buff, size) == size ? RET_OK : RET_FAIL;
  } else {
    return RET_FAIL;
  }
}

ret_t ring_buffer_skip(ring_buffer_t* ring_buffer, uint32_t size) {
  return_value_if_fail(ring_buffer != NULL, RET_BAD_PARAMS);

  if (ring_buffer_size(ring_buffer) >= size) {
    ring_buffer->r = (ring_buffer->r + size) % ring_buffer->capacity;

    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

ret_t ring_buffer_ensure_write_space(ring_buffer_t* ring_buffer, uint32_t size) {
  uint32_t free_size = ring_buffer_free_size(ring_buffer);
  if (free_size >= size) {
    return RET_OK;
  } else if (ring_buffer->capacity == ring_buffer->max_capacity) {
    return RET_FAIL;
  } else {
    uint8_t* data = NULL;
    uint32_t old_size = ring_buffer_size(ring_buffer);
    uint32_t capacity = ring_buffer->capacity + (size - free_size);
    return_value_if_fail(capacity <= ring_buffer->max_capacity, RET_FAIL);

    data = (uint8_t*)TKMEM_ALLOC(capacity);
    return_value_if_fail(data != NULL, RET_OOM);
    return_value_if_fail(ring_buffer_read_len(ring_buffer, data, old_size) == RET_OK, RET_FAIL);

    TKMEM_FREE(ring_buffer->data);
    ring_buffer->r = 0;
    ring_buffer->w = old_size;
    ring_buffer->data = data;
    ring_buffer->capacity = capacity;

    return RET_OK;
  }
}

ret_t ring_buffer_write_len(ring_buffer_t* ring_buffer, const void* buff, uint32_t size) {
  return_value_if_fail(ring_buffer != NULL && buff != NULL, RET_BAD_PARAMS);

  if (ring_buffer_ensure_write_space(ring_buffer, size) == RET_OK) {
    return ring_buffer_write(ring_buffer, buff, size) == size ? RET_OK : RET_FAIL;
  } else {
    return RET_FAIL;
  }
}

ret_t ring_buffer_destroy(ring_buffer_t* ring_buffer) {
  return_value_if_fail(ring_buffer != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(ring_buffer->data);

  memset(ring_buffer, 0x00, sizeof(ring_buffer_t));
  TKMEM_FREE(ring_buffer);

  return RET_OK;
}
