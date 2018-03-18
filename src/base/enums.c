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
#include "base/resource_manager.h"

static const key_type_value_t widget_name_value[] = {{"widget", 0, WIDGET_NONE},
                                                     {"window_manager", 0, WIDGET_WINDOW_MANAGER},
                                                     {"window", 0, WIDGET_NORMAL_WINDOW},
                                                     {"tool_bar", 0, WIDGET_TOOL_BAR},
                                                     {"dialog", 0, WIDGET_DIALOG},
                                                     {"popup", 0, WIDGET_POPUP},
                                                     {"sprite", 0, WIDGET_SPRITE},
                                                     {"keyboard", 0, WIDGET_KEYBOARD},
                                                     {"dragger", 0, WIDGET_DRAGGER},
                                                     {"label", 0, WIDGET_LABEL},
                                                     {"button", 0, WIDGET_BUTTON},
                                                     {"image", 0, WIDGET_IMAGE},
                                                     {"edit", 0, WIDGET_EDIT},
                                                     {"progress_bar", 0, WIDGET_PROGRESS_BAR},
                                                     {"group_box", 0, WIDGET_GROUP_BOX},
                                                     {"check_button", 0, WIDGET_CHECK_BUTTON},
                                                     {"radio_button", 0, WIDGET_RADIO_BUTTON}};

static const key_type_value_t style_name_value[] = {
    {"bg-color", TYPE_COLOR, STYLE_ID_BG_COLOR},
    {"fg-color", TYPE_COLOR, STYLE_ID_FG_COLOR},
    {"text-color", TYPE_COLOR, STYLE_ID_TEXT_COLOR},
    {"border-color", TYPE_COLOR, STYLE_ID_BORDER_COLOR},
    {"font-name", TYPE_STRING, STYLE_ID_FONT_NAME},
    {"font-size", TYPE_INT, STYLE_ID_FONT_SIZE},
    {"font-style", TYPE_INT, STYLE_ID_FONT_STYLE},
    {"text-align-h", TYPE_INT, STYLE_ID_TEXT_ALIGN_H},
    {"text-align-v", TYPE_INT, STYLE_ID_TEXT_ALIGN_V},
    {"icon", TYPE_STRING, STYLE_ID_ICON},
};

static const key_type_value_t state_name_value[] = {
    {"normal", 0, WIDGET_STATE_NORMAL},      {"over", 0, WIDGET_STATE_OVER},
    {"pressed", 0, WIDGET_STATE_PRESSED},    {"disable", 0, WIDGET_STATE_DISABLE},
    {"focused", 0, WIDGET_STATE_FOCUSED},    {"checked", 0, WIDGET_STATE_CHECKED},
    {"unchecked", 0, WIDGET_STATE_UNCHECKED}};

static const key_type_value_t align_v_name_value[] = {
    {"top", 0, ALIGN_V_TOP}, {"middle", 0, ALIGN_V_MIDDLE}, {"bottom", 0, ALIGN_V_BOTTOM}};

static const key_type_value_t align_h_name_value[] = {
    {"left", 0, ALIGN_H_LEFT}, {"center", 0, ALIGN_H_CENTER}, {"right", 0, ALIGN_H_RIGHT}};

static const key_type_value_t resource_type_name_value[] = {
    {"font", 0, RESOURCE_TYPE_FONT},
    {"image", 0, RESOURCE_TYPE_IMAGE},
    {"theme", 0, RESOURCE_TYPE_THEME},
    {"ui", 0, RESOURCE_TYPE_UI},
    {"xml", 0, RESOURCE_TYPE_XML},
    {"data", 0, RESOURCE_TYPE_DATA},
};

const key_type_value_t* find_item(const key_type_value_t* items, uint32_t nr, const char* name) {
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

const key_type_value_t* find_item_by_value(const key_type_value_t* items, uint32_t nr,
                                           uint32_t value) {
  uint32_t i = 0;
  return_value_if_fail(items != NULL, NULL);

  for (i = 0; i < nr; i++) {
    const key_type_value_t* iter = items + i;
    if (iter->value == value) {
      return iter;
    }
  }

  return NULL;
}

const key_type_value_t* widget_name_find(const char* name) {
  return find_item(widget_name_value, ARRAY_SIZE(widget_name_value), name);
}

const key_type_value_t* widget_name_find_by_value(uint32_t value) {
  return find_item_by_value(widget_name_value, ARRAY_SIZE(widget_name_value), value);
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

const key_type_value_t* resouce_type_name_find(const char* name) {
  return find_item(resource_type_name_value, ARRAY_SIZE(resource_type_name_value), name);
}

const key_type_value_t* resouce_type_name_find_by_value(uint32_t value) {
  return find_item_by_value(resource_type_name_value, ARRAY_SIZE(resource_type_name_value), value);
}
