/**
 * File:   dialog_highlighter_default.h
 * Author: AWTK Develop Team
 * Brief:  default dialog_highlighter
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_DIALOG_HIGHLIGHTER_DEFAULT_H
#define TK_DIALOG_HIGHLIGHTER_DEFAULT_H

#include "base/dialog_highlighter.h"

BEGIN_C_DECLS

struct _dialog_highlighter_default_t;
typedef struct _dialog_highlighter_default_t dialog_highlighter_default_t;

struct _dialog_highlighter_default_t {
  dialog_highlighter_t dialog_highlighter;
  
  uint8_t start_alpha;
  uint8_t end_alpha;
};

dialog_highlighter_t* dialog_highlighter_default_create(object_t* args);

#define DIALOG_HIGHLIGHTER_DEFAULT_PROP_START_ALPHA "start_alpha"
#define DIALOG_HIGHLIGHTER_DEFAULT_PROP_END_ALPHA "end_alpha"
#define DIALOG_HIGHLIGHTER_DEFAULT_PROP_ALPHA "alpha"

END_C_DECLS

#endif /*TK_DIALOG_HIGHLIGHTER_DEFAULT_H*/
