/**
 * File:   ui_builder_writer.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  ui_builder write widget info into buffer.
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef TK_UI_BUILDER_WRITER_H
#define TK_UI_BUILDER_WRITER_H

#include "base/buffer.h"
#include "ui_loader/ui_builder.h"

BEGIN_C_DECLS

ui_builder_t* ui_builder_writer(wbuffer_t* wbuffer);

END_C_DECLS

#endif /*TK_UI_BUILDER_WRITER_H*/
