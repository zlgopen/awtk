/**
 * File:   draggable.h
 * Author: AWTK Develop Team
 * Brief:  make parent widget or window draggable
 *
 * Copyright (c) 2019 - 2020 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-10-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DRAGGABLE_H
#define TK_DRAGGABLE_H

#include "base/widget.h"

BEGIN_C_DECLS
/**
 * @class draggable_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 *
 * 将draggable放入目标控件，即可让目标控件或当前窗口可以被拖动。
 *
 * draggable\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于draggable\_t控件。
 *
 * 在xml中使用"draggable"标签创建draggable控件。如：
 *
 * ```xml
 *  <button text="Drag Me" w="80" h="40" x="10" y="10">
 *   <draggable />
 *  </button>
 * ```
 *
 * 拖动对话框标题时移动对话框：
 *
 * ```xml
 * <dialog_title x="0" y="0" w="100%" h="30" text="Hello AWTK" >
 *   <draggable drag_window="true"/>
 * </dialog_title>
 * ```
 *
 * > 更多用法请参考：
 * [draggable.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/draggable.xml)
 *
 * 在c代码中使用函数draggable\_create创建按钮控件。如：
 *
 * ```c
 *  widget_t* draggable = draggable_create(target, 0, 0, 0, 0);
 * ```
 *
 * > draggable本身不可见，故无需style。
 *
 */
typedef struct _draggable_t {
  widget_t widget;

  /**
   * @property {int32_t} top
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 拖动范围的顶部限制。缺省为父控件的顶部。
   */
  int32_t top;

  /**
   * @property {int32_t} bottom
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 拖动范围的底部限制。缺省为父控件的底部。
   */
  int32_t bottom;

  /**
   * @property {int32_t} left
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 拖动范围的左边限制。缺省为父控件的左边。
   */
  int32_t left;

  /**
   * @property {int32_t} right
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 拖动范围的右边限制。缺省为父控件的右边边。
   */
  int32_t right;

  /**
   * @property {bool_t} vertical_only
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 只允许垂直拖动。
   */
  bool_t vertical_only;

  /**
   * @property {bool_t} horizontal_only
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 只允许水平拖动。
   */
  bool_t horizontal_only;

  /**
   * @property {bool_t} drag_window
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 拖动窗口而不是父控件。比如放在对话框的titlebar上，拖动titlebar其实是希望拖动对话框。
   */
  bool_t drag_window;

  /*private*/
  bool_t pressed;
  point_t down;
  point_t saved_position;
} draggable_t;

/**
 * @method draggable_create
 * @annotation ["constructor", "scriptable"]
 * 创建draggable对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* draggable_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method draggable_cast
 * 转换为draggable对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget draggable对象。
 *
 * @return {widget_t*} draggable对象。
 */
widget_t* draggable_cast(widget_t* widget);

/**
 * @method draggable_set_top
 * 设置top。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} top 拖动范围的顶部限制。缺省为父控件的顶部。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t draggable_set_top(widget_t* widget, int32_t top);

/**
 * @method draggable_set_bottom
 * 设置bottom。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} bottom 拖动范围的底部限制。缺省为父控件的底部。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t draggable_set_bottom(widget_t* widget, int32_t bottom);

/**
 * @method draggable_set_left
 * 设置left。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} left 拖动范围的左边限制。缺省为父控件的左边。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t draggable_set_left(widget_t* widget, int32_t left);

/**
 * @method draggable_set_right
 * 设置right。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} right 拖动范围的右边限制。缺省为父控件的右边边。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t draggable_set_right(widget_t* widget, int32_t right);

/**
 * @method draggable_set_vertical_only
 * 设置vertical_only。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} vertical_only 只允许垂直拖动。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t draggable_set_vertical_only(widget_t* widget, bool_t vertical_only);

/**
 * @method draggable_set_horizontal_only
 * 设置horizontal_only。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} horizontal_only 只允许水平拖动。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t draggable_set_horizontal_only(widget_t* widget, bool_t horizontal_only);

/**
 * @method draggable_set_drag_window
 * 设置drag_window。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} drag_window drag_window
 * 拖动窗口而不是父控件。比如放在对话框的titlebar上，拖动titlebar其实是希望拖动对话框。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t draggable_set_drag_window(widget_t* widget, bool_t drag_window);

#define DRAGGABLE_PROP_TOP "top"
#define DRAGGABLE_PROP_BOTTOM "bottom"
#define DRAGGABLE_PROP_LEFT "left"
#define DRAGGABLE_PROP_RIGHT "right"
#define DRAGGABLE_PROP_DRAG_WINDOW "drag_window"
#define DRAGGABLE_PROP_VERTICAL_ONLY "vertical_only"
#define DRAGGABLE_PROP_HORIZONTAL_ONLY "horizontal_only"

#define WIDGET_TYPE_DRAGGABLE "draggable"
#define DRAGGABLE_UNSPECIFIED_NUM 0x1fffffff

#define DRAGGABLE(widget) ((draggable_t*)(draggable_cast(WIDGET(widget))))

ret_t draggable_register(void);

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(draggable);

END_C_DECLS

#endif /*TK_DRAGGABLE_H*/
