/**
 * File:   list_view.h
 * Author: AWTK Develop Team
 * Brief:  list_view
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
 * 2018-07-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LIST_VIEW_H
#define TK_LIST_VIEW_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class list_view_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 列表视图控件。
 *
 * 列表视图控件是一个可以垂直滚动的列表控件。
 *
 * 如果不需要滚动，可以用view控件配置适当的layout参数作为列表控件。
 *
 * 列表视图中的列表项可以固定高度，也可以使用不同高度。请参考[变高列表项](
 *https://github.com/zlgopen/awtk/blob/master/design/default/ui/list_view_vh.xml)
 *
 * 列表视图控件的中可以有滚动条，也可以没有滚动条。
 * 可以使用移动设备风格的滚动条，也可以使用桌面风格的滚动条。
 *
 * list\_view\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于list\_view\_t控件。
 *
 * 在xml中使用"list\_view"标签创建列表视图控件。如：
 *
 * ```xml
 * <list_view x="0"  y="30" w="100%" h="-80" item_height="60">
 *   <scroll_view name="view" x="0"  y="0" w="100%" h="100%">
 *     <list_item style="odd" children_layout="default(rows=1,cols=0)">
 *       <image draw_type="icon" w="30" image="earth"/>
 *       <label w="-30" text="1.Hello AWTK !">
 *         <switch x="r:10" y="m" w="60" h="20"/>
 *       </label>
 *     </list_item>
 *     ...
 *   </scroll_view>
 *  </list_view>
 * ```
 *
 * > 注意：列表项不是作为列表视图控件的直接子控件，而是作为滚动视图的子控件。
 *
 *
 * > 更多用法请参考：[list\_view\_m.xml](
 *https://github.com/zlgopen/awtk/blob/master/design/default/ui/list_view_m.xml)
 *
 * 在c代码中使用函数list\_view\_create创建列表视图控件。如：
 *
 * ```c
 *  widget_t* list_view = list_view_create(win, 0, 0, 0, 0);
 * ```
 *
 * 用代码构造列表视图是比较繁琐的事情，最好用XML来构造。
 * 如果需要动态修改，可以使用widget\_clone来增加列表项，使用widget\_remove\_child来移出列表项。
 *
 * 可用通过style来设置控件的显示风格，如背景颜色和边框颜色等(一般情况不需要)。
 *
 */
typedef struct _list_view_t {
  widget_t widget;
  /**
   * @property {int32_t} item_height
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 列表项的高度。如果 item_height > 0，所有列表项使用固定高度，否则使用列表项自身的高度。
   */
  int32_t item_height;
  /**
   * @property {int32_t} default_item_height
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 列表项的缺省高度。如果item_height <= 0 而且列表项自身的高度 <= 0，则使用缺省高度。
   */
  int32_t default_item_height;
  /**
   * @property {bool_t} auto_hide_scroll_bar
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 如果不需要滚动条时，自动隐藏滚动条。
   */
  bool_t auto_hide_scroll_bar;

  /*private*/
  widget_t* scroll_view;
  widget_t* scroll_bar;
} list_view_t;

/**
 * @method list_view_create
 * 创建list_view对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* list_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method list_view_set_item_height
 * 设置列表项的高度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} item_height 列表项的高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_view_set_item_height(widget_t* widget, int32_t item_height);

/**
 * @method list_view_set_default_item_height
 * 设置列表项的缺省高度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} default_item_height 列表项的高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_view_set_default_item_height(widget_t* widget, int32_t default_item_height);

/**
 * @method list_view_set_auto_hide_scroll_bar
 * 设置是否自动隐藏滚动条。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} auto_hide_scroll_bar 是否自动隐藏滚动条。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_view_set_auto_hide_scroll_bar(widget_t* widget, bool_t auto_hide_scroll_bar);

/**
 * @method list_view_cast
 * 转换为list_view对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget list_view对象。
 *
 * @return {widget_t*} list_view对象。
 */
widget_t* list_view_cast(widget_t* widget);

/**
 * @method list_view_reinit
 * list_view重新初始化。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget list_view对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_view_reinit(widget_t* widget);

#define LIST_VIEW(widget) ((list_view_t*)(list_view_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(list_view);

END_C_DECLS

#endif /*TK_LIST_VIEW_H*/
