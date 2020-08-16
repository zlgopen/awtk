/**
 * File:   slide_view.h
 * Author: AWTK Develop Team
 * Brief:  slide_view
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
 * 2018-06-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SLIDE_VIEW_H
#define TK_SLIDE_VIEW_H

#include "base/widget.h"
#include "base/velocity.h"

BEGIN_C_DECLS

/**
 * @class slide_view_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 滑动视图。
 *
 * 滑动视图可以管理多个页面，并通过滑动来切换当前页面。也可以管理多张图片，让它们自动切换。
 *
 * slide\_view\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于slide\_view\_t控件。
 *
 * 在xml中使用"slide\_view"标签创建滑动视图控件。如：
 *
 * ```xml
 *   <slide_view x="0" y="0" w="100%" h="100%" style="dot">
 *   <view x="0" y="0" w="100%" h="100%" children_layout="default(w=60,h=60,m=5,s=10)">
 *    ...
 *   </view>
 *   <view x="0" y="0" w="100%" h="100%" children_layout="default(w=60,h=60,m=5,s=10)">
 *    ...
 *   </view>
 *   </slide_view>
 * ```
 *
 * > 更多用法请参考：[slide_view.xml](
 *https://github.com/zlgopen/awtk/blob/master/design/default/ui/slide_view.xml)
 *
 * 在c代码中使用函数slide\_view\_create创建滑动视图控件。如：
 *
 * ```c
 * slide_view = slide_view_create(win, 0, 0, win->w, win->h);
 * ```
 *
 * > 完整示例请参考：
 * [slide_view demo](
 * https://github.com/zlgopen/awtk-c-demos/blob/master/demos/slide_view.c)
 *
 * 可用通过style来设置控件的显示风格，如背景颜色和指示器的图标等等。如：
 *
 * ```xml
 * <style name="dot">
 *   <normal  icon="dot" active_icon="active_dot"/>
 * </style>
 * ```
 *
 * > 如果希望背景图片跟随滚动，请将背景图片设置到页面上，否则设置到slide\_view上。
 *
 * > 更多用法请参考：[theme default](
 * https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L458)
 *
 */
typedef struct _slide_view_t {
  widget_t widget;
  /**
   * @property {bool_t} vertical
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否为上下滑动模式。
   */
  bool_t vertical;

  /**
   * @property {uint16_t} auto_play
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 自动播放。0表示禁止自动播放，非0表示自动播放时每一页播放的时间。
   */
  uint16_t auto_play;

  /**
   * @property {bool_t} loop
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 循环切换模式。
   *
   * 向后切换：切换到最后一页时，再往后切换就到第一页。
   * 向前切换：切换到第一页时，再往前切换就到最后一页。
   */
  bool_t loop;

  /**
   * @property {char*} anim_hint
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 页面切换效果。
   *
   */
  char* anim_hint;

  /*private*/
  point_t down;
  int32_t xoffset;
  int32_t yoffset;
  uint32_t active;
  bool_t animating;
  uint32_t timer_id;
  bool_t dragged;
  bool_t pressed;
  velocity_t velocity;
} slide_view_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值(当前页)即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值(当前页)改变事件。
 */

/**
 * @method slide_view_create
 * 创建slide_view对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* slide_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method slide_view_cast
 * 转换为slide_view对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget slide_view对象。
 *
 * @return {widget_t*} slide_view对象。
 */
widget_t* slide_view_cast(widget_t* widget);

/**
 * @method slide_view_set_auto_play
 * 设置为自动播放模式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_view对象。
 * @param {uint16_t} auto_play 0表示禁止自动播放，非0表示自动播放时每一页播放的时间。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_view_set_auto_play(widget_t* widget, uint16_t auto_play);

/**
 * @method slide_view_set_active
 * 设置当前页的序号。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_view对象。
 * @param {uint32_t} index 当前页的序号。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_view_set_active(widget_t* widget, uint32_t index);

/**
 * @method slide_view_set_vertical
 * 设置为上下滑动(缺省为左右滑动)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_view对象。
 * @param {bool_t} vertical TRUE表示上下滑动，FALSE表示左右滑动。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_view_set_vertical(widget_t* widget, bool_t vertical);

/**
 * @method slide_view_set_anim_hint
 * 设置页面切换动画。
 *
 * anim_hint取值如下：
 *
 * * "translate"：平移。
 * * "overlap"：覆盖。
 * * "overlap\_with\_alpha"：覆盖并改变透明度。
 *
 *> 使用"overlap"或"overlap\_with\_alpha"动画时，背景图片最好指定到page上。
 *>
 *> 使用"overlap\_with\_alpha"动画时，slideview的背景设置为黑色，
 *> 或slideview的背景设置为透明，窗口的背景设置为黑色，以获得更好的视觉效果和性能。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_view对象。
 * @param {const char*} anim_hint 页面切换动画。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_view_set_anim_hint(widget_t* widget, const char* anim_hint);

/**
 * @method slide_view_set_loop
 * 设置循环切换模式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_view对象。
 * @param {bool_t} loop 是否启用循环切换模式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_view_set_loop(widget_t* widget, bool_t loop);

/*public for test only*/
widget_t* slide_view_get_prev(slide_view_t* slide_view);
widget_t* slide_view_get_next(slide_view_t* slide_view);
ret_t slide_view_activate_prev(slide_view_t* slide_view);
ret_t slide_view_activate_next(slide_view_t* slide_view);

#define SLIDE_VIEW(widget) ((slide_view_t*)(slide_view_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(slide_view);

END_C_DECLS

#endif /*TK_SLIDE_VIEW_H*/
