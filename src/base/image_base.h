/**
 * File:   image_base.h
 * Author: AWTK Develop Team
 * Brief:  image base
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
 * 2018-11-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IMAGE_BASE_H
#define TK_IMAGE_BASE_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class image_base_t
 * @parent widget_t
 * @annotation ["scriptable","widget"]
 * 图片控件基类。
 *
 * 本类把图片相关控件的公共行为进行抽象，放到一起方便重用。目前已知的具体实现如下图：
 *
 * ```graphviz
 *   [default_style]
 *
 *   image_t -> image_base_t[arrowhead = "empty"]
 *   svg_image_t -> image_base_t[arrowhead = "empty"]
 *   gif_image_t -> image_base_t[arrowhead = "empty"]
 * ```
 *
 * > 本类是一个抽象类，不能进行实例化。请在应用程序中使用具体的类，如image\_t。
 *
 * 如果需要显示文件系统中的图片，只需将图片名称换成实际的文件名，并加上"file://"前缀即可。如：
 *
 *```
 *  <image draw_type="center" image="file://./design/default/images/xx/flag_CN.png" />
 *  <gif image="file://./design/default/images/x2/bee.gif" />
 *  <svg image="file://./design/default/images/svg/china.bsvg" />
 * ```
 */
typedef struct _image_base_t {
  widget_t widget;

  /**
   * @property {char*} image
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 图片的名称。
   */
  char* image;
  /**
   * @property {float_t} anchor_x
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 锚点X(0-1)。0在控件左边，0.5在控件中间，1在控件右边。
   */
  float_t anchor_x;
  /**
   * @property {float_t} anchor_y
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 锚点Y(0-1)。0在控件顶部，0.5在控件中间，1在控件底部。
   */
  float_t anchor_y;
  /**
   * @property {float_t} scale_x
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 控件在X方向上的缩放比例。
   */
  float_t scale_x;
  /**
   * @property {float_t} scale_y
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 控件在Y方向上的缩放比例。
   */
  float_t scale_y;
  /**
   * @property {float_t} rotation
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 控件的旋转角度(幅度)。
   */
  float_t rotation;
  /**
   * @property {bool_t} clickable
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 点击时，是否触发EVT_CLICK事件。
   */
  bool_t clickable;
  /**
   * @property {bool_t} selectable
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否设置选中状态。
   */
  bool_t selectable;
  /**
   * @property {bool_t} selected
   * @annotation ["readable","scriptable"]
   * 当前是否被选中。
   */
  bool_t selected;
} image_base_t;

/**
 * @method image_base_set_image
 * 设置控件的图片名称。
 *
 *> 如果需要显示文件系统中的图片，只需将图片名称换成实际的文件名，并加上"file://"前缀即可。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image对象。
 * @param {char*}  name 图片名称，该图片必须存在于资源管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_base_set_image(widget_t* widget, const char* name);
#define image_set_image image_base_set_image

/**
 * @method image_base_set_rotation
 * 设置控件的旋转角度(仅在WITH_VGCANVAS定义时生效)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {float_t} rotation 旋转角度(幅度)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_base_set_rotation(widget_t* widget, float_t rotation);
#define image_set_rotation image_base_set_rotation

/**
 * @method image_base_set_scale
 * 设置控件的缩放比例(仅在WITH_VGCANVAS定义时生效)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {float_t} scale_x X方向缩放比例。
 * @param {float_t} scale_y Y方向缩放比例。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_base_set_scale(widget_t* widget, float_t scale_x, float_t scale_y);
#define image_set_scale image_base_set_scale

/**
 * @method image_base_set_anchor
 * 设置控件的锚点(仅在WITH_VGCANVAS定义时生效)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {float_t} anchor_x 锚点X(0-1)。0在控件左边，0.5在控件中间，1在控件右边。
 * @param {float_t} anchor_y 锚点Y(0-1)。0在控件顶部，0.5在控件中间，1在控件底部。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_base_set_anchor(widget_t* widget, float_t anchor_x, float_t anchor_y);
#define image_set_anchor image_base_set_anchor

/**
 * @method image_base_set_selected
 * 设置控件的选中状态。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} selected 是否被选中。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_base_set_selected(widget_t* widget, bool_t selected);
#define image_set_selected image_base_set_selected

/**
 * @method image_base_set_selectable
 * 设置控件是否可以被选中。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} selectable 是否可以被选中。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_base_set_selectable(widget_t* widget, bool_t selectable);
#define image_set_selectable image_base_set_selectable

/**
 * @method image_base_set_clickable
 * 设置控件是否可以被点击。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} clickable 是否可以被点击。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_base_set_clickable(widget_t* widget, bool_t clickable);
#define image_set_clickable image_base_set_clickable

/**
 * @method image_base_cast
 * 转换为image_base对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget image_base对象。
 *
 * @return {widget_t*} image_base对象。
 */
widget_t* image_base_cast(widget_t* widget);

/*public for subclass*/
widget_t* image_base_init(widget_t* widget);
ret_t image_base_on_destroy(widget_t* widget);
ret_t image_base_on_event(widget_t* widget, event_t* e);
ret_t image_base_get_prop(widget_t* widget, const char* name, value_t* v);
ret_t image_base_set_prop(widget_t* widget, const char* name, const value_t* v);
bool_t image_need_transform(widget_t* widget);
ret_t image_transform(widget_t* widget, canvas_t* c);
ret_t image_base_on_copy(widget_t* widget, widget_t* other);

#define IMAGE_BASE(widget) ((image_base_t*)(image_base_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(image_base);

END_C_DECLS

#endif /*TK_IMAGE_BASE_H*/
