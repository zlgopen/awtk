/**
 * File:   debugger_lldb.h
 * Author: AWTK Develop Team
 * Brief:  debugger lldb
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
 * 2023-05-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEBUGGER_LLDB_H
#define TK_DEBUGGER_LLDB_H

#include "tkc/iostream.h"
#include "debugger/debugger.h"
#include "debugger/debugger_client.h"
#include "debugger/debugger_const.h"
#include "debugger/debugger_global.h"
#include "debugger/debugger_message.h"

BEGIN_C_DECLS

/**
 * @class debugger_lldb_t
 * 调试器LLDB客户端(使用DAP协议与lldb-vscode连接)。
 *
 */
typedef struct _debugger_lldb_t {
  debugger_t debugger;

  /*private*/
  /*req/resp的body*/
  str_t body;
  /*req/resp的header*/
  str_t header;

  /*临时保存callstack*/
  str_t scallstack;
  /*临时保存breakpoints*/
  str_t sbreakpoints;

  tk_iostream_t* io;
  int64_t stop_thread_id;
  int64_t current_frame_id;
  int64_t current_frame_line;
  const char* current_frame_name;
  const char* current_frame_source;

  tk_object_t* resps;
  /*代码文件缓存*/
  tk_object_t* sources;
  /*callstack缓存*/
  tk_object_t* callstack;
  /*函数断点*/
  darray_t functions_break_points;
  /*源代码行号断点*/
  tk_object_t* source_break_points;
} debugger_lldb_t;

/**
 * @method debugger_lldb_create
 * 创建调试器TCP客户端对象。
 * @param {const char*} host 目标主机。
 * @param {uint32_t} port 目标端口。
 *
 * @return {debugger_t*} 返回debugger对象。
 */
debugger_t* debugger_lldb_create(const char* host, uint32_t port);

#define STR_SCHEMA_PID "pid://"
#define STR_SCHEMA_WASM "wasm://"

END_C_DECLS

#endif /*TK_DEBUGGER_LLDB_H*/
