/**
 * File:   ostream_noisy.c
 * Author: AWTK Develop Team
 * Brief:  output stream for noisy
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
 * 2019-10-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "streams/noisy/ostream_noisy.h"

static int32_t tk_ostream_noisy_write(tk_ostream_t* stream, const uint8_t* buff, uint32_t size) {
  tk_ostream_noisy_t* ostream_noisy = TK_OSTREAM_NOISY(stream);

  if (ostream_noisy->error_level && size > 0) {
    wbuffer_t* wb = &(ostream_noisy->wb);
    bool_t should_inject = ostream_noisy->write_count % ostream_noisy->error_level == 0;

    wb->cursor = 0;
    if (should_inject) {
      uint32_t i = random() % size;
      wbuffer_write_binary(wb, buff, size);

      wb->data[i] = ~buff[i];
      log_debug("inject at %u(%u) %02x => %02x\n", i, size, buff[i], wb->data[i]);
      ostream_noisy->error_count++;
      ostream_noisy->write_count++;

      return tk_ostream_write(ostream_noisy->real_ostream, wb->data, size);
    }
  }

  ostream_noisy->write_count++;
  return tk_ostream_write(ostream_noisy->real_ostream, buff, size);
}

static ret_t tk_ostream_noisy_set_prop(object_t* obj, const char* name, const value_t* v) {
  tk_ostream_noisy_t* ostream_noisy = TK_OSTREAM_NOISY(obj);
  tk_ostream_t* real_ostream = ostream_noisy->real_ostream;

  if (tk_str_eq(name, TK_OSTREAM_NOISY_PROP_ERROR_LEVEL)) {
    ostream_noisy->error_level = value_uint32(v);
    return RET_OK;
  } else if (tk_str_eq(name, TK_OSTREAM_NOISY_PROP_ERROR_COUNT)) {
    ostream_noisy->error_count = value_uint32(v);
    return RET_OK;
  } else if (tk_str_eq(name, TK_OSTREAM_NOISY_PROP_WRITE_COUNT)) {
    ostream_noisy->write_count = value_uint32(v);
    return RET_OK;
  }

  return object_set_prop(OBJECT(real_ostream), name, v);
}

static ret_t tk_ostream_noisy_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_ostream_noisy_t* ostream_noisy = TK_OSTREAM_NOISY(obj);
  tk_ostream_t* real_ostream = ostream_noisy->real_ostream;

  if (tk_str_eq(name, TK_OSTREAM_NOISY_PROP_ERROR_LEVEL)) {
    value_set_uint32(v, ostream_noisy->error_level);
    return RET_OK;
  } else if (tk_str_eq(name, TK_OSTREAM_NOISY_PROP_ERROR_COUNT)) {
    value_set_uint32(v, ostream_noisy->error_count);
    return RET_OK;
  } else if (tk_str_eq(name, TK_OSTREAM_NOISY_PROP_WRITE_COUNT)) {
    value_set_uint32(v, ostream_noisy->write_count);
    return RET_OK;
  }

  return object_get_prop(OBJECT(real_ostream), name, v);
}

static ret_t tk_ostream_noisy_on_destroy(object_t* obj) {
  return RET_OK;
}

static const object_vtable_t s_tk_ostream_noisy_vtable = {.type = "tk_ostream_noisy",
                                                          .desc = "tk_ostream_noisy",
                                                          .size = sizeof(tk_ostream_noisy_t),
                                                          .on_destroy = tk_ostream_noisy_on_destroy,
                                                          .get_prop = tk_ostream_noisy_get_prop,
                                                          .set_prop = tk_ostream_noisy_set_prop};

tk_ostream_t* tk_ostream_noisy_create(tk_ostream_t* real_ostream) {
  object_t* obj = NULL;
  tk_ostream_noisy_t* ostream_noisy = NULL;
  return_value_if_fail(real_ostream != NULL, NULL);

  obj = object_create(&s_tk_ostream_noisy_vtable);
  ostream_noisy = TK_OSTREAM_NOISY(obj);
  return_value_if_fail(ostream_noisy != NULL, NULL);

  ostream_noisy->error_level = 3;
  ostream_noisy->real_ostream = real_ostream;
  wbuffer_init_extendable(&(ostream_noisy->wb));
  TK_OSTREAM(obj)->write = tk_ostream_noisy_write;

  return TK_OSTREAM(obj);
}
