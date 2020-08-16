/**
 * File:   image_value.h
 * Author: AWTK Develop Team
 * Brief:  image_value
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
 * 2018-12-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IMAGE_VALUE_H
#define TK_IMAGE_VALUE_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class image_value_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 图片值控件。
 *
 * 可以用图片来表示如电池电量、WIFI信号强度和其它各种数值的值。
 *
 * 其原理如下：
 *
 *  * 1.把value以format为格式转换成字符串。
 *  * 2.把每个字符与image(图片文件名前缀)映射成一个图片名。
 *  * 3.最后把这些图片显示出来。
 *
 * 如果设置click\_add\_delta为非0，那么点击时自动增加指定的增量，值超过最大值时回到最小值, 
 * 或者值超过最小值时回到最大值。
 *
 *image\_value\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于image\_value\_t控件。
 *
 *在xml中使用"image\_value"标签创建图片值控件。如：
 *
 * ```xml
 * <image_value  value="0" image="num_" />
 * ```
 *
 * > 更多用法请参考：
 * [image\_value](https://github.com/zlgopen/awtk/blob/master/design/default/ui/image_value.xml)
 *
 * 在c代码中使用函数image\_value\_create创建图片值控件。如：
 *
 * ```c
 * image_value = image_value_create(win, 10, 10, 200, 200);
 * image_value_set_image(image_value, "num_");
 * image_value_set_value(image_value, 100);
 * ```
 *
 * > 完整示例请参考：
 * [image_value demo](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/image_value.c)
 *
 * 可用通过style来设置控件的显示风格，如背景颜色和边框等等，不过一般情况并不需要。
 *
 */
typedef struct _image_value_t {
  widget_t widget;

  /**
   * @property {char*} image
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 图片名称的前缀。
   */
  char* image;

  /**
   * @property {char*} format
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 数值到字符串转换时的格式，缺省为"%d"。
   */
  char* format;

  /**
   * @property {float_t} click_add_delta
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 点击时加上一个增量。
   */
  float_t click_add_delta;

  /**
   * @property {float_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值。
   */
  float_t value;

  /**
   * @property {float_t} min
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最小值(如果设置了click\_add\_delta，到达最小值后回到最大值)。
   */
  float_t min;

  /**
   * @property {float_t} max
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最大值(如果设置了click\_add\_delta，到达最大值后回到最小值)。
   */
  float_t max;

  /*private*/
  bool_t pressed;
} image_value_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值即将改变事件(click_add_delta为非0时，点击触发)。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值改变事件(click_add_delta为非0时，点击触发)。
 */

/**
 * @method image_value_create
 * 创建image_value对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* image_value_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method image_value_set_image
 * 设置图片前缀。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_value对象。
 * @param {const char*} image 图片前缀。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_value_set_image(widget_t* widget, const char* image);

/**
 * @method image_value_set_format
 * 设置格式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_value对象。
 * @param {const char*} format 格式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_value_set_format(widget_t* widget, const char* format);

/**
 * @method image_value_set_click_add_delta
 * 设置点击时加上的增量。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_value对象。
 * @param {float_t} delta 增量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_value_set_click_add_delta(widget_t* widget, float_t click_add_delta);

/**
 * @method image_value_set_value
 * 设置值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_value对象。
 * @param {float_t} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_value_set_value(widget_t* widget, float_t value);

/**
 * @method image_value_set_min
 * 设置最小值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_min对象。
 * @param {float_t} min 最小值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_value_set_min(widget_t* widget, float_t min);

/**
 * @method image_value_set_max
 * 设置最大值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_max对象。
 * @param {float_t} max 最大值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_value_set_max(widget_t* widget, float_t max);

/**
 * @method image_value_cast
 * 转换为image_value对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget image_value对象。
 *
 * @return {widget_t*} image_value对象。
 */
widget_t* image_value_cast(widget_t* widget);

#define WIDGET_TYPE_IMAGE_VALUE "image_value"

/* "."不是有效的文件名，所以把字符"."映射成"dot" */
#define IMAGE_VALUE_MAP_DOT "dot"
/* "/"不是有效的文件名，所以把字符"/"映射成"slash" */
#define IMAGE_VALUE_MAP_SLASH "slash"

#define WIDGET_PROP_CLICK_ADD_DELTA "click_add_delta"

#ifndef IMAGE_VALUE_MAX_CHAR_NR
#define IMAGE_VALUE_MAX_CHAR_NR 8
#endif /*IMAGE_VALUE_MAX_CHAR_NR*/

#define IMAGE_VALUE(widget) ((image_value_t*)(image_value_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(image_value);

/*public for test*/
ret_t image_value_add_delta(widget_t* widget);

END_C_DECLS

#endif /*TK_IMAGE_VALUE_H*/
