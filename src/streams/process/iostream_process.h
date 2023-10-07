/**
 * File:   iostream_process.h
 * Author: AWTK Develop Team
 * Brief:  input stream on process
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-09-27 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#ifndef TK_IOSTREAM_PROCESS_H
#define TK_IOSTREAM_PROCESS_H

#include "tkc/fs.h"
#include "tkc/iostream.h"
#include "process_helper.h"

BEGIN_C_DECLS

struct _tk_iostream_process_t;
typedef struct _tk_iostream_process_t tk_iostream_process_t;

/**
 * @class tk_iostream_process_t
 * @parent tk_iostream_t
 *
 * 基于子程序实现的输入流。
 * 备注：如果需要在 Windows 平台运行，请在应用程序初始化时，调用 tk_socket_init 函数初始化 socket。
 *
 */
struct _tk_iostream_process_t {
  tk_iostream_t iostream;

  /**
   * @property {char*} file_path
   * @annotation ["readable"]
   * 子进程路径。
   */
  char* file_path;

  /**
   * @property {char**} args
   * @annotation ["readable"]
   * 参数列表。
   */
  char** args;

  /**
   * @property {uint32_t} argc
   * @annotation ["readable"]
   * 参数列表长度。
   */
  uint32_t argc;

  /*private*/
  process_handle_t handle;
  
  tk_istream_t* istream;
  tk_ostream_t* ostream;
};

/**
 * @method tk_iostream_process_create
 *
 * 创建iostream对象。
 * 备注：如果需要在 Windows 平台运行，请在应用程序初始化时，调用 tk_socket_init 函数初始化 socket。
 * 
 * @param {const char*} file_path 子进程程序路径。
 * @param {const char**} args 子进程参数。
 * @param {uint32_t} argc 子进程参数长度。
 *
 * @return {tk_iostream_t*} 返回iostream对象。
 *
 */
tk_iostream_t* tk_iostream_process_create(const char* file_path, const char* args[], uint32_t argc);

/**
 * @method tk_iostream_process_start
 *
 * 启动子进程流。
 *
 * @param {tk_iostream_t*} iostream 子程序对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_iostream_process_start(tk_iostream_t* iostream);

#define TK_IOSTREAM_PROCESS(obj) ((tk_iostream_process_t*)(obj))

#define TK_IOSTREAM_PROCESS_FILE_PATH "file_path"

#define TK_IOSTREAM_PROCESS_CMD_START "start"

END_C_DECLS

#endif /*TK_IOSTREAM_PROCESS_H*/
