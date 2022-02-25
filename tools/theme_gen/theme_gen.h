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
#include "base/theme.h"
#include "tkc/buffer.h"

using std::string;
using std::vector;

class BinaryData {
 public:
  uint8_t* data;
  uint32_t size;

  void Reset(void) {
    delete[] this->data;
    this->data = NULL;
    this->size = 0;
  }
  void Copy(const void* data, uint32_t size) {
    this->size = size;
    this->data = new uint8_t[size];
    memcpy(this->data, data, size);
  }

  BinaryData(void) {
    this->data = NULL;
    this->size = 0;
  }

  BinaryData(const BinaryData& other) {
    this->Copy(other.data, other.size);
  }

  BinaryData& operator=(const BinaryData& other) {
    this->Reset();
    this->Copy(other.data, other.size);
    return *this;
  }

  BinaryData(const binary_data_t* bin) {
    this->Copy(bin->data, bin->size);
  }

  ~BinaryData() {
    this->Reset();
  }
};

template <class T>
class NameValue {
 public:
  NameValue(const string& name, const T& value, uint32_t type) {
    this->name = name;
    this->type = type;
    this->value = value;
  }

 public:
  string name;
  uint32_t type;
  T value;
};

template <class T>
class NameValues {
 private:
  void write_value_to_wbuffer(wbuffer_t* wbuffer, const string& name, uint32_t value) {
    wbuffer_write_uint32(wbuffer, value);
    log_debug("    %s=0x%08x\n", name.c_str(), value);
  }
  void write_value_to_wbuffer(wbuffer_t* wbuffer, const string& name, int32_t value) {
    wbuffer_write_int32(wbuffer, value);
    log_debug("    %s=%d \n", name.c_str(), value);
  }
  void write_value_to_wbuffer(wbuffer_t* wbuffer, const string& name, const string& value) {
    wbuffer_write_string(wbuffer, value.c_str());
    log_debug("    %s=%s\n", name.c_str(), value.c_str());
  }
  void write_value_to_wbuffer(wbuffer_t* wbuffer, const string& name, const BinaryData& value) {
    wbuffer_write_binary(wbuffer, value.data, value.size);
    log_debug("    %s=bin(%d)\n", name.c_str(), value.size);
  }

  void write_value_header_to_wbuffer(wbuffer_t* wbuffer, uint32_t type, const string& name,
                                     uint32_t value) {
    style_name_value_header_t nv;
    nv.type = type;
    nv.name_size = name.size() + 1;
    nv.value_size = sizeof(value);
    wbuffer_write_binary(wbuffer, &nv, sizeof(nv));
  }

  void write_value_header_to_wbuffer(wbuffer_t* wbuffer, uint32_t type, const string& name,
                                     int32_t value) {
    style_name_value_header_t nv;
    nv.type = type;
    nv.name_size = name.size() + 1;
    nv.value_size = sizeof(value);
    wbuffer_write_binary(wbuffer, &nv, sizeof(nv));
  }

  void write_value_header_to_wbuffer(wbuffer_t* wbuffer, uint32_t type, const string& name,
                                     const string& value) {
    style_name_value_header_t nv;
    nv.type = type;
    nv.name_size = name.size() + 1;
    nv.value_size = value.size() + 1;
    wbuffer_write_binary(wbuffer, &nv, sizeof(nv));
  }

  void write_value_header_to_wbuffer(wbuffer_t* wbuffer, uint32_t type, const string& name,
                                     const BinaryData& value) {
    style_name_value_header_t nv;
    nv.type = type;
    nv.name_size = name.size() + 1;
    nv.value_size = value.size;
    wbuffer_write_binary(wbuffer, &nv, sizeof(nv));
  }

 public:
  typedef T value_type;
  typedef NameValue<value_type> name_value_type;
  typedef typename vector<name_value_type>::iterator iter_type;
  typedef typename vector<name_value_type>::const_iterator const_iter_type;

  bool AddValue(const string& name, value_type value, uint32_t type) {
    for (iter_type i = values.begin(); i != values.end(); i++) {
      if (i->name == name) {
        i->type = type;
        i->value = value;
        return true;
      }
    }
    values.push_back(NameValue<value_type>(name, value, type));
    return true;
  }

  bool RemoveValue(const string& name) {
    for (iter_type i = values.begin(); i != values.end(); i++) {
      if (i->name == name) {
        values.erase(i);
        return true;
      }
    }
    return false;
  }

  uint32_t Size() {
    return values.size();
  }

  void Clear() {
    values.clear();
  }

  bool Merge(const NameValues<value_type>& other_values) {
    for (typename NameValues<value_type>::const_iter_type i = other_values.values.begin();
         i != other_values.values.end(); i++) {
      AddValue(i->name, i->value, i->type);
    }
    return true;
  }

  bool WriteToWbuffer(wbuffer_t* wbuffer) {
    return_value_if_fail(wbuffer != NULL, false);
    for (iter_type i = this->values.begin(); i != this->values.end(); i++) {
      const string& name = i->name;
      write_value_header_to_wbuffer(wbuffer, i->type, name, i->value);
      wbuffer_write_string(wbuffer, name.c_str());
      write_value_to_wbuffer(wbuffer, name, i->value);
    }
    return true;
  }

 public:
  vector<name_value_type> values;
};

class Style {
 public:
  Style();
  Style(const string& widget_type, const string& name, const string& state);
  ~Style();

  bool AddValue(const string& name, int32_t value);
  bool AddValue(const string& name, uint32_t value);
  bool AddValue(const string& name, const char* value);
  bool AddValue(const string& name, const binary_data_t* bin);
  bool AddValue(const string& name, const value_t& v);
  bool RemoveValue(const string& name);
  ret_t Output(wbuffer_t* wbuffer);
  bool Merge(Style& other);
  bool Reset();

 public:
  string widget_type;
  string name;
  string state;

  NameValues<BinaryData> bin_values;
  NameValues<string> str_values;
  NameValues<int32_t> int_values;
  NameValues<uint32_t> uint_values;

  vector<Style> datas;
};

class ThemeGen {
 public:
  bool AddStyle(const Style& style);
  ret_t Output(wbuffer_t* wbuffer);

 private:
  vector<Style> styles;
};

#endif /*TK_THEME_GEN_H*/
