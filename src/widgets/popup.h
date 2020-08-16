/**
 * File:   popup.h
 * Author: AWTK Develop Team
 * Brief:  popup
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
 * 2018-07-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_POPUP_H
#define TK_POPUP_H

#include "base/window_base.h"

BEGIN_C_DECLS

/**
 * @class popup_t
 * @parent window_base_t
 * @annotation ["scriptable","design","widget","window"]
 * 弹出窗口。
 *
 * 弹出窗口是一种特殊的窗口，大小和位置可以自由设置，主要用来实现右键菜单和combo\_box的下列列表等功能。
 *
 * popup\_t是[window\_base\_t](window_base_t.md)的子类控件，window\_base\_t的函数均适用于popup\_t控件。
 *
 * 在xml中使用"popup"标签创建弹出窗口。如：
 *
 * ```xml
 * <popup close_when_click_outside="true" x="c" y="m" w="80%" h="80" >
 * <list_view x="0"  y="0" w="100%" h="100%" item_height="30">
 *   <scroll_view name="view" x="0"  y="0" w="-12" h="100%">
 *     <combo_box_item tr_text="english"/>
 *     <combo_box_item tr_text="chinese" />
 *   </scroll_view>
 *   <scroll_bar_d name="bar" x="right" y="0" w="12" h="100%" value="0"/>
 * </list_view>
 * </popup>
 * ```
 *
 * >
 * 更多用法请参考：[popup](https://github.com/zlgopen/awtk/blob/master/design/default/ui/)
 *
 * 在c代码中使用函数popup\_create创建弹出窗口。如：
 *
 * ```c
 * widget_t* win = popup_create(NULL, 0, 0, w, h);
 * popup_set_close_when_click_outside(win, TRUE);
 * ```
 *
 * > 创建之后，和使用普通窗口是一样的。
 *
 * > 完整示例请参考：[combo_box.c](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/combo_box.c)
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 * <popup>
 *  <style name="default" border_color="#a0a0a0">
 *    <normal bg_color="#f0f0f0"/>
 *  </style>
 * </popup>
 * ```
 *
 * > 更多用法请参考：[theme
 * default](https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L324)
 */
typedef struct _popup_t {
  window_base_t window;

  /**
   * @property {bool_t} close_when_click
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 点击时是否关闭窗口。
   */
  bool_t close_when_click;

  /**
   * @property {bool_t} close_when_click_outside
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 点击到窗口外时是否关闭窗口。
   */
  bool_t close_when_click_outside;

  /*private*/
  bool_t is_outside;
} popup_t;

/**
 * @method popup_create
 * @annotation ["constructor", "scriptable"]
 * 创建popup对象。
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} popup对象。
 */
widget_t* popup_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method popup_cast
 * 转换为popup对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget popup对象。
 *
 * @return {widget_t*} popup对象。
 */
widget_t* popup_cast(widget_t* widget);

/**
 * @method popup_set_close_when_click
 * 设置点击时是否关闭窗口。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} close_when_click 点击时是否关闭窗口。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t popup_set_close_when_click(widget_t* widget, bool_t close_when_click);

/**
 * @method popup_set_close_when_click_outside
 * 设置点击窗口外部时是否关闭窗口。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} close_when_click_outside 点击窗口外部时是否关闭窗口。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t popup_set_close_when_click_outside(widget_t* widget, bool_t close_when_click_outside);

#define POPUP(widget) ((popup_t*)(popup_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(popup);

END_C_DECLS

#endif /*TK_POPUP_H*/
