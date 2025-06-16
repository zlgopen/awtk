/**
 * File:   self_layouter_flex.h
 * Author: AWTK Develop Team
 * Brief:  self layouter flex
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-06-11 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_SELF_LAYOUTER_FLEX_H
#define TK_SELF_LAYOUTER_FLEX_H

#include "layouters/self_layouter_default.h"

BEGIN_C_DECLS

/**
 * @class self_layouter_flex_t
 * @parent self_layouter_default_t
 *
 * 弹性自身布局算法实现。
 *
 * > 用法请参考：[弹性自身布局器](
 * https://github.com/zlgopen/awtk/blob/master/docs/self_layouter_flex.md)
 *
 */
typedef struct _self_layouter_flex_t {
  self_layouter_default_t layouter;
} self_layouter_flex_t;

/**
 * @method self_layouter_flex_create
 * @annotation ["constructor"]
 *
 * 创建控件自身布局对象。
 *
 * @return {self_layouter_t*} 返回创建控件自身布局对象。
 *
 */
self_layouter_t* self_layouter_flex_create(void);

bool_t self_layouter_flex_is_valid(self_layouter_t* layouter);

#define SELF_LAYOUTER_FLEX "flex"

END_C_DECLS

#endif /*TK_SELF_LAYOUTER_FLEX_H*/
