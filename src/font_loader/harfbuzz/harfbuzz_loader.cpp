/**
 * File:   harfbuzz_loader.cpp
 * Author: AWTK Develop Team
 * Brief:  harfbuzz loader
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

#include "awtk.h"
#include <string>
#include "tkc/types_def.h"
#include "harfbuzz_loader.h"

#ifdef WITH_HARFBUZZ_TEXT_SHAPING
#include "harfbuzz/hb.h"

#ifdef WITH_BIDI_SHEEN
#include "SheenBidi/SheenBidi.h"
#else
#include "fribidi/fribidi.h"
#endif /*WITH_BIDI_SHEEN*/

#define TEXT_LEVEL_IS_RTL(level) ((level)&1)

#ifdef WITH_BIDI_SHEEN
typedef SBLevel _raqm_bidi_level_t;
#else
typedef FriBidiLevel _raqm_bidi_level_t;
#endif /*WITH_BIDI_SHEEN*/

typedef struct {
  size_t pos;
  size_t len;
  _raqm_bidi_level_t level;
} raqm_bidi_run_t;

typedef enum {
  RAQM_BIDI_LEVEL_DEFAULT = 0,
  RAQM_BIDI_LEVEL_RTL = 1,
  RAQM_BIDI_LEVEL_LTR = 2,
} raqm_direction_t;

#ifdef WITH_BIDI_SHEEN
static raqm_bidi_run_t* _raqm_bidi_itemize(const wchar_t* text, uint32_t text_len,
                                           size_t* run_count) {
  int32_t cur_start = 0;
  size_t tmp_run_count = 0;
  SBLineRef line = NULL;
  SBUInteger par_len = NULL;
  SBParagraphRef par = NULL;
  SBAlgorithmRef bidi = NULL;
  raqm_bidi_run_t* runs = NULL;
  raqm_bidi_run_t* tmp_runs = NULL;
  SBLevel base_level = 0; /* force LTR base direction to match VSCode/Notepad */
  SBCodepointSequence input = {SBStringEncodingUTF32, (void*)text, text_len};
  if (sizeof(wchar_t) == 2) {
    input.stringEncoding = SBStringEncodingUTF16;
  }

  /* paragraph */
  while (cur_start < text_len) {
    input.stringBuffer = &text[cur_start];
    input.stringLength = text_len - cur_start;
    bidi = SBAlgorithmCreate(&input);
    par = SBAlgorithmCreateParagraph(bidi, 0, INT32_MAX, base_level);
    par_len = SBParagraphGetLength(par);

    /* lines */
    line = SBParagraphCreateLine(par, 0, par_len);
    tmp_run_count += SBLineGetRunCount(line);

    tmp_runs = (raqm_bidi_run_t*)TKMEM_REALLOC(tmp_runs, sizeof(raqm_bidi_run_t) * (tmp_run_count));

    if (tmp_runs) {
      const SBRun* sheenbidi_runs = SBLineGetRunsPtr(line);

      for (size_t i = 0; i < tmp_run_count - *run_count; ++i) {
        tmp_runs[i + *run_count].pos = sheenbidi_runs[i].offset + cur_start;
        tmp_runs[i + *run_count].len = sheenbidi_runs[i].length;
        tmp_runs[i + *run_count].level = sheenbidi_runs[i].level;
      }
    }
    cur_start += par_len;
    *run_count = tmp_run_count;

    SBLineRelease(line);
    SBParagraphRelease(par);
    SBAlgorithmRelease(bidi);
  }
  runs = (raqm_bidi_run_t*)TKMEM_CALLOC(1, sizeof(raqm_bidi_run_t) * (*run_count));
  memcpy(runs, tmp_runs, sizeof(raqm_bidi_run_t) * (*run_count));

  TKMEM_FREE(tmp_runs);

  return runs;
}

#else

static void _raqm_reverse_run(raqm_bidi_run_t* runs, size_t len) {
  size_t i = 0;
  for (i = 0; i < len / 2; i++) {
    raqm_bidi_run_t temp = runs[i];
    runs[i] = runs[len - 1 - i];
    runs[len - 1 - i] = temp;
  }
}

/**
 * 根据双向文本的字符类型和嵌套等级，划分成多个双向文本run（逻辑顺序）
 * "嵌套等级"指的是 "双向嵌入层级（Bidirectional Embedding Level）"，
 * 用于描述在混合方向文本（比如同时包含LTR和RTL文本）中字符显示的顺序和方向，是Bidi双向算法里面的概念
 */
static raqm_bidi_run_t* _raqm_split_runs(const FriBidiCharType* types, const size_t len,
                                         const FriBidiParType base_dir, FriBidiLevel* levels,
                                         size_t* run_count) {
  FriBidiLevel last_level = -1;
  size_t run_start = 0;
  size_t run_index = 0;
  raqm_bidi_run_t* runs = NULL;
  size_t count = 0;

  if (len == 0) {
    *run_count = 0;
    return NULL;
  }

  assert(types);
  assert(levels);

  for (int i = len - 1; i >= 0 && FRIBIDI_IS_EXPLICIT_OR_BN_OR_WS(types[i]); i--) {
    levels[i] = FRIBIDI_DIR_TO_LEVEL(base_dir);
  }

  for (size_t i = 0; i < len; i++) {
    if (levels[i] != last_level) count++;

    last_level = levels[i];
  }

  runs = (raqm_bidi_run_t*)TKMEM_CALLOC(1, sizeof(raqm_bidi_run_t) * count);

  while (run_start < len) {
    size_t run_end = run_start;
    while (run_end < len && levels[run_start] == levels[run_end]) {
      run_end++;
    }

    runs[run_index].pos = run_start;
    runs[run_index].level = levels[run_start];
    runs[run_index].len = run_end - run_start;
    run_start = run_end;
    run_index++;
  }

  *run_count = count;
  return runs;
}

/**
 * 根据双向文本的字符类型和嵌套等级，对文本进行重新排序并划分成多个双向文本
 * “嵌套等级”指的是 “双向嵌入层级（Bidirectional Embedding Level）”，
 * 用于描述在混合方向文本（比如同时包含LTR和RTL文本）中字符显示的顺序和方向，是Bidi双向算法里面的概念
 */
static raqm_bidi_run_t* _raqm_reorder_runs(const FriBidiCharType* types, const size_t len,
                                           const FriBidiParType base_dir,
                                           /* input and output */
                                           FriBidiLevel* levels,
                                           /* output */
                                           size_t* run_count) {
  FriBidiLevel level;
  FriBidiLevel last_level = -1;
  FriBidiLevel max_level = 0;
  size_t run_start = 0;
  size_t run_index = 0;
  raqm_bidi_run_t* runs = NULL;
  size_t count = 0;

  if (len == 0) {
    *run_count = 0;
    return NULL;
  }

  assert(types);
  assert(levels);

  // 从文本末尾开始，将连续的空白字符或特定类型字符嵌套级别重置为段落基本方向级别
  for (int i = len - 1; i >= 0 && FRIBIDI_IS_EXPLICIT_OR_BN_OR_WS(types[i]); i--) {
    levels[i] = FRIBIDI_DIR_TO_LEVEL(base_dir);
  }

  // 遍历文本嵌套等级列表，找到最大等级
  for (int i = len - 1; i >= 0; i--) {
    if (levels[i] > max_level) max_level = levels[i];
  }

  // 遍历文本嵌套等级列表，记录等级发生变化的次数，从而确定子串个数
  for (size_t i = 0; i < len; i++) {
    if (levels[i] != last_level) count++;

    last_level = levels[i];
  }

  // 根据上面得到的嵌套等级变化次数，创建对应个数的raqm_bidi_run_t对象，用来记录每个子串的信息
  runs = (raqm_bidi_run_t*)TKMEM_CALLOC(1, sizeof(raqm_bidi_run_t) * count);

  while (run_start < len) {
    size_t run_end = run_start;
    while (run_end < len && levels[run_start] == levels[run_end]) {
      run_end++;
    }

    // 初始化每个runs对象的信息
    runs[run_index].pos = run_start;
    runs[run_index].level = levels[run_start];
    runs[run_index].len = run_end - run_start;
    run_start = run_end;
    run_index++;
  }

  /* 遍历runs对象，从最大级别开始，对嵌套等级大于当前等级的子串文本进行反转 */
  for (level = max_level; level > 0; level--) {
    for (int i = count - 1; i >= 0; i--) {
      if (runs[i].level >= level) {
        int end = i;
        for (i--; (i >= 0 && runs[i].level >= level);) {
          i--;
        }
        _raqm_reverse_run(runs + i + 1, end - i);
      }
    }
  }

  *run_count = count;
  return runs;
}

static raqm_bidi_run_t* _raqm_bidi_itemize(const wchar_t* text, uint32_t text_len,
                                           size_t* run_count) {
  raqm_bidi_run_t* runs = NULL;
  FriBidiLevel* levels = NULL;
  FriBidiCharType* char_type = NULL;
  FriBidiBracketType* bracket_type = NULL;
  FriBidiParType pbase_dir = FRIBIDI_PAR_LTR; /* force LTR base direction to match VSCode/Notepad */

  // 初始化bidi相关信息
  levels = (FriBidiLevel*)TKMEM_CALLOC(text_len, sizeof(FriBidiLevel));
  char_type = (FriBidiCharType*)TKMEM_CALLOC(text_len, sizeof(FriBidiCharType));
  bracket_type = (FriBidiBracketType*)TKMEM_CALLOC(text_len, sizeof(FriBidiBracketType));
  return_value_if_fail(levels && char_type && bracket_type, NULL);
  fribidi_get_bidi_types(text, text_len, char_type);
  fribidi_get_bracket_types(text, text_len, char_type, bracket_type);
  fribidi_get_par_embedding_levels_ex(char_type, bracket_type, text_len, &pbase_dir, levels);

  /* 根据字符类型与嵌套级别，将文本划分为多个子串，并根据双向规则重新排序 */
  runs = _raqm_reorder_runs(char_type, text_len, pbase_dir, levels, run_count);

  TKMEM_FREE(levels);
  TKMEM_FREE(char_type);
  TKMEM_FREE(bracket_type);
  levels = NULL;
  char_type = NULL;
  bracket_type = NULL;

  return runs;
}
#endif /*WITH_BIDI_SHEEN*/

/* 按 Unicode 脚本(script)进一步细分 BIDI run。
 * _raqm_bidi_itemize 只按 BIDI 嵌套等级切分 run,
 * 同为LTR(level 0)时会被并入同一 run;
 * 导致夹在中间的复杂脚本不被整形(合字丢失)。此处按脚本变化再切分,
 * 使每个 run 为单一脚本,HarfBuzz 才能对各脚本分别应用 GSUB/GPOS。
 *
 * WITH_BIDI_SHEEN:用 SheenBidi 自带的 SBScriptLocator 做脚本分段(优化
 * 查表,直接产出 run 边界),再与 BIDI run 单遍求交(O(runs)),不再逐字符。
 * WITH_BIDI_FRIBIDI:用 hb_unicode_script 逐字符切分。 */
static raqm_bidi_run_t* _raqm_split_runs_by_script(const wchar_t* text, uint32_t text_len,
                                                   raqm_bidi_run_t* runs, size_t* run_count) {
  size_t i = 0;
  size_t out_count = 0;
  raqm_bidi_run_t* out = NULL;
  size_t cap = (text_len > 0) ? text_len : 1;
  if (runs == NULL || run_count == NULL || *run_count == 0 || text_len == 0) {
    return runs;
  }
  out = (raqm_bidi_run_t*)TKMEM_CALLOC(1, sizeof(raqm_bidi_run_t) * cap);
  if (out == NULL) {
    return runs; /* 分配失败,退化为不按脚本切分(旧行为) */
  }

#ifdef WITH_BIDI_SHEEN
  /* 用 SBScriptLocator 迭代脚本 run,与 BIDI run 单遍求交切分。 */
  {
    SBScriptLocatorRef locator = SBScriptLocatorCreate();
    SBCodepointSequence seq;
    const SBScriptAgent* agent = NULL;
    int has_script = 0;
    SBUInteger s_end = 0;
    if (locator == NULL) {
      TKMEM_FREE(out);
      return runs;
    }
    seq.stringEncoding = (sizeof(wchar_t) == 2) ? SBStringEncodingUTF16 : SBStringEncodingUTF32;
    seq.stringBuffer = (void*)text;
    seq.stringLength = text_len;
    SBScriptLocatorLoadCodepoints(locator, &seq);

    has_script = SBScriptLocatorMoveNext(locator) ? 1 : 0;
    if (has_script) {
      agent = SBScriptLocatorGetAgent(locator);
      s_end = agent->offset + agent->length;
    }

    for (i = 0; i < *run_count; i++) {
      size_t b_pos = runs[i].pos;
      size_t b_end = b_pos + runs[i].len;
      while (b_pos < b_end) {
        /* 推进已耗尽的 script run */
        while (has_script && s_end <= b_pos) {
          has_script = SBScriptLocatorMoveNext(locator) ? 1 : 0;
          if (has_script) {
            agent = SBScriptLocatorGetAgent(locator);
            s_end = agent->offset + agent->length;
          } else {
            break;
          }
        }
        /* 段末取 bidi run 与 script run 的较小者;无 script 信息时取 bidi run 末 */
        size_t seg_end = (has_script && s_end < b_end) ? s_end : b_end;
        if (out_count < cap) {
          out[out_count].pos = b_pos;
          out[out_count].len = seg_end - b_pos;
          out[out_count].level = runs[i].level;
          out_count++;
        }
        b_pos = seg_end;
      }
    }
    SBScriptLocatorRelease(locator);
  }
#else
  /* FriBidi 无脚本分段 API,用 hb_unicode_script 逐字符切分。 */
  {
    hb_unicode_funcs_t* uf = hb_unicode_funcs_get_default();
    if (uf == NULL) {
      TKMEM_FREE(out);
      return runs;
    }
    for (i = 0; i < *run_count; i++) {
      size_t p = runs[i].pos;
      size_t end = p + runs[i].len;
      hb_script_t cur_script = HB_SCRIPT_UNKNOWN;
      size_t seg_start = p;
      for (; p < end; p++) {
        hb_script_t sc = hb_unicode_script(uf, (hb_codepoint_t)text[p]);
        if (sc == HB_SCRIPT_COMMON || sc == HB_SCRIPT_INHERITED || sc == HB_SCRIPT_UNKNOWN) {
          continue;
        }
        if (cur_script == HB_SCRIPT_UNKNOWN) {
          cur_script = sc;
        } else if (sc != cur_script) {
          if (out_count < cap) {
            out[out_count].pos = seg_start;
            out[out_count].len = p - seg_start;
            out[out_count].level = runs[i].level;
            out_count++;
          }
          seg_start = p;
          cur_script = sc;
        }
      }
      if (out_count < cap) {
        out[out_count].pos = seg_start;
        out[out_count].len = end - seg_start;
        out[out_count].level = runs[i].level;
        out_count++;
      }
    }
  }
#endif

  TKMEM_FREE(runs);
  *run_count = out_count;
  return out;
}

static hb_info_t* harfbuzz_do_shape(const wchar_t* text, uint32_t text_len, const char* font_name,
                                    uint32_t* glyph_count, hb_font_t* font) {
  int i = 0;
  uint32_t r = 0;
  uint32_t m_pos = 0;
  size_t run_count = 0;
  hb_info_t* hb_info = NULL;
  raqm_bidi_run_t* runs = NULL;
  uint32_t cur_glyph_size = 0;
  uint32_t total_glyph_size = 0;
  hb_feature_t features[3];
  hb_codepoint_t glyphid = 0;
  hb_codepoint_t cluster = 0;
  hb_position_t x_offset = 0;
  hb_position_t y_offset = 0;
  hb_position_t x_advance = 0;
  hb_position_t y_advance = 0;
  hb_buffer_t* buffer = NULL;
  hb_buffer_t** buffer_arr = NULL;
  raqm_bidi_run_t* cur_run = NULL;
  hb_glyph_info_t* glyph_info = NULL;
  hb_glyph_position_t* glyph_pos = NULL;
  assets_manager_t* am = assets_manager();
  font_bidi_type_t* bidi_type_arr = NULL;
  return_value_if_fail(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4, NULL);
  return_value_if_fail(font != NULL, NULL);
  if (text == NULL || text_len <= 0) {
    return NULL;
  }

  // 对文本进行重新排序，得到每个子串的信息
  runs = _raqm_bidi_itemize(text, text_len, &run_count);
  runs = _raqm_split_runs_by_script(text, text_len, runs, &run_count);

  goto_error_if_fail(runs != NULL && run_count > 0);
  if (runs != NULL && run_count > 0) {
    buffer_arr = (hb_buffer_t**)TKMEM_CALLOC(1, sizeof(hb_buffer_t*) * run_count);
    bidi_type_arr = (font_bidi_type_t*)TKMEM_CALLOC(1, sizeof(font_bidi_type_t) * run_count);
    goto_error_if_fail(buffer_arr != NULL || bidi_type_arr != NULL);
  }
  hb_feature_from_string("liga=0", -1, &features[0]);
  hb_feature_from_string("clig=0", -1, &features[1]);
  hb_feature_from_string("calt=0", -1, &features[2]);

  // 获取每个小buffer的字模信息
  for (r = 0; r < run_count; ++r) {
    cur_run = &runs[r];
    buffer_arr[r] = hb_buffer_create();
    buffer = buffer_arr[r];

    if (sizeof(wchar_t) == 2) {
      hb_buffer_add_utf16(buffer, (const uint16_t*)(text), text_len, cur_run->pos, cur_run->len);
    } else if (sizeof(wchar_t) == 4) {
      hb_buffer_add_utf32(buffer, (const uint32_t*)(text), text_len, cur_run->pos, cur_run->len);
    }

    hb_buffer_guess_segment_properties(buffer);
    if (TEXT_LEVEL_IS_RTL(cur_run->level)) {
      bidi_type_arr[r] = FONT_BIDI_TYPE_RTL;
      hb_buffer_set_direction(buffer, HB_DIRECTION_RTL);
    } else {
      bidi_type_arr[r] = FONT_BIDI_TYPE_LTR;
      hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
    }

    hb_shape(font, buffer, features, 3);
    hb_buffer_get_glyph_infos(buffer, &cur_glyph_size);
    total_glyph_size += cur_glyph_size;
  }

  if (glyph_count != NULL) {
    *glyph_count = total_glyph_size;
  }

  // 合并每个小buffer字模信息，返回hb_info对象
  if (hb_info == NULL && total_glyph_size > 0) {
    hb_info = (hb_info_t*)TKMEM_CALLOC(1, sizeof(hb_info_t) * total_glyph_size);
    goto_error_if_fail(hb_info != NULL);
  }

  m_pos = 0;
  for (r = 0; r < run_count; ++r) {
    cur_run = &runs[r];
    buffer = buffer_arr[r];
    glyph_info = hb_buffer_get_glyph_infos(buffer, &cur_glyph_size);
    glyph_pos = hb_buffer_get_glyph_positions(buffer, &cur_glyph_size);

    for (i = 0; i < cur_glyph_size; ++i) {
      glyphid = glyph_info[i].codepoint;
      cluster = glyph_info[i].cluster;
      x_offset = glyph_pos[i].x_offset;
      y_offset = glyph_pos[i].y_offset;
      x_advance = glyph_pos[i].x_advance;
      y_advance = glyph_pos[i].y_advance;
      if (hb_info && (i + m_pos) < total_glyph_size) {
        (hb_info)[i + m_pos].glyphid = glyphid;
        (hb_info)[i + m_pos].cluster = cluster;
        (hb_info)[i + m_pos].x_offset = x_offset;
        (hb_info)[i + m_pos].y_offset = y_offset;
        (hb_info)[i + m_pos].x_advance = x_advance;
        (hb_info)[i + m_pos].y_advance = y_advance;
        (hb_info)[i + m_pos].bidi_type = bidi_type_arr[r];
      }
    }
    m_pos += cur_glyph_size;
  }

  if (buffer_arr) {
    for (r = 0; r < run_count; ++r) {
      if (buffer_arr[r] != NULL) {
        hb_buffer_destroy(buffer_arr[r]);
      }
    }
    TKMEM_FREE(buffer_arr);
    buffer_arr = NULL;
  }

  if (runs) {
    TKMEM_FREE(runs);
    runs = NULL;
  }

  if (bidi_type_arr) {
    TKMEM_FREE(bidi_type_arr);
  }

  return hb_info;

error:
  if (buffer_arr) {
    for (r = 0; r < run_count; ++r) {
      if (buffer_arr[r] != NULL) {
        hb_buffer_destroy(buffer_arr[r]);
      }
    }
    TKMEM_FREE(buffer_arr);
    buffer_arr = NULL;
  }

  if (runs) {
    TKMEM_FREE(runs);
    runs = NULL;
  }

  if (bidi_type_arr) {
    TKMEM_FREE(bidi_type_arr);
  }

  return NULL;
}

BEGIN_C_DECLS

hb_info_t* harfbuzz_shape_text(const wchar_t* text, uint32_t text_len, const char* font_name,
                               uint32_t* glyph_count, void* font) {
  return_value_if_fail(text != NULL && text_len > 0 && font_name != NULL, NULL);
  return harfbuzz_do_shape(text, text_len, font_name, glyph_count, (hb_font_t*)font);
}

ret_t harfbuzz_shape_text_destroy(hb_info_t* hb_info) {
  return_value_if_fail(hb_info != NULL, RET_OK);
  TKMEM_FREE(hb_info);
  return RET_OK;
}

END_C_DECLS

#else

BEGIN_C_DECLS

hb_info_t* harfbuzz_shape_text(const wchar_t* text, uint32_t text_len, const char* font_name,
                               uint32_t* glyph_count, void* font) {
  return NULL;
}

ret_t harfbuzz_shape_text_destroy(hb_info_t* hb_info) {
  return RET_NOT_IMPL;
}

END_C_DECLS

#endif /* WITH_HARFBUZZ_TEXT_SHAPING */
