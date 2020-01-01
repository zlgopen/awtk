/**
 * File:   font_stb.h
 * Author: AWTK Develop Team
 * Brief:  stb truetype font loader
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FONT_LOADER_STB_H
#define TK_FONT_LOADER_STB_H

#include "base/font_loader.h"

BEGIN_C_DECLS

font_t* font_stb_create(const char* name, const uint8_t* buff, uint32_t size);

/**
 * @class font_loader_stb_t
 * @parent font_loader_t
 * stb字体加载器。
 *
 * stb主要用于加载矢量字体(如truetype)，它功能强大，体积小巧。
 *
 * @annotation["fake"]
 *
 */

/**
 * @method font_loader_stb
 * @annotation ["constructor"]
 *
 * 获取stb字体加载器对象。
 *
 * @return {font_loader_t*} 返回字体加载器对象。
 */
font_loader_t* font_loader_stb(void);

END_C_DECLS

#endif /*TK_FONT_LOADER_STB_H*/
