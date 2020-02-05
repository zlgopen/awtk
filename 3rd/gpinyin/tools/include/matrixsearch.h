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

#ifndef PINYINIME_ANDPY_INCLUDE_MATRIXSEARCH_H__
#define PINYINIME_ANDPY_INCLUDE_MATRIXSEARCH_H__

#include <stdlib.h>
#include "./atomdictbase.h"
#include "./dicttrie.h"
#include "./searchutility.h"
#include "./spellingtrie.h"
#include "./splparser.h"

namespace ime_pinyin {

static const unsigned kMaxRowNum = kMaxSearchSteps;

typedef struct {
  // MileStoneHandle objects for the system and user dictionaries.
  MileStoneHandle dict_handles[2];
  // From which DMI node. -1 means it's from root.
  PoolPosType dmi_fr;
  // The spelling id for the Pinyin string from the previous DMI to this node.
  // If it is a half id like Shengmu, the node pointed by dict_node is the first
  // node with this Shengmu,
  uint16 spl_id;
  // What's the level of the dict node. Level of root is 0, but root is never
  // recorded by dict_node.
  unsigned char dict_level:7;
  // If this node is for composing phrase, this bit is 1.
  unsigned char c_phrase:1;
  // Whether the spl_id is parsed with a split character at the end.
  unsigned char splid_end_split:1;
  // What's the length of the spelling string for this match, for the whole
  // word.
  unsigned char splstr_len:7;
  // Used to indicate whether all spelling ids from the root are full spelling
  // ids. This information is useful for keymapping mode(not finished). Because
  // in this mode, there is no clear boundaries, we prefer those results which
  // have full spelling ids.
  unsigned char all_full_id:1;
} DictMatchInfo, *PDictMatchInfo;

typedef struct MatrixNode {
  LemmaIdType id;
  float score;
  MatrixNode *from;
  // From which DMI node. Used to trace the spelling segmentation.
  PoolPosType dmi_fr;
  uint16 step;
} MatrixNode, *PMatrixNode;

typedef struct {
  // The MatrixNode position in the matrix pool
  PoolPosType mtrx_nd_pos;
  // The DictMatchInfo position in the DictMatchInfo pool.
  PoolPosType dmi_pos;
  uint16 mtrx_nd_num;
  uint16 dmi_num:15;
  // Used to indicate whether there are dmi nodes in this step with full
  // spelling id. This information is used to decide whether a substring of a
  // valid Pinyin should be extended.
  //
  // Example1: shoudao
  // When the last char 'o' is added, the parser will find "dao" is a valid
  // Pinyin, and because all dmi nodes at location 'd' (including those for
  // "shoud", and those for "d") have Shengmu id only, so it is not necessary
  // to extend "ao", otherwise the result may be "shoud ao", that is not
  // reasonable.
  //
  // Example2: hengao
  // When the last 'o' is added, the parser finds "gao" is a valid Pinyin.
  // Because some dmi nodes at 'g' has Shengmu ids (hen'g and g), but some dmi
  // nodes at 'g' has full ids ('heng'), so it is necessary to extend "ao", thus
  // "heng ao" can also be the result.
  //
  // Similarly, "ganga" is expanded to "gang a".
  //
  // For Pinyin string "xian", because "xian" is a valid Pinyin, because all dmi
  // nodes at 'x' only have Shengmu ids, the parser will not try "x ian" (and it
  // is not valid either). If the parser uses break in the loop, the result
  // always be "xian"; but if the parser uses continue in the loop, "xi an" will
  // also be tried. This behaviour can be set via the function
  // set_xi_an_switch().
  uint16 dmi_has_full_id:1;
  // Points to a MatrixNode of the current step to indicate which choice the
  // user selects.
  MatrixNode *mtrx_nd_fixed;
} MatrixRow, *PMatrixRow;

// When user inputs and selects candidates, the fixed lemma ids are stored in
// lma_id_ of class MatrixSearch, and fixed_lmas_ is used to indicate how many
// lemmas from the beginning are fixed. If user deletes Pinyin characters one
// by one from the end, these fixed lemmas can be unlocked one by one when
// necessary. Whenever user deletes a Chinese character and its spelling string
// in these fixed lemmas, all fixed lemmas will be merged together into a unit
// named ComposingPhrase with a lemma id kLemmaIdComposing, and this composing
// phrase will be the first lemma in the sentence. Because it contains some
// modified lemmas (by deleting a character), these merged lemmas are called
// sub lemmas (sublma), and each of them are represented individually, so that
// when user deletes Pinyin characters from the end, these sub lemmas can also
// be unlocked one by one.
typedef struct {
  uint16 spl_ids[kMaxRowNum];
  uint16 spl_start[kMaxRowNum];
  char16 chn_str[kMaxRowNum];       // Chinese string.
  uint16 sublma_start[kMaxRowNum];  // Counted in Chinese characters.
  unsigned sublma_num;
  uint16 length;                    // Counted in Chinese characters.
} ComposingPhrase, *TComposingPhrase;

class MatrixSearch {
 private:
  // If it is true, prediction list by string whose length is greater than 1
  // will be limited to a reasonable number.
  static const bool kPredictLimitGt1 = false;

  // If it is true, the engine will prefer long history based prediction,
  // for example, when user inputs "BeiJing", we prefer "DaXue", etc., which are
  // based on the two-character history.
  static const bool kPreferLongHistoryPredict = true;

  // If it is true, prediction will only be based on user dictionary. this flag
  // is for debug purpose.
  static const bool kOnlyUserDictPredict = false;

  // The maximum buffer to store LmaPsbItems.
  static const unsigned kMaxLmaPsbItems = 1450;

  // How many rows for each step.
  static const unsigned kMaxNodeARow = 5;

  // The maximum length of the sentence candidates counted in chinese
  // characters
  static const unsigned kMaxSentenceLength = 16;

  // The size of the matrix node pool.
  static const unsigned kMtrxNdPoolSize = 200;

  // The size of the DMI node pool.
  static const unsigned kDmiPoolSize = 800;

  // Used to indicate whether this object has been initialized.
  bool inited_;

  // Spelling trie.
  const SpellingTrie *spl_trie_;

  // Used to indicate this switcher status: when "xian" is parseed, should
  // "xi an" also be extended. Default is false.
  // These cases include: xia, xian, xiang, zhuan, jiang..., etc. The string
  // should be valid for a FULL spelling, or a combination of two spellings,
  // first of which is a FULL id too. So even it is true, "da" will never be
  // split into "d a", because "d" is not a full spelling id.
  bool xi_an_enabled_;

  // System dictionary.
  DictTrie* dict_trie_;

  // User dictionary.
  AtomDictBase* user_dict_;

  // Spelling parser.
  SpellingParser* spl_parser_;

  // The maximum allowed length of spelling string (such as a Pinyin string).
  unsigned max_sps_len_;

  // The maximum allowed length of a result Chinese string.
  unsigned max_hzs_len_;

  // Pinyin string. Max length: kMaxRowNum - 1
  char pys_[kMaxRowNum];

  // The length of the string that has been decoded successfully.
  unsigned pys_decoded_len_;

  // Shared buffer for multiple purposes.
  unsigned *share_buf_;

  MatrixNode *mtrx_nd_pool_;
  PoolPosType mtrx_nd_pool_used_;    // How many nodes used in the pool
  DictMatchInfo *dmi_pool_;
  PoolPosType dmi_pool_used_;        // How many items used in the pool

  MatrixRow *matrix_;                // The first row is for starting

  DictExtPara *dep_;                 // Parameter used to extend DMI nodes.

  NPredictItem *npre_items_;         // Used to do prediction
  unsigned npre_items_len_;

  // The starting positions and lemma ids for the full sentence candidate.
  unsigned lma_id_num_;
  uint16 lma_start_[kMaxRowNum];     // Counted in spelling ids.
  LemmaIdType lma_id_[kMaxRowNum];
  unsigned fixed_lmas_;

  // If fixed_lmas_ is bigger than i,  Element i is used to indicate whether
  // the i'th lemma id in lma_id_ is the first candidate for that step.
  // If all candidates are the first one for that step, the whole string can be
  // decoded by the engine automatically, so no need to add it to user
  // dictionary. (We are considering to add it to user dictionary in the
  // future).
  uint8 fixed_lmas_no1_[kMaxRowNum];

  // Composing phrase
  ComposingPhrase c_phrase_;

  // If dmi_c_phrase_ is true, the decoder will try to match the
  // composing phrase (And definitely it will match successfully). If it
  // is false, the decoder will try to match lemmas items in dictionaries.
  bool dmi_c_phrase_;

  // The starting positions and spelling ids for the first full sentence
  // candidate.
  unsigned spl_id_num_;                // Number of splling ids
  uint16 spl_start_[kMaxRowNum];     // Starting positions
  uint16 spl_id_[kMaxRowNum];        // Spelling ids
  // Used to remember the last fixed position, counted in Hanzi.
  unsigned fixed_hzs_;

  // Lemma Items with possibility score, two purposes:
  // 1. In Viterbi decoding, this buffer is used to get all possible candidates
  // for current step;
  // 2. When the search is done, this buffer is used to get candiates from the
  // first un-fixed step and show them to the user.
  LmaPsbItem lpi_items_[kMaxLmaPsbItems];
  unsigned lpi_total_;

  // Assign the pointers with NULL. The caller makes sure that all pointers are
  // not valid before calling it. This function only will be called in the
  // construction function and free_resource().
  void reset_pointers_to_null();

  bool alloc_resource();

  void free_resource();

  // Reset the search space totally.
  bool reset_search0();

  // Reset the search space from ch_pos step. For example, if the original
  // input Pinyin is "an", reset_search(1) will reset the search space to the
  // result of "a". If the given position is out of range, return false.
  // if clear_fixed_this_step is true, and the ch_pos step is a fixed step,
  // clear its fixed status. if clear_dmi_his_step is true, clear the DMI nodes.
  // If clear_mtrx_this_sTep is true, clear the mtrx nodes of this step.
  // The DMI nodes will be kept.
  //
  // Note: this function should not destroy content of pys_.
  bool reset_search(unsigned ch_pos, bool clear_fixed_this_step,
                    bool clear_dmi_this_step, bool clear_mtrx_this_step);

  // Delete a part of the content in pys_.
  void del_in_pys(unsigned start, unsigned len);

  // Delete a spelling id and its corresponding Chinese character, and merge
  // the fixed lemmas into the composing phrase.
  // del_spl_pos indicates which spelling id needs to be delete.
  // This function will update the lemma and spelling segmentation information.
  // The caller guarantees that fixed_lmas_ > 0 and del_spl_pos is within
  // the fixed lemmas.
  void merge_fixed_lmas(unsigned del_spl_pos);

  // Get spelling start posistions and ids. The result will be stored in
  // spl_id_num_, spl_start_[], spl_id_[].
  // fixed_hzs_ will be also assigned.
  void get_spl_start_id();

  // Get all lemma ids with match the given spelling id stream(shorter than the
  // maximum length of a word).
  // If pfullsent is not NULL, means the full sentence candidate may be the
  // same with the coming lemma string, if so, remove that lemma.
  // The result is sorted in descendant order by the frequency score.
  unsigned get_lpis(const uint16* splid_str, unsigned splid_str_len,
                  LmaPsbItem* lma_buf, unsigned max_lma_buf,
                  const char16 *pfullsent, bool sort_by_psb);

  uint16 get_lemma_str(LemmaIdType id_lemma, char16 *str_buf, uint16 str_max);

  uint16 get_lemma_splids(LemmaIdType id_lemma, uint16 *splids,
                          uint16 splids_max, bool arg_valid);


  // Extend a DMI node with a spelling id. ext_len is the length of the rows
  // to extend, actually, it is the size of the spelling string of splid.
  // return value can be 1 or 0.
  // 1 means a new DMI is filled in (dmi_pool_used_ is the next blank DMI in
  // the pool).
  // 0 means either the dmi node can not be extended with splid, or the splid
  // is a Shengmu id, which is only used to get lpi_items, or the result node
  // in DictTrie has no son, it is not nccessary to keep the new DMI.
  //
  // This function modifies the content of lpi_items_ and lpi_total_.
  // lpi_items_ is used to get the LmaPsbItem list, lpi_total_ returns the size.
  // The function's returned value has no relation with the value of lpi_num.
  //
  // If dmi == NULL, this function will extend the root node of DictTrie
  //
  // This function will not change dmi_nd_pool_used_. Please change it after
  // calling this function if necessary.
  //
  // The caller should guarantees that NULL != dep.
  unsigned extend_dmi(DictExtPara *dep, DictMatchInfo *dmi_s);

  // Extend dmi for the composing phrase.
  unsigned extend_dmi_c(DictExtPara *dep, DictMatchInfo *dmi_s);

  // Extend a MatrixNode with the give LmaPsbItem list.
  // res_row is the destination row number.
  // This function does not change mtrx_nd_pool_used_. Please change it after
  // calling this function if necessary.
  // return 0 always.
  unsigned extend_mtrx_nd(MatrixNode *mtrx_nd, LmaPsbItem lpi_items[],
                        unsigned lpi_num, PoolPosType dmi_fr, unsigned res_row);


  // Try to find a dmi node at step_to position, and the found dmi node should
  // match the given spelling id strings.
  PoolPosType match_dmi(unsigned step_to, uint16 spl_ids[], uint16 spl_id_num);

  bool add_char(char ch);
  bool prepare_add_char(char ch);

  // Called after prepare_add_char, so the input char has been saved.
  bool add_char_qwerty();

  // Prepare candidates from the last fixed hanzi position.
  void prepare_candidates();

  // Is the character in step pos a splitter character?
  // The caller guarantees that the position is valid.
  bool is_split_at(uint16 pos);

  void fill_dmi(DictMatchInfo *dmi, MileStoneHandle *handles,
                PoolPosType dmi_fr,
                uint16 spl_id, uint16 node_num, unsigned char dict_level,
                bool splid_end_split, unsigned char splstr_len,
                unsigned char all_full_id);

  unsigned inner_predict(const char16 fixed_scis_ids[], uint16 scis_num,
                       char16 predict_buf[][kMaxPredictSize + 1],
                       unsigned buf_len);

  // Add the first candidate to the user dictionary.
  bool try_add_cand0_to_userdict();

  // Add a user lemma to the user dictionary. This lemma is a subset of
  // candidate 0. lma_from is from which lemma in lma_ids_, lma_num is the
  // number of lemmas to be combined together as a new lemma. The caller
  // gurantees that the combined new lemma's length is less or equal to
  // kMaxLemmaSize.
  bool add_lma_to_userdict(uint16 lma_from, uint16 lma_num, float score);

  // Update dictionary frequencies.
  void update_dict_freq();

  void debug_print_dmi(PoolPosType dmi_pos, uint16 nest_level);

 public:
  MatrixSearch();
  ~MatrixSearch();

  bool init(const char *fn_sys_dict, const char *fn_usr_dict);

  bool init_fd(int sys_fd, long start_offset, long length,
               const char *fn_usr_dict);

  void set_max_lens(unsigned max_sps_len, unsigned max_hzs_len);

  void close();

  void flush_cache();

  void set_xi_an_switch(bool xi_an_enabled);

  bool get_xi_an_switch();

  // Reset the search space. Equivalent to reset_search(0).
  // If inited, always return true;
  bool reset_search();

  // Search a Pinyin string.
  // Return value is the position successfully parsed.
  unsigned search(const char *py, unsigned py_len);

  // Used to delete something in the Pinyin string kept by the engine, and do
  // a re-search.
  // Return value is the new length of Pinyin string kept by the engine which
  // is parsed successfully.
  // If is_pos_in_splid is false, pos is used to indicate that pos-th Pinyin
  // character needs to be deleted. If is_pos_in_splid is true, all Pinyin
  // characters for pos-th spelling id needs to be deleted.
  // If the deleted character(s) is just after a fixed lemma or sub lemma in
  // composing phrase, clear_fixed_this_step indicates whether we needs to
  // unlock the last fixed lemma or sub lemma.
  // If is_pos_in_splid is false, and pos-th character is in the range for the
  // fixed lemmas or composing string, this function will do nothing and just
  // return the result of the previous search.
  unsigned delsearch(unsigned pos, bool is_pos_in_splid,
                   bool clear_fixed_this_step);

  // Get the number of candiates, called after search().
  unsigned get_candidate_num();

  // Get the Pinyin string stored by the engine.
  // *decoded_len returns the length of the successfully decoded string.
  const char* get_pystr(unsigned *decoded_len);

  // Get the spelling boundaries for the first sentence candidate.
  // Number of spellings will be returned. The number of valid elements in
  // spl_start is one more than the return value because the last one is used
  // to indicate the beginning of the next un-input speling.
  // For a Pinyin "women", the returned value is 2, spl_start is [0, 2, 5] .
  unsigned get_spl_start(const uint16 *&spl_start);

  // Get one candiate string. If full sentence candidate is available, it will
  // be the first one.
  char16* get_candidate(unsigned cand_id, char16 *cand_str, unsigned max_len);

  // Get the first candiate, which is a "full sentence".
  // retstr_len is not NULL, it will be used to return the string length.
  // If only_unfixed is true, only unfixed part will be fetched.
  char16* get_candidate0(char16* cand_str, unsigned max_len,
                         uint16 *retstr_len, bool only_unfixed);

  // Choose a candidate. The decoder will do a search after the fixed position.
  unsigned choose(unsigned cand_id);

  // Cancel the last choosing operation, and return the new number of choices.
  unsigned cancel_last_choice();

  // Get the length of fixed Hanzis.
  unsigned get_fixedlen();

  unsigned get_predicts(const char16 fixed_buf[],
                      char16 predict_buf[][kMaxPredictSize + 1],
                      unsigned buf_len);
};
}

#endif  // PINYINIME_ANDPY_INCLUDE_MATRIXSEARCH_H__
