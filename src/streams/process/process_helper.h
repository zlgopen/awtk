/**
 * File:   process_helper.h
 * Author: AWTK Develop Team
 * Brief:  process helper
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

#ifndef TK_PROCESS_HELPER_H
#define TK_PROCESS_HELPER_H
#ifndef WITH_WASM

#include "tkc/types_def.h"
#include "tkc/thread.h"

BEGIN_C_DECLS

struct _process_info_t;
typedef struct _process_info_t process_info_t;
typedef struct _process_info_t* process_handle_t;

/**
 * @class process_start_info_t
 * 子程序启动配置结构体。
 */
typedef struct _process_start_info_t {
  /**
   * @property {char*} work_dir
   * @annotation ["readable"]
   * 子进程当前工作目录。（如果 NULL 的话，就为父进程的工作目录）
   */
  char* work_dir;
} process_start_info_t;

#ifdef WIN32

#include "tkc/wstr.h"
#include <Windows.h>

struct _process_info_t {
  bool_t quit;
  bool_t broken;

  HANDLE h_std_in_wr;
  HANDLE h_std_out_rd;
  STARTUPINFOW start_info;
  PROCESS_INFORMATION proc_info;

  int client_fd;
  int server_fd;

  wchar_t* file_path;
  wstr_t cmd_line;
  tk_thread_t* rthread;
};

#elif defined(LINUX) || defined(MACOS)
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include "tkc/str.h"

struct _process_info_t {
  bool_t broken;

  pid_t pid;
  int read_pfd[2];
  int write_pfd[2];

  str_t str_tmp;
  tk_thread_t* thread;
};

#endif

/**
 * @method process_create
 * 创建子进程。
 * @annotation ["static"]
 * @param {const char*} file_path 子进程程序路径。(如果为空的话，使用命令行来执行参数的内容)
 * @param {const char**} args 子进程参数。
 * @param {uint32_t} argc 子进程参数长度。
 * @param {const process_start_info_t*} start_info 子进程启动信息。（如果 NULL，就使用默认是值）
 *
 * @return {process_handle_t} 返回子进程句柄。
 */
process_handle_t process_create(const char* file_path, const char** args, uint32_t argc,
                                const process_start_info_t* start_info);

/**
 * @method process_destroy
 * 释放子进程。
 * @annotation ["static"]
 * @param {process_handle_t} handle 子进程句柄。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t process_destroy(process_handle_t handle);

/**
 * @method process_handle_get_fd
 * 获取文件描述符。
 * @annotation ["static"]
 * @param {process_handle_t} handle 子进程句柄。
 *
 * @return {int} 返回文件描述符。
 */
int process_handle_get_fd(process_handle_t handle);

/**
 * @method process_wait_for_data
 * @annotation ["static"]
 * 等待数据。
 *
 * @param {process_handle_t} handle 子进程句柄。
 * @param {uint32_t} timeout_ms 等待时间(ms)。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t process_wait_for_data(process_handle_t handle, uint32_t timeout_ms);

/**
 * @method process_read
 * 读子进程的数据（StdIntput 的方式）
 * @annotation ["static"]
 * @param {process_handle_t} handle 子进程句柄。
 * @param {uint8_t*} buff 数据缓冲区。
 * @param {uint32_t} max_size 数据长度。
 *
 * @return {int32_t} 返回实际读取的字节数。
 */
int32_t process_read(process_handle_t handle, uint8_t* buff, uint32_t max_size);

/**
 * @method process_write
 * 给子进程写数据（StdOutput 的方式）
 * @annotation ["static"]
 * @param {process_handle_t} handle 子进程句柄。
 * @param {const uint8_t*} buff 数据缓冲区。
 * @param {uint32_t} max_size 数据长度。
 *
 * @return {int32_t} 返回实际写入的字节数。
 */
int32_t process_write(process_handle_t handle, const uint8_t* buff, uint32_t max_size);

/**
 * @method process_is_broken
 * 子进程是否断开
 * @annotation ["static"]
 * @param {process_handle_t} handle 子进程句柄。
 *
 * @return {bool_t} 断开返回 true。
 */
bool_t process_is_broken(process_handle_t handle);

/**
 * @method process_kill
 * 杀死子进程
 * @annotation ["static"]
 * @param {process_handle_t} handle 子进程句柄。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t process_kill(process_handle_t handle);

END_C_DECLS

#endif
#endif
