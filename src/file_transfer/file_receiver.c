/**
 * File:   file_receiver.h
 * Author: AWTK Develop Team
 * Brief:  file_receiver 
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "tkc/crc.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "tkc/istream.h"
#include "tkc/ostream.h"
#include "tkc/iostream.h"
#include "file_transfer/file_receiver.h"

file_receiver_t* file_receiver_create(uint32_t block_size, const char* productid) {
  file_receiver_t* receiver = TKMEM_ZALLOC(file_receiver_t);
  return_value_if_fail(receiver != NULL, NULL);

  receiver->block_size =
      tk_max(tk_min(block_size, FILE_TRANSFER_MAX_BLOCK_SIZE), FILE_TRANSFER_MIN_BLOCK_SIZE);

  if (productid != NULL) {
    tk_strncpy(receiver->productid, productid, TK_NAME_LEN);
  }
  emitter_init(&(receiver->emitter));

  return receiver;
}

ret_t file_receiver_run(file_receiver_t* receiver, tk_iostream_t* io, tk_ostream_t* target,
                        const char* filename) {
  int32_t ret = 0;
  uint32_t crc = 0;
  uint32_t offset = 0;
  uint32_t timeout = 50000;
  uint8_t* buff = NULL;
  tk_istream_t* in = NULL;
  tk_ostream_t* out = NULL;
  file_transfer_req_t req;
  file_transfer_ack_t ack;
  file_transfer_data_t data;
  file_transfer_meta_t meta;
  progress_event_t event;
  return_value_if_fail(receiver != NULL, RET_BAD_PARAMS);

  memset(&req, 0x00, sizeof(req));
  memset(&ack, 0x00, sizeof(ack));
  memset(&data, 0x00, sizeof(data));
  memset(&meta, 0x00, sizeof(meta));

  in = tk_iostream_get_istream(io);
  out = tk_iostream_get_ostream(io);
  goto_error_if_fail(in != NULL && out != NULL);

  if (filename != NULL) {
    tk_strncpy(req.filename, filename, TK_NAME_LEN);
  }

  if (receiver->productid != NULL) {
    tk_strncpy(req.productid, receiver->productid, TK_NAME_LEN);
  }

  req.type = FILE_TRANSFER_PACKET_REQ;
  req.block_size = receiver->block_size;
  ret = tk_ostream_write_len(out, (const uint8_t*)&req, sizeof(req), timeout);
  goto_error_if_fail(ret == sizeof(req));

  ret = tk_istream_read_len(in, (uint8_t*)&meta, sizeof(meta), timeout * 100);
  goto_error_if_fail(ret == sizeof(meta));

  goto_error_if_fail(meta.size > 0);
  buff = TKMEM_ALLOC(receiver->block_size + 1);
  goto_error_if_fail(buff != NULL);

  ack.type = FILE_TRANSFER_PACKET_ACK;
  do {
    ret = tk_istream_read_len(in, (uint8_t*)&data, sizeof(data), timeout);
    goto_error_if_fail(ret == sizeof(data));
    goto_error_if_fail(data.type == FILE_TRANSFER_PACKET_DATA);

    ret = tk_istream_read_len(in, (uint8_t*)buff, data.size, timeout);
    goto_error_if_fail(ret == data.size);

    ack.size = ret;
    ack.offset = data.offset;
    crc = tk_crc32(PPPINITFCS32, buff, data.size);

    if (crc != data.crc) {
      log_debug("crc error\n");
      ack.type = FILE_TRANSFER_PACKET_NACK;
      ret = tk_ostream_write_len(out, (const uint8_t*)&ack, sizeof(ack), timeout);

      break;
    }

    ret = tk_ostream_write_len(out, (const uint8_t*)&ack, sizeof(ack), timeout);
    goto_error_if_fail(ret == sizeof(ack));

    ret = tk_ostream_write_len(target, (const uint8_t*)buff, data.size, timeout);
    offset += data.size;

    emitter_dispatch(EMITTER(receiver), progress_event_init(&event, (100 * offset / meta.size)));
    if (offset == meta.size) {
      log_debug("transfer done.\n");
      break;
    }
  } while (TRUE);

error:
  TKMEM_FREE(buff);

  return (offset == meta.size && offset > 0) ? RET_OK : RET_FAIL;
}

ret_t file_receiver_destroy(file_receiver_t* receiver) {
  return_value_if_fail(receiver != NULL, RET_BAD_PARAMS);

  emitter_deinit(&(receiver->emitter));
  TKMEM_FREE(receiver);

  return RET_OK;
}
