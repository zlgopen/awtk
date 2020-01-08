/**
 * File:   rich_text_render_node.h
 * Author: AWTK Develop Team
 * Brief:  rich_text_render_node
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/widget.h"
#include "rich_text/rich_text_node.h"

BEGIN_C_DECLS

/*
 * 表示一个渲染节点。
 * 对于文本节点，当一行显示不下一个rich_text_node_t时，rich_text_node_t被拆分成多个rich_text_render_node_t。
 * 在排版进行自动换行时，右边可能会有多余的空间，为了让整行排满，我们需要把多余的空间分散开。具体做法是：
 *
 * 把多余的空间均匀的加到弹性宽度字符(flexible_w_char)的宽度中。
 *
 */
typedef struct _rich_text_render_node_t {
  rich_text_node_t* node;

  /*该节点显示的区域*/
  rect_t rect;
  /*文本节点的文本*/
  wchar_t* text;
  /*文本节点的文本的长度*/
  uint16_t size;
  /*该行剩余空间水平对齐使用*/
  int16_t align_h_w;

  /*该结点总共需要承担的多余空间*/
  uint16_t spacing;
  /*该结点中存在的弹性宽度字符个数*/
  uint8_t flexible_w_chars;
  /*每个弹性宽度字符应该承担的多余空间*/
  uint8_t flexible_w_char_delta_w;

  struct _rich_text_render_node_t* next;
} rich_text_render_node_t;

bool_t rich_text_is_flexable_w_char(wchar_t c);

rich_text_render_node_t* rich_text_render_node_layout(widget_t* widget, rich_text_node_t* node,
                                                      canvas_t* c, int32_t w, int32_t h,
                                                      int32_t margin, int32_t line_gap);
rich_text_render_node_t* rich_text_render_node_append(rich_text_render_node_t* node,
                                                      rich_text_render_node_t* next);

int32_t rich_text_render_node_count(rich_text_render_node_t* render_node);

ret_t rich_text_render_node_destroy(rich_text_render_node_t* render_node);

/*public for test*/
rich_text_render_node_t* rich_text_render_node_create(rich_text_node_t* node);

END_C_DECLS

#endif /*TK_RICH_TEXT_RENDER_NODE_H*/
