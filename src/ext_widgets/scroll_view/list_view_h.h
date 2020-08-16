/**
 * File:   list_view_h.h
 * Author: AWTK Develop Team
 * Brief:  list_view_h
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
 * 2018-07-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LIST_VIEW_H_H
#define TK_LIST_VIEW_H_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class list_view_h_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 水平列表视图控件。
 *
 * list\_view\_h\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于list\_view\_h\_t控件。
 *
 * 在xml中使用"list\_view\_h"标签创建水平列表视图控件。如：
 *
 * ```xml
 * <list_view_h x="center"  y="10" w="90%" h="100" item_width="200" spacing="5">
 *   <scroll_view name="view" w="100%" h="100%">
 *     <image style="border" draw_type="auto" image="1" text="1"/>
 *     ...
 *   </scroll_view>
 * </list_view_h>
 * ```
 *
 * > 注意：列表项不是作为列表视图控件的直接子控件，而是作为滚动视图的子控件。
 *
 *
 * > 更多用法请参考：[list\_view\_h.xml](
 *https://github.com/zlgopen/awtk/blob/master/design/default/ui/list_view_h.xml)
 *
 * 在c代码中使用函数list\_view\_h\_create创建水平列表视图控件。如：
 *
 * ```c
 *  widget_t* list_view = list_view_h_create(win, 0, 0, 0, 0);
 * ```
 *
 * 用代码构造列表视图是比较繁琐的事情，最好用XML来构造。
 * 如果需要动态修改，可以使用widget\_clone来增加列表项，使用widget\_remove\_child来移出列表项。
 *
 * 可用通过style来设置控件的显示风格，如背景颜色和边框颜色等(一般情况不需要)。
 *
 */
typedef struct _list_view_h_t {
  widget_t widget;
  /**
   * @property {int32_t} item_width
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 列表项的宽度。
   */
  int32_t item_width;
  /**
   * @property {int32_t} spacing
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 间距。
   */
  int32_t spacing;

  /*private*/
  widget_t* scroll_view;
} list_view_h_t;

/**
 * @method list_view_h_create
 * 创建list_view_h对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* list_view_h_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method list_view_h_set_item_width
 * 设置列表项的宽度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} item_width 列表项的宽度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_view_h_set_item_width(widget_t* widget, int32_t item_width);

/**
 * @method list_view_h_set_spacing
 * 设置列表项的间距。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} spacing 列表项的间距。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_view_h_set_spacing(widget_t* widget, bool_t spacing);

/**
 * @method list_view_h_cast
 * 转换为list_view_h对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget list_view_h对象。
 *
 * @return {widget_t*} list_view_h对象。
 */
widget_t* list_view_h_cast(widget_t* widget);

#define LIST_VIEW_H(widget) ((list_view_h_t*)(list_view_h_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(list_view_h);

END_C_DECLS

#endif /*TK_LIST_VIEW_H_H*/
