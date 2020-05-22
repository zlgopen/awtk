/**
 * File:   image_g2d.h
 * Author: AWTK Develop Team
 * Brief:  image 2d operations.
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
 * 2018-06-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IMAGE_G2D_H
#define TK_IMAGE_G2D_H

#include "tkc/rect.h"
#include "base/bitmap.h"

BEGIN_C_DECLS

/**
 * @method image_fill
 * 用颜色绘制指定的区域。
 * @param {bitmap_t*} dst 目标图片对象。
 * @param {rect_t*} dst_r 要填充的目标区域。
 * @param {color_t} c 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败，返回失败则上层用软件实现。
 */
ret_t image_fill(bitmap_t* dst, rect_t* dst_r, color_t c);

/**
 * @method image_clear
 * 用颜色填充指定的区域。
 * @param {bitmap_t*} dst 目标图片对象。
 * @param {rect_t*} dst_r 要填充的目标区域。
 * @param {color_t} c 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败，返回失败则上层用软件实现。
 */
ret_t image_clear(bitmap_t* dst, rect_t* dst_r, color_t c);

/**
 * @method image_copy
 * 把图片指定的区域拷贝到framebuffer中。
 * @param {bitmap_t*} dst 目标图片对象。
 * @param {bitmap_t*} src 源图片对象。
 * @param {rect_t*} src_r 要拷贝的区域。
 * @param {xy_t} dx 目标位置的x坐标。
 * @param {xy_t} dy 目标位置的y坐标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败，返回失败则上层用软件实现。
 */
ret_t image_copy(bitmap_t* dst, bitmap_t* src, rect_t* src_r, xy_t dx, xy_t dy);

/**
 * @method image_rotate
 * 把图片指定的区域进行旋转并拷贝到framebuffer相应的区域，本函数主要用于辅助实现横屏和竖屏的切换，一般支持90度旋转即可。
 * @param {bitmap_t*} dst 目标图片对象。
 * @param {bitmap_t*} src 源图片对象。
 * @param {rect_t*} src_r 要旋转并拷贝的区域。
 * @param {lcd_orientation_t} o 旋转角度(一般支持90度即可)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败，返回失败则上层用软件实现。
 */
ret_t image_rotate(bitmap_t* dst, bitmap_t* src, rect_t* src_r, lcd_orientation_t o);

/**
 * @method image_blend
 * 把图片指定的区域渲染到framebuffer指定的区域，src的大小和dst的大小不一致则进行缩放。
 *
 * @param {bitmap_t*} dst 目标图片对象。
 * @param {bitmap_t*} src 源图片对象。
 * @param {rect_t*} dst_r 目的区域。
 * @param {rect_t*} src_r 源区域。
 * @param {uint8_t} global_alpha 全局alpha。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败，返回失败则上层用软件实现。
 */
ret_t image_blend(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t global_alpha);

END_C_DECLS

#endif /*TK_IMAGE_G2D_H*/
