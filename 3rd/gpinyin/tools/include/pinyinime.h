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

#ifndef PINYINIME_INCLUDE_ANDPYIME_H__
#define PINYINIME_INCLUDE_ANDPYIME_H__

#include <stdlib.h>
#include "./dictdef.h"

#ifdef __cplusplus
extern "C" {
#endif

  namespace ime_pinyin {

  /**
   * Open the decoder engine via the system and user dictionary file names.
   *
   * @param fn_sys_dict The file name of the system dictionary.
   * @param fn_usr_dict The file name of the user dictionary.
   * @return true if open the decoder engine successfully.
   */
  bool im_open_decoder(const char *fn_sys_dict, const char *fn_usr_dict);

  /**
   * Open the decoder engine via the system dictionary FD and user dictionary
   * file name. Because on Android, the system dictionary is embedded in the
   * whole application apk file.
   *
   * @param sys_fd The file in which the system dictionary is embedded.
   * @param start_offset The starting position of the system dictionary in the
   * file sys_fd.
   * @param length The length of the system dictionary in the file sys_fd,
   * counted in byte.
   * @return true if succeed.
   */
  bool im_open_decoder_fd(int sys_fd, long start_offset, long length,
                          const char *fn_usr_dict);

  /**
   * Close the decoder engine.
   */
  void im_close_decoder();

  /**
   * Set maximum limitations for decoding. If this function is not called,
   * default values will be used. For example, due to screen size limitation,
   * the UI engine of the IME can only show a certain number of letters(input)
   * to decode, and a certain number of Chinese characters(output). If after
   * user adds a new letter, the input or the output string is longer than the
   * limitations, the engine will discard the recent letter.
   *
   * @param max_sps_len Maximum length of the spelling string(Pinyin string).
   * @max_hzs_len Maximum length of the decoded Chinese character string.
   */
  void im_set_max_lens(unsigned max_sps_len, unsigned max_hzs_len);

  /**
   * Flush cached data to persistent memory. Because at runtime, in order to
   * achieve best performance, some data is only store in memory.
   */
  void im_flush_cache();

  /**
   * Use a spelling string(Pinyin string) to search. The engine will try to do
   * an incremental search based on its previous search result, so if the new
   * string has the same prefix with the previous one stored in the decoder,
   * the decoder will only continue the search from the end of the prefix.
   * If the caller needs to do a brand new search, please call im_reset_search()
   * first. Calling im_search() is equivalent to calling im_add_letter() one by
   * one.
   *
   * @param sps_buf The spelling string buffer to decode.
   * @param sps_len The length of the spelling string buffer.
   * @return The number of candidates.
   */
  unsigned im_search(const char* sps_buf, unsigned sps_len);

  /**
   * Make a delete operation in the current search result, and make research if
   * necessary.
   *
   * @param pos The posistion of char in spelling string to delete, or the
   * position of spelling id in result string to delete.
   * @param is_pos_in_splid Indicate whether the pos parameter is the position
   * in the spelling string, or the position in the result spelling id string.
   * @return The number of candidates.
   */
  unsigned im_delsearch(unsigned pos, bool is_pos_in_splid,
                      bool clear_fixed_this_step);

  /**
   * Reset the previous search result.
   */
  void im_reset_search();

  /**
   * Add a Pinyin letter to the current spelling string kept by decoder. If the
   * decoder fails in adding the letter, it will do nothing. im_get_sps_str()
   * can be used to get the spelling string kept by decoder currently.
   *
   * @param ch The letter to add.
   * @return The number of candidates.
   */
  unsigned im_add_letter(char ch);

  /**
   * Get the spelling string kept by the decoder.
   *
   * @param decoded_len Used to return how many characters in the spelling
   * string is successfully parsed.
   * @return The spelling string kept by the decoder.
   */
  const char *im_get_sps_str(unsigned *decoded_len);

  /**
   * Get a candidate(or choice) string.
   *
   * @param cand_id The id to get a candidate. Started from 0. Usually, id 0
   * is a sentence-level candidate.
   * @param cand_str The buffer to store the candidate.
   * @param max_len The maximum length of the buffer.
   * @return cand_str if succeeds, otherwise NULL.
   */
  char16* im_get_candidate(unsigned cand_id, char16* cand_str,
                           unsigned max_len);

  /**
   * Get the segmentation information(the starting positions) of the spelling
   * string.
   *
   * @param spl_start Used to return the starting posistions.
   * @return The number of spelling ids. If it is L, there will be L+1 valid
   * elements in spl_start, and spl_start[L] is the posistion after the end of
   * the last spelling id.
   */
  unsigned im_get_spl_start_pos(const uint16 *&spl_start);

  /**
   * Choose a candidate and make it fixed. If the candidate does not match
   * the end of all spelling ids, new candidates will be provided from the
   * first unfixed position. If the candidate matches the end of the all
   * spelling ids, there will be only one new candidates, or the whole fixed
   * sentence.
   *
   * @param cand_id The id of candidate to select and make it fixed.
   * @return The number of candidates. If after the selection, the whole result
   * string has been fixed, there will be only one candidate.
   */
  unsigned im_choose(unsigned cand_id);

  /**
   * Cancel the last selection, or revert the last operation of im_choose().
   *
   * @return The number of candidates.
   */
  unsigned im_cancel_last_choice();

  /**
   * Get the number of fixed spelling ids, or Chinese characters.
   *
   * @return The number of fixed spelling ids, of Chinese characters.
   */
  unsigned im_get_fixed_len();

  /**
   * Cancel the input state and reset the search workspace.
   */
  bool im_cancel_input();

  /**
   * Get prediction candiates based on the given fixed Chinese string as the
   * history.
   *
   * @param his_buf The history buffer to do the prediction. It should be ended
   * with '\0'.
   * @param pre_buf Used to return prediction result list.
   * @return The number of predicted result string.
   */
  unsigned im_get_predicts(const char16 *his_buf,
                         char16 (*&pre_buf)[kMaxPredictSize + 1]);

  /**
   * Enable Shengmus in ShouZiMu mode.
   */
  void im_enable_shm_as_szm(bool enable);

  /**
   * Enable Yunmus in ShouZiMu mode.
   */
  void im_enable_ym_as_szm(bool enable);
}

#ifdef __cplusplus
}
#endif

#endif  // PINYINIME_INCLUDE_ANDPYIME_H__
