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

#ifndef PINYINIME_INCLUDE_USERDICT_H__
#define PINYINIME_INCLUDE_USERDICT_H__

#define ___CACHE_ENABLED___
#define ___SYNC_ENABLED___
#define ___PREDICT_ENABLED___

// Debug performance for operations
// #define ___DEBUG_PERF___

#include "atomdictbase.h"

namespace ime_pinyin {

class UserDict : public AtomDictBase {
 public:
  UserDict();
  ~UserDict();

  bool load_dict(const char *file_name, LemmaIdType start_id,
                 LemmaIdType end_id);

  bool close_dict();

  unsigned number_of_lemmas();

  void reset_milestones(uint16 from_step, MileStoneHandle from_handle);

  MileStoneHandle extend_dict(MileStoneHandle from_handle,
                              const DictExtPara *dep, LmaPsbItem *lpi_items,
                              unsigned lpi_max, unsigned *lpi_num);

  unsigned get_lpis(const uint16 *splid_str, uint16 splid_str_len,
                  LmaPsbItem *lpi_items, unsigned lpi_max);

  uint16 get_lemma_str(LemmaIdType id_lemma, char16* str_buf,
                       uint16 str_max);

  uint16 get_lemma_splids(LemmaIdType id_lemma, uint16 *splids,
                          uint16 splids_max, bool arg_valid);

  unsigned predict(const char16 last_hzs[], uint16 hzs_len,
                 NPredictItem *npre_items, unsigned npre_max,
                 unsigned b4_used);

  // Full spelling ids are required
  LemmaIdType put_lemma(char16 lemma_str[], uint16 splids[],
                        uint16 lemma_len, uint16 count);

  LemmaIdType update_lemma(LemmaIdType lemma_id, int16 delta_count,
                           bool selected);

  LemmaIdType get_lemma_id(char16 lemma_str[], uint16 splids[],
                           uint16 lemma_len);

  LmaScoreType get_lemma_score(LemmaIdType lemma_id);

  LmaScoreType get_lemma_score(char16 lemma_str[], uint16 splids[],
                        uint16 lemma_len);

  bool remove_lemma(LemmaIdType lemma_id);

  unsigned get_total_lemma_count();
  void set_total_lemma_count_of_others(unsigned count);

  void flush_cache();

  void set_limit(uint32 max_lemma_count, uint32 max_lemma_size,
                 uint32 reclaim_ratio);

  void reclaim();

  void defragment();

#ifdef ___SYNC_ENABLED___
  void clear_sync_lemmas(unsigned int start, unsigned int end);

  int get_sync_count();

  LemmaIdType put_lemma_no_sync(char16 lemma_str[], uint16 splids[],
                        uint16 lemma_len, uint16 count, uint64 lmt);
   /**
    * Add lemmas encoded in UTF-16LE into dictionary without adding sync flag.
    *
    * @param lemmas in format of 'wo men,WM,0.32;da jia,DJ,0.12'
    * @param len length of lemmas string in UTF-16LE
    * @return newly added lemma count
    */
  int put_lemmas_no_sync_from_utf16le_string(char16 * lemmas, int len);

  /**
   * Get lemmas need sync to a UTF-16LE string of above format.
   * Note: input buffer (str) must not be too small. If str is too small to 
   *       contain single one lemma, there might be a dead loop.
   *
   * @param str buffer to write lemmas
   * @param size buffer size in UTF-16LE
   * @param count output value of lemma returned
   * @return UTF-16LE string length
   */
  int get_sync_lemmas_in_utf16le_string_from_beginning(
      char16 * str, int size, int * count);

#endif

  struct UserDictStat {
    uint32 version;
    const char * file_name;
    struct timeval load_time;
    struct timeval last_update;
    uint32 disk_size;
    uint32 lemma_count;
    uint32 lemma_size;
    uint32 delete_count;
    uint32 delete_size;
#ifdef ___SYNC_ENABLED___
    uint32 sync_count;
#endif
    uint32 reclaim_ratio;
    uint32 limit_lemma_count;
    uint32 limit_lemma_size;
  };

  bool state(UserDictStat * stat);

 private:
  uint32 total_other_nfreq_;
  struct timeval load_time_;
  LemmaIdType start_id_;
  uint32 version_;
  uint8 * lemmas_;

  // In-Memory-Only flag for each lemma
  static const uint8 kUserDictLemmaFlagRemove = 1;
  // Inuse lemmas' offset
  uint32 * offsets_;
  // Highest bit in offset tells whether corresponding lemma is removed
  static const uint32 kUserDictOffsetFlagRemove = (1 << 31);
  // Maximum possible for the offset
  static const uint32 kUserDictOffsetMask = ~(kUserDictOffsetFlagRemove);
  // Bit width for last modified time, from 1 to 16
  static const uint32 kUserDictLMTBitWidth = 16;
  // Granularity for last modified time in second
  static const uint32 kUserDictLMTGranularity = 60 * 60 * 24 * 7;
  // Maximum frequency count
  static const uint16 kUserDictMaxFrequency = 0xFFFF;

#define COARSE_UTC(year, month, day, hour, minute, second) \
  ( \
    (year - 1970) * 365 * 24 * 60 * 60 + \
    (month - 1) * 30 * 24 * 60 * 60 + \
    (day - 1) * 24 * 60 * 60 + \
    (hour - 0) * 60 * 60 + \
    (minute - 0) * 60 + \
    (second - 0) \
  )
  static const uint64 kUserDictLMTSince = COARSE_UTC(2009, 1, 1, 0, 0, 0);

  // Correspond to offsets_
  uint32 * scores_;
  // Following two fields are only valid in memory
  uint32 * ids_;
#ifdef ___PREDICT_ENABLED___
  uint32 * predicts_;
#endif
#ifdef ___SYNC_ENABLED___
  uint32 * syncs_;
  unsigned sync_count_size_;
#endif
  uint32 * offsets_by_id_;

  unsigned lemma_count_left_;
  unsigned lemma_size_left_;

  const char * dict_file_;

  // Be sure size is 4xN
  struct UserDictInfo {
    // When limitation reached, how much percentage will be reclaimed (1 ~ 100)
    uint32 reclaim_ratio;
    // maximum lemma count, 0 means no limitation
    uint32 limit_lemma_count;
    // Maximum lemma size, it's different from
    // whole disk file size or in-mem dict size
    // 0 means no limitation
    uint32 limit_lemma_size;
    // Total lemma count including deleted and inuse
    // Also indicate offsets_ size
    uint32 lemma_count;
    // Total size of lemmas including used and freed
    uint32 lemma_size;
    // Freed lemma count
    uint32 free_count;
    // Freed lemma size in byte
    uint32 free_size;
#ifdef ___SYNC_ENABLED___
    uint32 sync_count;
#endif
    int32 total_nfreq;
  } dict_info_;

  static const uint32 kUserDictVersion = 0x0ABCDEF0;

  static const uint32 kUserDictPreAlloc = 32;
  static const uint32 kUserDictAverageNchar = 8;

  enum UserDictState {
    // Keep in order
    USER_DICT_NONE = 0,
    USER_DICT_SYNC,
#ifdef ___SYNC_ENABLED___
    USER_DICT_SYNC_DIRTY,
#endif
    USER_DICT_SCORE_DIRTY,
    USER_DICT_OFFSET_DIRTY,
    USER_DICT_LEMMA_DIRTY,

    USER_DICT_DEFRAGMENTED,
  } state_;

  struct UserDictSearchable {
    uint16 splids_len;
    uint16 splid_start[kMaxLemmaSize];
    uint16 splid_count[kMaxLemmaSize];
    // Compact inital letters for both FuzzyCompareSpellId and cache system
    uint32 signature[kMaxLemmaSize / 4];
  };

#ifdef ___CACHE_ENABLED___
  enum UserDictCacheType {
    USER_DICT_CACHE,
    USER_DICT_MISS_CACHE,
  };

  static const int kUserDictCacheSize = 4;
  static const int kUserDictMissCacheSize = kMaxLemmaSize - 1;

  struct UserDictMissCache {
    uint32 signatures[kUserDictMissCacheSize][kMaxLemmaSize / 4];
    uint16 head, tail;
  } miss_caches_[kMaxLemmaSize];

  struct UserDictCache {
    uint32 signatures[kUserDictCacheSize][kMaxLemmaSize / 4];
    uint32 offsets[kUserDictCacheSize];
    uint32 lengths[kUserDictCacheSize];
    // Ring buffer
    uint16 head, tail;
  } caches_[kMaxLemmaSize];

  void cache_init();

  void cache_push(UserDictCacheType type,
                 UserDictSearchable *searchable,
                 uint32 offset, uint32 length);

  bool cache_hit(UserDictSearchable *searchable,
                 uint32 *offset, uint32 *length);

  bool load_cache(UserDictSearchable *searchable,
                  uint32 *offset, uint32 *length);

  void save_cache(UserDictSearchable *searchable,
                  uint32 offset, uint32 length);

  void reset_cache();

  bool load_miss_cache(UserDictSearchable *searchable);

  void save_miss_cache(UserDictSearchable *searchable);

  void reset_miss_cache();
#endif

  LmaScoreType translate_score(int f);

  int extract_score_freq(int raw_score);

  uint64 extract_score_lmt(int raw_score);

  inline int build_score(uint64 lmt, int freq);

  inline int64 utf16le_atoll(uint16 *s, int len);

  inline int utf16le_lltoa(int64 v, uint16 *s, int size);

  LemmaIdType _put_lemma(char16 lemma_str[], uint16 splids[],
                        uint16 lemma_len, uint16 count, uint64 lmt);

  unsigned _get_lpis(const uint16 *splid_str, uint16 splid_str_len,
                   LmaPsbItem *lpi_items, unsigned lpi_max, bool * need_extend);

  int _get_lemma_score(char16 lemma_str[], uint16 splids[], uint16 lemma_len);

  int _get_lemma_score(LemmaIdType lemma_id);

  int is_fuzzy_prefix_spell_id(const uint16 * id1, uint16 len1,
                               const UserDictSearchable *searchable);

  bool is_prefix_spell_id(const uint16 * fullids,
                          uint16 fulllen, const UserDictSearchable *searchable);

  uint32 get_dict_file_size(UserDictInfo * info);

  bool reset(const char *file);

  bool validate(const char *file);

  bool load(const char *file, LemmaIdType start_id);

  bool is_valid_state();

  bool is_valid_lemma_id(LemmaIdType id);

  LemmaIdType get_max_lemma_id();

  void set_lemma_flag(uint32 offset, uint8 flag);

  char get_lemma_flag(uint32 offset);

  char get_lemma_nchar(uint32 offset);

  uint16 * get_lemma_spell_ids(uint32 offset);

  uint16 * get_lemma_word(uint32 offset);

  // Prepare searchable to fasten locate process
  void prepare_locate(UserDictSearchable *searchable,
                      const uint16 * splids, uint16 len);

  // Compare initial letters only
  int32 fuzzy_compare_spell_id(const uint16 * id1, uint16 len1,
                               const UserDictSearchable *searchable);

  // Compare exactly two spell ids
  // First argument must be a full id spell id
  bool equal_spell_id(const uint16 * fullids,
                      uint16 fulllen, const UserDictSearchable *searchable);

  // Find first item by initial letters
  int32 locate_first_in_offsets(const UserDictSearchable *searchable);

  LemmaIdType append_a_lemma(char16 lemma_str[], uint16 splids[],
                           uint16 lemma_len, uint16 count, uint64 lmt);

  // Check if a lemma is in dictionary
  int32 locate_in_offsets(char16 lemma_str[],
                          uint16 splid_str[], uint16 lemma_len);

  bool remove_lemma_by_offset_index(int offset_index);
#ifdef ___PREDICT_ENABLED___
  uint32 locate_where_to_insert_in_predicts(const uint16 * words,
                                            int lemma_len);

  int32 locate_first_in_predicts(const uint16 * words, int lemma_len);

  void remove_lemma_from_predict_list(uint32 offset);
#endif
#ifdef ___SYNC_ENABLED___
  void queue_lemma_for_sync(LemmaIdType id);

  void remove_lemma_from_sync_list(uint32 offset);

  void write_back_sync(int fd);
#endif
  void write_back_score(int fd);
  void write_back_offset(int fd);
  void write_back_lemma(int fd);
  void write_back_all(int fd);
  void write_back();

  struct UserDictScoreOffsetPair {
    int score;
    uint32 offset_index;
  };

  inline void swap(UserDictScoreOffsetPair * sop, int i, int j);

  void shift_down(UserDictScoreOffsetPair * sop, int i, int n);

  // On-disk format for each lemma
  // +-------------+
  // | Version (4) |
  // +-------------+
  // +-----------+-----------+--------------------+-------------------+
  // | Spare (1) | Nchar (1) | Splids (2 x Nchar) | Lemma (2 x Nchar) |
  // +-----------+-----------+--------------------+-------------------+
  // ...
  // +-----------------------+     +-------------+      <---Offset of offset
  // | Offset1 by_splids (4) | ... | OffsetN (4) |
  // +-----------------------+     +-------------+
#ifdef ___PREDICT_ENABLED___
  // +----------------------+     +-------------+
  // | Offset1 by_lemma (4) | ... | OffsetN (4) |
  // +----------------------+     +-------------+
#endif
  // +------------+     +------------+
  // | Score1 (4) | ... | ScoreN (4) |
  // +------------+     +------------+
#ifdef ___SYNC_ENABLED___
  // +-------------+     +-------------+
  // | NewAdd1 (4) | ... | NewAddN (4) |
  // +-------------+     +-------------+
#endif
  // +----------------+
  // | Dict Info (4x) |
  // +----------------+
};
}

#endif
