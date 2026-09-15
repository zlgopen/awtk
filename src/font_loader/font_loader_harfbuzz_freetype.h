/**
 * File:   font_loader_harfbuzz_freetype.h
 * Author: AWTK Develop Team
 * Brief:  freetype with harfbuzz font loader
 *
 * Copyright (c) 2018 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-03-27 Zhao Yingjian <zhaoyingjian@zlg.cn> created
 *
 */

#ifndef AWTK_SRC_FONT_LOADER_FONT_LOADER_HARFBUZZ_FREETYPE_H_
#define AWTK_SRC_FONT_LOADER_FONT_LOADER_HARFBUZZ_FREETYPE_H_

#include "base/font_loader.h"

BEGIN_C_DECLS

font_t* font_hb_ft_create(const char* name, const uint8_t* buff, uint32_t size);
font_t* font_hb_ft_mono_create(const char* name, const uint8_t* buff, uint32_t size);

/**
 * @class font_loader_hb_ft_t
 * @parent font_loader_t
 * freetype字体加载器。
 * @annotation["fake"]
 *
 * freetype主要用于加载矢量字体(如truetype)，它功能强大，效果优异，与stb字体加载器相比，缺点是代码量比较大。
 * 如果flash空间够大，优先使用freetype，否则使用stb。
 *
 */

/**
 * @method font_loader_hb_ft
 * @annotation ["constructor"]
 *
 * 获取freetype字体加载器对象。
 *
 * @return {font_loader_t*} 返回字体加载器对象。
 */
font_loader_t* font_loader_hb_ft(void);

font_loader_t* font_loader_hb_ft_mono(void);

END_C_DECLS

#endif /* AWTK_SRC_FONT_LOADER_FONT_LOADER_HARFBUZZ_FREETYPE_H_ */
