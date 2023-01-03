/**
 * File:   bsvg_common.h
 * Author: AWTK Develop Team
 * Brief:  svg common
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-11-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BSVG_COMMON_H
#define TK_BSVG_COMMON_H

#include "svg/svg_shape.h"
#include "svg/svg_path.h"

BEGIN_C_DECLS

#pragma pack(push, 1)
typedef struct _bsvg_header_t {
  uint32_t magic;
  uint8_t version;
  uint8_t reserved[3];
  uint32_t w;
  uint32_t h;
} bsvg_header_t;
#pragma pack(pop)

#define BSVG_MAGIC 0x20181115u
#define BSVG_VERSION ((uint8_t)2)
#define BSVG_MIN_SIZE (sizeof(bsvg_header_t) + sizeof(svg_shape_t))

END_C_DECLS

#endif /*TK_BSVG_COMMON_H*/
