/**
 * File:   data_writer_file.h
 * Author: AWTK Develop Team
 * Brief:  data_writer file
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
 * 2020-03-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DATA_WRITER_FILE_H
#define TK_DATA_WRITER_FILE_H

#include "tkc/data_writer.h"

BEGIN_C_DECLS

/**
 * @method data_writer_file_create
 * 创建基于文件的data writer。
 * @param {const char*} filename 文件名。
 *
 * @return {data_writer_t*} 返回data writer对象。
 */
data_writer_t* data_writer_file_create(const char* filename);

#define DATA_WRITER_FILE(writer) ((data_writer_t*)(writer))

END_C_DECLS

#endif /*TK_DATA_WRITER_FILE_H*/
