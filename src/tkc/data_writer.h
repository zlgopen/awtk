/**
 * File:   data_writer.h
 * Author: AWTK Develop Team
 * Brief:  data_writer
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

#ifndef TK_DATA_WRITER_H
#define TK_DATA_WRITER_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

struct _data_writer_t;
typedef struct _data_writer_t data_writer_t;

typedef int32_t (*data_writer_write_t)(data_writer_t* writer, uint64_t offset, const void* data,
                                       uint32_t size);
typedef ret_t (*data_writer_destroy_t)(data_writer_t* writer);

typedef struct _data_writer_vtable_t {
  data_writer_write_t write;
  data_writer_destroy_t destroy;
} data_writer_vtable_t;

/**
 * @class data_writer_t
 * 数据写入接口。
 *
 * >用于抽象flash等外部设备。
 *
 */
struct _data_writer_t {
  const data_writer_vtable_t* vt;
};

/**
 * @method data_writer_write
 * 在指定位置写入数据。
 *
 * @param {data_writer_t*} writer writer对象。
 * @param {uint64_t} offset 偏移量。
 * @param {const void*} data 数据缓冲区。
 * @param {uint32_t} size 数据长度。
 *
 * @return {int32_t} 返回实际读取数据的长度。
 */
int32_t data_writer_write(data_writer_t* writer, uint64_t offset, const void* data, uint32_t size);

/**
 * @method data_writer_destroy
 * 销毁writer对象。
 * @param {data_writer_t*} writer writer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t data_writer_destroy(data_writer_t* writer);

#define DATA_WRITER(writer) ((data_writer_t*)(writer))

END_C_DECLS

#endif /*TK_DATA_WRITER_H*/
