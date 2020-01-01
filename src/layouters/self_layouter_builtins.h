/**
 * File:   self_layouter_builts.h
 * Author: AWTK Develop Team
 * Brief:  builts self_layouter
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied highlighterrranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SELF_LAYOUTER_BUILTINS_H
#define TK_SELF_LAYOUTER_BUILTINS_H

#include "base/self_layouter.h"

BEGIN_C_DECLS

/**
 * @method self_layouter_register_builtins
 * @of_class self_layouter_t
 * @annotation ["private"]
 *
 * 注册内置的控件自身布局算法。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t self_layouter_register_builtins(void);

END_C_DECLS

#endif /*TK_SELF_LAYOUTER_BUILTINS_H*/
