/**
 * File:   list_item.h
 * Author: AWTK Develop Team
 * Brief:  list_item
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
 * 2018-07-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LIST_ITEM_H
#define TK_LIST_ITEM_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class list_item_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 列表项控件。
 *
 * 列表项控件是一个简单的容器控件，一般作为列表视图中滚动视图的子控件。
 *
 * list\_item\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于list\_item\_t控件。
 *
 * 在xml中使用"list\_item"标签创建列表项控件。如：
 *
 * ```xml
 * <list_view x="0"  y="30" w="100%" h="-80" item_height="60">
 *   <scroll_view name="view" x="0"  y="0" w="100%" h="100%">
 *     <list_item style="odd" children_layout="default(rows=1,cols=0)">
 *       <image draw_type="icon" w="30" image="earth"/>
 *       <label w="-30" text="1.Hello AWTK !">
 *         <switch x="r:10" y="m" w="60" h="20"/>
 *       </label>
 *     </list_item>
 *     ...
 *   </scroll_view>
 *  </list_view>
 * ```
 *
 * > 更多用法请参考：[list\_view\_m.xml](
 *https://github.com/zlgopen/awtk/blob/master/design/default/ui/list_view_m.xml)
 *
 * 在c代码中使用函数list\_item\_create创建列表项控件。如：
 *
 * ```c
 *  widget_t* list_item = list_item_create(scroll_view, 0, 0, 0, 0);
 * ```
 *
 * > 列表项控件大小一般由列表控制，不需指定xywh参数。
 *
 * 可以用style来实现可点击或不可点击的效果。如：
 *
 * ```xml
 * <style name="odd_clickable" border_color="#a0a0a0"  border="bottom" text_color="black">
 *   <normal     bg_color="#f5f5f5" />
 *   <pressed    bg_color="#c0c0c0" />
 *   <over       bg_color="#f5f5f5" />
 * </style>
 * ```
 *
 * > 更多用法请参考：[theme default](
 *https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L372)
 *
 */
typedef struct _list_item_t {
  widget_t widget;

  /*private*/
  int32_t timer_id;
  bool_t dragged;
  bool_t pressed;
  bool_t downed;
  point_t down;
} list_item_t;

/**
 * @method list_item_create
 * @annotation ["constructor", "scriptable"]
 * 创建list_item对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* list_item_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method list_item_cast
 * 转换为list_item对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget list_item对象。
 *
 * @return {widget_t*} list_item对象。
 */
widget_t* list_item_cast(widget_t* widget);

#define LIST_ITEM(widget) ((list_item_t*)(list_item_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(list_item);

END_C_DECLS

#endif /*TK_LIST_ITEM_H*/
