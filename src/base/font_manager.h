/**
 * File:   font_manager.h
 * Author: AWTK Develop Team
 * Brief:  font manager
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/darray.h"
#include "base/types_def.h"
#include "base/font_loader.h"
#include "base/assets_manager.h"

BEGIN_C_DECLS

typedef font_t* (*font_manager_get_font_t)(void* ctx, const char* name, font_size_t size);

/**
 * @class font_manager_t
 * @parent emitter_t
 * @annotation ["scriptable"]
 * 字体管理器，负责字体的加载和缓存管理。
 * (如果使用nanovg，字体由nanovg内部管理)
 */
struct _font_manager_t {
  emitter_t emitter;
  darray_t fonts;

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

  /*private*/
  char* name;
  int32_t refcount;
  font_manager_get_font_t fallback_get_font;
  void* fallback_get_font_ctx;
};

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
 *
 * 之所以需要设置资源管理器对象，而不是使用缺省的资源管理器对象，是因为在designer中有两个字体管理器：
 *
 * * 一个用于designer本身加载字体。
 *
 * * 一个用于被设计的窗口加载字体。
 *
 *这两个字体管理器需要从不同的路径加载资源。
 * @param {font_manager_t*} fm 字体管理器对象。
 * @param {assets_manager_t*} assets_manager 资源管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_manager_set_assets_manager(font_manager_t* fm, assets_manager_t* assets_manager);

/**
 * @method font_manager_add_font
 * 向缓存中加入字体。
 * @param {font_manager_t*} fm 字体管理器对象。
 * @param {font_t*} font 字体。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_manager_add_font(font_manager_t* fm, font_t* font);

/**
 * @method font_manager_get_font
 * 从缓存中查找字体，如果没找到，再加载字体，并缓存。
 * @param {font_manager_t*} fm 字体管理器对象。
 * @param {const char*} name 字体名，为NULL时使用缺省字体。
 * @param {font_size_t} size 字体的大小。
 *
 * @return {font_t*} 返回字体对象。
 */
font_t* font_manager_get_font(font_manager_t* fm, const char* name, font_size_t size);

/**
 * @method font_manager_set_fallback_get_font
 * 设置一个函数，该函数在找不到字体时加载后补字体。
 *
 * @param {font_manager_t*} fm 字体管理器对象。
 * @param {font_manager_get_font_t} fallback_get_font 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_manager_set_fallback_get_font(font_manager_t* fm,
                                         font_manager_get_font_t fallback_get_font, void* ctx);

/**
 * @method font_manager_unload_font
 * 卸载指定的字体。
 * @annotation ["scriptable"]
 * @param {font_manager_t*} fm 字体管理器对象。
 * @param {const char*} name 字体名，为NULL时使用缺省字体。
 * @param {font_size_t} size 字体的大小(矢量字体指定为0即可)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_manager_unload_font(font_manager_t* fm, const char* name, font_size_t size);

/**
 * @method font_manager_shrink_cache
 * 清除最久没有被使用的缓冲字模。
 * @annotation ["scriptable"]
 * @param {font_manager_t*} fm 字体管理器对象。
 * @param {uint32_t} cache_size 每种字体保留缓存字模的个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_manager_shrink_cache(font_manager_t* fm, uint32_t cache_size);

/**
 * @method font_manager_unload_all
 * 卸载全部字体。
 * @annotation ["scriptable"]
 * @param {font_manager_t*} fm 字体管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_manager_unload_all(font_manager_t* fm);

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

/*public for test*/
/**
 * @method font_manager_lookup
 * 查找字体。    
 * @param {font_manager_t*} fm 字体管理器对象。
 * @param {const char*} name 字体名称。
 * @param {font_size_t} size 字体大小。
 *
 * @return {font_t*} 返回字体对象。
 */
font_t* font_manager_lookup(font_manager_t* fm, const char* name, font_size_t size);

/**
 * @class font_managers_t
 * @annotation ["fake"]
 * 在某些情况下，需要多个资源管理器。比如在手表系统里，每个应用或表盘，可能放在独立的资源包中，
 * 此时优先加载应用自己的资源，如果没有就加载系统的资源。
 */

/**
 * @method font_managers_ref
 * 获取指定小应用程序(applet)的字体管理器。
 * @annotation ["constructor"]
 * @param {const char*} name 小应用程序(applet)的名称。
 *
 * @return {font_manager_t*} 返回asset manager对象。
 */
font_manager_t* font_managers_ref(const char* name);

/**
 * @method font_managers_unload_all
 * 卸载全部字体。
 * @annotation ["scriptable"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_managers_unload_all(void);

/**
 * @method font_managers_unref
 * 释放指定小应用程序(applet)的字体管理器。
 * @annotation ["deconstructor"]
 * @param {font_manager_t*} imm 字体管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_managers_unref(font_manager_t* imm);

/*public for test*/
/**
 * @method font_manager_fallback_get_font_default;
 */
font_t* font_manager_fallback_get_font_default(void* ctx, const char* name,
                                                      font_size_t size);
END_C_DECLS

#endif /*TK_FONT_MANAGER_H*/
