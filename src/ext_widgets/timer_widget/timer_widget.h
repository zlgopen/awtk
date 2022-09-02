/**
 * File:   timer_widget.h
 * Author: AWTK Develop Team
 * Brief:  定时器。
 *
 * Copyright (c) 2022 - 2022 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-07-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TIMER_WIDGET_H
#define TK_TIMER_WIDGET_H

#include "base/widget.h"

BEGIN_C_DECLS
/**
 * @class timer_widget_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 定时器。
 * > 主要目的是方便以拖拽的方式创建定时器，并用AWBlock编写简单的事件处理程序。
 * 在xml中使用"timer"标签创建控件。如：
 *
 * ```xml
 * <!-- ui -->
 * <timer x="c" y="50" w="100" h="100" duration="1000"/>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 * > 本控件默认不可见，无需指定style。
 * 
 * ```xml
 * <!-- style -->
 * <timer>
 *   <style name="default" font_size="32">
 *     <normal text_color="black" />
 *   </style>
 * </timer>
 * ```
 */
typedef struct _timer_widget_t {
  widget_t widget;

  /**
   * @property {uint32_t} duration
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 时长(ms)。
   */
  uint32_t duration;

  /*private*/
  uint32_t timer_id;
} timer_widget_t;

/**
 * @event {event_t} EVT_TIMER
 * 定时器事件。
 */

/**
 * @method timer_widget_create
 * @annotation ["constructor", "scriptable"]
 * 创建timer_widget对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} timer_widget对象。
 */
widget_t* timer_widget_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method timer_widget_cast
 * 转换为timer_widget对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget timer_widget对象。
 *
 * @return {widget_t*} timer_widget对象。
 */
widget_t* timer_widget_cast(widget_t* widget);

/**
 * @method timer_widget_set_duration
 * 设置 时长(ms)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} duration 时长(ms)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_widget_set_duration(widget_t* widget, uint32_t duration);

#define TIMER_WIDGET_PROP_DURATION "duration"

#define WIDGET_TYPE_TIMER_WIDGET "timer"

#define TIMER_WIDGET(widget) ((timer_widget_t*)(timer_widget_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(timer_widget);

END_C_DECLS

#endif /*TK_TIMER_WIDGET_H*/
