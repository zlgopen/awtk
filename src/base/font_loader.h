/**
 * File:   font_loader.h
 * Author: AWTK Develop Team
 * Brief:  font_loader interface
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
 * 2018-10-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FONT_LOADER_H
#define TK_FONT_LOADER_H

#include "base/font.h"
#include "base/assets_manager.h"

BEGIN_C_DECLS

struct _font_loader_t;
typedef struct _font_loader_t font_loader_t;

typedef font_t* (*font_loader_load_t)(font_loader_t* loader, const char* name, const uint8_t* buff,
                                      uint32_t size);

/**
 * @class font_loader_t
 * 字体加载接口。
 */
struct _font_loader_t {
  asset_font_type_t type;

  font_loader_load_t load;
};

/**
 * @method font_loader_load
 * 加载字体。
 *
 * @param {font_loader_t*} loader loader对象。
 * @param {const char*} name 字体名字。
 * @param {uint8_t*} buff 字体数据。
 * @param {uint32_t} size 字体数据长度。
 *
 * @return {font_t*} 成功返回font，失败返回NULL。
 */
font_t* font_loader_load(font_loader_t* loader, const char* name, const uint8_t* buff,
                         uint32_t size);

END_C_DECLS

#endif /*TK_FONT_LOADER_H*/
