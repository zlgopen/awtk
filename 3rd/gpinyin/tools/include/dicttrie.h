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

#ifndef PINYINIME_INCLUDE_DICTTRIE_H__
#define PINYINIME_INCLUDE_DICTTRIE_H__

#include <stdlib.h>
#include "./atomdictbase.h"
#include "./dictdef.h"
#include "./dictlist.h"
#include "./searchutility.h"

namespace ime_pinyin {

class DictTrie : AtomDictBase {
 private:
  struct ParsingMark {
    unsigned node_offset:24;
    unsigned node_num:8;           // Number of nodes with this spelling id given
                                 // by spl_id. If spl_id is a Shengmu, for nodes
                                 // in the first layer of DictTrie, it equals to
                                 // SpellingTrie::shm2full_num(); but for those
                                 // nodes which are not in the first layer,
                                 // node_num < SpellingTrie::shm2full_num().
                                 // For a full spelling id, node_num = 1;
  };

  // Used to indicate an extended mile stone.
  // An extended mile stone is used to mark a partial match in the dictionary
  // trie to speed up further potential extending.
  // For example, when the user inputs "w", a mile stone is created to mark the
  // partial match status, so that when user inputs another char 'm', it will be
  // faster to extend search space based on this mile stone.
  //
  // For partial match status of "wm", there can be more than one sub mile
  // stone, for example, "wm" can be matched to "wanm", "wom", ..., etc, so
  // there may be more one parsing mark used to mark these partial matchings.
  // A mile stone records the starting position in the mark list and number of
  // marks.
  struct MileStone {
    uint16 mark_start;
    uint16 mark_num;
  };

  DictList* dict_list_;

  const SpellingTrie *spl_trie_;

  LmaNodeLE0* root_;        // Nodes for root and the first layer.
  LmaNodeGE1* nodes_ge1_;   // Nodes for other layers.

  // An quick index from spelling id to the LmaNodeLE0 node buffer, or
  // to the root_ buffer.
  // Index length:
  // SpellingTrie::get_instance().get_spelling_num() + 1. The last one is used
  // to get the end.
  // All Shengmu ids are not indexed because they will be converted into
  // corresponding full ids.
  // So, given an id splid, the son is:
  // root_[splid_le0_index_[splid - kFullSplIdStart]]
  uint16 *splid_le0_index_;

  unsigned lma_node_num_le0_;
  unsigned lma_node_num_ge1_;

  // The first part is for homophnies, and the last  top_lma_num_ items are
  // lemmas with highest scores.
  unsigned char *lma_idx_buf_;
  unsigned lma_idx_buf_len_;  // The total size of lma_idx_buf_ in byte.
  unsigned total_lma_num_;    // Total number of lemmas in this dictionary.
  unsigned top_lmas_num_;     // Number of lemma with highest scores.

  // Parsing mark list used to mark the detailed extended statuses.
  ParsingMark *parsing_marks_;
  // The position for next available mark.
  uint16 parsing_marks_pos_;

  // Mile stone list used to mark the extended status.
  MileStone *mile_stones_;
  // The position for the next available mile stone. We use positions (except 0)
  // as handles.
  MileStoneHandle mile_stones_pos_;

  // Get the offset of sons for a node.
  inline unsigned get_son_offset(const LmaNodeGE1 *node);

  // Get the offset of homonious ids for a node.
  inline unsigned get_homo_idx_buf_offset(const LmaNodeGE1 *node);

  // Get the lemma id by the offset.
  inline LemmaIdType get_lemma_id(unsigned id_offset);

  void free_resource(bool free_dict_list);

  bool load_dict(FILE *fp);

  // Given a LmaNodeLE0 node, extract the lemmas specified by it, and fill
  // them into the lpi_items buffer.
  // This function is called by the search engine.
  unsigned fill_lpi_buffer(LmaPsbItem lpi_items[], unsigned max_size,
                         LmaNodeLE0 *node);

  // Given a LmaNodeGE1 node, extract the lemmas specified by it, and fill
  // them into the lpi_items buffer.
  // This function is called by inner functions extend_dict0(), extend_dict1()
  // and extend_dict2().
  unsigned fill_lpi_buffer(LmaPsbItem lpi_items[], unsigned max_size,
                         unsigned homo_buf_off, LmaNodeGE1 *node,
                         uint16 lma_len);

  // Extend in the trie from level 0.
  MileStoneHandle extend_dict0(MileStoneHandle from_handle,
                               const DictExtPara *dep, LmaPsbItem *lpi_items,
                               unsigned lpi_max, unsigned *lpi_num);

  // Extend in the trie from level 1.
  MileStoneHandle extend_dict1(MileStoneHandle from_handle,
                               const DictExtPara *dep, LmaPsbItem *lpi_items,
                               unsigned lpi_max, unsigned *lpi_num);

  // Extend in the trie from level 2.
  MileStoneHandle extend_dict2(MileStoneHandle from_handle,
                               const DictExtPara *dep, LmaPsbItem *lpi_items,
                               unsigned lpi_max, unsigned *lpi_num);

  // Try to extend the given spelling id buffer, and if the given id_lemma can
  // be successfully gotten, return true;
  // The given spelling ids are all valid full ids.
  bool try_extend(const uint16 *splids, uint16 splid_num, LemmaIdType id_lemma);

#ifdef ___BUILD_MODEL___
  bool save_dict(FILE *fp);
#endif  // ___BUILD_MODEL___

  static const int kMaxMileStone = 100;
  static const int kMaxParsingMark = 600;
  static const MileStoneHandle kFirstValidMileStoneHandle = 1;

  friend class DictParser;
  friend class DictBuilder;

 public:

  DictTrie();
  ~DictTrie();

#ifdef ___BUILD_MODEL___
  // Construct the tree from the file fn_raw.
  // fn_validhzs provide the valid hanzi list. If fn_validhzs is
  // NULL, only chars in GB2312 will be included.
  bool build_dict(const char *fn_raw, const char *fn_validhzs);

  // Save the binary dictionary
  // Actually, the SpellingTrie/DictList instance will be also saved.
  bool save_dict(const char *filename);
#endif  // ___BUILD_MODEL___

  void convert_to_hanzis(char16 *str, uint16 str_len);

  void convert_to_scis_ids(char16 *str, uint16 str_len);

  // Load a binary dictionary
  // The SpellingTrie instance/DictList will be also loaded
  bool load_dict(const char *filename, LemmaIdType start_id,
                 LemmaIdType end_id);
  bool load_dict_fd(int sys_fd, long start_offset, long length,
                    LemmaIdType start_id, LemmaIdType end_id);
  bool close_dict() {return true;}
  unsigned number_of_lemmas() {return 0;}

  void reset_milestones(uint16 from_step, MileStoneHandle from_handle);

  MileStoneHandle extend_dict(MileStoneHandle from_handle,
                              const DictExtPara *dep,
                              LmaPsbItem *lpi_items,
                              unsigned lpi_max, unsigned *lpi_num);

  unsigned get_lpis(const uint16 *splid_str, uint16 splid_str_len,
                  LmaPsbItem *lpi_items, unsigned lpi_max);

  uint16 get_lemma_str(LemmaIdType id_lemma, char16 *str_buf, uint16 str_max);

  uint16 get_lemma_splids(LemmaIdType id_lemma, uint16 *splids,
                          uint16 splids_max, bool arg_valid);

  unsigned predict(const char16 *last_hzs, uint16 hzs_len,
                 NPredictItem *npre_items, unsigned npre_max,
                 unsigned b4_used);

  LemmaIdType put_lemma(char16 lemma_str[], uint16 splids[],
                        uint16 lemma_len, uint16 count) {return 0;}

  LemmaIdType update_lemma(LemmaIdType lemma_id, int16 delta_count,
                           bool selected) {return 0;}

  LemmaIdType get_lemma_id(char16 lemma_str[], uint16 splids[],
                           uint16 lemma_len) {return 0;}

  LmaScoreType get_lemma_score(LemmaIdType lemma_id) {return 0;}

  LmaScoreType get_lemma_score(char16 lemma_str[], uint16 splids[],
                        uint16 lemma_len) {return 0;}

  bool remove_lemma(LemmaIdType lemma_id) {return false;}

  unsigned get_total_lemma_count() {return 0;}
  void set_total_lemma_count_of_others(unsigned count);

  void flush_cache() {}

  LemmaIdType get_lemma_id(const char16 lemma_str[], uint16 lemma_len);

  // Fill the lemmas with highest scores to the prediction buffer.
  // his_len is the history length to fill in the prediction buffer.
  unsigned predict_top_lmas(unsigned his_len, NPredictItem *npre_items,
                          unsigned npre_max, unsigned b4_used);
};
}

#endif  // PINYINIME_INCLUDE_DICTTRIE_H__
