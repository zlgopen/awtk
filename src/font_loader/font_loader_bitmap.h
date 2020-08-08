/**
 * File:   font_bitmap.h
 * Author: AWTK Develop Team
 * Brief:  font bitmap font loader
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

#ifndef TK_FONT_LOADER_BITMAP_H
#define TK_FONT_LOADER_BITMAP_H

#include "base/font_loader.h"

BEGIN_C_DECLS

typedef struct _font_bitmap_index_t {
  uint16_t c;
  uint16_t size;
  uint32_t offset;
} font_bitmap_index_t;

typedef struct _font_bitmap_header_t {
  uint16_t version;
  uint16_t format;
  uint16_t char_nr;
  int16_t font_size;
  int16_t ascent;
  int16_t descent;
  int16_t line_gap;
  int16_t reserved;
  font_bitmap_index_t index[1];
} font_bitmap_header_t;

font_t* font_bitmap_create(const char* name, const uint8_t* buff, uint32_t buff_size);

/**
 * @class font_loader_bitmap_t
 * @parent font_loader_t
 * bitmap字体加载器。
 *
 * bitmap字体是用工具预先生成位图字体数据，优势是速度快，不占用内存，缺点是字体大小固定。
 * 适合低端嵌入式平台。
 *
 * tools/font_gen用于把矢量字体(如truetype)转换成位图字体。
 *
 * @annotation["fake"]
 *
 */

/**
 * @method font_loader_bitmap
 * @annotation ["constructor"]
 *
 * 获取bitmap字体加载器对象。
 *
 * @return {font_loader_t*} 返回字体加载器对象。
 */
font_loader_t* font_loader_bitmap(void);

END_C_DECLS

#endif /*TK_FONT_LOADER_BITMAP_H*/
