/**
 * File:   data_reader_asset.h
 * Author: AWTK Develop Team
 * Brief:  data_reader asset
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/asset_info.h"
#include "tkc/data_reader.h"

BEGIN_C_DECLS
/**
 * @class data_reader_asset_t
 * @parent data_reader_t
 * @annotation ["fake"]
 * 基于资源实现的 data_reader。通过 data_reader_factory 创建 reader 时，URL的格式如下(请用函数data_reader_asset_build_url生成)：
 * 
 * ```
 * asset://type/name
 * ```
 */

/**
 * @method data_reader_asset_create
 * 创建基于asset的data reader。
 * 
 * > 不要直接调用，而是注册到 data\_reader\_factory后，通过data\_reader\_factory调用。
 * @annotation ["constructor"]
 * @param {const char*} assetname 资源的名称。
 *
 * @return {data_reader_t*} 返回data reader对象。
 */
data_reader_t* data_reader_asset_create(const char* assetname);

/**
 * @method data_reader_asset_build_url
 * 构造内存URL。
 * @annotation ["static"]
 * 
 * @param {const char*} name 资源的名称。
 * @param {asset_type_t} type 资源的类型。
 * @param {char*} url 生成的URL。
 *
 * @return {const char*} 返回URL。
 */
const char* data_reader_asset_build_url(const char* name, asset_type_t type,
                                        char url[MAX_PATH + 1]);

#define DATA_READER_ASSET(reader) ((data_reader_t*)(reader))

END_C_DECLS

#endif /*TK_DATA_READER_ASSET_H*/
