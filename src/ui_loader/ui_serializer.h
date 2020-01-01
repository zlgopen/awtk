/**
 * File:   ui_serializer.h
 * Author: AWTK Develop Team
 * Brief:  ui_serializer
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
 * 2018-07-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UI_SERIALIZER_H
#define TK_UI_SERIALIZER_H

#include "ui_loader/ui_xml_writer.h"

BEGIN_C_DECLS

ret_t widget_to_xml(widget_t* widget, str_t* str);

END_C_DECLS

#endif /*TK_UI_SERIALIZER_H*/
