/**
 * File:   file_transfer.h
 * Author: AWTK Develop Team
 * Brief:  file_transfer
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

#ifndef TK_FILE_TRANSFER_H
#define TK_FILE_TRANSFER_H

#include "tkc/iostream.h"

BEGIN_C_DECLS

/**
 * @enum file_transfer_packet_type_t
 * 文件传输包的类型。
 */
typedef enum _file_transfer_packet_type_t {
  /**
   * @const FILE_TRANSFER_PACKET_META
   * 文件元数据包(sender->receiver)。
   */
  FILE_TRANSFER_PACKET_META = 0xffeedd01,

  /**
   * @const FILE_TRANSFER_PACKET_DATA
   * 文件数据包(sender->receiver)。
   */
  FILE_TRANSFER_PACKET_DATA = 0xffeedd02,

  /**
   * @const FILE_TRANSFER_PACKET_ACK
   * 文件数据确认包(sender<-receiver)。
   */
  FILE_TRANSFER_PACKET_ACK = 0xffeedd03,

  /**
   * @const FILE_TRANSFER_PACKET_NACK
   * 文件数据错误确认包(sender<-receiver)。
   */
  FILE_TRANSFER_PACKET_NACK = 0xffeedd04,

  /**
   * @const FILE_TRANSFER_PACKET_REQ
   * 请求开始传输包(sender<-receiver)。
   */
  FILE_TRANSFER_PACKET_REQ = 0xffeedd05
} file_transfer_packet_type_t;

/**
 * @class file_transfer_meta_t
 * 文件元数据包(sender->receiver)。 
 */
typedef struct _file_transfer_meta_t {
  /**
   * @property {uint32_t} type
   * @annotation ["readable","writable"]
   * 包的类型。
   */
  uint32_t type;

  /**
   * @property {uint32_t} size
   * @annotation ["readable","writable"]
   * 即将传输文件的长度。
   */
  uint32_t size;

  /**
   * @property {char*} name
   * @annotation ["readable","writable"]
   * 即将传输文件的名称。
   */
  char name[TK_NAME_LEN + 1];
} file_transfer_meta_t;

/**
 * @class file_transfer_data_t
 * 文件数据包(sender->receiver)。 
 */
typedef struct _file_transfer_data_t {
  /**
   * @property {uint32_t} type
   * @annotation ["readable","writable"]
   * 包的类型。
   */
  uint32_t type;

  /**
   * @property {uint32_t} size
   * @annotation ["readable","writable"]
   * 本次传输文件数据的长度。
   */
  uint32_t size;

  /**
   * @property {uint32_t} offset
   * @annotation ["readable","writable"]
   * 本次传输文件数据的偏移量。
   */
  uint32_t offset;

  /**
   * @property {uint32_t} crc
   * @annotation ["readable","writable"]
   * 本次传输文件数据的CRC。
   */
  uint32_t crc;
} file_transfer_data_t;

/**
 * @class file_transfer_req_t
 * 请求开始传输包(sender<-receiver)。
 */
typedef struct _file_transfer_req_t {
  /**
   * @property {uint32_t} type
   * @annotation ["readable","writable"]
   * 包的类型。
   */
  uint32_t type;

  /**
   * @property {uint32_t} block_size
   * @annotation ["readable","writable"]
   * 一次传输包的大小。
   */
  uint32_t block_size;

  /**
   * @property {char*} filename
   * @annotation ["readable","writable"]
   * 请求传输的文件名。
   */
  char filename[TK_NAME_LEN + 1];

  /**
   * @property {char*} productid
   * @annotation ["readable","writable"]
   * 请求方的产品ID(有时可用于发送方发送不同的固件，暂时没用)
   */
  char productid[TK_NAME_LEN + 1];
} file_transfer_req_t;

/**
 * @class file_transfer_ack_t
 * 确认包(sender<-receiver)。
 */
typedef struct _file_transfer_ack_t {
  /**
   * @property {uint32_t} type
   * @annotation ["readable","writable"]
   * 包的类型。
   */
  uint32_t type;

  /**
   * @property {uint32_t} size
   * @annotation ["readable","writable"]
   * 确认收到的数据长度。
   */
  uint32_t size;

  /**
   * @property {uint32_t} offset
   * @annotation ["readable","writable"]
   * 确认收到的数据偏移量。
   */
  uint32_t offset;
} file_transfer_ack_t;

#define FILE_TRANSFER_MIN_BLOCK_SIZE 1024
#define FILE_TRANSFER_MAX_BLOCK_SIZE 64 * 1024
#define FILE_TRANSFER_DEFAULT_BLOCK_SIZE 4 * 1024

#define FILE_TRANSFER_FIRMWARE "firmware.bin"
#define FILE_TRANSFER_ASSETS "assets.bin"

END_C_DECLS

#endif /*TK_FILE_TRANSFER_H*/
