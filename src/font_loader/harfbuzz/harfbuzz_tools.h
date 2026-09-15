/**
 * File:   harfbuzz_tools.h
 * Author: AWTK Develop Team
 * Brief:  harfbuzz tools
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

#ifndef HARFBUZZ_TOOLS_H
#define HARFBUZZ_TOOLS_H

#include "base/bidi.h"
#include "base/font.h"
#include "harfbuzz_loader.h"
#include "harfbuzz_data_loader.h"

/**
 * @method hb_set_normal_glyphs_chr
 * 固定整形模式设置普通字符串的字符
 * @annotation ["static"]
 * @param {glyphs_t*} glyphs glyphs对象。
 * @param {bidi_t*} b bidi对象
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hb_set_normal_glyphs_chr(glyphs_t* glyphs, bidi_t* b);

/**
 * @method hb_set_hb_glyphs_chr
 * 固定整形模式设置harfbuzz字符串的字符
 * @annotation ["static"]
 * @param {glyphs_t*} glyphs glyphs对象。
 * @param {hb_info_t*} info harfbuzz信息对象
 * @param {uint32_t} len 信息对象数组的长度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hb_set_hb_glyphs_chr(glyphs_t* glyphs, hb_info_t* info, uint32_t len);

/**
 * @method hb_set_glyphs_maps
 * 固定整形模式设置harfbuzz字符串的maps表
 * @annotation ["static"]
 * @param {glyphs_t*} glyphs glyphs对象。
 * @param {hb_info_t*} info harfbuzz信息对象
 * @param {uint32_t} len 信息对象数组的长度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t hb_set_glyphs_maps(glyphs_t* glyphs, hb_info_t* info, uint32_t len);

#endif /*HARFBUZZ_TOOLS_H*/
