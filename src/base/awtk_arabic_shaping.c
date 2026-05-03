/**
 * File:   awtk_arabic_shaping.c
 * Author: AWTK Develop Team
 * Brief:  Unicode table-driven Arabic joining + shaping (SheenBidi companion, no FriBidi).
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * Data:   tools/gen_awtk_arabic_unicode_inc.py + Unicode ArabicShaping.txt / UnicodeData.txt.
 *         Presentation quadruples match Unicode Arabic Presentation Forms-B (see generator).
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 */

#include "awtk_arabic_shaping.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "tkc/mem.h"
#include "tkc/utils.h"

#include "awtk_arabic_unicode_inc.h"

#define AWTK_AR_SCRATCH_CAP 128

#define AWTK_MASK_JOINS_RIGHT 0x01u
#define AWTK_MASK_JOINS_LEFT 0x02u
#define AWTK_MASK_ARAB_SHAPES 0x04u
#define AWTK_MASK_TRANSPARENT 0x08u
#define AWTK_MASK_IGNORED 0x10u
#define AWTK_MASK_LIGATURED 0x20u

#define AWTK_SET_BITS(x, mask) ((x) |= (mask))
#define AWTK_UNSET_BITS(x, mask) ((x) &= (uint8_t) ~(mask))
#define AWTK_TEST_BITS(x, mask) (((x) & (mask)) != 0)

#define AWTK_LEVEL_IS_RTL(lev) (((lev) & 1) != 0)
#define AWTK_JOIN_SHAPE(p) ((p) & (AWTK_MASK_JOINS_RIGHT | AWTK_MASK_JOINS_LEFT))
#define AWTK_ARAB_SHAPES(p) AWTK_TEST_BITS((p), AWTK_MASK_ARAB_SHAPES)
#define AWTK_IS_JOIN_SKIPPED(p) (((p) & (AWTK_MASK_TRANSPARENT | AWTK_MASK_IGNORED)) != 0)
#define AWTK_IS_JOINING_TYPE_G(p) \
  (((p) & (AWTK_MASK_TRANSPARENT | AWTK_MASK_IGNORED)) == AWTK_MASK_IGNORED)

#define AWTK_JOINS_PRECEDING_MASK(level) \
  (AWTK_LEVEL_IS_RTL(level) ? AWTK_MASK_JOINS_RIGHT : AWTK_MASK_JOINS_LEFT)
#define AWTK_JOINS_FOLLOWING_MASK(level) \
  (AWTK_LEVEL_IS_RTL(level) ? AWTK_MASK_JOINS_LEFT : AWTK_MASK_JOINS_RIGHT)

#define AWTK_SENTINEL_LEVEL (-1)
#define AWTK_CHAR_FILL ((wchar_t)0xfeff)

#define AWTK_AR_PRES_BASE 0x0621u
#define AWTK_AR_PRES_LAST 0x06d3u

typedef struct _awtk_ar_pair_liga_t {
  uint32_t first;
  uint32_t second;
  uint32_t out_cp;
} awtk_ar_pair_liga_t;

/* Mandatory LAM+ALEF ligatures (Unicode presentation forms), sorted by first then second. */
static const awtk_ar_pair_liga_t g_awtk_ar_mandatory_liga[] = {
    {0xFEDFu, 0xFE82u, 0xFEF5u}, {0xFEDFu, 0xFE84u, 0xFEF7u}, {0xFEDFu, 0xFE88u, 0xFEF9u},
    {0xFEDFu, 0xFE8Eu, 0xFEFBu}, {0xFEE0u, 0xFE82u, 0xFEF6u}, {0xFEE0u, 0xFE84u, 0xFEF8u},
    {0xFEE0u, 0xFE88u, 0xFEFAu}, {0xFEE0u, 0xFE8Eu, 0xFEFCu},
};

static uint8_t awtk_ar_join_prop_lookup(uint32_t cp) {
  uint32_t lo = 0;
  uint32_t hi = AWTK_AR_JOIN_RANGE_COUNT;
  while (lo < hi) {
    uint32_t mid = (lo + hi) / 2u;
    const awtk_ar_join_range_t* r = &g_awtk_ar_join_ranges[mid];
    if (cp < r->lo) {
      hi = mid;
    } else if (cp > r->hi) {
      lo = mid + 1u;
    } else {
      return r->prop;
    }
  }
  return 0;
}

static bool_t awtk_ar_cp_in_explicit_bn(uint32_t cp) {
  uint32_t lo = 0;
  uint32_t hi = AWTK_AR_EXPLICIT_BN_RANGE_COUNT;
  while (lo < hi) {
    uint32_t mid = (lo + hi) / 2u;
    const awtk_ar_bn_range_t* r = &g_awtk_ar_explicit_bn_ranges[mid];
    if (cp < r->lo) {
      hi = mid;
    } else if (cp > r->hi) {
      lo = mid + 1u;
    } else {
      return TRUE;
    }
  }
  return FALSE;
}

static int16_t awtk_ar_consistent_level(uint32_t cp, int8_t lev_embed) {
  if (awtk_ar_cp_in_explicit_bn(cp)) {
    return AWTK_SENTINEL_LEVEL;
  }
  return (int16_t)lev_embed;
}

static bool_t awtk_ar_levels_match(int16_t a, int16_t b) {
  return (a == b) || (a == AWTK_SENTINEL_LEVEL) || (b == AWTK_SENTINEL_LEVEL);
}

static void awtk_ar_get_joining_types(const wchar_t* str, uint32_t len, uint8_t* ar_props) {
  uint32_t i;
  for (i = 0; i < len; i++) {
    uint32_t cp = (uint32_t)(wchar_t)str[i];
    ar_props[i] = awtk_ar_join_prop_lookup(cp);
  }
}

static void awtk_join_arabic(const wchar_t* str, uint32_t len, const int8_t* embedding_levels,
                             uint8_t* ar_props) {
  uint32_t saved = 0;
  int16_t saved_level = AWTK_SENTINEL_LEVEL;
  bool_t saved_shapes = FALSE;
  uint8_t saved_joins_following_mask = 0;
  bool_t joins = FALSE;
  uint32_t i;

  if (len == 0) {
    return;
  }

  for (i = 0; i < len; i++) {
    if (!AWTK_IS_JOINING_TYPE_G(ar_props[i])) {
      bool_t disjoin = FALSE;
      bool_t shapes = AWTK_ARAB_SHAPES(ar_props[i]);
      int16_t level = awtk_ar_consistent_level((uint32_t)(wchar_t)str[i], embedding_levels[i]);

      if (joins && !awtk_ar_levels_match(saved_level, level)) {
        disjoin = TRUE;
        joins = FALSE;
      }

      if (!AWTK_IS_JOIN_SKIPPED(ar_props[i])) {
        uint8_t joins_preceding_mask = AWTK_JOINS_PRECEDING_MASK((int8_t)level);

        if (!joins) {
          if (shapes) {
            AWTK_UNSET_BITS(ar_props[i], joins_preceding_mask);
          }
        } else if (!AWTK_TEST_BITS(ar_props[i], joins_preceding_mask)) {
          disjoin = TRUE;
        } else {
          uint32_t j;
          for (j = saved + 1; j < i; j++) {
            AWTK_SET_BITS(ar_props[j], joins_preceding_mask | saved_joins_following_mask);
          }
        }
      }

      if (disjoin && saved_shapes) {
        AWTK_UNSET_BITS(ar_props[saved], saved_joins_following_mask);
      }

      if (!AWTK_IS_JOIN_SKIPPED(ar_props[i])) {
        saved = i;
        saved_level = level;
        saved_shapes = shapes;
        saved_joins_following_mask = AWTK_JOINS_FOLLOWING_MASK((int8_t)level);
        joins = AWTK_TEST_BITS(ar_props[i], saved_joins_following_mask);
      }
    }
  }
  if (joins && saved_shapes) {
    AWTK_UNSET_BITS(ar_props[saved], saved_joins_following_mask);
  }
}

static void awtk_shape_arabic_pres(uint32_t len, const uint8_t* ar_props, wchar_t* str) {
  uint32_t i;
  for (i = 0; i < len; i++) {
    uint32_t ch;
    uint32_t idx;
    uint8_t shape;

    if (!AWTK_ARAB_SHAPES(ar_props[i])) {
      continue;
    }
    ch = (uint32_t)str[i];
    if (ch < AWTK_AR_PRES_BASE || ch > AWTK_AR_PRES_LAST) {
      continue;
    }
    idx = ch - AWTK_AR_PRES_BASE;
    assert(idx < AWTK_AR_PRES_ROW_COUNT);
    shape = AWTK_JOIN_SHAPE(ar_props[i]);
    assert(shape < 4u);
    str[i] = (wchar_t)g_awtk_ar_pres[idx][shape];
  }
}

static int awtk_ar_pair_liga_cmp(const void* a, const void* b) {
  const awtk_ar_pair_liga_t* x = (const awtk_ar_pair_liga_t*)a;
  const awtk_ar_pair_liga_t* y = (const awtk_ar_pair_liga_t*)b;
  if (x->first != y->first) {
    return (x->first < y->first) ? -1 : 1;
  }
  if (x->second != y->second) {
    return (x->second < y->second) ? -1 : 1;
  }
  return 0;
}

static uint32_t awtk_ar_find_mandatory_liga(uint32_t first, uint32_t second) {
  awtk_ar_pair_liga_t key = {first, second, 0};
  const awtk_ar_pair_liga_t* m = (const awtk_ar_pair_liga_t*)bsearch(
      &key, g_awtk_ar_mandatory_liga, ARRAY_SIZE(g_awtk_ar_mandatory_liga), sizeof(g_awtk_ar_mandatory_liga[0]),
      awtk_ar_pair_liga_cmp);
  return m != NULL ? m->out_cp : 0u;
}

static void awtk_shape_arabic_mandatory_liga(const int8_t* embedding_levels, uint32_t len, uint8_t* ar_props,
                                             wchar_t* str) {
  uint32_t i;
  for (i = 0; i + 1 < len; i++) {
    uint32_t out_cp;
    if (!AWTK_LEVEL_IS_RTL((uint8_t)embedding_levels[i])) {
      continue;
    }
    if (embedding_levels[i] != embedding_levels[i + 1]) {
      continue;
    }
    out_cp = awtk_ar_find_mandatory_liga((uint32_t)str[i], (uint32_t)str[i + 1]);
    if (out_cp != 0u) {
      str[i] = AWTK_CHAR_FILL;
      AWTK_SET_BITS(ar_props[i], AWTK_MASK_LIGATURED);
      str[i + 1] = (wchar_t)out_cp;
    }
  }
}

ret_t awtk_arabic_shape_logical(const wchar_t* str, uint32_t size, const uint8_t* embedding_levels,
                                wchar_t* shaped) {
  uint8_t* ar_props = NULL;
  uint8_t ar_props_stack[AWTK_AR_SCRATCH_CAP];
  int8_t* levels = NULL;
  int8_t levels_stack[AWTK_AR_SCRATCH_CAP];
  uint32_t i;

  return_value_if_fail(str != NULL && embedding_levels != NULL && shaped != NULL && size > 0, RET_BAD_PARAMS);

  if (size < AWTK_AR_SCRATCH_CAP) {
    ar_props = ar_props_stack;
    levels = levels_stack;
  } else {
    ar_props = TKMEM_ZALLOCN(uint8_t, size);
    levels = TKMEM_ZALLOCN(int8_t, size);
    if (ar_props == NULL || levels == NULL) {
      TKMEM_FREE(ar_props);
      TKMEM_FREE(levels);
      return RET_FAIL;
    }
  }

  for (i = 0; i < size; i++) {
    levels[i] = (int8_t)embedding_levels[i];
  }

  awtk_ar_get_joining_types(str, size, ar_props);
  awtk_join_arabic(str, size, levels, ar_props);
  awtk_shape_arabic_pres(size, ar_props, shaped);
  awtk_shape_arabic_mandatory_liga(levels, size, ar_props, shaped);

  if (size >= AWTK_AR_SCRATCH_CAP) {
    TKMEM_FREE(ar_props);
    TKMEM_FREE(levels);
  }

  return RET_OK;
}
