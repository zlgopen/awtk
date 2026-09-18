/**
 * File:   harfbuzz_tools.c
 * Author: AWTK Develop Team
 * Brief:  harfbuzz tools
 *
 * Copyright (c) 2018 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-03-27 Zhao Yingjian <zhaoyingjian@zlg.cn> created
 *
 */

#include "base/types_def.h"
#include "harfbuzz_tools.h"

static glyph_t* hb_get_last_glyph(glyphs_t* glyphs, uint32_t str_index) {
  int32_t index = 0;
  const glyph_t* g = NULL;
  uint32_t str_len = 0;

  if (glyphs == NULL) {
    return NULL;
  }

  str_len = glyphs->str_length;
  if (str_index >= str_len) {
    return NULL;
  }

  index = glyphs_get_glyph_index_from_str_index(glyphs, str_index);
  return_value_if_fail(index >= 0, NULL);
  g = glyphs_get(glyphs, index);
  while (g != NULL && g->next_glyph != NULL && g->next_glyph != g) {
    g = g->next_glyph;
  }

  return (glyph_t*)g;
}

#if defined(WITH_HARFBUZZ)
ret_t hb_set_glyphs_maps(glyphs_t* glyphs, hb_info_t* info, uint32_t len) {
  uint32_t i = 0;
  uint32_t j = 0;
  glyph_t* g = NULL;
  uint32_t str_len = 0;
  int32_t* maps = NULL;
  int32_t* maps1 = NULL;
  uint32_t v2l_len = 0;
  uint32_t maps_len = 0;
  int32_t cur_cluster = 0;
  int32_t last_cluster = -1;

  if (glyphs == NULL || glyphs->l2v_maps == NULL ||
    glyphs->v2l_maps == NULL || info == NULL) {
    return RET_FAIL;
  }

  maps = glyphs->l2v_maps;
  maps1 = glyphs->v2l_maps;
  str_len = glyphs_get_str_length(glyphs);
  maps_len = tk_min(len, str_len);

  for (i = 0; i < str_len; i++) {
    maps[i] = -1;
  }

  for (i = 0; i < maps_len; i++) {
    cur_cluster = info[i].cluster;
    if (cur_cluster < 0 || (uint32_t)cur_cluster >= str_len) {
      continue;
    }
    if (cur_cluster == last_cluster) {
      g = hb_get_last_glyph(glyphs, cur_cluster);
      if (g != NULL && g != &glyphs->glyphs[i]) {
        g->next_glyph = &glyphs->glyphs[i];
      }
    } else {
      maps[cur_cluster] = i;
    }
    last_cluster = cur_cluster;
  }

  // 有多个字符合并一个字模的情况出现
  for (i = 0; i < str_len; i++) {
    if (maps[i] < 0) {
      if (i == 0) {
        j = 1;
        while (j < str_len && maps[j] < 0) ++j;
        maps[i] = maps[j];
      } else {
        maps[i] = maps[i - 1];
      }
    }
  }

  /* 簇首字形沿 next_glyph 链一次性算出整簇的 str_count/glyph_count 并直接赋值。*/
  v2l_len = tk_min(len, glyphs->length);
  for (i = 0; i < v2l_len; i++) {
    cur_cluster = info[i].cluster;
    if (cur_cluster < 0 || (uint32_t)cur_cluster >= str_len) {
      maps1[i] = -1;
      glyphs->glyphs[i].str_count = 0;
      glyphs->glyphs[i].glyph_count = 0;
      continue;
    }
    maps1[i] = cur_cluster;
    if (maps[cur_cluster] == (int32_t)i) {
      uint16_t sc = 0;
      uint16_t gc = 0;
      const glyph_t* g = NULL;
      uint32_t s = 0;
      uint32_t k = 0;
      for (s = (uint32_t)cur_cluster; s < str_len && maps[s] == (int32_t)i; s++) {
        sc++;
      }
      for (g = &glyphs->glyphs[i]; g != NULL; g = g->next_glyph) {
        gc++;
      }
      for (k = 0; k < gc; k++) {
        glyphs->glyphs[i + k].str_count = sc;
        glyphs->glyphs[i + k].glyph_count = gc;
      }
    }
  }

  return RET_OK;
}

ret_t hb_set_hb_glyphs_chr(glyphs_t* glyphs, hb_info_t* info, uint32_t glyph_len) {
  return_value_if_fail(glyphs!= NULL, RET_BAD_PARAMS);
  uint32_t i = 0;
  glyph_t* g = NULL;
  int32_t cur_cluster = 0;

  if (info == NULL || glyph_len <= 0) {
    return RET_BAD_PARAMS;
  }

  // 通过cluster获取当前字模对应原来的字符，cluster是harfbuzz自带的属性，可以获取原来字符的index
  for (i = 0; i < glyph_len; i++) {
    g = &glyphs->glyphs[i];
    cur_cluster = info[i].cluster;
    if (g != NULL && cur_cluster >= 0 && (uint32_t)cur_cluster < glyphs->str_length) {
      g->chr = (uint16_t)glyphs->str[cur_cluster];
    }
  }

  return RET_OK;
}

ret_t hb_set_normal_glyphs_chr(glyphs_t* glyphs, bidi_t* b) {
  return_value_if_fail(glyphs!= NULL, RET_BAD_PARAMS);
  uint32_t i = 0;
  glyph_t* g = NULL;
  int32_t cur_cluster = 0;
  uint32_t len = glyphs->length;

  if (b != NULL && b->vis_str != NULL) {
    for (i = 0; i < len; i++) {
      g = &glyphs->glyphs[i];
      g->chr = (uint16_t)b->vis_str[i];
    }
  } else {
    for (i = 0; i < len; i++) {
      g = &glyphs->glyphs[i];
      g->chr = (uint16_t)glyphs->str[i];
    }
  }

  /* 无 hb 信息：每个字模自成单字形簇，str_count/glyph_count 置 1 */
  for (i = 0; i < len; i++) {
    glyphs->glyphs[i].str_count = 1;
    glyphs->glyphs[i].glyph_count = 1;
  }

  return RET_OK;
}

#else 
ret_t hb_set_glyphs_maps(glyphs_t* glyphs, hb_info_t* info, uint32_t len) {
  return RET_NOT_IMPL;
}

ret_t hb_set_hb_glyphs_chr(glyphs_t* glyphs, hb_info_t* info, uint32_t len) {
  return RET_NOT_IMPL;
}

ret_t hb_set_normal_glyphs_chr(glyphs_t* glyphs, bidi_t* b) {
  return RET_NOT_IMPL; 
}
#endif /*WITH_HARFBUZZ_DATA_STB_FONT || WITH_HARFBUZZ_DATA_FREETYPE_FONT*/
