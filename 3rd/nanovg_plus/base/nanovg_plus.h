/**
 * File:   nanovg_plus.h
 * Author: AWTK Develop Team
 * Brief:  nanovg plus head file.
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-10-27 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#ifndef __NANOVG_PLUS_H__
#define __NANOVG_PLUS_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "nanovg_plus_type.h"
#include "nvgp_darray.h"

#ifndef NVGP_INIT_STATES
#define NVGP_INIT_STATES 16
#endif

#ifndef NVGP_INIT_COMMANDS_SIZE
#define NVGP_INIT_COMMANDS_SIZE 256
#endif

#ifndef NVGP_INIT_CACHE_PATHS_SIZE
#define NVGP_INIT_CACHE_PATHS_SIZE 16
#endif

#ifndef NVGP_INIT_CACHE_POINTS_SIZE
#define NVGP_INIT_CACHE_POINTS_SIZE 128
#endif

#ifndef NVGP_INIT_CACHE_VERTS_SIZE 
#define NVGP_INIT_CACHE_VERTS_SIZE  256
#endif

typedef enum _nvgp_mode_t {
  NVGP_MODE_NONE = 0,
  NVGP_MODE_CPU,
  NVGP_MODE_GPU,

  NVGP_MODE_SPECIAL,
} nvgp_mode_t;

typedef enum _nvgp_line_cap_t {
  NVGP_BUTT = 0,
  NVGP_ROUND,
  NVGP_SQUARE,
} nvgp_line_cap_t;

typedef enum _nvgp_line_join_t {
  NVGP_MITER = 0,
  NVGP_BEVEL,
} nvgp_line_join_t;

typedef enum _nvgp_texture_t {
  NVGP_TEXTURE_ALPHA = 1,
  NVGP_TEXTURE_RGBA = 2,
  NVGP_TEXTURE_BGRA = 4,
  NVGP_TEXTURE_RGB  = 8,
  NVGP_TEXTURE_BGR  = 16,
  NVGP_TEXTURE_RGB565 = 32,
  NVGP_TEXTURE_BGR565 = 64
}nvgp_texture_t;

typedef enum _nvgp_draw_image_type_t {
  NVGP_IMAGE_DRAW_DEFAULT = 0,
  NVGP_IMAGE_DRAW_REPEAT,
} nvgp_draw_image_type_t;

typedef enum _nvgp_point_flags_t {
  NVGP_PT_CORNER = 0x01,
  NVGP_PT_LEFT = 0x02,
  NVGP_PT_BEVEL = 0x04,
  NVGP_PR_INNERBEVEL = 0x08,
} nvgp_point_flags_t;

typedef enum _nvgp_align_t {
  // Horizontal align
  NVGP_ALIGN_LEFT     = 1<<0, // Default, align text horizontally to left.
  NVGP_ALIGN_CENTER   = 1<<1, // Align text horizontally to center.
  NVGP_ALIGN_RIGHT    = 1<<2, // Align text horizontally to right.
  // Vertical align
  NVGP_ALIGN_TOP      = 1<<3, // Align text vertically to top.
  NVGP_ALIGN_MIDDLE   = 1<<4, // Align text vertically to middle.
  NVGP_ALIGN_BOTTOM   = 1<<5, // Align text vertically to bottom.
  NVGP_ALIGN_BASELINE = 1<<6, // Default, align text vertically to baseline.
} nvgp_align_t;

typedef enum _nvgp_texture_format_t {
  NVGP_TEXTURE_FMT_ALPHA = 1,
  NVGP_TEXTURE_FMT_RGBA = 2,
  NVGP_TEXTURE_FMT_BGRA = 4,
  NVGP_TEXTURE_FMT_RGB  = 8,
  NVGP_TEXTURE_FMT_BGR  = 16,
  NVGP_TEXTURE_FMT_RGB565 = 32,
  NVGP_TEXTURE_FMT_BGR565 = 64
}nvgp_texture_format_t;

typedef struct _nvgp_vertex_t {
  float x,y,u,v;
} nvgp_vertex_t;

/*
* | scale_x  skew_x   trans_x |
* | skew_y   scale_y  trans_y |
* | pers0    pers1    pers2   |
*  
*  xform 为 nanovg 的矩阵
*  scale_x = xform[0]
*  scale_y = xform[3] 
*  skew_x = xform[2] 
*  skew_y = xform[1] 
*  trans_x = xform[4]   
*  trans_y = xform[5]  
*/
typedef union _nvgp_matrix_t {
  float m[3][3]; 
  struct {
    float scale_x, skew_x, trans_x;
    float skew_y, scale_y, trans_y;
    float pers0, pers1, pers2;
  } mat;
} nvgp_matrix_t;

typedef struct _nvgp_scissor_t {
  float extent[2];
  nvgp_matrix_t matrix;
} nvgp_scissor_t;

typedef struct _nvgp_paint_t {
  int image;
  float radius;
  float feather;
  float extent[2];
  float draw_info[2];
  float draw_image_rect[4];
  nvgp_matrix_t mat;
  nvgp_color_t inner_color;
  nvgp_color_t outer_color;
  nvgp_draw_image_type_t draw_type;
} nvgp_paint_t;

typedef struct _nvgp_path_t {
  uint32_t first;
  uint32_t count;
  uint32_t closed;
  uint32_t nbevel;
  uint32_t nfill;
  nvgp_vertex_t* fill;
  uint32_t nstroke;
  nvgp_vertex_t* stroke;
  uint32_t winding;
  uint32_t convex;
} nvgp_path_t;

typedef struct _nvgp_state_t nvgp_state_t;
typedef struct _nvgp_context_t nvgp_context_t;
 
typedef struct _nvgp_vtable_t {
  nvgp_bool_t (*clear_cache)(void* uptr);
  int (*find_texture)(void* uptr, const void* data);
  int (*create_texture)(void* uptr, int type, int w, int h, int stride, int image_flags, const unsigned char* data);
  nvgp_bool_t (*delete_texture)(void* uptr, int image);
  nvgp_bool_t (*update_texture)(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data);
  nvgp_bool_t (*get_texture_size)(void* uptr, int image, int* w, int* h);
  int (*get_edge_anti_alias)(void* uptr);

  void (*end_frame)(void* uptr);
  void (*begin_frame)(void* uptr, float width, float height, float pixel_ratio);
  void (*set_line_cap)(void* uptr, int line_cap);
  void (*set_line_join)(void* uptr, int line_join);
  void (*render_cancel)(void* uptr);
  void (*render_fill)(void* uptr, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, const float* bounds, const nvgp_darray_t* paths);
  void (*render_stroke)(void* uptr, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, float stroke_width, const nvgp_darray_t* paths);
  void (*render_draw_text)(void* uptr, nvgp_paint_t* paint, nvgp_scissor_t* scissor, nvgp_vertex_t* verts, uint32_t nverts);
  void (*destroy)(void* uptr);
} nvgp_vtable_t;

/**
 * @method nvgp_create_by_vt
 * 创建矢量画布对象
 * @annotation ["constructor", "scriptable"]
 * @param {uint32_t} w 矢量画布宽
 * @param {uint32_t} h 矢量画布高
 * @param {const nvgp_vtable_t* } vt 矢量画布函数虚构表
 * @param {void* } ctx 矢量画布上下文
 * 
 * @return {nvgp_context_t*} 成功返回矢量画布对象。
 */
nvgp_context_t* nvgp_create_by_vt(uint32_t w, uint32_t h, const nvgp_vtable_t* vt, void* ctx);

/**
 * @method nvgp_create
 * 创建矢量画布对象
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_mode_t} nvgp_mode 矢量画布类型
 * @param {uint32_t} w 矢量画布宽
 * @param {uint32_t} h 矢量画布高
 * 
 * @return {nvgp_context_t*} 成功返回矢量画布对象。
 */
nvgp_context_t* nvgp_create(nvgp_mode_t nvgp_mode, uint32_t w, uint32_t h);

/**
 * @method nvgp_destroy
 * 释放矢量画布对象
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 *
 */
void nvgp_destroy(nvgp_context_t* ctx);




/**
 * @method nvgp_reset_curr_state
 * 刷新当前状态
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 *
 */
void nvgp_reset_curr_state(nvgp_context_t* ctx);

/**
 * @method nvgp_begin_frame_ex
 * 开始当前帧
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} width 矢量画布宽
 * @param {float} height 矢量画布高
 * @param {float} pixel_ratio 矢量画布屏幕比例
 * @param {nvgp_bool_t} reset 是否重新刷新当前状态
 *
 */
void nvgp_begin_frame_ex(nvgp_context_t *ctx, float width, float height, float pixel_ratio, nvgp_bool_t reset);

/**
 * @method nvgp_begin_frame
 * 开始当前帧
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} width 矢量画布宽
 * @param {float} height 矢量画布高
 * @param {float} pixel_ratio 矢量画布屏幕比例
 *
 */
void nvgp_begin_frame(nvgp_context_t *ctx, float width, float height, float pixel_ratio);

/**
 * @method nvgp_end_frame
 * 结束当前帧
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 *
 */
void nvgp_end_frame(nvgp_context_t *ctx);

/**
 * @method nvgp_restore
 * 保存当前状态
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 *
 */
void nvgp_save(nvgp_context_t *ctx);

/**
 * @method nvgp_restore
 * 回退上一级状态
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 *
 */
void nvgp_restore(nvgp_context_t *ctx);




/**
 * @method nvgp_get_image_size
 * 获取贴图宽高
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {int} image 贴图的数据长度
 * @param {int*} w 返回贴图的宽度
 * @param {int*} h 返回贴图的高度
 *
 */
void nvgp_get_image_size(nvgp_context_t* ctx, int image, int* w, int* h);

/**
 * @method nvgp_create_image_rgba
 * 创建贴图（rgba格式）
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {int} w 贴图的宽度
 * @param {int} h 贴图的高度
 * @param {int} imageFlags 贴图的属性
 * @param {const unsigned char*} rgba_data 贴图的数据
 *
 * @return {int32_t} 成功返回贴图 id。
 */
int32_t nvgp_create_image_rgba(nvgp_context_t* ctx, int w, int h, int imageFlags, const unsigned char* rgba_data);

/**
 * @method nvgp_update_image_rgba
 * 更新贴图数据（rgba格式）
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {int} image 贴图的数据长度
 * @param {const unsigned char*} rgba_data 贴图的数据
 *
 */
void nvgp_update_image_rgba(nvgp_context_t* ctx, int image, const unsigned char* rgba_data);




/**
 * @method nvgp_create_font_mem
 * 创建新的字库
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {const char*} name 字库的名字
 * @param {unsigned char*} data 字库的数据
 * @param {int} ndata 字库的数据长度
 * @param {nvgp_bool_t} freeData 是否需要自动释放
 *
 * @return {int} 成功返回 字库 id。
 */
int nvgp_create_font_mem(nvgp_context_t* ctx, const char* name, unsigned char* data, int ndata, nvgp_bool_t freeData);

/**
 * @method nvgp_find_font
 * 查找字库
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {const char*} name 查找对应名字的字库
 *
 * @return {int} 成功返回 字库 id。
 */
int nvgp_find_font(nvgp_context_t* ctx, const char* name);

/**
 * @method nvgp_delete_font_by_name
 * 删除字库
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {const char*} name 删除对应名字的字库，如果为 NULL 则删除全部字库。
 *
 */
void nvgp_delete_font_by_name(nvgp_context_t* ctx, const char* name);

/**
 * @method nvgp_delete_image
 * 删除贴图
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {int} image 贴图 id
 *
 * @return {nvgp_bool_t} 成功返回 TRUE。
 */
void nvgp_delete_image(nvgp_context_t* ctx, int image);

/**
 * @method nvgp_begin_path
 * 清除矢量画布临时缓存
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 *
 * @return {nvgp_bool_t} 成功返回 TRUE。
 */
nvgp_bool_t nvgp_clear_cache(nvgp_context_t* ctx);



/**
 * @method nvgp_begin_path
 * 开始新一条线段
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_begin_path(nvgp_context_t *ctx);

/**
 * @method nvgp_close_path
 * 闭合线段
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_close_path(nvgp_context_t *ctx);

/**
 * @method nvgp_move_to
 * 设置线段起始点
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} x
 * @param {float} y 
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_move_to(nvgp_context_t *ctx, float x, float y);

/**
 * @method nvgp_line_to
 * 设置连接点
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} x
 * @param {float} y 
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_line_to(nvgp_context_t *ctx, float x, float y);

/**
 * @method nvgp_bezier_to
 * 设置三阶贝塞尔曲线路径
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} c1x
 * @param {float} c1y
 * @param {float} c2x
 * @param {float} c2y
 * @param {float} x
 * @param {float} y 
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_bezier_to(nvgp_context_t *ctx, float c1x, float c1y, float c2x, float c2y, float x, float y);

/**
 * @method nvgp_quad_to
 * 设置二阶贝塞尔曲线路径
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} cx
 * @param {float} cy
 * @param {float} x
 * @param {float} y
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_quad_to(nvgp_context_t *ctx, float cx, float cy, float x, float y);

/**
 * @method nvgp_arc
 * 设置圆弧路径
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} x 圆心 x 坐标
 * @param {float} y 圆心 y 坐标
 * @param {float} r 半径
 * @param {float} a0 始点角度（单位弧度）
 * @param {float} a1 终点角度（单位弧度）
 * @param {nvgp_bool_t} ccw 是否为顺序针
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_arc(nvgp_context_t *ctx, float cx, float cy, float r, float a0, float a1, nvgp_bool_t ccw);

/**
 * @method nvgp_arc_to
 * 设置圆弧路径
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} x 始点 x 坐标
 * @param {float} y 始点 y 坐标
 * @param {float} x2 终点 x 坐标
 * @param {float} y2 终点 y 坐标
 * @param {float} radius 半径
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_arc_to(nvgp_context_t *ctx, float x1, float y1, float x2, float y2, float radius);

/**
 * @method nvgp_rect
 * 设置矩形路径
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} x x 坐标
 * @param {float} y y 坐标
 * @param {float} w 宽度
 * @param {float} h 高度
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_rect(nvgp_context_t *ctx, float x, float y, float w, float h);

/**
 * @method nvgp_rounded_rect_varying
 * 设置圆角矩形路径
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} x x 坐标
 * @param {float} y y 坐标
 * @param {float} w 宽度
 * @param {float} h 高度
 * @param {float} rad_top_left 左上角圆角半径
 * @param {float} rad_top_right 右上角圆角半径
 * @param {float} rad_bottom_right 右下角圆角半径
 * @param {float} rad_bottom_left 左下角圆角半径
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_rounded_rect_varying(nvgp_context_t *ctx, float x, float y, float w, float h, float rad_top_left, float rad_top_right, float rad_bottom_right, float rad_bottom_left);

/**
 * @method nvgp_rounded_rect
 * 设置圆角矩形路径
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} x x 坐标
 * @param {float} y y 坐标
 * @param {float} w 宽度
 * @param {float} h 高度
 * @param {float} r 圆角半径
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_rounded_rect(nvgp_context_t *ctx, float x, float y, float w, float h, float r);

/**
 * @method nvgp_ellipse
 * 设置椭圆路径
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} cx 圆心 x
 * @param {float} cy 圆心 y
 * @param {float} rx x 轴半径
 * @param {float} ry y 轴半径
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_ellipse(nvgp_context_t *ctx, float cx, float cy, float rx, float ry);

/**
 * @method nvgp_circle
 * 设置圆路径
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} cx 圆心 x
 * @param {float} cy 圆心 y
 * @param {float} r 半径
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_circle(nvgp_context_t *ctx, float cx, float cy, float r);





/**
 * @method nvgp_translate
 * 偏移
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} x 偏移 x
 * @param {float} y 偏移 y
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_translate(nvgp_context_t *ctx, float x, float y);

/**
 * @method nvgp_rotate
 * 旋转（单位为弧度）
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} angle 旋转弧度
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_rotate(nvgp_context_t *ctx, float angle);

/**
 * @method nvgp_scale
 * 缩放
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} x 缩放 x
 * @param {float} y 缩放 y
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_scale(nvgp_context_t *ctx, float x, float y);

/**
 * @method nvgp_reset_transform
 * 重置当前旋转矩阵
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_reset_transform(nvgp_context_t *ctx);

/**
 * @method nvgp_transform
 * 叠加旋转矩阵
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} a 缩放 x
 * @param {float} b 拉伸 y
 * @param {float} c 拉伸 x
 * @param {float} d 缩放 y
 * @param {float} e 偏移 x
 * @param {float} f 偏移 y
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_transform(nvgp_context_t *ctx, float a, float b, float c, float d, float e, float f);



/**
 * @method nvgp_scissor
 * 设置当前裁剪区
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} x 裁剪区 x
 * @param {float} y 裁剪区 y
 * @param {float} w 裁剪区 w
 * @param {float} h 裁剪区 h
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_scissor(nvgp_context_t* ctx, float x, float y, float w, float h);

/**
 * @method nvgp_intersect_scissor
 * 叠加裁剪区，同时返回叠加后的裁剪区
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float*} x 设置裁剪区 x，同时返回新的裁剪区 x
 * @param {float*} y 设置裁剪区 y, 同时返回裁剪区 y
 * @param {float*} w 设置裁剪区 w，同时返回裁剪区 w
 * @param {float*} h 设置裁剪区 h，同时返回裁剪区 h
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_intersect_scissor(nvgp_context_t* ctx, float* x, float* y, float* w, float* h);

/**
 * @method nvgp_get_curr_clip_rect
 * 获取当前裁剪区
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float*} x 返回裁剪区 x
 * @param {float*} y 返回裁剪区 y
 * @param {float*} w 返回裁剪区 w
 * @param {float*} h 返回裁剪区 h
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_get_curr_clip_rect(nvgp_context_t* ctx, float* x, float* y, float* w, float* h);




/**
 * @method nvgp_text_metrics
 * 设置字符串基线等信息
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float*} ascender 
 * @param {float*} descender 
 * @param {float*} lineh 
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
void nvgp_text_metrics(nvgp_context_t* ctx, float* ascender, float* descender, float* lineh);

/**
 * @method nvgp_text_bounds
 * 获取字符串宽度
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} x 坐标 x
 * @param {float} y 坐标 y
 * @param {const char*} string 字符串开头 
 * @param {const char*} end 字符串结尾
 * @param {float*} bounds 返回一个字符串矩形区域（是一个长度为四的数组）
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
float nvgp_text_bounds(nvgp_context_t* ctx, float x, float y, const char* string, const char* end, float* bounds);

/**
 * @method nvgp_text
 * 渲染字体
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} x 坐标 x
 * @param {float} y 坐标 y
 * @param {const char*} string 字符串开头 
 * @param {const char*} end 字符串结尾
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
float nvgp_text(nvgp_context_t* ctx, float x, float y, const char* string, const char* end);



/**
 * @method nvgp_set_shape_anti_alias
 * 设置线颜色
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {nvgp_color_t} color 线颜色
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_set_stroke_color(nvgp_context_t *ctx, nvgp_color_t color);
/**
 * @method nvgp_set_fill_color
 * 设置填充色
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {nvgp_color_t} color 填充色
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_set_fill_color(nvgp_context_t *ctx, nvgp_color_t color);

/**
 * @method nvgp_set_shape_anti_alias
 * 设置抗锯齿
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {nvgp_bool_t} enabled 是否启用抗锯齿
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_set_shape_anti_alias(nvgp_context_t* ctx, nvgp_bool_t enabled);

/**
 * @method nvgp_set_miter_limit
 * 设置线斜面
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} cap limit
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_set_miter_limit(nvgp_context_t *ctx, float limit);

/**
 * @method nvgp_set_line_cap
 * 设置线帽
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {nvgp_line_cap_t} cap 线帽
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_set_line_cap(nvgp_context_t* ctx, nvgp_line_cap_t cap);

/**
 * @method nvgp_set_line_join
 * 设置线连接处样式
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {nvgp_line_join_t} join 线连接处样式
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_set_line_join(nvgp_context_t* ctx, nvgp_line_join_t join);

/**
 * @method nvgp_set_stroke_width
 * 设置线宽
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} stroke_width 线宽
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_set_stroke_width(nvgp_context_t *ctx, float stroke_width);

/**
 * @method nvgp_set_global_alpha
 * 设置全局透明度
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {uint8_t} alpha 全局透明度
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_set_global_alpha(nvgp_context_t *ctx, uint8_t alpha);

/**
 * @method nvgp_text_align
 * 设置字对齐
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {int32_t} align 对齐枚举
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_text_align(nvgp_context_t* ctx, int32_t align);

/**
 * @method nvgp_font_face_id
 * 设置字库id
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {int32_t} font_id 字库id
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_font_face_id(nvgp_context_t* ctx, int32_t font_id);

/**
 * @method nvgp_set_font_size
 * 设置字号（单位px）
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} font_size 字号（单位px）
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_set_font_size(nvgp_context_t *ctx, float font_size);

/**
 * @method nvgp_set_fill_paint
 * 设置填充画笔
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {nvgp_paint_t} paint 画笔
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_set_fill_paint(nvgp_context_t* ctx, nvgp_paint_t paint);

/**
 * @method nvgp_set_stroke_paint
 * 设置画线画笔
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {nvgp_paint_t} paint 画笔
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_set_stroke_paint(nvgp_context_t* ctx, nvgp_paint_t paint);





/**
 * @method nvgp_linear_gradient
 * 创建直线渐变画笔
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} sx 起始坐标x
 * @param {float} sy 起始坐标y
 * @param {float} ex 结束坐标x
 * @param {float} ey 结束坐标y
 * @param {nvgp_color_t} icol 起始颜色
 * @param {nvgp_color_t} ocol 结束颜色
 *
 * @return {nvgp_paint_t} 成功返回画笔。
 */
nvgp_paint_t nvgp_linear_gradient(nvgp_context_t* ctx, float sx, float sy, float ex, float ey, nvgp_color_t icol, nvgp_color_t ocol);

/**
 * @method nvgp_radial_gradient
 * 创建圆形渐变画笔
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} cx 渐变中心坐标
 * @param {float} cy 渐变中心坐标
 * @param {float} inr 内半径
 * @param {float} outr 外半径
 * @param {nvgp_color_t} icol 内半径颜色
 * @param {nvgp_color_t} ocol 外半径颜色
 *
 * @return {nvgp_paint_t} 成功返回画笔。
 */
nvgp_paint_t nvgp_radial_gradient(nvgp_context_t* ctx, float cx, float cy, float inr, float outr, nvgp_color_t icol, nvgp_color_t ocol);

/**
 * @method nvgp_image_pattern
 * 创建贴图画笔
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} cx 截取原图中心坐标
 * @param {float} cy 截取原图中心坐标
 * @param {float} w 截取原图宽度
 * @param {float} h 截取原图高度
 * @param {float} angle 旋转角度
 * @param {int} image 贴图 id
 * @param {uint8_t} alpha 透明度
 *
 * @return {nvgp_paint_t} 成功返回画笔。
 */
nvgp_paint_t nvgp_image_pattern(nvgp_context_t* ctx, float cx, float cy, float w, float h, float angle, int image, uint8_t alpha);

/**
 * @method nvgp_image_pattern_repeat
 * 创建重复贴图画笔
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 * @param {float} sx 截取原图 x 坐标
 * @param {float} sy 截取原图 y 坐标
 * @param {float} sw 截取原图宽度
 * @param {float} sh 截取原图高度
 * @param {float} dw 目标宽度
 * @param {float} dh 目标高度
 * @param {float} image_w 原图宽度
 * @param {float} image_h 原图高度
 * @param {float} angle 旋转角度
 * @param {int} image 贴图 id
 * @param {uint8_t} alpha 透明度
 *
 * @return {nvgp_paint_t} 成功返回画笔。
 */
nvgp_paint_t nvgp_image_pattern_repeat(nvgp_context_t* ctx, float sx, float sy, float sw, float sh, float dw, float dh, float image_w, float image_h, float angle, int image, uint8_t alpha);





/**
 * @method nvgp_fill
 * 填充颜色
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_fill(nvgp_context_t* ctx);

/**
 * @method nvgp_stroke
 * 画线
 * @annotation ["constructor", "scriptable"]
 * @param {nvgp_context_t*} ctx 矢量画布上下文
 *
 * @return {nvgp_error_t} 成功返回 NVGP_OK。
 */
nvgp_error_t nvgp_stroke(nvgp_context_t* ctx);

/* private */
void* nvgp_get_vt_ctx(nvgp_context_t* ctx);
void nvgp_transform_scale(nvgp_matrix_t* mat, float sx, float sy);
void nvgp_transform_translate(nvgp_matrix_t* mat, float tx, float ty);
nvgp_matrix_t* nvgp_transform_multiply_to_t(nvgp_matrix_t* t, nvgp_matrix_t* s);
nvgp_bool_t nvgp_transform_inverse(nvgp_matrix_t* inv, const nvgp_matrix_t* t);

#ifdef __cplusplus
}
#endif
#endif
