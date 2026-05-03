/**
 * File:   awtk_arabic_shaping.h
 * Author: AWTK Develop Team
 * Brief:  Arabic joining + presentation shaping (Unicode table-driven, no FriBidi).
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 */

#ifndef AWTK_ARABIC_SHAPING_H
#define AWTK_ARABIC_SHAPING_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * Apply Arabic cursive joining + mandatory presentation shaping + mandatory ligatures
 * on a logical-order buffer, using caller-provided embedding levels (e.g. from SheenBidi).
 *
 * `shaped` must hold a copy of the logical string; it is updated in place.
 * `embedding_levels` has one byte per code unit (same layout as SheenBidi `SBLevel`).
 */
ret_t awtk_arabic_shape_logical(const wchar_t* str, uint32_t size, const uint8_t* embedding_levels,
                                wchar_t* shaped);

END_C_DECLS

#endif /* AWTK_ARABIC_SHAPING_H */
