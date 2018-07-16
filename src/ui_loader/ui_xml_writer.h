/**
 * File:   ui_xml_writer.h
 * Author: AWTK Develop Team
 * Brief:  ui_builder write widget info into buffer.
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_UI_XML_WRITER_H
#define TK_UI_XML_WRITER_H

#include "base/str.h"
#include "base/ui_builder.h"

BEGIN_C_DECLS

typedef struct _ui_xml_writer_t {
  ui_builder_t builder;
  str_t* str;
  str_t tags;
  str_t temp;
  uint32_t level;
} ui_xml_writer_t;

ui_builder_t* ui_xml_writer_init(ui_xml_writer_t* writer, str_t* str);

END_C_DECLS

#endif /*TK_UI_XML_WRITER_H*/
