/**
 * File:   dialog_highlighter.c
 * Author: AWTK Develop Team
 * Brief:  dialog_highlighter
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/dialog_highlighter.h"

ret_t dialog_highlighter_clear_image(dialog_highlighter_t* h) {
  if (h->img.buffer != NULL) {
    bitmap_destroy(&(h->img));
    memset(&(h->img), 0x00, sizeof(h->img));
  }

  return RET_OK;
}

dialog_highlighter_t* dialog_highlighter_create(const dialog_highlighter_vtable_t* vt) {
  dialog_highlighter_t* h = NULL;
  return_value_if_fail(vt != NULL && vt->size >= sizeof(dialog_highlighter_t), NULL);

  h = (dialog_highlighter_t*)TKMEM_ALLOC(vt->size);
  return_value_if_fail(h != NULL, NULL);

  memset(h, 0x00, vt->size);
  emitter_init(EMITTER(h));
  h->vt = vt;

  return h;
}

ret_t dialog_highlighter_prepare_ex(dialog_highlighter_t* h, canvas_t* c,
                                    canvas_t* canvas_offline) {
  return_value_if_fail(h != NULL && h->vt != NULL && c != NULL, RET_BAD_PARAMS);

  h->canvas = c;
  if (h->vt->prepare != NULL) {
    return h->vt->prepare(h, canvas_offline);
  }

  return RET_NOT_IMPL;
}

ret_t dialog_highlighter_prepare(dialog_highlighter_t* h, canvas_t* c) {
  return dialog_highlighter_prepare_ex(h, c, c);
}

ret_t dialog_highlighter_set_bg(dialog_highlighter_t* h, bitmap_t* img) {
  return_value_if_fail(h != NULL && h->vt != NULL, RET_BAD_PARAMS);

  dialog_highlighter_clear_image(h);

  if (img != NULL) {
    h->img = *img;
  }

  return RET_OK;
}

ret_t dialog_highlighter_set_bg_clip_rect(dialog_highlighter_t* h, rect_t* clip_rect) {
  return_value_if_fail(h != NULL && h->vt != NULL, RET_BAD_PARAMS);

  h->clip_rect = rect_init(clip_rect->x, clip_rect->y, clip_rect->w, clip_rect->h);

  return RET_OK;
}

ret_t dialog_highlighter_set_system_bar_alpha(dialog_highlighter_t* h, uint8_t alpha) {
  return_value_if_fail(h != NULL && h->vt != NULL, RET_BAD_PARAMS);

  if (h->vt->set_system_bar_alpha != NULL) {
    return h->vt->set_system_bar_alpha(h, alpha);
  }

  return RET_NOT_IMPL;
}

uint8_t dialog_highlighter_get_alpha(dialog_highlighter_t* h, float_t percent) {
  return_value_if_fail(h != NULL && h->vt != NULL, 0x0);

  if (h->vt->get_alpha != NULL) {
    return h->vt->get_alpha(h, percent);
  }

  return 0x0;
}

ret_t dialog_highlighter_draw(dialog_highlighter_t* h, float_t percent) {
  return_value_if_fail(h != NULL && h->vt != NULL, RET_BAD_PARAMS);

  if (h->vt->draw != NULL) {
    return h->vt->draw(h, percent);
  }

  return RET_NOT_IMPL;
}

ret_t dialog_highlighter_draw_mask(dialog_highlighter_t* h, canvas_t* c, float_t percent) {
  return_value_if_fail(h != NULL && h->vt != NULL, RET_BAD_PARAMS);

  if (h->vt->draw_mask != NULL) {
    return h->vt->draw_mask(h, c, percent);
  }

  return RET_NOT_IMPL;
}

bool_t dialog_highlighter_is_dynamic(dialog_highlighter_t* h) {
  if (h != NULL && h->vt != NULL && h->vt->is_dynamic != NULL) {
    return h->vt->is_dynamic(h);
  } else {
    return FALSE;
  }
}

static ret_t dialog_highlighter_on_destroy(dialog_highlighter_t* h) {
  return_value_if_fail(h != NULL && h->vt != NULL, RET_BAD_PARAMS);

  if (h->vt->on_destroy != NULL) {
    h->vt->on_destroy(h);
  }

  return RET_OK;
}

ret_t dialog_highlighter_destroy(dialog_highlighter_t* h) {
  return_value_if_fail(h != NULL && h->vt != NULL, RET_BAD_PARAMS);

  emitter_dispatch_simple_event(EMITTER(h), EVT_DESTROY);
  dialog_highlighter_on_destroy(h);
  dialog_highlighter_clear_image(h);

  emitter_deinit(EMITTER(h));
  memset(h, 0x00, h->vt->size);
  TKMEM_FREE(h);

  return RET_OK;
}
