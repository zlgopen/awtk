/*
 * font_loader_ft.h
 *
 *  Created on: 2018年12月26日
 *      Author: zjm09
 */

#ifndef AWTK_SRC_FONT_LOADER_FONT_LOADER_FT_H_
#define AWTK_SRC_FONT_LOADER_FONT_LOADER_FT_H_

#include "base/font_loader.h"

BEGIN_C_DECLS

font_t* font_ft_create(const char* name, const uint8_t* buff, uint32_t size);
font_t* font_ft_mono_create(const char* name, const uint8_t* buff, uint32_t size);

/**
 * @class font_loader_ft_t
 * @parent font_loader_t
 * freetype字体加载器。
 * @annotation["fake"]
 *
 * freetype主要用于加载矢量字体(如truetype)，它功能强大，效果优异，与stb字体加载器相比，缺点是代码量比较大。
 * 如果flash空间够大，优先使用freetype，否则使用stb。
 *
 */

/**
 * @method font_loader_ft
 * @annotation ["constructor"]
 *
 * 获取freetype字体加载器对象。
 *
 * @return {font_loader_t*} 返回字体加载器对象。
 */
font_loader_t* font_loader_ft(void);

font_loader_t* font_loader_ft_mono(void);

END_C_DECLS

#endif /* AWTK_SRC_FONT_LOADER_FONT_LOADER_FT_H_ */
