/**
 * File:   pages.h
 * Author: AWTK Develop Team
 * Brief:  pages
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
 * 2018-06-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PAGES_H
#define TK_PAGES_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class pages_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 页面管理控件。
 *
 * 只有一个Page处于active状态，处于active状态的Page才能显示并接收事件。
 * 常用于实现标签控件中的页面管理。
 *
 * pages\_t是[widget\_t](widget_t.md)的子类控件，
 * widget\_t的函数均适用于pages\_t控件。
 *
 * 在xml中使用"pages"标签创建页面管理控件。如：
 *
 * ```xml
 * <tab_control x="0" y="0" w="100%" h="100%" >
 *   <pages x="c" y="20" w="90%" h="-60" value="1">
 *   ...
 *   </pages>
 *   <tab_button_group>
 *   ...
 *   </tab_button_group>
 * </tab_control>
 * ```
 *
 * > 更多用法请参考：
 * [tab control](https://github.com/zlgopen/awtk/blob/master/design/default/ui/)
 *
 */
typedef struct _pages_t {
  widget_t widget;
  /**
   * @property {uint32_t} active
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 当前活跃的page。
   */
  uint32_t active;
} pages_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值(当前页)即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值(当前页)改变事件。
 */

/**
 * @method pages_create
 * 创建pages对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* pages_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method pages_cast
 * 转换为pages对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget pages对象。
 *
 * @return {widget_t*} pages对象。
 */
widget_t* pages_cast(widget_t* widget);

/**
 * @method pages_set_active
 * 设置当前的Page。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} index 当前Page的序号。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t pages_set_active(widget_t* widget, uint32_t index);

/**
 * @method pages_set_active_by_name
 * 通过页面的名字设置当前的Page。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} name 当前Page的名字。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t pages_set_active_by_name(widget_t* widget, const char* name);

#define PAGES(widget) ((pages_t*)(pages_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(pages);

END_C_DECLS

#endif /*TK_PAGES_H*/
