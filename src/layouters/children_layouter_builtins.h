/**
 * File:   children_layouter_builts.h
 * Author: AWTK Develop Team
 * Brief:  builts children_layouter
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
 * 2019-05-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CHILDREN_LAYOUTER_BUILTINS_H
#define TK_CHILDREN_LAYOUTER_BUILTINS_H

#include "base/children_layouter.h"

BEGIN_C_DECLS

/**
 * @method children_layouter_register_builtins
 * @annotation ["private"]
 *
 * 注册内置的子控件布局算法。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t children_layouter_register_builtins(void);

END_C_DECLS

#endif /*TK_CHILDREN_LAYOUTER_BUILTINS_H*/
