/**
 * File:   buffer.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  buffer 
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-02-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_BUFFER_H
#define LFTK_BUFFER_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef struct _wbuffer_t {
  uint8_t* data;
  uint32_t cursor; 
  uint32_t capacity; 
}wbuffer_t;

typedef struct _rbuffer_t {
  const uint8_t* data;
  uint32_t cursor; 
  uint32_t capacity; 
}rbuffer_t;

wbuffer_t* wbuffer_init(wbuffer_t* wbuffer, uint8_t* data, uint32_t capacity);

ret_t wbuffer_write_uint8(wbuffer_t* wbuffer, uint8_t value);
ret_t wbuffer_write_uint16(wbuffer_t* wbuffer, uint16_t value);
ret_t wbuffer_write_uint32(wbuffer_t* wbuffer, uint32_t value);
ret_t wbuffer_write_binary(wbuffer_t* wbuffer, const void* data, uint32_t size);
ret_t wbuffer_write_string(wbuffer_t* wbuffer, const char* data);

rbuffer_t* rbuffer_init(rbuffer_t* rbuffer, const uint8_t* data, uint32_t capacity);

bool_t rbuffer_has_more(rbuffer_t* rbuffer);
ret_t rbuffer_read_uint8(rbuffer_t* rbuffer, uint8_t* value);
ret_t rbuffer_read_uint16(rbuffer_t* rbuffer, uint16_t* value);
ret_t rbuffer_read_uint32(rbuffer_t* rbuffer, uint32_t* value);
ret_t rbuffer_read_binary(rbuffer_t* rbuffer, void* data, uint32_t size);
ret_t rbuffer_read_string(rbuffer_t* rbuffer, const char** str);

ret_t rbuffer_peek_uint8(rbuffer_t* rbuffer, uint8_t* value);
ret_t rbuffer_peek_uint16(rbuffer_t* rbuffer, uint16_t* value);
ret_t rbuffer_peek_uint32(rbuffer_t* rbuffer, uint32_t* value);

END_C_DECLS

#endif/*LFTK_BUFFER_H*/

