/**
 * File:   widget_vtable.h
 * Author: AWTK Develop Team
 * Brief:  widget vtable default impl
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_VTABLE_H
#define TK_WIDGET_VTABLE_H

#include "base/widget.h"

BEGIN_C_DECLS

/*widget 虚函数的缺省实现，用于辅助实现子控件*/

#define WIDGET_VTABLE_GET_VTABLE(name) name##_get_widget_vtable()

/**
 * @method widget_invalidate_default
 * awtk默认的invalidate接口。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 控件。
 * @param {const rect_t*} rect 无效化区域。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_invalidate_default(widget_t* widget, const rect_t* rect);

/**
 * @method widget_on_event_default
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 控件。
 * @param {event_t*} e 事件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_event_default(widget_t* widget, event_t* e);

/**
 * @method widget_on_paint_null
 * 空的paint函数。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t widget_on_paint_null(widget_t* widget, canvas_t* c);

/**
 * @method widget_on_paint_self_default
 * 绘制控件自身的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_paint_self_default(widget_t* widget, canvas_t* c);

/**
 * @method widget_on_paint_children_clip
 * 绘制子控件带clip功能的实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_paint_children_clip(widget_t* widget, canvas_t* c);

/**
 * @method widget_paint_with_clip
 * 根据当前widget进行clip，然后调用on_paint回调函数进行绘制。
 *
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {rect_t*} clip clip区域(为NULL时使用widget的区域)。
 * @param {canvas_t*} c 画布对象。
 * @param {widget_on_paint_t} on_paint 绘制的回调函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_paint_with_clip(widget_t* widget, rect_t* clip, canvas_t* c,
                             widget_on_paint_t on_paint);

/**
 * @method widget_on_paint_children_default
 * 绘制子控件的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_paint_children_default(widget_t* widget, canvas_t* c);

/**
 * @method widget_on_keydown_default
 * 控件按键按下事件回调的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {key_event_t*} e 按键事件。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_keydown_default(widget_t* widget, key_event_t* e);

/**
 * @method widget_on_keyup_default
 * 控件按键抬起事件回调的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {key_event_t*} e 按键事件。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_keyup_default(widget_t* widget, key_event_t* e);
ret_t widget_on_click_default(widget_t* widget, pointer_event_t* e);

/**
 * @method widget_on_pointer_down_default
 * 控件指针按下事件回调的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {pointer_event_t*} e 指针事件。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_pointer_down_default(widget_t* widget, pointer_event_t* e);

/**
 * @method widget_on_pointer_move_default
 * 控件指针移动事件回调的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {pointer_event_t*} e 指针事件。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_pointer_move_default(widget_t* widget, pointer_event_t* e);

/**
 * @method widget_on_pointer_up_default
 * 控件指针抬起事件回调的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {pointer_event_t*} e 指针事件。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_pointer_up_default(widget_t* widget, pointer_event_t* e);

/**
 * @method widget_on_copy_default
 * 拷贝控件的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 目的控件对象。
 * @param {widget_t*} other 源控件对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_copy_default(widget_t* widget, widget_t* other);

/**
 * @method widget_on_copy_recursive
 * 拷贝控件的递归实现（逐级递归基类控件）。
 * @annotation ["global"]
 * @param {widget_t*} widget 目的控件对象。
 * @param {widget_t*} other 源控件对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_copy_recursive(widget_t* widget, widget_t* other);

/**
 * @method widget_copy_props
 * 拷贝控件属性。
 * @annotation ["global"]
 * @param {widget_t*} dst 目的控件对象。
 * @param {widget_t*} src 源控件对象。
 * @param {const char* const*} properties 属性列表。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_copy_props(widget_t* dst, widget_t* src, const char* const* properties);

/**
 * @method widget_get_prop_default
 * 获取控件属性的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性名称。
 * @param {value_t*} v 返回的属性值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_get_prop_default(widget_t* widget, const char* name, value_t* v);

/**
 * @method widget_set_prop_default
 * 设置控件属性的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性名称。
 * @param {const value_t*} v 设置的属性值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_prop_default(widget_t* widget, const char* name, const value_t* v);

/**
 * @method widget_find_target_default
 * awtk默认的find_target接口。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 控件。
 * @param {xy_t} x 坐标。
 * @param {xy_t} y 坐标。
 *
 * @return {widget_t*} 返回响应的控件。
 */
widget_t* widget_find_target_default(widget_t* widget, xy_t x, xy_t y);

/**
 * @method widget_vtable_init
 * 动态初始化虚表。
 * 
 * 备注：
 * 初始化的时候，虚表会自动继承父类的所有属性。
 * @annotation ["global"]
 *
 * @param {widget_vtable_t*} vt 子类虚表。
 * @param {const widget_vtable_t*} parent 被继承的父类虚表。
 *
 * @return {widget_vtable_t*} 成功返回虚表，失败返回 NULL。
 */
widget_vtable_t* widget_vtable_init(widget_vtable_t* vt, const widget_vtable_t* parent);

/**
 * @method widget_get_parent_vtable
 * 获取父类的虚表。
 * 
 * @annotation ["global"]
 *
 * @param {const widget_vtable_t*} vt 子类的虚表。
 *
 * @return {const widget_vtable_t*} 成功返回虚表，失败返回 NULL。
 */
const widget_vtable_t* widget_get_parent_vtable(const widget_vtable_t* vt);

/**
 * @method widget_set_self_vtable
 * 设置控件的虚表。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 控件。
 * @param {const widget_vtable_t*} vt 新的虚表。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_self_vtable(widget_t* widget, const widget_vtable_t* vt);

/**
 * @method widget_get_self_vtable
 * 获取控件的虚表。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 控件。
 *
 * @return {const widget_vtable_t*} 成功返回虚表，失败返回 NULL。
 */
const widget_vtable_t* widget_get_self_vtable(widget_t* widget);

/**
 * @method widget_vtable_get_prop_by_parent
 * 调用子类控件的父类的 get_prop 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} v 返回属性的值。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_get_prop_by_parent(widget_t* widget, const char* name, value_t* v,
                                       const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_set_prop_by_parent
 * 调用子类控件的父类的 set_prop 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {const char*} name 属性的名称。
 * @param {const value_t*} v 属性的值。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_set_prop_by_parent(widget_t* widget, const char* name, const value_t* v,
                                       const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_invalidate_by_parent
 * 调用子类控件的父类的 invalidate 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {const rect_t*} r 矩形对象(widget本地坐标)。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_invalidate_by_parent(widget_t* widget, const rect_t* r,
                                         const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_find_target_by_parent
 * 调用子类控件的父类的 find_target 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 * @param {widget_t**} ret_widget 返回查找到的控件。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_find_target_by_parent(widget_t* widget, xy_t x, xy_t y, widget_t** ret_widget,
                                          const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_is_point_in_by_parent
 * 调用子类控件的父类的 is_point_in 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 * @param {bool_t*} ret_is_point_in 返回判断结果。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_is_point_in_by_parent(widget_t* widget, xy_t x, xy_t y, bool_t* ret_is_point_in,
                                          const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_get_offset_by_parent
 * 调用子类控件的父类的 get_offset 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {xy_t*} out_x 返回x坐标。
 * @param {xy_t*} out_y 返回y坐标。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_get_offset_by_parent(widget_t* widget, xy_t* out_x, xy_t* out_y,
                                         const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_auto_adjust_size_by_parent
 * 调用子类控件的父类的 auto_adjust_size 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_auto_adjust_size_by_parent(widget_t* widget, const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_get_prop_default_value_by_parent
 * 调用子类控件的父类的 get_prop_default_value 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} v 返回属性的值。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_get_prop_default_value_by_parent(widget_t* widget, const char* name, value_t* v,
                                                     const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_get_only_active_children_by_parent
 * 调用子类控件的父类的 get_only_active_children 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {darray_t*} all_focusable 返回active的子控件列表。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_get_only_active_children_by_parent(widget_t* widget, darray_t* all_focusable,
                                                       const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_copy_by_parent
 * 调用子类控件的父类的 on_copy 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {widget_t*} other 源控件对象。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_copy_by_parent(widget_t* widget, widget_t* other,
                                      const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_keyup_by_parent
 * 调用子类控件的父类的 on_keyup 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {key_event_t*} e 按键事件。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_keyup_by_parent(widget_t* widget, key_event_t* e,
                                       const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_keydown_by_parent
 * 调用子类控件的父类的 on_keydown 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {key_event_t*} e 按键事件。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_keydown_by_parent(widget_t* widget, key_event_t* e,
                                         const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_wheel_by_parent
 * 调用子类控件的父类的 on_wheel 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {wheel_event_t*} e 滚轮事件。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_wheel_by_parent(widget_t* widget, wheel_event_t* e,
                                       const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_multi_gesture_by_parent
 * 调用子类控件的父类的 on_multi_gesture 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {multi_gesture_event_t*} e 多点触控事件。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_multi_gesture_by_parent(widget_t* widget, multi_gesture_event_t* e,
                                               const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_re_translate_by_parent
 * 调用子类控件的父类的 on_re_translate 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_re_translate_by_parent(widget_t* widget, const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_paint_background_by_parent
 * 调用子类控件的父类的 on_paint_background 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {canvas_t*} c 画布句柄。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_paint_background_by_parent(widget_t* widget, canvas_t* c,
                                                  const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_paint_self_by_parent
 * 调用子类控件的父类的 on_paint_self 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {canvas_t*} c 画布句柄。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_paint_self_by_parent(widget_t* widget, canvas_t* c,
                                            const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_paint_children_by_parent
 * 调用子类控件的父类的 on_paint_children 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {canvas_t*} c 画布句柄。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_paint_children_by_parent(widget_t* widget, canvas_t* c,
                                                const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_paint_border_by_parent
 * 调用子类控件的父类的 on_paint_border 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {canvas_t*} c 画布句柄。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_paint_border_by_parent(widget_t* widget, canvas_t* c,
                                              const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_paint_begin_by_parent
 * 调用子类控件的父类的 on_paint_begin 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {canvas_t*} c 画布句柄。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_paint_begin_by_parent(widget_t* widget, canvas_t* c,
                                             const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_paint_end_by_parent
 * 调用子类控件的父类的 on_paint_end 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {canvas_t*} c 画布句柄。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_paint_end_by_parent(widget_t* widget, canvas_t* c,
                                           const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_pointer_down_by_parent
 * 调用子类控件的父类的 on_pointer_down 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {pointer_event_t*} e 指针事件。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_pointer_down_by_parent(widget_t* widget, pointer_event_t* e,
                                              const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_pointer_move_by_parent
 * 调用子类控件的父类的 on_pointer_move 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {pointer_event_t*} e 指针事件。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_pointer_move_by_parent(widget_t* widget, pointer_event_t* e,
                                              const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_pointer_up_by_parent
 * 调用子类控件的父类的 on_pointer_up 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {pointer_event_t*} e 指针事件。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_pointer_up_by_parent(widget_t* widget, pointer_event_t* e,
                                            const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_context_menu_by_parent
 * 调用子类控件的父类的 on_context_menu 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {pointer_event_t*} e 指针事件。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_context_menu_by_parent(widget_t* widget, pointer_event_t* e,
                                              const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_layout_children_by_parent
 * 调用子类控件的父类的 on_layout_children 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_layout_children_by_parent(widget_t* widget, const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_add_child_by_parent
 * 调用子类控件的父类的 on_add_child 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {widget_t*} child 添加控件对象。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_add_child_by_parent(widget_t* widget, widget_t* child,
                                           const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_remove_child_by_parent
 * 调用子类控件的父类的 on_remove_child 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {widget_t*} child 移除控件对象。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_remove_child_by_parent(widget_t* widget, widget_t* child,
                                              const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_attach_parent_by_parent
 * 调用子类控件的父类的 on_attach_parent 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {widget_t*} parent 父控件对象。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_attach_parent_by_parent(widget_t* widget, widget_t* parent,
                                               const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_detach_parent_by_parent
 * 调用子类控件的父类的 on_detach_parent 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {widget_t*} parent 移除父控件对象。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_detach_parent_by_parent(widget_t* widget, widget_t* parent,
                                               const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_event_by_parent
 * 调用子类控件的父类的 on_event 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {event_t*} e 消息事件。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_event_by_parent(widget_t* widget, event_t* e,
                                       const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_event_before_children_by_parent
 * 调用子类控件的父类的 on_event_before_children 函数。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {event_t*} e 消息事件。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_event_before_children_by_parent(widget_t* widget, event_t* e,
                                                       const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_on_destroy_by_parent
 * 调用子类控件的父类的 on_destroy 函数。
 * 备注：通知父类触发释放事件。
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_on_destroy_by_parent(widget_t* widget, const widget_vtable_t* curr_vt);

/**
 * @method widget_vtable_init_by_parent
 * 调用子类控件的父类的 init 函数。
 * 备注：用于父类的属性初始化。
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 子类控件对象。
 * @param {const widget_vtable_t*} curr_vt 调用类的虚表对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，返回 RET_NOT_IMPL 表示父类不支持该函数，否则表示失败。
 */
ret_t widget_vtable_init_by_parent(widget_t* widget, const widget_vtable_t* curr_vt);

ret_t widget_grab_default(widget_t* widget, widget_t* child);
ret_t widget_ungrab_default(widget_t* widget, widget_t* child);
ret_t widget_destroy_default(widget_t* widget);

/* internal */
const char* const* widget_vtable_get_clone_properties(const widget_vtable_t* vt);
const char* const* widget_vtable_get_persistent_properties(const widget_vtable_t* vt);
const char* widget_vtable_get_pointer_cursor(const widget_vtable_t* vt);
ret_t widget_vtable_get_prop(widget_t* widget, const char* name, value_t* v);
ret_t widget_vtable_set_prop(widget_t* widget, const char* name, const value_t* v);
ret_t widget_vtable_invalidate(widget_t* widget, const rect_t* r);
ret_t widget_vtable_find_target(widget_t* widget, xy_t x, xy_t y, widget_t** ret_widget);
ret_t widget_vtable_is_point_in(widget_t* widget, xy_t x, xy_t y, bool_t* ret_is_point_in);
ret_t widget_vtable_get_offset(widget_t* widget, xy_t* out_x, xy_t* out_y);
ret_t widget_vtable_auto_adjust_size(widget_t* widget);
ret_t widget_vtable_get_prop_default_value(widget_t* widget, const char* name, value_t* v);
ret_t widget_vtable_get_only_active_children(widget_t* widget, darray_t* all_focusable);
ret_t widget_vtable_on_copy(widget_t* widget, widget_t* other);
ret_t widget_vtable_on_keyup(widget_t* widget, key_event_t* e);
ret_t widget_vtable_on_keydown(widget_t* widget, key_event_t* e);
ret_t widget_vtable_on_wheel(widget_t* widget, wheel_event_t* e);
ret_t widget_vtable_on_multi_gesture(widget_t* widget, multi_gesture_event_t* e);
ret_t widget_vtable_on_re_translate(widget_t* widget);
ret_t widget_vtable_on_paint_background(widget_t* widget, canvas_t* c);
ret_t widget_vtable_on_paint_self(widget_t* widget, canvas_t* c);
ret_t widget_vtable_on_paint_children(widget_t* widget, canvas_t* c);
ret_t widget_vtable_on_paint_border(widget_t* widget, canvas_t* c);
ret_t widget_vtable_on_paint_begin(widget_t* widget, canvas_t* c);
ret_t widget_vtable_on_paint_end(widget_t* widget, canvas_t* c);
ret_t widget_vtable_on_pointer_down(widget_t* widget, pointer_event_t* e);
ret_t widget_vtable_on_pointer_move(widget_t* widget, pointer_event_t* e);
ret_t widget_vtable_on_pointer_up(widget_t* widget, pointer_event_t* e);
ret_t widget_vtable_on_context_menu(widget_t* widget, pointer_event_t* e);
ret_t widget_vtable_on_layout_children(widget_t* widget);
ret_t widget_vtable_on_add_child(widget_t* widget, widget_t* child);
ret_t widget_vtable_on_remove_child(widget_t* widget, widget_t* child);
ret_t widget_vtable_on_attach_parent(widget_t* widget, widget_t* parent);
ret_t widget_vtable_on_detach_parent(widget_t* widget, widget_t* parent);
ret_t widget_vtable_on_event(widget_t* widget, event_t* e);
ret_t widget_vtable_on_event_before_children(widget_t* widget, event_t* e);
ret_t widget_vtable_on_destroy(widget_t* widget);

END_C_DECLS

#endif /*TK_WIDGET_VTABLE_H*/
