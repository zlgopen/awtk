/**
 * File:   window_base.h
 * Author: AWTK Develop Team
 * Brief:  window_base
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WINDOW_BASE_H
#define TK_WINDOW_BASE_H

#include "base/widget.h"
#include "base/native_window.h"

BEGIN_C_DECLS

/**
 * @class window_base_t
 * @parent widget_t
 * @annotation ["scriptable","widget"]
 * 窗口。
 *
 * 本类把窗口相关的公共行为进行抽象，放到一起方便重用。目前已知的具体实现如下图：
 *
 * ```graphviz
 *   [default_style]
 *
 *   window_t -> window_base_t[arrowhead = "empty"]
 *   popup_t -> window_base_t[arrowhead = "empty"]
 *   dialog_t -> window_base_t[arrowhead = "empty"]
 *   system_bar_t -> window_base_t[arrowhead = "empty"]
 *   calibration_win_t -> window_base_t[arrowhead = "empty"]
 * ```
 *
 * > 本类是一个抽象类，不能进行实例化。请在应用程序中使用具体的类，如window\_t。
 *
 */
typedef struct _window_base_t {
  widget_t widget;
  /**
   * @property {char*} theme
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 主题资源的名称。
   * 每个窗口都可以有独立的主题文件，如果没指定，则使用系统缺省的主题文件。
   * 主题是一个XML文件，放在assets/raw/styles目录下。
   * 请参考[主题](https://github.com/zlgopen/awtk/blob/master/docs/theme.md)
   */
  char* theme;

  /**
   * @property {bool_t} disable_anim
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 禁用窗口动画。
   */
  bool_t disable_anim;

  /**
   * @property {window_closable_t} closable
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 收到EVT_REQUEST_CLOSE_WINDOW是否自动关闭窗口。
   *
   * 如果关闭窗口时，需要用户确认:
   *
   * * 1.将closable设置为WINDOW\_CLOSABLE\_CONFIRM
   *
   * * 2.处理窗口的EVT\_REQUEST\_CLOSE\_WINDOW事件
   *
   *> closable在XML中取值为：yes/no/confirm，缺省为yes。
   */
  window_closable_t closable;

  /**
   * @property {char*} open_anim_hint
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 打开窗口动画的名称。
   * 请参考[窗口动画](https://github.com/zlgopen/awtk/blob/master/docs/window_animator.md)
   */
  char* open_anim_hint;

  /**
   * @property {char*} close_anim_hint
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 关闭窗口动画的名称。
   * 请参考[窗口动画](https://github.com/zlgopen/awtk/blob/master/docs/window_animator.md)
   */
  char* close_anim_hint;

  /**
   * @property {char*} stage
   * @annotation ["readable", "get_prop"]
   * 窗口当前处于的状态。
   */
  window_stage_t stage;

  /**
   * @property {theme_t*} theme_obj
   * @annotation ["get_prop"]
   * 窗口的常量主题数据。
   *
   *>
   *把主题管理器对象与窗口关联起来，是为了解决UI设计器与被设计的窗口需要从不同的位置加载主题资源的问题。
   */
  theme_t* theme_obj;

  /**
   * @property {image_manager_t*} image_manager
   * @annotation ["get_prop"]
   * 获取图片管理器对象。
   *
   *>
   *把图片管理器对象与窗口关联起来，是为了解决UI设计器与被设计的窗口需要从不同的位置加载图片资源的问题。
   */

  /**
   * @property {font_manager_t*} font_manager
   * @annotation ["get_prop"]
   * 获取字体管理器对象。
   *
   *>
   *把字体管理器对象与窗口关联起来，是为了解决UI设计器与被设计的窗口需要从不同的位置加载字体资源的问题。
   */

  /**
   * @property {assets_manager_t*} assets_manager
   * @annotation ["get_prop"]
   * 获取资源管理器对象。
   *
   * >
   * 把资源管理器对象与窗口关联起来，是为了解决UI设计器与被设计的窗口需要从不同的位置加载资源资源的问题。
   */

  /**
   * @property {char*} move_focus_prev_key
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 向前移动焦点的键值。
   *
   * 请参考[控件焦点](https://github.com/zlgopen/awtk/blob/master/docs/widget_focus.md)
   */
  char* move_focus_prev_key;

  /**
   * @property {char*} move_focus_next_key
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 向后移动焦点的键值。
   *
   * 请参考[控件焦点](https://github.com/zlgopen/awtk/blob/master/docs/widget_focus.md)
   */
  char* move_focus_next_key;

  /**
   * @property {char*} move_focus_up_key
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 向上移动焦点的键值。
   *
   * 请参考[控件焦点](https://github.com/zlgopen/awtk/blob/master/docs/widget_focus.md)
   */
  char* move_focus_up_key;

  /**
   * @property {char*} move_focus_down_key
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 向下移动焦点的键值。
   *
   * 请参考[控件焦点](https://github.com/zlgopen/awtk/blob/master/docs/widget_focus.md)
   */
  char* move_focus_down_key;

  /**
   * @property {char*} move_focus_left_key
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 向左移动焦点的键值。
   *
   * 请参考[控件焦点](https://github.com/zlgopen/awtk/blob/master/docs/widget_focus.md)
   */
  char* move_focus_left_key;

  /**
   * @property {char*} move_focus_right_key
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 向右移动焦点的键值。
   *
   * 请参考[控件焦点](https://github.com/zlgopen/awtk/blob/master/docs/widget_focus.md)
   */
  char* move_focus_right_key;

  /*private*/
  const asset_info_t* res_theme;
  font_manager_t* font_manager;
  native_window_t* native_window;
  widget_t* save_focus_widget;
  uint32_t grab_count_when_to_foreground;
} window_base_t;

/*for sub class*/

/**
 * @method window_base_on_destroy
 * 窗口on_destroy函数的缺省实现。
 * @param {widget_t*} widget window_base对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_base_on_destroy(widget_t* widget);
/**
 * @method window_base_on_event
 * 窗口on_event函数的缺省实现。
 * @param {widget_t*} widget window_base对象。
 * @param {event_t*} e event对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_base_on_event(widget_t* widget, event_t* e);
ret_t window_base_invalidate(widget_t* widget, rect_t* r);

/**
 * @method window_base_on_paint_begin
 * 窗口on_paint_begin函数的缺省实现。
 * @param {widget_t*} widget window_base对象。
 * @param {canvas_t*} c canvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_base_on_paint_begin(widget_t* widget, canvas_t* c);
/**
 * @method window_base_on_paint_end
 * 窗口on_paint_end函数的缺省实现。
 * @param {widget_t*} widget window_base对象。
 * @param {canvas_t*} c canvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。1
 */
ret_t window_base_on_paint_end(widget_t* widget, canvas_t* c);

/**
 * @method window_base_on_paint_self
 * 窗口on_paint_self函数的缺省实现。
 * @param {widget_t*} widget window_base对象。
 * @param {canvas_t*} c canvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_base_on_paint_self(widget_t* widget, canvas_t* c);

/**
 * @method window_base_get_prop
 * 窗口get_prop函数的缺省实现。
 * @param {widget_t*} widget window_base对象。g
 * @param {const char*} name 属性名。
 * @param {value_t*} v value对象
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。。
 */
ret_t window_base_get_prop(widget_t* widget, const char* name, value_t* v);

/**
 * @method window_base_set_prop
 * 窗口set_prop函数的缺省实现。
 *
 * @param {widget_t*} widget window_base对象。
 * @param {const char*} name 属性名。
 * @param {const value_t*} v value对象
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_base_set_prop(widget_t* widget, const char* name, const value_t* v);

/**
 * @method window_base_create
 * 创建window base对象。
 *
 * @param {widget_t*} parent 父控件
 * @param {const widget_vtable_t*} vt vtable对象。
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 窗口对象。
 */
widget_t* window_base_create(widget_t* parent, const widget_vtable_t* vt, xy_t x, xy_t y, wh_t w,
                             wh_t h);
/**
 * @method window_base_cast
 * 转换为window_base对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget window_base对象。
 *
 * @return {widget_t*} window_base对象。
 */
widget_t* window_base_cast(widget_t* widget);

#define WINDOW_BASE(widget) ((window_base_t*)(window_base_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(window_base);

END_C_DECLS

#endif /*TK_WINDOW_BASE_H*/
