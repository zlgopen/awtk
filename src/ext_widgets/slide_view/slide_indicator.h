/**
 * File:   slide_indicator.h
 * Author: AWTK Develop Team
 * Brief:  slide_indicator
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
 * 2019-05-29 Xu ChaoZe <xuchaoze@zlg.cn> created
 *
 */

#ifndef TK_SLIDE_INDICATOR_H
#define TK_SLIDE_INDICATOR_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @enum indicator_default_paint_t
 * @prefix INDICATOR_DEFAULT_PAINT_
 * @annotation ["scriptable"]
 * 指示器默认绘制的类型。
 */
typedef enum _indicator_default_paint_t {
  /**
   * @const INDICATOR_DEFAULT_PAINT_AUTO
   * 自动选择。
   */
  INDICATOR_DEFAULT_PAINT_AUTO = 0,
  /**
   * @const INDICATOR_DEFAULT_PAINT_FILL_DOT
   * 实心圆点指示器，当前项填充selected_fg_color，其他填充fg_color。
   */
  INDICATOR_DEFAULT_PAINT_FILL_DOT = 1,
  /**
   * @const INDICATOR_DEFAULT_PAINT_STROKE_DOT
   * 镂空圆点指示器，当前项填充selected_fg_color，其他镂空fg_color。
   */
  INDICATOR_DEFAULT_PAINT_STROKE_DOT = 2,
  /**
   * @const INDICATOR_DEFAULT_PAINT_FILL_RECT
   * 实心矩形指示器，当前项填充selected_fg_color，其他填充fg_color。
   */
  INDICATOR_DEFAULT_PAINT_FILL_RECT = 3,
  /**
   * @const INDICATOR_DEFAULT_PAINT_STROKE_RECT
   * 镂空矩形指示器，当前项填充selected_fg_color，其他镂空fg_color。
   */
  INDICATOR_DEFAULT_PAINT_STROKE_RECT = 4
} indicator_default_paint_t;

/**
 * @class slide_indicator_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * slide_view的指示器控件。
 *
 *> 支持直线、弧线排布，默认有4种绘制样式，若设置了icon/active_icon，则优先使用icon/active_icon
 *
 * slide\_indicator\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于slide\_indicator\_t控件。
 *
 * 在xml中使用"slide\_indicator"或"slide\_indicator\_arc"标签创建指示器控件。如：
 *
 * ```xml
 * <slide_view name="view" x="0"  y="0" w="100%" h="100%">
 * ...
 * </slide_view>
 * <slide_indicator name="indicator" x="right" y="0" w="6" h="100%"/>
 * ```
 *
 * > 更多用法请参考：[slide\_view.xml](
 *https://github.com/zlgopen/awtk/blob/master/design/default/ui/slide_view.xml)
 *
 * 在c代码中使用函数slide\_indicator\_create创建指示器控件。如：
 *
 * ```c
 *  widget_t* slide_indicator = slide_indicator_create(parent, 0, 0, 0, 0);
 * ```
 *
 * ```xml
 * <style name="default">
 *   <normal fg_color="#FFFFFF80" selected_fg_color="#FFFFFF"/>
 * </style>
 * ```
 *
 * > 更多用法请参考：[theme default](
 *https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L350)
 *
 */
typedef struct _slide_indicator_t {
  widget_t widget;

  /**
   * @property {uint32_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值(缺省为0)。
   */
  uint32_t value;

  /**
   * @property {uint32_t} max
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最大值(缺省为100)。
   */
  uint32_t max;

  /**
   * @property {indicator_default_paint_t} default_paint
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 指示器的类型。
   */
  indicator_default_paint_t default_paint;

  /**
   * @property {uint16_t} auto_hide
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 自动隐藏。0表示禁止，非0表示无操作后延迟多久隐藏。
   */
  uint16_t auto_hide;

  /**
   * @property {int32_t} margin
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 指示器与边缘的边距。
   */
  int32_t margin;

  /**
   * @property {float_t} spacing
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 指示器的中心之间的间距（圆弧显示时，间距的单位为弧度，否则为像素）。
   */
  float_t spacing;

  /**
   * @property {uint32_t} size
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 指示器的大小。
   */
  uint32_t size;

  /**
   * @property {float_t} anchor_x
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 锚点x坐标。
   */
  float_t anchor_x;

  /**
   * @property {float_t} anchor_y
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 锚点y坐标。
   */
  float_t anchor_y;

  /**
   * @property {char*} indicated_target
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 指示器指示的目标。
   */
  char* indicated_target;

  /*private*/
  uint8_t inited : 1;
  uint8_t anchor_x_fixed : 1;
  uint8_t anchor_y_fixed : 1;
  uint8_t chilren_indicated : 1;
  widget_animator_t* wa_opactiy;
  widget_t* indicated_widget;
} slide_indicator_t;

/**
 * @method slide_indicator_create
 * 创建slide_indicator对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* slide_indicator_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method slide_indicator_create_linear
 * 创建slide_indicator对象（线性显示）
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* slide_indicator_create_linear(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method slide_indicator_create_arc
 * 创建slide_indicator对象（圆弧显示）
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* slide_indicator_create_arc(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method slide_indicator_cast
 * 转换为slide_indicator对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget slide_indicator对象。
 *
 * @return {widget_t*} slide_indicator对象。
 */
widget_t* slide_indicator_cast(widget_t* widget);

/**
 * @method slide_indicator_set_value
 * 设置当前页的序号。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_indicator对象。
 * @param {uint32_t} value 当前项的序号。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_indicator_set_value(widget_t* widget, uint32_t value);

/**
 * @method slide_indicator_set_max
 * 设置指示器的数量。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_indicator对象。
 * @param {uint32_t} max 数量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_indicator_set_max(widget_t* widget, uint32_t max);

/**
 * @method slide_indicator_set_default_paint
 * 设置指示器的默认绘制类型。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_indicator对象。
 * @param {indicator_default_paint_t} default_paint 默认绘制类型。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_indicator_set_default_paint(widget_t* widget, indicator_default_paint_t default_paint);

/**
 * @method slide_indicator_set_auto_hide
 * 设置指示器是否自动隐藏。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_indicator对象。
 * @param {uint16_t} auto_hide 0表示禁止，非0表示无操作后延迟多久隐藏。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_indicator_set_auto_hide(widget_t* widget, uint16_t auto_hide);

/**
 * @method slide_indicator_set_margin
 * 设置指示器的边距(默认为10像素)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_indicator对象。
 * @param {int32_t} margin 指示器的边距。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_indicator_set_margin(widget_t* widget, int32_t margin);

/**
 * @method slide_indicator_set_spacing
 * 设置指示器的间距(指示器有弧度时为角度值，否则为直线间距)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_indicator对象。
 * @param {float_t} spacing 指示器的间距。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_indicator_set_spacing(widget_t* widget, float_t spacing);

/**
 * @method slide_indicator_set_size
 * 设置指示器的大小(默认为8)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_indicator对象。
 * @param {uint32_t} size 指示器的大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_indicator_set_size(widget_t* widget, uint32_t size);

/**
 * @method slide_indicator_set_anchor
 * 设置旋转锚点。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_indicator对象。
 * @param {const char*} anchor_x 锚点坐标x。(后面加上px为像素点，不加px为相对百分比坐标)
 * @param {const char*} anchor_y 锚点坐标y。(后面加上px为像素点，不加px为相对百分比坐标)
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_indicator_set_anchor(widget_t* widget, const char* anchor_x, const char* anchor_y);

/**
 * @method slide_indicator_set_indicated_target
 * 设置指示器指示的目标。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_indicator对象。
 * @param {const char*} indicated_target 指示器指示的目标
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_indicator_set_indicated_target(widget_t* widget, const char* indicated_target);

#define SLIDE_INDICATOR(widget) ((slide_indicator_t*)(slide_indicator_cast(WIDGET(widget))))
#define SLIDE_INDICATOR_PROP_DEFAULT_PAINT "default_paint"
#define SLIDE_INDICATOR_PROP_AUTO_HIDE "auto_hide"
#define SLIDE_INDICATOR_PROP_INDICATED_TARGET "indicated_target"
#define SLIDE_INDICATOR_PROP_SIZE "size"

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(slide_indicator_linear);
TK_EXTERN_VTABLE(slide_indicator_arc);

END_C_DECLS

#endif /*TK_SLIDE_INDICATOR_H*/
