/**
 * File:   resource_manager.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  resource manager
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
 * 2018-03-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_RESOURCE_MANAGER_H
#define LFTK_RESOURCE_MANAGER_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @enum resource_type_t
 * 资源类型常量定义。
 */
typedef enum _resource_type_t {
  RESOURCE_TYPE_NONE,
  RESOURCE_TYPE_FONT,
  RESOURCE_TYPE_IMAGE,
  RESOURCE_TYPE_THEME,
  RESOURCE_TYPE_UI,
  RESOURCE_TYPE_XML,
  RESOURCE_TYPE_DATA
}resource_type_t;

/**
 * @enum resource_font_type_t
 * 字体资源类型定义。
 */
typedef enum _resource_font_type_t {
  RESOURCE_TYPE_FONT_NONE,
  RESOURCE_TYPE_FONT_TTF,
  RESOURCE_TYPE_FONT_BMP
} resource_font_type_t;

/**
 * @enum resource_image_type_t
 * 图片资源类型定义。
 */
typedef enum _resource_image_type_t {
  RESOURCE_TYPE_IMAGE_NONE,
  RESOURCE_TYPE_IMAGE_RAW,
  RESOURCE_TYPE_IMAGE_BMP,
  RESOURCE_TYPE_IMAGE_PNG,
  RESOURCE_TYPE_IMAGE_JPG,
  RESOURCE_TYPE_IMAGE_GIF
} resource_image_type_t;

/**
 * @enum resource_ui_type_t
 * UI资源类型定义。
 */
typedef enum _resource_ui_type_t {
  RESOURCE_TYPE_UI_NONE,
  RESOURCE_TYPE_UI_BIN,
  RESOURCE_TYPE_UI_XML
}resource_ui_type_t;

/**
 * @class resource_info_t
 * 单个资源的描述信息。
 */
typedef struct _resource_info_t {
  uint16_t type;
  uint16_t subtype;
  uint32_t size;
  char name[NAME_LEN+1];
  uint8_t data[4];
} resource_info_t;

/**
 * @class resource_manager_t
 * @fake
 * 资源管理器。
 */

/**
 * @method resource_manager_init
 * 初始化资源管理器。
 * @static
 * @param {uint32_t} init_res_nr 预先分配资源的个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t resource_manager_init(uint32_t init_res_nr);

/**
 * @method resource_manager_add
 * 向资源管理器中增加一个资源。
 * @static
 * @param {resource_info_t} info 待增加的资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t resource_manager_add(const void* info);

/**
 * @method resource_manager_ref
 * 在资源管理器中查找指定的资源并引用它。
 * @static
 * @param {resource_type_t} type 资源的类型。
 * @param {char*} name 资源的名称。
 *
 * @return {ret_t} 返回资源。
 */
const resource_info_t* resource_manager_ref(resource_type_t type, const char* name);

/**
 * @method resource_manager_unref
 * 释放指定的资源。
 * @static
 * @param {resource_info_t} info 资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t resource_manager_unref(const resource_info_t* info);

/**
 * @method resource_manager_deinit
 * 释放全部资源。
 * @static
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t resource_manager_deinit(void);

const resource_info_t** resource_manager_get_all(uint32_t* size);

END_C_DECLS

#endif/*LFTK_RESOURCE_MANAGER_H*/

