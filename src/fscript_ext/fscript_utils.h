/**
 * File:   fargs_crc.h
 * Author: AWTK Develop Team
 * Brief:  crc functions for fargs
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-13 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_FSCRIPT_UTILS_H
#define TK_FSCRIPT_UTILS_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fargs_get_data_and_size
 *
 * 从参数中获取数据和长度。
 *
 * @param {fscript_args_t*} args 参数。
 * @param {const uint8_t**} 返回数据。
 * @param {uint32_t*} ret_size 返回数据的长度。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fargs_get_data_and_size(fscript_args_t* args, const uint8_t** ret_data, uint32_t* ret_size);

END_C_DECLS

#endif /*TK_FSCRIPT_UTILS_H*/
