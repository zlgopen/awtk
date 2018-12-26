/*
 * font_loader_ft.h
 *
 *  Created on: 2018Äê12ÔÂ26ÈÕ
 *      Author: zjm09
 */

#ifndef AWTK_SRC_FONT_LOADER_FONT_LOADER_FT_H_
#define AWTK_SRC_FONT_LOADER_FONT_LOADER_FT_H_

#include "base/font_loader.h"

BEGIN_C_DECLS

font_t* font_ft_create(const char* name, const uint8_t* buff, uint32_t size);

font_loader_t* font_loader_ft(void);

END_C_DECLS

#endif /* AWTK_SRC_FONT_LOADER_FONT_LOADER_FT_H_ */
