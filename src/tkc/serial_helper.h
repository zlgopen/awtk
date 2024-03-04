/**
 * File:   serial_helper.h
 * Author: AWTK Develop Team
 * Brief:  serial helper functions
 *
 * Copyright (c) 2019 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SERIAL_HELPER_H
#define TK_SERIAL_HELPER_H

#include <errno.h>
#include <string.h>
#include "tkc/types_def.h"

BEGIN_C_DECLS

#ifdef WITH_WASM
typedef int64_t serial_handle_t;
typedef int64_t serial_dev_t;
#elif defined(WIN32)
#include "windows.h"
#include "tkc/mutex.h"
#include "tkc/cond_var.h"
#include "tkc/thread.h"

typedef HANDLE serial_dev_t;
typedef struct _serial_info_t {
  serial_dev_t dev;
  OVERLAPPED read_overlapped;
  OVERLAPPED write_overlapped;
  bool_t closed;
  bool_t has_signal;
  int client_fd;
  int server_fd;
  tk_cond_t* cond;
  tk_mutex_t* mutex;
  tk_thread_t* thread;
} serial_info_t;

typedef struct _serial_info_t* serial_handle_t;
#else
#if defined(LINUX) || defined(MACOS)
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#endif /*LINUX || MACOS*/

typedef int serial_dev_t;

typedef struct _serial_info_t {
  serial_dev_t dev;
} serial_info_t;

typedef struct _serial_info_t* serial_handle_t;
#endif /*WIN32*/

/**
 * @enum bytesize_t
 * 串口字节位数。
 */
typedef enum {
  /**
   * @const fivebits
   * 每字节5位。
   */
  fivebits = 5,
  /**
   * @const sixbits
   * 每字节6位。
   */
  sixbits = 6,
  /**
   * @const sevenbits
   * 每字节7位。
   */
  sevenbits = 7,
  /**
   * @const eightbits
   * 每字节8位。
   */
  eightbits = 8
} bytesize_t;

/**
 * @enum parity_t
 * 串口奇偶校验。
 */
typedef enum {
  /**
   * @const parity_none
   * 无。
   */
  parity_none = 0,
  /**
   * @const parity_odd
   * 奇校验。
   */
  parity_odd = 1,
  /**
   * @const parity_even
   * 偶校验。
   */
  parity_even = 2,
  /**
   * @const parity_mark
   * 校验位的电平保持为逻辑 "1"。
   */
  parity_mark = 3,
  /**
   * @const parity_space
   * 校验位的电平保持为逻辑 "0"。
   */
  parity_space = 4
} parity_t;

/**
 * @enum stopbits_t
 * 串口停止位。
 */
typedef enum {
  /**
   * @const stopbits_one
   * 1位。
   */
  stopbits_one = 1,
  /**
   * @const stopbits_two
   * 2位。
   */
  stopbits_two = 2,
  /**
   * @const stopbits_one_point_five
   * 1.5位。
   */
  stopbits_one_point_five
} stopbits_t;

/**
 * @enum flowcontrol_t
 * 串口流控。
 */
typedef enum {
  /**
   * @const flowcontrol_none
   * 无。
   */
  flowcontrol_none = 0,
  /**
   * @const flowcontrol_software
   * 软件。
   */
  flowcontrol_software,
  /**
   * @const flowcontrol_hardware
   * 硬件。
   */
  flowcontrol_hardware
} flowcontrol_t;

/**
 * @class serial_t
 * @annotation ["fake"]
 */

/**
 * @method serial_open
 * 打开串口
 * > Windows下，需要在应用程序初始化时，调用 tk_socket_init。
 * @annotation ["static"]
 * @param {const char*} port 串口号。
 *
 * @return {serial_handle_t} 失败返回NULL。
 */
serial_handle_t serial_open(const char* port);

/**
 * @method serial_read
 * 串口读数据
 * @annotation ["static"]
 * @param {serial_handle_t} handle 串口句柄。
 * @param {uint8_t*} buff 数据缓冲区。
 * @param {uint32_t} max_size 数据长度。
 *
 * @return {int32_t} 返回实际读取的字节数。
 */
int32_t serial_read(serial_handle_t handle, uint8_t* buff, uint32_t max_size);

/**
 * @method serial_write
 * 串口写数据
 * @annotation ["static"]
 * @param {serial_handle_t} handle 串口句柄。
 * @param {const uint8_t*} buff 数据缓冲区。
 * @param {uint32_t} max_size 数据长度。
 *
 * @return {int32_t} 返回实际写入的字节数。
 */
int32_t serial_write(serial_handle_t handle, const uint8_t* buff, uint32_t max_size);

/**
 * @method serial_close
 * 关闭串口
 * @annotation ["static"]
 * @param {serial_handle_t} handle 串口句柄。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t serial_close(serial_handle_t handle);

/**
 * @method serial_config
 * 配置串口
 * @annotation ["static"]
 * @param {serial_handle_t} handle 串口句柄。
 * @param {uint32_t} baudrate 波特率。
 * @param {bytesize_t} bytesize 数据位。
 * @param {stopbits_t} stopbits 停止位
 * @param {flowcontrol_t} flowcontrol 流控。
 * @param {parity_t} parity 校验位。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t serial_config(serial_handle_t handle, uint32_t baudrate, bytesize_t bytesize,
                    stopbits_t stopbits, flowcontrol_t flowcontrol, parity_t parity);

/**
 * @method serial_handle_get_fd
 * 获取文件描述符。
 * @annotation ["static"]
 * @param {serial_handle_t} handle 串口句柄。
 *
 * @return {int} 返回文件描述符。
 */
int serial_handle_get_fd(serial_handle_t handle);

/**
 * @method serial_handle_get_dev
 * 获取设备句柄。
 * @annotation ["static"]
 * @param {serial_handle_t} handle 串口句柄。
 *
 * @return {serial_dev_t} 返回设备句柄。
 */
serial_dev_t serial_handle_get_dev(serial_handle_t handle);

/**
 * @method serial_iflush
 * 刷新input缓冲区。
 * @annotation ["static"]
 * @param {serial_handle_t} handle 串口句柄。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t serial_iflush(serial_handle_t handle);

/**
 * @method serial_oflush
 * 刷新output缓冲区。
 * @annotation ["static"]
 * @param {serial_handle_t} handle 串口句柄。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t serial_oflush(serial_handle_t handle);

/**
 * @method serial_wait_for_data
 * @annotation ["static"]
 * 等待数据。
 *
 * @param {serial_handle_t} handle 串口句柄。
 * @param {uint32_t} timeout_ms 等待时间(毫秒)。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t serial_wait_for_data(serial_handle_t handle, uint32_t timeout_ms);

/**
 * @method serial_stopbits_from_str
 * 字符串转stopbits
 * @annotation ["static"]
 * @param {const char*} str 字符串。
 *
 * @return {stopbits_t} 返回stopbits。
 */
stopbits_t serial_stopbits_from_str(const char* str);

/**
 * @method serial_flowcontrol_from_str
 * 字符串转flowcontrol
 * @annotation ["static"]
 * @param {const char*} str 字符串。
 *
 * @return {flowcontrol_t} 返回flowcontrol。
 */
flowcontrol_t serial_flowcontrol_from_str(const char* str);

/**
 * @method serial_parity_from_str
 * 字符串转parity
 * @annotation ["static"]
 * @param {const char*} str 字符串。
 *
 * @return {parity_t} 返回parity。
 */
parity_t serial_parity_from_str(const char* str);

/**
 * @method serial_bytesize_from_str
 * 字符串转bytesize
 * @annotation ["static"]
 * @param {const char*} str 字符串。
 *
 * @return {bytesize_t} 返回bytesize。
 */
bytesize_t serial_bytesize_from_str(const char* str);

/*不再使用 begin{*/
typedef struct {
  uint32_t rd_timeout;          /* 读超时时间(毫秒) */
  uint32_t rd_interval_timeout; /* 码间超时(毫秒) */
} serial_timeout_t;
ret_t serial_timeout_set(serial_handle_t handle, serial_timeout_t* timeout);
ret_t serial_timeout_get(serial_handle_t handle, serial_timeout_t* timeout);
/*}不再使用 end*/

END_C_DECLS

#endif /*TK_SERIAL_HELPER_H*/
