/**
 * File:   image_loader_stb.h
 * Author: AWTK Develop Team
 * Brief:  stb image loader
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IMAGE_LOADER_STB_H
#define TK_IMAGE_LOADER_STB_H

#include "base/image_loader.h"

BEGIN_C_DECLS

/**
 * @class image_loader_stb_t
 * @parent image_loader_t
 * stb图片加载器。
 *
 * stb主要用于加载jpg/png/gif等格式的图片，它功能强大，体积小巧。
 *
 * @annotation["fake"]
 *
 */

/**
 * @method image_loader_stb
 * @annotation ["constructor"]
 *
 * 获取stb图片加载器对象。
 *
 * @return {image_loader_t*} 返回图片加载器对象。
 */
image_loader_t* image_loader_stb(void);

/*for tool image_gen only*/

/**
 * @method stb_load_image
 * 加载图片。
 *
 * @annotation ["static"]
 * @param {int32_t} subtype 资源类型。
 * @param {const uint8_t*} buff 资源数据。
 * @param {uint32_t} buff_size 资源数据长度。
 * @param {bitmap_t*} image image 对象。
 * @param {bitmap_format_t} transparent_bitmap_format 带透明通道的位图格式（只能 BITMAP_FMT_RGBA8888 和 BITMAP_FMT_RGBA8888 二选一，其他类型默认都为 BITMAP_FMT_RGBA8888）
 * @param {bitmap_format_t} opaque_bitmap_format 不透明位图格式（暂时支持 BITMAP_FMT_RGBA8888，BITMAP_FMT_RGBA8888，16 位色和 24 位色以及 mono 格式）
 * @param {lcd_orientation_t} o 旋转方向
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t stb_load_image(int32_t subtype, const uint8_t* buff, uint32_t buff_size, bitmap_t* image,
                     bitmap_format_t transparent_bitmap_format,
                     bitmap_format_t opaque_bitmap_format, lcd_orientation_t o);

/*for gif image only*/

/**
 * @method stb_load_gif_next_frame
 * 按帧加载gif图片。
 *
 * @annotation ["static"]
 * @param {bitmap_t*} bitmap bitmap对象。
 * @param {void**} gif_context gif_context对象。
 * @param {void**} gif_msg gif_msg对象。
 * @param {void**} gif_cache gif_cache对象。
 * @param {int*} gif_delays gif_delays对象。
 * @param {const uint8_t*} buff 资源数据。
 * @param {uint32_t} buff_size 资源数据长度。
 * @param {int*} layers 图层数量。
 * @param {bool_t*} on_end 是否结束。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t stb_load_gif_next_frame(bitmap_t* bitmap, void** gif_context, void** gif_msg,
                              void** gif_cache, int* gif_delays, const uint8_t* buff,
                              uint32_t buff_size, int* layers, bool_t* on_end);

/**
 * @method stb_gif_frame_create
 * 创建gif图缓存。
 *
 * @annotation ["static"]
 * @param {void**} gif_context 对象。
 * @param {void**} gif_msg 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t stb_gif_frame_create(void** gif_context, void** gif_msg);

/**
 * @method stb_gif_frame_reset
 * 重置gif图缓存。
 *
 * @annotation ["static"]
 * @param {void**} gif_context 对象。
 * @param {void**} gif_msg 对象。
 * @param {void**} gif_cache 对象。
 * @param {bool_t} is_reset_gif_cache 是否重置gif_cache。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t stb_gif_frame_reset(void** gif_context, void** gif_msg, void** gif_cache,
                          bool_t is_reset_gif_cache);

/**
 * @method stb_gif_frame_free
 * 释放gif图缓存。
 *
 * @annotation ["static"]
 * @param {void**} gif_context 对象。
 * @param {void**} gif_msg 对象。
 * @param {void**} gif_cache 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t stb_gif_frame_free(void** gif_context, void** gif_msg, void** gif_cache);

END_C_DECLS

#endif /*TK_IMAGE_LOADER_STB_H*/
