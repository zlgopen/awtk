/**
 * File:   lcd.h
 * Author: AWTK Develop Team
 * Brief:  lcd interface
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

#ifndef TK_LCD_H
#define TK_LCD_H

#include "tkc/rect.h"
#include "base/font.h"
#include "tkc/matrix.h"
#include "base/bitmap.h"
#include "base/vgcanvas.h"

BEGIN_C_DECLS

typedef struct _draw_image_info_t {
  bitmap_t* img;
  rect_t src;
  rect_t dst;
  rect_t clip;
  matrix_t matrix;
} draw_image_info_t;

struct _lcd_t;
typedef struct _lcd_t lcd_t;

typedef ret_t (*lcd_begin_frame_t)(lcd_t* lcd, rect_t* dirty_rect);
typedef ret_t (*lcd_set_clip_rect_t)(lcd_t* lcd, rect_t* rect);
typedef ret_t (*lcd_get_clip_rect_t)(lcd_t* lcd, rect_t* rect);
typedef ret_t (*lcd_resize_t)(lcd_t* lcd, wh_t w, wh_t h, uint32_t line_length);
typedef ret_t (*lcd_get_text_metrics_t)(lcd_t* lcd, float_t* ascent, float_t* descent,
                                        float_t* line_hight);

typedef ret_t (*lcd_set_global_alpha_t)(lcd_t* lcd, uint8_t alpha);
typedef ret_t (*lcd_set_text_color_t)(lcd_t* lcd, color_t color);
typedef ret_t (*lcd_set_stroke_color_t)(lcd_t* lcd, color_t color);
typedef ret_t (*lcd_set_fill_color_t)(lcd_t* lcd, color_t color);
typedef ret_t (*lcd_set_font_name_t)(lcd_t* lcd, const char* name);
typedef ret_t (*lcd_set_font_size_t)(lcd_t* lcd, uint32_t size);

typedef ret_t (*lcd_draw_image_repeat_t)(lcd_t* lcd, bitmap_t* img, rect_t* src_in, rect_t* dst_in,
                                         wh_t dst_w, wh_t dst_h);

typedef ret_t (*lcd_draw_vline_t)(lcd_t* lcd, xy_t x, xy_t y, wh_t h);
typedef ret_t (*lcd_draw_hline_t)(lcd_t* lcd, xy_t x, xy_t y, wh_t w);
typedef ret_t (*lcd_draw_points_t)(lcd_t* lcd, point_t* points, uint32_t nr);
typedef color_t (*lcd_get_point_color_t)(lcd_t* lcd, xy_t x, xy_t y);

typedef ret_t (*lcd_fill_rect_t)(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h);
typedef ret_t (*lcd_stroke_rect_t)(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h);

typedef ret_t (*lcd_draw_glyph_t)(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y);
typedef float_t (*lcd_measure_text_t)(lcd_t* lcd, const wchar_t* str, uint32_t nr);
typedef ret_t (*lcd_draw_text_t)(lcd_t* lcd, const wchar_t* str, uint32_t nr, xy_t x, xy_t y);

typedef ret_t (*lcd_draw_image_t)(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst);
typedef ret_t (*lcd_draw_image_matrix_t)(lcd_t* lcd, draw_image_info_t* info);
typedef vgcanvas_t* (*lcd_get_vgcanvas_t)(lcd_t* lcd);
typedef ret_t (*lcd_take_snapshot_t)(lcd_t* lcd, bitmap_t* img, bool_t auto_rotate);
typedef bitmap_format_t (*lcd_get_desired_bitmap_format_t)(lcd_t* lcd);

typedef wh_t (*lcd_get_width_t)(lcd_t* lcd);
typedef wh_t (*lcd_get_height_t)(lcd_t* lcd);
typedef ret_t (*lcd_swap_t)(lcd_t* lcd);
typedef ret_t (*lcd_flush_t)(lcd_t* lcd);
typedef ret_t (*lcd_sync_t)(lcd_t* lcd);
typedef ret_t (*lcd_end_frame_t)(lcd_t* lcd);
typedef ret_t (*lcd_destroy_t)(lcd_t* lcd);

/**
 * @enum lcd_draw_mode_t
 * @prefix LCD_DRAW_
 * LCD绘制模式常量定义。
 */
typedef enum _lcd_draw_mode_t {
  /**
   * @const LCD_DRAW_NORMAL
   * 正常绘制。
   */
  LCD_DRAW_NORMAL = 0,

  /**
   * @const LCD_DRAW_ANIMATION
   * 绘制窗口动画，两个窗口无重叠。
   * 在该模式下，可以直接绘制到显存，不用绘制到framebuffer中。
   */
  LCD_DRAW_ANIMATION,

  /**
   * @const LCD_DRAW_ANIMATION_OVERLAP
   * 绘制窗口动画，两个窗口有重叠。
   * (目前无特殊用途)。
   */
  LCD_DRAW_ANIMATION_OVERLAP,

  /**
   * @const LCD_DRAW_SWAP
   * 如果lcd支持swap操作，在特殊情况下，可以使用该模式提速。
   */
  LCD_DRAW_SWAP,

  /**
   * @const LCD_DRAW_OFFLINE
   * 离线模式绘制(仅适用于framebuffer)。
   * 在该模式下，绘制的内容不会显示出来，但可以用take_snapshot取出来，主要用于窗口动画。
   */
  LCD_DRAW_OFFLINE
} lcd_draw_mode_t;

/**
 * @enum lcd_type_t
 * @prefix LCD_
 * LCD类型常量定义。
 */
typedef enum _lcd_type_t {
  /**
   * @const LCD_FRAMEBUFFER
   * 基于FrameBuffer的LCD。
   */
  LCD_FRAMEBUFFER = 0,
  /**
   * @const LCD_REGISTER
   * 基于寄存器的LCD。
   */
  LCD_REGISTER,
  /**
   * @const LCD_VGCANVAS
   * 基于VGCANVS的LCD。仅在支持OpenGL时，用nanovg实现。
   */
  LCD_VGCANVAS,
  /**
   * @const LCD_MONO
   * 单色LCD。
   */
  LCD_MONO
} lcd_type_t;

/**
 * @class lcd_t
 * 显示设备抽象基类。
 */
struct _lcd_t {
  lcd_begin_frame_t begin_frame;
  lcd_set_clip_rect_t set_clip_rect;
  lcd_get_clip_rect_t get_clip_rect;
  lcd_set_global_alpha_t set_global_alpha;
  lcd_set_text_color_t set_text_color;
  lcd_set_stroke_color_t set_stroke_color;
  lcd_set_fill_color_t set_fill_color;
  lcd_set_font_name_t set_font_name;
  lcd_set_font_size_t set_font_size;
  lcd_get_text_metrics_t get_text_metrics;
  lcd_draw_vline_t draw_vline;
  lcd_draw_hline_t draw_hline;
  lcd_fill_rect_t fill_rect;
  lcd_stroke_rect_t stroke_rect;
  lcd_draw_image_t draw_image;
  lcd_draw_image_matrix_t draw_image_matrix;
  lcd_draw_glyph_t draw_glyph;
  lcd_draw_text_t draw_text;
  lcd_measure_text_t measure_text;
  lcd_draw_points_t draw_points;
  lcd_draw_image_repeat_t draw_image_repeat;
  lcd_get_point_color_t get_point_color;
  lcd_swap_t swap; /*适用于double fb，可选*/
  lcd_get_width_t get_width;
  lcd_get_height_t get_height;
  lcd_flush_t flush;
  lcd_sync_t sync;
  lcd_end_frame_t end_frame;
  lcd_get_vgcanvas_t get_vgcanvas;
  lcd_take_snapshot_t take_snapshot;
  lcd_get_desired_bitmap_format_t get_desired_bitmap_format;
  lcd_resize_t resize;
  lcd_destroy_t destroy;

  /**
   * @property {wh_t} w
   * @annotation ["readable"]
   * 屏幕的宽度
   */
  wh_t w;
  /**
   * @property {wh_t} height
   * @annotation ["readable"]
   * 屏幕的高度
   */
  wh_t h;
  /**
   * @property {uint8_t} global_alpha
   * @annotation ["readable"]
   * 全局alpha
   */
  uint8_t global_alpha;
  /**
   * @property {color_t} text_color
   * @annotation ["readable"]
   * 文本颜色
   */
  color_t text_color;
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
   * @property {char*} font_name
   * @annotation ["readable"]
   * 字体名称。
   */
  const char* font_name;
  /**
   * @property {uint32_t} font_size
   * @annotation ["readable"]
   * 字体大小。
   */
  uint32_t font_size;

  /**
   * @property {lcd_draw_mode_t} draw_mode
   * @annotation ["readable"]
   * 绘制模式。
   */
  lcd_draw_mode_t draw_mode;

  /**
   * @property {lcd_type_t} type
   * @annotation ["readable"]
   * LCD的类型。
   */
  lcd_type_t type;

  /**
   * @property {float_t} ratio
   * @annotation ["readable"]
   * 屏幕密度。
   */
  float_t ratio;

  /**
   * @property {bool_t} support_dirty_rect
   * @annotation ["readable"]
   * 是否支持脏矩形。
   */
  bool_t support_dirty_rect;

  /**
   * @property {uint64_t} last_update_time
   * @annotation ["readable"]
   * last update time
   */
  uint64_t last_update_time;

  /*private*/
  rect_t fps_rect;
  rect_t dirty_rect;
  void* impl_data;
};

/**
 * @method lcd_begin_frame
 * 准备绘制。
 * @param {lcd_t*} lcd lcd对象。
 * @param {rect_t*} dirty_rect 需要绘制的区域。
 * @param {lcd_draw_mode_t} anim_mode 动画模式，如果可能，直接画到显存而不是离线的framebuffer。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_begin_frame(lcd_t* lcd, rect_t* dirty_rect, lcd_draw_mode_t draw_mode);

/**
 * @method lcd_set_clip_rect
 * 设置裁剪区域。
 * @param {lcd_t*} lcd lcd对象。
 * @param {rect_t*} rect 裁剪区域。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_set_clip_rect(lcd_t* lcd, rect_t* rect);

/**
 * @method lcd_get_clip_rect
 * 获取裁剪区域。
 * @param {lcd_t*} lcd lcd对象。
 * @param {rect_t*} rect 裁剪区域。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_get_clip_rect(lcd_t* lcd, rect_t* rect);

/**
 * @method lcd_resize
 * 基于SDL的PC软件，在SDL窗口resize时，需要调用本函数resize lcd。
 * 屏幕旋转时会调用本函数，调整LCD的大小。
 * @param {lcd_t*} lcd lcd对象。
 * @param {wh_t} w 新的宽度。
 * @param {wh_t} h 新的高度。
 * @param {uint32_t} line_length line_length。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_resize(lcd_t* lcd, wh_t w, wh_t h, uint32_t line_length);

/**
 * @method lcd_set_global_alpha
 * 设置全局alpha。
 * @param {lcd_t*} lcd lcd对象。
 * @param {uint8_t} alpha 全局alpha。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_set_global_alpha(lcd_t* lcd, uint8_t alpha);

/**
 * @method lcd_set_text_color
 * 设置文本颜色。
 * @param {lcd_t*} lcd lcd对象。
 * @param {color_t} color 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_set_text_color(lcd_t* lcd, color_t color);

/**
 * @method lcd_set_stroke_color
 * 设置线条颜色。
 * @param {lcd_t*} lcd lcd对象。
 * @param {color_t} color 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_set_stroke_color(lcd_t* lcd, color_t color);

/**
 * @method lcd_set_fill_color
 * 设置填充颜色。
 * @param {lcd_t*} lcd lcd对象。
 * @param {color_t} color 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_set_fill_color(lcd_t* lcd, color_t color);

/**
 * @method lcd_set_font_name
 * 设置字体名称。
 * @param {lcd_t*} lcd lcd对象。
 * @param {const char*} name 字体名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_set_font_name(lcd_t* lcd, const char* name);

/**
 * @method lcd_set_font_size
 * 设置字体大小。
 * @param {lcd_t*} lcd lcd对象。
 * @param {uint32_t} font_size 字体大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_set_font_size(lcd_t* lcd, uint32_t font_size);

/**
 * @method lcd_draw_vline
 * 绘制一条垂直线。
 * @param {lcd_t*} lcd lcd对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 * @param {xy_t} h 直线高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h);

/**
 * @method lcd_draw_hline
 * 绘制一条水平线。
 * @param {lcd_t*} lcd lcd对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 * @param {xy_t} w 直线宽度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w);

/**
 * @method lcd_draw_points
 * 绘制一组点。
 * @param {lcd_t*} lcd lcd对象。
 * @param {point_t*} points 要绘制的点集合。
 * @param {uint32_t} nr 点的个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_draw_points(lcd_t* lcd, point_t* points, uint32_t nr);

/**
 * @method lcd_get_point_color
 * 获取指定点的颜色，对于基于非FrameBuffer的LCD，返回当前的fill_color。
 * @param {lcd_t*} lcd lcd对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 *
 * @return {color_t} 返回RET_OK表示成功，否则表示失败。
 */
color_t lcd_get_point_color(lcd_t* lcd, xy_t x, xy_t y);

/**
 * @method lcd_fill_rect
 * 绘制实心矩形。
 * @param {lcd_t*} lcd lcd对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 * @param {wh_t} w 宽度。
 * @param {wh_t} h 高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method lcd_stroke_rect
 * 绘制矩形。
 * @param {lcd_t*} lcd lcd对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 * @param {wh_t} w 宽度。
 * @param {wh_t} h 高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_stroke_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method lcd_draw_glyph
 * 绘制字符。如果实现了measure_text/draw_text则不需要实现本函数。
 * @param {lcd_t*} lcd lcd对象。
 * @param {glyph_t*} glyph 字模
 * @param {rect_t*} src 只绘制指定区域的部分。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y);

/**
 * @method lcd_measure_text
 * 测量字符串占用的宽度。
 * @param {lcd_t*} lcd lcd对象。
 * @param {const wchar_t*} str 字符串。
 * @param {uint32_t} nr 字符数。
 *
 * @return {float_t} 返回字符串占用的宽度。
 */
float_t lcd_measure_text(lcd_t* lcd, const wchar_t* str, uint32_t nr);

/**
 * @method lcd_draw_text
 * 绘制字符。
 * @param {lcd_t*} lcd lcd对象。
 * @param {const wchar_t*} str 字符串。
 * @param {uint32_t} nr 字符数。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_draw_text(lcd_t* lcd, const wchar_t* str, uint32_t nr, xy_t x, xy_t y);

/**
 * @method lcd_draw_image
 * 绘制图片。
 * @param {lcd_t*} lcd lcd对象。
 * @param {bitmap_t*} img 图片。
 * @param {rect_t*} src 只绘制指定区域的部分。
 * @param {rect_t*} dst 绘制到目标区域。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst);

/**
 * @method lcd_draw_image_matrix
 * 绘制图片。
 * @param {lcd_t*} lcd lcd对象。
 * @param {draw_image_info_t*} info 绘制参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_draw_image_matrix(lcd_t* lcd, draw_image_info_t* info);

/**
 * @method lcd_get_vgcanvas
 * 获取矢量图canvas。
 * @param {lcd_t*} lcd lcd对象。
 *
 * @return {vgcanvas_t*} 返回矢量图canvas。
 */
vgcanvas_t* lcd_get_vgcanvas(lcd_t* lcd);

/**
 * @method lcd_take_snapshot
 * 拍摄快照，一般用于窗口动画，只有framebuffer模式，才支持。
 * @param {lcd_t*} lcd lcd对象。
 * @param {bitmap_t*} img 返回快照图片。
 * @param {bool_t} auto_rotate 是否根据LCD实际方向自动旋转。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_take_snapshot(lcd_t* lcd, bitmap_t* img, bool_t auto_rotate);

/**
 * @method lcd_get_desired_bitmap_format
 * 获取期望的位图格式。绘制期望的位图格式可以提高绘制性能。
 * @param {lcd_t*} lcd lcd对象。
 *
 * @return {bitmap_format_t} 返回期望的位图格式。
 */
bitmap_format_t lcd_get_desired_bitmap_format(lcd_t* lcd);

/**
 * @method lcd_swap
 * 对于double fb，如果硬件支持swap，在LCD_DRAW_SWAP模式下，该函数用于切换fb。
 * @annotation ["private"]
 * @param {lcd_t*} lcd lcd对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_swap(lcd_t* lcd);

/**
 * @method lcd_get_width
 * 获取宽度。
 * @param {lcd_t*} lcd lcd对象。
 *
 * @return {wh_t} 返回宽度。
 */
wh_t lcd_get_width(lcd_t* lcd);

/**
 * @method lcd_get_height
 * 获取高度。
 * @param {lcd_t*} lcd lcd对象。
 *
 * @return {wh_t} 返回高度。
 */
wh_t lcd_get_height(lcd_t* lcd);

/**
 * @method lcd_flush
 * flush。
 * @annotation ["private"]
 * @param {lcd_t*} lcd lcd对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_flush(lcd_t* lcd);

/**
 * @method lcd_sync
 * sync。
 * @annotation ["private"]
 * @param {lcd_t*} lcd lcd对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_sync(lcd_t* lcd);

/**
 * @method lcd_is_swappable
 * 判读lcd是否支持swap。
 * @param {lcd_t*} lcd lcd对象。
 *
 * @return {bool_t} 返回是否支持swap。
 */
bool_t lcd_is_swappable(lcd_t* lcd);

/**
 * @method lcd_end_frame
 * 完成绘制，同步到显示设备。
 * @param {lcd_t*} lcd lcd对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_end_frame(lcd_t* lcd);

/**
 * @method lcd_get_text_metrics
 * 获取当前字体的度量信息。
 *
 * @param {lcd_t*} lcd lcd对象。
 * @param {float_t*} ascent 用于返回ascent。
 * @param {float_t*} descent 用于返回descent。
 * @param {float_t*} line_hight 用于返回line height。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_get_text_metrics(lcd_t* lcd, float_t* ascent, float_t* descent, float_t* line_hight);

/**
 * @method lcd_destroy
 * 销毁lcd对象。
 * @param {lcd_t*} lcd lcd对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_destroy(lcd_t* lcd);

END_C_DECLS

#endif /*TK_LCD_H*/
