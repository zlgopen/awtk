/**
 * File:   svg_path_parser.h
 * Author: AWTK Develop Team
 * Brief:  svg path_parser
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-11-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SVG_PATH_BUILDER_H
#define TK_SVG_PATH_BUILDER_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class svg_path_t
 * @annotation ["fake"]
 */

/**
 * @method svg_path_parse
 *
 * 解析路径。
 *
 * @param {const char*} path 路径数据。
 * @param {void*} ctx 回调函数上下文。
 * @param {tk_visit_t} on_path 路径处理回调函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t svg_path_parse(const char* path, void* ctx, tk_visit_t on_path);

END_C_DECLS

#endif /*TK_SVG_PATH_BUILDER_H*/
