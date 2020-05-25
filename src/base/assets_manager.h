/**
 * File:   assets_manager.h
 * Author: AWTK Develop Team
 * Brief:  asset manager
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
 * 2018-03-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ASSETS_MANAGER_H
#define TK_ASSETS_MANAGER_H

#include "tkc/darray.h"
#include "tkc/asset_info.h"
#include "base/types_def.h"
#include "base/asset_loader.h"

BEGIN_C_DECLS

typedef ret_t (*assets_manager_build_asset_dir_t)(void* ctx, char* path, uint32_t size,
                                                  const char* theme, const char* ratio,
                                                  const char* subpath);

typedef asset_info_t* (*assets_manager_load_asset_t)(assets_manager_t* am, asset_type_t type,
                                                     const char* name);

/**
 * @class assets_manager_t
 * @annotation ["scriptable"]
 * 资源管理器。
 * 这里的资源管理器并非Windows下的文件浏览器，而是负责对各种资源，比如字体、主题、图片、界面数据、字符串和其它数据的进行集中管理的组件。引入资源管理器的目的有以下几个：
 *
 * * 让上层不需要了解存储的方式。
 * 在没有文件系统时或者内存紧缺时，把资源转成常量数组直接编译到代码中。在有文件系统而且内存充足时，资源放在文件系统中。在有网络时，资源也可以存放在服务器上(暂未实现)。资源管理器为上层提供统一的接口，让上层而不用关心底层的存储方式。
 *
 * * 让上层不需要了解资源的具体格式。
 * 比如一个名为earth的图片，没有文件系统或内存紧缺，图片直接用位图数据格式存在ROM中，而有文件系统时，则用PNG格式存放在文件系统中。资源管理器让上层不需要关心图片的格式，访问时指定图片的名称即可(不用指定扩展名)。
 *
 * * 让上层不需要了解屏幕的密度。
 * 不同的屏幕密度下需要加载不同的图片，比如MacPro的Retina屏就需要用双倍解析度的图片，否则就出现界面模糊。AWTK以后会支持PC软件和手机软件的开发，所以资源管理器需要为此提供支持，让上层不需关心屏幕的密度。
 *
 * * 对资源进行内存缓存。
  * 不同类型的资源使用方式是不一样的，比如字体和主题加载之后会一直使用，UI文件在生成界面之后就暂时不需要了，PNG文件解码之后就只需要保留解码的位图数据即可。资源管理器配合图片管理器等其它组件实现资源的自动缓存。
 *
 *当从文件系统加载资源时，目录结构要求如下：
 *
 * ```
 * assets/{theme}/raw/
 *  fonts   字体
 *  images  图片
 *    x1   普通密度屏幕的图片。
 *    x2   2倍密度屏幕的图片。
 *    x3   3倍密度屏幕的图片。
 *    xx   密度无关的图片。
 *  strings 需要翻译的字符串。
 *  styles  主题数据。
 *  ui      UI描述数据。
 * ```
 *
 */
struct _assets_manager_t {
  darray_t assets;

  /*private*/
  char* theme;
  char* res_root;
  locale_info_t* locale_info;
  system_info_t* system_info;

  void* custom_load_asset_ctx;
  assets_manager_load_asset_t custom_load_asset;

  void* custom_build_asset_dir_ctx;
  assets_manager_build_asset_dir_t custom_build_asset_dir;

  asset_loader_t* loader;
};

/**
 * @method assets_manager
 * 获取缺省资源管理器。
 * @alias assets_manager_instance
 * @annotation ["constructor", "scriptable"]
 *
 * @return {assets_manager_t*} 返回asset manager对象。
 */
assets_manager_t* assets_manager(void);

/**
 * @method assets_manager_set
 * 设置缺省资源管理器。
 * @param {assets_manager_t*} am asset manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_set(assets_manager_t* am);

/**
 * @method assets_manager_create
 * 创建资源管理器。
 * @annotation ["constructor"]
 * @param {uint32_t} init_nr 预先分配资源的个数。
 *
 * @return {assets_manager_t*} 返回asset manager对象。
 */
assets_manager_t* assets_manager_create(uint32_t init_nr);

/**
 * @method assets_manager_init
 * 初始化资源管理器。
 * @annotation ["constructor"]
 * @param {assets_manager_t*} am asset manager对象。
 * @param {uint32_t} init_nr 预先分配资源的个数。
 *
 * @return {assets_manager_t*} 返回asset manager对象。
 */
assets_manager_t* assets_manager_init(assets_manager_t* am, uint32_t init_nr);

/**
 * @method assets_manager_set_res_root
 * 设置资源所在的目录(其下目录结构请参考demos)。
 * @param {assets_manager_t*} am asset manager对象。
 * @param {const char*} res_root 资源所在的目录。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_set_res_root(assets_manager_t* am, const char* res_root);

/**
 * @method assets_manager_set_theme
 * 设置当前的主题。
 * @annotation ["scriptable"]
 * @param {assets_manager_t*} am asset manager对象。
 * @param {const char*} theme 主题名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_set_theme(assets_manager_t* am, const char* theme);

/**
 * @method assets_manager_set_system_info
 * 设置system_info对象。
 * @param {assets_manager_t*} am asset manager对象。
 * @param {system_info_t*} system_info system_info对象。。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_set_system_info(assets_manager_t* am, system_info_t* system_info);

/**
 * @method assets_manager_set_locale_info
 * 设置locale_info对象。
 * @param {assets_manager_t*} am asset manager对象。
 * @param {locale_info_t*} locale_info locale_info对象。。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_set_locale_info(assets_manager_t* am, locale_info_t* locale_info);

/**
 * @method assets_manager_add
 * 向资源管理器中增加一个资源。
 * @param {assets_manager_t*} am asset manager对象。
 * @param {asset_info_t} info 待增加的资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_add(assets_manager_t* am, const void* info);

/**
 * @method assets_manager_add_data
 * 向资源管理器中增加一个资源data。
 * @param {assets_manager_t*} am asset manager对象。
 * @param {const char*} name 待增加的资源的名字。
 * @param {uint16_t} type 待增加的资源的主类型枚举。
 * @param {uint16_t} subtype 待增加的资源的子类型枚举。
 * @param {uint8_t*} buff 待增加的资源的data数据。
 * @param {uint32_t} size 待增加的资源的data数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_add_data(assets_manager_t* am, const char* name, uint16_t type,
                              uint16_t subtype, uint8_t* buff, uint32_t size);

/**
 * @method assets_manager_ref
 * 在资源管理器的缓存中查找指定的资源并引用它，如果缓存中不存在，尝试加载该资源。
 * @annotation ["scriptable"]
 * @param {assets_manager_t*} am asset manager对象。
 * @param {asset_type_t} type 资源的类型。
 * @param {char*} name 资源的名称。
 *
 * @return {asset_info_t*} 返回资源。
 */
const asset_info_t* assets_manager_ref(assets_manager_t* am, asset_type_t type, const char* name);

/**
 * @method assets_manager_unref
 * 释放指定的资源。
 * @annotation ["scriptable"]
 * @param {assets_manager_t*} am asset manager对象。
 * @param {asset_info_t*} info 资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_unref(assets_manager_t* am, const asset_info_t* info);

/**
 * @method assets_manager_find_in_cache
 * 在资源管理器的缓存中查找指定的资源(不引用)。
 * @param {assets_manager_t*} am asset manager对象。
 * @param {asset_type_t} type 资源的类型。
 * @param {char*} name 资源的名称。
 *
 * @return {asset_info_t*} 返回资源。
 */
const asset_info_t* assets_manager_find_in_cache(assets_manager_t* am, asset_type_t type,
                                                 const char* name);
/**
 * @method assets_manager_load
 * 从文件系统中加载指定的资源，并缓存到内存中。在定义了宏WITH\_FS\_RES时才生效。
 * @param {assets_manager_t*} am asset manager对象。
 * @param {asset_type_t} type 资源的类型。
 * @param {char*} name 资源的名称。
 *
 * @return {asset_info_t*} 返回资源。
 */
asset_info_t* assets_manager_load(assets_manager_t* am, asset_type_t type, const char* name);

/**
 * @method assets_manager_preload
 * 从文件系统中加载指定的资源，并缓存到内存中。在定义了宏WITH\_FS\_RES时才生效。
 * @param {assets_manager_t*} am asset manager对象。
 * @param {asset_type_t} type 资源的类型。
 * @param {char*} name 资源的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_preload(assets_manager_t* am, asset_type_t type, const char* name);

/**
 * @method assets_manager_set_loader
 * 设置loader。
 * @param {assets_manager_t*} am asset manager对象。
 * @param {asset_loader_t*} loader 加载器(由assets manager销毁)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_set_loader(assets_manager_t* am, asset_loader_t* loader);

/**
 * @method assets_manager_set_custom_build_asset_dir
 * 设置一个函数，该函数用于生成资源路径。
 *
 * > 有时我们需要优先加载用户自定义的资源，加载失败才加载系统缺省的，可用设置一个函数去实现这类功能。
 *
 * @param {assets_manager_t*} am asset manager对象。
 * @param {assets_manager_build_asset_dir_t} custom_build_asset_dir 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_set_custom_build_asset_dir(
    assets_manager_t* am, assets_manager_build_asset_dir_t custom_build_asset_dir, void* ctx);

/**
 * @method assets_manager_set_custom_load_asset
 * 设置一个函数，该函数用于实现自定义加载资源。
 *
 * > 如果不支持文件系统，开发者可以设置一个加载资源的回调函数，从flash或其它地方读取资源。
 *
 * @param {assets_manager_t*} am asset manager对象。
 * @param {assets_manager_load_asset_t} custom_load_asset 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_set_custom_load_asset(assets_manager_t* am,
                                           assets_manager_load_asset_t custom_load_asset,
                                           void* ctx);

/**
 * @method assets_manager_clear_cache
 * 清除指定类型的缓存。
 * @param {assets_manager_t*} am asset manager对象。
 * @param {asset_type_t} type 资源的类型。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_clear_cache(assets_manager_t* am, asset_type_t type);

/**
 * @method assets_manager_clear_all
 * 清除全部缓存的资源。
 * @param {assets_manager_t*} am asset manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_clear_all(assets_manager_t* am);

/**
 * @method assets_manager_deinit
 * @param {assets_manager_t*} am asset manager对象。
 * 释放全部资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_deinit(assets_manager_t* am);

/**
 * @method assets_manager_destroy
 * @param {assets_manager_t*} am asset manager对象。
 * 释放全部资源并销毁asset manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_destroy(assets_manager_t* am);

/**
 * @method assets_manager_load_file
 * @param {assets_manager_t*} am asset manager对象。
 * @param {asset_type_t} type 资源类型。
 * @param {const char*} path 目录。
 * 获取path里的资源。
 *
 * @return {asset_info_t*} 返回asset_info_t。
 */
asset_info_t* assets_manager_load_file(assets_manager_t* am, asset_type_t type, const char* path);

END_C_DECLS

#endif /*TK_ASSETS_MANAGER_H*/
