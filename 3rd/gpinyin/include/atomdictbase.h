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

/**
 * This class defines AtomDictBase class which is the base class for all atom
 * dictionaries. Atom dictionaries are managed by the decoder class
 * MatrixSearch.
 *
 * When the user appends a new character to the Pinyin string, all enabled atom
 * dictionaries' extend_dict() will be called at least once to get candidates
 * ended in this step (the information of starting step is also given in the
 * parameter). Usually, when extend_dict() is called, a MileStoneHandle object
 * returned by a previous calling for a earlier step is given to speed up the
 * look-up process, and a new MileStoneHandle object will be returned if
 * the extension is successful.
 *
 * A returned MileStoneHandle object should keep alive until Function
 * reset_milestones() is called and this object is noticed to be reset.
 *
 * Usually, the atom dictionary can use step information to manage its
 * MileStoneHandle objects, or it can make the objects in ascendant order to
 * make the reset easier.
 *
 * When the decoder loads the dictionary, it will give a starting lemma id for
 * this atom dictionary to map a inner id to a global id. Global ids should be
 * used when an atom dictionary talks to any component outside.
 */
#ifndef PINYINIME_INCLUDE_ATOMDICTBASE_H__
#define PINYINIME_INCLUDE_ATOMDICTBASE_H__

#include <stdlib.h>
#include "./dictdef.h"
#include "./searchutility.h"

namespace ime_pinyin {
class AtomDictBase {
 public:
  virtual ~AtomDictBase() {}

  /**
   * Load an atom dictionary from a file.
   *
   * @param file_name The file name to load dictionary.
   * @param start_id The starting id used for this atom dictionary.
   * @param end_id The end id (included) which can be used for this atom
   * dictionary. User dictionary will always use the last id space, so it can
   * ignore this paramter. All other atom dictionaries should check this
   * parameter.
   * @return True if succeed.
   */
  virtual bool load_dict(const char *file_name, LemmaIdType start_id,
                         LemmaIdType end_id) = 0;

  /**
   * Close this atom dictionary.
   *
   * @return True if succeed.
   */
  virtual bool close_dict() = 0;

  /**
   * Get the total number of lemmas in this atom dictionary.
   *
   * @return The total number of lemmas.
   */
  virtual unsigned number_of_lemmas() = 0;

  /**
   * This function is called by the decoder when user deletes a character from
   * the input string, or begins a new input string.
   *
   * Different atom dictionaries may implement this function in different way.
   * an atom dictionary can use one of these two parameters (or both) to reset
   * its corresponding MileStoneHandle objects according its detailed
   * implementation.
   *
   * For example, if an atom dictionary uses step information to manage its
   * MileStoneHandle objects, parameter from_step can be used to identify which
   * objects should be reset; otherwise, if another atom dictionary does not
   * use the detailed step information, it only uses ascendant handles
   * (according to step. For the same step, earlier call, smaller handle), it
   * can easily reset those MileStoneHandle which are larger than from_handle.
   *
   * The decoder always reset the decoding state by step. So when it begins
   * resetting, it will call reset_milestones() of its atom dictionaries with
   * the step information, and the MileStoneHandle objects returned by the
   * earliest calling of extend_dict() for that step.
   *
   * If an atom dictionary does not implement incremental search, this function
   * can be totally ignored.
   *
   * @param from_step From which step(included) the MileStoneHandle
   * objects should be reset.
   * @param from_handle The ealiest MileStoneHandle object for step from_step
   */
  virtual void reset_milestones(uint16 from_step,
                                MileStoneHandle from_handle) = 0;

  /**
   * Used to extend in this dictionary. The handle returned should keep valid
   * until reset_milestones() is called.
   *
   * @param from_handle Its previous returned extended handle without the new
   * spelling id, it can be used to speed up the extending.
   * @param dep The paramter used for extending.
   * @param lpi_items Used to fill in the lemmas matched.
   * @param lpi_max The length of the buffer
   * @param lpi_num Used to return the newly added items.
   * @return The new mile stone for this extending. 0 if fail.
   */
  virtual MileStoneHandle extend_dict(MileStoneHandle from_handle,
                                      const DictExtPara *dep,
                                      LmaPsbItem *lpi_items,
                                      unsigned lpi_max, unsigned *lpi_num) = 0;

  /**
   * Get lemma items with scores according to a spelling id stream.
   * This atom dictionary does not need to sort the returned items.
   *
   * @param splid_str The spelling id stream buffer.
   * @param splid_str_len The length of the spelling id stream buffer.
   * @param lpi_items Used to return matched lemma items with scores.
   * @param lpi_max The maximum size of the buffer to return result.
   * @return The number of matched items which have been filled in to lpi_items.
   */
  virtual unsigned get_lpis(const uint16 *splid_str, uint16 splid_str_len,
                          LmaPsbItem *lpi_items, unsigned lpi_max) = 0;

  /**
   * Get a lemma string (The Chinese string) by the given lemma id.
   *
   * @param id_lemma The lemma id to get the string.
   * @param str_buf The buffer to return the Chinese string.
   * @param str_max The maximum size of the buffer.
   * @return The length of the string, 0 if fail.
   */
  virtual uint16 get_lemma_str(LemmaIdType id_lemma, char16 *str_buf,
                               uint16 str_max) = 0;

  /**
   * Get the full spelling ids for the given lemma id.
   * If the given buffer is too short, return 0.
   *
   * @param splids Used to return the spelling ids.
   * @param splids_max The maximum buffer length of splids.
   * @param arg_valid Used to indicate if the incoming parameters have been
   * initialized are valid. If it is true, the splids and splids_max are valid
   * and there may be half ids in splids to be updated to full ids. In this
   * case, splids_max is the number of valid ids in splids.
   * @return The number of ids in the buffer.
   */
  virtual uint16 get_lemma_splids(LemmaIdType id_lemma, uint16 *splids,
                                  uint16 splids_max, bool arg_valid) = 0;

  /**
   * Function used for prediction.
   * No need to sort the newly added items.
   *
   * @param last_hzs The last n Chinese chracters(called Hanzi), its length
   * should be less than or equal to kMaxPredictSize.
   * @param hzs_len specifies the length(<= kMaxPredictSize) of the history.
   * @param npre_items Used used to return the result.
   * @param npre_max The length of the buffer to return result
   * @param b4_used Number of prediction result (from npre_items[-b4_used])
   * from other atom dictionaries. A atom ditionary can just ignore it.
   * @return The number of prediction result from this atom dictionary.
   */
  virtual unsigned predict(const char16 last_hzs[], uint16 hzs_len,
                         NPredictItem *npre_items, unsigned npre_max,
                         unsigned b4_used) = 0;

  /**
   * Add a lemma to the dictionary. If the dictionary allows to add new
   * items and this item does not exist, add it.
   *
   * @param lemma_str The Chinese string of the lemma.
   * @param splids The spelling ids of the lemma.
   * @param lemma_len The length of the Chinese lemma.
   * @param count The frequency count for this lemma.
   */
  virtual LemmaIdType put_lemma(char16 lemma_str[], uint16 splids[],
                                uint16 lemma_len, uint16 count) = 0;

  /**
   * Update a lemma's occuring count.
   *
   * @param lemma_id The lemma id to update.
   * @param delta_count The frequnecy count to ajust.
   * @param selected Indicate whether this lemma is selected by user and
   * submitted to target edit box.
   * @return The id if succeed, 0 if fail.
   */
  virtual LemmaIdType update_lemma(LemmaIdType lemma_id, int16 delta_count,
                                   bool selected) = 0;

  /**
   * Get the lemma id for the given lemma.
   *
   * @param lemma_str The Chinese string of the lemma.
   * @param splids The spelling ids of the lemma.
   * @param lemma_len The length of the lemma.
   * @return The matched lemma id, or 0 if fail.
   */
  virtual LemmaIdType get_lemma_id(char16 lemma_str[], uint16 splids[],
                                   uint16 lemma_len) = 0;

  /**
   * Get the lemma score.
   *
   * @param lemma_id The lemma id to get score.
   * @return The score of the lemma, or 0 if fail.
   */
  virtual LmaScoreType get_lemma_score(LemmaIdType lemma_id) = 0;

  /**
   * Get the lemma score.
   *
   * @param lemma_str The Chinese string of the lemma.
   * @param splids The spelling ids of the lemma.
   * @param lemma_len The length of the lemma.
   * @return The score of the lamm, or 0 if fail.
   */
  virtual LmaScoreType get_lemma_score(char16 lemma_str[], uint16 splids[],
                                uint16 lemma_len) = 0;

  /**
   * If the dictionary allowed, remove a lemma from it.
   *
   * @param lemma_id The id of the lemma to remove.
   * @return True if succeed.
   */
  virtual bool remove_lemma(LemmaIdType lemma_id) = 0;

  /**
   * Get the total occuring count of this atom dictionary.
   *
   * @return The total occuring count of this atom dictionary.
   */
  virtual unsigned get_total_lemma_count() = 0;

  /**
   * Set the total occuring count of other atom dictionaries.
   *
   * @param count The total occuring count of other atom dictionaies.
   */
  virtual void set_total_lemma_count_of_others(unsigned count) = 0;

  /**
   * Notify this atom dictionary to flush the cached data to persistent storage
   * if necessary.
   */
  virtual void flush_cache() = 0;
};
}

#endif  // PINYINIME_INCLUDE_ATOMDICTBASE_H__
