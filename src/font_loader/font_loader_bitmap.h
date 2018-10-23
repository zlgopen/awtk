/**
 * File:   font_bitmap.h
 * Author: AWTK Develop Team
 * Brief:  font bitmap font loader
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

#ifndef TK_FONT_LOADER_BITMAP_H
#define TK_FONT_LOADER_BITMAP_H

#include "base/font_loader.h"

BEGIN_C_DECLS

typedef struct _font_bitmap_index_t {
  uint16_t c;
  uint16_t offset;
} font_bitmap_index_t;

typedef struct _font_bitmap_header_t {
  uint16_t char_nr;
  uint8_t font_size;
  uint8_t baseline;
  font_bitmap_index_t index[1];
} font_bitmap_header_t;

font_t* font_bitmap_create(const char* name, const uint8_t* buff, uint32_t buff_size);

font_loader_t* font_loader_bitmap(void);

END_C_DECLS

#endif /*TK_FONT_LOADER_BITMAP_H*/
