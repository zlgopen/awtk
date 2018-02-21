/**
 * File:   enums.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  enumerations
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/enums.h"
#include "base/widget.h"

static const key_type_value_t widget_name_value[] = {{"widget", 0, WIDGET_NONE},
                                                     {"dialog", 0, WIDGET_DIALOG},
                                                     {"window", 0, WIDGET_NORMAL_WINDOW},
                                                     {"label", 0, WIDGET_LABEL},
                                                     {"button", 0, WIDGET_BUTTON},
                                                     {"image", 0, WIDGET_IMAGE},
                                                     {"edit", 0, WIDGET_EDIT},
                                                     {"check_button", 0, WIDGET_CHECK_BUTTON},
                                                     {"radio_button", 0, WIDGET_RADIO_BUTTON},
                                                     {"progress_bar", 0, WIDGET_PROGRESS_BAR}};

static const key_type_value_t style_name_value[] = {
    {"bg-color", TYPE_COLOR, E_BG_COLOR},       {"fg-color", TYPE_COLOR, E_FG_COLOR},
    {"text-color", TYPE_COLOR, E_TEXT_COLOR},   {"border-color", TYPE_COLOR, E_BORDER_COLOR},
    {"font-name", TYPE_STRING, E_FONT_NAME},    {"font-size", TYPE_INT, E_FONT_SIZE},
    {"font-style", TYPE_INT, E_FONT_STYLE},     {"text-align-h", TYPE_INT, E_TEXT_ALIGN_H},
    {"text-align-v", TYPE_INT, E_TEXT_ALIGN_V}, {"icon", TYPE_STRING, E_ICON},
};

static const key_type_value_t state_name_value[] = {
    {"normal", 0, WIDGET_STATE_NORMAL},   {"over", 0, WIDGET_STATE_OVER},
    {"pressed", 0, WIDGET_STATE_PRESSED}, {"disable", 0, WIDGET_STATE_DISABLE},
    {"focus", 0, WIDGET_STATE_FOCUS},     {"selected", 0, WIDGET_STATE_SELECTED},
    {"checked", 0, WIDGET_STATE_CHECKED}, {"unchecked", 0, WIDGET_STATE_UNCHECKED}};

static const key_type_value_t align_v_name_value[] = {
    {"top", 0, ALIGN_V_TOP}, {"middle", 0, ALIGN_V_MIDDLE}, {"bottom", 0, ALIGN_V_BOTTOM}};

static const key_type_value_t align_h_name_value[] = {
    {"left", 0, ALIGN_H_LEFT}, {"center", 0, ALIGN_H_CENTER}, {"right", 0, ALIGN_H_RIGHT}};

static const key_type_value_t* find_item(const key_type_value_t* items, uint32_t nr,
                                         const char* name) {
  uint32_t i = 0;
  return_value_if_fail(items != NULL && name != NULL, NULL);

  for (i = 0; i < nr; i++) {
    const key_type_value_t* iter = items + i;
    if (strcmp(iter->name, name) == 0) {
      return iter;
    }
  }

  return NULL;
}

const key_type_value_t* widget_name_find(const char* name) {
  return find_item(widget_name_value, ARRAY_SIZE(widget_name_value), name);
}

const key_type_value_t* style_name_find(const char* name) {
  return find_item(style_name_value, ARRAY_SIZE(style_name_value), name);
}

const key_type_value_t* state_name_find(const char* name) {
  return find_item(state_name_value, ARRAY_SIZE(state_name_value), name);
}

const key_type_value_t* align_v_name_find(const char* name) {
  return find_item(align_v_name_value, ARRAY_SIZE(align_v_name_value), name);
}

const key_type_value_t* align_h_name_find(const char* name) {
  return find_item(align_h_name_value, ARRAY_SIZE(align_h_name_value), name);
}
