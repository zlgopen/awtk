/**
 * File:   theme.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  theme interface
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_THEME_H
#define LFTK_THEME_H

#include "base/color.h"

BEGIN_C_DECLS

typedef struct _style_t {
  const uint8_t* data;
}style_t;

uint32_t    style_get_int(style_t* s, uint32_t name, uint32_t defval);
color_t     style_get_color(style_t* s, uint32_t name, color_t defval);
const char* style_get_str(style_t* s, uint32_t name, const char* defval);

typedef struct _theme_t {
  const uint8_t* data;
}theme_t;

theme_t* default_theme(void);
theme_t* default_theme_init(const uint8_t* data);

const uint8_t* theme_find_style(theme_t* t, uint16_t type, uint8_t subtype, uint8_t state);

enum {
  E_BG_COLOR = 1,
  E_FG_COLOR,
  E_FONT_NAME,
  E_FONT_SIZE,
  E_FONT_STYLE,
  E_TEXT_COLOR,
  E_TEXT_ALIGN_H,
  E_TEXT_ALIGN_V,
  E_BORDER_COLOR,
  E_ICON
};

enum {
  ALIGN_V_NONE= 0,
  ALIGN_V_MIDDLE,
  ALIGN_V_TOP,
  ALIGN_V_BOTTOM
};

enum {
  ALIGN_H_NONE = 0,
  ALIGN_H_CENTER,
  ALIGN_H_LEFT,
  ALIGN_H_RIGHT
};

#define THEME_MAGIC 0xFAFBFCFD

END_C_DECLS

#endif/*LFTK_THEME_H*/

