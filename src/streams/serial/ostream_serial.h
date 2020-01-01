/**
 * File:   ostream_serial.h
 * Author: AWTK Develop Team
 * Brief:  input stream base on serial
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

#ifndef TK_OSTREAM_SERIAL_H
#define TK_OSTREAM_SERIAL_H

#include "tkc/fs.h"
#include "tkc/ostream.h"
#include "streams/serial/serial_helper.h"

BEGIN_C_DECLS

struct _tk_ostream_serial_t;
typedef struct _tk_ostream_serial_t tk_ostream_serial_t;

/**
 * @class tk_ostream_serial_t
 * 基于串口实现的输出流。
 *
 * input stream base on fd
 *
 */
struct _tk_ostream_serial_t {
  tk_ostream_t ostream;

  serial_handle_t fd;
  bool_t is_broken;
};

/**
 * @method tk_ostream_serial_create
 *
 * 创建ostream对象。
 *
 * @param {int} fd fd.
 *
 * @return {tk_ostream_t*} 返回ostream对象。
 *
 */
tk_ostream_t* tk_ostream_serial_create(serial_handle_t fd);

#define TK_OSTREAM_SERIAL(obj) ((tk_ostream_serial_t*)(obj))

END_C_DECLS

#endif /*TK_OSTREAM_SERIAL_H*/
