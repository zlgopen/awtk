/**
 * File:   font_gen.h
 * Author: AWTK Develop Team
 * Brief:  bitmap font generator
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

#ifndef FONT_GEN_H
#define FONT_GEN_H

#include "base/font.h"
#include "tkc/buffer.h"

BEGIN_C_DECLS

ret_t font_gen(font_t* font, uint16_t font_size, const char* str, const char* output_filename,
               const char* theme);
uint32_t font_gen_buff(font_t* font, uint16_t font_size, const char* str, wbuffer_t* wbuffer);

END_C_DECLS

#endif /*FONT_GEN_H*/
