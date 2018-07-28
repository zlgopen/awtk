/**
 * File:   rich_text_render_node.c
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
 * 2018-07-28 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "base/mem.h"
#include "base/image_manager.h"
#include "rich_text/rich_text_render_node.h"

#define ICON_SIZE 128

rich_text_render_node_t* rich_text_render_node_create(rich_text_node_t* node) {
  rich_text_render_node_t* render_node = NULL;
  return_value_if_fail(node != NULL, NULL);
  render_node = TKMEM_ZALLOC(rich_text_render_node_t);
  return_value_if_fail(render_node != NULL, NULL);

  render_node->node = node;

  return render_node;
}

rich_text_render_node_t* rich_text_render_node_layout(rich_text_node_t* node, canvas_t* c,
                                                      int32_t w, int32_t h, int32_t margin) {
  int32_t row_h = 0;
  int32_t x = margin;
  int32_t y = margin;
  int32_t right = w - margin;
  int32_t bottom = h - margin;
  int32_t client_w = w - 2 * margin;
  int32_t client_h = h - 2 * margin;

  rich_text_node_t* iter = node;
  rich_text_render_node_t* new_node = NULL;
  rich_text_render_node_t* render_node = NULL;
  return_value_if_fail(node != NULL && c != NULL && client_w > 0 && client_h > 0, NULL);

  while (iter != NULL) {
    switch (iter->type) {
      case RICH_TEXT_IMAGE: {
        bitmap_t bitmap;
        const char* name = iter->u.image.name;
        new_node = rich_text_render_node_create(iter);
        return_value_if_fail(new_node != NULL, render_node);

        if (image_manager_load(image_manager(), name, &bitmap) == RET_OK) {
          if (iter->u.image.w == 0) {
            iter->u.image.w = bitmap.w;
          }
          if (iter->u.image.h == 0) {
            iter->u.image.h = bitmap.h;
          }
        }

        if (iter->u.image.w > client_w) {
          iter->u.image.w = client_h;
        }

        if ((y + iter->u.image.h) > bottom) {
          return render_node;
        }

        if (iter->u.image.w > ICON_SIZE || (x + iter->u.image.w) > right) {
          x = margin;
          y += row_h;
        }

        if (row_h < iter->u.image.h) {
          row_h = iter->u.image.h;
        }

        new_node->rect.x = x;
        new_node->rect.y = y;
        new_node->rect.w = iter->u.image.w;
        new_node->rect.h = iter->u.image.h;

        render_node = rich_text_render_node_append(render_node, new_node);
        break;
      }
      case RICH_TEXT_TEXT: {
        float_t tw = 0;
        float_t cw = 0;
        int32_t i = 0;
        int32_t start = 0;
        wchar_t* str = iter->u.text.text;
        int32_t font_size = iter->u.text.font.size;

        if (row_h < font_size) {
          row_h = font_size;
        }

        canvas_set_font(c, iter->u.text.font.name, font_size);

        for (i = 0; str[i]; i++) {
          cw = canvas_measure_text(c, str + i, 1);
          if ((x + tw + cw) > right) {
            x = margin;
            y += row_h;

            new_node = rich_text_render_node_create(iter);
            return_value_if_fail(new_node != NULL, render_node);

            new_node->text = str + start;
            new_node->size = i - start + 1;
            new_node->rect = rect_init(x, y + row_h - font_size, tw, font_size);
            x += tw;

            render_node = rich_text_render_node_append(render_node, new_node);

            start = i + 1;
          } else {
            tw += cw;
          }
        }

        if (i > start) {
          new_node = rich_text_render_node_create(iter);
          return_value_if_fail(new_node != NULL, render_node);

          new_node->text = str + start;
          new_node->size = i - start;
          new_node->rect = rect_init(x, y + row_h - font_size, tw, font_size);
          x += tw;

          render_node = rich_text_render_node_append(render_node, new_node);
        }

        break;
      }
      default:
        break;
    }

    iter = iter->next;
  }

  return render_node;
}

rich_text_render_node_t* rich_text_render_node_append(rich_text_render_node_t* node,
                                                      rich_text_render_node_t* next) {
  rich_text_render_node_t* iter = node;
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

int32_t rich_text_render_node_count(rich_text_render_node_t* node) {
  int32_t nr = 0;
  rich_text_render_node_t* iter = node;
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    nr++;
    iter = iter->next;
  }

  return nr;
}

ret_t rich_text_render_node_destroy(rich_text_render_node_t* node) {
  rich_text_render_node_t* iter = node;
  rich_text_render_node_t* next = node;
  return_value_if_fail(node != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    next = iter->next;
    memset(iter, 0x00, sizeof(rich_text_render_node_t));
    TKMEM_FREE(iter);

    iter = next;
  }

  return RET_OK;
}
