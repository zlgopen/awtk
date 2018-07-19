/**
 * File:   theme.h
 * Author: AWTK Develop Team
 * Brief:  theme interface
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_THEME_H
#define TK_THEME_H

#include "base/color.h"

BEGIN_C_DECLS

/**
 * @class style_t
 * 控件风格的参数。
 */
typedef struct _style_t {
  const uint8_t* data;
} style_t;

uint32_t style_get_int(style_t* s, uint32_t name, uint32_t defval);
color_t style_get_color(style_t* s, uint32_t name, color_t defval);
const char* style_get_str(style_t* s, uint32_t name, const char* defval);

/**
 * @class theme_t
 * 主题。
 */
typedef struct _theme_t {
  const uint8_t* data;
} theme_t;

theme_t* theme(void);
theme_t* theme_init(const uint8_t* data);

typedef struct _theme_header_t {
  uint32_t magic;
  uint32_t version;
  uint32_t nr;
} theme_header_t;

typedef struct _theme_item_t {
  uint32_t offset;
  uint16_t state;
  char name[NAME_LEN + 1];
  char widget_type[NAME_LEN + 1];
} theme_item_t;

const uint8_t* theme_find_style(theme_t* t, const char* widget_type, const char* name,
                                uint16_t state);

/**
 * @enum style_type_t
 * @prefix STYLE
 * 类型常量定义。
 */
typedef enum _style_id_t {
  /**
   * @const STYLE_ID_BG_COLOR
   * 背景颜色。
   */
  STYLE_ID_BG_COLOR = 1,
  /**
   * @const STYLE_ID_BG_COLOR
   * 前景颜色。
   */
  STYLE_ID_FG_COLOR,
  /**
   * @const STYLE_ID_FONT_NAME
   * 字体名称。
   */
  STYLE_ID_FONT_NAME,
  /**
   * @const STYLE_ID_FONT_SIZE
   * 字体大小。
   */
  STYLE_ID_FONT_SIZE,
  /**
   * @const STYLE_ID_FONT_STYLE
   * 字体风格(粗体、斜体等)。
   */
  STYLE_ID_FONT_STYLE,
  /**
   * @const STYLE_ID_TEXT_COLOR
   * 文本颜色。
   */
  STYLE_ID_TEXT_COLOR,
  /**
   * @const STYLE_ID_TIPS_TEXT_COLOR
   * 提示文本颜色。
   */
  STYLE_ID_TIPS_TEXT_COLOR,
  /**
   * @const STYLE_ID_TEXT_ALIGN_H
   * 文本水平对齐的方式。
   */
  STYLE_ID_TEXT_ALIGN_H,
  /**
   * @const STYLE_ID_TEXT_ALIGN_V
   * 文本垂直对齐的方式。
   */
  STYLE_ID_TEXT_ALIGN_V,
  /**
   * @const STYLE_ID_BORDER_COLOR
   * 边框颜色。
   */
  STYLE_ID_BORDER_COLOR,
  /**
   * @const STYLE_ID_BORDER
   * 边框类型。
   */
  STYLE_ID_BORDER,
  /**
   * @const STYLE_ID_BG_IMAGE
   * 图片的名称。
   */
  STYLE_ID_BG_IMAGE,
  /**
   * @const STYLE_ID_BG_IMAGE_DRAW_TYPE
   * 图片的显示方式。
   */
  STYLE_ID_BG_IMAGE_DRAW_TYPE,
  /**
   * @const STYLE_ID_ICON
   * 图标的名称。
   */
  STYLE_ID_ICON,
  /**
   * @const STYLE_ID_FG_IMAGE
   * 图片的名称。
   */
  STYLE_ID_FG_IMAGE,
  /**
   * @const STYLE_ID_FG_IMAGE_DRAW_TYPE
   * 图片的显示方式。
   */
  STYLE_ID_FG_IMAGE_DRAW_TYPE,
  /**
   * @const STYLE_ID_MARGIN
   * 边距。
   */
  STYLE_ID_MARGIN,
  /**
   * @const STYLE_ID_ICON_AT
   * 图标的位置。
   */
  STYLE_ID_ICON_AT
} style_id_t;

#define THEME_MAGIC 0xFAFBFCFD
#define TK_DEFAULT_STYLE "default"

END_C_DECLS

#endif /*TK_THEME_H*/
