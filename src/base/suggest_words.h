/**
 * File:   suggest_words.h
 * Author: AWTK Develop Team
 * Brief:  suggest_words
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-06-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SUGGEST_WORDS_H
#define TK_SUGGEST_WORDS_H

#include "base/assets_manager.h"

BEGIN_C_DECLS

#define TK_IM_MAX_SUGGEST_CHARS 127

struct _suggest_words_t;
typedef struct _suggest_words_t suggest_words_t;

/**
 * @class suggest_words_t
 * 输入法联想字词。
 */
struct _suggest_words_t {
  /**
   * @property {char*} words
   * @annotation ["private"]
   * 联想字列表。find函数执行成功后会更新。
   */
  char words[TK_IM_MAX_SUGGEST_CHARS + 1];
  /**
   * @property {uint32_t} words_nr
   * @annotation ["private"]
   * 联想字/词个数。find函数执行成功后会更新。
   */
  uint32_t words_nr;

  const asset_info_t* res;
};

/**
 * @method suggest_words_create
 * @annotation ["constructor"]
 * 创建联想字对象。
 * @param {asset_info_t*} res 数据资源。
 *
 * @return {suggest_words_t*} 返回联想字对象。
 */
suggest_words_t* suggest_words_create(const asset_info_t* res);

/**
 * @method suggest_words_find
 * 查找prefix对应的联想字列表，成功后会更新words/words_nr。
 * @param {suggest_words_t*} suggest_words 联想字对象。
 * @param {wchar_t} c 字符。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t suggest_words_find(suggest_words_t* suggest_words, wchar_t c);

/**
 * @method suggest_words_destroy
 * @deconstructor
 * 销毁联想字对象。
 * @param {suggest_words_t*} suggest_words 联想字对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t suggest_words_destroy(suggest_words_t* suggest_words);

END_C_DECLS

#endif /*TK_SUGGEST_WORDS_H*/
