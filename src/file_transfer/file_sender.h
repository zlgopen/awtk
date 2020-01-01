/**
 * File:   file_sender.h
 * Author: AWTK Develop Team
 * Brief:  file_sender
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

#ifndef TK_FILE_SENDER_H
#define TK_FILE_SENDER_H

#include "tkc/emitter.h"
#include "file_transfer/file_transfer.h"

BEGIN_C_DECLS

/**
 * @class file_sender_t
 * 文件发送方。 
 * 
 * > 可用注册EVT_PROGRESS事件，以获取传输的进度状态。
 */
typedef struct _file_sender_t {
  emitter_t emitter;
} file_sender_t;

/**
 * @method file_sender_create
 * @annotation ["constructor"]
 * 创建sender对象。
 * 
 * @return {file_sender_t*} 返回sender对象。
 */
file_sender_t* file_sender_create(void);

/**
 * @method file_sender_run
 * 开始传输文件。
 * 
 * @param {file_sender_t*} sender sender对象。
 * @param {tk_iostream_t*} io 数据传输流。
 * @param {const char*} filename 文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_sender_run(file_sender_t* sender, tk_iostream_t* io, const char* filename);

/**
 * @method file_sender_destroy
 * 销毁sender对象。
 * 
 * @param {file_sender_t*} sender sender对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_sender_destroy(file_sender_t* sender);

END_C_DECLS

#endif /*TK_FILE_SENDER_H*/
