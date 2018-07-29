/**
 * File:   rich_text_render_node.h
 * Author: AWTK Develop Team
 * Brief:  rich_text_render_node
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is dirich_text_render_nodeibuted in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-07-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_RICH_TEXT_RENDER_NODE_H
#define TK_RICH_TEXT_RENDER_NODE_H

#include "base/canvas.h"
#include "rich_text/rich_text_node.h"

BEGIN_C_DECLS

typedef struct _rich_text_render_node_t {
  rich_text_node_t* node;

  rect_t rect;
  wchar_t* text;
  uint32_t size;

  struct _rich_text_render_node_t* next;
} rich_text_render_node_t;

rich_text_render_node_t* rich_text_render_node_create(rich_text_node_t* node);
rich_text_render_node_t* rich_text_render_node_layout(rich_text_node_t* node, canvas_t* c,
                                                      int32_t w, int32_t h, int32_t margin);
rich_text_render_node_t* rich_text_render_node_append(rich_text_render_node_t* node,
                                                      rich_text_render_node_t* next);

int32_t rich_text_render_node_count(rich_text_render_node_t* render_node);

ret_t rich_text_render_node_destroy(rich_text_render_node_t* render_node);

END_C_DECLS

#endif /*TK_RICH_TEXT_RENDER_NODE_H*/
