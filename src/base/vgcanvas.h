/**
 * File:   vgcanvas.h
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas interface.
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
 * 2018-03-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VG_CANVAS_H
#define TK_VG_CANVAS_H

#include "tkc/rect.h"
#include "base/types_def.h"
#include "base/bitmap.h"

BEGIN_C_DECLS

/*用于离线渲染*/
typedef struct _framebuffer_object_t {
  int w;
  int h;
  int id;
  void* handle;
  float_t ratio;
  bool_t init;
  int online_fbo;
  int offline_fbo;
  rect_t online_dirty_rect;
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
typedef ret_t (*vgcanvas_path_winding_t)(vgcanvas_t* vg, bool_t dir);

typedef ret_t (*vgcanvas_rotate_t)(vgcanvas_t* vg, float_t rad);
typedef ret_t (*vgcanvas_scale_t)(vgcanvas_t* vg, float_t x, float_t y);
typedef ret_t (*vgcanvas_translate_t)(vgcanvas_t* vg, float_t x, float_t y);
typedef ret_t (*vgcanvas_transform_t)(vgcanvas_t* vg, float_t a, float_t b, float_t c, float_t d,
                                      float_t e, float_t f);
typedef ret_t (*vgcanvas_set_transform_t)(vgcanvas_t* vg, float_t a, float_t b, float_t c,
                                          float_t d, float_t e, float_t f);

typedef ret_t (*vgcanvas_clip_rect_t)(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h);
typedef ret_t (*vgcanvas_nanovg_intersect_clip_rect_t)(vgcanvas_t* vg, float_t* x, float_t* y,
                                                       float_t* w, float_t* h);
typedef ret_t (*vgcanvas_fill_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_stroke_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_paint_t)(vgcanvas_t* vg, bool_t stroke, bitmap_t* img);

typedef ret_t (*vgcanvas_set_font_t)(vgcanvas_t* vg, const char* font);
typedef ret_t (*vgcanvas_set_font_size_t)(vgcanvas_t* vg, float_t size);
typedef ret_t (*vgcanvas_set_text_align_t)(vgcanvas_t* vg, const char* value);
typedef ret_t (*vgcanvas_set_text_baseline_t)(vgcanvas_t* vg, const char* value);
typedef ret_t (*vgcanvas_get_text_metrics_t)(vgcanvas_t* vg, float_t* ascent, float_t* descent,
                                             float_t* line_hight);
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

typedef ret_t (*vgcanvas_set_fill_linear_gradient_t)(vgcanvas_t* vg, float_t sx, float_t sy,
                                                     float_t ex, float_t ey, color_t icolor,
                                                     color_t ocolor);
typedef ret_t (*vgcanvas_set_fill_radial_gradient_t)(vgcanvas_t* vg, float_t cx, float_t cy,
                                                     float_t inr, float_t outr, color_t icolor,
                                                     color_t ocolor);

typedef ret_t (*vgcanvas_set_stroke_color_t)(vgcanvas_t* vg, color_t color);
typedef ret_t (*vgcanvas_set_stroke_linear_gradient_t)(vgcanvas_t* vg, float_t sx, float_t sy,
                                                       float_t ex, float_t ey, color_t icolor,
                                                       color_t ocolor);
typedef ret_t (*vgcanvas_set_stroke_radial_gradient_t)(vgcanvas_t* vg, float_t cx, float_t cy,
                                                       float_t inr, float_t outr, color_t icolor,
                                                       color_t ocolor);

typedef ret_t (*vgcanvas_set_line_cap_t)(vgcanvas_t* vg, const char* value);
typedef ret_t (*vgcanvas_set_line_join_t)(vgcanvas_t* vg, const char* value);
typedef ret_t (*vgcanvas_set_miter_limit_t)(vgcanvas_t* vg, float_t value);

typedef wh_t (*vgcanvas_get_width_t)(vgcanvas_t* vg);
typedef wh_t (*vgcanvas_get_height_t)(vgcanvas_t* vg);

typedef ret_t (*vgcanvas_save_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_restore_t)(vgcanvas_t* vg);

typedef ret_t (*vgcanvas_create_fbo_t)(vgcanvas_t* vg, uint32_t w, uint32_t h,
                                       framebuffer_object_t* fbo);
typedef ret_t (*vgcanvas_destroy_fbo_t)(vgcanvas_t* vg, framebuffer_object_t* fbo);
typedef ret_t (*vgcanvas_bind_fbo_t)(vgcanvas_t* vg, framebuffer_object_t* fbo);
typedef ret_t (*vgcanvas_unbind_fbo_t)(vgcanvas_t* vg, framebuffer_object_t* fbo);
typedef ret_t (*vgcanvas_nanovg_fbo_to_bitmap_t)(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo,
                                                 bitmap_t* img, rect_t* r);

typedef ret_t (*vgcanvas_clear_cache_t)(vgcanvas_t* vg);

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
  vgcanvas_path_winding_t path_winding;

  vgcanvas_scale_t scale;
  vgcanvas_rotate_t rotate;
  vgcanvas_translate_t translate;
  vgcanvas_transform_t transform;
  vgcanvas_set_transform_t set_transform;

  vgcanvas_clip_rect_t clip_rect;
  vgcanvas_nanovg_intersect_clip_rect_t intersect_clip_rect;
  vgcanvas_fill_t fill;
  vgcanvas_stroke_t stroke;
  vgcanvas_paint_t paint;

  vgcanvas_set_font_t set_font;
  vgcanvas_set_font_size_t set_font_size;
  vgcanvas_set_text_align_t set_text_align;
  vgcanvas_set_text_baseline_t set_text_baseline;
  vgcanvas_get_text_metrics_t get_text_metrics;
  vgcanvas_fill_text_t fill_text;
  vgcanvas_measure_text_t measure_text;
  vgcanvas_draw_image_t draw_image;

  vgcanvas_set_antialias_t set_antialias;
  vgcanvas_set_global_alpha_t set_global_alpha;
  vgcanvas_set_line_width_t set_line_width;
  vgcanvas_set_fill_color_t set_fill_color;
  vgcanvas_set_fill_linear_gradient_t set_fill_linear_gradient;
  vgcanvas_set_fill_radial_gradient_t set_fill_radial_gradient;
  vgcanvas_set_stroke_color_t set_stroke_color;
  vgcanvas_set_stroke_linear_gradient_t set_stroke_linear_gradient;
  vgcanvas_set_stroke_radial_gradient_t set_stroke_radial_gradient;
  vgcanvas_set_line_join_t set_line_join;
  vgcanvas_set_line_cap_t set_line_cap;
  vgcanvas_set_miter_limit_t set_miter_limit;

  vgcanvas_save_t save;
  vgcanvas_restore_t restore;
  vgcanvas_end_frame_t end_frame;

  vgcanvas_get_width_t get_width;
  vgcanvas_get_height_t get_height;
  vgcanvas_create_fbo_t create_fbo;
  vgcanvas_destroy_fbo_t destroy_fbo;
  vgcanvas_bind_fbo_t bind_fbo;
  vgcanvas_unbind_fbo_t unbind_fbo;
  vgcanvas_nanovg_fbo_to_bitmap_t fbo_to_bitmap;

  vgcanvas_clear_cache_t clear_cache;

  vgcanvas_destroy_t destroy;
} vgcanvas_vtable_t;

/**
 * @class vgcanvas_t
 * 矢量图画布抽象基类。
 * @annotation ["scriptable"]
 *
 * 具体实现时可以使用agg，nanovg, cairo和skia等方式。
 *
 * cairo和skia体积太大，不适合嵌入式平台，但在PC平台也是一种选择。
 *
 * 目前我们只提供了基于nanovg的实现，支持软件渲染和硬件渲染。
 *
 * 我们对nanovg进行了一些改进:
 *
 * * 可以用agg/agge实现软件渲染(暂时不支持文本绘制)。
 *
 * * 可以用bgfx使用DirectX(Windows平台)和Metal(iOS)平台硬件加速。
 *
 * ```graphviz
 *   [default_style]
 *
 *   vgcanvas_cairo_t -> vgcanvas_t[arrowhead = "empty"]
 *   vgcanvas_nanovg_t -> vgcanvas_t[arrowhead = "empty"]
 *   opengl -> vgcanvas_nanovg_t[arrowhead = "none"]
 *   bgfx -> vgcanvas_nanovg_t[arrowhead = "none"]
 *   agg -> vgcanvas_nanovg_t[arrowhead = "none"]
 *   agge -> vgcanvas_nanovg_t[arrowhead = "none"]
 *   vgcanvas_skia_t -> vgcanvas_t[arrowhead = "empty"]
 *   vgcanvas_agge_t -> vgcanvas_t[arrowhead = "empty"]
 * ```
 *
 * 示例：
 *
 * ```c
 *   vgcanvas_t* vg = canvas_get_vgcanvas(c);
 *   vgcanvas_save(vg);
 *   vgcanvas_translate(vg, 0, 100);
 *
 *   vgcanvas_set_line_width(vg, 1);
 *   vgcanvas_set_fill_color(vg, color_init(0xff, 0, 0, 0xff));
 *   vgcanvas_rect(vg, 5, 5, 100, 100);
 *   vgcanvas_fill(vg);
 *   vgcanvas_restore(vg);
 * ```
 *
 * >请参考：https://www.w3schools.com/tags/ref_canvas.asp
 *
 */
struct _vgcanvas_t {
  /**
   * @property {wh_t} w
   * @annotation ["readable", "scriptable"]
   * canvas的宽度
   */
  uint32_t w;
  /**
   * @property {wh_t} h
   * @annotation ["readable", "scriptable"]
   * canvas的高度
   */
  uint32_t h;
  /**
   * @property {uint32_t} stride
   * @annotation ["readable", "scriptable"]
   * 一行占的字节
   */
  uint32_t stride;
  /**
   * @property {float_t} ratio
   * @annotation ["readable", "scriptable"]
   * 显示比例。
   */
  float_t ratio;
  /**
   * @property {bool_t} anti_alias
   * @annotation ["readable", "scriptable"]
   * 是否启用反走样功能。
   */
  bool_t anti_alias;
  /**
   * @property {float_t} line_width
   * @annotation ["readable", "scriptable"]
   * 线宽。
   */
  float_t line_width;
  /**
   * @property {float_t} global_alpha
   * @annotation ["readable", "scriptable"]
   * 全局alpha。
   */
  float_t global_alpha;
  /**
   * @property {float_t} miter_limit
   * @annotation ["readable", "scriptable"]
   * miter\_limit。
   * @see http://www.w3school.com.cn/tags/canvas_miterlimit.asp
   */
  float_t miter_limit;
  /**
   * @property {const char*} line_cap
   * @annotation ["readable", "scriptable"]
   * line\_cap。
   * @see http://www.w3school.com.cn/tags/canvas_linecap.asp
   */
  const char* line_cap;
  /**
   * @property {const char*} line_join
   * @annotation ["readable", "scriptable"]
   * line\_join。
   * @see http://www.w3school.com.cn/tags/canvas_linejoin.asp
   */
  const char* line_join;
  /**
   * @property {char*} font
   * @annotation ["readable", "scriptable"]
   * 字体。
   */
  char* font;
  /**
   * @property {float_t} font_size
   * @annotation ["readable", "scriptable"]
   * 字体大小。
   */
  float_t font_size;
  /**
   * @property {const char*} text_align
   * @annotation ["readable", "scriptable"]
   * 文本对齐方式。
   *
   * @see http://www.w3school.com.cn/tags/canvas_textalign.asp
   */
  char* text_align;
  /**
   * @property {const char*} text_baseline
   * @annotation ["readable", "scriptable"]
   * 文本基线。
   *
   * @see http://www.w3school.com.cn/tags/canvas_textbaseline.asp
   */
  char* text_baseline;

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

  /**
   * @property {bitmap_format_t} format;
   * @annotation ["private"]
   * frame buffer format
   */
  bitmap_format_t format;
  rect_t clip_rect;
  rect_t dirty_rect;
  const vgcanvas_vtable_t* vt;
  assets_manager_t* assets_manager;
};

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
 * @method vgcanvas_cast
 * 转换为vgcanvas对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {vgcanvas_t*} vgcanvas对象。
 */
vgcanvas_t* vgcanvas_cast(vgcanvas_t* vg);

/**
 * @method vgcanvas_reinit
 * 重新初始化，系统内部调用。
 *
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
 * @method vgcanvas_reset
 * 重置状态。
 *
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_reset(vgcanvas_t* vg);

/**
 * @method vgcanvas_flush
 * flush
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_flush(vgcanvas_t* vg);

/**
 * @method vgcanvas_begin_frame
 * 开始绘制，系统内部调用。
 *
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {rect_t*} dirty_rect 需要绘制的区域。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_begin_frame(vgcanvas_t* vg, rect_t* dirty_rect);

/**
 * @method vgcanvas_clear_rect
 * 用颜色清除指定矩形区域。
 *
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
 * @method vgcanvas_begin_path
 * 清除之前的路径，并重新开始一条路径。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_begin_path(vgcanvas_t* vg);

/**
 * @method vgcanvas_move_to
 * 移动当前点到指定点。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_move_to(vgcanvas_t* vg, float_t x, float_t y);

/**
 * @method vgcanvas_line_to
 * 生成一条线段(从当前点到目标点)。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_line_to(vgcanvas_t* vg, float_t x, float_t y);

/**
 * @method vgcanvas_quad_to
 * 生成一条二次贝塞尔曲线。
 *
 * @annotation ["scriptable"]
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
 * 生成一条三次贝塞尔曲线。
 *
 * @annotation ["scriptable"]
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
 * 生成一条圆弧路径到指定点。
 *
 * @annotation ["scriptable"]
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
 * 生成一条圆弧。
 *
 * @annotation ["scriptable"]
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
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 *
 * @return {bool_t} 返回TRUE表示在，否则表示不在。
 */
bool_t vgcanvas_is_point_in_path(vgcanvas_t* vg, float_t x, float_t y);

/**
 * @method vgcanvas_rect
 * 生成一个矩形路径。
 *
 * @annotation ["scriptable"]
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
 * 生成一个圆角矩形路径。
 *
 * @annotation ["scriptable"]
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
 * 生成一个椭圆路径。
 *
 * @annotation ["scriptable"]
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
 * 闭合路径。
 *
 * >闭合路径是指把起点和终点连接起来，形成一个封闭的多边形。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_close_path(vgcanvas_t* vg);

/**
 * @method vgcanvas_path_winding
 * 设置路径填充实心与否。
 *
 * >CCW(1)为实心，CW(2)为镂空，设置其他则默认根据非零环绕规则判断(nonzero)。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {bool_t} dir 填充方法。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_path_winding(vgcanvas_t* vg, bool_t dir);

/**
 * @method vgcanvas_rotate
 * 旋转。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} rad 角度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_rotate(vgcanvas_t* vg, float_t rad);

/**
 * @method vgcanvas_scale
 * 缩放。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x方向缩放比例。
 * @param {float_t} y y方向缩放比例。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_scale(vgcanvas_t* vg, float_t x, float_t y);

/**
 * @method vgcanvas_translate
 * 平移。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x方向偏移。
 * @param {float_t} y y方向偏移。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_translate(vgcanvas_t* vg, float_t x, float_t y);

/**
 * @method vgcanvas_transform
 * 变换矩阵。
 *
 * @annotation ["scriptable"]
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
 * 设置变换矩阵。
 *
 * @annotation ["scriptable"]
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
 * 矩形裁剪。
 *
 * @annotation ["scriptable"]
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
 * @method vgcanvas_intersect_clip_rect
 * 设置一个与前一个裁剪区做交集的矩形裁剪区。
 * 如果下面这种情况，则不能直接调用 rect_intersect 函数来做矩形交集和 vgcanvas_clip_rect 函数设置裁剪区，而采用本函数做交集。
 * 由于缩放和旋转以及平移会导致 vg 的坐标系和上一个裁剪区的坐标系不同，
 * 导致直接使用做交集的话，裁剪区会出错。
 * 
 * ```
 * vgcanvas_clip_rect(vg, old_r.x, old_r.y, old_r.w, old_r.h);
 * vgcanvas_save(vg);
 * vgcanvas_scale(vg, scale_x, scale_y);
 * vgcanvas_rotate(vg, TK_D2R(15));
 * vgcanvas_intersect_clip_rect(vg, r.x, r.y, r.w, r.h);
 * ..................
 * vgcanvas_restore(vg);
 * ```
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} x x坐标。
 * @param {float_t} y y坐标。
 * @param {float_t} w 宽度。
 * @param {float_t} h 高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_intersect_clip_rect(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h);

/**
 * @method vgcanvas_fill
 * 填充多边形。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_fill(vgcanvas_t* vg);

/**
 * @method vgcanvas_stroke
 * 画线。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_stroke(vgcanvas_t* vg);

/**
 * @method vgcanvas_paint
 * 用图片填充/画多边形(可能存在可移植性问题，除非必要请勿使用)。
 * 多边形的顶点必须在图片范围内，可以通过矩阵变化画到不同的位置。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {bool_t} stroke TRUE表示画线FALSE表示填充。
 * @param {bitmap_t*} img 图片。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_paint(vgcanvas_t* vg, bool_t stroke, bitmap_t* img);

/**
 * @method vgcanvas_set_font
 * 设置字体的名称。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} font 字体名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_font(vgcanvas_t* vg, const char* font);

/**
 * @method vgcanvas_set_font_size
 * 设置字体的大小。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} font 字体大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_font_size(vgcanvas_t* vg, float_t size);

/**
 * @method vgcanvas_set_text_align
 * 设置文本水平对齐的方式。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} value 取值：left|center|right，必须为常量字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_text_align(vgcanvas_t* vg, const char* value);

/**
 * @method vgcanvas_set_text_baseline
 * 设置文本垂直对齐的方式。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} value 取值：top|middle|bottom，必须为常量字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_text_baseline(vgcanvas_t* vg, const char* value);

/**
 * @method vgcanvas_get_text_metrics
 * 获取当前字体的度量信息。
 *
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t*} ascent 用于返回ascent。
 * @param {float_t*} descent 用于返回descent。
 * @param {float_t*} line_hight 用于返回line height。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_get_text_metrics(vgcanvas_t* vg, float_t* ascent, float_t* descent,
                                float_t* line_hight);

/**
 * @method vgcanvas_fill_text
 * 绘制文本。
 *
 * @annotation ["scriptable"]
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
 * 测量文本的宽度。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} text text
 *
 * @return {float_t} 返回text的宽度。
 */
float_t vgcanvas_measure_text(vgcanvas_t* vg, const char* text);

/**
 * @method vgcanvas_draw_image
 * 绘制图片。
 *
 * @annotation ["scriptable"]
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
 * 绘制图标。
 *
 * 绘制图标时会根据屏幕密度进行自动缩放，而绘制普通图片时不会。
 *
 * @annotation ["scriptable"]
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
 * 设置是否启用反走样。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {bool_t} value 是否启用反走样。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_antialias(vgcanvas_t* vg, bool_t value);

/**
 * @method vgcanvas_set_global_alpha
 * 设置全局透明度。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} alpha global alpha。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_global_alpha(vgcanvas_t* vg, float_t alpha);

/**
 * @method vgcanvas_set_line_width
 * 设置线条的宽度。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} value 线宽。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_line_width(vgcanvas_t* vg, float_t value);

/**
 * @method vgcanvas_set_fill_color
 * 设置填充颜色。
 *
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {color_t} color 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_fill_color(vgcanvas_t* vg, color_t color);

/**
 * @method vgcanvas_set_fill_color_str
 * 设置填充颜色。
 *
 * @alias vgcanvas_set_fill_color
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {const char*} color 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_fill_color_str(vgcanvas_t* vg, const char* color);

/**
 * @method vgcanvas_set_fill_linear_gradient
 * 设置填充颜色为线性渐变色。
 *
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} sx start x
 * @param {float_t} sy start y
 * @param {float_t} ex end x
 * @param {float_t} ey end y
 * @param {color_t} icolor 开始颜色。
 * @param {color_t} ocolor 结束颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_fill_linear_gradient(vgcanvas_t* vg, float_t sx, float_t sy, float_t ex,
                                        float_t ey, color_t icolor, color_t ocolor);

/**
 * @method vgcanvas_set_fill_radial_gradient
 * 设置填充颜色为径向渐变色。
 *
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} cx center x
 * @param {float_t} cy center y
 * @param {float_t} inr 内环半径
 * @param {float_t} outr 外环半径。
 * @param {color_t} icolor 开始颜色。
 * @param {color_t} ocolor 结束颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_fill_radial_gradient(vgcanvas_t* vg, float_t cx, float_t cy, float_t inr,
                                        float_t outr, color_t icolor, color_t ocolor);

/**
 * @method vgcanvas_set_stroke_color
 * 设置线条颜色。
 *
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {color_t} color 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_stroke_color(vgcanvas_t* vg, color_t color);

/**
 * @method vgcanvas_set_stroke_color_str
 * 设置线条颜色。
 *
 * @alias vgcanvas_set_stroke_color
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {const char*} color 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_stroke_color_str(vgcanvas_t* vg, const char* str);

/**
 * @method vgcanvas_set_stroke_linear_gradient
 * 设置线条颜色为线性渐变色。
 *
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} sx start x
 * @param {float_t} sy start y
 * @param {float_t} ex end x
 * @param {float_t} ey end y
 * @param {color_t} icolor 开始颜色。
 * @param {color_t} ocolor 结束颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_stroke_linear_gradient(vgcanvas_t* vg, float_t sx, float_t sy, float_t ex,
                                          float_t ey, color_t icolor, color_t ocolor);

/**
 * @method vgcanvas_set_stroke_radial_gradient
 * 设置线条颜色为径向渐变色。
 *
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} cx center x
 * @param {float_t} cy center y
 * @param {float_t} inr 内环半径
 * @param {float_t} outr 外环半径。
 * @param {color_t} icolor 开始颜色。
 * @param {color_t} ocolor 结束颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_stroke_radial_gradient(vgcanvas_t* vg, float_t cx, float_t cy, float_t inr,
                                          float_t outr, color_t icolor, color_t ocolor);

/**
 * @method vgcanvas_set_line_cap
 * 设置line cap。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} value 取值：butt|round|square，必须为常量字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_line_cap(vgcanvas_t* vg, const char* value);

/**
 * @method vgcanvas_set_line_join
 * 设置line join。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {char*} value 取值：bevel|round|miter，必须为常量字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_line_join(vgcanvas_t* vg, const char* value);

/**
 * @method vgcanvas_set_miter_limit
 * 设置miter limit。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 * @param {float_t} value miter limit
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_set_miter_limit(vgcanvas_t* vg, float_t value);

/**
 * @method vgcanvas_save
 * 保存当前的状态。如颜色和矩阵等信息。
 *
 * > save/restore必须配套使用，否则可能导致状态混乱。
 *
 * @annotation ["scriptable"]
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_save(vgcanvas_t* vg);

/**
 * @method vgcanvas_restore
 * 恢复上次save的状态。
 *
 *> save/restore必须配套使用，否则可能导致状态混乱。
 *
 * @annotation ["scriptable"]
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
 * @method vgcanvas_get_width
 * 获取宽度。
 * @param {vgcanvas_t*} vgcanvas vgcanvas对象。
 *
 * @return {wh_t} 返回宽度。
 */
wh_t vgcanvas_get_width(vgcanvas_t* vgcanvas);

/**
 * @method vgcanvas_get_height
 * 获取高度。
 * @param {vgcanvas_t*} vgcanvas vgcanvas对象。
 *
 * @return {wh_t} 返回高度。
 */
wh_t vgcanvas_get_height(vgcanvas_t* vgcanvas);

/**
 * @method vgcanvas_clear_cache
 * 释放vgcanvas对象的缓冲数据。
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_clear_cache(vgcanvas_t* vg);

/**
 * @method vgcanvas_destroy
 * 销毁vgcanvas对象。
 * @param {vgcanvas_t*} vg vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_destroy(vgcanvas_t* vg);

ret_t vgcanvas_create_fbo(vgcanvas_t* vg, uint32_t w, uint32_t h, framebuffer_object_t* fbo);
ret_t vgcanvas_destroy_fbo(vgcanvas_t* vg, framebuffer_object_t* fbo);
ret_t vgcanvas_bind_fbo(vgcanvas_t* vg, framebuffer_object_t* fbo);
ret_t vgcanvas_unbind_fbo(vgcanvas_t* vg, framebuffer_object_t* fbo);
ret_t vgcanvas_fbo_to_bitmap(vgcanvas_t* vg, framebuffer_object_t* fbo, bitmap_t* img, rect_t* r);
ret_t fbo_to_img(framebuffer_object_t* fbo, bitmap_t* img);
ret_t vgcanvas_set_assets_manager(vgcanvas_t* vg, assets_manager_t* assets_manager);

/**
 * @enum vgcanvas_line_cap_t
 * @annotation ["scriptable", "string"]
 * @prefix VGCANVAS_LINE_CAP_
 * 线帽类型。
 */
/**
 * @const VGCANVAS_LINE_CAP_ROUND
 * 圆头。
 */
#define VGCANVAS_LINE_CAP_ROUND "round"

/**
 * @const VGCANVAS_LINE_CAP_SQUARE
 * 方头。 
 */
#define VGCANVAS_LINE_CAP_SQUARE "square"

/**
 * @enum vgcanvas_line_join_t
 * @annotation ["scriptable", "string"]
 * @prefix VGCANVAS_LINE_JOIN_
 * 线条连接类型。
 */
/**
 * @const VGCANVAS_LINE_JOIN_ROUND
 * round。
 */
#define VGCANVAS_LINE_JOIN_ROUND "round"

/**
 * @const VGCANVAS_LINE_JOIN_BEVEL
 * bevel。 
 */
#define VGCANVAS_LINE_JOIN_BEVEL "bevel"

/**
 * @const VGCANVAS_LINE_JOIN_MITTER
 * mitter。 
 */
#define VGCANVAS_LINE_JOIN_MITTER "mitter"

END_C_DECLS

#endif /*TK_VG_CANVAS_H*/
