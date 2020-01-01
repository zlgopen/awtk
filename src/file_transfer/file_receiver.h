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

#ifndef TK_FILE_RECEIVER_H
#define TK_FILE_RECEIVER_H

#include "tkc/emitter.h"
#include "file_transfer/file_transfer.h"

BEGIN_C_DECLS

/**
 * @class file_receiver_t
 * 文件接收方。 
 * 
 * > 可用注册EVT_PROGRESS事件，以获取传输的进度状态。
 */
typedef struct _file_receiver_t {
  emitter_t emitter;

  uint32_t block_size;
  char productid[TK_NAME_LEN + 1];
} file_receiver_t;

/**
 * @method file_receiver_create
 * @annotation ["constructor"]
 * 创建receiver对象。
 *  
 * @param {uint32_t} block_size 一次传输块的大小。
 * @param {const char*} productid 产品名称(有时可用于发送方发送不同的固件，暂时没用)。
 * 
 * @return {file_receiver_t*} 返回receiver对象。
 */
file_receiver_t* file_receiver_create(uint32_t block_size, const char* productid);

/**
 * @method file_receiver_run
 * 开始传输文件。
 * 
 * @param {file_receiver_t*} receiver receiver对象。
 * @param {tk_iostream_t*} io 数据传输流。
 * @param {tk_ostream_t*} target 用于写入接收到的文件数据。
 * @param {const char*} filename 文件名(期望接收的文件名)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_receiver_run(file_receiver_t* receiver, tk_iostream_t* io, tk_ostream_t* target,
                        const char* filename);

/**
 * @method file_receiver_destroy
 * 销毁receiver对象。
 * 
 * @param {file_receiver_t*} receiver receiver对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_receiver_destroy(file_receiver_t* receiver);

END_C_DECLS

#endif /*TK_FILE_RECEIVER_H*/
