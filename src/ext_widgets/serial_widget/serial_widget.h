/**
 * File:   serial_widget.h
 * Author: AWTK Develop Team
 * Brief:  用于串口通信的控件
 *
 * Copyright (c) 2022 - 2022 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-07-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SERIAL_WIDGET_H
#define TK_SERIAL_WIDGET_H

#include "base/widget.h"
#include "tkc/iostream.h"

BEGIN_C_DECLS
/**
 * @class serial_widget_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 用于串口通信的控件
 * 在xml中使用"serial"标签创建控件。如：
 *
 * ```xml
 * <!-- ui -->
 * <serial device="COM1" baudrate="115200"/>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 * > 本控件默认不可见，无需指定style。
 *
 * ```xml
 * <!-- style -->
 * <serial>
 *   <style name="default" font_size="32">
 *     <normal text_color="black" />
 *   </style>
 * </serial>
 * ```
 */
typedef struct _serial_widget_t {
  widget_t widget;

  /**
   * @property {char*} device
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 设备(文件)名。
   */
  char* device;

  /**
   * @property {uint32_t} baudrate
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 波特率。
   */
  uint32_t baudrate;

  /**
   * @property {uint32_t} bytesize
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 字节位数。
   */
  uint32_t bytesize;

  /**
   * @property {uint32_t} parity
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 奇偶校验。
   */
  uint32_t parity;

  /**
   * @property {uint32_t} stopbits
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 停止位。
   */
  uint32_t stopbits;

  /**
   * @property {uint32_t} flowcontrol
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 流控。
   */
  uint32_t flowcontrol;

  /**
   * @property {tk_istream_t*} istream
   * @annotation ["set_prop","get_prop","readable","persitent","design"]
   * 输入流。
   */
  tk_istream_t* istream;

  /**
   * @property {tk_ostream_t*} ostream
   * @annotation ["set_prop","get_prop","readable","persitent","design"]
   * 输出流。
   */
  tk_ostream_t* ostream;

  /**
   * @property {tk_iostream_t*} iostream
   * @annotation ["set_prop","get_prop","readable","persitent","design"]
   * 输入/出流。
   */
  tk_iostream_t* iostream;

  /**
   * @property {uint32_t} check_interval
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 轮询时间（单位：ms）。
   * > 仅在不支持用select等待串口数据的嵌入式设备上有效。
   */
  uint32_t check_interval;

  /*private*/
  uint32_t idle_id;
  uint32_t timer_id;
  event_source_t* event_source;
} serial_widget_t;

/**
 * @event {event_t} EVT_DATA
 * 数据到来事件。
 */

/**
 * @method serial_widget_create
 * @annotation ["constructor", "scriptable"]
 * 创建serial_widget对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} serial_widget对象。
 */
widget_t* serial_widget_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method serial_widget_cast
 * 转换为serial_widget对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget serial_widget对象。
 *
 * @return {widget_t*} serial_widget对象。
 */
widget_t* serial_widget_cast(widget_t* widget);

/**
 * @method serial_widget_set_baudrate
 * 设置 波特率。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} baudrate 波特率。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t serial_widget_set_baudrate(widget_t* widget, uint32_t baudrate);

/**
 * @method serial_widget_set_device
 * 设置 设备。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} device 设备。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t serial_widget_set_device(widget_t* widget, const char* device);

/**
 * @method serial_widget_set_bytesize
 * 设置 字节位数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} bytesize 字节位数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t serial_widget_set_bytesize(widget_t* widget, uint32_t bytesize);

/**
 * @method serial_widget_set_parity
 * 设置 奇偶校验。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} parity 奇偶校验。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t serial_widget_set_parity(widget_t* widget, uint32_t parity);

/**
 * @method serial_widget_set_stopbits
 * 设置 停止位。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} stopbits 停止位。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t serial_widget_set_stopbits(widget_t* widget, uint32_t stopbits);

/**
 * @method serial_widget_set_flowcontrol
 * 设置 流控。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} flowcontrol 流控。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t serial_widget_set_flowcontrol(widget_t* widget, uint32_t flowcontrol);

/**
 * @method serial_widget_set_check_interval
 * 设置 轮询时间。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} check_interval 轮询时间（单位：ms）。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t serial_widget_set_check_interval(widget_t* widget, uint32_t check_interval);

#define SERIAL_WIDGET_PROP_BAUDRATE "baudrate"
#define SERIAL_WIDGET_PROP_DEVICE "device"
#define SERIAL_WIDGET_PROP_BYTESIZE "bytesize"
#define SERIAL_WIDGET_PROP_PARITY "parity"
#define SERIAL_WIDGET_PROP_STOPBITS "stopbits"
#define SERIAL_WIDGET_PROP_FLOWCONTROL "flowcontrol"
#define SERIAL_WIDGET_PROP_ISTREAM "istream"
#define SERIAL_WIDGET_PROP_OSTREAM "ostream"
#define SERIAL_WIDGET_PROP_IOSTREAM "iostream"
#define SERIAL_WIDGET_PROP_CHECK_INTERVAL "check_interval"

#define WIDGET_TYPE_SERIAL_WIDGET "serial"

#define SERIAL_WIDGET(widget) ((serial_widget_t*)(serial_widget_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(serial_widget);

END_C_DECLS

#endif /*TK_SERIAL_WIDGET_H*/
