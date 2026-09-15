/**
 * File:   font.c
 * Author: AWTK Develop Team
 * Brief:  font interface
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/events.h"
#include "base/font.h"
#include "tkc/wchar32.h"
#include "base/system_info.h"

static ret_t glyphs_on_unload_font(void* ctx, event_t* e) {
  glyphs_t* g = (glyphs_t*)(ctx);

  if (e->type == EVT_ASSET_MANAGER_UNLOAD_ASSET) {
    if (g != NULL && tk_str_eq(g->font->name, (char*)e->target)) {
      g->valid = FALSE;
      g->font = NULL;
    }
  }

  return RET_REMOVE;
}

static ret_t glyphs_on_destroy_emitter(void* ctx) {
  emitter_item_t* item = (emitter_item_t*)(ctx);
  glyphs_t* g = item->on_destroy_ctx;
  if (g != NULL) {
    g->valid = FALSE;
    g->font = NULL;
  }
  return RET_OK;
}

ret_t font_get_glyph(font_t* f, wchar_t chr, font_size_t font_size, glyph_t* g) {
  return_value_if_fail(f != NULL && f->get_glyph != NULL && g != NULL, RET_BAD_PARAMS);
#ifdef WITH_GPU
  if (system_info() != NULL) font_size *= system_info()->device_pixel_ratio;
#endif
  return f->get_glyph(f, chr, font_size, g);
}

glyphs_t* font_create_glyphs(font_t* f, const wchar_t* str, uint32_t len, font_size_t font_size,
                             font_raster_params_t* params) {
  glyphs_t* glyphs = NULL;
  return_value_if_fail(f != NULL && f->create_glyphs != NULL && str != NULL && len > 0, NULL);
  if (params == NULL) {
    params = &f->params;
  }
#ifdef WITH_GPU
  if (system_info() != NULL) font_size *= system_info()->device_pixel_ratio;
#endif
  glyphs = f->create_glyphs(f, str, len, font_size, params);
  if (glyphs != NULL) {
    int32_t id =
        emitter_on(EMITTER(f->fm), EVT_ASSET_MANAGER_UNLOAD_ASSET, glyphs_on_unload_font, glyphs);
    emitter_set_on_destroy(EMITTER(f->fm), id, glyphs_on_destroy_emitter, glyphs);
  }
  return glyphs;
}

ret_t font_set_raster_params(font_t* font, const font_raster_params_t* params) {
  return_value_if_fail(font != NULL && params != NULL, RET_BAD_PARAMS);
  memcpy(&font->params, params, sizeof(font_raster_params_t));
  return RET_OK;
}

ret_t font_get_raster_params(font_t* font, font_raster_params_t* params) {
  return_value_if_fail(font != NULL && params != NULL, RET_BAD_PARAMS);
  memcpy(params, &font->params, sizeof(font_raster_params_t));
  return RET_OK;
}

ret_t font_shrink_cache(font_t* f, uint32_t cache_size) {
  return_value_if_fail(f != NULL, RET_BAD_PARAMS);

  if (f->shrink_cache != NULL) {
    f->shrink_cache(f, cache_size);
  }

  return RET_OK;
}

font_vmetrics_t font_get_vmetrics(font_t* f, font_size_t font_size) {
  font_vmetrics_t vmetrics = {font_size, 0, 0};
  if (f != NULL && f->get_vmetrics != NULL) {
    return f->get_vmetrics(f, font_size);
  } else {
    return vmetrics;
  }
}

bool_t font_match(font_t* f, const char* name, font_size_t font_size) {
  return_value_if_fail(f != NULL && f->match != NULL, FALSE);

  return f->match(f, name, font_size);
}

ret_t font_destroy(font_t* f) {
  return_value_if_fail(f != NULL && f->destroy != NULL, RET_BAD_PARAMS);

  return f->destroy(f);
}

glyph_t* glyph_create(void) {
  return TKMEM_ZALLOC(glyph_t);
}

glyph_t* glyph_clone(glyph_t* old) {
  glyph_t* g = NULL;
  return_value_if_fail(old != NULL, NULL);

  g = glyph_create();
  return_value_if_fail(g != NULL, NULL);
  *g = *old;

  return g;
}

ret_t glyph_destroy(glyph_t* g) {
  return_value_if_fail(g != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(g);

  return RET_OK;
}

glyphs_t* glyphs_create(wchar_t* str, uint32_t glyph_len, uint32_t str_length, bool_t has_maps,
                        font_size_t font_size, const font_vmetrics_t* vmetrics) {
  glyphs_t* glyphs = NULL;
  return_value_if_fail(glyph_len > 0, NULL);

  glyphs = TKMEM_ZALLOC(glyphs_t);
  return_value_if_fail(glyphs != NULL, NULL);

  glyphs->glyphs = TKMEM_ZALLOCN(glyph_t, glyph_len);
  goto_error_if_fail(glyphs->glyphs != NULL);

  glyphs->str_length = tk_min(wcslen(str), str_length);
  glyphs->str = TKMEM_CALLOC(glyphs->str_length + 1, sizeof(wchar_t));
  goto_error_if_fail(glyphs->str != NULL);
  memcpy(glyphs->str, str, glyphs->str_length * sizeof(wchar_t));

  if (has_maps) {
    glyphs->l2v_maps = TKMEM_ZALLOCN(int32_t, glyphs->str_length);
    goto_error_if_fail(glyphs->l2v_maps != NULL);
    glyphs->v2l_maps = TKMEM_ZALLOCN(int32_t, glyph_len);
    goto_error_if_fail(glyphs->v2l_maps != NULL);
  }

  glyphs->descent = vmetrics->descent;
  glyphs->ascent = vmetrics->ascent;
  glyphs->font_size = font_size;
  glyphs->length = glyph_len;
  glyphs->valid = TRUE;
  return glyphs;
error:
  glyphs_destroy(glyphs);
  return NULL;
}

int32_t glyphs_get_glyph_index_from_str_index(glyphs_t* glyphs, uint32_t str_index) {
  return_value_if_fail(glyphs != NULL && glyphs->valid == TRUE, -1);
  if (str_index >= glyphs->str_length) {
    return -1;
  }
  if (glyphs->l2v_maps != NULL) {
    return glyphs->l2v_maps[str_index];
  }
  return str_index;
}

static int compare_uint32(const void* a, const void* b) {
  uint32_t va = *(const uint32_t*)a;
  uint32_t vb = *(const uint32_t*)b;
  return va < vb ? -1 : (va > vb ? 1 : 0);
}

int32_t* glyphs_get_glyph_indexs_from_str_indexs(glyphs_t* glyphs, uint32_t str_start, uint32_t len,
                                                 int32_t* dst, uint32_t dst_cap, int32_t* ret_cap) {
  uint32_t i = 0;
  uint32_t count = 0;
  uint32_t need_cap = 0;

  if (ret_cap != NULL) {
    *ret_cap = 0;
  }
  return_value_if_fail(glyphs != NULL && glyphs->valid == TRUE, NULL);
  return_value_if_fail(str_start <= glyphs->str_length, NULL);
  len = tk_min(len, glyphs->str_length - str_start);

  /* 预估所需容量 */
  need_cap = len > 0 ? len : 1;
  if (dst == NULL || dst_cap < need_cap) {
    dst_cap = need_cap;
    dst = (int32_t*)TKMEM_REALLOCT(int32_t, dst, dst_cap);
    return_value_if_fail(dst != NULL, NULL);
  }

  if (glyphs->l2v_maps == NULL) {
    /* 无 bidi 映射：str 序号即字模序号，字模物理连续、单字形簇，结果天然升序，无需排序 */
    for (i = 0; i < len; i++) {
      uint32_t real_index = str_start + i;
      if (real_index >= glyphs->length) {
        break;
      }
      if (count >= dst_cap) {
        dst_cap = dst_cap * 2;
        dst = (int32_t*)TKMEM_REALLOCT(int32_t, dst, dst_cap);
        return_value_if_fail(dst != NULL, NULL);
      }
      dst[count++] = (int32_t)real_index;
    }
  } else {
    /* bidi/harfbuzz 路径：经 l2v 取 cluster 首字形，再沿 next_glyph 展开同簇后续字形 */
    int32_t* l2v = glyphs->l2v_maps;
    int32_t last_head = -1;
    for (i = 0; i < len; i++) {
      int32_t real_index = l2v[str_start + i];
      const glyph_t* g = NULL;
      if (real_index < 0 || (uint32_t)real_index >= glyphs->length) {
        continue;
      }
      if (real_index == last_head) {
        continue;
      }
      if (count >= dst_cap) {
        dst_cap = dst_cap * 2;
        dst = (int32_t*)TKMEM_REALLOCT(int32_t, dst, dst_cap);
        return_value_if_fail(dst != NULL, NULL);
      }
      dst[count++] = real_index;
      last_head = real_index;
      g = &glyphs->glyphs[real_index];
      while (g->next_glyph != NULL) {
        if (count >= dst_cap) {
          dst_cap = dst_cap * 2;
          dst = (int32_t*)TKMEM_REALLOCT(int32_t, dst, dst_cap);
          return_value_if_fail(dst != NULL, NULL);
        }
        dst[count++] = (++real_index);
        g = g->next_glyph;
      }
    }
    qsort(dst, count, sizeof(int32_t), compare_uint32);
  }

  if (ret_cap != NULL) {
    *ret_cap = (int32_t)count;
  }
  return dst;
}

int32_t glyphs_get_str_index_from_glyph_index(glyphs_t* glyphs, uint32_t glyph_index) {
  return_value_if_fail(glyphs != NULL && glyphs->valid == TRUE, -1);
  if (glyph_index >= glyphs->length) {
    return -1;
  }
  if (glyphs->v2l_maps != NULL) {
    return glyphs->v2l_maps[glyph_index];
  }
  return glyph_index;
}

const wchar_t* glyphs_get_str(glyphs_t* glyphs) {
  return_value_if_fail(glyphs != NULL && glyphs->valid == TRUE, NULL);
  return (const wchar_t*)glyphs->str;
}

uint32_t glyphs_get_str_length(glyphs_t* glyphs) {
  return_value_if_fail(glyphs != NULL && glyphs->valid == TRUE, 0);
  return glyphs->str_length;
}

uint32_t glyphs_get_length(glyphs_t* glyphs) {
  return_value_if_fail(glyphs != NULL && glyphs->valid == TRUE, 0);
  return glyphs->length;
}

bool_t glyphs_get_valid(glyphs_t* glyphs) {
  return_value_if_fail(glyphs != NULL, FALSE);
  return glyphs->valid;
}

font_size_t glyphs_get_font_size(glyphs_t* glyphs) {
  return_value_if_fail(glyphs != NULL && glyphs->valid == TRUE, 0);
#ifdef WITH_GPU
  return glyphs->font_size / system_info()->device_pixel_ratio;
#else
  return glyphs->font_size;
#endif
}

font_size_t glyphs_get_physical_font_size(glyphs_t* glyphs) {
  return_value_if_fail(glyphs != NULL && glyphs->valid == TRUE, 0);
  return glyphs->font_size;
}

const glyph_t* glyphs_get(glyphs_t* glyphs, uint32_t index) {
  return_value_if_fail(glyphs != NULL && glyphs->valid == TRUE, NULL);
  if (index < glyphs->length) {
    return &glyphs->glyphs[index];
  } else {
    return NULL;
  }
}

int16_t glyphs_get_font_ascent(glyphs_t* glyphs) {
  return_value_if_fail(glyphs != NULL && glyphs->valid == TRUE, 0);
#ifdef WITH_GPU
  return glyphs->ascent / system_info()->device_pixel_ratio;
#else
  return glyphs->ascent;
#endif
}

int16_t glyphs_get_physical_font_ascent(glyphs_t* glyphs) {
  return_value_if_fail(glyphs != NULL && glyphs->valid == TRUE, 0);
  return glyphs->ascent;
}

int32_t glyphs_get_height(glyphs_t* glyphs) {
  int16_t h = 0;
  return_value_if_fail(glyphs != NULL && glyphs->valid == TRUE, -1);
  h = glyphs->ascent - glyphs->descent;
#ifdef WITH_GPU
  return h / system_info()->device_pixel_ratio;
#else
  return h;
#endif
}

float_t glyphs_measure(glyphs_t* glyphs, uint32_t start, uint32_t len) {
  float_t w = 0;
  uint32_t i = 0, end = start + len;
  return_value_if_fail(
      glyphs != NULL && glyphs->valid == TRUE && start < glyphs->length && end <= glyphs->length,
      0.0f);

  for (i = start; i < end; i++) {
    const glyph_t* glyph = &glyphs->glyphs[i];
    if (glyph != NULL) {
      w += glyph->advance;
    } else {
      w += 4;
    }
  }
#ifdef WITH_GPU
  return w / system_info()->device_pixel_ratio;
#else
  return w;
#endif
}

ret_t glyphs_destroy(glyphs_t* glyphs) {
  return_value_if_fail(glyphs != NULL, RET_BAD_PARAMS);
  if (glyphs->font != NULL) {
    emitter_off_by_func(EMITTER(glyphs->font->fm), EVT_ASSET_MANAGER_UNLOAD_ASSET,
                        glyphs_on_unload_font, glyphs);
  }
  if (glyphs->l2v_maps != NULL) {
    TKMEM_FREE(glyphs->l2v_maps);
    glyphs->l2v_maps = NULL;
  }
  if (glyphs->v2l_maps != NULL) {
    TKMEM_FREE(glyphs->v2l_maps);
    glyphs->v2l_maps = NULL;
  }
  if (glyphs->glyphs != NULL) {
    TKMEM_FREE(glyphs->glyphs);
    glyphs->glyphs = NULL;
  }
  if (glyphs->str != NULL) {
    TKMEM_FREE(glyphs->str);
    glyphs->str = NULL;
  }
  TKMEM_FREE(glyphs);
  return RET_OK;
}
