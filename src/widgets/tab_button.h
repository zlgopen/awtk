/**
 * File:   tab_button.h
 * Author: AWTK Develop Team
 * Brief:  tab_button
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TAB_BUTTON_H
#define TK_TAB_BUTTON_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class tab_button_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 标签按钮控件。
 *
 * 标签按钮有点类似单选按钮，但点击标签按钮之后会自动切换当前的标签页。
 *
 * tab\_button\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于tab\_button\_t控件。
 *
 * 在xml中使用"tab\_button"标签创建标签按钮控件。如：
 *
 *```xml
 *   <!-- tab_button_view_page1.xml -->
 *   <view w="100%" h="100%">
 *     <label x="c" y="m" w="100%" h="60" text="page1" /> 
 *     <button name="close" x="c" y="bottom:100" w="80" h="40" text="Close" />
 *   </view>
 * ```
 * 
 * ```xml
 *   <!-- tab_button dynamic load UI -->
 *   <pages name="pages" x="right" y="0" w="70%" h="100%"> 
 *   </pages>
 *   <list_view x="0" y="0" w="30%" h="100%" item_height="40" auto_hide_scroll_bar="true">
 *   <scroll_view name="view" x="0"  y="0" w="-12" h="100%">
 *     <tab_button text="page1" load_ui="tab_button_view_page1" value="true"/>
 *     <tab_button text="page2" load_ui="tab_button_view_page2" />
 *     <tab_button text="page3" load_ui="tab_button_view_page3" />
 *   <scroll_view />
 *   <scroll_bar_d name="bar" x="right" y="0" w="12" h="100%" value="0"/>
 *   </list_view>
 * ```
 * 
 * ```xml
 *   <!-- tab_button static load UI -->
 *   <tab_button_group x="c" y="bottom:10" w="90%" h="30" compact="true" >
 *     <tab_button text="General"/>
 *     <tab_button text="Network" value="true" />
 *     <tab_button text="Security"/>
 *   </tab_button_group>
 * ```
 *
 * 标签按钮一般放在标签按钮分组中，布局由标签按钮分组控件决定，不需要指定自己的布局参数和坐标。
 *
 * > 更多用法请参考：
 * [tab control](https://github.com/zlgopen/awtk/blob/master/design/default/ui/)
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 * <tab_button>
 *  <style name="default" border_color="#a0a0a0"  text_color="black">
 *   <normal     bg_color="#d0d0d0" />
 *   <pressed    bg_color="#f0f0f0" />
 *   <over       bg_color="#e0e0e0" />
 *   <normal_of_active     bg_color="#f0f0f0" />
 *   <pressed_of_active    bg_color="#f0f0f0" />
 *   <over_of_active       bg_color="#f0f0f0" />
 * </style>
 * </tab_button>
 * ```
 *
 */
typedef struct _tab_button_t {
  widget_t widget;
  /**
   * @property {bool_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值。
   */
  bool_t value;
  /**
   * @property {char*} load_ui
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 激活后加载的UI名字。
   */
  char* load_ui;
  /**
   * @property {char*}active_icon
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 当前项的图标的名称。
   */
  char* active_icon;
  /**
   * @property {char*} icon
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 非当前项的图标的名称。
   */
  char* icon;

  /*private*/
  widget_t* ui;
} tab_button_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值(激活状态)即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值(激活状态)改变事件。
 */

/**
 * @method tab_button_create
 * 创建tab_button对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* tab_button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method tab_button_cast
 * 转换tab_button对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget tab_button对象。
 *
 * @return {widget_t*} tab_button对象。
 */
widget_t* tab_button_cast(widget_t* widget);

/**
 * @method tab_button_set_value
 * 设置为当前标签。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget tab_button对象。
 * @param {uint32_t}  value 是否为当前标签。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tab_button_set_value(widget_t* widget, bool_t value);

/**
 * @method tab_button_set_icon
 * 设置控件的图标。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget tab_button对象。
 * @param {char*}  name 当前项的图标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tab_button_set_icon(widget_t* widget, const char* name);

/**
 * @method tab_button_set_active_icon
 * 设置控件的active图标。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget tab_button对象。
 * @param {char*}  name 当前项的图标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tab_button_set_active_icon(widget_t* widget, const char* name);

/**
 * @method tab_button_set_load_ui
 * 设置控件动态加载显示UI。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget tab_button对象。
 * @param {char*}  name 动态加载UI的资源名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tab_button_set_load_ui(widget_t* widget, const char* name);

#define TAB_BUTTON(widget) ((tab_button_t*)(tab_button_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(tab_button);

END_C_DECLS

#endif /*TK_TAB_BUTTON_H*/
