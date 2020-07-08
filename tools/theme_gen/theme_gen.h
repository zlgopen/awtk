/**
 * File:   theme_gen.h
 * Author: AWTK Develop Team
 * Brief:  theme_gen
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
 * 2018-01-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_THEME_GEN_H
#define TK_THEME_GEN_H

#include <string>
#include <vector>
#include "tkc/types_def.h"
#include "tkc/buffer.h"

using std::string;
using std::vector;

class NameIntValue {
 public:
  NameIntValue(const string& name, uint32_t value) {
    this->name = name;
    this->value = value;
  }

 public:
  string name;
  uint32_t value;
};

class NameStringValue {
 public:
  NameStringValue(const string& name, const string& value) {
    this->name = name;
    this->value = value;
  }

 public:
  string name;
  string value;
};

class Style {
 public:
  Style();
  Style(const string& widget_type, const string& name, const string& state);
  ~Style();

  bool AddInt(const string& name, int32_t value);
  bool AddString(const string& name, const string& str);
  ret_t Output(wbuffer_t* wbuffer);
  bool Merge(Style& other);
  bool Reset();

 public:
  string widget_type;
  string name;
  string state;
  vector<NameIntValue> int_values;
  vector<NameStringValue> str_values;
};

class ThemeGen {
 public:
  bool AddStyle(const Style& style);
  ret_t Output(wbuffer_t* wbuffer);

 private:
  vector<Style> styles;
};

#endif /*TK_THEME_GEN_H*/
