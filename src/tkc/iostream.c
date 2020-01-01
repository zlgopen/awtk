/**
 * File:   tk_iostream.h
 * Author: AWTK Develop Team
 * Brief:  input/ouput stream interface
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
 * 2019-08-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/iostream.h"

tk_istream_t* tk_iostream_get_istream(tk_iostream_t* stream) {
  return_value_if_fail(stream != NULL && stream->get_istream != NULL, NULL);

  return stream->get_istream(stream);
}

tk_ostream_t* tk_iostream_get_ostream(tk_iostream_t* stream) {
  return_value_if_fail(stream != NULL && stream->get_ostream != NULL, NULL);

  return stream->get_ostream(stream);
}

int32_t tk_iostream_read(tk_iostream_t* stream, void* buff, uint32_t max_size) {
  tk_istream_t* is = tk_iostream_get_istream(stream);

  return tk_istream_read(is, buff, max_size);
}

int32_t tk_iostream_read_len(tk_iostream_t* stream, void* buff, uint32_t max_size,
                             uint32_t timeout_ms) {
  tk_istream_t* is = tk_iostream_get_istream(stream);

  return tk_istream_read_len(is, buff, max_size, timeout_ms);
}

int32_t tk_iostream_write(tk_iostream_t* stream, const void* buff, uint32_t max_size) {
  tk_ostream_t* os = tk_iostream_get_ostream(stream);

  return tk_ostream_write(os, buff, max_size);
}

int32_t tk_iostream_write_len(tk_iostream_t* stream, const void* buff, uint32_t max_size,
                              uint32_t timeout_ms) {
  tk_ostream_t* os = tk_iostream_get_ostream(stream);

  return tk_ostream_write_len(os, buff, max_size, timeout_ms);
}
