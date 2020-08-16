/**
 * File:   color_picker.h
 * Author: AWTK Develop Team
 * Brief:  color_picker
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
 * 2018-08-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COLOR_PICKER_H
#define TK_COLOR_PICKER_H

#include "base/widget.h"
#include "color_picker/color_picker_const.h"

BEGIN_C_DECLS

/**
 * @class color_picker_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 颜色选择器。
 *
 * color\_picker\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于color\_picker\_t控件。
 *
 * 在xml中使用"color\_picker"标签创建颜色选择器控件。如：
 *
 * ```xml
 * <color_picker x="0" y="0" w="100%" h="100%" value="orange">
 *  <color_component x="0" y="0" w="200" h="200" name="sv"/>
 *  <color_component x="210" y="0" w="20" h="200" name="h"/>
 *  <color_tile x="0" y="210" w="50%" h="20" name="new" bg_color="green"/>
 *  <color_tile x="right" y="210" w="50%" h="20" name="old" bg_color="blue"/>
 * </color_picker>
 * ```
 *
 * > 更多用法请参考：
 * [color\_picker](https://github.com/zlgopen/awtk/blob/master/design/default/ui/color_picker.xml)
 *
 * 其中的子控件必须按下列规则命名：
 *
 * * r 红色分量。可以是spin_box、edit和slider。
 * * g 绿色分量。可以是spin_box、edit和slider。
 * * b 蓝色分量。可以是spin_box、edit和slider。
 * * h Hue分量。可以是spin_box、edit、slider和color_component。
 * * s Saturation分量。可以是spin_box、edit和slider。
 * * v Value/Brightness分量。可以是spin_box、edit和slider。
 * * sv Saturation和Value/Brightness分量。可以是color_component。
 * * old 旧的值。可以是spin_box、edit和color_tile。
 * * new 新的值。可以是spin_box、edit和color_tile。
 *
 */
typedef struct _color_picker_t {
  widget_t widget;

  /**
   * @property {const char*} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 颜色。
   */
  char value[TK_COLOR_HEX_LEN + 1];

  /*private*/
  color_t c;
  bool_t inited;
  color_t init_c;
  int32_t last_hue;
  widget_t* trigger_child;
} color_picker_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值(颜色)即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值(颜色)改变事件。
 */

/**
 * @method color_picker_create
 * 创建color_picker对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* color_picker_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method color_picker_set_color
 * 设置颜色。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} color 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t color_picker_set_color(widget_t* widget, const char* color);

/**
 * @method color_picker_cast
 * 转换为color_picker对象(供脚本语言使用)。
 *
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget color_picker对象。
 *
 * @return {widget_t*} color_picker对象。
 */
widget_t* color_picker_cast(widget_t* widget);

#define COLOR_PICKER(widget) ((color_picker_t*)(color_picker_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(color_picker);

END_C_DECLS

#endif /*TK_COLOR_PICKER_H*/
