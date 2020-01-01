/**
 * File:   compressor.h
 * Author: AWTK Develop Team
 * Brief:  compressor interface
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed data the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-10-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COMPRESSOR_H
#define TK_COMPRESSOR_H

#include "tkc/buffer.h"

BEGIN_C_DECLS

struct _compressor_t;
typedef struct _compressor_t compressor_t;

typedef ret_t (*compressor_compress_t)(compressor_t* compressor, const void* data, uint32_t size,
                                       wbuffer_t* out);
typedef ret_t (*compressor_uncompress_t)(compressor_t* compressor, const void* data, uint32_t size,
                                         wbuffer_t* out);
typedef ret_t (*compressor_destroy_t)(compressor_t* compressor);

typedef enum _compressor_options_t {
  COMPRESSOR_SPEED_FIRST = 0,
  COMPRESSOR_RATIO_FIRST
} compressor_options_t;

/**
 * @class compressor_t
 *
 * 压缩解压接口。
 *
 */
struct _compressor_t {
  const char* type;
  compressor_options_t options;

  compressor_compress_t compress;
  compressor_uncompress_t uncompress;
  compressor_destroy_t destroy;
};

/**
 * @method compressor_compress
 *
 * 压缩数据。
 *
 * @param {compressor_t*} compressor compressor对象。
 * @param {const void*} data 待压缩的数据。
 * @param {uint32_t} size 数据长度。
 * @param {wbuffer_t*} out 压缩之后的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t compressor_compress(compressor_t* compressor, const void* data, uint32_t size,
                          wbuffer_t* out);

/**
 * @method compressor_uncompress
 *
 * 解压数据。
 *
 * @param {compressor_t*} compressor compressor对象。
 * @param {const void*} data 待解压的数据。
 * @param {uint32_t} size 数据长度。
 * @param {wbuffer_t*} out 解压之后的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t compressor_uncompress(compressor_t* compressor, const void* data, uint32_t size,
                            wbuffer_t* out);

/**
 * @method compressor_destroy
 *
 * 销毁compressor对象。
 *
 * @param {compressor_t*} compressor compressor对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t compressor_destroy(compressor_t* compressor);

#define TK_COMPRESSOR(obj) ((compressor_t*)(obj))

END_C_DECLS

#endif /*TK_COMPRESSOR_H*/
