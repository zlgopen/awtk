/**
 * File:   lang_indicator.h
 * Author: AWTK Develop Team
 * Brief:  lang_indicator
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
 * 2020-04-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LANG_INDICATOR_H
#define TK_LANG_INDICATOR_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class lang_indicator_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 
 * 输入法语言指示器。
 * 
 * 用于显示输入法的输入语言或类型，主要用于T9输入法。
 * 
 * lang_indicator\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于lang_indicator\_t控件。
 *
 * 在xml中使用"lang_indicator"标签创建lang_indicator。如：
 *
 * ```xml
 *  <lang_indicator x="0" y="0" w="100%" h="100%" children_layout="default(c=2,r=2,m=5,s=5)">
 *   </lang_indicator>
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
typedef struct _lang_indicator_t {
  widget_t widget;
  /**
   * @property {char*} image
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 
   * 如果希望用图片格式显示，本属性用于指定图片的前缀。
   * 
   */
  char* image;

  /*private*/
  uint32_t event_id;
} lang_indicator_t;

/**
 * @method lang_indicator_create
 * 创建lang_indicator对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* lang_indicator_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method lang_indicator_set_image
 * 设置缺省获得焦点的子控件(可用控件名或类型)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  image 缺省获得焦点的子控件(可用控件名或类型)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lang_indicator_set_image(widget_t* widget, const char* image);

/**
 * @method lang_indicator_cast
 * 转换为lang_indicator对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget lang_indicator对象。
 *
 * @return {widget_t*} lang_indicator对象。
 */
widget_t* lang_indicator_cast(widget_t* widget);

#define WIDGET_TYPE_LANG_INDICATOR "lang_indicator"
#define LANG_INDICATOR(widget) ((lang_indicator_t*)(lang_indicator_cast(WIDGET(widget))))

END_C_DECLS

#endif /*TK_LANG_INDICATOR_H*/
