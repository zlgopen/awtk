/**
 * File:   file_sender.h
 * Author: AWTK Develop Team
 * Brief:  file_sender
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-11-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FILE_SENDER_H
#define TK_FILE_SENDER_H

#include "tkc/emitter.h"
#include "file_transfer/file_transfer.h"

BEGIN_C_DECLS

typedef struct _file_sender_t {
  emitter_t emitter;
  uint32_t sent_bytes;
  uint32_t total_bytes;

  tk_iostream_t* io;
  const char* filename;
} file_sender_t;

file_sender_t* file_sender_create(const char* filename, tk_iostream_t* io);
ret_t file_sender_run(file_sender_t* sender);
ret_t file_sender_destroy(file_sender_t* sender);

END_C_DECLS

#endif /*TK_FILE_SENDER_H*/
