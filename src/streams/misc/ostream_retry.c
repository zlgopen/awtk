/**
 * File:   ostream_retry.c
 * Author: AWTK Develop Team
 * Brief:  retry ostream: if write fail retry write later.
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
 * 2019-10-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/buffer.h"
#include "streams/misc/ostream_retry.h"

static ret_t tk_ostream_retry_write_to_buffer(tk_ostream_retry_t* ostream_retry,
                                              const uint8_t* buff, uint32_t max_size) {
  ring_buffer_t* rb = ostream_retry->rb;
  uint32_t size = max_size + sizeof(max_size);
  data_discard_policy_t policy = ostream_retry->discard_policy;

  if (ring_buffer_ensure_write_space(rb, size) != RET_OK) {
    if (policy == DATA_DISCARD_NEW) {
      ostream_retry->discard_packets++;
      ostream_retry->discard_bytes += max_size;
      return RET_OK;
    } else {
      tk_ostream_retry_clear_buffer(TK_OSTREAM(ostream_retry));
    }
  }

  return_value_if_fail(ring_buffer_write_len(rb, &max_size, sizeof(max_size)) == RET_OK, RET_FAIL);
  return_value_if_fail(ring_buffer_write_len(rb, buff, max_size) == RET_OK, RET_FAIL);
  ostream_retry->pending_bytes += max_size;
  ostream_retry->pending_packets++;

  return RET_OK;
}

static int32_t tk_ostream_retry_write(tk_ostream_t* stream, const uint8_t* buff,
                                      uint32_t max_size) {
  tk_ostream_retry_t* ostream_retry = TK_OSTREAM_RETRY(stream);
  tk_ostream_t* ostream = ostream_retry->real_ostream;
  uint32_t timeout = ostream_retry->timeout;
  ring_buffer_t* rb = ostream_retry->rb;

  if (ring_buffer_size(rb) > 0) {
    tk_ostream_flush(stream);
  }

  if (ring_buffer_size(rb) > 0) {
    tk_ostream_retry_write_to_buffer(ostream_retry, buff, max_size);
    tk_ostream_flush(stream);
  } else {
    int32_t ret = tk_ostream_write_len(ostream, buff, max_size, timeout);
    if (ret <= 0) {
      tk_ostream_retry_write_to_buffer(ostream_retry, buff, max_size);
    } else if (ret < max_size) {
      tk_ostream_retry_write_to_buffer(ostream_retry, buff + ret, max_size - ret);
    }
  }

  return max_size;
}

static ret_t tk_ostream_retry_flush(tk_ostream_t* stream) {
  uint32_t size = 0;
  tk_ostream_retry_t* ostream_retry = TK_OSTREAM_RETRY(stream);
  uint32_t timeout = ostream_retry->timeout;
  ring_buffer_t* rb = ostream_retry->rb;

  if (!ring_buffer_is_empty(rb)) {
    wbuffer_t* wb = &(ostream_retry->wb);
    tk_ostream_t* ostream = ostream_retry->real_ostream;

    do {
      uint32_t packet_size = 0;
      ostream_retry->retried_times++;
      if (ring_buffer_peek(rb, &size, sizeof(size)) != sizeof(size)) {
        break;
      }
      wb->cursor = 0;
      packet_size = size + sizeof(size);

      return_value_if_fail(wbuffer_extend_capacity(wb, packet_size) == RET_OK, RET_OOM);
      if (ring_buffer_peek(rb, wb->data, packet_size) != packet_size) {
        break;
      }

      if (tk_ostream_write_len(ostream, wb->data + sizeof(size), size, timeout) == size) {
        ostream_retry->retried_times = 0;
        ostream_retry->pending_packets--;
        ostream_retry->pending_bytes -= size;
        ring_buffer_skip(rb, packet_size);
      } else {
        if (ostream_retry->retried_times == ostream_retry->max_retry_times) {
          tk_ostream_retry_clear_buffer(stream);
        }
        break;
      }
    } while (TRUE);
  }

  return RET_OK;
}

static ret_t tk_ostream_retry_set_prop(object_t* obj, const char* name, const value_t* v) {
  tk_ostream_retry_t* ostream_retry = TK_OSTREAM_RETRY(obj);
  tk_ostream_t* real_ostream = ostream_retry->real_ostream;

  return object_set_prop(OBJECT(real_ostream), name, v);
}

static ret_t tk_ostream_retry_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_ostream_retry_t* ostream_retry = TK_OSTREAM_RETRY(obj);
  tk_ostream_t* real_ostream = ostream_retry->real_ostream;

  return object_get_prop(OBJECT(real_ostream), name, v);
}

static ret_t tk_ostream_retry_on_destroy(object_t* obj) {
  tk_ostream_retry_t* ostream_retry = TK_OSTREAM_RETRY(obj);

  wbuffer_deinit(&(ostream_retry->wb));
  ring_buffer_destroy(ostream_retry->rb);
  OBJECT_UNREF(ostream_retry->real_ostream);

  return RET_OK;
}

static const object_vtable_t s_tk_ostream_retry_vtable = {.type = "tk_ostream_retry",
                                                          .desc = "tk_ostream_retry",
                                                          .size = sizeof(tk_ostream_retry_t),
                                                          .on_destroy = tk_ostream_retry_on_destroy,
                                                          .get_prop = tk_ostream_retry_get_prop,
                                                          .set_prop = tk_ostream_retry_set_prop};

tk_ostream_t* tk_ostream_retry_create(tk_ostream_t* real_ostream) {
  object_t* obj = NULL;
  tk_ostream_retry_t* ostream_retry = NULL;
  return_value_if_fail(real_ostream != NULL, NULL);

  obj = object_create(&s_tk_ostream_retry_vtable);
  ostream_retry = TK_OSTREAM_RETRY(obj);
  return_value_if_fail(ostream_retry != NULL, NULL);

  OBJECT_REF(real_ostream);
  ostream_retry->timeout = 3000;
  ostream_retry->max_retry_times = 10;
  ostream_retry->real_ostream = real_ostream;
  ostream_retry->rb = ring_buffer_create(1024, 4096);
  wbuffer_init_extendable(&(ostream_retry->wb));
  TK_OSTREAM(obj)->write = tk_ostream_retry_write;
  TK_OSTREAM(obj)->flush = tk_ostream_retry_flush;

  return TK_OSTREAM(obj);
}

ret_t tk_ostream_retry_set_max_retry_times(tk_ostream_t* ostream, uint32_t max_retry_times) {
  tk_ostream_retry_t* ostream_retry = TK_OSTREAM_RETRY(ostream);
  return_value_if_fail(ostream_retry != NULL, RET_BAD_PARAMS);

  ostream_retry->max_retry_times = max_retry_times;

  return RET_OK;
}

ret_t tk_ostream_retry_set_max_buffer_size(tk_ostream_t* ostream, uint32_t max_buffer_size) {
  tk_ostream_retry_t* ostream_retry = TK_OSTREAM_RETRY(ostream);
  ring_buffer_t* rb = ostream_retry->rb;
  return_value_if_fail(ostream_retry != NULL, RET_BAD_PARAMS);

  rb->max_capacity = tk_max(rb->max_capacity, max_buffer_size);

  return RET_OK;
}

ret_t tk_ostream_retry_clear_buffer(tk_ostream_t* ostream) {
  tk_ostream_retry_t* ostream_retry = TK_OSTREAM_RETRY(ostream);
  ring_buffer_t* rb = ostream_retry->rb;
  return_value_if_fail(ostream_retry != NULL, RET_BAD_PARAMS);

  ostream_retry->retried_times = 0;
  ostream_retry->discard_bytes += ostream_retry->pending_bytes;
  ostream_retry->discard_packets += ostream_retry->pending_packets;

  ring_buffer_reset(rb);
  ostream_retry->pending_bytes = 0;
  ostream_retry->pending_packets = 0;

  return RET_OK;
}

ret_t tk_ostream_retry_set_discard_policy(tk_ostream_t* ostream,
                                          data_discard_policy_t discard_policy) {
  tk_ostream_retry_t* ostream_retry = TK_OSTREAM_RETRY(ostream);
  return_value_if_fail(ostream_retry != NULL, RET_BAD_PARAMS);

  ostream_retry->discard_policy = discard_policy;

  return RET_OK;
}
