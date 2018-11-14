/**
 * File:   font_manager.h
 * Author: AWTK Develop Team
 * Brief:  font manager
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FONT_MANAGER_H
#define TK_FONT_MANAGER_H

#include "base/array.h"
#include "base/font_loader.h"
#include "base/assets_manager.h"

BEGIN_C_DECLS

/**
 * @class font_manager_t
 * 字体管理器。
 * (如果使用nanovg，字体由nanovg内部管理)
 */
typedef struct _font_manager_t {
  array_t fonts;

  /**
   * @property {font_loader_t*} loader
   * @annotation ["private"]
   * 字体加载器。
   */
  font_loader_t* loader;

  /**
   * @property {assets_manager_t*} assets_manager
   * @annotation ["private"]
   * 资源管理器。
   */
  assets_manager_t* assets_manager;
} font_manager_t;

/**
 * @method font_manager
 * 获取缺省的字体管理器。
 * @annotation ["constructor"]
 * @return {font_manager_t*} 返回字体管理器对象。
 */
font_manager_t* font_manager(void);

/**
 * @method font_manager_set
 * 设置缺省的字体管理器。
 * @param {font_manager_t*} fm 字体管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_manager_set(font_manager_t* fm);

/**
 * @method font_manager_create
 * 创建字体管理器。
 * @annotation ["constructor"]
 * @param {font_loader_t*} loader 字体加载器。
 *
 * @return {font_manager_t*} 返回字体管理器对象。
 */
font_manager_t* font_manager_create(font_loader_t* loader);

/**
 * @method font_manager_init
 * 初始化字体管理器。
 * @annotation ["constructor"]
 * @param {font_manager_t*} fm 字体管理器对象。
 * @param {font_loader_t*} loader 字体加载器。
 *
 * @return {font_manager_t*} 返回字体管理器对象。
 */
font_manager_t* font_manager_init(font_manager_t* fm, font_loader_t* loader);

/**
 * @method font_manager_set_assets_manager
 * 设置资源管理器对象
 * @param {font_manager_t*} imm 图片管理器对象。
 * @param {assets_manager_t*} assets_manager 资源管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_manager_set_assets_manager(font_manager_t* imm, assets_manager_t* assets_manager);

/**
 * @method font_manager_add
 * 向缓存中加入字体。
 * @param {font_manager_t*} fm 字体管理器对象。
 * @param {char*} name 字体名。
 * @param {font_t*} font 字体。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_manager_add(font_manager_t* fm, font_t* font);

/**
 * @method font_manager_find
 * 从缓存中查找字体。
 * @param {font_manager_t*} fm 字体管理器对象。
 * @param {char*} name 字体名，为NULL时使用TK_DEFAULT_FONT。
 * @param {uint16_t} size 字体的大小。
 *
 * @return {font_t*} 返回字体对象。
 */
font_t* font_manager_find(font_manager_t* fm, const char* name, uint16_t size);

/**
 * @method font_manager_deinit
 * 析构字体管理器。
 * @param {font_manager_t*} fm 字体管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_manager_deinit(font_manager_t* fm);

/**
 * @method font_manager_destroy
 * 析构并释放字体管理器。
 * @param {font_manager_t*} fm 字体管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_manager_destroy(font_manager_t* fm);

END_C_DECLS

#endif /*TK_FONT_MANAGER_H*/
