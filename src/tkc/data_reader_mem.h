/**
 * File:   data_reader_mem.h
 * Author: AWTK Develop Team
 * Brief:  data_reader mem
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License mem for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-12-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DATA_READER_MEM_H
#define TK_DATA_READER_MEM_H

#include "tkc/utils.h"
#include "tkc/data_reader.h"

BEGIN_C_DECLS

/**
 * @method data_reader_mem_create
 * 创建基于文件的data reader。
 * @param {const char*} memname 文件名。
 *
 * @return {data_reader_t*} 返回data reader对象。
 */
data_reader_t* data_reader_mem_create(const char* memname);

/**
 * @method data_reader_mem_build_url
 *
 * @param {mem_t*} mem pointer to mem 。
 * @param {uint32_t} size memory size
 * @param {char*} url url.
 *
 * @return {const char*} 返回URL。
 */
const char* data_reader_mem_build_url(const void* buffer, uint32_t size, char url[TK_NAME_LEN + 1]);

#define DATA_READER_MEM(reader) ((data_reader_t*)(reader))

END_C_DECLS

#endif /*TK_DATA_READER_MEM_H*/
