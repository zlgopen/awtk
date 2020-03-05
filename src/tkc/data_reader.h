/**
 * File:   data_reader.h
 * Author: AWTK Develop Team
 * Brief:  data_reader
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

#ifndef TK_DATA_READER_H
#define TK_DATA_READER_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

struct _data_reader_t;
typedef struct _data_reader_t data_reader_t;

typedef int32_t (*data_reader_read_t)(data_reader_t* reader, uint64_t offset, void* data,
                                      uint32_t size);
typedef uint64_t (*data_reader_get_size_t)(data_reader_t* reader);
typedef ret_t (*data_reader_destroy_t)(data_reader_t* reader);

typedef struct _data_reader_vtable_t {
  data_reader_read_t read;
  data_reader_get_size_t get_size;
  data_reader_destroy_t destroy;
} data_reader_vtable_t;

/**
 * @class data_reader_t
 * 数据读取接口。
 *
 * >用于抽象flash等外部设备。
 *
 */
struct _data_reader_t {
  const data_reader_vtable_t* vt;
};

/**
 * @method data_reader_read
 * 在指定位置读取数据。
 *
 * @param {data_reader_t*} reader reader对象。
 * @param {uint64_t} offset 偏移量。
 * @param {void*} data 用于读取数据的缓冲区。
 * @param {uint32_t} size 最大读取数据长度。
 *
 * @return {int32_t} 返回实际读取数据的长度。
 */
int32_t data_reader_read(data_reader_t* reader, uint64_t offset, void* data, uint32_t size);

/**
 * @method data_reader_get_size
 * 获取数据长度。
 *
 * @param {data_reader_t*} reader reader对象。
 *
 * @return {uint64_t} 返回数据长度。
 */
uint64_t data_reader_get_size(data_reader_t* reader);

/**
 * @method data_reader_destroy
 * 销毁reader对象。
 * @param {data_reader_t*} reader reader对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t data_reader_destroy(data_reader_t* reader);

/**
 * @method data_reader_read_all
 * 从指定的URL读取全部数据。
 * @param {const char*} url URL。
 * @param {uint32_t*} size 返回数据长度。
 *
 * @return {void*} 返回全部数据，调用者需要调用TKMEM_FREE释放返回值。
 */
void* data_reader_read_all(const char* url, uint32_t* size);

#define DATA_READER(reader) ((data_reader_t*)(reader))

END_C_DECLS

#endif /*TK_DATA_READER_H*/
