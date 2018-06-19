/**
 * File:   edit.h
 * Author: AWTK Develop Team
 * Brief:  edit
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_EDIT_H
#define TK_EDIT_H

#include "base/widget.h"
#include "base/input_method.h"

BEGIN_C_DECLS

typedef struct _input_limit_t {
  input_type_t type;
  union {
    struct {
      uint32_t min;
      uint32_t max;
    } t;
    struct {
      uint32_t min;
      uint32_t max;
    } i;
    struct {
      float min;
      float max;
      float step;
    } f;
  } u;
} input_limit_t;

/**
 * @class edit_t
 * @parent widget_t
 * @scriptable
 * 文本控件。
 */
typedef struct _edit_t {
  widget_t widget;
  uint16_t visible_end;
  uint16_t visible_start;
  uint16_t selected_start;
  uint16_t selected_end;
  xy_t caret_x;
  bool_t readonly;
  bool_t caret_visible;
  bool_t password_visible;

  wstr_t tips;
  uint32_t timer_id;
  input_limit_t limit;
} edit_t;

/**
 * @method edit_create
 * @constructor
 * 创建edit对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* edit_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method edit_set_text_limit
 * 设置为文本输入及其长度限制，不允许输入超过max个字符，少于min个字符时进入error状态。
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} min 最小长度。
 * @param {uint32_t} max 最大长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t edit_set_text_limit(widget_t* widget, uint32_t min, uint32_t max);

/**
 * @method edit_set_int_limit
 * 设置为整数输入及取值范围。
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} min 最小值。
 * @param {int32_t} max 最大值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t edit_set_int_limit(widget_t* widget, int32_t min, int32_t max);

/**
 * @method edit_set_float_limit
 * 设置为浮点数输入及取值范围。
 * @param {widget_t*} widget widget对象。
 * @param {float} min 最小值。
 * @param {float} max 最大值。
 * @param {float} step 步长。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t edit_set_float_limit(widget_t* widget, float min, float max, float step);

/**
 * @method edit_set_readonly
 * 设置编辑器是否为只读。
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} readonly 只读。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t edit_set_readonly(widget_t* widget, bool_t readonly);

/**
 * @method edit_set_input_type
 * 设置编辑器的输入类型。
 * @param {widget_t*} widget widget对象。
 * @param {input_type_t} type 输入类型。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t edit_set_input_type(widget_t* widget, input_type_t type);

/**
 * @method edit_set_input_tips
 * 设置编辑器的输入提示。
 * @param {widget_t*} widget widget对象。
 * @param {wchar_t*} tips 输入提示。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t edit_set_input_tips(widget_t* widget, const wchar_t* tips);

/**
 * @method edit_set_password_visible
 * 当编辑器输入类型为密码时，设置密码是否可见。
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} password_visible 密码是否可见。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t edit_set_password_visible(widget_t* widget, bool_t password_visible);

#define EDIT(widget) ((edit_t*)(widget))

END_C_DECLS

#endif /*TK_EDIT_H*/
