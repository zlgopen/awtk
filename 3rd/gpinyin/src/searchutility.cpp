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

#include <assert.h>
#include "../include/mystdlib.h"
#include "../include/searchutility.h"

namespace ime_pinyin {

bool is_system_lemma(LemmaIdType lma_id) {
  return (0 < lma_id && lma_id <= kSysDictIdEnd);
}

bool is_user_lemma(LemmaIdType lma_id) {
  return (kUserDictIdStart <= lma_id && lma_id <= kUserDictIdEnd);
}

bool is_composing_lemma(LemmaIdType lma_id) {
  return (kLemmaIdComposing == lma_id);
}

int cmp_lpi_with_psb(const void *p1, const void *p2) {
  if ((static_cast<const LmaPsbItem*>(p1))->psb >
      (static_cast<const LmaPsbItem*>(p2))->psb)
    return 1;
  if ((static_cast<const LmaPsbItem*>(p1))->psb <
      (static_cast<const LmaPsbItem*>(p2))->psb)
    return -1;
  return 0;
}

int cmp_lpi_with_unified_psb(const void *p1, const void *p2) {
  const LmaPsbItem *item1 = static_cast<const LmaPsbItem*>(p1);
  const LmaPsbItem *item2 = static_cast<const LmaPsbItem*>(p2);

  // The real unified psb is psb1 / lma_len1 and psb2 * lma_len2
  // But we use psb1 * lma_len2 and psb2 * lma_len1 to get better
  // precision.
  unsigned up1 = item1->psb * (item2->lma_len);
  unsigned up2 = item2->psb * (item1->lma_len);
  if (up1 < up2) {
    return -1;
  }
  if (up1 > up2) {
    return 1;
  }
  return 0;
}

int cmp_lpi_with_id(const void *p1, const void *p2) {
  if ((static_cast<const LmaPsbItem*>(p1))->id <
      (static_cast<const LmaPsbItem*>(p2))->id)
    return -1;
  if ((static_cast<const LmaPsbItem*>(p1))->id >
      (static_cast<const LmaPsbItem*>(p2))->id)
    return 1;
  return 0;
}

int cmp_lpi_with_hanzi(const void *p1, const void *p2) {
  if ((static_cast<const LmaPsbItem*>(p1))->hanzi <
      (static_cast<const LmaPsbItem*>(p2))->hanzi)
    return -1;
  if ((static_cast<const LmaPsbItem*>(p1))->hanzi >
      (static_cast<const LmaPsbItem*>(p2))->hanzi)
    return 1;

  return 0;
}

int cmp_lpsi_with_str(const void *p1, const void *p2) {
  return utf16_strcmp((static_cast<const LmaPsbStrItem*>(p1))->str,
                      (static_cast<const LmaPsbStrItem*>(p2))->str);
}


int cmp_hanzis_1(const void *p1, const void *p2) {
  if (*static_cast<const char16*>(p1) <
      *static_cast<const char16*>(p2))
    return -1;

  if (*static_cast<const char16*>(p1) >
      *static_cast<const char16*>(p2))
    return 1;
  return 0;
}

int cmp_hanzis_2(const void *p1, const void *p2) {
  return  utf16_strncmp(static_cast<const char16*>(p1),
                        static_cast<const char16*>(p2), 2);
}

int cmp_hanzis_3(const void *p1, const void *p2) {
  return  utf16_strncmp(static_cast<const char16*>(p1),
                        static_cast<const char16*>(p2), 3);
}

int cmp_hanzis_4(const void *p1, const void *p2) {
  return  utf16_strncmp(static_cast<const char16*>(p1),
                        static_cast<const char16*>(p2), 4);
}

int cmp_hanzis_5(const void *p1, const void *p2) {
  return  utf16_strncmp(static_cast<const char16*>(p1),
                        static_cast<const char16*>(p2), 5);
}

int cmp_hanzis_6(const void *p1, const void *p2) {
  return  utf16_strncmp(static_cast<const char16*>(p1),
                        static_cast<const char16*>(p2), 6);
}

int cmp_hanzis_7(const void *p1, const void *p2) {
  return  utf16_strncmp(static_cast<const char16*>(p1),
                        static_cast<const char16*>(p2), 7);
}

int cmp_hanzis_8(const void *p1, const void *p2) {
  return  utf16_strncmp(static_cast<const char16*>(p1),
                        static_cast<const char16*>(p2), 8);
}

int cmp_npre_by_score(const void *p1, const void *p2) {
  if ((static_cast<const NPredictItem*>(p1))->psb >
      (static_cast<const NPredictItem*>(p2))->psb)
    return 1;

  if ((static_cast<const NPredictItem*>(p1))->psb <
      (static_cast<const NPredictItem*>(p2))->psb)
    return -1;

  return 0;
}

int cmp_npre_by_hislen_score(const void *p1, const void *p2) {
  if ((static_cast<const NPredictItem*>(p1))->his_len <
      (static_cast<const NPredictItem*>(p2))->his_len)
    return 1;

  if ((static_cast<const NPredictItem*>(p1))->his_len >
      (static_cast<const NPredictItem*>(p2))->his_len)
    return -1;

  if ((static_cast<const NPredictItem*>(p1))->psb >
      (static_cast<const NPredictItem*>(p2))->psb)
    return 1;

  if ((static_cast<const NPredictItem*>(p1))->psb <
      (static_cast<const NPredictItem*>(p2))->psb)
    return -1;

  return 0;
}

int cmp_npre_by_hanzi_score(const void *p1, const void *p2) {
  int ret_v = (utf16_strncmp((static_cast<const NPredictItem*>(p1))->pre_hzs,
      (static_cast<const NPredictItem*>(p2))->pre_hzs, kMaxPredictSize));
  if (0 != ret_v)
    return ret_v;

  if ((static_cast<const NPredictItem*>(p1))->psb >
      (static_cast<const NPredictItem*>(p2))->psb)
    return 1;

  if ((static_cast<const NPredictItem*>(p1))->psb <
      (static_cast<const NPredictItem*>(p2))->psb)
    return -1;

  return 0;
}

unsigned remove_duplicate_npre(NPredictItem *npre_items, unsigned npre_num) {
  if (NULL == npre_items || 0 == npre_num)
    return 0;

  myqsort(npre_items, npre_num, sizeof(NPredictItem), cmp_npre_by_hanzi_score);

  unsigned remain_num = 1;  // The first one is reserved.
  for (unsigned pos = 1; pos < npre_num; pos++) {
    if (utf16_strncmp(npre_items[pos].pre_hzs,
                      npre_items[remain_num - 1].pre_hzs,
                      kMaxPredictSize) != 0) {
      if (remain_num != pos) {
        npre_items[remain_num] = npre_items[pos];
      }
      remain_num++;
    }
  }
  return remain_num;
}

unsigned align_to_unsigned(unsigned size) {
  unsigned s = sizeof(unsigned);
  return (size + s -1) / s * s;
}

}  // namespace ime_pinyin
