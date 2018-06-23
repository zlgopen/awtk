/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PINYINIME_ANDPY_INCLUDE_SEARCHCOMMON_H__
#define PINYINIME_ANDPY_INCLUDE_SEARCHCOMMON_H__

#include <stdlib.h>
#include "./spellingtrie.h"

namespace ime_pinyin {

// Type used to identify the size of a pool, such as id pool, etc.
typedef uint16 PoolPosType;

// Type used to identify a parsing mile stone in an atom dictionary.
typedef uint16 MileStoneHandle;

// Type used to express a lemma and its probability score.
typedef struct {
  unsigned id:(kLemmaIdSize * 8);
  unsigned lma_len:4;
  uint16 psb;  // The score, the lower psb, the higher possibility.
  // For single character items, we may also need Hanzi.
  // For multiple characer items, ignore it.
  char16 hanzi;
} LmaPsbItem, *PLmaPsbItem;

// LmaPsbItem extended with string.
typedef struct {
  LmaPsbItem lpi;
  char16 str[kMaxLemmaSize + 1];
} LmaPsbStrItem, *PLmaPsbStrItem;


typedef struct {
  float psb;
  char16 pre_hzs[kMaxPredictSize];
  uint16 his_len;  // The length of the history used to do the prediction.
} NPredictItem, *PNPredictItem;

// Parameter structure used to extend in a dictionary. All dictionaries
// receives the same DictExtPara and a dictionary specific MileStoneHandle for
// extending.
//
// When the user inputs a new character, AtomDictBase::extend_dict() will be
// called at least once for each dictionary.
//
// For example, when the user inputs "wm", extend_dict() will be called twice,
// and the DictExtPara parameter are as follows respectively:
// 1. splids = {w, m}; splids_extended = 1; ext_len = 1; step_no = 1;
// splid_end_split = false; id_start = wa(the first id start with 'w');
// id_num = number of ids starting with 'w'.
// 2. splids = {m}; splids_extended = 0; ext_len = 1; step_no = 1;
// splid_end_split = false; id_start = wa; id_num = number of ids starting with
// 'w'.
//
// For string "women", one of the cases of the DictExtPara parameter is:
// splids = {wo, men}, splids_extended = 1, ext_len = 3 (length of "men"),
// step_no = 4; splid_end_split = false; id_start = men, id_num = 1.
//
typedef struct {
  // Spelling ids for extending, there are splids_extended + 1 ids in the
  // buffer.
  // For a normal lemma, there can only be kMaxLemmaSize spelling ids in max,
  // but for a composing phrase, there can kMaxSearchSteps spelling ids.
  uint16 splids[kMaxSearchSteps];

  // Number of ids that have been used before. splids[splids_extended] is the
  // newly added id for the current extension.
  uint16 splids_extended;

  // The step span of the extension. It is also the size of the string for
  // the newly added spelling id.
  uint16 ext_len;

  // The step number for the current extension. It is also the ending position
  // in the input Pinyin string for the substring of spelling ids in splids[].
  // For example, when the user inputs "women", step_no = 4.
  // This parameter may useful to manage the MileStoneHandle list for each
  // step. When the user deletes a character from the string, MileStoneHandle
  // objects for the the steps after that character should be reset; when the
  // user begins a new string, all MileStoneHandle objects should be reset.
  uint16 step_no;

  // Indicate whether the newly added spelling ends with a splitting character
  bool splid_end_split;

  // If the newly added id is a half id, id_start is the first id of the
  // corresponding full ids; if the newly added id is a full id, id_start is
  // that id.
  uint16 id_start;

  // If the newly added id is a half id, id_num is the number of corresponding
  // ids; if it is a full id, id_num == 1.
  uint16 id_num;
}DictExtPara, *PDictExtPara;

bool is_system_lemma(LemmaIdType lma_id);
bool is_user_lemma(LemmaIdType lma_id);
bool is_composing_lemma(LemmaIdType lma_id);

int cmp_lpi_with_psb(const void *p1, const void *p2);
int cmp_lpi_with_unified_psb(const void *p1, const void *p2);
int cmp_lpi_with_id(const void *p1, const void *p2);
int cmp_lpi_with_hanzi(const void *p1, const void *p2);

int cmp_lpsi_with_str(const void *p1, const void *p2);

int cmp_hanzis_1(const void *p1, const void *p2);
int cmp_hanzis_2(const void *p1, const void *p2);
int cmp_hanzis_3(const void *p1, const void *p2);
int cmp_hanzis_4(const void *p1, const void *p2);
int cmp_hanzis_5(const void *p1, const void *p2);
int cmp_hanzis_6(const void *p1, const void *p2);
int cmp_hanzis_7(const void *p1, const void *p2);
int cmp_hanzis_8(const void *p1, const void *p2);

int cmp_npre_by_score(const void *p1, const void *p2);
int cmp_npre_by_hislen_score(const void *p1, const void *p2);
int cmp_npre_by_hanzi_score(const void *p1, const void *p2);


unsigned remove_duplicate_npre(NPredictItem *npre_items, unsigned npre_num);

unsigned align_to_unsigned(unsigned size);

}  // namespace

#endif  // PINYINIME_ANDPY_INCLUDE_SEARCHCOMMON_H__
