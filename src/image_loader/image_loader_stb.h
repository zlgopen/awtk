/**
 * File:   stb.h
 * Author: AWTK Develop Team
 * Brief:  stb image loader
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IMAGE_LOADER_STB_H
#define TK_IMAGE_LOADER_STB_H

#include "base/image_loader.h"

BEGIN_C_DECLS

image_loader_t* image_loader_stb(void);

/*for tool image_gen only*/
ret_t stb_load_image(const uint8_t* buff, uint32_t buff_size, bitmap_t* image, bool_t require_bgra,
                     bool_t enable_bgr565);

END_C_DECLS

#endif /*TK_IMAGE_LOADER_STB_H*/
