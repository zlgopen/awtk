/**
 * File:   preprocess_text.h
 * Author: AWTK Develop Team
 * Brief:  preprocess_text
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-12-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "tkc/str.h"

#ifndef TK_PREPROCESS_TEXT_H
#define TK_PREPROCESS_TEXT_H

BEGIN_C_DECLS

const char* font_gen_expand_text(const char* in, str_t* out);

END_C_DECLS

#endif /*TK_PREPROCESS_TEXT_H*/
