/**
 * File:   bidi.c
 * Author: AWTK Develop Team
 * Brief:  Unicode Bidirectional Algorithm.
 */

#include "base/bidi.h"

#ifdef WITH_TEXT_BIDI

#include <assert.h>
#include <string.h>

#include "tkc/mem.h"
#include "tkc/str.h"

#if defined(WITH_BIDI_SHEEN) && defined(WITH_BIDI_FRIBIDI)
#error "Define at most one of WITH_BIDI_SHEEN or WITH_BIDI_FRIBIDI"
#elif !defined(WITH_BIDI_SHEEN) && !defined(WITH_BIDI_FRIBIDI)
#error "WITH_TEXT_BIDI requires WITH_BIDI_SHEEN or WITH_BIDI_FRIBIDI"
#endif

#if defined(WITH_BIDI_FRIBIDI)
#include "fribidi/fribidi.h"
#endif

#if defined(WITH_BIDI_SHEEN)
#include <SheenBidi/SBBase.h>
#include <SheenBidi/SBCodepointSequence.h>
#include <SheenBidi/SBText.h>
#include <SheenBidi/SBTextConfig.h>
#include <SheenBidi/SBTextIterators.h>
#endif

#if defined(WITH_BIDI_FRIBIDI)
static bidi_type_t bidi_type_from_fribidi_par_type(int fribidi_type) {
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

static FriBidiParType bidi_type_to_fribidi_par_type(bidi_type_t type) {
  switch (type) {
    case BIDI_TYPE_RTL:
      return (FriBidiParType)FRIBIDI_PAR_RTL;
    case BIDI_TYPE_LTR:
      return (FriBidiParType)FRIBIDI_PAR_LTR;
    case BIDI_TYPE_RLO:
      return (FriBidiParType)FRIBIDI_TYPE_RLO;
    case BIDI_TYPE_LRO:
      return (FriBidiParType)FRIBIDI_TYPE_LRO;
    case BIDI_TYPE_WRTL:
      return (FriBidiParType)FRIBIDI_PAR_WRTL;
    case BIDI_TYPE_WLTR:
      return (FriBidiParType)FRIBIDI_PAR_WLTR;
    default:
      return (FriBidiParType)FRIBIDI_PAR_ON;
  }
}
#endif

#if defined(WITH_BIDI_SHEEN)
static SBLevel bidi_type_to_sb_base_level(bidi_type_t type) {
  switch (type) {
    case BIDI_TYPE_RTL:
    case BIDI_TYPE_RLO:
    case BIDI_TYPE_WRTL:
      return SBLevelDefaultRTL;
    case BIDI_TYPE_LTR:
    case BIDI_TYPE_LRO:
    case BIDI_TYPE_WLTR:
    case BIDI_TYPE_AUTO:
    default:
      return SBLevelDefaultLTR;
  }
}

static bidi_type_t sb_base_level_to_bidi_type(SBLevel base_level) {
  if (base_level == 1) {
    return BIDI_TYPE_RTL;
  }
  if (base_level == 0) {
    return BIDI_TYPE_LTR;
  }
  return BIDI_TYPE_AUTO;
}

static SBStringEncoding awtk_wchar_sb_encoding(void) {
  return (sizeof(wchar_t) == sizeof(uint32_t)) ? SBStringEncodingUTF32 : SBStringEncodingUTF16;
}
#endif

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

#if defined(WITH_BIDI_FRIBIDI)
static ret_t bidi_log2vis_fribidi(bidi_t* bidi, const wchar_t* str, uint32_t size) {
  FriBidiLevel level = 0;
  FriBidiParType type = FRIBIDI_PAR_ON;
  return_value_if_fail(bidi != NULL && str != NULL && size > 0, RET_BAD_PARAMS);
  return_value_if_fail(sizeof(wchar_t) == sizeof(FriBidiChar), RET_BAD_PARAMS);
  return_value_if_fail(sizeof(uint32_t) == sizeof(FriBidiStrIndex), RET_BAD_PARAMS);

  if (ARRAY_SIZE(bidi->vis_str_static) > size) {
    bidi->vis_str_size = size;
    bidi->vis_str = bidi->vis_str_static;
  } else {
    bidi->vis_str_size = size;
    assert(bidi->vis_str == NULL);
    bidi->vis_str = TKMEM_ALLOC((size + 1) * sizeof(wchar_t));
  }
  return_value_if_fail(bidi->vis_str != NULL, RET_FAIL);

  if (bidi->alloc_l2v) {
    bidi->positions_L_to_V = TKMEM_ZALLOCN(int32_t, size);
  }
  if (bidi->alloc_v2l) {
    bidi->positions_V_to_L = TKMEM_ZALLOCN(int32_t, size);
  }

  type = bidi_type_to_fribidi_par_type(bidi->request_type);
  level = fribidi_log2vis((const FriBidiChar*)str, (FriBidiStrIndex)size, &type, (FriBidiChar*)bidi->vis_str,
                           (FriBidiStrIndex*)bidi->positions_L_to_V, (FriBidiStrIndex*)bidi->positions_V_to_L, NULL);
  bidi->resolved_type = bidi_type_from_fribidi_par_type(type);
  (void)level;

  return RET_OK;
}
#endif

#if defined(WITH_BIDI_SHEEN)
static void bidi_clear_log2vis_buffers(bidi_t* bidi) {
  TKMEM_FREE(bidi->positions_L_to_V);
  TKMEM_FREE(bidi->positions_V_to_L);
  bidi->positions_L_to_V = NULL;
  bidi->positions_V_to_L = NULL;
  if (bidi->vis_str != NULL && bidi->vis_str != bidi->vis_str_static) {
    TKMEM_FREE(bidi->vis_str);
  }
  bidi->vis_str = NULL;
  bidi->vis_str_size = 0;
}

static ret_t bidi_log2vis_sheenbidi(bidi_t* bidi, const wchar_t* str, uint32_t size) {
  SBTextConfigRef config = NULL;
  SBTextRef text = NULL;
  SBVisualRunIteratorRef iterator = NULL;
  SBUInteger vis_pos = 0;

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

  if (bidi->alloc_l2v) {
    bidi->positions_L_to_V = TKMEM_ZALLOCN(int32_t, size);
    if (bidi->positions_L_to_V == NULL) {
      bidi_clear_log2vis_buffers(bidi);
      return RET_FAIL;
    }
  }
  if (bidi->alloc_v2l) {
    bidi->positions_V_to_L = TKMEM_ZALLOCN(int32_t, size);
    if (bidi->positions_V_to_L == NULL) {
      bidi_clear_log2vis_buffers(bidi);
      return RET_FAIL;
    }
  }

  config = SBTextConfigCreate();
  if (config == NULL) {
    bidi_clear_log2vis_buffers(bidi);
    return RET_FAIL;
  }
  SBTextConfigSetBaseLevel(config, bidi_type_to_sb_base_level(bidi->request_type));

  text = SBTextCreate((const void*)str, (SBUInteger)size, awtk_wchar_sb_encoding(), config);
  SBTextConfigRelease(config);
  config = NULL;
  if (text == NULL) {
    bidi_clear_log2vis_buffers(bidi);
    return RET_FAIL;
  }

  {
    SBParagraphInfo pinfo;
    memset(&pinfo, 0, sizeof(pinfo));
    SBTextGetCodeUnitParagraphInfo(text, 0, &pinfo);
    bidi->resolved_type = sb_base_level_to_bidi_type(pinfo.baseLevel);
  }

  iterator = SBTextCreateVisualRunIterator(text, 0, (SBUInteger)size);
  if (iterator == NULL) {
    SBTextRelease(text);
    bidi_clear_log2vis_buffers(bidi);
    return RET_FAIL;
  }

  while (SBVisualRunIteratorMoveNext(iterator)) {
    const SBVisualRun* run = SBVisualRunIteratorGetCurrent(iterator);
    SBUInteger start = run->index;
    SBUInteger len = run->length;
    SBBoolean reverse = (SBBoolean)((run->level % 2) == 1);
    SBUInteger k = 0;

    for (k = 0; k < len; k++) {
      SBUInteger logical_index = reverse ? (start + len - 1 - k) : (start + k);
      wchar_t ch = str[logical_index];

      bidi->vis_str[vis_pos] = ch;

      if (bidi->positions_L_to_V != NULL) {
        bidi->positions_L_to_V[logical_index] = (int32_t)vis_pos;
      }
      if (bidi->positions_V_to_L != NULL) {
        bidi->positions_V_to_L[vis_pos] = (int32_t)logical_index;
      }

      vis_pos++;
    }
  }

  SBVisualRunIteratorRelease(iterator);
  SBTextRelease(text);

  if (vis_pos != (SBUInteger)size) {
    bidi_clear_log2vis_buffers(bidi);
    return RET_FAIL;
  }

  bidi->vis_str[size] = 0;

  return RET_OK;
}
#endif

ret_t bidi_log2vis(bidi_t* bidi, const wchar_t* str, uint32_t size) {
#if defined(WITH_BIDI_SHEEN)
  return bidi_log2vis_sheenbidi(bidi, str, size);
#else
  return bidi_log2vis_fribidi(bidi, str, size);
#endif
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
