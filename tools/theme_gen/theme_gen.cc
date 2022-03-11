/**
 * File:   theme_gen.cc
 * Author: AWTK Develop Team
 * Brief:  theme_gen interface
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

#include "theme_gen.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "base/theme.h"
#include "tkc/buffer.h"
#include "tkc/types_def.h"
#include "tkc/mem.h"

Style::Style() {
}

Style::Style(const string& widget_type, const string& name, const string& state) {
  this->widget_type = widget_type;
  this->name = name;
  this->state = state;
}

Style::~Style() {
}

bool Style::AddValue(const string& name, int32_t value) {
  return int_values.AddValue(name, value, VALUE_TYPE_INT32);
}

bool Style::AddValue(const string& name, uint32_t value) {
  return uint_values.AddValue(name, value, VALUE_TYPE_UINT32);
}

bool Style::AddValue(const string& name, const char* value) {
  return str_values.AddValue(name, string(value), VALUE_TYPE_STRING);
}

bool Style::AddValue(const string& name, const binary_data_t* bin) {
  return bin_values.AddValue(name, BinaryData(bin), VALUE_TYPE_STRING);
}

bool Style::AddValue(const string& name, const value_t& v) {
  if (v.type == VALUE_TYPE_STRING) {
    return str_values.AddValue(name, string(value_str(&v)), v.type);
  } else if (v.type == VALUE_TYPE_GRADIENT) {
    return bin_values.AddValue(name, value_gradient(&v), v.type);
  } else {
    if (v.type == VALUE_TYPE_INT32) {
      return int_values.AddValue(name, value_int(&v), v.type);
    } else {
      return uint_values.AddValue(name, value_uint32(&v), v.type);
    }
  }
  return true;
}

bool Style::Reset() {
  this->str_values.Clear();
  this->bin_values.Clear();
  this->int_values.Clear();
  this->uint_values.Clear();
  this->datas.clear();

  return true;
}

bool Style::Merge(Style& other) {
  str_values.Merge(other.str_values);
  for (NameValues<std::string>::const_iter_type i = (other.str_values).values.begin();
       i != (other.str_values).values.end(); i++) {
    bin_values.RemoveValue(i->name);
    int_values.RemoveValue(i->name);
    uint_values.RemoveValue(i->name);
  }

  bin_values.Merge(other.bin_values);
  for (NameValues<BinaryData>::const_iter_type i = (other.bin_values).values.begin();
       i != (other.bin_values).values.end(); i++) {
    str_values.RemoveValue(i->name);
    int_values.RemoveValue(i->name);
    uint_values.RemoveValue(i->name);
  }

  int_values.Merge(other.int_values);
  for (NameValues<int32_t>::const_iter_type i = (other.int_values).values.begin();
       i != (other.int_values).values.end(); i++) {
    bin_values.RemoveValue(i->name);
    str_values.RemoveValue(i->name);
    uint_values.RemoveValue(i->name);
  }

  uint_values.Merge(other.uint_values);
  for (NameValues<uint32_t>::const_iter_type i = (other.uint_values).values.begin();
       i != (other.uint_values).values.end(); i++) {
    bin_values.RemoveValue(i->name);
    int_values.RemoveValue(i->name);
    str_values.RemoveValue(i->name);
  }

  return true;
}

ret_t Style::Output(wbuffer_t* wbuffer) {
  uint32_t size = 0;

  size = uint_values.Size() + int_values.Size() + str_values.Size() + bin_values.Size();
  log_debug("  size=%d widget_type=%s name=%s state=%s\n", size, this->widget_type.c_str(),
            this->name.c_str(), this->state.c_str());

  wbuffer_write_uint32(wbuffer, size);
  int_values.WriteToWbuffer(wbuffer);
  uint_values.WriteToWbuffer(wbuffer);
  str_values.WriteToWbuffer(wbuffer);
  bin_values.WriteToWbuffer(wbuffer);

  return RET_OK;
}

bool ThemeGen::AddStyle(const Style& style) {
  this->styles.push_back(style);

  return true;
}

ret_t ThemeGen::Output(wbuffer_t* wbuffer) {
  uint32_t nr = this->styles.size();
  return_value_if_fail(nr > 0, RET_FAIL);
  uint32_t data_start = sizeof(theme_header_t) + nr * sizeof(theme_item_t);
  theme_header_t* header = (theme_header_t*)TKMEM_ALLOC(data_start);
  memset(header, 0, data_start);
  theme_item_t* item = (theme_item_t*)((uint8_t*)header + sizeof(theme_header_t));
  wbuffer_write_binary(wbuffer, header, data_start);

  header->magic = THEME_MAGIC;
  header->version = 0;
  header->nr = nr;

  for (vector<Style>::iterator iter = this->styles.begin(); iter != this->styles.end(); iter++) {
    item->offset = wbuffer->cursor;
    tk_strncpy(item->state, iter->state.c_str(), TK_NAME_LEN);
    tk_strncpy(item->name, iter->name.c_str(), TK_NAME_LEN);
    tk_strncpy(item->widget_type, iter->widget_type.c_str(), TK_NAME_LEN);
    iter->Output(wbuffer);
    item++;
  }

  int32_t size = wbuffer->cursor;
  wbuffer->cursor = 0;
  wbuffer_write_binary(wbuffer, header, data_start);
  wbuffer->cursor = size;
  TKMEM_FREE(header);
  return RET_OK;
}
