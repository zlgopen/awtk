/**
 * File:   svg_image.h
 * Author: AWTK Develop Team
 * Brief:  svg_image
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
 * 2018-11-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SVG_IMAGE_H
#define TK_SVG_IMAGE_H

#include "base/widget.h"
#include "base/image_base.h"

BEGIN_C_DECLS

/**
 * @class svg_image_t
 * @parent image_base_t
 * @annotation ["scriptable","design","widget"]
 * SVG图片控件。
 *
 * svg\_image\_t是[image\_base\_t](image_base_t.md)的子类控件，image\_base\_t的函数均适用于svg\_image\_t控件。
 *
 * 在xml中使用"svg"标签创建SVG图片控件。如：
 *
 * ```xml
 * <svg image="girl"/>
 * ```
 *
 * >更多用法请参考：[svg image](
 * https://github.com/zlgopen/awtk/blob/master/design/default/ui/svg_image.xml)
 *
 * 在c代码中使用函数svg\_image\_create创建SVG图片控件。如：
 *
 * ```c
 *  widget_t* image = svg_image_create(win, 10, 10, 200, 200);
 *  image_set_image(image, "girl");
 * ```
 *
 * > 创建之后: 需要用widget\_set\_image设置图片名称。
 *
 * > 完整示例请参考：[svg image demo](
 * https://github.com/zlgopen/awtk-c-demos/blob/master/demos/svg_image.c)
 *
 * 可用通过style来设置控件的显示风格，如背景和边框等。如：
 *
 * ```xml
 * <svg>
 * <style name="default">
 *   <normal border_color="green" fg_color="red" />
 * </style>
 * </svg>
 * ```
 *
 * > 更多用法请参考：[theme default](
 * https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml)
 *
 */
typedef struct _svg_image_t {
  image_base_t image_base;

  /**
   * @property {bool_t} is_cache_mode
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 离线缓存渲染模式。
   */
  bool_t is_cache_mode;

  /**
   * @property {image_draw_type_t} draw_type
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * svg图片的绘制方式(支持旋转缩放, 目前仅支持scale、scale_auto模式)。
   */
  image_draw_type_t draw_type;

  /*private*/
  wh_t org_w;
  wh_t org_h;
  float_t scale_x;
  float_t scale_y;
  canvas_t* canvas_offline;
  const asset_info_t* bsvg_asset;
} svg_image_t;

/**
 * @method svg_image_create
 * 创建svg_image对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* svg_image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method svg_image_set_image
 * 设置控件的图片名称。
 *
 *> 如果需要显示文件系统中的图片，只需将图片名称换成实际的文件名，并加上"file://"前缀即可。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image对象。
 * @param {const char*}  name 图片名称，该图片必须存在于资源管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t svg_image_set_image(widget_t* widget, const char* name);

/**
 * @method svg_image_set_cache_mode
 * 控件设置是否开启离线缓存渲染模式。
 *
 *> 在确保svg图片不经常变化大小及状态的情况下，开启离线缓存渲染能够减少解析bsvg的开销，提高效率。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image对象。
 * @param {bool_t}  is_cache_mode 是否开启缓存模式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t svg_image_set_cache_mode(widget_t* widget, bool_t is_cache_mode);

/**
 * @method svg_image_set_draw_type
 * 控件设置svg图片绘制模式。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image对象。
 * @param {image_draw_type_t}  draw_type 绘制模式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t svg_image_set_draw_type(widget_t* widget, image_draw_type_t draw_type);

/**
 * @method svg_image_cast
 * 转换为svg_image对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget svg_image对象。
 *
 * @return {widget_t*} svg_image对象。
 */
widget_t* svg_image_cast(widget_t* widget);

#define WIDGET_TYPE_SVG_IMAGE "svg"
#define SVG_IMAGE_CACHE_MODE "cache_mode"

#define SVG_IMAGE(widget) ((svg_image_t*)(svg_image_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(svg_image);

END_C_DECLS

#endif /*TK_SVG_IMAGE_H*/
