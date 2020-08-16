/**
 * File:   digit_clock.h
 * Author: AWTK Develop Team
 * Brief:  digit_clock
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
 * 2018-11-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DIGIT_CLOCK_H
#define TK_DIGIT_CLOCK_H

#include "base/widget.h"
#include "tkc/date_time.h"

BEGIN_C_DECLS

/**
 * @class digit_clock_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 数字时钟控件。
 *
 * digit\_clock\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于digit\_clock\_t控件。
 *
 * 在xml中使用"digit\_clock"标签创建数字时钟控件。如：
 *
 * ```xml
 * <digit_clock format="YY/MM/DD h:mm:ss"/>
 * ```
 *
 * > 更多用法请参考：[digit\_clock.xml](
 * https://github.com/zlgopen/awtk/blob/master/design/default/ui/digit_clock.xml)
 *
 * 在c代码中使用函数digit\_clock\_create创建数字时钟控件。如：
 *
 * ```c
 * widget_t* tc = digit_clock_create(win, 10, 10, 240, 30);
 * digit_clock_set_format(tc, "YY/MM/DD h:mm:ss");
 * ```
 *
 * > 完整示例请参考：[digit\_clock demo](
 * https://github.com/zlgopen/awtk-c-demos/blob/master/demos/digit_clock.c)
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 * <style name="default">
 *   <normal text_color="black" />
 * </style>
 * ```
 *
 * > 更多用法请参考：[theme default](
 * https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L138)
 *
 */
typedef struct _digit_clock_t {
  widget_t widget;
  /**
   * @property {char*} format
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 显示格式。
   *
   * * Y 代表年(完整显示)
   * * M 代表月(1-12)
   * * D 代表日(1-31)
   * * h 代表时(0-23)
   * * m 代表分(0-59)
   * * s 代表秒(0-59)
   * * w 代表星期(0-6)
   * * W 代表星期的英文缩写(支持翻译)
   * * YY 代表年(只显示末两位)
   * * MM 代表月(01-12)
   * * DD 代表日(01-31)
   * * hh 代表时(00-23)
   * * mm 代表分(00-59)
   * * ss 代表秒(00-59)
   * * MMM 代表月的英文缩写(支持翻译)
   *
   * 如 日期时间为：2018/11/12 9:10:20
   * * "Y/M/D"显示为"2018/11/12"
   * * "Y-M-D"显示为"2018-11-12"
   * * "Y-M-D h:m:s"显示为"2018-11-12 9:10:20"
   * * "Y-M-D hh:mm:ss"显示为"2018-11-12 09:10:20"
   */
  char* format;

  wstr_t last_time;
} digit_clock_t;

/**
 * @method digit_clock_create
 * 创建digit_clock对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* digit_clock_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method digit_clock_cast
 * 转换为digit_clock对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget digit_clock对象。
 *
 * @return {widget_t*} digit_clock对象。
 */
widget_t* digit_clock_cast(widget_t* widget);

/**
 * @method digit_clock_set_format
 * 设置显示格式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} format 格式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t digit_clock_set_format(widget_t* widget, const char* format);

#define WIDGET_TYPE_DIGIT_CLOCK "digit_clock"

#define DIGIT_CLOCK(widget) ((digit_clock_t*)(digit_clock_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(digit_clock);

/*public for test*/
ret_t digit_clock_format_time(widget_t* widget, const char* format, date_time_t* dt);

END_C_DECLS

#endif /*TK_DIGIT_CLOCK_H*/
