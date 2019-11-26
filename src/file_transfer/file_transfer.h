/**
 * File:   file_transfer.h
 * Author: AWTK Develop Team
 * Brief:  file_transfer
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

#ifndef TK_FILE_TRANSFER_H
#define TK_FILE_TRANSFER_H

#include "tkc/iostream.h"

BEGIN_C_DECLS

typedef enum _file_transfer_packet_type_t {
  FILE_TRANSFER_PACKET_HEADER = 0xffeedd01,
  FILE_TRANSFER_PACKET_DATA = 0xffeedd02,
  FILE_TRANSFER_PACKET_ACK = 0xffeedd03,
  FILE_TRANSFER_PACKET_NACK = 0xffeedd04,
  FILE_TRANSFER_PACKET_REQ = 0xffeedd05
} file_transfer_packet_type_t;

typedef struct _file_transfer_header_t {
  uint32_t type;
  uint32_t size;
  char name[TK_NAME_LEN + 1];
} file_transfer_header_t;

typedef struct _file_transfer_req_t {
  uint32_t type;
  uint32_t block_size;
  char filename[TK_NAME_LEN + 1];
  char productid[TK_NAME_LEN + 1];
} file_transfer_req_t;

typedef struct _file_transfer_data_t {
  uint32_t type;
  uint32_t size;
  uint32_t offset;
  uint32_t crc;
} file_transfer_data_t;

typedef struct _file_transfer_ack_t {
  uint32_t type;
  uint32_t size;
  uint32_t offset;
  uint32_t crc;
} file_transfer_ack_t;

#define FILE_TRANSFER_MIN_BLOCK_SIZE 1024
#define FILE_TRANSFER_MAX_BLOCK_SIZE 64 * 1024
#define FILE_TRANSFER_DEFAULT_BLOCK_SIZE 4 * 1024

#define FILE_TRANSFER_FIRMWARE "firmware.bin"
#define FILE_TRANSFER_ASSETS "assets.bin"

END_C_DECLS

#endif /*TK_FILE_TRANSFER_H*/
