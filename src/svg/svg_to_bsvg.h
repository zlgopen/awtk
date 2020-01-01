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

ret_t svg_to_bsvg(const char* xml, uint32_t size, uint32_t** out, uint32_t* out_size);

/*public for test*/
uint32_t svg_parse_numbers(const char* str, float* out, uint32_t max_nr);

END_C_DECLS

#endif /*TK_SVG_TO_BSVG_H*/
