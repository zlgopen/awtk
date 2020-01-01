/**
 * File:   ui_loader_default.h
 * Author: AWTK Develop Team
 * Brief:  default ui_loader
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UI_LOADER_DEFAULT_H
#define TK_UI_LOADER_DEFAULT_H

#include "base/ui_loader.h"

BEGIN_C_DECLS

/**
 * @class ui_loader_default_t
 * @parent ui_loader_t
 *
 * 二进制格式的UI资源加载器。
 *
 * @annotation["fake"]
 *
 */

/**
 * @method default_ui_loader
 * @annotation ["constructor"]
 *
 * 获取二进制格式格式的UI资源加载器对象。
 *
 * @return {ui_loader_t*} 返回UI加载器对象。
 *
 */

ui_loader_t* default_ui_loader(void);

END_C_DECLS

#endif /*TK_UI_LOADER_DEFAULT_H*/
