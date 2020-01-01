/**
 * File:   compressor.c
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

#include "tkc/compressor.h"

ret_t compressor_compress(compressor_t* compressor, const void* data, uint32_t size,
                          wbuffer_t* out) {
  return_value_if_fail(compressor != NULL && compressor->compress, RET_BAD_PARAMS);
  return_value_if_fail(data != NULL && out != NULL && out->extendable, RET_BAD_PARAMS);

  return compressor->compress(compressor, data, size, out);
}

ret_t compressor_uncompress(compressor_t* compressor, const void* data, uint32_t size,
                            wbuffer_t* out) {
  return_value_if_fail(compressor != NULL && compressor->uncompress, RET_BAD_PARAMS);
  return_value_if_fail(data != NULL && out != NULL && out->extendable, RET_BAD_PARAMS);

  return compressor->uncompress(compressor, data, size, out);
}

ret_t compressor_destroy(compressor_t* compressor) {
  return_value_if_fail(compressor != NULL && compressor->destroy, RET_BAD_PARAMS);

  return compressor->destroy(compressor);
}
