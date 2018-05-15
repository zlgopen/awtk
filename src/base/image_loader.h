/**
 * File:   image_loader.h
 * Author: AWTK Develop Team
 * Brief:  image_loader interface
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
 * 2018-04-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IMAGE_LOADER_H
#define TK_IMAGE_LOADER_H

#include "base/bitmap.h"

BEGIN_C_DECLS

struct _image_loader_t;
typedef struct _image_loader_t image_loader_t;

typedef ret_t (*image_loader_load_t)(image_loader_t* loader, const uint8_t* buff, uint32_t size,
                                     bitmap_t* bitmap);

/**
 * @class image_loader_t
 * 图片加载接口。
 */
struct _image_loader_t {
  image_loader_load_t load;
};

/**
 * @method image_loader_load
 * 加载图片。
 * @param {image_loader_t*} loader loader对象。
 * @param {uint8_t*} buff 图片数据。
 * @param {uint32_t} size 图片数据长度。
 * @param {bitmap_t*} bitmap 用于返回位图对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_loader_load(image_loader_t* loader, const uint8_t* buff, uint32_t size,
                        bitmap_t* bitmap);

END_C_DECLS

#endif /*TK_IMAGE_LOADER_H*/
