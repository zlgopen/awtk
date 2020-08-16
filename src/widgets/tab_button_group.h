/**
 * File:   tab_button_group.h
 * Author: AWTK Develop Team
 * Brief:  tab_button_group
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

#ifndef TK_TAB_BUTTON_GROUP_H
#define TK_TAB_BUTTON_GROUP_H

#include "base/widget.h"
#include "base/hscrollable.h"

BEGIN_C_DECLS

/**
 * @class tab_button_group_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 标签按钮分组控件。
 *
 * 一个简单的容器，主要用于对标签按钮进行布局和管理。
 *
 * tab\_button\_group\_t是[widget\_t](widget_t.md)的子类控件，
 * widget\_t的函数均适用于tab\_button\_group\_t控件。
 *
 * 在xml中使用"tab\_button\_group"标签创建标签按钮分组控件。如：
 *
 * ```xml
 *   <tab_button_group x="c" y="bottom:10" w="90%" h="30" compact="true" >
 *     <tab_button text="General"/>
 *     <tab_button text="Network" value="true" />
 *     <tab_button text="Security"/>
 *   </tab_button_group>
 * ```
 *
 * > 更多用法请参考：
 * [tab control](https://github.com/zlgopen/awtk/blob/master/design/default/ui/)
 *
 * 可用通过style来设置控件的显示风格，如颜色等等。如：
 *
 * ```xml
 * <tab_button_group>
 *  <style name="default">
 *   <normal/>
 *  </style>
 * </tab_button_group>
 * ```
 *
 */
typedef struct _tab_button_group_t {
  widget_t widget;

  /**
   * @property {bool_t} compact
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 紧凑型排版子控件(缺省FALSE)。
   */
  bool_t compact;

  /**
   * @property {bool_t} scrollable
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否支持滚动(缺省FALSE)。
   *
   * > 紧凑型排版子控件时才支持滚动。
   */
  bool_t scrollable;

  /*private*/
  hscrollable_t* hscrollable;
} tab_button_group_t;

/**
 * @method tab_button_group_create
 * 创建tab_button_group对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* tab_button_group_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method tab_button_group_set_compact
 * 设置compact。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  compact 是否使用紧凑布局(缺省FALSE)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tab_button_group_set_compact(widget_t* widget, bool_t compact);

/**
 * @method tab_button_group_set_scrollable
 * 设置scrollable。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  scrollable 是否允许滚动(缺省FALSE)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tab_button_group_set_scrollable(widget_t* widget, bool_t scrollable);

/**
 * @method tab_button_group_cast
 * 转换tab_button_group对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget tab_button_group对象。
 *
 * @return {widget_t*} tab_button_group对象。
 */
widget_t* tab_button_group_cast(widget_t* widget);

#define TAB_BUTTON_GROUP(widget) ((tab_button_group_t*)(tab_button_group_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(tab_button_group);

END_C_DECLS

#endif /*TK_TAB_BUTTON_GROUP_H*/
