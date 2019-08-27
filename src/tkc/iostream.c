/**
 * File:   tk_iostream.h
 * Author: AWTK Develop Team
 * Brief:  input/ouput stream interface
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
