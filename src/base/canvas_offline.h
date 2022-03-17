/**
 * File:   canvas_offline.h
 * Author: AWTK Develop Team
 * Brief:  offline canvas.
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-07-10 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#ifndef TK_CANVAS_OFFLINE_H
#define TK_CANVAS_OFFLINE_H

#include "base/system_info.h"
#include "base/font_manager.h"
#include "base/lcd.h"

BEGIN_C_DECLS
#include "base/canvas.h"

/**
 * @class canvas_offline_t
 * @annotation ["scriptable"]
 * 离线画布 canvas。
 *
 */
typedef struct _canvas_offline_t {
  canvas_t base;
  /**
   * @property {bitmap_t*} bitmap
   * @annotation ["readable"]
   * 绑定的离线 bitmap
   */
  bitmap_t* bitmap;

  /* private */
  /* 保存在线的 canvas 的裁减区 */
  rect_t canvas_clip_rect;
  /*确保 begin_draw / end_draw 配对使用*/
  int32_t begin_draw;
  uint32_t lcd_w;
  uint32_t lcd_h;
} canvas_offline_t;

/**
 * @method canvas_offline_create
 * 创建一个离线的 canvas
 * 在 opengl 模式下 format 参数只能为 BITMAP_FMT_RGBA8888
 * 在其他模式下，离线 canvas 格式可以为 rgba，bgar，bgr565和rgb565
 * 旋转方向和 lcd 旋转方向保存一致，旋转方向不同可能会导致 bitmap 的逻辑宽高不同。
 * 
 * @param {uint32_t} w 离线 canvas 的物理宽。
 * @param {uint32_t} h 离线 canvas 的物理高。
 * @param {bitmap_format_t} format 离线 canvas 的格式。
 *
 * @return {canvas_t*} 成功返回 canvas ，失败返回 NULL。
 */
canvas_t* canvas_offline_create(uint32_t w, uint32_t h, bitmap_format_t format);

/**
 * @method canvas_offline_clear_canvas
 * 清除离线 canvas 所有数据，并把背景设置为全透明（注意：该离线 canvas 需要有透明通道）
 * 该函数调用前必须要先 canvas_offline_begin_draw 函数。
 * 该函数用来解决离线 canvas 多次绘图半透效果后导致半透效果无效的问题。
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_clear_canvas(canvas_t* canvas);

/**
 * @method canvas_offline_begin_draw
 * 设置离线 canvas 开始绘图
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_begin_draw(canvas_t* canvas);

/**
 * @method canvas_offline_end_draw
 * 设置离线 canvas 结束绘图
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_end_draw(canvas_t* canvas);

/**
 * @method canvas_offline_get_bitmap
 * 获取离线 canvas 的离线 bitmap。
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {bitmap_t*} 返回 bitmap_t 对象表示成功，返回 NULL 表示失败。
 */
bitmap_t* canvas_offline_get_bitmap(canvas_t* canvas);

/**
 * @method canvas_offline_bitmap_move_to_new_bitmap
 * 把离线 canvas 的离线 bitmap 移动赋值给新的 bitmap。
 * 移动赋值后原来的离线 canvas 的离线 bitmap 就会被置空。
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 * @param {bitmap_t*} bitmap 新的 bitmap 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_bitmap_move_to_new_bitmap(canvas_t* canvas, bitmap_t* bitmap);

/**
 * @method canvas_offline_flush_bitmap
 * 把离线 canvas 的数据放到绑定的 bitmap 中
 * 该函数只有在 opengl 模式才需要调用，是否把显存中的数据回传到内存中。
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_flush_bitmap(canvas_t* canvas);

/**
 * @method canvas_offline_destroy
 * 释放离线 canvas 和离线 bitmap
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_destroy(canvas_t* canvas);

/*
 * WITH_CANVAS_OFFLINE_CUSTION 宏提供给用户在外部自定义离线 canvas 的机会，
 * 主要是用于给用户在外部定义一些特殊的离线 canvas 使用的，而这些特殊的离线 canvas 很大概率和平台相关的，
 * 所以可以通过 WITH_CANVAS_OFFLINE_CUSTION 宏来外部实现自定义离线 canvas 的效果。
 */
#ifdef WITH_CANVAS_OFFLINE_CUSTION
/**
 * @method canvas_offline_custom_create
 * @export none
 * 用户自定义 canvas_offline_create
 * 
 * @param {uint32_t} w 离线 canvas 的宽。
 * @param {uint32_t} h 离线 canvas 的高。
 * @param {bitmap_format_t} format 离线 canvas 的格式。
 *
 * @return {canvas_t*} 成功返回 canvas ，失败返回 NULL。
 */
canvas_t* canvas_offline_custom_create(uint32_t w, uint32_t h, bitmap_format_t format);

/**
 * @method canvas_offline_custom_clear_canvas
 * @export none
 * 用户自定义 canvas_offline_custom_clear_canvas
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_custom_clear_canvas(canvas_t* canvas);

/**
 * @method canvas_offline_custom_begin_draw
 * @export none
 * 用户自定义 canvas_offline_custom_begin_draw
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_custom_begin_draw(canvas_t* canvas);

/**
 * @method canvas_offline_custom_end_draw
 * @export none
 * 用户自定义 canvas_offline_custom_end_draw
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_custom_end_draw(canvas_t* canvas);

/**
 * @method canvas_offline_custom_get_bitmap
 * @export none
 * 用户自定义 canvas_offline_custom_get_bitmap
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {bitmap_t*} 返回 bitmap_t 对象表示成功，返回 NULL 表示失败。
 */
bitmap_t* canvas_offline_custom_get_bitmap(canvas_t* canvas);

/**
 * @method canvas_offline_custom_bitmap_move_to_new_bitmap
 * @export none
 * 用户自定义 canvas_offline_custom_bitmap_move_to_new_bitmap
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {bitmap_t*} 返回 bitmap_t 对象表示成功，返回 NULL 表示失败。
 */
ret_t canvas_offline_custom_bitmap_move_to_new_bitmap(canvas_t* canvas, bitmap_t* bitmap);

/**
 * @method canvas_offline_custom_flush_bitmap
 * @export none
 * 用户自定义 canvas_offline_custom_flush_bitmap
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_custom_flush_bitmap(canvas_t* canvas);

/**
 * @method canvas_offline_custom_destroy
 * @export none
 * 用户自定义 canvas_offline_custom_destroy
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_custom_destroy(canvas_t* canvas);

#endif

END_C_DECLS

#endif /*TK_CANVAS_OFFLINE_H*/
