/**
 * File:   input_device_status.h
 * Author: AWTK Develop Team
 * Brief:  input device status
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-11-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_INPUT_DEVICE_STATUS_H
#define TK_INPUT_DEVICE_STATUS_H

#include "base/widget.h"

BEGIN_C_DECLS

#define MAX_PRESSED_KEYS_NR 16

typedef struct _key_pressed_info_t {
  uint32_t key;
  uint32_t emitted;
  uint64_t time;
  /*
  * 窗口切换时，旧窗口按下的键，一直不松开，会持续触发PRESS，可能会干扰新窗口的用户功能。
  * 所在窗口切换时，abort全部已经按下的键，直到按键松开。
  */
  bool_t should_abort;
} key_pressed_info_t;

/**
 * @class input_device_status_t
 * 输入设备状态管理器。本类仅供窗口管理器内部使用。
 */
typedef struct _input_device_status_t {
  uint32_t ctrl : 1;
  uint32_t alt : 1;
  uint32_t shift : 1;
  uint32_t lctrl : 1;
  uint32_t lalt : 1;
  uint32_t lshift : 1;
  uint32_t rctrl : 1;
  uint32_t ralt : 1;
  uint32_t rshift : 1;
  uint32_t cmd : 1;
  uint32_t menu : 1;
  uint32_t capslock : 1;

  xy_t last_x;
  xy_t last_y;
  bool_t pressed;
  widget_t* widget;
  uint32_t long_press_check_timer;
  key_pressed_info_t pressed_info[MAX_PRESSED_KEYS_NR + 1];
} input_device_status_t;

/**
 * @method input_device_status_init
 * 初始化输入设备状态管理器。
 * @annotation ["constructor"]
 * @param {input_device_status_t*} ids 输入设备状态管理器对象。
 *
 * @return {input_device_status_t*} 返回输入设备状态管理器对象。
 */
input_device_status_t* input_device_status_init(input_device_status_t* ids);

/**
 * @method input_device_status_on_input_event
 * 对输入事件进行处理，然后分发给widget。
 * @param {input_device_status_t*} ids 输入设备状态管理器对象。
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {event_t*} e 事件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t input_device_status_on_input_event(input_device_status_t* ids, widget_t* widget, event_t* e);

/**
 * @method input_device_status_abort_all_pressed_keys
 * 取消全部已经按下的键。
 *
 * 窗口切换时，旧窗口按下的键，一直不松开，会持续触发PRESS，可能会干扰新窗口的用户功能。
 * 所在窗口切换时，abort全部已经按下的键，直到按键松开。
 * 
 * @param {input_device_status_t*} ids 输入设备状态管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t input_device_status_abort_all_pressed_keys(input_device_status_t* ids);

END_C_DECLS

#endif /*TK_INPUT_DEVICE_STATUS_H*/
