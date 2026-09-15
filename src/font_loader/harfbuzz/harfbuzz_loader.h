/**
 * File:   harfbuzz_loader.h
 * Author: AWTK Develop Team
 * Brief:  harfbuzz loader
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

#ifndef HARFBUZZ_LOADER_H
#define HARFBUZZ_LOADER_H

#include "awtk.h"
#include "hb_info.h"

#ifdef WITH_HARFBUZZ_TEXT_SHAPING
#include "3rd/harfbuzz/hb.h"
#endif

BEGIN_C_DECLS

/**
 * @method harfbuzz_shape_text
 * 调用harfbuzz库整形字符串。
 * @param {const wchar_t*} text 要整形的字符串。
 * @param {uint32_t} text_len 字符串的长度。
 * @param {const char*} font_name 字体名称（需要在资源管理器缓存中存在）。
 * @param {uint32_t*} glyph_count 用于接收整形后的字模个数。
 * @param {void*} hb_font harfbuzz字体对象。
 *
 * @return {hb_info_t*} hb_info 用于接收整形后的字模位置信息（只负责申请内存不负责销毁，需要用户调用harfbuzz_shape_destroy函数销毁），返回NULL为失败。
 */
hb_info_t* harfbuzz_shape_text(const wchar_t* text, uint32_t text_len, const char* font_name,
                               uint32_t* glyph_count, void* hb_font);

/**
 * @method harfbuzz_shape_text_destroy
 * 销毁harfbuzz信息对象
 * @param {hb_info_t*} hb_info harfbuzz信息对象
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t harfbuzz_shape_text_destroy(hb_info_t* hb_info);

END_C_DECLS

#endif /* HARFBUZZ_LOADER_H */