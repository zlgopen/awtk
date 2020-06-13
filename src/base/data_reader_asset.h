/**
 * File:   data_reader_asset.h
 * Author: AWTK Develop Team
 * Brief:  data_reader asset
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License asset for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-06-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DATA_READER_ASSET_H
#define TK_DATA_READER_ASSET_H

#include "tkc/data_reader.h"

BEGIN_C_DECLS

/**
 * @method data_reader_asset_create
 * 创建基于asset的data reader。
 * @param {const char*} assetname 资源的名称。
 *
 * @return {data_reader_t*} 返回data reader对象。
 */
data_reader_t* data_reader_asset_create(const char* assetname);

#define DATA_READER_ASSET(reader) ((data_reader_t*)(reader))

END_C_DECLS

#endif /*TK_DATA_READER_ASSET_H*/
