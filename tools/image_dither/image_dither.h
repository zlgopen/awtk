/**
 * File:   image_dither.h
 * Author: AWTK Develop Team
 * Brief:  bitmap image generator
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
 * 2020-05-26 Luo ZhiMing <luozhiming@zlg.cn> created
 *
 */

#ifndef IMAGE_GEN_H
#define IMAGE_GEN_H

#include "widgets/image.h"

BEGIN_C_DECLS

ret_t image_dither_load_image(const uint8_t* buff, uint32_t buff_size, bitmap_t* image,
                              bitmap_format_t bitmap_format);

ret_t image_dither_image_wirte_png_file(const char* output_file, bitmap_t* image);

END_C_DECLS

#endif /*IMAGE_GEN_H*/
