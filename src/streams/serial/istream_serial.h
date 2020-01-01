/**
 * File:   istream_serial.h
 * Author: AWTK Develop Team
 * Brief:  input stream base on serial port
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

#ifndef TK_ISTREAM_SERIAL_H
#define TK_ISTREAM_SERIAL_H

#include "tkc/fs.h"
#include "tkc/istream.h"
#include "streams/serial/serial_helper.h"

BEGIN_C_DECLS

struct _tk_istream_serial_t;
typedef struct _tk_istream_serial_t tk_istream_serial_t;

/**
 * @class tk_istream_serial_t
 * @parent tk_istream_t
 *
 * 基于串口实现的输入流。
 *
 */
struct _tk_istream_serial_t {
  tk_istream_t istream;

  serial_handle_t fd;
  bool_t is_broken;
};

/**
 * @method tk_istream_serial_create
 *
 * 创建istream对象。
 *
 * @param {int} fd fd。
 *
 * @return {tk_istream_t*} 返回istream对象。
 *
 */
tk_istream_t* tk_istream_serial_create(serial_handle_t fd);

#define TK_ISTREAM_SERIAL(obj) ((tk_istream_serial_t*)(obj))

END_C_DECLS

#endif /*TK_ISTREAM_SERIAL_H*/
