/*
 * File:   clip_view.h
 * Author: AWTK Develop Team
 * Brief:  clip view
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
* 2019-12-12 Lou ZhiMing <louzhiming@zlg.cn> created
 *
 */

#ifndef TK_CLIP_VIEW_H
#define TK_CLIP_VIEW_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class clip_view_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 一个裁剪子控件的容器控件。
 *
 * 它本身不提供布局功能，仅提供具有语义的标签，让xml更具有可读性。
 * 子控件的布局可用layout\_children属性指定。
 * 请参考[布局参数](https://github.com/zlgopen/awtk/blob/master/docs/layout.md)。
 *
 * clip\_view\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于clip\_view\_t控件。
 *
 * 在xml中使用"clip_view"标签创建clip_view，在clip_view控件下的所有子控件都会被裁剪。如下button控件会被裁剪，无法画出clip_view控件 ：
 *
 * ```xml
 *  <clip_view x="0" y="0" w="100" h="100">
 *    <button x="50" y="10" w="100" h="50" />  
 *  </clip_view>
 * ```
 *
 * 备注：在clip_view控件下的所有子控件都会被裁剪，如果子控件本身会设置裁剪区的话，在子控件中计算裁剪区的交集，具体请参考scroll_view控件的scroll_view_on_paint_children函数。
 * 
 * 可用通过style来设置控件的显示风格，如背景颜色等。如：
 *
 * ```xml
 * <style name="default" border_color="#a0a0a0">
 *   <normal     bg_color="#f0f0f0" />
 * </style>
 * ```
 */

typedef struct _clip_view_t {
  widget_t widget;
} clip_view_t;

/**
 * @method clip_view_create
 * 创建clip_view对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* clip_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method clip_view_cast
 * 转换为clip_view对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget clip_view对象。
 *
 * @return {widget_t*} clip_view对象。
 */
widget_t* clip_view_cast(widget_t* widget);

#define WIDGET_TYPE_CLIP_VIEW "clip_view"

#define CLIP_VIEW(widget) ((clip_view_t*)(widget))

END_C_DECLS

#endif /*TK_CLIP_VIEW_H*/
