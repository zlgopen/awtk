/**
 * File:   font_gen_tools.h
 * Author: AWTK Develop Team
 * Brief:  font_gen 专用的字体创建接口
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 */

/**
 * History:
 * ================================================================
 * 2026-08-04 Yang Zewu <yangzewu@zlg.cn> created
 *
 */

#ifndef FONT_GEN_TOOLS_H
#define FONT_GEN_TOOLS_H

#include "base/font.h"

BEGIN_C_DECLS

/**
 * @method font_gen_create_font
 * 创建字体对象，供 font_gen 烘焙使用（gen_one 的统一入口）。
 * @annotation ["constructor"]
 * @param {const char*} name 字体名称。
 * @param {const uint8_t*} buff 字体数据。
 * @param {uint32_t} size 字体数据长度。
 * @param {bool_t} mono 是否单色。
 *
 * @return {font_t*} 返回字体对象。
 */
font_t* font_gen_create_font(const char* name, const uint8_t* buff, uint32_t size, bool_t mono);

END_C_DECLS

#endif /* FONT_GEN_TOOLS_H */
