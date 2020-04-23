/**
 * File:   rich_text_render_node.c
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

#include "tkc/mem.h"
#include "base/line_break.h"
#include "base/image_manager.h"
#include "rich_text/rich_text.h"
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

static const wchar_t* s_flexable_w_chars = L" ,.?!;:>}]　，。？！；：》｝】』";

bool_t rich_text_is_flexable_w_char(wchar_t c) {
  return wcschr(s_flexable_w_chars, c) != NULL;
}

ret_t rich_text_render_node_tune_row(rich_text_render_node_t* row_first_node, int32_t row_h,
                                     int32_t flexible_w, int32_t client_w) {
  int32_t dx = 0;
  int32_t flexible_w_chars = 0;
  int32_t flexible_w_chars_w = 0;
  int32_t align_h_w = flexible_w > 0 ? 0 : client_w;
  rich_text_render_node_t* iter = row_first_node;

  while (iter != NULL) {
    if (flexible_w > 0) {
      if (iter->node->type == RICH_TEXT_TEXT) {
        int32_t i = 0;
        for (i = 0; i < iter->size; i++) {
          if (rich_text_is_flexable_w_char(iter->text[i])) {
            iter->flexible_w_chars++;
            flexible_w_chars++;
          }
        }
      }
    } else {
      align_h_w -= iter->rect.w;
    }

    iter = iter->next;
  }

  if (flexible_w_chars > 0) {
    flexible_w_chars_w = tk_max(1, (flexible_w / flexible_w_chars));
  } else {
    flexible_w_chars_w = 0;
  }

  iter = row_first_node;
  while (iter != NULL) {
    iter->align_h_w = align_h_w;
    iter->rect.h = row_h;
    iter->rect.x += dx;

    switch (iter->node->type) {
      case RICH_TEXT_TEXT: {
        if (flexible_w > 0) {
          int32_t spacing = iter->flexible_w_chars * flexible_w_chars_w;
          iter->spacing = tk_min(spacing, flexible_w);
          iter->flexible_w_char_delta_w = flexible_w_chars_w;

          flexible_w -= iter->spacing;
          dx += iter->spacing;
        }
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

  return RET_OK;
}

#define MOVE_TO_NEXT_ROW()                                                       \
  x = margin;                                                                    \
  y += row_h + line_gap;                                                         \
  if (row_first_node != NULL) {                                                  \
    rich_text_render_node_tune_row(row_first_node, row_h, flexible_w, client_w); \
    row_first_node = NULL;                                                       \
  }                                                                              \
  row_h = 0;

break_type_t rich_text_line_break_check(wchar_t c1, wchar_t c2) {
  break_type_t break_type = line_break_check(c1, c2);
  if (break_type == LINE_BREAK_NO) {
    break_type = word_break_check(c1, c2);
  }

  return break_type;
}

rich_text_render_node_t* rich_text_render_node_layout(widget_t* widget, rich_text_node_t* node,
                                                      canvas_t* c, int32_t w, int32_t h,
                                                      int32_t margin, int32_t line_gap) {
  int32_t row_h = 0;
  int32_t x = margin;
  int32_t y = margin;
  int32_t right = w - margin;
  int32_t client_w = w - 2 * margin;
  int32_t client_h = h - 2 * margin;
  rich_text_node_t* iter = node;
  rich_text_t* rich_text = RICH_TEXT(widget);
  rich_text_render_node_t* new_node = NULL;
  rich_text_render_node_t* render_node = NULL;
  rich_text_render_node_t* row_first_node = NULL;
  return_value_if_fail(node != NULL && c != NULL && client_w > 0 && client_h > 0, NULL);

  while (iter != NULL) {
    switch (iter->type) {
      case RICH_TEXT_IMAGE: {
        bitmap_t bitmap;
        int32_t flexible_w = 0;
        rich_text_image_t* image = &(iter->u.image);
        const char* name = image->name;
        new_node = rich_text_render_node_create(iter);
        return_value_if_fail(new_node != NULL, render_node);

        if (widget_load_image(widget, name, &bitmap) == RET_OK) {
          if (image->w == 0) {
            image->w = bitmap.w;
          }
          if (image->h == 0) {
            image->h = bitmap.h;
          }
        }

        if (x > margin && (image->w > ICON_SIZE || x + image->w > right)) {
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
        if (x + image->w >= right) {
          MOVE_TO_NEXT_ROW();
        } else {
          if (row_first_node == NULL) {
            row_first_node = new_node;
          }
          x += new_node->rect.w + 1;
        }

        rich_text->content_h = new_node->rect.y + new_node->rect.h;
        break;
      }
      case RICH_TEXT_TEXT: {
        int32_t i = 0;
        float_t tw = 0;
        float_t cw = 0;
        int32_t start = 0;
        int32_t flexible_w = 0;
        int32_t last_breakable = 0;
        wchar_t* str = iter->u.text.text;
        break_type_t break_type = LINE_BREAK_ALLOW;
        int32_t font_size = iter->u.text.font.size;

        if (row_h < font_size) {
          row_h = font_size;
        }
        canvas_set_font(c, iter->u.text.font.name, font_size);

        for (i = 0; str[i]; i++) {
          cw = canvas_measure_text(c, str + i, 1);
          if (i > 0) {
            break_type = rich_text_line_break_check(str[i - 1], str[i]);
          }
          if (str[i] == '\r' || str[i] == '\n') {
            break_type = LINE_BREAK_MUST;
          }

          if ((x + tw + cw) > right || break_type == LINE_BREAK_MUST) {
            if (break_type != LINE_BREAK_MUST) {
              if (last_breakable > start) {
                if (i != last_breakable + 1 || break_type != LINE_BREAK_ALLOW) {
                  tw -= canvas_measure_text(c, str + last_breakable, i - last_breakable);
                  i = last_breakable;
                }
              }
              if (x == margin) {
                // 一行的起始不需要换行，且最少包含一个字符
                if (i == start) {
                  i = start + 1;
                }
              } else if (start == 0 && last_breakable == 0) {
                // 不是起始，换行,重新计算
                MOVE_TO_NEXT_ROW();
                row_h = font_size;
                --i;
                continue;
              }
            }

            new_node = rich_text_render_node_create(iter);
            return_value_if_fail(new_node != NULL, render_node);

            new_node->text = str + start;
            new_node->size = i - start;
            new_node->rect = rect_init(x, y, tw, font_size);

            render_node = rich_text_render_node_append(render_node, new_node);
            if (row_first_node == NULL) {
              row_first_node = new_node;
            }

            if (break_type == LINE_BREAK_MUST) {
              ++i;
              if (str[i - 1] == '\r' && str[i] == '\n') ++i;
            } else {
              if (str[i] == ' ' || str[i] == '\t') {
                i++;
              }
            }

            MOVE_TO_NEXT_ROW();
            row_h = font_size;

            while (str[i] == '\r' || str[i] == '\n') {
              if (str[i] == '\r' && str[i + 1] == '\n') {
                ++i;
              }
              MOVE_TO_NEXT_ROW();
              row_h = font_size;
              ++i;
            }
            start = i;

            if (!str[i]) break;
            last_breakable = i;
            tw = canvas_measure_text(c, str + i, 1);
          } else {
            if (i > 0) {
              if (rich_text_line_break_check(str[i - 1], str[i]) == LINE_BREAK_ALLOW) {
                last_breakable = i;
              }
            }

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
          tw = 0;

          render_node = rich_text_render_node_append(render_node, new_node);
          if (row_first_node == NULL) {
            row_first_node = new_node;
          }
        }

        rich_text->content_h = new_node->rect.y + new_node->rect.h;
        break;
      }
      default:
        break;
    }

    iter = iter->next;
  }

  if (row_first_node != NULL) {
    rich_text_render_node_tune_row(row_first_node, row_h, 0, client_w);
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
