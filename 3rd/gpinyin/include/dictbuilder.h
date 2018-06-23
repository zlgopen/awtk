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

#ifndef PINYINIME_INCLUDE_DICTBUILDER_H__
#define PINYINIME_INCLUDE_DICTBUILDER_H__

#include <stdlib.h>
#include "./utf16char.h"
#include "./dictdef.h"
#include "./dictlist.h"
#include "./spellingtable.h"
#include "./spellingtrie.h"
#include "./splparser.h"

namespace ime_pinyin {

#ifdef ___BUILD_MODEL___

#define ___DO_STATISTICS___

class DictTrie;

class DictBuilder {
 private:
  // The raw lemma array buffer.
  LemmaEntry *lemma_arr_;
  unsigned lemma_num_;

  // Used to store all possible single char items.
  // Two items may have the same Hanzi while their spelling ids are different.
  SingleCharItem *scis_;
  unsigned scis_num_;

  // In the tree, root's level is -1.
  // Lemma nodes for root, and level 0
  LmaNodeLE0 *lma_nodes_le0_;

  // Lemma nodes for layers whose levels are deeper than 0
  LmaNodeGE1 *lma_nodes_ge1_;

  // Number of used lemma nodes
  unsigned lma_nds_used_num_le0_;
  unsigned lma_nds_used_num_ge1_;

  // Used to store homophonies' ids.
  LemmaIdType *homo_idx_buf_;
  // Number of homophonies each of which only contains one Chinese character.
  unsigned homo_idx_num_eq1_;
  // Number of homophonies each of which contains more than one character.
  unsigned homo_idx_num_gt1_;

  // The items with highest scores.
  LemmaEntry *top_lmas_;
  unsigned top_lmas_num_;

  SpellingTable *spl_table_;
  SpellingParser *spl_parser_;

#ifdef ___DO_STATISTICS___
  unsigned max_sonbuf_len_[kMaxLemmaSize];
  unsigned max_homobuf_len_[kMaxLemmaSize];

  unsigned total_son_num_[kMaxLemmaSize];
  unsigned total_node_hasson_[kMaxLemmaSize];
  unsigned total_sonbuf_num_[kMaxLemmaSize];
  unsigned total_sonbuf_allnoson_[kMaxLemmaSize];
  unsigned total_node_in_sonbuf_allnoson_[kMaxLemmaSize];
  unsigned total_homo_num_[kMaxLemmaSize];

  unsigned sonbufs_num1_;     // Number of son buffer with only 1 son
  unsigned sonbufs_numgt1_;   // Number of son buffer with more 1 son;

  unsigned total_lma_node_num_;

  void stat_init();
  void stat_print();
#endif

 public:

  DictBuilder();
  ~DictBuilder();

  // Build dictionary trie from the file fn_raw. File fn_validhzs provides
  // valid chars. If fn_validhzs is NULL, only chars in GB2312 will be
  // included.
  bool build_dict(const char* fn_raw, const char* fn_validhzs,
                  DictTrie *dict_trie);

 private:
  // Fill in the buffer with id. The caller guarantees that the paramters are
  // vaild.
  void id_to_charbuf(unsigned char *buf, LemmaIdType id);

  // Update the offset of sons for a node.
  void set_son_offset(LmaNodeGE1 *node, unsigned offset);

  // Update the offset of homophonies' ids for a node.
  void set_homo_id_buf_offset(LmaNodeGE1 *node, unsigned offset);

  // Format a speling string.
  void format_spelling_str(char *spl_str);

  // Sort the lemma_arr by the hanzi string, and give each of unique items
  // a id. Why we need to sort the lemma list according to their Hanzi string
  // is to find items started by a given prefix string to do prediction.
  // Actually, the single char items are be in other order, for example,
  // in spelling id order, etc.
  // Return value is next un-allocated idx available.
  LemmaIdType sort_lemmas_by_hz();

  // Build the SingleCharItem list, and fill the hanzi_scis_ids in the
  // lemma buffer lemma_arr_.
  // This function should be called after the lemma array is ready.
  // Return the number of unique SingleCharItem elements.
  unsigned build_scis();

  // Construct a subtree using a subset of the spelling array (from
  // item_star to item_end)
  // parent is the parent node to update the necessary information
  // parent can be a member of LmaNodeLE0 or LmaNodeGE1
  bool construct_subset(void* parent, LemmaEntry* lemma_arr,
                        unsigned item_start, unsigned item_end, unsigned level);


  // Read valid Chinese Hanzis from the given file.
  // num is used to return number of chars.
  // The return buffer is sorted and caller needs to free the returned buffer.
  char16* read_valid_hanzis(const char *fn_validhzs, unsigned *num);


  // Read a raw dictionary. max_item is the maximum number of items. If there
  // are more items in the ditionary, only the first max_item will be read.
  // Returned value is the number of items successfully read from the file.
  unsigned read_raw_dict(const char* fn_raw, const char *fn_validhzs,
                       unsigned max_item);

  // Try to find if a character is in hzs buffer.
  bool hz_in_hanzis_list(const char16 *hzs, unsigned hzs_len, char16 hz);

  // Try to find if all characters in str are in hzs buffer.
  bool str_in_hanzis_list(const char16 *hzs, unsigned hzs_len,
                          const char16 *str, unsigned str_len);

  // Get these lemmas with toppest scores.
  void get_top_lemmas();

  // Allocate resource to build dictionary.
  // lma_num is the number of items to be loaded
  bool alloc_resource(unsigned lma_num);

  // Free resource.
  void free_resource();
};
#endif  // ___BUILD_MODEL___
}

#endif  // PINYINIME_INCLUDE_DICTBUILDER_H__
