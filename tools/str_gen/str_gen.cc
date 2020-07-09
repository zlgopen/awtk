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
#include "tkc/mem.h"
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

int32_t StrGen::Output(const string& language, wbuffer_t* wbuffer) {
  StrTable::iterator iter = this->str_table.find(language);
  return_value_if_fail(iter != this->str_table.end(), 0);

  Sentences& sentences = iter->second;
  sentences.Sort();

  return this->OutputSentences(sentences, wbuffer);
}

vector<string> StrGen::GetLanguages() {
  vector<string> languages;

  for (StrTable::iterator i = this->str_table.begin(); i != this->str_table.end(); i++) {
    languages.push_back(i->first);
  }

  return languages;
}

int32_t StrGen::OutputSentences(const Sentences& sentences, wbuffer_t* wbuffer) {
  size_t header_size = 0;
  size_t nr = sentences.sentences.size();
  return_value_if_fail(nr >= 1, 0);
  header_size = nr * sizeof(str_pair_t) + sizeof(str_table_t);
  str_table_t* table = (str_table_t*)TKMEM_ALLOC(header_size);
  memset(table, 0, header_size);
  wbuffer_write_binary(wbuffer, table, header_size);
  table->nr = nr;
  table->version = 0;

  for (size_t i = 0; i < nr; i++) {
    const Sentence& iter = sentences.sentences[i];

    table->strs[i].key = wbuffer->cursor;
    wbuffer_write_string(wbuffer, iter.key.c_str());

    table->strs[i].value = wbuffer->cursor;
    wbuffer_write_string(wbuffer, iter.value.c_str());
  }
  int32_t size = wbuffer->cursor;
  wbuffer->cursor = 0;
  wbuffer_write_binary(wbuffer, table, header_size);
  TKMEM_FREE(table);
  return size;
}
