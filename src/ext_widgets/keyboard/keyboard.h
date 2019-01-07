/**
 * File:   keyboard.h
 * Author: AWTK Develop Team
 * Brief:  keyboard
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-06-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_KEYBOARD_H
#define TK_KEYBOARD_H

#include "base/window_base.h"

BEGIN_C_DECLS

/**
 * @class keyboard_t
 * @parent widget_t
 * 键盘。
 *
 */
typedef struct _keyboard_t {
  window_base_t window;

  /*private*/
  array_t action_buttons;
  uint32_t action_info_id;
} keyboard_t;

/**
 * @method keyboard_create
 * 创建keyboard对象
 * @annotation ["constructor"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* keyboard_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method keyboard_close
 * 关闭keyboard窗口。
 * @annotation ["deconstructor"]
 * @param {widget_t*} parent keyboard对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t keyboard_close(widget_t* parent);

#define KEYBOARD(widget) ((keyboard_t*)(widget))

END_C_DECLS

#endif /*TK_KEYBOARD_H*/
