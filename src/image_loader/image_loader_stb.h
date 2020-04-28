/**
 * File:   image_loader_stb.h
 * Author: AWTK Develop Team
 * Brief:  stb image loader
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
 * @param {bool_t} require_bgra require_bgra
 * @param {bool_t} enable_bgr565 enable_bgr565
 * @param {bool_t} enable_rgb565 enable_rgb565
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t stb_load_image(int32_t subtype, const uint8_t* buff, uint32_t buff_size, bitmap_t* image,
                     bool_t require_bgra, bool_t enable_bgr565, bool_t enable_rgb565);

END_C_DECLS

#endif /*TK_IMAGE_LOADER_STB_H*/
