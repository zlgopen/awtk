/**
 * File:   resource_manager.h
 * Author: AWTK Develop Team
 * Brief:  resource manager
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
 * 2018-03-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_RESOURCE_MANAGER_H
#define TK_RESOURCE_MANAGER_H

#include "base/array.h"

BEGIN_C_DECLS

/**
 * @enum resource_type_t
 * @prefix RESOURCE_TYPE_
 * @annotation ["scriptable"]
 * 资源类型常量定义。
 */
typedef enum _resource_type_t {
  /**
   * @const RESOURCE_TYPE_NONE
   * 无效资源。
   */
  RESOURCE_TYPE_NONE,

  /**
   * @const RESOURCE_TYPE_FONT
   * 字体资源。
   */
  RESOURCE_TYPE_FONT,

  /**
   * @const RESOURCE_TYPE_IMAGE
   * 图片资源。
   */
  RESOURCE_TYPE_IMAGE,

  /**
   * @const RESOURCE_TYPE_THEME
   * 主题资源。
   */
  RESOURCE_TYPE_THEME,

  /**
   * @const RESOURCE_TYPE_UI
   * UI数据资源。
   */
  RESOURCE_TYPE_UI,

  /**
   * @const RESOURCE_TYPE_XML
   * XML数据资源。
   */
  RESOURCE_TYPE_XML,

  /**
   * @const RESOURCE_TYPE_STRINGS
   * 字符串数据资源。
   */
  RESOURCE_TYPE_STRINGS,

  /**
   * @const RESOURCE_TYPE_DATA
   * 其它数据资源。
   */
  RESOURCE_TYPE_DATA
} resource_type_t;

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
} resource_ui_type_t;

/**
 * @class preload_res_t
 * 预加载资源的描述信息。
 */
typedef struct _preload_res_t {
  resource_type_t type;
  const char* name;
} preload_res_t;

/**
 * @class resource_info_t
 * @annotation ["constructor", "scriptable"]
 * 单个资源的描述信息。
 */
typedef struct _resource_info_t {
  /**
   * @property {uint16_t} type
   * @annotation ["readable","scriptable"]
   * 类型。
   */
  uint16_t type;
  uint8_t subtype;
  uint8_t is_in_rom;
  /**
   * @property {uint32_t} size
   * @annotation ["readable","scriptable"]
   * 大小。
   */
  uint32_t size;
  uint32_t refcount; /*is_in_rom == FALSE,才有效*/
  /**
   * @property {char*} name
   * @annotation ["readable","scriptable"]
   * 名称。
   */
  char name[NAME_LEN + 1];
  uint8_t data[4];
} resource_info_t;

/**
 * @class resource_manager_t
 * @annotation ["scriptable"]
 * 资源管理器。
 */
typedef struct _resource_manager_t {
  array_t resources;
} resource_manager_t;

/**
 * @method resource_manager
 * 获取缺省资源管理器。
 * @annotation ["constructor", "scriptable"]
 *
 * @return {resource_manager_t*} 返回resource manager对象。
 */
resource_manager_t* resource_manager(void);

/**
 * @method resource_manager_set
 * 设置缺省资源管理器。
 * @param {resource_manager_t*} rm resource manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t resource_manager_set(resource_manager_t* rm);

/**
 * @method resource_manager_create
 * 创建资源管理器。
 * @annotation ["constructor"]
 * @param {uint32_t} init_res_nr 预先分配资源的个数。
 *
 * @return {resource_manager_t*} 返回resource manager对象。
 */
resource_manager_t* resource_manager_create(uint32_t init_res_nr);

/**
 * @method resource_manager_init
 * 初始化资源管理器。
 * @annotation ["constructor"]
 * @param {resource_manager_t*} rm resource manager对象。
 * @param {uint32_t} init_res_nr 预先分配资源的个数。
 *
 * @return {resource_manager_t*} 返回resource manager对象。
 */
resource_manager_t* resource_manager_init(resource_manager_t* rm, uint32_t init_res_nr);

/**
 * @method resource_manager_add
 * 向资源管理器中增加一个资源。
 * @param {resource_manager_t*} rm resource manager对象。
 * @param {resource_info_t} info 待增加的资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t resource_manager_add(resource_manager_t* rm, const void* info);

/**
 * @method resource_manager_ref
 * 在资源管理器的缓存中查找指定的资源并引用它，如果缓存中不存在，尝试加载该资源。
 * @annotation ["scriptable"]
 * @param {resource_manager_t*} rm resource manager对象。
 * @param {resource_type_t} type 资源的类型。
 * @param {char*} name 资源的名称。
 *
 * @return {resource_info_t*} 返回资源。
 */
const resource_info_t* resource_manager_ref(resource_manager_t* rm, resource_type_t type,
                                            const char* name);

/**
 * @method resource_manager_unref
 * 释放指定的资源。
 * @annotation ["scriptable"]
 * @param {resource_manager_t*} rm resource manager对象。
 * @param {resource_info_t*} info 资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t resource_manager_unref(resource_manager_t* rm, const resource_info_t* info);

/**
 * @method resource_manager_find_in_cache
 * 在资源管理器的缓存中查找指定的资源(不引用)。
 * @param {resource_manager_t*} rm resource manager对象。
 * @param {resource_type_t} type 资源的类型。
 * @param {char*} name 资源的名称。
 *
 * @return {resource_info_t*} 返回资源。
 */
const resource_info_t* resource_manager_find_in_cache(resource_manager_t* rm, resource_type_t type,
                                                      const char* name);
/**
 * @method resource_manager_load
 * 从文件系统中加载指定的资源，并缓存到内存中。在定义了宏WITH_FS_RES时才生效。
 * @param {resource_manager_t*} rm resource manager对象。
 * @param {resource_type_t} type 资源的类型。
 * @param {char*} name 资源的名称。
 *
 * @return {resource_info_t*} 返回资源。
 */
resource_info_t* resource_manager_load(resource_manager_t* rm, resource_type_t type,
                                       const char* name);

/**
 * @method resource_manager_clear_cache
 * 清除指定类型的缓存。
 * @param {resource_manager_t*} rm resource manager对象。
 * @param {resource_type_t} type 资源的类型。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t resource_manager_clear_cache(resource_manager_t* rm, resource_type_t type);

/**
 * @method resource_manager_deinit
 * @param {resource_manager_t*} rm resource manager对象。
 * 释放全部资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t resource_manager_deinit(resource_manager_t* rm);

/**
 * @method resource_manager_destroy
 * @param {resource_manager_t*} rm resource manager对象。
 * 释放全部资源并销毁resource manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t resource_manager_destroy(resource_manager_t* rm);

END_C_DECLS

#endif /*TK_RESOURCE_MANAGER_H*/
