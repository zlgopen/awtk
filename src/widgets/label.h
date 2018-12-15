/**
 * File:   label.h
 * Author: AWTK Develop Team
 * Brief:  label
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

#ifndef TK_LABEL_H
#define TK_LABEL_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class label_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 文本控件。
 */
typedef struct _label_t {
  widget_t widget;

  /**
   * @property {int32_t} length
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 显示字符的长度。主要用于动态改变显示字符的个数，来实现类似[拨号中...]的动画效果。
   */
  int32_t length;
} label_t;

/**
 * @method label_create
 * @annotation ["constructor", "scriptable"]
 * 创建label对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* label_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method label_set_length
 * 设置最大可显示字符数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  repeat 释放EVT_CLICK事件的时间间隔(毫秒)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t label_set_length(widget_t* widget, int32_t length);

#define LABEL(widget) ((label_t*)(widget))

/*public for test*/
uint32_t line_breaker_count(const wchar_t* str);
typedef ret_t (*line_breaker_on_line_t)(void* ctx, uint32_t index, const wchar_t* str,
                                        uint32_t size);
ret_t line_breaker_break(const wchar_t* str, line_breaker_on_line_t on_line, void* ctx);

END_C_DECLS

#endif /*TK_LABEL_H*/
