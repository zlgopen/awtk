/**
 * File:   bidi.h
 * Author: AWTK Develop Team
 * Brief:  Unicode Bidirectional Algorithm.
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-07-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/bidi.h"

#ifdef WITH_TEXT_BIDI
#include "tkc/mem.h"
#include "fribidi/fribidi.h"

static bidi_type_t bidi_type_from(int fribidi_type) {
  switch (fribidi_type) {
    case FRIBIDI_PAR_RTL:
      return BIDI_TYPE_RTL;
    case FRIBIDI_PAR_LTR:
      return BIDI_TYPE_LTR;
    case FRIBIDI_TYPE_RLO:
      return BIDI_TYPE_RLO;
    case FRIBIDI_TYPE_LRO:
      return BIDI_TYPE_LRO;
    case FRIBIDI_PAR_WRTL:
      return BIDI_TYPE_WRTL;
    case FRIBIDI_PAR_WLTR:
      return BIDI_TYPE_WLTR;
    default:
      return BIDI_TYPE_AUTO;
  }
}

static FriBidiParType bidi_type_to(bidi_type_t type) {
  switch (type) {
    case BIDI_TYPE_RTL:
      return FRIBIDI_PAR_RTL;
    case BIDI_TYPE_LTR:
      return FRIBIDI_PAR_LTR;
    case BIDI_TYPE_RLO:
      return FRIBIDI_TYPE_RLO;
    case BIDI_TYPE_LRO:
      return FRIBIDI_TYPE_LRO;
    case BIDI_TYPE_WRTL:
      return FRIBIDI_PAR_WRTL;
    case BIDI_TYPE_WLTR:
      return FRIBIDI_PAR_WLTR;
    default:
      return FRIBIDI_PAR_ON;
  }
}

bidi_type_t bidi_type_from_name(const char* name) {
  if (name == NULL || *name == '\0') {
    return BIDI_TYPE_AUTO;
  }

  if (tk_str_eq(name, "rtl")) {
    return BIDI_TYPE_RTL;
  } else if (tk_str_eq(name, "ltr")) {
    return BIDI_TYPE_LTR;
  } else if (tk_str_eq(name, "rlo")) {
    return BIDI_TYPE_RLO;
  } else if (tk_str_eq(name, "lro")) {
    return BIDI_TYPE_LRO;
  } else if (tk_str_eq(name, "wrtl")) {
    return BIDI_TYPE_WRTL;
  } else if (tk_str_eq(name, "wltr")) {
    return BIDI_TYPE_WLTR;
  } else {
    return BIDI_TYPE_AUTO;
  }
}

bidi_t* bidi_init(bidi_t* bidi, bool_t alloc_l2v, bool_t alloc_v2l, bidi_type_t type) {
  return_value_if_fail(bidi != NULL, NULL);
  memset(bidi, 0x00, sizeof(*bidi));

  bidi->request_type = type;
  bidi->alloc_l2v = alloc_l2v;
  bidi->alloc_v2l = alloc_v2l;

  return bidi;
}

ret_t bidi_log2vis(bidi_t* bidi, const wchar_t* str, uint32_t size) {
  FriBidiLevel level = 0;
  FriBidiParType type = FRIBIDI_PAR_ON;
  return_value_if_fail(bidi != NULL && str != NULL && size > 0, RET_BAD_PARAMS);
  if (ARRAY_SIZE(bidi->vis_str_static) > size) {
    bidi->vis_str_size = size;
    bidi->vis_str = bidi->vis_str_static;
  } else {
    bidi->vis_str_size = size;
    assert(bidi->vis_str == NULL);
    bidi->vis_str = TKMEM_ALLOC((size + 1) * sizeof(wchar_t));
  }
  return_value_if_fail(bidi->vis_str != NULL, RET_FAIL);

  return_value_if_fail(sizeof(wchar_t) == sizeof(FriBidiChar), RET_BAD_PARAMS);
  return_value_if_fail(sizeof(uint32_t) == sizeof(FriBidiStrIndex), RET_BAD_PARAMS);

  if (bidi->alloc_l2v) {
    bidi->positions_L_to_V = TKMEM_ZALLOCN(int32_t, size);
  }
  if (bidi->alloc_v2l) {
    bidi->positions_L_to_V = TKMEM_ZALLOCN(int32_t, size);
  }

  type = bidi_type_to(bidi->request_type);
  level = fribidi_log2vis(str, size, &type, bidi->vis_str, bidi->positions_L_to_V,
                          bidi->positions_V_to_L, NULL);
  bidi->resolved_type = bidi_type_from(type);
  (void)level;

  return RET_OK;
}

ret_t bidi_deinit(bidi_t* bidi) {
  return_value_if_fail(bidi != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(bidi->positions_L_to_V);
  TKMEM_FREE(bidi->positions_V_to_L);
  if (bidi->vis_str != bidi->vis_str_static) {
    TKMEM_FREE(bidi->vis_str);
  }
  memset(bidi, 0x00, sizeof(*bidi));

  return RET_OK;
}

#endif /*WITH_TEXT_BIDI*/
