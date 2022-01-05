/**
 * File:   data_reader_mem.h
 * Author: AWTK Develop Team
 * Brief:  data_reader mem
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * @class data_reader_mem_t
 * @parent data_reader_t
 * @annotation ["fake"]
 * 基于内存实现的 data_reader。通过 data_reader_factory 创建 reader 时，URL的格式如下(请用函数data_reader_mem_build_url生成)：
 * 
 * ```
 * mem://addr:size
 * ```
 */

/**
 * @method data_reader_mem_create
 * 创建基于内存的 data reader。
 * 
 * > 不要直接调用，而是注册到 data\_reader\_factory后，通过data\_reader\_factory调用。
 * @annotation ["constructor"]
 * @param {const char*} memname 内存地址和大小的字符串格式表示 。
 *
 * @return {data_reader_t*} 返回data reader对象。
 */
data_reader_t* data_reader_mem_create(const char* memname);

/**
 * @method data_reader_mem_build_url
 * 构造内存URL。
 * @annotation ["static"]
 * 
 * @param {const void*} mem 内存的地址。
 * @param {uint32_t} size 内存的长度。
 * @param {char*} url 生成的URL。
 *
 * @return {const char*} 返回URL。
 */
const char* data_reader_mem_build_url(const void* buffer, uint32_t size, char url[MAX_PATH + 1]);

#define DATA_READER_MEM(reader) ((data_reader_t*)(reader))

END_C_DECLS

#endif /*TK_DATA_READER_MEM_H*/
