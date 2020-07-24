/**
 * File:   asset_loader.h
 * Author: AWTK Develop Team
 * Brief:  asset_loader
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-02-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ASSET_LOADER_H
#define TK_ASSET_LOADER_H

#include "tkc/asset_info.h"

BEGIN_C_DECLS

struct _asset_loader_t;
typedef struct _asset_loader_t asset_loader_t;

typedef asset_info_t* (*asset_loader_load_t)(asset_loader_t* loader, uint16_t type,
                                             uint16_t subtype, const char* path, const char* name);
typedef bool_t (*asset_loader_exist_t)(asset_loader_t* loader, const char* path);
typedef ret_t (*asset_loader_destroy_t)(asset_loader_t* loader);

typedef struct _asset_loader_vtable_t {
  asset_loader_load_t load;
  asset_loader_exist_t exist;
  asset_loader_destroy_t destroy;
} asset_loader_vtable_t;

/**
 * @class asset_loader_t
 * 资源加载器接口。
 */
struct _asset_loader_t {
  const asset_loader_vtable_t* vt;
};

/**
 * @method asset_loader_create
 * 创建缺省的资源加载器。
 *
 * @return {asset_loader_t*} 返回loader对象。
 */
asset_loader_t* asset_loader_create(void);

/**
 * @method asset_loader_load
 * 加载指定的资源。
 * @param {asset_loader_t*} loader loader对象。
 * @param {uint16_t} type 资源类型。
 * @param {uint16_t} subtype 资源子类型。
 * @param {const char*} path 路径。
 * @param {const char*} name 名称。
 *
 * @return {asset_info_t*} 返回资源对象(由调用者销毁)。
 */
asset_info_t* asset_loader_load(asset_loader_t* loader, uint16_t type, uint16_t subtype,
                                const char* path, const char* name);

/**
 * @method asset_loader_exist
 * 判断指定资源是否存在。
 * 。
 * @param {asset_loader_t*} loader loader对象。
 * @param {const char*} path 资源是路径。
 *
 * @return {bool_t} 返回TRUE表示存在，否则不存在。
 */
bool_t asset_loader_exist(asset_loader_t* loader, const char* path);

/**
 * @method asset_loader_destroy
 * 销毁loader对象。
 * @param {asset_loader_t*} loader loader对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t asset_loader_destroy(asset_loader_t* loader);

#define ASSET_LOADER(loader) ((asset_loader_t*)(loader))

END_C_DECLS

#endif /*TK_ASSET_LOADER_H*/
