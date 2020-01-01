/**
 * File:   font_stb.h
 * Author: AWTK Develop Team
 * Brief:  truetype font loader
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
 * 2018-12-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FONT_LOADER_TRUE_TYPE_H
#define TK_FONT_LOADER_TRUE_TYPE_H

#ifdef WITH_STB_FONT

#define WITH_TRUETYPE_FONT 1
#include "font_loader/font_loader_stb.h"
#define font_truetype_create font_stb_create
#define font_loader_truetype font_loader_stb

#elif defined(WITH_FT_FONT)

#define WITH_TRUETYPE_FONT 1
#include "font_loader/font_loader_ft.h"
#define font_truetype_create font_ft_create
#define font_loader_truetype font_loader_ft

#else
#undef WITH_TRUETYPE_FONT
#define font_truetype_create NULL
#define font_loader_truetype NULL
#endif /*WITH_STB_FONT*/

#endif /*TK_FONT_LOADER_TRUE_TYPE_H*/
