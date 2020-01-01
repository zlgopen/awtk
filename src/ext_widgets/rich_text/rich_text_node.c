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

#include "tkc/str.h"
#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "rich_text/rich_text_node.h"

rich_text_node_t* rich_text_text_create_with_len(rich_text_font_t* font, const char* text,
                                                 uint32_t len) {
  rich_text_node_t* node = NULL;
  return_value_if_fail(font != NULL && text != NULL, NULL);

  node = TKMEM_ZALLOC(rich_text_node_t);
  return_value_if_fail(node != NULL, NULL);

  node->next = NULL;
  node->type = RICH_TEXT_TEXT;
  node->u.text.font = *font;

  if (font->name != NULL) {
    node->u.text.font.name = tk_strdup(font->name);
    goto_error_if_fail(node->u.text.font.name != NULL);
  }

  node->u.text.text = (wchar_t*)TKMEM_ALLOC(sizeof(wchar_t) * (len + 1));
  goto_error_if_fail(node->u.text.text != NULL);

  tk_utf8_to_utf16(text, node->u.text.text, len + 1);

  return node;
error:
  TKMEM_FREE(node->u.text.font.name);
  TKMEM_FREE(node);

  return NULL;
}

rich_text_node_t* rich_text_text_create(rich_text_font_t* font, const char* text) {
  return_value_if_fail(font != NULL && text != NULL, NULL);

  return rich_text_text_create_with_len(font, text, strlen(text));
}

rich_text_node_t* rich_text_image_create(const char* name, uint32_t w, uint32_t h,
                                         uint32_t draw_type) {
  rich_text_node_t* node = NULL;
  return_value_if_fail(name != NULL, NULL);

  node = TKMEM_ZALLOC(rich_text_node_t);
  return_value_if_fail(node != NULL, NULL);

  node->next = NULL;
  node->type = RICH_TEXT_IMAGE;
  node->u.image.w = w;
  node->u.image.h = h;
  node->u.image.draw_type = (image_draw_type_t)draw_type;
  node->u.image.name = tk_strdup(name);
  goto_error_if_fail(node->u.image.name != NULL);

  return node;
error:
  TKMEM_FREE(node);
  return NULL;
}

ret_t rich_text_text_destroy(rich_text_node_t* node) {
  return_value_if_fail(node != NULL && node->type == RICH_TEXT_TEXT, RET_BAD_PARAMS);

  TKMEM_FREE(node->u.text.text);
  TKMEM_FREE(node->u.text.font.name);
  memset(node, 0x00, sizeof(rich_text_node_t));
  TKMEM_FREE(node);

  return RET_OK;
}

ret_t rich_text_image_destroy(rich_text_node_t* node) {
  return_value_if_fail(node != NULL && node->type == RICH_TEXT_IMAGE, RET_BAD_PARAMS);

  TKMEM_FREE(node->u.image.name);
  memset(node, 0x00, sizeof(rich_text_node_t));
  TKMEM_FREE(node);

  return RET_OK;
}

rich_text_node_t* rich_text_node_append(rich_text_node_t* node, rich_text_node_t* next) {
  rich_text_node_t* iter = node;
  return_value_if_fail(next != NULL, node);

  if (node == NULL) {
    return next;
  }

  while (iter->next != NULL) {
    iter = iter->next;
  }

  iter->next = next;

  return node;
}

ret_t rich_text_node_destroy(rich_text_node_t* node) {
  rich_text_node_t* next = NULL;
  rich_text_node_t* iter = node;
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    next = iter->next;
    if (iter->type == RICH_TEXT_TEXT) {
      rich_text_text_destroy(iter);
    } else if (iter->type == RICH_TEXT_IMAGE) {
      rich_text_image_destroy(iter);
    }
    iter = next;
  }

  return RET_OK;
}

int32_t rich_text_node_count(rich_text_node_t* node) {
  int32_t nr = 0;
  rich_text_node_t* iter = node;
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    nr++;
    iter = iter->next;
  }

  return nr;
}
