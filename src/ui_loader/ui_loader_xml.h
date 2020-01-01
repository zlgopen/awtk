/**
 * File:   ui_loader_xml.h
 * Author: AWTK Develop Team
 * Brief:  xml ui_loader
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
 * 2018-02-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UI_LOADER_XML_H
#define TK_UI_LOADER_XML_H

#include "base/ui_loader.h"

BEGIN_C_DECLS

/**
 * @class ui_loader_xml_t
 * @parent ui_loader_t
 *
 * XML格式的UI资源加载器。
 *
 * @annotation["fake"]
 *
 */

/**
 * @method xml_ui_loader
 * @annotation ["constructor"]
 *
 * 获取XML格式的UI资源加载器对象。
 *
 * @return {ui_loader_t*} 返回UI加载器对象。
 *
 */
ui_loader_t* xml_ui_loader(void);

END_C_DECLS

#endif /*TK_UI_LOADER_XML_H*/
