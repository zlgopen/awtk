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
 * 2018-07-28 Li XianJing <xianjimli@hotmail.com> created
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

#define MOVE_TO_NEXT_ROW()                                 \
  x = margin;                                              \
  y += row_h + line_gap;                                              \
  if (row_first_node != NULL) {                            \
    rich_text_render_node_tune_row(row_first_node, row_h); \
    row_first_node = NULL;                                 \
  } 

rich_text_render_node_t* rich_text_render_node_tune_row(rich_text_render_node_t* row_first_node,
                                                        int32_t row_h) {
  rich_text_render_node_t* iter = row_first_node;

  while (iter != NULL) {
    iter->rect.h = row_h;
    switch (iter->node->type) {
      case RICH_TEXT_TEXT: {
        break;
      }
      case RICH_TEXT_IMAGE: {
        break;
      }
      default:
        break;
    }

    iter = iter->next;
  }

  return NULL;
}

rich_text_render_node_t* rich_text_render_node_layout(rich_text_node_t* node, canvas_t* c,
                                                      int32_t w, int32_t h, int32_t margin, int32_t line_gap) {
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
  rich_text_render_node_t* row_first_node = NULL;
  return_value_if_fail(node != NULL && c != NULL && client_w > 0 && client_h > 0, NULL);

  while (iter != NULL) {
    switch (iter->type) {
      case RICH_TEXT_IMAGE: {
        bitmap_t bitmap;
        rich_text_image_t* image = &(iter->u.image);
        const char* name = image->name;
        new_node = rich_text_render_node_create(iter);
        return_value_if_fail(new_node != NULL, render_node);

        if (image_manager_load(image_manager(), name, &bitmap) == RET_OK) {
          if (image->w == 0) {
            image->w = bitmap.w;
          }
          if (image->h == 0) {
            image->h = bitmap.h;
          }
        }

        if (image->w > ICON_SIZE || (x + image->w) > right) {
          MOVE_TO_NEXT_ROW();
        }

        new_node->rect.x = x;
        new_node->rect.y = y;
        new_node->rect.w = image->w;
        new_node->rect.h = image->h;

        if (image->w > ICON_SIZE) {
          new_node->rect.w = client_w;
        }

        if (row_h < image->h) {
          row_h = image->h;
        }

        render_node = rich_text_render_node_append(render_node, new_node);
        if (row_first_node == NULL) {
          row_first_node = new_node;
        }
        x += new_node->rect.w + 1;
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
            i = i - 1;
            new_node = rich_text_render_node_create(iter);
            return_value_if_fail(new_node != NULL, render_node);

            new_node->text = str + start;
            new_node->size = i - start + 1;
            new_node->rect = rect_init(x, y, tw, font_size);

            render_node = rich_text_render_node_append(render_node, new_node);
            if (row_first_node == NULL) {
              row_first_node = new_node;
            }

            tw = 0;
            start = i + 1;
            MOVE_TO_NEXT_ROW();
            row_h = font_size;
          } else {
            tw += cw;
          }
        }

        if (i > start) {
          new_node = rich_text_render_node_create(iter);
          return_value_if_fail(new_node != NULL, render_node);

          new_node->text = str + start;
          new_node->size = i - start;
          new_node->rect = rect_init(x, y, tw, font_size);
          x += tw + 1;

          render_node = rich_text_render_node_append(render_node, new_node);
          if (row_first_node == NULL) {
            row_first_node = new_node;
          }
        }

        break;
      }
      default:
        break;
    }

    iter = iter->next;
  }
  MOVE_TO_NEXT_ROW();

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
