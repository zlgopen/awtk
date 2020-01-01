/**
 * File:   compressor_miniz.h
 * Author: AWTK Develop Team
 * Brief:  compressor base on miniz
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed data the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-10-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COMPRESSOR_MINIZ_H
#define TK_COMPRESSOR_MINIZ_H

#include "tkc/compressor.h"

BEGIN_C_DECLS

/**
 * @class compressor_miniz_t
 * @parent compressor_t
 * @annotation ["fake"]
 *
 * 基于miniz实现的compressor接口。
 *
 */

/**
 * @method compressor_miniz_create
 *
 * 创建compressor对象。
 *
 * @param {compressor_options_t} options 选项。
 *
 * @return {compressor_t*} 返回compressor对象。
 *
 */
compressor_t* compressor_miniz_create(compressor_options_t options);

END_C_DECLS

#endif /*TK_COMPRESSOR_MINIZ_H*/
