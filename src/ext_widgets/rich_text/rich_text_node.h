/**
 * File:   rich_text_node.h
 * Author: AWTK Develop Team
 * Brief:  rich_text_node
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is dirich_text_nodeibuted in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-07-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_RICH_TEXT_NODE_H
#define TK_RICH_TEXT_NODE_H

#include "tkc/color.h"
#include "base/bitmap.h"

BEGIN_C_DECLS

typedef struct _rich_text_font_t {
  char* name;
  color_t color;
  align_v_t align_v;
  uint16_t size;
  uint16_t bold : 1;
  uint16_t italic : 1;
  uint16_t underline : 1;
} rich_text_font_t;

typedef struct _rich_text_image_t {
  char* name;
  uint32_t w;
  uint32_t h;
  image_draw_type_t draw_type;
} rich_text_image_t;

typedef struct _rich_text_text_t {
  wchar_t* text;
  rich_text_font_t font;
} rich_text_text_t;

typedef enum _rich_text_node_type_t {
  RICH_TEXT_TEXT = 0,
  RICH_TEXT_IMAGE = 1
} rich_text_node_type_t;

typedef struct _rich_text_node_t {
  rich_text_node_type_t type;
  union {
    rich_text_text_t text;
    rich_text_image_t image;
  } u;
  struct _rich_text_node_t* next;
} rich_text_node_t;

rich_text_node_t* rich_text_text_create(rich_text_font_t* font, const char* text);
rich_text_node_t* rich_text_text_create_with_len(rich_text_font_t* font, const char* text,
                                                 uint32_t len);
rich_text_node_t* rich_text_image_create(const char* name, uint32_t w, uint32_t h,
                                         uint32_t draw_type);
rich_text_node_t* rich_text_node_append(rich_text_node_t* node, rich_text_node_t* next);

int32_t rich_text_node_count(rich_text_node_t* node);

ret_t rich_text_text_destroy(rich_text_node_t* node);
ret_t rich_text_image_destroy(rich_text_node_t* node);
ret_t rich_text_node_destroy(rich_text_node_t* node);

END_C_DECLS

#endif /*TK_RICH_TEXT_NODE_H*/
