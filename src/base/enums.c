/**
 * File:   enums.c
 * Author: AWTK Develop Team
 * Brief:  enumerations
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "widgets/edit.h"
#include "base/enums.h"
#include "base/widget.h"
#include "base/window_animator.h"
#include "base/assets_manager.h"

static const key_type_value_t window_closable_name_value[] = {
    {"yes", 0, WINDOW_CLOSABLE_YES},
    {"no", 0, WINDOW_CLOSABLE_NO},
    {"confirm", 0, WINDOW_CLOSABLE_CONFIRM}};

static const key_type_value_t align_v_name_value[] = {
    {"top", 0, ALIGN_V_TOP}, {"middle", 0, ALIGN_V_MIDDLE}, {"bottom", 0, ALIGN_V_BOTTOM}};

static const key_type_value_t input_type_name_value[] = {
    {"int", 0, INPUT_INT},       {"float", 0, INPUT_FLOAT}, {"uint", 0, INPUT_UINT},
    {"ufloat", 0, INPUT_UFLOAT}, {"text", 0, INPUT_TEXT},   {"password", 0, INPUT_PASSWORD},
    {"hex", 0, INPUT_HEX},       {"email", 0, INPUT_EMAIL}, {"phone", 0, INPUT_PHONE},
    {"custom", 0, INPUT_CUSTOM},
};

static const key_type_value_t align_h_name_value[] = {
    {"left", 0, ALIGN_H_LEFT}, {"center", 0, ALIGN_H_CENTER}, {"right", 0, ALIGN_H_RIGHT}};

static const key_type_value_t asset_type_value[] = {
    {"font", 0, ASSET_TYPE_FONT},       {"image", 0, ASSET_TYPE_IMAGE},
    {"style", 0, ASSET_TYPE_STYLE},     {"ui", 0, ASSET_TYPE_UI},
    {"strings", 0, ASSET_TYPE_STRINGS}, {"xml", 0, ASSET_TYPE_XML},
    {"data", 0, ASSET_TYPE_DATA},
};

static const key_type_value_t image_draw_type_name_value[] = {
    {"default", 0, IMAGE_DRAW_DEFAULT},
    {"icon", 0, IMAGE_DRAW_ICON},
    {"center", 0, IMAGE_DRAW_CENTER},
    {"scale", 0, IMAGE_DRAW_SCALE},
    {"scale_auto", 0, IMAGE_DRAW_SCALE_AUTO},
    {"scale_down", 0, IMAGE_DRAW_SCALE_DOWN},
    {"scale_w", 0, IMAGE_DRAW_SCALE_W},
    {"scale_h", 0, IMAGE_DRAW_SCALE_H},
    {"repeat", 0, IMAGE_DRAW_REPEAT},
    {"repeat_x", 0, IMAGE_DRAW_REPEAT_X},
    {"repeat_y", 0, IMAGE_DRAW_REPEAT_Y},
    {"patch9", 0, IMAGE_DRAW_PATCH9},
    {"patch3_x", 0, IMAGE_DRAW_PATCH3_X},
    {"patch3_y", 0, IMAGE_DRAW_PATCH3_Y},
    {"patch3_x_scale_y", 0, IMAGE_DRAW_PATCH3_X_SCALE_Y},
    {"patch3_y_scale_x", 0, IMAGE_DRAW_PATCH3_Y_SCALE_X}};

static const key_type_value_t easing_type_name_value[] = {
    {"linear", 0, EASING_LINEAR},
    {"quadratic_in", 0, EASING_QUADRATIC_IN},
    {"quadratic_out", 0, EASING_QUADRATIC_OUT},
    {"quadratic_inout", 0, EASING_QUADRATIC_INOUT},
    {"cubic_in", 0, EASING_CUBIC_IN},
    {"cubic_out", 0, EASING_CUBIC_OUT},
    {"sin_in", 0, EASING_SIN_IN},
    {"sin_out", 0, EASING_SIN_OUT},
    {"sin_inout", 0, EASING_SIN_INOUT},
    {"pow_in", 0, EASING_POW_IN},
    {"pow_out", 0, EASING_POW_OUT},
    {"pow_inout", 0, EASING_POW_INOUT},
    {"circular_in", 0, EASING_CIRCULAR_IN},
    {"circular_out", 0, EASING_CIRCULAR_OUT},
    {"circular_inout", 0, EASING_CIRCULAR_INOUT},
    {"elastic_in", 0, EASING_ELASTIC_IN},
    {"elastic_out", 0, EASING_ELASTIC_OUT},
    {"elastic_inout", 0, EASING_ELASTIC_INOUT},
    {"back_in", 0, EASING_BACK_IN},
    {"back_out", 0, EASING_BACK_OUT},
    {"back_inout", 0, EASING_BACK_INOUT},
    {"bounce_in", 0, EASING_BOUNCE_IN},
    {"bounce_out", 0, EASING_BOUNCE_OUT},
    {"bounce_inout", 0, EASING_BOUNCE_INOUT}};

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

const key_type_value_t* window_closable_type_find(const char* name) {
  return find_item(window_closable_name_value, ARRAY_SIZE(window_closable_name_value), name);
}

const key_type_value_t* align_v_type_find(const char* name) {
  return find_item(align_v_name_value, ARRAY_SIZE(align_v_name_value), name);
}

const key_type_value_t* align_h_type_find(const char* name) {
  return find_item(align_h_name_value, ARRAY_SIZE(align_h_name_value), name);
}

const key_type_value_t* asset_type_find(const char* name) {
  return find_item(asset_type_value, ARRAY_SIZE(asset_type_value), name);
}

const key_type_value_t* asset_type_find_by_value(uint32_t value) {
  return find_item_by_value(asset_type_value, ARRAY_SIZE(asset_type_value), value);
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

const key_type_value_t* easing_type_find(const char* name) {
  return find_item(easing_type_name_value, ARRAY_SIZE(easing_type_name_value), name);
}

const key_type_value_t* easing_type_find_by_value(uint32_t value) {
  return find_item_by_value(easing_type_name_value, ARRAY_SIZE(easing_type_name_value), value);
}
