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

bool Style::AddInt(const string& name, int32_t value) {
  for (vector<NameIntValue>::iterator i = this->int_values.begin(); i != this->int_values.end();
       i++) {
    if (i->name == name) {
      i->value = value;
      return true;
    }
  }

  this->int_values.push_back(NameIntValue(name, value));

  return true;
}

bool Style::AddString(const string& name, const string& value) {
  for (vector<NameStringValue>::iterator i = this->str_values.begin(); i != this->str_values.end();
       i++) {
    if (i->name == name) {
      i->value = value;
      return true;
    }
  }

  this->str_values.push_back(NameStringValue(name, value));

  return true;
}

bool Style::Reset() {
  this->int_values.clear();
  this->str_values.clear();

  return true;
}

bool Style::Merge(Style& other) {
  for (vector<NameIntValue>::iterator i = other.int_values.begin(); i != other.int_values.end();
       i++) {
    this->AddInt(i->name, i->value);
  }

  for (vector<NameStringValue>::iterator i = other.str_values.begin(); i != other.str_values.end();
       i++) {
    this->AddString(i->name, i->value);
  }

  return true;
}

static uint8_t* write_str(uint8_t* buff, const string& str) {
  memcpy(buff, str.c_str(), str.size() + 1);

  return buff + str.size() + 1;
}

static uint8_t* write_binary(uint8_t* buff, void* data, uint32_t size) {
  memcpy(buff, data, size);

  return buff + size;
}

static uint8_t* write_uint32(uint8_t* buff, uint32_t value) {
  save_uint32(buff, value);

  return buff;
}

ret_t Style::Output(wbuffer_t* wbuffer) {
  uint32_t size = 0;
  style_name_value_header_t nv;

  size = this->int_values.size() + this->str_values.size();
  printf("  size=%d widget_type=%s name=%s state=%s\n", size, this->widget_type.c_str(),
         this->name.c_str(), this->state.c_str());

  wbuffer_write_uint32(wbuffer, size);
  for (vector<NameIntValue>::iterator i = this->int_values.begin(); i != this->int_values.end();
       i++) {
    const string& name = i->name;
    uint32_t value = i->value;

    nv.type = VALUE_TYPE_UINT32;
    nv.name_size = name.size() + 1;
    nv.value_size = sizeof(value);

    wbuffer_write_binary(wbuffer, &nv, sizeof(nv));
    wbuffer_write_string(wbuffer, name.c_str());
    wbuffer_write_uint32(wbuffer, value);

    printf("    %s=0x%08x\n", name.c_str(), value);
  }

  size = this->str_values.size();
  for (vector<NameStringValue>::iterator i = this->str_values.begin(); i != this->str_values.end();
       i++) {
    const string& name = i->name;
    const string& value = i->value;

    nv.type = VALUE_TYPE_STRING;
    nv.name_size = name.size() + 1;
    nv.value_size = value.size() + 1;

    wbuffer_write_binary(wbuffer, &nv, sizeof(nv));
    wbuffer_write_string(wbuffer, name.c_str());
    wbuffer_write_string(wbuffer, value.c_str());

    printf("    %s=%s\n", name.c_str(), value.c_str());
  }

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
