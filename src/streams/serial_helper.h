/**
 * File:   serial_helper.h
 * Author: AWTK Develop Team
 * Brief:  serial helper functions
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/types_def.h"

BEGIN_C_DECLS

#ifdef WIN32
#include "windows.h"
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#endif /*WIN32*/

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

int serial_open(const char* port);

ret_t serial_iflush(int fd);
ret_t serial_oflush(int fd);

int32_t serial_read(int fd, uint8_t* buff, uint32_t max_size);
int32_t serial_write(int fd, const uint8_t* buff, uint32_t max_size);

int serial_close(int fd);

ret_t serial_config(int fd, uint32_t baudrate, bytesize_t bytesize, stopbits_t stopbits,
                    flowcontrol_t flowcontrol, parity_t parity);

END_C_DECLS

#endif /*TK_SERIAL_HELPER_H*/
