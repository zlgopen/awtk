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

END_C_DECLS

#endif /*TK_ASSET_LOADER_ZIP_H*/
