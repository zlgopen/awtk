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

#include "../include/utf16reader.h"

namespace ime_pinyin {

#define MIN_BUF_LEN 128
#define MAX_BUF_LEN 65535

Utf16Reader::Utf16Reader() {
  fp_ = NULL;
  buffer_ = NULL;
  buffer_total_len_ = 0;
  buffer_next_pos_ = 0;
  buffer_valid_len_ = 0;
}

Utf16Reader::~Utf16Reader() {
  if (NULL != fp_)
    fclose(fp_);

  if (NULL != buffer_)
    delete [] buffer_;
}


bool Utf16Reader::open(const char* filename, unsigned buffer_len) {
  if (filename == NULL)
    return false;

  if (buffer_len < MIN_BUF_LEN)
    buffer_len = MIN_BUF_LEN;
  else if (buffer_len > MAX_BUF_LEN)
    buffer_len = MAX_BUF_LEN;

  buffer_total_len_ = buffer_len;

  if (NULL != buffer_)
    delete [] buffer_;
  buffer_ = new char16[buffer_total_len_];
  if (NULL == buffer_)
    return false;

  if ((fp_ = fopen(filename, "rb")) == NULL)
    return false;

  // the UTF16 file header, skip
  char16 header;
  if (fread(&header, sizeof(header), 1, fp_) != 1 || header != 0xfeff) {
    fclose(fp_);
    fp_ = NULL;
    return false;
  }

  return true;
}

char16* Utf16Reader::readline(char16* read_buf, unsigned max_len) {
  if (NULL == fp_ || NULL == read_buf || 0 == max_len)
    return NULL;

  unsigned ret_len = 0;

  do {
    if (buffer_valid_len_ == 0) {
      buffer_next_pos_ = 0;
      buffer_valid_len_ = fread(buffer_, sizeof(char16),
                                buffer_total_len_, fp_);
      if (buffer_valid_len_ == 0) {
        if (0 == ret_len)
          return NULL;
        read_buf[ret_len] = (char16)'\0';
        return read_buf;
      }
    }

    for (unsigned i = 0; i < buffer_valid_len_; i++) {
      if (i == max_len - 1 ||
          buffer_[buffer_next_pos_ + i] == (char16)'\n') {
        if (ret_len + i > 0 && read_buf[ret_len + i - 1] == (char16)'\r') {
          read_buf[ret_len + i - 1] = (char16)'\0';
        } else {
          read_buf[ret_len + i] = (char16)'\0';
        }

        i++;
        buffer_next_pos_ += i;
        buffer_valid_len_ -= i;
        if (buffer_next_pos_ == buffer_total_len_) {
          buffer_next_pos_ = 0;
          buffer_valid_len_ = 0;
        }
        return read_buf;
      } else {
        read_buf[ret_len + i] = buffer_[buffer_next_pos_ + i];
      }
    }

    ret_len += buffer_valid_len_;
    buffer_valid_len_ = 0;
  } while (true);

  // Never reach here
  return NULL;
}

bool Utf16Reader::close() {
  if (NULL != fp_)
    fclose(fp_);
  fp_ = NULL;

  if (NULL != buffer_)
    delete [] buffer_;
  buffer_ = NULL;
  return true;
}
}  // namespace ime_pinyin
