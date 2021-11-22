

#ifndef TK_info_view2_H
#define TK_info_view2_H

#include "base/window_base.h"

BEGIN_C_DECLS


#define WIDGET_TYPE_info_view2 "info_view2"
#define WIDGET_PROP_CLICK_THROUGH "click_through"
#define WIDGET_PROP_ALWAYS_ON_TOP "always_on_top"

typedef struct _info_view2_t {
  window_base_t window;

  /**
   * @property {bool_t} click_through
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 点击穿透。点击没有子控件的位置，是否穿透到底层窗口。
   *
   * 缺省不启用。
   *
   */
  bool_t click_through;

  /**
   * @property {bool_t} always_on_top
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否总在最上面。
   *
   * 缺省不启用。
   *
   */
  bool_t always_on_top;
  int32_t yoffset;
  rect_t dragger_rect;
  bool_t info_view2_show;
  widget_t *prev_window;

} info_view2_t;

/**
 * @method info_view2_create
 * 创建info_view2对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* info_view2_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method info_view2_set_click_through
 * 设置是否启用点击穿透。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  click_through 是否启用点击穿透。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t info_view2_set_click_through(widget_t* widget, bool_t click_through);

/**
 * @method info_view2_set_always_on_top
 * 设置是否总是在最上面。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  always_on_top 是否总是在最上面。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t info_view2_set_always_on_top(widget_t* widget, bool_t always_on_top);

/**
 * @method info_view2_cast
 * 转换为info_view2对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget info_view2对象。
 *
 * @return {widget_t*} info_view2对象。
 */
widget_t* info_view2_cast(widget_t* widget);

#define info_view2(widget) ((info_view2_t*)(info_view2_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(info_view2);

END_C_DECLS

#endif /*TK_info_view2_H*/
