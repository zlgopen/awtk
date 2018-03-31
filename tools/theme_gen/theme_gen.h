/**
 * File:   theme_gen.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  theme_gen
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_THEME_GEN_H
#define LFTK_THEME_GEN_H

#include <string>
#include <vector>
#include "base/types_def.h"

using std::string;
using std::vector;

class NameIntValue {
 public:
  NameIntValue(uint32_t name, uint32_t value) {
    this->name = name;
    this->value = value;
  }

 public:
  uint32_t name;
  uint32_t value;
};

class NameStringValue {
 public:
  NameStringValue(uint32_t name, const string& value) {
    this->name = name;
    this->value = value;
  }

 public:
  uint32_t name;
  string value;
};

class Style {
 public:
  Style();
  Style(uint16_t widget_type, uint8_t style_type, uint8_t state);
  ~Style();

  bool AddInt(uint32_t name, int32_t value);
  bool AddString(uint32_t name, const string& str);
  uint8_t* Output(uint8_t* buff, uint32_t max_size);
  bool Merge(Style& other);
  bool Reset();

 public:
  uint16_t widget_type;
  uint8_t style_type;
  uint8_t state;
  vector<NameIntValue> int_values;
  vector<NameStringValue> str_values;
};

class ThemeGen {
 public:
  bool AddStyle(const Style& style);
  uint8_t* Output(uint8_t* buff, uint32_t max_size);

 private:
  vector<Style> styles;
};

#endif /*LFTK_THEME_GEN_H*/
