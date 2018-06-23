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
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/mystdlib.h"
#include "../include/ngram.h"

namespace ime_pinyin {

#define ADD_COUNT 0.3

int comp_double(const void *p1, const void *p2) {
  if (*static_cast<const double*>(p1) < *static_cast<const double*>(p2))
    return -1;
  if (*static_cast<const double*>(p1) > *static_cast<const double*>(p2))
    return 1;
  return 0;
}

inline double distance(double freq, double code) {
  // return fabs(freq - code);
  return freq * fabs(log(freq) - log(code));
}

// Find the index of the code value which is nearest to the given freq
int qsearch_nearest(double code_book[], double freq, int start, int end) {
  if (start == end)
    return start;

  if (start + 1 == end) {
    if (distance(freq, code_book[end]) > distance(freq, code_book[start]))
      return start;
    return end;
  }

  int mid = (start + end) / 2;

  if (code_book[mid] > freq)
    return qsearch_nearest(code_book, freq, start, mid);
  else
    return qsearch_nearest(code_book, freq, mid, end);
}

unsigned update_code_idx(double freqs[], unsigned num, double code_book[],
                       CODEBOOK_TYPE *code_idx) {
  unsigned changed = 0;
  for (unsigned pos = 0; pos < num; pos++) {
    CODEBOOK_TYPE idx;
    idx = qsearch_nearest(code_book, freqs[pos], 0, kCodeBookSize - 1);
    if (idx != code_idx[pos])
      changed++;
    code_idx[pos] = idx;
  }
  return changed;
}

double recalculate_kernel(double freqs[], unsigned num, double code_book[],
                          CODEBOOK_TYPE *code_idx) {
  double ret = 0;

  unsigned *item_num =  new unsigned[kCodeBookSize];
  assert(item_num);
  memset(item_num, 0, sizeof(unsigned) * kCodeBookSize);

  double *cb_new = new double[kCodeBookSize];
  assert(cb_new);
  memset(cb_new, 0, sizeof(double) * kCodeBookSize);

  for (unsigned pos = 0; pos < num; pos++) {
    ret += distance(freqs[pos], code_book[code_idx[pos]]);

    cb_new[code_idx[pos]] += freqs[pos];
    item_num[code_idx[pos]] += 1;
  }

  for (unsigned code = 0; code < kCodeBookSize; code++) {
    assert(item_num[code] > 0);
    code_book[code] = cb_new[code] / item_num[code];
  }

  delete [] item_num;
  delete [] cb_new;

  return ret;
}

void iterate_codes(double freqs[], unsigned num, double code_book[],
                   CODEBOOK_TYPE *code_idx) {
  unsigned iter_num = 0;
  double delta_last = 0;
  do {
    unsigned changed = update_code_idx(freqs, num, code_book, code_idx);

    double delta = recalculate_kernel(freqs, num, code_book, code_idx);

    if (kPrintDebug0) {
      printf("---Unigram codebook iteration: %d : %d, %.9f\n",
             iter_num, changed, delta);
    }
    iter_num++;

    if (iter_num > 1 &&
        (delta == 0 || fabs(delta_last - delta)/fabs(delta) < 0.000000001))
      break;
    delta_last = delta;
  } while (true);
}


NGram* NGram::instance_ = NULL;

NGram::NGram() {
  initialized_ = false;
  idx_num_ = 0;
  lma_freq_idx_ = NULL;
  sys_score_compensation_ = 0;

#ifdef ___BUILD_MODEL___
  freq_codes_df_ = NULL;
#endif
  freq_codes_ = NULL;
}

NGram::~NGram() {
  if (NULL != lma_freq_idx_)
    free(lma_freq_idx_);

#ifdef ___BUILD_MODEL___
  if (NULL != freq_codes_df_)
    free(freq_codes_df_);
#endif

  if (NULL != freq_codes_)
    free(freq_codes_);
}

NGram& NGram::get_instance() {
  if (NULL == instance_)
    instance_ = new NGram();
  return *instance_;
}

bool NGram::save_ngram(FILE *fp) {
  if (!initialized_ || NULL == fp)
    return false;

  if (0 == idx_num_ || NULL == freq_codes_ ||  NULL == lma_freq_idx_)
    return false;

  if (fwrite(&idx_num_, sizeof(unsigned), 1, fp) != 1)
    return false;

  if (fwrite(freq_codes_, sizeof(LmaScoreType), kCodeBookSize, fp) !=
      kCodeBookSize)
    return false;

  if (fwrite(lma_freq_idx_, sizeof(CODEBOOK_TYPE), idx_num_, fp) != idx_num_)
    return false;

  return true;
}

bool NGram::load_ngram(FILE *fp) {
  if (NULL == fp)
    return false;

  initialized_ = false;

  if (fread(&idx_num_, sizeof(unsigned), 1, fp) != 1 )
    return false;

  if (NULL != lma_freq_idx_)
    free(lma_freq_idx_);

  if (NULL != freq_codes_)
    free(freq_codes_);

  lma_freq_idx_ = static_cast<CODEBOOK_TYPE*>
                  (malloc(idx_num_ * sizeof(CODEBOOK_TYPE)));
  freq_codes_ = static_cast<LmaScoreType*>
      (malloc(kCodeBookSize * sizeof(LmaScoreType)));

  if (NULL == lma_freq_idx_ || NULL == freq_codes_)
    return false;

  if (fread(freq_codes_, sizeof(LmaScoreType), kCodeBookSize, fp) !=
      kCodeBookSize)
    return false;

  if (fread(lma_freq_idx_, sizeof(CODEBOOK_TYPE), idx_num_, fp) != idx_num_)
    return false;

  initialized_ = true;

  total_freq_none_sys_ = 0;
  return true;
}

void NGram::set_total_freq_none_sys(unsigned freq_none_sys) {
  total_freq_none_sys_ = freq_none_sys;
  if (0 == total_freq_none_sys_) {
    sys_score_compensation_ = 0;
  } else {
    double factor = static_cast<double>(kSysDictTotalFreq) / (
        kSysDictTotalFreq + total_freq_none_sys_);
    sys_score_compensation_ = static_cast<float>(
        log(factor) * kLogValueAmplifier);
  }
}

// The caller makes sure this oject is initialized.
float NGram::get_uni_psb(LemmaIdType lma_id) {
  return  static_cast<float>(freq_codes_[lma_freq_idx_[lma_id]]) +
      sys_score_compensation_;
}

float NGram::convert_psb_to_score(double psb) {
  float score = static_cast<float>(
      log(psb) * static_cast<double>(kLogValueAmplifier));
  if (score > static_cast<float>(kMaxScore)) {
    score = static_cast<float>(kMaxScore);
  }
  return score;
}

#ifdef ___BUILD_MODEL___
bool NGram::build_unigram(LemmaEntry *lemma_arr, unsigned lemma_num,
                          LemmaIdType next_idx_unused) {
  if (NULL == lemma_arr || 0 == lemma_num || next_idx_unused <= 1)
    return false;

  double total_freq = 0;
  double *freqs = new double[next_idx_unused];
  if (NULL == freqs)
    return false;

  freqs[0] = ADD_COUNT;
  total_freq += freqs[0];
  LemmaIdType idx_now = 0;
  for (unsigned pos = 0; pos < lemma_num; pos++) {
    if (lemma_arr[pos].idx_by_hz == idx_now)
      continue;
    idx_now++;

    assert(lemma_arr[pos].idx_by_hz == idx_now);

    freqs[idx_now] = lemma_arr[pos].freq;
    if (freqs[idx_now] <= 0)
      freqs[idx_now] = 0.3;

    total_freq += freqs[idx_now];
  }

  double max_freq = 0;
  idx_num_ = idx_now + 1;
  assert(idx_now + 1 == next_idx_unused);

  for (unsigned pos = 0; pos < idx_num_; pos++) {
    freqs[pos] = freqs[pos] / total_freq;
    assert(freqs[pos] > 0);
    if (freqs[pos] > max_freq)
      max_freq = freqs[pos];
  }

  // calculate the code book
  if (NULL == freq_codes_df_)
    freq_codes_df_ = new double[kCodeBookSize];
  assert(freq_codes_df_);
  memset(freq_codes_df_, 0, sizeof(double) * kCodeBookSize);

  if (NULL == freq_codes_)
    freq_codes_ = new LmaScoreType[kCodeBookSize];
  assert(freq_codes_);
  memset(freq_codes_, 0, sizeof(LmaScoreType) * kCodeBookSize);

  unsigned freq_pos = 0;
  for (unsigned code_pos = 0; code_pos < kCodeBookSize; code_pos++) {
    bool found = true;

    while (found) {
      found = false;
      double cand = freqs[freq_pos];
      for (unsigned i = 0; i < code_pos; i++)
        if (freq_codes_df_[i] == cand) {
          found = true;
          break;
        }
      if (found)
        freq_pos++;
    }

    freq_codes_df_[code_pos] = freqs[freq_pos];
    freq_pos++;
  }

  myqsort(freq_codes_df_, kCodeBookSize, sizeof(double), comp_double);

  if (NULL == lma_freq_idx_)
    lma_freq_idx_ = new CODEBOOK_TYPE[idx_num_];
  assert(lma_freq_idx_);

  iterate_codes(freqs, idx_num_, freq_codes_df_, lma_freq_idx_);

  delete [] freqs;

  if (kPrintDebug0) {
    printf("\n------Language Model Unigram Codebook------\n");
  }

  for (unsigned code_pos = 0; code_pos < kCodeBookSize; code_pos++) {
    double log_score = log(freq_codes_df_[code_pos]);
    float final_score = convert_psb_to_score(freq_codes_df_[code_pos]);
    if (kPrintDebug0) {
      printf("code:%d, probability:%.9f, log score:%.3f, final score: %.3f\n",
             code_pos, freq_codes_df_[code_pos], log_score, final_score);
    }
    freq_codes_[code_pos] = static_cast<LmaScoreType>(final_score);
  }

  initialized_ = true;
  return true;
}
#endif

}  // namespace ime_pinyin
