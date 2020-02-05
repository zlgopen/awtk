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

#include "../include/userdict.h"
#include "../include/splparser.h"
#include "../include/ngram.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef ___DEBUG_PERF___
#include <cutils/log.h>
#endif
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#ifdef HAS_PTHREAD
#include <pthread.h>
#else
#define pthread_mutex_lock(m) (m)
#define pthread_mutex_trylock(m) 0
#define pthread_mutex_unlock(m) (m)
#define pthread_mutex_t int
#define PTHREAD_MUTEX_INITIALIZER 0
#endif

#include <math.h>

namespace ime_pinyin {

#ifdef ___DEBUG_PERF___
static uint64 _ellapse_ = 0;
static struct timeval _tv_start_, _tv_end_;
#define DEBUG_PERF_BEGIN \
    do { \
      gettimeofday(&_tv_start_, NULL); \
    } while(0)
#define DEBUG_PERF_END \
    do { \
      gettimeofday(&_tv_end_, NULL); \
      _ellapse_ = (_tv_end_.tv_sec - _tv_start_.tv_sec) * 1000000 + \
                  (_tv_end_.tv_usec - _tv_start_.tv_usec); \
    } while(0)
#define LOGD_PERF(message) \
    LOGD("PERFORMANCE[%s] %llu usec.", message, _ellapse_);
#else
#define DEBUG_PERF_BEGIN
#define DEBUG_PERF_END
#define LOGD_PERF(message)
#endif

// XXX File load and write are thread-safe by g_mutex_
static pthread_mutex_t g_mutex_ = PTHREAD_MUTEX_INITIALIZER;
static struct timeval g_last_update_ = {0, 0};

inline uint32 UserDict::get_dict_file_size(UserDictInfo * info) {
  return (4 + info->lemma_size + (info->lemma_count << 3)
#ifdef ___PREDICT_ENABLED___
          + (info->lemma_count << 2)
#endif
#ifdef ___SYNC_ENABLED___
          + (info->sync_count << 2)
#endif
          + sizeof(*info));
}

inline LmaScoreType UserDict::translate_score(int raw_score) {
  // 1) ori_freq: original user frequency
  uint32 ori_freq = extract_score_freq(raw_score);
  // 2) lmt_off: lmt index (week offset for example)
  uint64 lmt_off = ((raw_score & 0xffff0000) >> 16);
  if (kUserDictLMTBitWidth < 16) {
    uint64 mask = ~(1 << kUserDictLMTBitWidth);
    lmt_off &= mask;
  }
  // 3) now_off: current time index (current week offset for example)
  // assuming load_time_ is around current time
  uint64 now_off = load_time_.tv_sec;
  now_off = (now_off - kUserDictLMTSince) / kUserDictLMTGranularity;
  now_off = (now_off << (64 - kUserDictLMTBitWidth));
  now_off = (now_off >> (64 - kUserDictLMTBitWidth));
  // 4) factor: decide expand-factor
  int delta = now_off - lmt_off;
  if (delta > 4)
    delta = 4;
  int factor = 80 - (delta << 4);

  double tf = (double)(dict_info_.total_nfreq + total_other_nfreq_);
  return (LmaScoreType)(log((double)factor * (double)ori_freq / tf)
                        * NGram::kLogValueAmplifier);
}

inline int UserDict::extract_score_freq(int raw_score) {
  // Frequence stored in lowest 16 bits
  int freq = (raw_score & 0x0000ffff);
  return freq;
}

inline uint64 UserDict::extract_score_lmt(int raw_score) {
  uint64 lmt = ((raw_score & 0xffff0000) >> 16);
  if (kUserDictLMTBitWidth < 16) {
    uint64 mask = ~(1 << kUserDictLMTBitWidth);
    lmt &= mask;
  }
  lmt = lmt * kUserDictLMTGranularity + kUserDictLMTSince;
  return lmt;
}

inline int UserDict::build_score(uint64 lmt, int freq) {
  lmt = (lmt - kUserDictLMTSince) / kUserDictLMTGranularity;
  lmt = (lmt << (64 - kUserDictLMTBitWidth));
  lmt = (lmt >> (64 - kUserDictLMTBitWidth));
  uint16 lmt16 = (uint16)lmt;
  int s = freq;
  s &= 0x0000ffff;
  s = (lmt16 << 16) | s;
  return s;
}

inline int64 UserDict::utf16le_atoll(uint16 *s, int len) {
  int64 ret = 0;
  if (len <= 0)
    return ret;

  int flag = 1;
  const uint16 * endp = s + len;
  if (*s == '-') {
    flag = -1;
    s++;
  } else if (*s == '+') {
    s++;
  }

  while (*s >= '0' && *s <= '9' && s < endp) {
    ret += ret * 10 + (*s) - '0';
    s++;
  }
  return ret * flag;
}

inline int UserDict::utf16le_lltoa(int64 v, uint16 *s, int size) {
  if (!s || size <= 0)
    return 0;
  uint16 *endp = s + size;
  int ret_len = 0;
  if (v < 0) {
    *(s++) = '-';
    ++ret_len;
    v *= -1;
  }

  uint16 *b = s;
  while (s < endp && v != 0) {
    *(s++) = '0' + (v % 10);
    v = v / 10;
    ++ret_len;
  }

  if (v != 0)
    return 0;

  --s;

  while (b < s) {
    *b = *s;
    ++b, --s;
  }

  return ret_len;
}

inline void UserDict::set_lemma_flag(uint32 offset, uint8 flag) {
  offset &= kUserDictOffsetMask;
  lemmas_[offset] |= flag;
}

inline char UserDict::get_lemma_flag(uint32 offset) {
  offset &= kUserDictOffsetMask;
  return (char)(lemmas_[offset]);
}

inline char UserDict::get_lemma_nchar(uint32 offset) {
  offset &= kUserDictOffsetMask;
  return (char)(lemmas_[offset + 1]);
}

inline uint16 * UserDict::get_lemma_spell_ids(uint32 offset) {
  offset &= kUserDictOffsetMask;
  return (uint16 *)(lemmas_ + offset + 2);
}

inline uint16 * UserDict::get_lemma_word(uint32 offset) {
  offset &= kUserDictOffsetMask;
  uint8 nchar = get_lemma_nchar(offset);
  return (uint16 *)(lemmas_ + offset + 2 + (nchar << 1));
}

inline LemmaIdType UserDict::get_max_lemma_id() {
  // When a lemma is deleted, we don't not claim its id back for
  // simplicity and performance
  return start_id_ + dict_info_.lemma_count - 1;
}

inline bool UserDict::is_valid_lemma_id(LemmaIdType id) {
  if (id >= start_id_ && id <= get_max_lemma_id())
    return true;
  return false;
}

inline bool UserDict::is_valid_state() {
  if (state_ == USER_DICT_NONE)
    return false;
  return true;
}

UserDict::UserDict()
    : start_id_(0),
      version_(0),
      lemmas_(NULL),
      offsets_(NULL),
      scores_(NULL),
      ids_(NULL),
#ifdef ___PREDICT_ENABLED___
      predicts_(NULL),
#endif
#ifdef ___SYNC_ENABLED___
      syncs_(NULL),
      sync_count_size_(0),
#endif
      offsets_by_id_(NULL),
      lemma_count_left_(0),
      lemma_size_left_(0),
      dict_file_(NULL),
      state_(USER_DICT_NONE) {
  memset(&dict_info_, 0, sizeof(dict_info_));
  memset(&load_time_, 0, sizeof(load_time_));
#ifdef ___CACHE_ENABLED___
  cache_init();
#endif
}

UserDict::~UserDict() {
  close_dict();
}

bool UserDict::load_dict(const char *file_name, LemmaIdType start_id,
                         LemmaIdType end_id) {
#ifdef ___DEBUG_PERF___
  DEBUG_PERF_BEGIN;
#endif
  dict_file_ = strdup(file_name);
  if (!dict_file_)
    return false;

  start_id_ = start_id;

  if (false == validate(file_name) && false == reset(file_name)) {
    goto error;
  }
  if (false == load(file_name, start_id)) {
    goto error;
  }

  state_ = USER_DICT_SYNC;

  gettimeofday(&load_time_, NULL);

#ifdef ___DEBUG_PERF___
  DEBUG_PERF_END;
  LOGD_PERF("load_dict");
#endif
  return true;
 error:
  free((void*)dict_file_);
  start_id_ = 0;
  return false;
}

bool UserDict::close_dict() {
  if (state_ == USER_DICT_NONE)
    return true;
  if (state_ == USER_DICT_SYNC)
    goto out;

  // If dictionary is written back by others,
  // we can not simply write back here
  // To do a safe flush, we have to discard all newly added
  // lemmas and try to reload dict file.
  pthread_mutex_lock(&g_mutex_);
  if (load_time_.tv_sec > g_last_update_.tv_sec ||
    (load_time_.tv_sec == g_last_update_.tv_sec &&
     load_time_.tv_usec > g_last_update_.tv_usec)) {
    write_back();
    gettimeofday(&g_last_update_, NULL);
  }
  pthread_mutex_unlock(&g_mutex_);

 out:
  free((void*)dict_file_);
  free(lemmas_);
  free(offsets_);
  free(offsets_by_id_);
  free(scores_);
  free(ids_);
#ifdef ___PREDICT_ENABLED___
  free(predicts_);
#endif

  version_ = 0;
  dict_file_ = NULL;
  lemmas_ = NULL;
#ifdef ___SYNC_ENABLED___
  syncs_ = NULL;
  sync_count_size_ = 0;
#endif
  offsets_ = NULL;
  offsets_by_id_ = NULL;
  scores_ = NULL;
  ids_ = NULL;
#ifdef ___PREDICT_ENABLED___
  predicts_ = NULL;
#endif

  memset(&dict_info_, 0, sizeof(dict_info_));
  lemma_count_left_ = 0;
  lemma_size_left_ = 0;
  state_ = USER_DICT_NONE;

  return true;
}

unsigned UserDict::number_of_lemmas() {
  return dict_info_.lemma_count;
}

void UserDict::reset_milestones(uint16 from_step, MileStoneHandle from_handle) {
  return;
}

MileStoneHandle UserDict::extend_dict(MileStoneHandle from_handle,
                                      const DictExtPara *dep,
                                      LmaPsbItem *lpi_items,
                                      unsigned lpi_max, unsigned *lpi_num) {
  if (is_valid_state() == false)
    return 0;

  bool need_extend = false;

#ifdef ___DEBUG_PERF___
  DEBUG_PERF_BEGIN;
#endif
  *lpi_num = _get_lpis(dep->splids, dep->splids_extended + 1,
                       lpi_items, lpi_max, &need_extend);
#ifdef ___DEBUG_PERF___
  DEBUG_PERF_END;
  LOGD_PERF("extend_dict");
#endif
  return ((*lpi_num > 0 || need_extend) ? 1 : 0);
}

int UserDict::is_fuzzy_prefix_spell_id(
    const uint16 * id1, uint16 len1, const UserDictSearchable *searchable) {
  if (len1 < searchable->splids_len)
    return 0;

  SpellingTrie &spl_trie = SpellingTrie::get_instance();
  uint32 i = 0;
  for (i = 0; i < searchable->splids_len; i++) {
    const char py1 = *spl_trie.get_spelling_str(id1[i]);
    uint16 off = 8 * (i % 4);
    const char py2 = ((searchable->signature[i/4] & (0xff << off)) >> off);
    if (py1 == py2)
      continue;
    return 0;
  }
  return 1;
}

int UserDict::fuzzy_compare_spell_id(
    const uint16 * id1, uint16 len1, const UserDictSearchable *searchable) {
  if (len1 < searchable->splids_len)
    return -1;
  if (len1 > searchable->splids_len)
    return 1;

  SpellingTrie &spl_trie = SpellingTrie::get_instance();
  uint32 i = 0;
  for (i = 0; i < len1; i++) {
    const char py1 = *spl_trie.get_spelling_str(id1[i]);
    uint16 off = 8 * (i % 4);
    const char py2 = ((searchable->signature[i/4] & (0xff << off)) >> off);
    if (py1 == py2)
      continue;
    if (py1 > py2)
      return 1;
    return -1;
  }
  return 0;
}

bool UserDict::is_prefix_spell_id(
    const uint16 * fullids, uint16 fulllen,
    const UserDictSearchable *searchable) {
  if (fulllen < searchable->splids_len)
    return false;

  uint32 i = 0;
  for (; i < searchable->splids_len; i++) {
    uint16 start_id = searchable->splid_start[i];
    uint16 count = searchable->splid_count[i];
    if (fullids[i] >= start_id && fullids[i] < start_id + count)
      continue;
    else
      return false;
  }
  return true;
}

bool UserDict::equal_spell_id(
    const uint16 * fullids, uint16 fulllen,
    const UserDictSearchable *searchable) {
  if (fulllen != searchable->splids_len)
    return false;

  uint32 i = 0;
  for (; i < fulllen; i++) {
    uint16 start_id = searchable->splid_start[i];
    uint16 count = searchable->splid_count[i];
    if (fullids[i] >= start_id && fullids[i] < start_id + count)
      continue;
    else
      return false;
  }
  return true;
}

int32 UserDict::locate_first_in_offsets(const UserDictSearchable * searchable) {
  int32 begin = 0;
  int32 end = dict_info_.lemma_count - 1;
  int32 middle = -1;

  int32 first_prefix = middle;
  int32 last_matched = middle;

  while (begin <= end) {
    middle = (begin + end) >> 1;
    uint32 offset = offsets_[middle];
    uint8 nchar = get_lemma_nchar(offset);
    const uint16 * splids = get_lemma_spell_ids(offset);
    int cmp = fuzzy_compare_spell_id(splids, nchar, searchable);
    int pre = is_fuzzy_prefix_spell_id(splids, nchar, searchable);

    if (pre)
      first_prefix = middle;

    if (cmp < 0) {
      begin = middle + 1;
    } else if (cmp > 0) {
      end = middle - 1;
    } else {
      end = middle - 1;
      last_matched = middle;
    }
  }

  return first_prefix;
}

void UserDict::prepare_locate(UserDictSearchable *searchable,
                             const uint16 *splid_str,
                             uint16 splid_str_len) {
  searchable->splids_len = splid_str_len;
  memset(searchable->signature, 0, sizeof(searchable->signature));

  SpellingTrie &spl_trie = SpellingTrie::get_instance();
  uint32 i = 0;
  for (; i < splid_str_len; i++) {
    if (spl_trie.is_half_id(splid_str[i])) {
      searchable->splid_count[i] =
          spl_trie.half_to_full(splid_str[i],
                                &(searchable->splid_start[i]));
    } else {
      searchable->splid_count[i] = 1;
      searchable->splid_start[i] = splid_str[i];
    }
    const unsigned char py = *spl_trie.get_spelling_str(splid_str[i]);
    searchable->signature[i>>2] |= (py << (8 * (i % 4)));
  }
}

unsigned UserDict::get_lpis(const uint16 *splid_str, uint16 splid_str_len,
                          LmaPsbItem *lpi_items, unsigned lpi_max) {
  return _get_lpis(splid_str, splid_str_len, lpi_items, lpi_max, NULL);
}

unsigned UserDict::_get_lpis(const uint16 *splid_str,
                           uint16 splid_str_len, LmaPsbItem *lpi_items,
                           unsigned lpi_max, bool * need_extend) {
  bool tmp_extend;
  if (!need_extend)
    need_extend = &tmp_extend;

  *need_extend = false;

  if (is_valid_state() == false)
    return 0;
  if (lpi_max <= 0)
    return 0;

  if (0 == pthread_mutex_trylock(&g_mutex_)) {
    if (load_time_.tv_sec < g_last_update_.tv_sec ||
      (load_time_.tv_sec == g_last_update_.tv_sec &&
       load_time_.tv_usec < g_last_update_.tv_usec)) {
      // Others updated disk file, have to reload
      pthread_mutex_unlock(&g_mutex_);
      flush_cache();
    } else {
      pthread_mutex_unlock(&g_mutex_);
    }
  } else {
  }

  UserDictSearchable searchable;
  prepare_locate(&searchable, splid_str, splid_str_len);

  uint32 max_off = dict_info_.lemma_count;
#ifdef ___CACHE_ENABLED___
  int32 middle;
  uint32 start, count;
  bool cached = cache_hit(&searchable, &start, &count);
  if (cached) {
    middle = start;
    max_off = start + count;
  } else {
    middle = locate_first_in_offsets(&searchable);
    start = middle;
  }
#else
  int32 middle = locate_first_in_offsets(&searchable);
#endif

  if (middle == -1) {
#ifdef ___CACHE_ENABLED___
    if (!cached)
      cache_push(USER_DICT_MISS_CACHE, &searchable, 0, 0);
#endif
    return 0;
  }

  unsigned lpi_current = 0;

  bool fuzzy_break = false;
  bool prefix_break = false;
  while ((unsigned)middle < max_off && !fuzzy_break && !prefix_break) {
    if (lpi_current >= lpi_max)
      break;
    uint32 offset = offsets_[middle];
    // Ignore deleted lemmas
    if (offset & kUserDictOffsetFlagRemove) {
      middle++;
      continue;
    }
    uint8 nchar = get_lemma_nchar(offset);
    uint16 * splids = get_lemma_spell_ids(offset);
#ifdef ___CACHE_ENABLED___
    if (!cached && 0 != fuzzy_compare_spell_id(splids, nchar, &searchable)) {
#else
    if (0 != fuzzy_compare_spell_id(splids, nchar, &searchable)) {
#endif
      fuzzy_break = true;
    }

    if (prefix_break == false) {
      if (is_fuzzy_prefix_spell_id(splids, nchar, &searchable)) {
        if (*need_extend == false &&
            is_prefix_spell_id(splids, nchar, &searchable)) {
          *need_extend = true;
        }
      } else {
        prefix_break = true;
      }
    }

    if (equal_spell_id(splids, nchar, &searchable) == true) {
      lpi_items[lpi_current].psb = translate_score(scores_[middle]);
      lpi_items[lpi_current].id = ids_[middle];
      lpi_items[lpi_current].lma_len = nchar;
      lpi_current++;
    }
    middle++;
  }

#ifdef ___CACHE_ENABLED___
  if (!cached) {
    count = middle - start;
    cache_push(USER_DICT_CACHE, &searchable, start, count);
  }
#endif

  return lpi_current;
}

uint16 UserDict::get_lemma_str(LemmaIdType id_lemma, char16* str_buf,
                               uint16 str_max) {
  if (is_valid_state() == false)
    return 0;
  if (is_valid_lemma_id(id_lemma) == false)
    return 0;
  uint32 offset = offsets_by_id_[id_lemma - start_id_];
  uint8 nchar = get_lemma_nchar(offset);
  char16 * str = get_lemma_word(offset);
  uint16 m = nchar < str_max -1 ? nchar : str_max - 1;
  int i = 0;
  for (; i < m; i++) {
    str_buf[i] = str[i];
  }
  str_buf[i] = 0;
  return m;
}

uint16 UserDict::get_lemma_splids(LemmaIdType id_lemma, uint16 *splids,
                                  uint16 splids_max, bool arg_valid) {
  if (is_valid_lemma_id(id_lemma) == false)
    return 0;
  uint32 offset = offsets_by_id_[id_lemma - start_id_];
  uint8 nchar = get_lemma_nchar(offset);
  const uint16 * ids = get_lemma_spell_ids(offset);
  int i = 0;
  for (; i < nchar && i < splids_max; i++)
    splids[i] = ids[i];
  return i;
}

unsigned UserDict::predict(const char16 last_hzs[], uint16 hzs_len,
                         NPredictItem *npre_items, unsigned npre_max,
                         unsigned b4_used) {
  uint32 new_added = 0;
#ifdef ___PREDICT_ENABLED___
  int32 end = dict_info_.lemma_count - 1;
  int j = locate_first_in_predicts((const uint16*)last_hzs, hzs_len);
  if (j == -1)
    return 0;

  while (j <= end) {
    uint32 offset = predicts_[j];
    // Ignore deleted lemmas
    if (offset & kUserDictOffsetFlagRemove) {
      j++;
      continue;
    }
    uint32 nchar = get_lemma_nchar(offset);
    uint16 * words = get_lemma_word(offset);
    uint16 * splids = get_lemma_spell_ids(offset);

    if (nchar <= hzs_len) {
      j++;
      continue;
    }

    if (memcmp(words, last_hzs, hzs_len << 1) == 0) {
      if (new_added >= npre_max) {
        return new_added;
      }
      uint32 cpy_len =
          (nchar < kMaxPredictSize ? (nchar << 1) : (kMaxPredictSize << 1))
          - (hzs_len << 1);
      npre_items[new_added].his_len = hzs_len;
      npre_items[new_added].psb = get_lemma_score(words, splids, nchar);
      memcpy(npre_items[new_added].pre_hzs, words + hzs_len, cpy_len);
      if ((cpy_len >> 1) < kMaxPredictSize) {
        npre_items[new_added].pre_hzs[cpy_len >> 1] = 0;
      }
      new_added++;
    } else {
      break;
    }

    j++;
  }
#endif
  return new_added;
}

int32 UserDict::locate_in_offsets(char16 lemma_str[], uint16 splid_str[],
                                  uint16 lemma_len) {
  int32 max_off = dict_info_.lemma_count;

  UserDictSearchable searchable;
  prepare_locate(&searchable, splid_str, lemma_len);
#ifdef ___CACHE_ENABLED___
  int32 off;
  uint32 start, count;
  bool cached = load_cache(&searchable, &start, &count);
  if (cached) {
    off = start;
    max_off = start + count;
  } else {
    off = locate_first_in_offsets(&searchable);
    start = off;
  }
#else
  int32 off = locate_first_in_offsets(&searchable);
#endif

  if (off == -1) {
    return off;
  }

  while (off < max_off) {
    uint32 offset = offsets_[off];
    if (offset & kUserDictOffsetFlagRemove) {
      off++;
      continue;
    }
    uint16 * splids = get_lemma_spell_ids(offset);
#ifdef ___CACHE_ENABLED___
    if (!cached && 0 != fuzzy_compare_spell_id(splids, lemma_len, &searchable))
      break;
#else
    if (0 != fuzzy_compare_spell_id(splids, lemma_len, &searchable))
      break;
#endif
    if (equal_spell_id(splids, lemma_len, &searchable) == true) {
      uint16 * str = get_lemma_word(offset);
      uint32 i = 0;
      for (i = 0; i < lemma_len; i++) {
        if (str[i] == lemma_str[i])
          continue;
        break;
      }
      if (i < lemma_len) {
        off++;
        continue;
      }
#ifdef ___CACHE_ENABLED___
      // No need to save_cache here, since current function is invoked by
      // put_lemma. It's rarely possible for a user input same lemma twice.
      // That means first time user type a new lemma, it is newly added into
      // user dictionary, then it's possible that user type the same lemma
      // again.
      // Another reason save_cache can not be invoked here is this function
      // aborts when lemma is found, and it never knows the count.
#endif
      return off;
    }
    off++;
  }

  return -1;
}

#ifdef ___PREDICT_ENABLED___
uint32 UserDict::locate_where_to_insert_in_predicts(
    const uint16 * words, int lemma_len) {
  int32 begin = 0;
  int32 end = dict_info_.lemma_count - 1;
  int32 middle = end;

  uint32 last_matched = middle;

  while (begin <= end) {
    middle = (begin + end) >> 1;
    uint32 offset = offsets_[middle];
    uint8 nchar = get_lemma_nchar(offset);
    const uint16 * ws = get_lemma_word(offset);

    uint32 minl = nchar < lemma_len ? nchar : lemma_len;
    uint32 k = 0;
    int cmp = 0;

    for (; k < minl; k++) {
      if (ws[k] < words[k]) {
        cmp = -1;
        break;
      } else if (ws[k] > words[k]) {
        cmp = 1;
        break;
      }
    }
    if (cmp == 0) {
      if (nchar < lemma_len)
        cmp = -1;
      else if (nchar > lemma_len)
        cmp = 1;
    }

    if (cmp < 0) {
      begin = middle + 1;
      last_matched = middle;
    } else if (cmp > 0) {
      end = middle - 1;
    } else {
      end = middle - 1;
      last_matched = middle;
    }
  }

  return last_matched;
}

int32 UserDict::locate_first_in_predicts(const uint16 * words, int lemma_len) {
  int32 begin = 0;
  int32 end = dict_info_.lemma_count - 1;
  int32 middle = -1;

  int32 last_matched = middle;

  while (begin <= end) {
    middle = (begin + end) >> 1;
    uint32 offset = offsets_[middle];
    uint8 nchar = get_lemma_nchar(offset);
    const uint16 * ws = get_lemma_word(offset);

    uint32 minl = nchar < lemma_len ? nchar : lemma_len;
    uint32 k = 0;
    int cmp = 0;

    for (; k < minl; k++) {
      if (ws[k] < words[k]) {
        cmp = -1;
        break;
      } else if (ws[k] > words[k]) {
        cmp = 1;
        break;
      }
    }
    if (cmp == 0) {
      if (nchar >= lemma_len)
        last_matched = middle;
      if (nchar < lemma_len)
        cmp = -1;
      else if (nchar > lemma_len)
        cmp = 1;
    }

    if (cmp < 0) {
      begin = middle + 1;
    } else if (cmp > 0) {
      end = middle - 1;
    } else {
      end = middle - 1;
    }
  }

  return last_matched;
}

#endif

LemmaIdType UserDict::get_lemma_id(char16 lemma_str[], uint16 splids[],
                                   uint16 lemma_len) {
  int32 off = locate_in_offsets(lemma_str, splids, lemma_len);
  if (off == -1) {
    return 0;
  }

  return ids_[off];
}

LmaScoreType UserDict::get_lemma_score(LemmaIdType lemma_id) {
  if (is_valid_state() == false)
    return 0;
  if (is_valid_lemma_id(lemma_id) == false)
    return 0;

  return translate_score(_get_lemma_score(lemma_id));
}

LmaScoreType UserDict::get_lemma_score(char16 lemma_str[], uint16 splids[],
                                uint16 lemma_len) {
  if (is_valid_state() == false)
    return 0;
  return translate_score(_get_lemma_score(lemma_str, splids, lemma_len));
}

int UserDict::_get_lemma_score(LemmaIdType lemma_id) {
  if (is_valid_state() == false)
    return 0;
  if (is_valid_lemma_id(lemma_id) == false)
    return 0;

  uint32 offset = offsets_by_id_[lemma_id - start_id_];

  uint32 nchar = get_lemma_nchar(offset);
  uint16 * spl = get_lemma_spell_ids(offset);
  uint16 * wrd = get_lemma_word(offset);

  int32 off = locate_in_offsets(wrd, spl, nchar);
  if (off == -1) {
    return 0;
  }

  return scores_[off];
}

int UserDict::_get_lemma_score(char16 lemma_str[], uint16 splids[],
                                uint16 lemma_len) {
  if (is_valid_state() == false)
    return 0;

  int32 off = locate_in_offsets(lemma_str, splids, lemma_len);
  if (off == -1) {
    return 0;
  }

  return scores_[off];
}

#ifdef ___SYNC_ENABLED___
void UserDict::remove_lemma_from_sync_list(uint32 offset) {
  offset &= kUserDictOffsetMask;
  uint32 i = 0;
  for (; i < dict_info_.sync_count; i++) {
    unsigned int off = (syncs_[i] & kUserDictOffsetMask);
    if (off == offset)
      break;
  }
  if (i < dict_info_.sync_count) {
    syncs_[i] = syncs_[dict_info_.sync_count - 1];
    dict_info_.sync_count--;
  }
}
#endif

#ifdef ___PREDICT_ENABLED___
void UserDict::remove_lemma_from_predict_list(uint32 offset) {
  offset &= kUserDictOffsetMask;
  uint32 i = 0;
  for (; i < dict_info_.lemma_count; i++) {
    unsigned int off = (predicts_[i] & kUserDictOffsetMask);
    if (off == offset) {
      predicts_[i] |= kUserDictOffsetFlagRemove;
      break;
    }
  }
}
#endif

bool UserDict::remove_lemma_by_offset_index(int offset_index) {
  if (is_valid_state() == false)
    return 0;

  int32 off = offset_index;
  if (off == -1) {
    return false;
  }

  uint32 offset = offsets_[off];
  uint32 nchar = get_lemma_nchar(offset);

  offsets_[off] |= kUserDictOffsetFlagRemove;

#ifdef ___SYNC_ENABLED___
  // Remove corresponding sync item
  remove_lemma_from_sync_list(offset);
#endif

#ifdef ___PREDICT_ENABLED___
  remove_lemma_from_predict_list(offset);
#endif
  dict_info_.free_count++;
  dict_info_.free_size += (2 + (nchar << 2));

  if (state_ < USER_DICT_OFFSET_DIRTY)
    state_ = USER_DICT_OFFSET_DIRTY;
  return true;
}

bool UserDict::remove_lemma(LemmaIdType lemma_id) {
  if (is_valid_state() == false)
    return 0;
  if (is_valid_lemma_id(lemma_id) == false)
    return false;
  uint32 offset = offsets_by_id_[lemma_id - start_id_];

  uint32 nchar = get_lemma_nchar(offset);
  uint16 * spl = get_lemma_spell_ids(offset);
  uint16 * wrd = get_lemma_word(offset);

  int32 off = locate_in_offsets(wrd, spl, nchar);

  return remove_lemma_by_offset_index(off);
}

void UserDict::flush_cache() {
  LemmaIdType start_id = start_id_;
  const char * file = strdup(dict_file_);
  if (!file)
    return;
  close_dict();
  load_dict(file, start_id, kUserDictIdEnd);
  free((void*)file);
#ifdef ___CACHE_ENABLED___
  cache_init();
#endif
  return;
}

bool UserDict::reset(const char *file) {
  FILE *fp = fopen(file, "w+");
  if (!fp) {
    return false;
  }
  uint32 version = kUserDictVersion;
  unsigned wred = fwrite(&version, 1, 4, fp);
  UserDictInfo info;
  memset(&info, 0, sizeof(info));
  // By default, no limitation for lemma count and size
  // thereby, reclaim_ratio is never used
  wred += fwrite(&info, 1, sizeof(info), fp);
  if (wred != sizeof(info) + sizeof(version)) {
    fclose(fp);
    unlink(file);
    return false;
  }
  fclose(fp);
  return true;
}

bool UserDict::validate(const char *file) {
  // b is ignored in POSIX compatible os including Linux
  // while b is important flag for Windows to specify binary mode
  FILE *fp = fopen(file, "rb");
  if (!fp) {
    return false;
  }

  unsigned size;
  unsigned readed;
  uint32 version;
  UserDictInfo dict_info;

  // validate
  int err = fseek(fp, 0, SEEK_END);
  if (err) {
    goto error;
  }

  size = ftell(fp);
  if (size < 4 + sizeof(dict_info)) {
    goto error;
  }

  err = fseek(fp, 0, SEEK_SET);
  if (err) {
    goto error;
  }

  readed = fread(&version, 1, sizeof(version), fp);
  if (readed < sizeof(version)) {
    goto error;
  }
  if (version != kUserDictVersion) {
    goto error;
  }

  err = fseek(fp, -1 * sizeof(dict_info), SEEK_END);
  if (err) {
    goto error;
  }

  readed = fread(&dict_info, 1, sizeof(dict_info), fp);
  if (readed != sizeof(dict_info)) {
    goto error;
  }

  if (size != get_dict_file_size(&dict_info)) {
    goto error;
  }

  fclose(fp);
  return true;

 error:
  fclose(fp);
  return false;
}

bool UserDict::load(const char *file, LemmaIdType start_id) {
  if (0 != pthread_mutex_trylock(&g_mutex_)) {
    return false;
  }
  // b is ignored in POSIX compatible os including Linux
  // while b is important flag for Windows to specify binary mode
  FILE *fp = fopen(file, "rb");
  if (!fp) {
    pthread_mutex_unlock(&g_mutex_);
    return false;
  }

  unsigned readed, toread;
  UserDictInfo dict_info;
  uint8 *lemmas = NULL;
  uint32 *offsets = NULL;
#ifdef ___SYNC_ENABLED___
  uint32 *syncs = NULL;
#endif
  uint32 *scores = NULL;
  uint32 *ids = NULL;
  uint32 *offsets_by_id = NULL;
#ifdef ___PREDICT_ENABLED___
  uint32 *predicts = NULL;
#endif
  unsigned i;
  int err;

  err = fseek(fp, -1 * sizeof(dict_info), SEEK_END);
  if (err) goto error;

  readed = fread(&dict_info, 1, sizeof(dict_info), fp);
  if (readed != sizeof(dict_info)) goto error;

  lemmas = (uint8 *)malloc(
      dict_info.lemma_size +
      (kUserDictPreAlloc * (2 + (kUserDictAverageNchar << 2))));

  if (!lemmas) goto error;

  offsets = (uint32 *)malloc((dict_info.lemma_count + kUserDictPreAlloc) << 2);
  if (!offsets) goto error;

#ifdef ___PREDICT_ENABLED___
  predicts = (uint32 *)malloc((dict_info.lemma_count + kUserDictPreAlloc) << 2);
  if (!predicts) goto error;
#endif

#ifdef ___SYNC_ENABLED___
  syncs = (uint32 *)malloc((dict_info.sync_count + kUserDictPreAlloc) << 2);
  if (!syncs) goto error;
#endif

  scores = (uint32 *)malloc((dict_info.lemma_count + kUserDictPreAlloc) << 2);
  if (!scores) goto error;

  ids = (uint32 *)malloc((dict_info.lemma_count + kUserDictPreAlloc) << 2);
  if (!ids) goto error;

  offsets_by_id = (uint32 *)malloc(
      (dict_info.lemma_count + kUserDictPreAlloc) << 2);
  if (!offsets_by_id) goto error;

  err = fseek(fp, 4, SEEK_SET);
  if (err) goto error;

  readed = 0;
  while (readed < dict_info.lemma_size && !ferror(fp) && !feof(fp)) {
    readed += fread(lemmas + readed, 1, dict_info.lemma_size - readed, fp);
  }
  if (readed < dict_info.lemma_size)
    goto error;

  toread = (dict_info.lemma_count << 2);
  readed = 0;
  while (readed < toread && !ferror(fp) && !feof(fp)) {
    readed += fread((((uint8*)offsets) + readed), 1, toread - readed, fp);
  }
  if (readed < toread)
    goto error;

#ifdef ___PREDICT_ENABLED___
  toread = (dict_info.lemma_count << 2);
  readed = 0;
  while (readed < toread && !ferror(fp) && !feof(fp)) {
    readed += fread((((uint8*)predicts) + readed), 1, toread - readed, fp);
  }
  if (readed < toread)
    goto error;
#endif

  readed = 0;
  while (readed < toread && !ferror(fp) && !feof(fp)) {
    readed += fread((((uint8*)scores) + readed), 1, toread - readed, fp);
  }
  if (readed < toread)
    goto error;

#ifdef ___SYNC_ENABLED___
  toread = (dict_info.sync_count << 2);
  readed = 0;
  while (readed < toread && !ferror(fp) && !feof(fp)) {
    readed += fread((((uint8*)syncs) + readed), 1, toread - readed, fp);
  }
  if (readed < toread)
    goto error;
#endif

  for (i = 0; i < dict_info.lemma_count; i++) {
    ids[i] = start_id + i;
    offsets_by_id[i] = offsets[i];
  }

  lemmas_ = lemmas;
  offsets_ = offsets;
#ifdef ___SYNC_ENABLED___
  syncs_ = syncs;
  sync_count_size_ = dict_info.sync_count + kUserDictPreAlloc;
#endif
  offsets_by_id_ = offsets_by_id;
  scores_ = scores;
  ids_ = ids;
#ifdef ___PREDICT_ENABLED___
  predicts_ = predicts;
#endif
  lemma_count_left_ = kUserDictPreAlloc;
  lemma_size_left_ = kUserDictPreAlloc * (2 + (kUserDictAverageNchar << 2));
  memcpy(&dict_info_, &dict_info, sizeof(dict_info));
  state_ = USER_DICT_SYNC;

  fclose(fp);

  pthread_mutex_unlock(&g_mutex_);
  return true;

 error:
  if (lemmas) free(lemmas);
  if (offsets) free(offsets);
#ifdef ___SYNC_ENABLED___
  if (syncs) free(syncs);
#endif
  if (scores) free(scores);
  if (ids) free(ids);
  if (offsets_by_id) free(offsets_by_id);
#ifdef ___PREDICT_ENABLED___
  if (predicts) free(predicts);
#endif
  fclose(fp);
  pthread_mutex_unlock(&g_mutex_);
  return false;
}

void UserDict::write_back() {
  // XXX write back is only allowed from close_dict due to thread-safe sake
  if (state_ == USER_DICT_NONE || state_ == USER_DICT_SYNC)
    return;
  int fd = open(dict_file_, O_WRONLY);
  if (fd == -1)
    return;
  switch (state_) {
    case USER_DICT_DEFRAGMENTED:
      write_back_all(fd);
      break;
    case USER_DICT_LEMMA_DIRTY:
      write_back_lemma(fd);
      break;
    case USER_DICT_OFFSET_DIRTY:
      write_back_offset(fd);
      break;
    case USER_DICT_SCORE_DIRTY:
      write_back_score(fd);
      break;
#ifdef ___SYNC_ENABLED___
    case USER_DICT_SYNC_DIRTY:
      write_back_sync(fd);
      break;
#endif
    default:
      break;
  }
  // It seems truncate is not need on Linux, Windows except Mac
  // I am doing it here anyway for safety.
  off_t cur = lseek(fd, 0, SEEK_CUR);
  int ftruncate_ret = 0;
  ftruncate_ret = ftruncate(fd, cur);
  close(fd);
  state_ = USER_DICT_SYNC;
}

#ifdef ___SYNC_ENABLED___
void UserDict::write_back_sync(int fd) {
  int err = lseek(fd, 4 + dict_info_.lemma_size
                  + (dict_info_.lemma_count << 3)
#ifdef ___PREDICT_ENABLED___
                  + (dict_info_.lemma_count << 2)
#endif
                  , SEEK_SET);
  if (err == -1)
    return;
  ssize_t write_ret = 0;
  write_ret = write(fd, (const char *)syncs_, dict_info_.sync_count << 2);
  write_ret = write(fd, (const char *)&dict_info_, sizeof(dict_info_));
}
#endif

void UserDict::write_back_offset(int fd) {
  int err = lseek(fd, 4 + dict_info_.lemma_size, SEEK_SET);
  if (err == -1)
    return;
  ssize_t write_ret = 0;
  write_ret = write(fd, (const char *)offsets_, dict_info_.lemma_count << 2);
#ifdef ___PREDICT_ENABLED___
  write_ret = write(fd, (const char *)predicts_, dict_info_.lemma_count << 2);
#endif
  write_ret = write(fd, (const char *)scores_, dict_info_.lemma_count << 2);
#ifdef ___SYNC_ENABLED___
  write_ret = write(fd, (const char *)syncs_, dict_info_.sync_count << 2);
#endif
  write_ret = write(fd, (const char *)&dict_info_, sizeof(dict_info_));
}

void UserDict::write_back_score(int fd) {
  int err = lseek(fd, 4 + dict_info_.lemma_size
                  + (dict_info_.lemma_count << 2)
#ifdef ___PREDICT_ENABLED___
                  + (dict_info_.lemma_count << 2)
#endif
                  , SEEK_SET);
  if (err == -1)
    return;
  ssize_t write_ret = 0;
  write_ret = write(fd, (const char *)scores_, dict_info_.lemma_count << 2);
#ifdef ___SYNC_ENABLED___
  write_ret = write(fd, (const char *)syncs_, dict_info_.sync_count << 2);
#endif
  write_ret = write(fd, (const char *)&dict_info_, sizeof(dict_info_));
}

void UserDict::write_back_lemma(int fd) {
  int err = lseek(fd, 4, SEEK_SET);
  if (err == -1)
    return;
  // New lemmas are always appended, no need to write whole lemma block
  unsigned need_write = kUserDictPreAlloc *
      (2 + (kUserDictAverageNchar << 2)) - lemma_size_left_;
  err = lseek(fd, dict_info_.lemma_size - need_write, SEEK_CUR);
  if (err == -1)
    return;
  ssize_t write_ret = 0;
  write_ret = write(fd, (const char *)lemmas_ + dict_info_.lemma_size - need_write, need_write);

  write_ret = write(fd, (const char *)offsets_,  dict_info_.lemma_count << 2);
#ifdef ___PREDICT_ENABLED___
  write_ret = write(fd, (const char *)predicts_,  dict_info_.lemma_count << 2);
#endif
  write_ret = write(fd, (const char *)scores_, dict_info_.lemma_count << 2);
#ifdef ___SYNC_ENABLED___
  write_ret = write(fd, (const char *)syncs_, dict_info_.sync_count << 2);
#endif
  write_ret = write(fd, (const char *)&dict_info_, sizeof(dict_info_));
}

void UserDict::write_back_all(int fd) {
  // XXX lemma_size is handled differently in writeall
  // and writelemma. I update lemma_size and lemma_count in different
  // places for these two cases. Should fix it to make it consistent.
  int err = lseek(fd, 4, SEEK_SET);
  if (err == -1)
    return;
  ssize_t write_ret = 0;
  write_ret = write(fd, (const char *)lemmas_, dict_info_.lemma_size);
  write_ret = write(fd, (const char *)offsets_, dict_info_.lemma_count << 2);
#ifdef ___PREDICT_ENABLED___
  write_ret = write(fd, (const char *)predicts_, dict_info_.lemma_count << 2);
#endif
  write_ret = write(fd, (const char *)scores_, dict_info_.lemma_count << 2);
#ifdef ___SYNC_ENABLED___
  write_ret = write(fd, (const char *)syncs_, dict_info_.sync_count << 2);
#endif
  write_ret = write(fd, (const char *)&dict_info_, sizeof(dict_info_));
}

#ifdef ___CACHE_ENABLED___
bool UserDict::load_cache(UserDictSearchable *searchable,
                          uint32 *offset, uint32 *length) {
  UserDictCache *cache = &caches_[searchable->splids_len - 1];
  if (cache->head == cache->tail)
    return false;

  uint16 j, sig_len = kMaxLemmaSize / 4;
  uint16 i = cache->head;
  while (1) {
    j = 0;
    for (; j < sig_len; j++) {
      if (cache->signatures[i][j] != searchable->signature[j])
        break;
    }
    if (j < sig_len) {
      i++;
      if (i >= kUserDictCacheSize)
        i -= kUserDictCacheSize;
      if (i == cache->tail)
        break;
      continue;
    }
    *offset = cache->offsets[i];
    *length = cache->lengths[i];
    return true;
  }
  return false;
}

void UserDict::save_cache(UserDictSearchable *searchable,
                          uint32 offset, uint32 length) {
  UserDictCache *cache = &caches_[searchable->splids_len - 1];
  uint16 next = cache->tail;

  cache->offsets[next] = offset;
  cache->lengths[next] = length;
  uint16 sig_len = kMaxLemmaSize / 4;
  uint16 j = 0;
  for (; j < sig_len; j++) {
    cache->signatures[next][j] = searchable->signature[j];
  }

  if (++next >= kUserDictCacheSize) {
    next -= kUserDictCacheSize;
  }
  if (next == cache->head) {
    cache->head++;
    if (cache->head >= kUserDictCacheSize) {
      cache->head -= kUserDictCacheSize;
    }
  }
  cache->tail = next;
}

void UserDict::reset_cache() {
  memset(caches_, 0, sizeof(caches_));
}

bool UserDict::load_miss_cache(UserDictSearchable *searchable) {
  UserDictMissCache *cache = &miss_caches_[searchable->splids_len - 1];
  if (cache->head == cache->tail)
    return false;

  uint16 j, sig_len = kMaxLemmaSize / 4;
  uint16 i = cache->head;
  while (1) {
    j = 0;
    for (; j < sig_len; j++) {
      if (cache->signatures[i][j] != searchable->signature[j])
        break;
    }
    if (j < sig_len) {
      i++;
      if (i >= kUserDictMissCacheSize)
        i -= kUserDictMissCacheSize;
      if (i == cache->tail)
        break;
      continue;
    }
    return true;
  }
  return false;
}

void UserDict::save_miss_cache(UserDictSearchable *searchable) {
  UserDictMissCache *cache = &miss_caches_[searchable->splids_len - 1];
  uint16 next = cache->tail;

  uint16 sig_len = kMaxLemmaSize / 4;
  uint16 j = 0;
  for (; j < sig_len; j++) {
    cache->signatures[next][j] = searchable->signature[j];
  }

  if (++next >= kUserDictMissCacheSize) {
    next -= kUserDictMissCacheSize;
  }
  if (next == cache->head) {
    cache->head++;
    if (cache->head >= kUserDictMissCacheSize) {
      cache->head -= kUserDictMissCacheSize;
    }
  }
  cache->tail = next;
}

void UserDict::reset_miss_cache() {
  memset(miss_caches_, 0, sizeof(miss_caches_));
}

void UserDict::cache_init() {
  reset_cache();
  reset_miss_cache();
}

bool UserDict::cache_hit(UserDictSearchable *searchable,
                         uint32 *offset, uint32 *length) {
  bool hit = load_miss_cache(searchable);
  if (hit) {
    *offset = 0;
    *length = 0;
    return true;
  }
  hit = load_cache(searchable, offset, length);
  if (hit) {
    return true;
  }
  return false;
}

void UserDict::cache_push(UserDictCacheType type,
                         UserDictSearchable *searchable,
                         uint32 offset, uint32 length) {
  switch (type) {
    case USER_DICT_MISS_CACHE:
      save_miss_cache(searchable);
      break;
    case USER_DICT_CACHE:
      save_cache(searchable, offset, length);
      break;
    default:
      break;
  }
}

#endif

void UserDict::defragment(void) {
#ifdef ___DEBUG_PERF___
  DEBUG_PERF_BEGIN;
#endif
  if (is_valid_state() == false)
    return;
  // Fixup offsets_, set REMOVE flag to lemma's flag if needed
  unsigned first_freed = 0;
  unsigned first_inuse = 0;
  while (first_freed < dict_info_.lemma_count) {
    // Find first freed offset
    while ((offsets_[first_freed] & kUserDictOffsetFlagRemove) == 0 &&
            first_freed < dict_info_.lemma_count) {
      first_freed++;
    }
    if (first_freed < dict_info_.lemma_count) {
      // Save REMOVE flag to lemma flag
      int off = offsets_[first_freed];
      set_lemma_flag(off, kUserDictLemmaFlagRemove);
    } else {
      break;
    }
    // Find first inuse offse after first_freed
    first_inuse = first_freed + 1;
    while ((offsets_[first_inuse] & kUserDictOffsetFlagRemove) &&
           (first_inuse < dict_info_.lemma_count)) {
      // Save REMOVE flag to lemma flag
      int off = offsets_[first_inuse];
      set_lemma_flag(off, kUserDictLemmaFlagRemove);
      first_inuse++;
    }
    if (first_inuse >= dict_info_.lemma_count) {
      break;
    }
    // Swap offsets_
    int tmp = offsets_[first_inuse];
    offsets_[first_inuse] = offsets_[first_freed];
    offsets_[first_freed] = tmp;
    // Move scores_, no need to swap
    tmp = scores_[first_inuse];
    scores_[first_inuse] = scores_[first_freed];
    scores_[first_freed] = tmp;
    // Swap ids_
    LemmaIdType tmpid = ids_[first_inuse];
    ids_[first_inuse] = ids_[first_freed];
    ids_[first_freed] = tmpid;
    // Go on
    first_freed++;
  }
#ifdef ___PREDICT_ENABLED___
  // Fixup predicts_
  first_freed = 0;
  first_inuse = 0;
  while (first_freed < dict_info_.lemma_count) {
    // Find first freed offset
    while ((predicts_[first_freed] & kUserDictOffsetFlagRemove) == 0 &&
            first_freed < dict_info_.lemma_count) {
      first_freed++;
    }
    if (first_freed >= dict_info_.lemma_count)
      break;
    // Find first inuse offse after first_freed
    first_inuse = first_freed + 1;
    while ((predicts_[first_inuse] & kUserDictOffsetFlagRemove)
           && (first_inuse < dict_info_.lemma_count)) {
      first_inuse++;
    }
    if (first_inuse >= dict_info_.lemma_count) {
      break;
    }
    // Swap offsets_
    int tmp = predicts_[first_inuse];
    predicts_[first_inuse] = predicts_[first_freed];
    predicts_[first_freed] = tmp;
    // Go on
    first_freed++;
  }
#endif
  dict_info_.lemma_count = first_freed;
  // Fixup lemmas_
  unsigned begin = 0;
  unsigned end = 0;
  unsigned dst = 0;
  int total_size = dict_info_.lemma_size + lemma_size_left_;
  int total_count = dict_info_.lemma_count + lemma_count_left_;
  unsigned real_size = total_size - lemma_size_left_;
  while (dst < real_size) {
    unsigned char flag = get_lemma_flag(dst);
    unsigned char nchr = get_lemma_nchar(dst);
    if ((flag & kUserDictLemmaFlagRemove) == 0) {
      dst += nchr * 4 + 2;
      continue;
    }
    break;
  }
  if (dst >= real_size)
    return;

  end = dst;
  while (end < real_size) {
    begin = end + get_lemma_nchar(end) * 4 + 2;
 repeat:
    // not used any more
    if (begin >= real_size)
      break;
    unsigned char flag = get_lemma_flag(begin);
    unsigned char nchr = get_lemma_nchar(begin);
    if (flag & kUserDictLemmaFlagRemove) {
      begin += nchr * 4 + 2;
      goto repeat;
    }
    end = begin + nchr * 4 + 2;
    while (end < real_size) {
      unsigned char eflag = get_lemma_flag(end);
      unsigned char enchr = get_lemma_nchar(end);
      if ((eflag & kUserDictLemmaFlagRemove) == 0) {
        end += enchr * 4 + 2;
        continue;
      }
      break;
    }
    memmove(lemmas_ + dst, lemmas_ + begin, end - begin);
    for (unsigned j = 0; j < dict_info_.lemma_count; j++) {
      if (offsets_[j] >= begin && offsets_[j] < end) {
        offsets_[j] -= (begin - dst);
        offsets_by_id_[ids_[j] - start_id_] = offsets_[j];
      }
#ifdef ___PREDICT_ENABLED___
      if (predicts_[j] >= begin && predicts_[j] < end) {
        predicts_[j] -= (begin - dst);
      }
#endif
    }
#ifdef ___SYNC_ENABLED___
    for (unsigned j = 0; j < dict_info_.sync_count; j++) {
      if (syncs_[j] >= begin && syncs_[j] < end) {
        syncs_[j] -= (begin - dst);
      }
    }
#endif
    dst += (end - begin);
  }

  dict_info_.free_count = 0;
  dict_info_.free_size = 0;
  dict_info_.lemma_size = dst;
  lemma_size_left_ = total_size - dict_info_.lemma_size;
  lemma_count_left_ = total_count - dict_info_.lemma_count;

  // XXX Without following code,
  // offsets_by_id_ is not reordered.
  // That's to say, all removed lemmas' ids are not collected back.
  // There may not be room for addition of new lemmas due to
  // offsests_by_id_ reason, although lemma_size_left_ is fixed.
  // By default, we do want defrag as fast as possible, because
  // during defrag procedure, other peers can not write new lemmas
  // to user dictionary file.
  // XXX If write-back is invoked immediately after
  // this defragment, no need to fix up following in-mem data.
  for (uint32 i = 0; i < dict_info_.lemma_count; i++) {
    ids_[i] = start_id_ + i;
    offsets_by_id_[i] = offsets_[i];
  }

  state_ = USER_DICT_DEFRAGMENTED;

#ifdef ___DEBUG_PERF___
  DEBUG_PERF_END;
  LOGD_PERF("defragment");
#endif
}

#ifdef ___SYNC_ENABLED___
void UserDict::clear_sync_lemmas(unsigned int start, unsigned int end) {
  if (is_valid_state() == false)
    return;
  if (end > dict_info_.sync_count)
    end = dict_info_.sync_count;
  memmove(syncs_ + start, syncs_ + end, (dict_info_.sync_count - end) << 2);
  dict_info_.sync_count -= (end - start);
  if (state_ < USER_DICT_SYNC_DIRTY)
    state_ = USER_DICT_SYNC_DIRTY;
}

int UserDict::get_sync_count() {
  if (is_valid_state() == false)
    return 0;
  return dict_info_.sync_count;
}

LemmaIdType UserDict::put_lemma_no_sync(char16 lemma_str[], uint16 splids[],
                        uint16 lemma_len, uint16 count, uint64 lmt) {
  int again = 0;
 begin:
  LemmaIdType id;
  uint32 * syncs_bak = syncs_;
  syncs_ = NULL;
  id = _put_lemma(lemma_str, splids, lemma_len, count, lmt);
  syncs_ = syncs_bak;
  if (id == 0 && again == 0) {
    if ((dict_info_.limit_lemma_count > 0 &&
        dict_info_.lemma_count >= dict_info_.limit_lemma_count)
        || (dict_info_.limit_lemma_size > 0 &&
            dict_info_.lemma_size + (2 + (lemma_len << 2))
            > dict_info_.limit_lemma_size)) {
      // XXX Always reclaim and defrag in sync code path
      //     sync thread is background thread and ok with heavy work
      reclaim();
      defragment();
      flush_cache();
      again = 1;
      goto begin;
    }
  }
  return id;
}

int UserDict::put_lemmas_no_sync_from_utf16le_string(char16 * lemmas, int len) {
  int newly_added = 0;

  SpellingParser * spl_parser = new SpellingParser();
  if (!spl_parser) {
    return 0;
  }
#ifdef ___DEBUG_PERF___
  DEBUG_PERF_BEGIN;
#endif
  char16 *ptr = lemmas;

  // Extract pinyin,words,frequence,last_mod_time
  char16 * p = ptr, * py16 = ptr;
  char16 * hz16 = NULL;
  int py16_len = 0;
  uint16 splid[kMaxLemmaSize];
  unsigned int splid_len = 0;
  unsigned int hz16_len = 0;
  char16 * fr16 = NULL;
  int fr16_len = 0;

  while (p - ptr < len) {
    // Pinyin
    py16 = p;
    splid_len = 0;
    while (*p != 0x2c && (p - ptr) < len) {
      if (*p == 0x20)
        splid_len++;
      p++;
    }
    splid_len++;
    if (p - ptr == len)
      break;
    py16_len = p - py16;
    if (kMaxLemmaSize < splid_len) {
      break;
    }
    bool is_pre;
    unsigned int splidl = spl_parser->splstr16_to_idxs_f(
        py16, py16_len, splid, NULL, kMaxLemmaSize, is_pre);
    if (splidl != splid_len)
      break;
    // Phrase
    hz16 = ++p;
    while (*p != 0x2c && (p - ptr) < len) {
      p++;
    }
    hz16_len = p - hz16;
    if (hz16_len != splid_len)
      break;
    // Frequency
    fr16 = ++p;
    fr16_len = 0;
    while (*p != 0x2c && (p - ptr) < len) {
      p++;
    }
    fr16_len = p - fr16;
    uint32 intf = (uint32)utf16le_atoll(fr16, fr16_len);
    // Last modified time
    fr16 = ++p;
    fr16_len = 0;
    while (*p != 0x3b && (p - ptr) < len) {
      p++;
    }
    fr16_len = p - fr16;
    uint64 last_mod = utf16le_atoll(fr16, fr16_len);

    put_lemma_no_sync(hz16, splid, splid_len, intf, last_mod);
    newly_added++;

    p++;
  }

#ifdef ___DEBUG_PERF___
  DEBUG_PERF_END;
  LOGD_PERF("put_lemmas_no_sync_from_utf16le_string");
#endif
  return newly_added;
}

int UserDict::get_sync_lemmas_in_utf16le_string_from_beginning(
    char16 * str, int size, int * count) {
  int len = 0;
  *count = 0;

  int left_len = size;

  if (is_valid_state() == false)
    return len;

  SpellingTrie * spl_trie = &SpellingTrie::get_instance();
  if (!spl_trie) {
    return 0;
  }

  uint32 i;
  for (i = 0; i < dict_info_.sync_count; i++) {
    int offset = syncs_[i];
    uint32 nchar = get_lemma_nchar(offset);
    uint16 *spl = get_lemma_spell_ids(offset);
    uint16 *wrd = get_lemma_word(offset);
    int score = _get_lemma_score(wrd, spl, nchar);

    static char score_temp[32], *pscore_temp = score_temp;
    static char16 temp[256], *ptemp = temp;

    pscore_temp = score_temp;
    ptemp = temp;

    uint32 j;
    // Add pinyin
    for (j = 0; j < nchar; j++) {
      int ret_len = spl_trie->get_spelling_str16(
          spl[j], ptemp, temp + sizeof(temp) - ptemp);
      if (ret_len <= 0)
        break;
      ptemp += ret_len;
      if (ptemp < temp + sizeof(temp) - 1) {
        *(ptemp++) = ' ';
      } else {
        j = 0;
        break;
      }
    }
    if (j < nchar) {
      continue;
    }
    ptemp--;
    if (ptemp < temp + sizeof(temp) - 1) {
      *(ptemp++) = ',';
    } else {
      continue;
    }
    // Add phrase
    for (j = 0; j < nchar; j++) {
      if (ptemp < temp + sizeof(temp) - 1) {
        *(ptemp++) = wrd[j];
      } else {
        break;
      }
    }
    if (j < nchar) {
      continue;
    }
    if (ptemp < temp + sizeof(temp) - 1) {
      *(ptemp++) = ',';
    } else {
      continue;
    }
    // Add frequency
    uint32 intf = extract_score_freq(score);
    int ret_len = utf16le_lltoa(intf, ptemp, temp + sizeof(temp) - ptemp);
    if (ret_len <= 0)
      continue;
    ptemp += ret_len;
    if (ptemp < temp + sizeof(temp) - 1) {
      *(ptemp++) = ',';
    } else {
      continue;
    }
    // Add last modified time
    uint64 last_mod = extract_score_lmt(score);
    ret_len = utf16le_lltoa(last_mod, ptemp, temp + sizeof(temp) - ptemp);
    if (ret_len <= 0)
      continue;
    ptemp += ret_len;
    if (ptemp < temp + sizeof(temp) - 1) {
      *(ptemp++) = ';';
    } else {
      continue;
    }

    // Write to string
    int need_len = ptemp - temp;
    if (need_len > left_len)
      break;
    memcpy(str + len, temp, need_len * 2);
    left_len -= need_len;

    len += need_len;
    (*count)++;
  }

  if (len > 0) {
    if (state_ < USER_DICT_SYNC_DIRTY)
      state_ = USER_DICT_SYNC_DIRTY;
  }
  return len;
}

#endif

bool UserDict::state(UserDictStat * stat) {
  if (is_valid_state() == false)
    return false;
  if (!stat)
    return false;
  stat->version = version_;
  stat->file_name = dict_file_;
  stat->load_time.tv_sec = load_time_.tv_sec;
  stat->load_time.tv_usec = load_time_.tv_usec;
  pthread_mutex_lock(&g_mutex_);
  stat->last_update.tv_sec = g_last_update_.tv_sec;
  stat->last_update.tv_usec = g_last_update_.tv_usec;
  pthread_mutex_unlock(&g_mutex_);
  stat->disk_size = get_dict_file_size(&dict_info_);
  stat->lemma_count = dict_info_.lemma_count;
  stat->lemma_size = dict_info_.lemma_size;
  stat->delete_count = dict_info_.free_count;
  stat->delete_size = dict_info_.free_size;
#ifdef ___SYNC_ENABLED___
  stat->sync_count = dict_info_.sync_count;
#endif
  stat->limit_lemma_count = dict_info_.limit_lemma_count;
  stat->limit_lemma_size = dict_info_.limit_lemma_size;
  stat->reclaim_ratio = dict_info_.reclaim_ratio;
  return true;
}

void UserDict::set_limit(uint32 max_lemma_count,
                         uint32 max_lemma_size, uint32 reclaim_ratio) {
  dict_info_.limit_lemma_count = max_lemma_count;
  dict_info_.limit_lemma_size = max_lemma_size;
  if (reclaim_ratio > 100)
    reclaim_ratio = 100;
  dict_info_.reclaim_ratio = reclaim_ratio;
}

void UserDict::reclaim() {
  if (is_valid_state() == false)
    return;

  switch (dict_info_.reclaim_ratio) {
    case 0:
      return;
    case 100:
      // TODO: CLEAR to be implemented
      assert(false);
      return;
    default:
      break;
  }

  // XXX Reclaim is only based on count, not size
  uint32 count = dict_info_.lemma_count;
  int rc = count * dict_info_.reclaim_ratio / 100;

  UserDictScoreOffsetPair * score_offset_pairs = NULL;
  score_offset_pairs = (UserDictScoreOffsetPair *)malloc(
      sizeof(UserDictScoreOffsetPair) * rc);
  if (score_offset_pairs == NULL) {
    return;
  }

  for (int i = 0; i < rc; i++) {
    int s = scores_[i];
    score_offset_pairs[i].score = s;
    score_offset_pairs[i].offset_index = i;
  }

  for (int i = (rc + 1) / 2; i >= 0; i--)
    shift_down(score_offset_pairs, i, rc);

  for (uint32 i = rc; i < dict_info_.lemma_count; i++) {
    int s = scores_[i];
    if (s < score_offset_pairs[0].score) {
      score_offset_pairs[0].score = s;
      score_offset_pairs[0].offset_index = i;
      shift_down(score_offset_pairs, 0, rc);
    }
  }

  for (int i = 0; i < rc; i++) {
    int off = score_offset_pairs[i].offset_index;
    remove_lemma_by_offset_index(off);
  }
  if (rc > 0) {
    if (state_ < USER_DICT_OFFSET_DIRTY)
      state_ = USER_DICT_OFFSET_DIRTY;
  }

  free(score_offset_pairs);
}

inline void UserDict::swap(UserDictScoreOffsetPair * sop, int i, int j) {
  int s = sop[i].score;
  int p = sop[i].offset_index;
  sop[i].score = sop[j].score;
  sop[i].offset_index = sop[j].offset_index;
  sop[j].score = s;
  sop[j].offset_index = p;
}

void UserDict::shift_down(UserDictScoreOffsetPair * sop, int i, int n) {
  int par = i;
  while (par < n) {
    int left = par * 2 + 1;
    int right = left + 1;
    if (left >= n && right >= n)
      break;
    if (right >= n) {
      if (sop[left].score > sop[par].score) {
        swap(sop, left, par);
        par = left;
        continue;
      }
    } else if (sop[left].score > sop[right].score &&
               sop[left].score > sop[par].score) {
      swap(sop, left, par);
      par = left;
      continue;
    } else if (sop[right].score > sop[left].score &&
               sop[right].score > sop[par].score) {
      swap(sop, right, par);
      par = right;
      continue;
    }
    break;
  }
}

LemmaIdType UserDict::put_lemma(char16 lemma_str[], uint16 splids[],
                                uint16 lemma_len, uint16 count) {
  return _put_lemma(lemma_str, splids, lemma_len, count, time(NULL));
}

LemmaIdType UserDict::_put_lemma(char16 lemma_str[], uint16 splids[],
                                 uint16 lemma_len, uint16 count, uint64 lmt) {
#ifdef ___DEBUG_PERF___
  DEBUG_PERF_BEGIN;
#endif
  if (is_valid_state() == false)
    return 0;
  int32 off = locate_in_offsets(lemma_str, splids, lemma_len);
  if (off != -1) {
    int delta_score = count - scores_[off];
    dict_info_.total_nfreq += delta_score;
    scores_[off] = build_score(lmt, count);
    if (state_ < USER_DICT_SCORE_DIRTY)
      state_ = USER_DICT_SCORE_DIRTY;
#ifdef ___DEBUG_PERF___
    DEBUG_PERF_END;
    LOGD_PERF("_put_lemma(update)");
#endif
    return ids_[off];
  } else {
    if ((dict_info_.limit_lemma_count > 0 &&
        dict_info_.lemma_count >= dict_info_.limit_lemma_count)
        || (dict_info_.limit_lemma_size > 0 &&
            dict_info_.lemma_size + (2 + (lemma_len << 2))
            > dict_info_.limit_lemma_size)) {
      // XXX Don't defragment here, it's too time-consuming.
      return 0;
    }
    int flushed = 0;
    if (lemma_count_left_ == 0 ||
        lemma_size_left_ < (unsigned)(2 + (lemma_len << 2))) {

      // XXX When there is no space for new lemma, we flush to disk
      // flush_cache() may be called by upper user
      // and better place shoule be found instead of here
      flush_cache();
      flushed = 1;
      // Or simply return and do nothing
      // return 0;
    }
#ifdef ___DEBUG_PERF___
    DEBUG_PERF_END;
    LOGD_PERF(flushed ? "_put_lemma(flush+add)" : "_put_lemma(add)");
#endif
    LemmaIdType id = append_a_lemma(lemma_str, splids, lemma_len, count, lmt);
#ifdef ___SYNC_ENABLED___
    if (syncs_ && id != 0) {
      queue_lemma_for_sync(id);
    }
#endif
    return id;
  }
  return 0;
}

#ifdef ___SYNC_ENABLED___
void UserDict::queue_lemma_for_sync(LemmaIdType id) {
  if (dict_info_.sync_count < sync_count_size_) {
    syncs_[dict_info_.sync_count++] = offsets_by_id_[id - start_id_];
  } else {
    uint32 * syncs = (uint32*)realloc(
        syncs_, (sync_count_size_ + kUserDictPreAlloc) << 2);
    if (syncs) {
      sync_count_size_ += kUserDictPreAlloc;
      syncs_ = syncs;
      syncs_[dict_info_.sync_count++] = offsets_by_id_[id - start_id_];
    }
  }
}
#endif

LemmaIdType UserDict::update_lemma(LemmaIdType lemma_id, int16 delta_count,
                                   bool selected) {
#ifdef ___DEBUG_PERF___
  DEBUG_PERF_BEGIN;
#endif
  if (is_valid_state() == false)
    return 0;
  if (is_valid_lemma_id(lemma_id) == false)
    return 0;
  uint32 offset = offsets_by_id_[lemma_id - start_id_];
  uint8 lemma_len = get_lemma_nchar(offset);
  char16 * lemma_str = get_lemma_word(offset);
  uint16 * splids = get_lemma_spell_ids(offset);

  int32 off = locate_in_offsets(lemma_str, splids, lemma_len);
  if (off != -1) {
    int score = scores_[off];
    int count = extract_score_freq(score);
    uint64 lmt = extract_score_lmt(score);
    if (count + delta_count > kUserDictMaxFrequency ||
        count + delta_count < count) {
      delta_count = kUserDictMaxFrequency - count;
    }
    count += delta_count;
    dict_info_.total_nfreq += delta_count;
    if (selected) {
      lmt = time(NULL);
    }
    scores_[off] = build_score(lmt, count);
    if (state_ < USER_DICT_SCORE_DIRTY)
      state_ = USER_DICT_SCORE_DIRTY;
#ifdef ___DEBUG_PERF___
    DEBUG_PERF_END;
    LOGD_PERF("update_lemma");
#endif
#ifdef ___SYNC_ENABLED___
    queue_lemma_for_sync(ids_[off]);
#endif
    return ids_[off];
  }
  return 0;
}

unsigned UserDict::get_total_lemma_count() {
  return dict_info_.total_nfreq;
}

void UserDict::set_total_lemma_count_of_others(unsigned count) {
  total_other_nfreq_ = count;
}

LemmaIdType UserDict::append_a_lemma(char16 lemma_str[], uint16 splids[],
                                   uint16 lemma_len, uint16 count, uint64 lmt) {
  LemmaIdType id = get_max_lemma_id() + 1;
  unsigned offset = dict_info_.lemma_size;
  if (offset > kUserDictOffsetMask)
    return 0;

  lemmas_[offset] = 0;
  lemmas_[offset + 1] = (uint8)lemma_len;
  for (unsigned i = 0; i < lemma_len; i++) {
    *((uint16*)&lemmas_[offset + 2 + (i << 1)]) = splids[i];
    *((char16*)&lemmas_[offset + 2 + (lemma_len << 1) + (i << 1)])
        = lemma_str[i];
  }
  uint32 off = dict_info_.lemma_count;
  offsets_[off] = offset;
  scores_[off] = build_score(lmt, count);
  ids_[off] = id;
#ifdef ___PREDICT_ENABLED___
  predicts_[off] = offset;
#endif

  offsets_by_id_[id - start_id_] = offset;

  dict_info_.lemma_count++;
  dict_info_.lemma_size += (2 + (lemma_len << 2));
  lemma_count_left_--;
  lemma_size_left_ -= (2 + (lemma_len << 2));

  // Sort

  UserDictSearchable searchable;
  prepare_locate(&searchable, splids, lemma_len);

  unsigned i = 0;
  while (i < off) {
    offset = offsets_[i];
    uint32 nchar = get_lemma_nchar(offset);
    uint16 * spl = get_lemma_spell_ids(offset);

    if (0 <= fuzzy_compare_spell_id(spl, nchar, &searchable))
      break;
    i++;
  }
  if (i != off) {
    uint32 temp = offsets_[off];
    memmove(offsets_ + i + 1, offsets_ + i, (off - i) << 2);
    offsets_[i] = temp;

    temp = scores_[off];
    memmove(scores_ + i + 1, scores_ + i, (off - i) << 2);
    scores_[i] = temp;

    temp = ids_[off];
    memmove(ids_ + i + 1, ids_ + i, (off - i) << 2);
    ids_[i] = temp;
  }

#ifdef ___PREDICT_ENABLED___
  uint32 j = 0;
  uint16 * words_new = get_lemma_word(predicts_[off]);
  j = locate_where_to_insert_in_predicts(words_new, lemma_len);
  if (j != off) {
    uint32 temp = predicts_[off];
    memmove(predicts_ + j + 1, predicts_ + j, (off - j) << 2);
    predicts_[j] = temp;
  }
#endif

  if (state_ < USER_DICT_LEMMA_DIRTY)
    state_ = USER_DICT_LEMMA_DIRTY;

#ifdef ___CACHE_ENABLED___
  cache_init();
#endif

  dict_info_.total_nfreq += count;
  return id;
}
}
