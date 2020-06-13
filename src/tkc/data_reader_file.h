/**
 * File:   data_reader_file.h
 * Author: AWTK Develop Team
 * Brief:  data_reader file
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

#ifndef TK_DATA_READER_FILE_H
#define TK_DATA_READER_FILE_H

#include "tkc/data_reader.h"

BEGIN_C_DECLS

/**
 * @method data_reader_file_create
 * 创建基于文件的data reader。
 * @param {const char*} filename 文件名。
 *
 * @return {data_reader_t*} 返回data reader对象。
 */
data_reader_t* data_reader_file_create(const char* filename);

#define DATA_READER_FILE(reader) ((data_reader_t*)(reader))

END_C_DECLS

#endif /*TK_DATA_READER_FILE_H*/
