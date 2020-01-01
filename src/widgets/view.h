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
 * @method view_cast
 * 转换为view对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget view对象。
 *
 * @return {widget_t*} view对象。
 */
widget_t* view_cast(widget_t* widget);

END_C_DECLS

#endif /*TK_VIEW_H*/
