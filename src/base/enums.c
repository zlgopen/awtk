/**
 * File:   enums.c
 * Author: AWTK Develop Team
 * Brief:  enumerations
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "base/keys.h"
#include "base/enums.h"
#include "base/widget.h"
#include "base/input_method.h"
#include "base/window_animator.h"
#include "base/assets_manager.h"

static const key_type_value_t window_closable_name_value[] = {
    {"yes", 0, WINDOW_CLOSABLE_YES},
    {"no", 0, WINDOW_CLOSABLE_NO},
    {"confirm", 0, WINDOW_CLOSABLE_CONFIRM}};

static const key_type_value_t align_v_name_value[] = {
    {"top", 0, ALIGN_V_TOP}, {"middle", 0, ALIGN_V_MIDDLE}, {"bottom", 0, ALIGN_V_BOTTOM}};

static const key_type_value_t input_type_name_value[] = {
    {"int", 0, INPUT_INT},
    {"float", 0, INPUT_FLOAT},
    {"uint", 0, INPUT_UINT},
    {"ufloat", 0, INPUT_UFLOAT},
    {"text", 0, INPUT_TEXT},
    {"password", 0, INPUT_PASSWORD},
    {"hex", 0, INPUT_HEX},
    {"email", 0, INPUT_EMAIL},
    {"phone", 0, INPUT_PHONE},
    {"date", 0, INPUT_DATE},
    {"time", 0, INPUT_TIME},
    {"time_full", 0, INPUT_TIME_FULL},
    {"ipv4", 0, INPUT_IPV4},
    {"custom", 0, INPUT_CUSTOM},
    {"custom_password", 0, INPUT_CUSTOM_PASSWORD},
};

static const key_type_value_t align_h_name_value[] = {
    {"left", 0, ALIGN_H_LEFT}, {"center", 0, ALIGN_H_CENTER}, {"right", 0, ALIGN_H_RIGHT}};

static const key_type_value_t asset_type_value[] = {
    {"font", 0, ASSET_TYPE_FONT},       {"image", 0, ASSET_TYPE_IMAGE},
    {"style", 0, ASSET_TYPE_STYLE},     {"ui", 0, ASSET_TYPE_UI},
    {"strings", 0, ASSET_TYPE_STRINGS}, {"xml", 0, ASSET_TYPE_XML},
    {"data", 0, ASSET_TYPE_DATA},       {"script", 0, ASSET_TYPE_SCRIPT},
};

static const key_type_value_t image_draw_type_name_value[] = {
    {"default", 0, IMAGE_DRAW_DEFAULT},
    {"icon", 0, IMAGE_DRAW_ICON},
    {"center", 0, IMAGE_DRAW_CENTER},
    {"scale", 0, IMAGE_DRAW_SCALE},
    {"auto", 0, IMAGE_DRAW_SCALE_AUTO},
    {"scale_auto", 0, IMAGE_DRAW_SCALE_AUTO},
    {"scale_down", 0, IMAGE_DRAW_SCALE_DOWN},
    {"scale_w", 0, IMAGE_DRAW_SCALE_W},
    {"scale_h", 0, IMAGE_DRAW_SCALE_H},
    {"repeat", 0, IMAGE_DRAW_REPEAT},
    {"repeat_x", 0, IMAGE_DRAW_REPEAT_X},
    {"repeat_y", 0, IMAGE_DRAW_REPEAT_Y},
    {"repeat_y_inverse", 0, IMAGE_DRAW_REPEAT_Y_INVERSE},
    {"patch9", 0, IMAGE_DRAW_PATCH9},
    {"patch3_x", 0, IMAGE_DRAW_PATCH3_X},
    {"patch3_y", 0, IMAGE_DRAW_PATCH3_Y},
    {"patch3_x_scale_y", 0, IMAGE_DRAW_PATCH3_X_SCALE_Y},
    {"patch3_y_scale_x", 0, IMAGE_DRAW_PATCH3_Y_SCALE_X},
    {"repeat9", 0, IMAGE_DRAW_REPEAT9},
    {"repeat3_x", 0, IMAGE_DRAW_REPEAT3_X},
    {"repeat3_y", 0, IMAGE_DRAW_REPEAT3_Y}};

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

static const key_type_value_t keys_type_name_value[] = {
    {"RETURN", 0, TK_KEY_RETURN},
    {"ESCAPE", 0, TK_KEY_ESCAPE},
    {"BACKSPACE", 0, TK_KEY_BACKSPACE},
    {"TAB", 0, TK_KEY_TAB},
    {"SPACE", 0, TK_KEY_SPACE},
    {"EXCLAIM", 0, TK_KEY_EXCLAIM},
    {"QUOTEDBL", 0, TK_KEY_QUOTEDBL},
    {"HASH", 0, TK_KEY_HASH},
    {"PERCENT", 0, TK_KEY_PERCENT},
    {"DOLLAR", 0, TK_KEY_DOLLAR},
    {"AMPERSAND", 0, TK_KEY_AMPERSAND},
    {"QUOTE", 0, TK_KEY_QUOTE},
    {"LEFTPAREN", 0, TK_KEY_LEFTPAREN},
    {"RIGHTPAREN", 0, TK_KEY_RIGHTPAREN},
    {"ASTERISK", 0, TK_KEY_ASTERISK},
    {"PLUS", 0, TK_KEY_PLUS},
    {"COMMA", 0, TK_KEY_COMMA},
    {"MINUS", 0, TK_KEY_MINUS},
    {"PERIOD", 0, TK_KEY_PERIOD},
    {"SLASH", 0, TK_KEY_SLASH},
    {"0", 0, TK_KEY_0},
    {"1", 0, TK_KEY_1},
    {"2", 0, TK_KEY_2},
    {"3", 0, TK_KEY_3},
    {"4", 0, TK_KEY_4},
    {"5", 0, TK_KEY_5},
    {"6", 0, TK_KEY_6},
    {"7", 0, TK_KEY_7},
    {"8", 0, TK_KEY_8},
    {"9", 0, TK_KEY_9},
    {"COLON", 0, TK_KEY_COLON},
    {"SEMICOLON", 0, TK_KEY_SEMICOLON},
    {"LESS", 0, TK_KEY_LESS},
    {"EQUAL", 0, TK_KEY_EQUAL},
    {"GREATER", 0, TK_KEY_GREATER},
    {"QUESTION", 0, TK_KEY_QUESTION},
    {"AT", 0, TK_KEY_AT},
    {"LEFTBRACKET", 0, TK_KEY_LEFTBRACKET},
    {"BACKSLASH", 0, TK_KEY_BACKSLASH},
    {"RIGHTBRACKET", 0, TK_KEY_RIGHTBRACKET},
    {"CARET", 0, TK_KEY_CARET},
    {"UNDERSCORE", 0, TK_KEY_UNDERSCORE},
    {"BACKQUOTE", 0, TK_KEY_BACKQUOTE},
    {"a", 0, TK_KEY_a},
    {"b", 0, TK_KEY_b},
    {"c", 0, TK_KEY_c},
    {"d", 0, TK_KEY_d},
    {"e", 0, TK_KEY_e},
    {"f", 0, TK_KEY_f},
    {"g", 0, TK_KEY_g},
    {"h", 0, TK_KEY_h},
    {"i", 0, TK_KEY_i},
    {"j", 0, TK_KEY_j},
    {"k", 0, TK_KEY_k},
    {"l", 0, TK_KEY_l},
    {"m", 0, TK_KEY_m},
    {"n", 0, TK_KEY_n},
    {"o", 0, TK_KEY_o},
    {"p", 0, TK_KEY_p},
    {"q", 0, TK_KEY_q},
    {"r", 0, TK_KEY_r},
    {"s", 0, TK_KEY_s},
    {"t", 0, TK_KEY_t},
    {"u", 0, TK_KEY_u},
    {"v", 0, TK_KEY_v},
    {"w", 0, TK_KEY_w},
    {"x", 0, TK_KEY_x},
    {"y", 0, TK_KEY_y},
    {"z", 0, TK_KEY_z},
    {"A", 0, TK_KEY_A},
    {"B", 0, TK_KEY_B},
    {"C", 0, TK_KEY_C},
    {"D", 0, TK_KEY_D},
    {"E", 0, TK_KEY_E},
    {"F", 0, TK_KEY_F},
    {"G", 0, TK_KEY_G},
    {"H", 0, TK_KEY_H},
    {"I", 0, TK_KEY_I},
    {"J", 0, TK_KEY_J},
    {"K", 0, TK_KEY_K},
    {"L", 0, TK_KEY_L},
    {"M", 0, TK_KEY_M},
    {"N", 0, TK_KEY_N},
    {"O", 0, TK_KEY_O},
    {"P", 0, TK_KEY_P},
    {"Q", 0, TK_KEY_Q},
    {"R", 0, TK_KEY_R},
    {"S", 0, TK_KEY_S},
    {"T", 0, TK_KEY_T},
    {"U", 0, TK_KEY_U},
    {"V", 0, TK_KEY_V},
    {"W", 0, TK_KEY_W},
    {"X", 0, TK_KEY_X},
    {"Y", 0, TK_KEY_Y},
    {"Z", 0, TK_KEY_Z},
    {"DOT", 0, TK_KEY_DOT},
    {"DELETE", 0, TK_KEY_DELETE},
    {"LEFTBRACE", 0, TK_KEY_LEFTBRACE},
    {"RIGHTBRACE", 0, TK_KEY_RIGHTBRACE},
    {"BACK", 0, TK_KEY_BACK},
    {"CANCEL", 0, TK_KEY_CANCEL},
#ifdef SDL2
    {"PRINTSCREEN", 0, TK_KEY_PRINTSCREEN},
    {"SCROLLLOCK", 0, TK_KEY_SCROLLLOCK},
    {"PAUSE", 0, TK_KEY_PAUSE},
    {"NUMLOCKCLEAR", 0, TK_KEY_NUMLOCKCLEAR},
    {"APPLICATION", 0, TK_KEY_APPLICATION},
    {"POWER", 0, TK_KEY_POWER},
    {"F13", 0, TK_KEY_F13},
    {"F14", 0, TK_KEY_F14},
    {"F15", 0, TK_KEY_F15},
    {"F16", 0, TK_KEY_F16},
    {"F17", 0, TK_KEY_F17},
    {"F18", 0, TK_KEY_F18},
    {"F19", 0, TK_KEY_F19},
    {"F20", 0, TK_KEY_F20},
    {"F21", 0, TK_KEY_F21},
    {"F22", 0, TK_KEY_F22},
    {"F23", 0, TK_KEY_F23},
    {"F24", 0, TK_KEY_F24},
    {"EXECUTE", 0, TK_KEY_EXECUTE},
    {"HELP", 0, TK_KEY_HELP},
    {"MENU", 0, TK_KEY_MENU},
    {"SELECT", 0, TK_KEY_SELECT},
    {"STOP", 0, TK_KEY_STOP},
    {"AGAIN", 0, TK_KEY_AGAIN},
    {"UNDO", 0, TK_KEY_UNDO},
    {"CUT", 0, TK_KEY_CUT},
    {"COPY", 0, TK_KEY_COPY},
    {"PASTE", 0, TK_KEY_PASTE},
    {"FIND", 0, TK_KEY_FIND},
    {"MUTE", 0, TK_KEY_MUTE},
    {"VOLUMEUP", 0, TK_KEY_VOLUMEUP},
    {"VOLUMEDOWN", 0, TK_KEY_VOLUMEDOWN},
    {"ALTERASE", 0, TK_KEY_ALTERASE},
    {"SYSREQ", 0, TK_KEY_SYSREQ},
    {"CLEAR", 0, TK_KEY_CLEAR},
    {"PRIOR", 0, TK_KEY_PRIOR},
    {"RETURN2", 0, TK_KEY_RETURN2},
    {"SEPARATOR", 0, TK_KEY_SEPARATOR},
    {"OUT", 0, TK_KEY_OUT},
    {"OPER", 0, TK_KEY_OPER},
    {"CLEARAGAIN", 0, TK_KEY_CLEARAGAIN},
    {"CRSEL", 0, TK_KEY_CRSEL},
    {"EXSEL", 0, TK_KEY_EXSEL},
    {"THOUSANDSSEPARATOR", 0, TK_KEY_THOUSANDSSEPARATOR},
    {"DECIMALSEPARATOR", 0, TK_KEY_DECIMALSEPARATOR},
    {"CURRENCYUNIT", 0, TK_KEY_CURRENCYUNIT},
    {"CURRENCYSUBUNIT", 0, TK_KEY_CURRENCYSUBUNIT},
    {"LGUI", 0, TK_KEY_LGUI},
    {"RGUI", 0, TK_KEY_RGUI},
    {"MODE", 0, TK_KEY_MODE},
    {"AUDIONEXT", 0, TK_KEY_AUDIONEXT},
    {"AUDIOPREV", 0, TK_KEY_AUDIOPREV},
    {"AUDIOSTOP", 0, TK_KEY_AUDIOSTOP},
    {"AUDIOPLAY", 0, TK_KEY_AUDIOPLAY},
    {"AUDIOMUTE", 0, TK_KEY_AUDIOMUTE},
    {"MEDIASELECT", 0, TK_KEY_MEDIASELECT},
    {"WWW", 0, TK_KEY_WWW},
    {"MAIL", 0, TK_KEY_MAIL},
    {"CALCULATOR", 0, TK_KEY_CALCULATOR},
    {"COMPUTER", 0, TK_KEY_COMPUTER},
    {"AC_SEARCH", 0, TK_KEY_AC_SEARCH},
    {"AC_HOME", 0, TK_KEY_AC_HOME},
    {"AC_BACK", 0, TK_KEY_AC_BACK},
    {"AC_FORWARD", 0, TK_KEY_AC_FORWARD},
    {"AC_STOP", 0, TK_KEY_AC_STOP},
    {"AC_REFRESH", 0, TK_KEY_AC_REFRESH},
    {"AC_BOOKMARKS", 0, TK_KEY_AC_BOOKMARKS},
    {"BRIGHTNESSDOWN", 0, TK_KEY_BRIGHTNESSDOWN},
    {"BRIGHTNESSUP", 0, TK_KEY_BRIGHTNESSUP},
    {"DISPLAYSWITCH", 0, TK_KEY_DISPLAYSWITCH},
    {"KBDILLUMTOGGLE", 0, TK_KEY_KBDILLUMTOGGLE},
    {"KBDILLUMDOWN", 0, TK_KEY_KBDILLUMDOWN},
    {"KBDILLUMUP", 0, TK_KEY_KBDILLUMUP},
    {"EJECT", 0, TK_KEY_EJECT},
    {"SLEEP", 0, TK_KEY_SLEEP},
    {"APP1", 0, TK_KEY_APP1},
    {"APP2", 0, TK_KEY_APP2},

#endif /*SDL2*/
    {"LCTRL", 0, TK_KEY_LCTRL},
    {"RCTRL", 0, TK_KEY_RCTRL},
    {"LSHIFT", 0, TK_KEY_LSHIFT},
    {"RSHIFT", 0, TK_KEY_RSHIFT},
    {"LALT", 0, TK_KEY_LALT},
    {"RALT", 0, TK_KEY_RALT},
    {"CAPSLOCK", 0, TK_KEY_CAPSLOCK},
    {"HOME", 0, TK_KEY_HOME},
    {"END", 0, TK_KEY_END},
    {"INSERT", 0, TK_KEY_INSERT},
    {"UP", 0, TK_KEY_UP},
    {"DOWN", 0, TK_KEY_DOWN},
    {"LEFT", 0, TK_KEY_LEFT},
    {"RIGHT", 0, TK_KEY_RIGHT},
    {"PAGEUP", 0, TK_KEY_PAGEUP},
    {"PAGEDOWN", 0, TK_KEY_PAGEDOWN},
    {"F1", 0, TK_KEY_F1},
    {"F2", 0, TK_KEY_F2},
    {"F3", 0, TK_KEY_F3},
    {"F4", 0, TK_KEY_F4},
    {"F5", 0, TK_KEY_F5},
    {"F6", 0, TK_KEY_F6},
    {"F7", 0, TK_KEY_F7},
    {"F8", 0, TK_KEY_F8},
    {"F9", 0, TK_KEY_F9},
    {"F10", 0, TK_KEY_F10},
    {"F11", 0, TK_KEY_F11},
    {"F12", 0, TK_KEY_F12}};

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

const key_type_value_t* keys_type_find(const char* name) {
  char fixed_name[TK_NAME_LEN + 1];
  return_value_if_fail(name != NULL, NULL);

  memset(fixed_name, 0x00, sizeof(fixed_name));
  return find_item(keys_type_name_value, ARRAY_SIZE(keys_type_name_value),
                   tk_normalize_key_name(name, fixed_name));
}

const key_type_value_t* keys_type_find_by_value(uint32_t value) {
  return find_item_by_value(keys_type_name_value, ARRAY_SIZE(keys_type_name_value), value);
}
