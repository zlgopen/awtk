/**
 * File:   dialog_highlighter.h
 * Author: AWTK Develop Team
 * Brief:  dialog_highlighter
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

#ifndef TK_DIALOG_HIGHLIGHTER_H
#define TK_DIALOG_HIGHLIGHTER_H

#include "base/widget.h"

BEGIN_C_DECLS

struct _dialog_highlighter_t;
typedef struct _dialog_highlighter_t dialog_highlighter_t;

typedef ret_t (*dialog_highlighter_prepare_t)(dialog_highlighter_t* h, canvas_t* c);
typedef ret_t (*dialog_highlighter_draw_t)(dialog_highlighter_t* h, float_t percent);
typedef ret_t (*dialog_highlighter_on_destroy_t)(dialog_highlighter_t* h);

typedef dialog_highlighter_t* (*dialog_highlighter_create_t)(object_t* args);

typedef struct _dialog_highlighter_vtable_t {
  const char* type;
  const char* desc;
  uint32_t size;
  dialog_highlighter_prepare_t prepare;
  dialog_highlighter_draw_t draw;
  dialog_highlighter_on_destroy_t on_destroy;
} dialog_highlighter_vtable_t;

/**
 * @enum dialog_highlighter_type_t
 * @prefix DIALOG_HIGHLIGHTER_
 * @type string
 * 内置的highlighter。
 */

/**
 * @const DIALOG_HIGHLIGHTER_DEFAULT
 * 缺省的highlighter。
 */
#define DIALOG_HIGHLIGHTER_DEFAULT "default"

/**
 * @class dialog_highlighter_t
 * dialog_highlighte。
 */
struct _dialog_highlighter_t {
  bitmap_t img;
  canvas_t* canvas;
  framebuffer_object_t fbo;

  const dialog_highlighter_vtable_t* vt;
};

dialog_highlighter_t* dialog_highlighter_create(const dialog_highlighter_vtable_t* vt);
ret_t dialog_highlighter_prepare(dialog_highlighter_t* h, canvas_t* c);
ret_t dialog_highlighter_set_bg(dialog_highlighter_t* h, bitmap_t* img, framebuffer_object_t* fbo);
ret_t dialog_highlighter_draw(dialog_highlighter_t* h, float_t percent);
ret_t dialog_highlighter_on_destroy(dialog_highlighter_t* h);

/**
 * @method dialog_highlighter_destroy
 * 销毁窗口高亮对象。
 * @param {dialog_highlighter_t*} h 窗口高亮对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_highlighter_destroy(dialog_highlighter_t* h);

END_C_DECLS

#endif /*TK_DIALOG_HIGHLIGHTER_H*/
