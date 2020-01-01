/**
 * File:   serial_helper.h
 * Author: AWTK Develop Team
 * Brief:  serial helper functions
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/thread.h"

#include <errno.h>
#include <string.h>

BEGIN_C_DECLS

#ifdef WIN32
#include "windows.h"

typedef HANDLE serial_dev_t;

typedef struct _serial_info_t {
  int client_fd;
  int server_fd;
  bool_t closed;
  bool_t quited;
  serial_dev_t dev;
  tk_thread_t* thread;
} serial_info_t;

#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
typedef int serial_dev_t;

typedef struct _serial_info_t {
  serial_dev_t dev;
} serial_info_t;
#endif /*WIN32*/

typedef struct _serial_info_t* serial_handle_t;

int serial_handle_get_fd(serial_handle_t handle);
serial_dev_t serial_handle_get_dev(serial_handle_t handle);

#include "tkc/fs.h"
#include "tkc/iostream.h"

typedef enum { fivebits = 5, sixbits = 6, sevenbits = 7, eightbits = 8 } bytesize_t;

typedef enum {
  parity_none = 0,
  parity_odd = 1,
  parity_even = 2,
  parity_mark = 3,
  parity_space = 4
} parity_t;

typedef enum { stopbits_one = 1, stopbits_two = 2, stopbits_one_point_five } stopbits_t;

typedef enum { flowcontrol_none = 0, flowcontrol_software, flowcontrol_hardware } flowcontrol_t;

serial_handle_t serial_open(const char* port);

ret_t serial_iflush(serial_handle_t handle);
ret_t serial_oflush(serial_handle_t handle);
ret_t serial_wait_for_data(serial_handle_t handle, uint32_t timeout_ms);

int32_t serial_read(serial_handle_t handle, uint8_t* buff, uint32_t max_size);
int32_t serial_write(serial_handle_t handle, const uint8_t* buff, uint32_t max_size);

int serial_close(serial_handle_t handle);

ret_t serial_config(serial_handle_t handle, uint32_t baudrate, bytesize_t bytesize,
                    stopbits_t stopbits, flowcontrol_t flowcontrol, parity_t parity);

END_C_DECLS

#endif /*TK_SERIAL_HELPER_H*/
