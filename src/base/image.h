/**
 * File:   image.h
 * Author: AWTK Develop Team
 * Brief:  image
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IMAGE_H
#define TK_IMAGE_H

#include "base/wstr.h"
#include "base/image.h"
#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class image_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 图片控件。
 */
typedef struct _image_t {
  widget_t widget;
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
   * 控件的旋转角度。
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
   * @property {char*} image
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 图片的名称。
   */
  char* image;
  /**
   * @property {image_draw_type_t} draw_type
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 图片的绘制方式(仅在没有旋转和缩放时生效)。
   */
  image_draw_type_t draw_type;

  /**
   * @property {bool_t} selected
   * @annotation ["readable","scriptable"]
   * 当前是否被选中。
   */
  bool_t selected;
} image_t;

/**
 * @method image_create
 * 创建image对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method image_set_image
 * 设置控件的图片名称。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image对象。
 * @param {char*}  name 图片名称，该图片必须存在于资源管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_image(widget_t* widget, const char* name);

/**
 * @method image_set_draw_type
 * 设置图片的显示方式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image对象。
 * @param {image_draw_type_t}  draw_type 显示方式(仅在没有旋转和缩放时生效)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_draw_type(widget_t* widget, image_draw_type_t draw_type);

/**
 * @method image_set_rotation
 * 设置控件的旋转角度(仅在WITH_VGCANVAS定义时生效)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {float_t} rotation 旋转角度(幅度)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_rotation(widget_t* widget, float_t rotation);

/**
 * @method image_set_scale
 * 设置控件的缩放比例(仅在WITH_VGCANVAS定义时生效)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {float_t} scale_x X方向缩放比例。
 * @param {float_t} scale_y Y方向缩放比例。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_scale(widget_t* widget, float_t scale_x, float_t scale_y);

/**
 * @method image_set_anchor
 * 设置控件的锚点(仅在WITH_VGCANVAS定义时生效)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {float_t} anchor_x 锚点X(0-1)。0在控件左边，0.5在控件中间，1在控件右边。
 * @param {float_t} anchor_y 锚点Y(0-1)。0在控件顶部，0.5在控件中间，1在控件底部。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_anchor(widget_t* widget, float_t anchor_x, float_t anchor_y);

/**
 * @method image_set_selected
 * 设置控件的选中状态。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} selected 是否被选中。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_selected(widget_t* widget, bool_t selected);

/**
 * @method image_set_selectable
 * 设置控件是否可以被选中。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} selectable 是否可以被选中。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_selectable(widget_t* widget, bool_t selectable);

/**
 * @method image_set_clickable
 * 设置控件是否可以被点击。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} clickable 是否可以被点击。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_clickable(widget_t* widget, bool_t clickable);

#define IMAGE(widget) ((image_t*)(widget))

/*deprecated*/
#define image_set_image_name image_set_image

END_C_DECLS

#endif /*TK_IMAGE_H*/
