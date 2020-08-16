/**
 * File:   progress_bar.h
 * Author: AWTK Develop Team
 * Brief:  progress_bar
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PROGRESS_BAR_H
#define TK_PROGRESS_BAR_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class progress_bar_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 进度条控件。
 *
 * 进度条控件可以水平显示也可以垂直显示，由vertical属性决定。
 *
 * progress\_bar\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于progress\_bar\_t控件。
 *
 * 在xml中使用"progress\_bar"标签创建进度条控件。如：
 *
 * ```xml
 *  <progress_bar name="bar1" x="10" y="128" w="240" h="30" value="40"/>
 *  <progress_bar name="bar2" x="280" y="128" w="30" h="118" value="20" vertical="true"/>
 * ```
 *
 * > 更多用法请参考：
 * [basic demo](https://github.com/zlgopen/awtk/blob/master/design/default/ui/basic.xml)
 *
 * 在c代码中使用函数progress\_bar\_create创建进度条控件。如：
 *
 * ```c
 *  widget_t* progress_bar = progress_bar_create(win, 10, 10, 128, 30);
 * ```
 *
 * > 完整示例请参考：
 * [progress_bar demo](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/progress_bar.c)
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 * <style>
 *   <normal bg_color="#f0f0f0" text_color="gold" fg_color="#c0c0c0" border_color="#a0a0a0" />
 * </style>
 * ```
 *
 * > 更多用法请参考：
 * [theme
 * default](https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L183)
 *
 */
typedef struct _progress_bar_t {
  widget_t widget;
  /**
   * @property {float_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 进度条的值[0-max]。
   */
  float_t value;
  /**
   * @property {float_t} max
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最大值(缺省为100)。
   */
  float_t max;
  /**
   * @property {bool_t} vertical
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 进度条的是否为垂直方向。
   */
  bool_t vertical;
  /**
   * @property {bool_t} show_text
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否显示文本。
   */
  bool_t show_text;
} progress_bar_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值改变事件。
 */

/**
 * @method progress_bar_create
 * @annotation ["constructor", "scriptable"]
 * 创建progress_bar对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* progress_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method progress_bar_cast
 * 转换为progress_bar对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget progress_bar对象。
 *
 * @return {widget_t*} progress_bar对象。
 */
widget_t* progress_bar_cast(widget_t* widget);

/**
 * @method progress_bar_set_value
 * 设置进度条的进度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {float_t}  value 进度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_bar_set_value(widget_t* widget, float_t value);

/**
 * @method progress_bar_set_max
 * 设置最大值。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t}  max 最大值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_bar_set_max(widget_t* widget, float_t max);

/**
 * @method progress_bar_set_vertical
 * 设置进度条的方向。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  vertical 是否为垂直方向。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_bar_set_vertical(widget_t* widget, bool_t vertical);

/**
 * @method progress_bar_set_show_text
 * 设置进度条的是否显示文本。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  show_text 是否显示文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_bar_set_show_text(widget_t* widget, bool_t show_text);

/**
 * @method progress_bar_get_percent
 * 获取进度百分比。
 *
 * > 当max为100时，percent和value取整后一致。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {uint32_t} 返回百分比。
 */
uint32_t progress_bar_get_percent(widget_t* widget);

#define PROGRESS_BAR(widget) ((progress_bar_t*)(progress_bar_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(progress_bar);

END_C_DECLS

#endif /*TK_PROGRESS_BAR_H*/
