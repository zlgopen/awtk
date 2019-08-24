/**
 * File:   asset_info.h
 * Author: AWTK Develop Team
 * Brief:  asset info
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
 * 2018-03-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ASSET_INFO_H
#define TK_ASSET_INFO_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @enum asset_type_t
 * @prefix ASSET_TYPE_
 * @annotation ["scriptable"]
 * 资源类型常量定义。
 */
typedef enum _asset_type_t {
  /**
   * @const ASSET_TYPE_NONE
   * 无效资源。
   */
  ASSET_TYPE_NONE,

  /**
   * @const ASSET_TYPE_FONT
   * 字体资源。
   */
  ASSET_TYPE_FONT,

  /**
   * @const ASSET_TYPE_IMAGE
   * 图片资源。
   */
  ASSET_TYPE_IMAGE,

  /**
   * @const ASSET_TYPE_STYLE
   * 主题资源。
   */
  ASSET_TYPE_STYLE,

  /**
   * @const ASSET_TYPE_UI
   * UI数据资源。
   */
  ASSET_TYPE_UI,

  /**
   * @const ASSET_TYPE_XML
   * XML数据资源。
   */
  ASSET_TYPE_XML,

  /**
   * @const ASSET_TYPE_STRINGS
   * 字符串数据资源。
   */
  ASSET_TYPE_STRINGS,

  /**
   * @const ASSET_TYPE_SCRIPT
   * JS等脚本资源。
   */
  ASSET_TYPE_SCRIPT,

  /**
   * @const ASSET_TYPE_DATA
   * 其它数据资源。
   */
  ASSET_TYPE_DATA
} asset_type_t;

/**
 * @enum asset_font_type_t
 * 字体资源类型定义。
 */
typedef enum _asset_font_type_t {
  ASSET_TYPE_FONT_NONE,
  ASSET_TYPE_FONT_TTF,
  ASSET_TYPE_FONT_BMP
} asset_font_type_t;

/**
 * @enum asset_data_type_t
 * 字体资源类型定义。
 */
typedef enum _asset_data_type_t {
  ASSET_TYPE_DATA_NONE,
  ASSET_TYPE_DATA_TEXT,
  ASSET_TYPE_DATA_BIN,
  ASSET_TYPE_DATA_JSON,
  ASSET_TYPE_DATA_DAT
} asset_data_type_t;

/**
 * @enum asset_script_type_t
 * 脚本资源类型定义。
 */
typedef enum _asset_script_type_t {
  ASSET_TYPE_SCRIPT_NONE,
  ASSET_TYPE_SCRIPT_JS,
  ASSET_TYPE_SCRIPT_LUA,
  ASSET_TYPE_SCRIPT_PYTHON
} asset_script_type_t;

/**
 * @enum asset_image_type_t
 * 图片资源类型定义。
 */
typedef enum _asset_image_type_t {
  ASSET_TYPE_IMAGE_NONE,
  ASSET_TYPE_IMAGE_RAW,
  ASSET_TYPE_IMAGE_BMP,
  ASSET_TYPE_IMAGE_PNG,
  ASSET_TYPE_IMAGE_JPG,
  ASSET_TYPE_IMAGE_BSVG,
  ASSET_TYPE_IMAGE_GIF,
  ASSET_TYPE_IMAGE_WEBP,
  ASSET_TYPE_IMAGE_LZ4,
  ASSET_TYPE_IMAGE_OTHER,
} asset_image_type_t;

/**
 * @enum asset_ui_type_t
 * UI资源类型定义。
 */
typedef enum _asset_ui_type_t {
  ASSET_TYPE_UI_NONE,
  ASSET_TYPE_UI_BIN,
  ASSET_TYPE_UI_XML
} asset_ui_type_t;

/**
 * @class preload_res_t
 * 预加载资源的描述信息。
 */
typedef struct _preload_res_t {
  asset_type_t type;
  const char* name;
} preload_res_t;

/**
 * @class asset_info_t
 * @annotation ["constructor", "scriptable"]
 * 单个资源的描述信息。
 */
typedef struct _asset_info_t {
  /**
   * @property {uint16_t} type
   * @annotation ["readable","scriptable"]
   * 类型。
   */
  uint16_t type;
  /**
   * @property {uint8_t} subtype
   * @annotation ["readable", "scriptable"]
   * 子类型。
   */
  uint8_t subtype;
  /**
   * @property {uint8_t} is_in_rom
   * @annotation ["readable", "scriptable"]
   * 资源是否在ROM中。
   */
  uint8_t is_in_rom;
  /**
   * @property {uint32_t} size
   * @annotation ["readable","scriptable"]
   * 大小。
   */
  uint32_t size;
  /**
   * @property {uint32_t} refcount
   * @annotation ["readable","scriptable"]
   * 引用计数。
   * is\_in\_rom == FALSE时才有效。
   */
  uint32_t refcount;
  /**
   * @property {char*} name
   * @annotation ["readable","scriptable"]
   * 名称。
   */
  char name[TK_NAME_LEN + 1];
  uint8_t data[4];
} asset_info_t;

END_C_DECLS

#endif /*TK_ASSET_INFO_H*/
