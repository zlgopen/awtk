/**
 * File:   label.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  label
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_LABEL_H
#define LFTK_LABEL_H

#include "base/wstr.h"
#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class label_t
 * @parent widget_t
 * @scriptable
 * 文本控件。
 */
typedef struct _label_t {
  widget_t widget;
  wstr_t text;
  /**
   * @property {uint8_t} border
   * @readonly
   * 边距。
   */
  uint8_t border;
  /**
   * @property {uint8_t} text_align_v
   * @readonly
   * 文本垂直对齐方式。
   */
  uint8_t text_align_v;
  /**
   * @property {uint8_t} text_align_h
   * @readonly
   * 文本水平对齐方式。
   */
  uint8_t text_align_h;
} label_t;

/**
 * @method label_create
 * @constructor
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
 * @method label_set_border
 * 设置控件的边距。
 * @param {widget_t*} widget label对象。
 * @param {uint8_t}  border 边距
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t label_set_border(widget_t* widget, uint8_t border);

/**
 * @method label_set_text_align_h
 * 设置控件的文本水平对齐方式。
 * @param {widget_t*} widget label对象。
 * @param {uint8_t}  text_align_h 文本水平对齐方式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t label_set_text_align_h(widget_t* widget, uint8_t text_align_h);

/**
 * @method label_set_text_align_v
 * 设置控件的文本垂直对齐方式。
 * @param {widget_t*} widget label对象。
 * @param {uint8_t}  text_align_v 文本垂直对齐方式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t label_set_text_align_v(widget_t* widget, uint8_t text_align_v);

/**
 * @method label_set_text
 * 设置控件的文本。
 * @param {widget_t*} widget label对象。
 * @param {wchar_t*}  text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t label_set_text(widget_t* widget, const wchar_t* text);

#define LABEL(widget) ((label_t*)(widget))

END_C_DECLS

#endif /*LFTK_LABEL_H*/
