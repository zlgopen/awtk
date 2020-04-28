/**
 * File:   svg_to_bsvg.c
 * Author: AWTK Develop Team
 * Brief:  svg to  bsvg
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-11-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SVG_TO_BSVG_H
#define TK_SVG_TO_BSVG_H

#include "svg/bsvg.h"

BEGIN_C_DECLS

/**
 * @class svg_helper_t
 * @annotation ["fake"]
 */

/**
 * @method svg_to_bsvg
 *
 * svg to bsvg
 *
 * @param {const char*} xml svg数据。
 * @param {uint32_t} size svg数据长度。
 * @param {uint32_t*} out 用于返回bsvg数据。
 * @param {uint32_t*} out_size bsvg数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t svg_to_bsvg(const char* xml, uint32_t size, uint32_t** out, uint32_t* out_size);

/*public for test*/
uint32_t svg_parse_numbers(const char* str, float* out, uint32_t max_nr);

END_C_DECLS

#endif /*TK_SVG_TO_BSVG_H*/
