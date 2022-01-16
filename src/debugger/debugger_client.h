/**
 * File:   debugger_client.h
 * Author: AWTK Develop Team
 * Brief:  debugger_client
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-01-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEBUGGER_CLIENT_H
#define TK_DEBUGGER_CLIENT_H

#include "tkc/mem.h"
#include "tkc/iostream.h"
#include "debugger/debugger.h"

BEGIN_C_DECLS

/**
 * @class debugger_client_t
 * 调试器客户端。
 *
 */
typedef struct _debugger_client_t {
  debugger_t debugger;

  /**
   * @property {tk_iostream_t*} io
   * @annotation ["readable"]
   * 与服务器通信的stream对象。
   */
  tk_iostream_t* io;

  /*private*/
  /*读取包的缓冲区*/
  void* buff;
  uint32_t capacity;
  /*程序执行完成*/
  bool_t program_completed;
} debugger_client_t;

/**
 * @method debugger_client_create
 * 创建调试器对象。
 * @param {tk_iostream_t*} io io对象。
 *
 * @return {debugger_t*} 返回debugger对象。
 */
debugger_t* debugger_client_create(tk_iostream_t* io);

/**
 * @method debugger_client_cast
 * 类型转换。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {debugger_client_t*} 返回debugger对象。
 */
debugger_client_t* debugger_client_cast(debugger_t* debugger);

/**
 * @method debugger_client_dispatch
 * 分发一个服务端推送的事件。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_client_dispatch(debugger_t* debugger);

/**
 * @method debugger_client_wait_for_completed
 * 等待完成事件。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_client_wait_for_completed(debugger_t* debugger);

#define DEBUGGER_CLIENT(debugger) debugger_client_cast((debugger_t*)debugger);

END_C_DECLS

#endif /*TK_DEBUGGER_CLIENT_H*/
