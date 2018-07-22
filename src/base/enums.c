/**
 * File:   enums.c
 * Author: AWTK Develop Team
 * Brief:  enumerations
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/edit.h"
#include "base/enums.h"
#include "base/widget.h"
#include "base/window_animator.h"
#include "base/resource_manager.h"

static const key_type_value_t widget_type_value[] = {
    {WIDGET_TYPE_NONE, 0, WIDGET_NONE},
    {WIDGET_TYPE_WINDOW_MANAGER, 0, WIDGET_WINDOW_MANAGER},
    {WIDGET_TYPE_NORMAL_WINDOW, 0, WIDGET_NORMAL_WINDOW},
    {WIDGET_TYPE_TOOL_BAR, 0, WIDGET_TOOL_BAR},
    {WIDGET_TYPE_DIALOG, 0, WIDGET_DIALOG},
    {WIDGET_TYPE_POPUP, 0, WIDGET_POPUP},
    {WIDGET_TYPE_SPRITE, 0, WIDGET_SPRITE},
    {WIDGET_TYPE_KEYBOARD, 0, WIDGET_KEYBOARD},
    {WIDGET_TYPE_DND, 0, WIDGET_DND},
    {WIDGET_TYPE_DRAGGER, 0, WIDGET_DRAGGER},
    {WIDGET_TYPE_LABEL, 0, WIDGET_LABEL},
    {WIDGET_TYPE_BUTTON, 0, WIDGET_BUTTON},
    {WIDGET_TYPE_IMAGE, 0, WIDGET_IMAGE},
    {WIDGET_TYPE_EDIT, 0, WIDGET_EDIT},
    {WIDGET_TYPE_PROGRESS_BAR, 0, WIDGET_PROGRESS_BAR},
    {WIDGET_TYPE_GROUP_BOX, 0, WIDGET_GROUP_BOX},
    {WIDGET_TYPE_SLIDER, 0, WIDGET_SLIDER},
    {WIDGET_TYPE_PAGES, 0, WIDGET_PAGES},
    {WIDGET_TYPE_TAB_BUTTON, 0, WIDGET_TAB_BUTTON},
    {WIDGET_TYPE_BUTTON_GROUP, 0, WIDGET_BUTTON_GROUP},
    {WIDGET_TYPE_SPIN_BOX, 0, WIDGET_SPIN_BOX},
    {WIDGET_TYPE_KEYBOARD, 0, WIDGET_KEYBOARD},
    {WIDGET_TYPE_VIEW, 0, WIDGET_VIEW},
    {WIDGET_TYPE_CANDIDATES, 0, WIDGET_CANDIDATES},
    {WIDGET_TYPE_SLIDE_VIEW, 0, WIDGET_SLIDE_VIEW},
    {WIDGET_TYPE_CHECK_BUTTON, 0, WIDGET_CHECK_BUTTON},
    {WIDGET_TYPE_RADIO_BUTTON, 0, WIDGET_RADIO_BUTTON},
    {WIDGET_TYPE_DIALOG_TITLE, 0, WIDGET_DIALOG_TITLE},
    {WIDGET_TYPE_VIEW, 0, WIDGET_VIEW},
    {WIDGET_TYPE_SCROLL_BAR, 0, WIDGET_SCROLL_BAR},
    {WIDGET_TYPE_SCROLL_BAR_MOBILE, 0, WIDGET_SCROLL_BAR_MOBILE},
    {WIDGET_TYPE_SCROLL_BAR_DESKTOP, 0, WIDGET_SCROLL_BAR_DESKTOP},
    {WIDGET_TYPE_SCROLL_VIEW, 0, WIDGET_SCROLL_VIEW},
    {WIDGET_TYPE_LIST_VIEW, 0, WIDGET_LIST_VIEW},
    {WIDGET_TYPE_LIST_VIEW_H, 0, WIDGET_LIST_VIEW_H},
    {WIDGET_TYPE_LIST_ITEM, 0, WIDGET_LIST_ITEM},
    {WIDGET_TYPE_DIALOG_CLIENT, 0, WIDGET_DIALOG_CLIENT}};

static const key_type_value_t style_id_name_value[] = {
    {"bg_color", TYPE_COLOR, STYLE_ID_BG_COLOR},
    {"fg_color", TYPE_COLOR, STYLE_ID_FG_COLOR},
    {"text_color", TYPE_COLOR, STYLE_ID_TEXT_COLOR},
    {"tips_text_color", TYPE_COLOR, STYLE_ID_TIPS_TEXT_COLOR},
    {"border_color", TYPE_COLOR, STYLE_ID_BORDER_COLOR},
    {"border", TYPE_COLOR, STYLE_ID_BORDER},
    {"font_name", TYPE_STRING, STYLE_ID_FONT_NAME},
    {"font_size", TYPE_INT, STYLE_ID_FONT_SIZE},
    {"font_style", TYPE_INT, STYLE_ID_FONT_STYLE},
    {"text_align_h", TYPE_INT, STYLE_ID_TEXT_ALIGN_H},
    {"text_align_v", TYPE_INT, STYLE_ID_TEXT_ALIGN_V},
    {"bg_image", TYPE_STRING, STYLE_ID_BG_IMAGE},
    {"bg_image_draw_type", TYPE_INT, STYLE_ID_BG_IMAGE_DRAW_TYPE},
    {"fg_image", TYPE_STRING, STYLE_ID_FG_IMAGE},
    {"fg_image_draw_type", TYPE_INT, STYLE_ID_FG_IMAGE_DRAW_TYPE},
    {"icon", TYPE_STRING, STYLE_ID_ICON},
    {"icon_at", TYPE_STRING, STYLE_ID_ICON_AT},
    {"margin", TYPE_INT, STYLE_ID_MARGIN}};

static const key_type_value_t widget_state_name_value[] = {
    {"normal", 0, WIDGET_STATE_NORMAL},       {"over", 0, WIDGET_STATE_OVER},
    {"pressed", 0, WIDGET_STATE_PRESSED},     {"disable", 0, WIDGET_STATE_DISABLE},
    {"focused", 0, WIDGET_STATE_FOCUSED},     {"checked", 0, WIDGET_STATE_CHECKED},
    {"unchecked", 0, WIDGET_STATE_UNCHECKED}, {"error", 0, WIDGET_STATE_ERROR},
    {"selected", 0, WIDGET_STATE_SELECTED},   {"empty", 0, WIDGET_STATE_EMPTY}};

static const key_type_value_t align_v_name_value[] = {
    {"top", 0, ALIGN_V_TOP}, {"middle", 0, ALIGN_V_MIDDLE}, {"bottom", 0, ALIGN_V_BOTTOM}};

static const key_type_value_t input_type_name_value[] = {
    {"int", 0, INPUT_INT},       {"float", 0, INPUT_FLOAT}, {"uint", 0, INPUT_UINT},
    {"ufloat", 0, INPUT_UFLOAT}, {"text", 0, INPUT_TEXT},   {"password", 0, INPUT_PASSWORD},
    {"hex", 0, INPUT_HEX},       {"email", 0, INPUT_EMAIL}, {"phone", 0, INPUT_PHONE},
};

static const key_type_value_t align_h_name_value[] = {
    {"left", 0, ALIGN_H_LEFT}, {"center", 0, ALIGN_H_CENTER}, {"right", 0, ALIGN_H_RIGHT}};

static const key_type_value_t resource_type_value[] = {
    {"font", 0, RESOURCE_TYPE_FONT},       {"image", 0, RESOURCE_TYPE_IMAGE},
    {"theme", 0, RESOURCE_TYPE_THEME},     {"ui", 0, RESOURCE_TYPE_UI},
    {"strings", 0, RESOURCE_TYPE_STRINGS}, {"xml", 0, RESOURCE_TYPE_XML},
    {"data", 0, RESOURCE_TYPE_DATA},
};

static const key_type_value_t image_draw_type_name_value[] = {
    {"default", 0, IMAGE_DRAW_DEFAULT},
    {"icon", 0, IMAGE_DRAW_ICON},
    {"center", 0, IMAGE_DRAW_CENTER},
    {"scale", 0, IMAGE_DRAW_SCALE},
    {"scale_auto", 0, IMAGE_DRAW_SCALE_AUTO},
    {"scale_w", 0, IMAGE_DRAW_SCALE_W},
    {"scale_h", 0, IMAGE_DRAW_SCALE_H},
    {"repeat", 0, IMAGE_DRAW_REPEAT},
    {"repeat_x", 0, IMAGE_DRAW_REPEAT_X},
    {"repeat_y", 0, IMAGE_DRAW_REPEAT_Y},
    {"9patch", 0, IMAGE_DRAW_9PATCH},
    {"3patch_x", 0, IMAGE_DRAW_3PATCH_X},
    {"3patch_y", 0, IMAGE_DRAW_3PATCH_Y},
    {"3patch_x_scale_y", 0, IMAGE_DRAW_3PATCH_X_SCALE_Y},
    {"3patch_y_scale_x", 0, IMAGE_DRAW_3PATCH_Y_SCALE_X}};

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

const key_type_value_t* widget_type_find(const char* name) {
  return find_item(widget_type_value, ARRAY_SIZE(widget_type_value), name);
}

const key_type_value_t* widget_type_find_by_value(uint32_t value) {
  return find_item_by_value(widget_type_value, ARRAY_SIZE(widget_type_value), value);
}

const key_type_value_t* style_id_find(const char* name) {
  return find_item(style_id_name_value, ARRAY_SIZE(style_id_name_value), name);
}

const key_type_value_t* style_id_find_by_value(uint32_t value) {
  return find_item_by_value(style_id_name_value, ARRAY_SIZE(style_id_name_value), value);
}

const key_type_value_t* widget_state_find(const char* name) {
  return find_item(widget_state_name_value, ARRAY_SIZE(widget_state_name_value), name);
}

const key_type_value_t* align_v_type_find(const char* name) {
  return find_item(align_v_name_value, ARRAY_SIZE(align_v_name_value), name);
}

const key_type_value_t* align_h_type_find(const char* name) {
  return find_item(align_h_name_value, ARRAY_SIZE(align_h_name_value), name);
}

const key_type_value_t* resource_type_find(const char* name) {
  return find_item(resource_type_value, ARRAY_SIZE(resource_type_value), name);
}

const key_type_value_t* resource_type_find_by_value(uint32_t value) {
  return find_item_by_value(resource_type_value, ARRAY_SIZE(resource_type_value), value);
}

const key_type_value_t* image_draw_type_find(const char* name) {
  return find_item(image_draw_type_name_value, ARRAY_SIZE(image_draw_type_name_value), name);
}

const key_type_value_t* image_draw_type_find_by_value(uint32_t value) {
  return find_item_by_value(image_draw_type_name_value, ARRAY_SIZE(image_draw_type_name_value),
                            value);
}

const key_type_value_t* input_type_find(const char* name) {
  return find_item(input_type_name_value, ARRAY_SIZE(input_type_name_value), name);
}
