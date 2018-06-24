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

#include "../include/sync.h"
#include <assert.h>
#include <string.h>

#ifdef ___SYNC_ENABLED___

namespace ime_pinyin {

Sync::Sync()
  : userdict_(NULL),
    dictfile_(NULL),
    last_count_(0) {
};

Sync::~Sync() {
}


bool Sync::begin(const char * filename) {
  if (userdict_) {
    finish();
  }

  if (!filename) {
    return false;
  }

  dictfile_ = strdup(filename);
  if (!dictfile_) {
    return false;
  }

  userdict_ = new UserDict();
  if (!userdict_) {
    free(dictfile_);
    dictfile_ = NULL;
    return false;
  }

  if (userdict_->load_dict((const char*)dictfile_, kUserDictIdStart,
                           kUserDictIdEnd) == false) {
    delete userdict_;
    userdict_ = NULL;
    free(dictfile_);
    dictfile_ = NULL;
    return false;
  }

  userdict_->set_limit(kUserDictMaxLemmaCount, kUserDictMaxLemmaSize, kUserDictRatio);

  return true;
}

int Sync::put_lemmas(char16 * lemmas, int len) {
  return userdict_->put_lemmas_no_sync_from_utf16le_string(lemmas, len);
}

int Sync::get_lemmas(char16 * str, int size) {
  return userdict_->get_sync_lemmas_in_utf16le_string_from_beginning(str, size, &last_count_);
}

int Sync::get_last_got_count() {
  return last_count_;
}

int Sync::get_total_count() {
  return userdict_->get_sync_count();
}

void Sync::clear_last_got() {
  if (last_count_ < 0) {
    return;
  }
  userdict_->clear_sync_lemmas(0, last_count_);
  last_count_ = 0;
}

void Sync::finish() {
  if (userdict_) {
    userdict_->close_dict();
    delete userdict_;
    userdict_ = NULL;
    free(dictfile_);
    dictfile_ = NULL;
    last_count_ = 0;
  }
}

int Sync::get_capacity() {
  UserDict::UserDictStat stat;
  userdict_->state(&stat);
  return stat.limit_lemma_count - stat.lemma_count;
}

}
#endif
