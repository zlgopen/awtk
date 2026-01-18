/**
 * File:   stream_factory.h
 * Author: AWTK Develop Team
 * Brief:  stream factory
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-10-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_STREAM_FACTORY_H
#define TK_STREAM_FACTORY_H

#include "tkc/iostream.h"

BEGIN_C_DECLS

/**
 * @class tk_stream_factory_t
 * @annotation ["fake"]
 * 流工厂。
 *
 */

/**
 * @method tk_stream_factory_create_iostream
 *
 * 创建iostream对象。目前支持：
 * * tcp client: tcp://host:port
 * * udp client: udp://host:port
 * * serial    : serial://device?baudrate=115200&stopbits=1
 * * serial    : serial://device?baudrate=115200&stopbits=1&parity=odd&flowcontrol=soft&bytesize=8
 * 
 * 串口URL参数说明：
 * * baudrate {int} 波特率。常用值： 9600, 38400, 57600, 115200。(默认值：115200)
 * * stopbits {string} 停止位。可选值：1, 1.5, 2。(默认值：1)
 * * parity {string} 校验位。可选值：none, odd, even, mark, space。(默认值：none)
 * * flowcontrol {string} 流控。可选值：none, hardware, software。(默认值：none)
 * * bytesize {string} 数据位。可选值：5, 6, 7, 8 。(默认值：8) 
 * @param {const char*} url URL。
 *
 * @return {tk_iostream_t*} 返回iostream对象。
 *
 */
tk_iostream_t* tk_stream_factory_create_iostream(const char* url);

END_C_DECLS

#endif /*TK_STREAM_FACTORY_H*/
