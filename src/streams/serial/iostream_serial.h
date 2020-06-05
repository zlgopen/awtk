/**
 * File:   iostream_serial.h
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

#ifndef TK_IOSTREAM_SERIAL_H
#define TK_IOSTREAM_SERIAL_H

#include "tkc/fs.h"
#include "tkc/iostream.h"
#include "streams/serial/serial_helper.h"

BEGIN_C_DECLS

struct _tk_iostream_serial_t;
typedef struct _tk_iostream_serial_t tk_iostream_serial_t;

/**
 * @class tk_iostream_serial_t
 * @parent tk_iostream_t
 *
 * 基于串口实现的输入输出流。
 *
 */
struct _tk_iostream_serial_t {
  tk_iostream_t iostream;

  serial_handle_t fd;
  parity_t parity;
  int32_t baudrate;
  bytesize_t bytesize;
  stopbits_t stopbits;
  flowcontrol_t flowcontrol;

  tk_istream_t* istream;
  tk_ostream_t* ostream;
};

/**
 * @method tk_iostream_serial_create
 *
 * 创建iostream对象。
 * > 缺省配置: baudrate=115200 bytesize=8 stopbits=1 flowcontrol=0 parity=0
 * @param {const char*} port port name。
 *
 * @return {tk_iostream_t*} 返回iostream对象。
 *
 */
tk_iostream_t* tk_iostream_serial_create(const char* port);

/**
 * @method tk_iostream_serial_config
 *
 * 配置串口。
 *
 * @param {tk_iostream_t*} iostream 串口对象。
 * @param {int32_t} baudrate 波特率。
 * @param {bytesize_t} bytesize 字节位数。
 * @param {parity_t} parity 奇偶校验。
 * @param {stopbits_t} stopbits 停止位。
 * @param {flowcontrol_t} flowcontrol 流控。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_iostream_serial_config(tk_iostream_t* iostream, int32_t baudrate, bytesize_t bytesize,
                                parity_t parity, stopbits_t stopbits, flowcontrol_t flowcontrol);

#define TK_IOSTREAM_SERIAL_PROP_PARITY "parity"
#define TK_IOSTREAM_SERIAL_PROP_STOPBITS "stopbits"
#define TK_IOSTREAM_SERIAL_PROP_BAUDRATE "baudrate"
#define TK_IOSTREAM_SERIAL_PROP_BYTESIZE "bytesize"
#define TK_IOSTREAM_SERIAL_PROP_FLOWCONTROL "flowcontrol"

#define TK_IOSTREAM_SERIAL_CMD_CONFIG "config"

#define TK_IOSTREAM_SERIAL(obj) ((tk_iostream_serial_t*)(obj))

END_C_DECLS

#endif /*TK_IOSTREAM_SERIAL_H*/
