/**
 * File:   str_gen.cc
 * Author: AWTK Develop Team
 * Brief:  str_gen
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
 * 2018-05-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "str_gen.h"
#include "tkc/wstr.h"
#include "tkc/buffer.h"
#include "base/locale_info.h"

void StrGen::Add(const string& language, const Sentence& sentence) {
  StrTable::iterator iter = this->str_table.find(language);
  if (iter == this->str_table.end()) {
    this->str_table.insert(pair<string, Sentences>(language, Sentences()));
  }

  iter = this->str_table.find(language);

  iter->second.Add(sentence);

  return;
}

uint8_t* StrGen::Output(const string& language, uint8_t* buff, uint32_t max_size) {
  StrTable::iterator iter = this->str_table.find(language);
  return_value_if_fail(buff != NULL && max_size > 1024 && iter != this->str_table.end(), buff);

  Sentences& sentences = iter->second;
  sentences.Sort();

  return this->OutputSentences(sentences, buff, max_size);
}

vector<string> StrGen::GetLanguages() {
  vector<string> languages;

  for (StrTable::iterator i = this->str_table.begin(); i != this->str_table.end(); i++) {
    languages.push_back(i->first);
  }

  return languages;
}

uint8_t* StrGen::OutputSentences(const Sentences& sentences, uint8_t* buff, uint32_t max_size) {
  size_t i = 0;
  wbuffer_t wb;
  wbuffer_t* b;
  size_t header_size = 0;
  size_t nr = sentences.sentences.size();
  str_table_t* table = (str_table_t*)buff;
  return_value_if_fail(buff != NULL && max_size > (nr * 30) && nr >= 1, buff);

  memset(table, 0x00, header_size);

  table->nr = nr;
  table->version = 0;
  header_size = nr * sizeof(str_pair_t) + sizeof(str_table_t);

  b = wbuffer_init(&wb, buff, max_size);
  wbuffer_skip(b, header_size);

  for (i = 0; i < nr; i++) {
    const Sentence& iter = sentences.sentences[i];

    table->strs[i].key = b->cursor;
    wbuffer_write_string(b, iter.key.c_str());

    table->strs[i].value = b->cursor;
    wbuffer_write_string(b, iter.value.c_str());
  }

  return buff + b->cursor;
}
