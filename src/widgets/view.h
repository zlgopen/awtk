/**
 * File:   view.h
 * Author: AWTK Develop Team
 * Brief:  view
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
 * 2018-04-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_H
#define TK_VIEW_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class view_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 一个通用的容器控件。
 *
 * 它本身不提供布局功能，仅提供具有语义的标签，让xml更具有可读性。
 * 子控件的布局可用layout\_children属性指定。
 * 请参考[布局参数](https://github.com/zlgopen/awtk/blob/master/docs/layout.md)。
 *
 * view\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于view\_t控件。
 *
 * 在xml中使用"view"标签创建view。如：
 *
 * ```xml
 *  <view x="0" y="0" w="100%" h="100%" children_layout="default(c=2,r=2,m=5,s=5)">
 *   </view>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如背景颜色等。如：
 *
 * ```xml
 * <style name="default" border_color="#a0a0a0">
 *   <normal     bg_color="#f0f0f0" />
 * </style>
 * ```
 */
typedef struct _view_t {
  widget_t widget;
  /**
   * @property {char*} default_focused_child
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 
   * 缺省获得焦点的子控件(可用控件名或类型)。
   * 
   *> view作为pages/slideview的直接子控件才需要设置。
   *> 正常情况下，一个窗口只能指定一个初始焦点。
   *> 但是对于pages/slideview来说，可能希望每一个页面都有一个初始焦点，此时可用default\_focused\_child来指定。
   */
  char* default_focused_child;
} view_t;

/**
 * @method view_create
 * 创建view对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method view_set_default_focused_child
 * 设置缺省获得焦点的子控件(可用控件名或类型)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  default_focused_child 缺省获得焦点的子控件(可用控件名或类型)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_set_default_focused_child(widget_t* widget, const char* default_focused_child);

/**
 * @method view_cast
 * 转换为view对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget view对象。
 *
 * @return {widget_t*} view对象。
 */
widget_t* view_cast(widget_t* widget);

#define VIEW(widget) ((view_t*)(view_cast(WIDGET(widget))))

END_C_DECLS

#endif /*TK_VIEW_H*/
