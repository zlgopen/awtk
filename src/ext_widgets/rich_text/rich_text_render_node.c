/**
 * File:   rich_text_render_node.c
 * Author: AWTK Develop Team
 * Brief:  rich_text_render_node
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/bidi.h"
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
                                     int32_t line_base, int32_t flexible_w, int32_t client_w) {
  uint32_t i = 0;
  int32_t dx = 0;
  int32_t flexible_w_chars = 0;
  int32_t flexible_w_chars_w = 0;
  int32_t align_h_w = flexible_w > 0 ? 0 : client_w;
  rich_text_render_node_t* iter = row_first_node;

  while (iter != NULL) {
    if (flexible_w > 0) {
      if (iter->node->type == RICH_TEXT_TEXT) {
        uint32_t end = iter->size;
        for (i = 0; i < end; i++) {
          const glyph_t* g = glyphs_get(iter->glyphs, iter->glyphs_array[i]);
          if (g != NULL && rich_text_is_flexable_w_char(g->chr)) {
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
    iter->line_base = line_base;

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

#define MOVE_TO_NEXT_ROW()                                                                  \
  x = margin;                                                                               \
  y += row_h + line_gap;                                                                    \
  if (row_first_node != NULL) {                                                             \
    rich_text_render_node_tune_row(row_first_node, row_h, line_base, flexible_w, client_w); \
    row_first_node = NULL;                                                                  \
  }                                                                                         \
  row_h = 0;                                                                                \
  line_base = 0;

/* 创建一个 render_node 并填充文本行数据，通过接口批量获取字模序号集合 */
static rich_text_render_node_t* rich_text_render_node_push(
    rich_text_node_t* node, rich_text_render_node_t* render_node,
    rich_text_render_node_t** row_first_node, rich_text_render_node_t** out_node, glyphs_t* glyphs,
    uint32_t str_start, uint32_t str_len, int32_t x, int32_t y, int32_t tw, int32_t height) {
  int32_t glyph_num = 0;
  int32_t* arr = NULL;
  rich_text_render_node_t* new_node = NULL;

  arr = glyphs_get_glyph_indexs_from_str_indexs(glyphs, str_start, str_len, NULL, 0, &glyph_num);

  new_node = rich_text_render_node_create(node);
  return_value_if_fail(new_node != NULL, render_node);

  new_node->glyphs = glyphs;
  new_node->index = str_start;
  new_node->size = glyph_num > 0 ? (uint16_t)glyph_num : 0;
  new_node->rect = rect_init(x, y, tw, height);
  new_node->glyphs_array = arr;

  render_node = rich_text_render_node_append(render_node, new_node);
  if (*row_first_node == NULL) {
    *row_first_node = new_node;
  }
  if (out_node != NULL) {
    *out_node = new_node;
  }
  return render_node;
}

break_type_t rich_text_line_break_check(glyphs_t* glyphs, uint32_t index) {
  break_type_t break_type = LINE_BREAK_NO;
  uint32_t real_index = glyphs_get_glyph_index_from_str_index(glyphs, index);
  uint32_t last_real_index = glyphs_get_glyph_index_from_str_index(glyphs, index - 1);
  const glyph_t* g1 = glyphs_get(glyphs, last_real_index);
  const glyph_t* g2 = glyphs_get(glyphs, real_index);
  if (g1 != NULL && g2 != NULL) {
    break_type = line_break_check(g1->chr, g2->chr);
    if (break_type == LINE_BREAK_NO) {
      break_type = word_break_check(g1->chr, g2->chr);
    }
  }
  return break_type;
}

rich_text_render_node_t* rich_text_render_node_layout(widget_t* widget, rich_text_node_t* node,
                                                      canvas_t* c, int32_t w, int32_t h,
                                                      int32_t margin, int32_t line_gap) {
  int32_t row_h = 0;
  int32_t line_base = 0;
  int32_t x = margin;
  int32_t y = margin;
  uint32_t glyph_count = 0;
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

        if (new_node != NULL) {
          rich_text->content_h = new_node->rect.y + new_node->rect.h;
        }
        break;
      }
      case RICH_TEXT_TEXT: {
        int32_t i = 0;
        float_t tw = 0;
        float_t cw = 0;
        uint32_t start = 0;
        uint32_t str_len = 0;
        uint32_t real_index = 0;
        uint32_t last_breakable = 0;
        float_t tw_last_breakable = 0;
        glyphs_t* glyphs = NULL;
        const glyph_t* g = NULL;
        int32_t height = 0;
        int32_t flexible_w = 0;
        font_vmetrics_t vmetrics;
        font_raster_params_t params;
        wchar_t* str = iter->u.text.text;
        int32_t font_size = iter->u.text.font.size;
        break_type_t break_type = LINE_BREAK_ALLOW;

        canvas_set_font(c, iter->u.text.font.name, font_size);
        str_len = wcslen(str);
        vmetrics = font_get_vmetrics(c->font, c->font_size);
        height = vmetrics.ascent - vmetrics.descent;

        if (row_h < height) {
          row_h = height;
        }
        if (line_base < row_h + vmetrics.descent) {
          line_base = row_h + vmetrics.descent;
        }

        font_get_raster_params(c->font, &params);
        params.bidi_type = bidi_type_from_name(widget_get_bidi(widget));
        params.bold = iter->u.text.font.bold;
        params.italic = iter->u.text.font.italic;
        params.underline = iter->u.text.font.underline;
        glyphs = font_create_glyphs(c->font, str, str_len, c->font_size, &params);
        break_if_fail(glyphs != NULL);
        str_len = glyphs_get_str_length(glyphs);

        for (i = 0; i < str_len;) {
          real_index = glyphs_get_glyph_index_from_str_index(glyphs, i);
          g = glyphs_get(glyphs, real_index);
          break_if_fail(g != NULL);
          cw = glyphs_measure(glyphs, real_index, g->glyph_count);

          if (i > 0) {
            if (rich_text->word_wrap) {
              break_type = rich_text_line_break_check(glyphs, i);
            } else {
              break_type = LINE_BREAK_ALLOW;
            }
          }
          if (g->chr == '\r' || g->chr == '\n') {
            break_type = LINE_BREAK_MUST;
          }

          if ((x + tw + cw) > right || break_type == LINE_BREAK_MUST) {
            if (break_type != LINE_BREAK_MUST) {
              if (last_breakable > start) {
                if (i != last_breakable + 1 || break_type != LINE_BREAK_ALLOW) {
                  tw = tw_last_breakable;
                  i = last_breakable;
                }
              }
              if (x == margin) {
                /* 一行的起始不需要换行，且最少包含一个字模 */
                if (i == start) {
                  tw += cw;
                  i += g->str_count;
                }
              } else if (start == 0 && last_breakable == 0) {
                /* 不是起始，换行后重新从当前字符开始计算 */
                MOVE_TO_NEXT_ROW();
                row_h = height;
                line_base = row_h + vmetrics.descent;
                continue;
              }
            }

            render_node = rich_text_render_node_push(iter, render_node, &row_first_node, &new_node,
                                                     glyphs, start, i - start, x, y, tw, height);
            return_value_if_fail(new_node != NULL, render_node);

            if (break_type == LINE_BREAK_MUST) {
              i += g->str_count;
              if (g->chr == '\r' && i < str_len) {
                if (str[i - 1] == '\r' && str[i] == '\n') ++i;
              }
            } else {
              if (str[i] == ' ' || str[i] == '\t') {
                i++;
              }
            }

            MOVE_TO_NEXT_ROW();
            row_h = height;
            line_base = row_h + vmetrics.descent;

            /* 连续换行符：每个占一行 */
            while (str[i] == '\r' || str[i] == '\n') {
              if (i + 1 < str_len && str[i] == '\r' && str[i + 1] == '\n') {
                ++i;
              }
              MOVE_TO_NEXT_ROW();
              row_h = height;
              line_base = row_h + vmetrics.descent;
              ++i;
            }

            start = i;
            last_breakable = i;
            real_index = glyphs_get_glyph_index_from_str_index(glyphs, i);
            g = glyphs_get(glyphs, real_index);
            if (g != NULL) {
              tw = glyphs_measure(glyphs, i, g->glyph_count);
              i += g->str_count;
            }
            continue;
          } else {
            if (i > 0 && rich_text->word_wrap) {
              if (rich_text_line_break_check(glyphs, i) == LINE_BREAK_ALLOW) {
                last_breakable = i;
                tw_last_breakable = tw;
              }
            } else {
              last_breakable = i;
            }
            tw += cw;
            i += g->str_count;
          }
        }

        if (i > start) {
          render_node = rich_text_render_node_push(iter, render_node, &row_first_node, &new_node,
                                                   glyphs, start, i - start, x, y, tw, height);
          return_value_if_fail(new_node != NULL, render_node);
          x += tw + 1;
          tw = 0;
        }

        if (row_first_node != NULL) {
          row_first_node->free_glyphs = TRUE;
        }
        if (new_node != NULL) {
          rich_text->content_h = new_node->rect.y + new_node->rect.h;
        }
        break;
      }

      default:
        break;
    }

    iter = iter->next;
  }

  if (row_first_node != NULL) {
    rich_text_render_node_tune_row(row_first_node, row_h, line_base, 0, client_w);
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
    if (iter->free_glyphs && iter->glyphs != NULL) {
      glyphs_destroy(iter->glyphs);
      iter->glyphs = NULL;
    }
    if (iter->glyphs_array != NULL) {
      TKMEM_FREE(iter->glyphs_array);
    }
    memset(iter, 0x00, sizeof(rich_text_render_node_t));
    TKMEM_FREE(iter);

    iter = next;
  }

  return RET_OK;
}
