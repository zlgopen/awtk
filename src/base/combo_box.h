/**
 * File:   combo_box.h
 * Author: AWTK Develop Team
 * Brief:  combo_box
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
 * 2018-07-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COMBO_BOX_H
#define TK_COMBO_BOX_H

#include "base/edit.h"

BEGIN_C_DECLS

typedef struct _combo_box_option_t {
  char* text;
  int32_t value;
  struct _combo_box_option_t* next;
} combo_box_option_t;

/**
 * @class combo_box_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * combobox控件。
 */
typedef struct _combo_box_t {
  edit_t edit;

  /**
   * @property {char*} open_window
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 为点击按钮时，要打开窗口的名称。
   */
  char* open_window;

  /**
   * @property {int32_t} selected_index
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   *当前选中的选项。
   */
  int32_t selected_index;

  /**
   * @property {int32_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值。
   */
  int32_t value;

  /**
   * @property {char*} options
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 设置可选项(冒号分隔值和文本，分号分隔选项，如:1:red;2:green;3:blue)。
   */
  char* options;

  /*private*/
  str_t text;
  combo_box_option_t* option_items;
} combo_box_t;

/**
 * @method combo_box_create
 * 创建combo_box对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* combo_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method combo_box_set_open_window
 * 点击按钮时可以打开popup窗口，本函数可设置窗口的名称。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 * @param {char*} open_window 弹出窗口的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_set_open_window(widget_t* widget, const char* open_window);

/**
 * @method combo_box_reset_options
 * 重置所有选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_reset_options(widget_t* widget);

/**
 * @method combo_box_count_options
 * 获取选项个数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 *
 * @return {int32_t} 返回选项个数。
 */
int32_t combo_box_count_options(widget_t* widget);

/**
 * @method combo_box_set_selected_index
 * 设置第index个选项为当前选中的选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 * @param {uint32_t} index 选项的索引。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_set_selected_index(widget_t* widget, uint32_t index);

/**
 * @method combo_box_append_option
 * 追加一个选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 * @param {int32_t} value 值。
 * @param {char*} text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_append_option(widget_t* widget, int32_t value, const char* text);

/**
 * @method combo_box_get_option
 * 获取第index个选项。
 * @annotation ["scriptable"] no
 * @param {widget_t*} widget combo_box对象。
 * @param {uint32_t} index 选项的索引。
 *
 * @return {combo_box_option_t*} 返回index个选项。
 */
combo_box_option_t* combo_box_get_option(widget_t* widget, uint32_t index);

/**
 * @method combo_box_get_value
 * 获取combo_box的值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 *
 * @return {int32_t} 返回值。
 */
int32_t combo_box_get_value(widget_t* widget);

/**
 * @method combo_box_get_text
 * 获取combo_box的文本。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 *
 * @return {char*} 返回文本。
 */
const char* combo_box_get_text(widget_t* widget);

#define COMBO_BOX(widget) ((combo_box_t*)(widget))

/*public for test*/
ret_t combo_box_parse_options(widget_t* widget, const char* str);

END_C_DECLS

#endif /*TK_COMBO_BOX_H*/
