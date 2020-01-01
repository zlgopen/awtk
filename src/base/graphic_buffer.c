/**
 * File:   graphic_buffer.c
 * Author: AWTK Develop Team
 * Brief:  graphic_buffer
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
 * 2019-10-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/bitmap.h"
#include "base/graphic_buffer.h"

uint8_t* graphic_buffer_lock_for_read(graphic_buffer_t* buffer) {
  return_value_if_fail(buffer != NULL && buffer->vt != NULL && buffer->vt->lock_for_read != NULL,
                       NULL);

  return buffer->vt->lock_for_read(buffer);
}

uint8_t* graphic_buffer_lock_for_write(graphic_buffer_t* buffer) {
  return_value_if_fail(buffer != NULL && buffer->vt != NULL && buffer->vt->lock_for_write != NULL,
                       NULL);

  return buffer->vt->lock_for_write(buffer);
}

ret_t graphic_buffer_unlock(graphic_buffer_t* buffer) {
  return_value_if_fail(buffer != NULL && buffer->vt != NULL && buffer->vt->unlock != NULL,
                       RET_BAD_PARAMS);

  return buffer->vt->unlock(buffer);
}

ret_t graphic_buffer_attach(graphic_buffer_t* buffer, void* data) {
  return_value_if_fail(buffer != NULL && buffer->vt != NULL && buffer->vt->attach != NULL,
                       RET_BAD_PARAMS);

  return buffer->vt->attach(buffer, data);
}

ret_t graphic_buffer_destroy(graphic_buffer_t* buffer) {
  return_value_if_fail(buffer != NULL && buffer->vt != NULL && buffer->vt->destroy != NULL,
                       RET_BAD_PARAMS);

  return buffer->vt->destroy(buffer);
}
