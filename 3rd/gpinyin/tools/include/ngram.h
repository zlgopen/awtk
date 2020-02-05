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

#ifndef PINYINIME_INCLUDE_NGRAM_H__
#define PINYINIME_INCLUDE_NGRAM_H__

#include <stdio.h>
#include <stdlib.h>
#include "./dictdef.h"

namespace ime_pinyin {

typedef unsigned char CODEBOOK_TYPE;

static const unsigned kCodeBookSize = 256;

class NGram {
 public:
  // The maximum score of a lemma item.
  static const LmaScoreType kMaxScore = 0x3fff;

  // In order to reduce the storage size, the original log value is amplified by
  // kScoreAmplifier, and we use LmaScoreType to store.
  // After this process, an item with a lower score has a higher frequency.
  static const int kLogValueAmplifier = -800;

  // System words' total frequency. It is not the real total frequency, instead,
  // It is only used to adjust system lemmas' scores when the user dictionary's
  // total frequency changes.
  // In this version, frequencies of system lemmas are fixed. We are considering
  // to make them changable in next version.
  static const unsigned kSysDictTotalFreq = 100000000;

 private:

  static NGram* instance_;

  bool initialized_;
  unsigned idx_num_;

  unsigned total_freq_none_sys_;

  // Score compensation for system dictionary lemmas.
  // Because after user adds some user lemmas, the total frequency changes, and
  // we use this value to normalize the score.
  float sys_score_compensation_;

#ifdef ___BUILD_MODEL___
  double *freq_codes_df_;
#endif
  LmaScoreType *freq_codes_;
  CODEBOOK_TYPE *lma_freq_idx_;

 public:
  NGram();
  ~NGram();

  static NGram& get_instance();

  bool save_ngram(FILE *fp);
  bool load_ngram(FILE *fp);

  // Set the total frequency of all none system dictionaries.
  void set_total_freq_none_sys(unsigned freq_none_sys);

  float get_uni_psb(LemmaIdType lma_id);

  // Convert a probability to score. Actually, the score will be limited to
  // kMaxScore, but at runtime, we also need float expression to get accurate
  // value of the score.
  // After the conversion, a lower score indicates a higher probability of the
  // item.
  static float convert_psb_to_score(double psb);

#ifdef ___BUILD_MODEL___
  // For constructing the unigram mode model.
  bool build_unigram(LemmaEntry *lemma_arr, unsigned num,
                     LemmaIdType next_idx_unused);
#endif
};
}

#endif  // PINYINIME_INCLUDE_NGRAM_H__
