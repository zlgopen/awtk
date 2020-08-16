/**
 * File:   hscroll_label.h
 * Author: AWTK Develop Team
 * Brief:  hscroll_label
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
 * 2019-07-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_HSCROLL_LABEL_H
#define TK_HSCROLL_LABEL_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class hscroll_label_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 *
 * 可水平滚动的文本控件，方便实现长文本滚动。
 *
 *
 hscroll\_label\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于hscroll\_label\_t控件。
 *
 * 在xml中使用"hscroll\_label"标签创建行号控件，一般配合mledit使用。如：
 *
 * ```xml
 * ```
 *
 * > 更多用法请参考：[mledit.xml](
 *https://github.com/zlgopen/awtk/blob/master/design/default/ui/mledit.xml)
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
  <style name="default" text_color="black">
    <normal   />
    <focused  />
  </style>
 * ```
 *
 * > 更多用法请参考：
 * [theme default](
 *https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml)
 *
 */
typedef struct _hscroll_label_t {
  widget_t widget;

  /**
   * @property {bool_t} only_focus
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 只有处于focus时才滚动(缺省否)。
   */
  bool_t only_focus;

  /**
   * @property {bool_t} only_parent_focus
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 只有父控件处于focus时才滚动(缺省否)。
   */
  bool_t only_parent_focus;

  /**
   * @property {bool_t} loop
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * loop是否循环滚动(缺省FALSE)。
   */
  bool_t loop;

  /**
   * @property {bool_t} yoyo
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否往返滚动(缺省FALSE)。
   */
  bool_t yoyo;

  /**
   * @property {bool_t} ellipses
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 文本显示不下时，在行尾显示省略号(缺省FALSE)。
   */
  bool_t ellipses;

  /**
   * @property {int32_t} lull
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 滚动之间的间歇时间(ms)，缺省3000ms。
   */
  int32_t lull;

  /**
   * @property {int32_t} duration
   * @annotation ["set_prop","get_prop","readable","scriptable"]
   * 完整的滚动一次需要的时间(ms)，缺省5000ms。
   */
  int32_t duration;

  /**
   * @property {int32_t} xoffset
   * @annotation ["set_prop","get_prop","readable","scriptable"]
   * 偏移量。
   */
  int32_t xoffset;

  /**
   * @property {int32_t} text_w
   * @annotation ["set_prop","get_prop","readable","scriptable"]
   * 文本的宽度。
   */
  int32_t text_w;

  /*private*/
  uint32_t timer_id;
  uint32_t elapsed;
  bool_t paused;
  bool_t pressed;
  bool_t reversed;
} hscroll_label_t;

/**
 * @method hscroll_label_create
 * @annotation ["constructor", "scriptable"]
 * 创建hscroll_label对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* hscroll_label_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method hscroll_label_set_lull
 * 设置lull。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  lull 间歇时间(ms)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hscroll_label_set_lull(widget_t* widget, int32_t lull);

/**
 * @method hscroll_label_set_duration
 * 设置duration。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  duration 滚动时间(ms)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hscroll_label_set_duration(widget_t* widget, int32_t duration);

/**
 * @method hscroll_label_set_only_focus
 * 设置only_focus。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  only_focus 是否只有处于focus时才滚动。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hscroll_label_set_only_focus(widget_t* widget, bool_t only_focus);

/**
 * @method hscroll_label_set_only_parent_focus
 * 设置only_parent_focus。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  only_parent_focus 是否只有处于focus时才滚动。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hscroll_label_set_only_parent_focus(widget_t* widget, bool_t only_parent_focus);

/**
 * @method hscroll_label_set_loop
 * 设置loop。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  loop 是否循环滚动。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hscroll_label_set_loop(widget_t* widget, bool_t loop);

/**
 * @method hscroll_label_set_yoyo
 * 设置yoyo。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  yoyo 是否往返滚动。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hscroll_label_set_yoyo(widget_t* widget, bool_t yoyo);

/**
 * @method hscroll_label_set_ellipses
 * 设置ellipses。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  ellipses 是否在文本超长时在行尾显示"..."。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hscroll_label_set_ellipses(widget_t* widget, bool_t ellipses);

/**
 * @method hscroll_label_set_xoffset
 * 设置x偏移(一般无需用户调用)。。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  xoffset x偏移。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hscroll_label_set_xoffset(widget_t* widget, int32_t xoffset);

/**
 * @method hscroll_label_start
 * 启动(一般无需用户调用)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hscroll_label_start(widget_t* widget);

/**
 * @method hscroll_label_stop
 * 停止(一般无需用户调用)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hscroll_label_stop(widget_t* widget);

/**
 * @method hscroll_label_cast
 * 转换为hscroll_label对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget hscroll_label对象。
 *
 * @return {widget_t*} hscroll_label对象。
 */
widget_t* hscroll_label_cast(widget_t* widget);

#define HSCROLL_LABEL_PROP_YOYO "yoyo"
#define HSCROLL_LABEL_PROP_LOOP "loop"
#define HSCROLL_LABEL_PROP_LULL "lull"
#define HSCROLL_LABEL_PROP_XOFFSET "xoffset"
#define HSCROLL_LABEL_PROP_DURATION "duration"
#define HSCROLL_LABEL_PROP_ELLIPSES "ellipses"
#define HSCROLL_LABEL_PROP_ONLY_FOCUS "only_focus"
#define HSCROLL_LABEL_PROP_ONLY_PARENT_FOCUS "only_parent_focus"

#define WIDGET_TYPE_HSCROLL_LABEL "hscroll_label"
#define HSCROLL_LABEL(widget) ((hscroll_label_t*)(hscroll_label_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(hscroll_label);

END_C_DECLS

#endif /*TK_HSCROLL_LABEL_H*/
