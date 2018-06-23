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

#ifndef PINYINIME_INCLUDE_UTF16READER_H__
#define PINYINIME_INCLUDE_UTF16READER_H__

#include <stdio.h>
#include "./utf16char.h"

namespace ime_pinyin {

class Utf16Reader {
 private:
  FILE *fp_;
  char16 *buffer_;
  unsigned buffer_total_len_;
  unsigned buffer_next_pos_;

  // Always less than buffer_total_len_ - buffer_next_pos_
  unsigned buffer_valid_len_;

 public:
  Utf16Reader();
  ~Utf16Reader();

  // filename is the name of the file to open.
  // buffer_len specifies how long buffer should be allocated to speed up the
  // future reading
  bool open(const char* filename, unsigned buffer_len);
  char16* readline(char16* read_buf, unsigned max_len);
  bool close();
};
}

#endif  // PINYINIME_INCLUDE_UTF16READER_H__
