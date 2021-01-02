/**
 * File:   data_writer_file.h
 * Author: AWTK Develop Team
 * Brief:  data_writer file
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-03-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DATA_WRITER_FILE_H
#define TK_DATA_WRITER_FILE_H

#include "tkc/data_writer.h"

BEGIN_C_DECLS

/**
 * @class data_writer_file_t
 * @parent data_writer_t
 * @annotation ["fake"]
 * 基于文件实现的 data_writer。通过 data_writer_factory 创建 writer 时，URL的格式如下：
 * 
 * ```
 * file://filename
 * ```
 */

/**
 * @method data_writer_file_create
 * 创建基于文件的data writer。
 * 
 * > 不要直接调用，而是注册到 data\_writer\_factory后，通过data\_writer\_factory调用
 * @annotation ["constructor"]
 * 
 * @param {const char*} filename 文件名。
 *
 * @return {data_writer_t*} 返回data writer对象。
 */
data_writer_t* data_writer_file_create(const char* filename);

END_C_DECLS

#endif /*TK_DATA_WRITER_FILE_H*/
