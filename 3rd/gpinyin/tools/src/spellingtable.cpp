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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/spellingtable.h"

namespace ime_pinyin {

#ifdef ___BUILD_MODEL___

const char SpellingTable::
    kNotSupportList[kNotSupportNum][kMaxSpellingSize + 1] = {"HM", "HNG", "NG"};

// "" is the biggest, so that all empty strings will be moved to the end
// _eb mean empty is biggest
int compare_raw_spl_eb(const void* p1, const void* p2) {
  if ('\0' == (static_cast<const RawSpelling*>(p1))->str[0])
    return 1;

  if ('\0' == (static_cast<const RawSpelling*>(p2))->str[0])
    return -1;

  return strcmp((static_cast<const RawSpelling*>(p1))->str,
                (static_cast<const RawSpelling*>(p2))->str);
}

unsigned get_odd_next(unsigned value) {
  unsigned v_next = value;
  while (true) {
    unsigned v_next_sqrt = (unsigned)sqrt(v_next);

    bool is_odd = true;
    for (unsigned v_dv = 2; v_dv < v_next_sqrt + 1; v_dv++) {
      if (v_next % v_dv == 0) {
        is_odd = false;
        break;
      }
    }

    if (is_odd)
      return v_next;

    v_next++;
  }

  // never reach here
  return 0;
}

SpellingTable::SpellingTable() {
  need_score_ = false;
  raw_spellings_ = NULL;
  spelling_buf_ = NULL;
  spelling_num_ = 0;
  total_freq_ = 0;
  frozen_ = true;
}

SpellingTable::~SpellingTable() {
  free_resource();
}

unsigned SpellingTable::get_hash_pos(const char* spelling_str) {
  unsigned hash_pos = 0;
  for (unsigned pos = 0; pos < spelling_size_; pos++) {
    if ('\0' == spelling_str[pos])
      break;
    hash_pos += (unsigned)spelling_str[pos];
  }

  hash_pos = hash_pos % spelling_max_num_;
  return hash_pos;
}

unsigned SpellingTable::hash_pos_next(unsigned hash_pos) {
  hash_pos += 123;
  hash_pos = hash_pos % spelling_max_num_;
  return hash_pos;
}

void SpellingTable::free_resource() {
  if (NULL != raw_spellings_)
    delete [] raw_spellings_;
  raw_spellings_ = NULL;

  if (NULL != spelling_buf_)
    delete [] spelling_buf_;
  spelling_buf_ = NULL;
}

bool SpellingTable::init_table(unsigned pure_spl_size, unsigned spl_max_num,
                               bool need_score) {
  if (pure_spl_size == 0 || spl_max_num ==0)
    return false;

  need_score_ = need_score;

  free_resource();

  spelling_size_ = pure_spl_size + 1;
  if (need_score)
    spelling_size_ += 1;
  spelling_max_num_ = get_odd_next(spl_max_num);
  spelling_num_ = 0;

  raw_spellings_ = new RawSpelling[spelling_max_num_];
  spelling_buf_ = new char[spelling_max_num_ * (spelling_size_)];
  if (NULL == raw_spellings_ || NULL == spelling_buf_) {
    free_resource();
    return false;
  }

  memset(raw_spellings_, 0, spelling_max_num_ * sizeof(RawSpelling));
  memset(spelling_buf_, 0, spelling_max_num_ * (spelling_size_));
  frozen_ = false;
  total_freq_ = 0;
  return true;
}

bool SpellingTable::put_spelling(const char* spelling_str, double freq) {
  if (frozen_ || NULL == spelling_str)
    return false;

  for (unsigned pos = 0; pos < kNotSupportNum; pos++) {
    if (strcmp(spelling_str, kNotSupportList[pos]) == 0) {
      return false;
    }
  }

  total_freq_ += freq;

  unsigned hash_pos = get_hash_pos(spelling_str);

  raw_spellings_[hash_pos].str[spelling_size_ - 1] = '\0';

  if (strncmp(raw_spellings_[hash_pos].str, spelling_str,
              spelling_size_ - 1) == 0) {
    raw_spellings_[hash_pos].freq += freq;
    return true;
  }

  unsigned hash_pos_ori = hash_pos;

  while (true) {
    if (strncmp(raw_spellings_[hash_pos].str,
                spelling_str, spelling_size_ - 1) == 0) {
      raw_spellings_[hash_pos].freq += freq;
      return true;
    }

    if ('\0' == raw_spellings_[hash_pos].str[0]) {
      raw_spellings_[hash_pos].freq += freq;
      strncpy(raw_spellings_[hash_pos].str, spelling_str, spelling_size_ - 1);
      raw_spellings_[hash_pos].str[spelling_size_ - 1] = '\0';
      spelling_num_++;
      return true;
    }

    hash_pos = hash_pos_next(hash_pos);
    if (hash_pos_ori == hash_pos)
      return false;
  }

  // never reach here
  return false;
}

bool SpellingTable::contain(const char* spelling_str) {
  if (NULL == spelling_str || NULL == spelling_buf_ || frozen_)
    return false;

  unsigned hash_pos = get_hash_pos(spelling_str);

  if ('\0' == raw_spellings_[hash_pos].str[0])
    return false;

  if (strncmp(raw_spellings_[hash_pos].str, spelling_str, spelling_size_ - 1)
      == 0)
    return true;

  unsigned hash_pos_ori = hash_pos;

  while (true) {
    hash_pos = hash_pos_next(hash_pos);
    if (hash_pos_ori == hash_pos)
      return false;

    if ('\0' == raw_spellings_[hash_pos].str[0])
      return false;

    if (strncmp(raw_spellings_[hash_pos].str, spelling_str, spelling_size_ - 1)
        == 0)
      return true;
  }

  // never reach here
  return false;
}

const char* SpellingTable::arrange(unsigned *item_size, unsigned *spl_num) {
  if (NULL == raw_spellings_ || NULL == spelling_buf_ ||
      NULL == item_size || NULL == spl_num)
    return NULL;

  qsort(raw_spellings_, spelling_max_num_, sizeof(RawSpelling),
        compare_raw_spl_eb);

  // After sorting, only the first spelling_num_ items are valid.
  // Copy them to the destination buffer.
  for (unsigned pos = 0; pos < spelling_num_; pos++) {
    strncpy(spelling_buf_ + pos * spelling_size_, raw_spellings_[pos].str,
            spelling_size_);
  }

  if (need_score_) {
    if (kPrintDebug0)
      printf("------------Spelling Possiblities--------------\n");

    double max_score = 0;
    double min_score = 0;

    // After sorting, only the first spelling_num_ items are valid.
    for (unsigned pos = 0; pos < spelling_num_; pos++) {
      raw_spellings_[pos].freq /= total_freq_;
      if (need_score_) {
        if (0 == pos) {
          max_score = raw_spellings_[0].freq;
          min_score = max_score;
        } else {
          if (raw_spellings_[pos].freq > max_score)
            max_score = raw_spellings_[pos].freq;
          if (raw_spellings_[pos].freq < min_score)
            min_score = raw_spellings_[pos].freq;
        }
      }
    }

    if (kPrintDebug0)
      printf("-----max psb: %f, min psb: %f\n", max_score, min_score);

    max_score = log(max_score);
    min_score = log(min_score);

    if (kPrintDebug0)
      printf("-----max log value: %f, min log value: %f\n",
             max_score, min_score);

    // The absolute value of min_score is bigger than that of max_score because
    // both of them are negative after log function.
    score_amplifier_ = 1.0 * 255 / min_score;

    double average_score = 0;
    for (unsigned pos = 0; pos < spelling_num_; pos++) {
      double score = log(raw_spellings_[pos].freq) * score_amplifier_;
      assert(score >= 0);

      average_score += score;

      // Because of calculation precision issue, score might be a little bigger
      // than 255 after being amplified.
      if (score > 255)
        score = 255;
      char *this_spl_buf = spelling_buf_ + pos * spelling_size_;
      this_spl_buf[spelling_size_ - 1] =
          static_cast<char>((unsigned char)score);

      if (kPrintDebug0) {
        printf("---pos:%d, %s, psb:%d\n", pos, this_spl_buf,
               (unsigned char)this_spl_buf[spelling_size_ -1]);
      }
    }
    average_score /= spelling_num_;
    assert(average_score <= 255);
    average_score_ = static_cast<uint8>(average_score);

    if (kPrintDebug0)
      printf("\n----Score Amplifier: %f, Average Score: %d\n", score_amplifier_,
             average_score_);
  }

  *item_size = spelling_size_;
  *spl_num = spelling_num_;
  frozen_ = true;
  return spelling_buf_;
}

float SpellingTable::get_score_amplifier() {
  return static_cast<float>(score_amplifier_);
}

unsigned char SpellingTable::get_average_score() {
  return average_score_;
}

#endif  // ___BUILD_MODEL___
}  // namespace ime_pinyin
