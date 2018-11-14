/**
 * File:   vgcanvas.h
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas interface.
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
 * 2018-03-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VG_CANVAS_H
#define TK_VG_CANVAS_H

#include "base/rect.h"
#include "base/bitmap.h"

BEGIN_C_DECLS

/*用于离线渲染*/
typedef struct _framebuffer_object_t {
  int w;
  int h;
  int id;
  void* handle;
  float_t ratio;
} framebuffer_object_t;

struct _vgcanvas_t;
typedef struct _vgcanvas_t vgcanvas_t;

typedef ret_t (*vgcanvas_reinit_t)(vgcanvas_t* vg, uint32_t w, uint32_t h, uint32_t stride,
                                   bitmap_format_t format, void* data);
typedef ret_t (*vgcanvas_begin_frame_t)(vgcanvas_t* vg, rect_t* dirty_rect);
typedef ret_t (*vgcanvas_end_frame_t)(vgcanvas_t* vg);

typedef ret_t (*vgcanvas_reset_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_flush_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_clear_rect_t)(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h,
                                       color_t color);

typedef ret_t (*vgcanvas_begin_path_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_move_to_t)(vgcanvas_t* vg, float_t x, float_t y);
typedef ret_t (*vgcanvas_line_to_t)(vgcanvas_t* vg, float_t x, float_t y);
typedef ret_t (*vgcanvas_quad_to_t)(vgcanvas_t* vg, float_t cpx, float_t cpy, float_t x, float_t y);
typedef ret_t (*vgcanvas_bezier_to_t)(vgcanvas_t* vg, float_t cp1x, float_t cp1y, float_t cp2x,
                                      float_t cp2y, float_t x, float_t y);
typedef ret_t (*vgcanvas_arc_to_t)(vgcanvas_t* vg, float_t x1, float_t y1, float_t x2, float_t y2,
                                   float_t r);
typedef ret_t (*vgcanvas_arc_t)(vgcanvas_t* vg, float_t x, float_t y, float_t r,
                                float_t start_angle, float_t end_angle, bool_t ccw);
typedef bool_t (*vgcanvas_is_point_in_path_t)(vgcanvas_t* vg, float_t x, float_t y);
typedef ret_t (*vgcanvas_rounded_rect_t)(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h,
                                         float_t r);
typedef ret_t (*vgcanvas_ellipse_t)(vgcanvas_t* vg, float_t x, float_t y, float_t rx, float_t ry);
typedef ret_t (*vgcanvas_close_path_t)(vgcanvas_t* vg);

typedef ret_t (*vgcanvas_rotate_t)(vgcanvas_t* vg, float_t rad);
typedef ret_t (*vgcanvas_scale_t)(vgcanvas_t* vg, float_t x, float_t y);
typedef ret_t (*vgcanvas_translate_t)(vgcanvas_t* vg, float_t x, float_t y);
typedef ret_t (*vgcanvas_transform_t)(vgcanvas_t* vg, float_t a, float_t b, float_t c, float_t d,
                                      float_t e, float_t f);
typedef ret_t (*vgcanvas_set_transform_t)(vgcanvas_t* vg, float_t a, float_t b, float_t c,
                                          float_t d, float_t e, float_t f);

typedef ret_t (*vgcanvas_clip_rect_t)(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h);
typedef ret_t (*vgcanvas_fill_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_stroke_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_paint_t)(vgcanvas_t* vg, bool_t stroke, bitmap_t* img);

typedef ret_t (*vgcanvas_set_font_t)(vgcanvas_t* vg, const char* font);
typedef ret_t (*vgcanvas_set_font_size_t)(vgcanvas_t* vg, float_t size);
typedef ret_t (*vgcanvas_set_text_align_t)(vgcanvas_t* vg, const char* value);
typedef ret_t (*vgcanvas_set_text_baseline_t)(vgcanvas_t* vg, const char* value);
typedef ret_t (*vgcanvas_fill_text_t)(vgcanvas_t* vg, const char* text, float_t x, float_t y,
                                      float_t max_width);
typedef float_t (*vgcanvas_measure_text_t)(vgcanvas_t* vg, const char* text);
typedef ret_t (*vgcanvas_draw_image_t)(vgcanvas_t* vg, bitmap_t* img, float_t sx, float_t sy,
                                       float_t sw, float_t sh, float_t dx, float_t dy, float_t dw,
                                       float_t dh);

typedef ret_t (*vgcanvas_set_antialias_t)(vgcanvas_t* vg, bool_t value);
typedef ret_t (*vgcanvas_set_global_alpha_t)(vgcanvas_t* vg, float_t alpha);
typedef ret_t (*vgcanvas_set_line_width_t)(vgcanvas_t* vg, float_t value);
typedef ret_t (*vgcanvas_set_fill_color_t)(vgcanvas_t* vg, color_t color);
typedef ret_t (*vgcanvas_set_stroke_color_t)(vgcanvas_t* vg, color_t color);
typedef ret_t (*vgcanvas_set_line_cap_t)(vgcanvas_t* vg, const char* value);
typedef ret_t (*vgcanvas_set_line_join_t)(vgcanvas_t* vg, const char* value);
typedef ret_t (*vgcanvas_set_miter_limit_t)(vgcanvas_t* vg, float_t value);

typedef ret_t (*vgcanvas_save_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_restore_t)(vgcanvas_t* vg);

typedef ret_t (*vgcanvas_create_fbo_t)(vgcanvas_t* vg, framebuffer_object_t* fbo);
typedef ret_t (*vgcanvas_destroy_fbo_t)(vgcanvas_t* vg, framebuffer_object_t* fbo);
typedef ret_t (*vgcanvas_bind_fbo_t)(vgcanvas_t* vg, framebuffer_object_t* fbo);
typedef ret_t (*vgcanvas_unbind_fbo_t)(vgcanvas_t* vg, framebuffer_object_t* fbo);

typedef ret_t (*vgcanvas_destroy_t)(vgcanvas_t* vg);

typedef struct _vgcanvas_vtable_t {
  vgcanvas_reinit_t reinit;

  vgcanvas_begin_frame_t begin_frame;
  vgcanvas_reset_t reset;
  vgcanvas_flush_t flush;
  vgcanvas_clear_rect_t clear_rect;

  vgcanvas_begin_path_t begin_path;
  vgcanvas_move_to_t move_to;
  vgcanvas_line_to_t line_to;
  vgcanvas_arc_t arc;
  vgcanvas_arc_to_t arc_to;
  vgcanvas_bezier_to_t bezier_to;
  vgcanvas_quad_to_t quad_to;
  vgcanvas_is_point_in_path_t is_point_in_path;
  vgcanvas_ellipse_t ellipse;
  vgcanvas_rounded_rect_t rounded_rect;
  vgcanvas_close_path_t close_path;

  vgcanvas_scale_t scale;
  vgcanvas_rotate_t rotate;
  vgcanvas_translate_t translate;
  vgcanvas_transform_t transform;
  vgcanvas_set_transform_t set_transform;

  vgcanvas_clip_rect_t clip_rect;
  vgcanvas_fill_t fill;
  vgcanvas_stroke_t stroke;
  vgcanvas_paint_t paint;

  vgcanvas_set_font_t set_font;
  vgcanvas_set_font_size_t set_font_size;
  vgcanvas_set_text_align_t set_text_align;
  vgcanvas_set_text_baseline_t set_text_baseline;
  vgcanvas_fill_text_t fill_text;
  vgcanvas_measure_text_t measure_text;
  vgcanvas_draw_image_t draw_image;

  vgcanvas_set_antialias_t set_antialias;
  vgcanvas_set_global_alpha_t set_global_alpha;
  vgcanvas_set_line_width_t set_line_width;
  vgcanvas_set_fill_color_t set_fill_color;
  vgcanvas_set_stroke_color_t set_stroke_color;
  vgcanvas_set_line_join_t set_line_join;
  vgcanvas_set_line_cap_t set_line_cap;
  vgcanvas_set_miter_limit_t set_miter_limit;

  vgcanvas_save_t save;
  vgcanvas_restore_t restore;
  vgcanvas_end_frame_t end_frame;

  vgcanvas_create_fbo_t create_fbo;
  vgcanvas_destroy_fbo_t destroy_fbo;
  vgcanvas_bind_fbo_t bind_fbo;
  vgcanvas_unbind_fbo_t unbind_fbo;

  vgcanvas_destroy_t destroy;
} vgcanvas_vtable_t;

/**
 * @class vgcanvas_t
 * 矢量图画布抽象基类。
 */
struct _vgcanvas_t {
  /**
   * @property {wh_t} w
   * @annotation ["readable"]
   * canvas的宽度
   */
  uint32_t w;
  /**
   * @property {wh_t} height
   * @annotation ["readable"]
   * canvas的高度
   */
  uint32_t h;
  /**
   * @property {float_t} ratio
   * @annotation ["readable"]
   * 显示比例。
   */
  float_t ratio;
  /**
   * @property {bool_t} anti_alias
   * @annotation ["readable"]
   * 是否启用反走样功能。
   */
  bool_t anti_alias;
  /**
   * @property {float_t} line_width
   * @annotation ["readable"]
   * 线宽。
   */
  float_t line_width;
  /**
   * @property {float_t} global_alpha
   * @annotation ["readable"]
   * 全局alpha。
   */
  float_t global_alpha;
  /**
   * @property {float_t} miter_limit
   * @annotation ["readable"]
   * miter_limit。
   * @see http://www.w3school.com.cn/tags/canvas_miterlimit.asp
   */
  float_t miter_limit;
  /**
   * @property {char*} line_cap
   * @annotation ["readable"]
   * line_cap。
   * @see http://www.w3school.com.cn/tags/canvas_linecap.asp
   */
  const char* line_cap;
  /**
   * @property {char*} line_join
   * @annotation ["readable"]
   * line_join。
   * @see http://www.w3school.com.cn/tags/canvas_linejoin.asp
   */
  const char* line_join;
  /**
   * @property {char*} font
   * @annotation ["readable"]
   * 字体。
   */
  const char* font;
  /**
   * @property {float_t} font_size
   * @annotation ["readable"]
   * 字体大小。
   */
  float_t font_size;
  /**
   * @property {char*} text_align
   * @annotation ["readable"]
   * text_align。
   * @see http://www.w3school.com.cn/tags/canvas_textalign.asp
   */
  const char* text_align;
  /**
   * @property {char*} text_baseline
   * @annotation ["readable"]
   * text_baseline。
   * @see http://www.w3school.com.cn/tags/canvas_textbaseline.asp
   */
  const char* text_baseline;
  /**
   * @property {color_t} fill_color
   * @annotation ["readable"]
   * 填充颜色
   */
  color_t fill_color;
  /**
   * @property {color_t} stroke_color
   * @annotation ["readable"]
   * 线条颜色
   */
  color_t stroke_color;

  /**
   * @property {uint32_t*} buff;
   * @annotation ["private"]
   * frame buffer
   */
  uint32_t* buff;

  const vgcanvas_vtable_t* vt;
};

/*大部分函数可以参考：https://www.w3schools.com/tags/ref_canvas.asp*/

/**
 * @method vgcanvas_create
 * 创建vgcanvas。
 * @annotation ["constructor"]
 * @param {uint32_t} w 宽度
 * @param {uint32_t} h 高度
 * @param {uint32_t} stride 一行占用的字节数。
 * @param {bitmap_format_t} format 如果data是framebuffer，format指定data的格式。
 * @param {void*} data framebuffer或其它ctx。
 *
 * @return {vgcanvas_t} 返回vgcanvas
 */
vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                            void* data);

/**
 * @method vgcanvas_begin_path
 * begin path
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_begin_path(vgcanvas_t* vg);

/**
 * @method vgcanvas_reinit
 * 重新初始化，系统内部调用。
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {uint32_t} w 宽度
 * @param {uint32_t} h 高度
 * @param {uint32_t} stride 一行占用的字节数。
 * @param {bitmap_format_t} format 如果data是framebuffer，format指定data的格式。
 * @param {void*} data framebuffer或其它ctx。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_reinit(vgcanvas_t* vg, uint32_t w, uint32_t h, uint32_t stride,
                      bitmap_format_t format, void* data);

/**
 * @method vgcanvas_begin_frame
 * 开始绘制，系统内部调用。
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {rect_t*} dirty_rect 需要绘制的区域。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_begin_frame(vgcanvas_t* vg, rect_t* dirty_rect);

/**
 * @method vgcanvas_move_to
 * move to
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_move_to(vgcanvas_t* vg, float_t x, float_t y);

/**
 * @method vgcanvas_line_to
 * line to。
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_line_to(vgcanvas_t* vg, float_t x, float_t y);

/**
 * @method vgcanvas_quad_to
 * quadratic curve to
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} cpx 控制点x坐标。
 * @param {float_t} cpy 控制点y坐标。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_quad_to(vgcanvas_t* vg, float_t cpx, float_t cpy, float_t x, float_t y);

/**
 * @method vgcanvas_bezier_to
 * bezier curve to
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} cp1x 控制点1x坐标。
 * @param {float_t} cp1y 控制点1y坐标。
 * @param {float_t} cp2x 控制点2x坐标。
 * @param {float_t} cp2y 控制点3y坐标。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_bezier_to(vgcanvas_t* vg, float_t cp1x, float_t cp1y, float_t cp2x, float_t cp2y,
                         float_t x, float_t y);

/**
 * @method vgcanvas_arc_to
 * arc to
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x1 起始点x坐标。
 * @param {float_t} y1 起始点y坐标。
 * @param {float_t} x2 结束点x坐标。
 * @param {float_t} y2 结束点y坐标。
 * @param {float_t} r 半径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_arc_to(vgcanvas_t* vg, float_t x1, float_t y1, float_t x2, float_t y2, float_t r);

/**
 * @method vgcanvas_arc
 * arc
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x 原点x坐标。
 * @param {float_t} y 原点y坐标。
 * @param {float_t} r 半径。
 * @param {float_t} start_angle 起始角度。
 * @param {float_t} end_angle 结束角度。
 * @param {bool_t} ccw 是否逆时针。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_arc(vgcanvas_t* vg, float_t x, float_t y, float_t r, float_t start_angle,
                   float_t end_angle, bool_t ccw);

/**
 * @method vgcanvas_is_point_in_path
 * 检查点是否在当前路径中。
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 *
 * @return {bool_t} 返回TRUE表示在，否则表示不在。
 */
bool_t vgcanvas_is_point_in_path(vgcanvas_t* vg, float_t x, float_t y);

/**
 * @method vgcanvas_rect
 * rect
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 * @param {float_t} w 宽度。
 * @param {float_t} h 高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_rect(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h);

/**
 * @method vgcanvas_rounded_rect
 * round rect
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 * @param {float_t} w 宽度。
 * @param {float_t} h 高度。
 * @param {float_t} r 圆角半径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_rounded_rect(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h, float_t r);

/**
 * @method vgcanvas_ellipse
 * ellipse
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 * @param {float_t} rx 圆角半径。
 * @param {float_t} ry 圆角半径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_ellipse(vgcanvas_t* vg, float_t x, float_t y, float_t rx, float_t ry);

/**
 * @method vgcanvas_close_path
 * close_path
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_close_path(vgcanvas_t* vg);

/**
 * @method vgcanvas_reset
 * reset
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_reset(vgcanvas_t* vg);

/**
 * @method vgcanvas_flush
 * flush
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_flush(vgcanvas_t* vg);

/**
 * @method vgcanvas_clear_rect
 * clear rect
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 * @param {float_t} w 宽度。
 * @param {float_t} h 高度。
 * @param {color_t} c 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_clear_rect(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h,
                          color_t color);

/**
 * @method vgcanvas_rotate
 * rotate
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} rad 角度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_rotate(vgcanvas_t* vg, float_t rad);

/**
 * @method vgcanvas_scale
 * scale
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x方向缩放比例。
 * @param {float_t} y y方向缩放比例。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_scale(vgcanvas_t* vg, float_t x, float_t y);

/**
 * @method vgcanvas_translate
 * scale
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x方向偏移。
 * @param {float_t} y y方向偏移。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_translate(vgcanvas_t* vg, float_t x, float_t y);

/**
 * @method vgcanvas_transform
 * transform
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} a a
 * @param {float_t} b b
 * @param {float_t} c c
 * @param {float_t} d d
 * @param {float_t} e e
 * @param {float_t} f f
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_transform(vgcanvas_t* vg, float_t a, float_t b, float_t c, float_t d, float_t e,
                         float_t f);

/**
 * @method vgcanvas_set_transform
 * set transform
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} a a
 * @param {float_t} b b
 * @param {float_t} c c
 * @param {float_t} d d
 * @param {float_t} e e
 * @param {float_t} f f
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_transform(vgcanvas_t* vg, float_t a, float_t b, float_t c, float_t d, float_t e,
                             float_t f);

/**
 * @method vgcanvas_clip_rect
 * clip_rect
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 * @param {float_t} w 宽度。
 * @param {float_t} h 高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_clip_rect(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h);

/**
 * @method vgcanvas_fill
 * 填充多边形。
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_fill(vgcanvas_t* vg);

/**
 * @method vgcanvas_stroke
 * 画线。
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_stroke(vgcanvas_t* vg);

/**
 * @method vgcanvas_paint
 * 用图片填充/画多边形(可能存在可移植性问题，除非必要请勿使用)。
 * 多边形的顶点必须在图片范围内，可以通过矩阵变化画到不同的位置。
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {bool_t} stroke TRUE表示画线FALSE表示填充。
 * @param {bitmap_t*} img 图片。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_paint(vgcanvas_t* vg, bool_t stroke, bitmap_t* img);

/**
 * @method vgcanvas_set_font
 * set font
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} font 字体名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_font(vgcanvas_t* vg, const char* font);

/**
 * @method vgcanvas_set_font_size
 * set font size
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} font 字体大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_font_size(vgcanvas_t* vg, float_t size);

/**
 * @method vgcanvas_set_text_align
 * set text align
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} value 取值：left|center|right，必须为常量字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_text_align(vgcanvas_t* vg, const char* value);

/**
 * @method vgcanvas_set_text_baseline
 * set text baseline
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} value 取值：top|middle|bottom，必须为常量字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_text_baseline(vgcanvas_t* vg, const char* value);

/**
 * @method vgcanvas_fill_text
 * fill text
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} text text
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 * @param {float_t} max_width 最大宽度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_fill_text(vgcanvas_t* vg, const char* text, float_t x, float_t y, float_t max_width);

/**
 * @method vgcanvas_measure_text
 * measure text
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} text text
 *
 * @return {float_t} 返回text的宽度。
 */
float_t vgcanvas_measure_text(vgcanvas_t* vg, const char* text);

/**
 * @method vgcanvas_draw_image
 * draw image
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {bitmap_t*} img 图片。
 * @param {float_t} sx sx
 * @param {float_t} sy sy
 * @param {float_t} sw sw
 * @param {float_t} sh sh
 * @param {float_t} dx dx
 * @param {float_t} dy dy
 * @param {float_t} dw dw
 * @param {float_t} dh dh
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_draw_image(vgcanvas_t* vg, bitmap_t* img, float_t sx, float_t sy, float_t sw,
                          float_t sh, float_t dx, float_t dy, float_t dw, float_t dh);

/**
 * @method vgcanvas_draw_icon
 * draw icon
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {bitmap_t*} img 图片。
 * @param {float_t} sx sx
 * @param {float_t} sy sy
 * @param {float_t} sw sw
 * @param {float_t} sh sh
 * @param {float_t} dx dx
 * @param {float_t} dy dy
 * @param {float_t} dw dw
 * @param {float_t} dh dh
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_draw_icon(vgcanvas_t* vg, bitmap_t* img, float_t sx, float_t sy, float_t sw,
                         float_t sh, float_t dx, float_t dy, float_t dw, float_t dh);
/**
 * @method vgcanvas_set_antialias
 * set antialias
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {bool_t} value 是否启用反走样。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_antialias(vgcanvas_t* vg, bool_t value);

/**
 * @method vgcanvas_set_global_alpha
 * set global alpha
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} alpha global alpha。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_global_alpha(vgcanvas_t* vg, float_t alpha);

/**
 * @method vgcanvas_set_line_width
 * set line width
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} value 线宽。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_line_width(vgcanvas_t* vg, float_t value);

/**
 * @method vgcanvas_set_fill_color
 * set fill color
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {color_t} color 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_fill_color(vgcanvas_t* vg, color_t color);

/**
 * @method vgcanvas_set_stroke_color
 * set stroke color
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {color_t} color 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_stroke_color(vgcanvas_t* vg, color_t color);

/**
 * @method vgcanvas_set_line_cap
 * set line cap
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} value 取值：butt|round|square，必须为常量字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_line_cap(vgcanvas_t* vg, const char* value);

/**
 * @method vgcanvas_set_line_join
 * set line join
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} value 取值：bevel|round|miter，必须为常量字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_line_join(vgcanvas_t* vg, const char* value);

/**
 * @method vgcanvas_set_miter_limit
 * set miter limit
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} value miter limit
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_miter_limit(vgcanvas_t* vg, float_t value);

/**
 * @method vgcanvas_save
 * save
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_save(vgcanvas_t* vg);

/**
 * @method vgcanvas_restore
 * restore
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_restore(vgcanvas_t* vg);

/**
 * @method vgcanvas_end_frame
 * 结束绘制。系统内部调用。
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_end_frame(vgcanvas_t* vg);

/**
 * @method vgcanvas_destroy
 * destroy
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_destroy(vgcanvas_t* vg);

ret_t vgcanvas_create_fbo(vgcanvas_t* vg, framebuffer_object_t* fbo);
ret_t vgcanvas_destroy_fbo(vgcanvas_t* vg, framebuffer_object_t* fbo);
ret_t vgcanvas_bind_fbo(vgcanvas_t* vg, framebuffer_object_t* fbo);
ret_t vgcanvas_unbind_fbo(vgcanvas_t* vg, framebuffer_object_t* fbo);

END_C_DECLS

#endif /*TK_VG_CANVAS_H*/
