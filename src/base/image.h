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
 * @scriptable
 * 图片控件。
 */
typedef struct _image_t {
  widget_t widget;
  /**
   * @property {float_t} anchor_x
   * @realonly
   * 锚点X(0-1)。0在控件左边，0.5在控件中间，1在控件右边。
   */
  float_t anchor_x;
  /**
   * @property {float_t} anchor_y
   * @realonly
   * 锚点Y(0-1)。0在控件顶部，0.5在控件中间，1在控件底部。
   */
  float_t anchor_y;
  /**
   * @property {float_t} scale_x
   * @realonly
   * 控件在X方向上的缩放比例。
   */
  float_t scale_x;
  /**
   * @property {float_t} scale_y
   * @realonly
   * 控件在Y方向上的缩放比例。
   */
  float_t scale_y;
  /**
   * @property {float_t} rotation
   * @realonly
   * 控件的旋转角度。
   */
  float_t rotation;
  /**
   * @property {char*} image
   * @fake
   * 图片的名称(只能通过widget_get_prop/set_prop访问)。
   */

  /**
   * @property {char*} draw_type
   * @fake
   * 图片的绘制方式(只能通过widget_get_prop/set_prop访问)。
   */

  /*private*/
  bitmap_t bitmap;
  image_draw_type_t draw_type;
} image_t;

/**
 * @method image_create
 * @constructor
 * 创建image对象
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
 * @method image_set_image_name
 * 设置控件的图片名称。
 * @param {widget_t*} widget image对象。
 * @param {char*}  name 图片名称，该图片必须存在于资源管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_image_name(widget_t* widget, const char* name);

/**
 * @method image_set_draw_type
 * 设置图片的显示方式。
 * @param {widget_t*} widget image对象。
 * @param {image_draw_type_t}  draw_type 显示方式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_draw_type(widget_t* widget, image_draw_type_t draw_type);

/**
 * @method image_set_rotation
 * 设置控件的旋转角度(仅在WITH_VGCANVAS定义时生效)。
 * @param {widget_t*} widget 控件对象。
 * @param {float_t} rotation 旋转角度(幅度)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_rotation(widget_t* widget, float_t rotation);

/**
 * @method image_set_scale
 * 设置控件的缩放比例(仅在WITH_VGCANVAS定义时生效)。
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
 * @param {widget_t*} widget 控件对象。
 * @param {float_t} anchor_x 锚点X(0-1)。0在控件左边，0.5在控件中间，1在控件右边。
 * @param {float_t} anchor_y 锚点Y(0-1)。0在控件顶部，0.5在控件中间，1在控件底部。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_anchor(widget_t* widget, float_t anchor_x, float_t anchor_y);

#define IMAGE(widget) ((image_t*)(widget))

END_C_DECLS

#endif /*TK_IMAGE_H*/
