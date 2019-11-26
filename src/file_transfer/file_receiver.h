/**
 * File:   file_receiver.h
 * Author: AWTK Develop Team
 * Brief:  file_receiver 
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

#ifndef TK_FILE_RECEIVER_H
#define TK_FILE_RECEIVER_H

#include "tkc/emitter.h"
#include "file_transfer/file_transfer.h"

BEGIN_C_DECLS

typedef struct _file_receiver_t {
  emitter_t emitter;
  uint32_t received_bytes;
  uint32_t total_bytes;

  tk_iostream_t* io;
  tk_ostream_t* target;

  uint32_t block_size;
  char productid[TK_NAME_LEN + 1];
} file_receiver_t;

file_receiver_t* file_receiver_create(tk_iostream_t* io, tk_ostream_t* target, uint32_t block_size,
                                      const char* productid);
ret_t file_receiver_run(file_receiver_t* receiver, const char* filename);
ret_t file_receiver_destroy(file_receiver_t* receiver);

END_C_DECLS

#endif /*TK_FILE_RECEIVER_H*/
