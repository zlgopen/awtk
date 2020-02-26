/**
 * File:   asset_loader_zip.h
 * Author: AWTK Develop Team
 * Brief:  asset_loader_zip
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

#ifndef TK_ASSET_LOADER_ZIP_H
#define TK_ASSET_LOADER_ZIP_H

#include "tkc/data_reader.h"
#include "base/asset_loader.h"

BEGIN_C_DECLS

/**
 * @method asset_loader_zip_create
 * 创建zip资源加载器。
 * @param {const char*} zipfile zip文件名。
 *
 * @return {asset_loader_t*} 返回loader对象。
 */
asset_loader_t* asset_loader_zip_create(const char* zipfile);

/**
 * @method asset_loader_zip_create_with_reader
 * 创建zip资源加载器。
 * @param {data_reader_t*} reader reader对象。
 * @param {bool_t} own_the_reader 如果希望销毁loader时销毁reader，请设置为TRUE。
 *
 * @return {asset_loader_t*} 返回loader对象。
 */
asset_loader_t* asset_loader_zip_create_with_reader(data_reader_t* reader, bool_t own_the_reader);

END_C_DECLS

#endif /*TK_ASSET_LOADER_ZIP_H*/
