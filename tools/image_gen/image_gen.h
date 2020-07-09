/**
 * File:   image_gen.h
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
 * 2018-01-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef IMAGE_GEN_H
#define IMAGE_GEN_H

#include "widgets/image.h"
#include "tkc/buffer.h"

BEGIN_C_DECLS

ret_t image_gen(bitmap_t* image, const char* output_filename, const char* theme, bool_t mono);
uint32_t image_gen_buff(bitmap_t* image, wbuffer_t* wbuffer, bool_t mono);

END_C_DECLS

#endif /*IMAGE_GEN_H*/
