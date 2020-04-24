/**
 * File:   dialog.c
 * Author: AWTK Develop Team
 * Brief:  dialog
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/idle.h"
#include "base/enums.h"
#include "tkc/utils.h"
#include "base/layout.h"
#include "base/dialog.h"
#include "base/main_loop.h"
#include "base/image_manager.h"
#include "base/window_manager.h"
#include "layouters/self_layouter_default.h"

static ret_t dialog_on_add_child(widget_t* widget, widget_t* child) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(child->vt->type, WIDGET_TYPE_DIALOG_TITLE)) {
    dialog->title = child;
  } else if (tk_str_eq(child->vt->type, WIDGET_TYPE_DIALOG_CLIENT)) {
    dialog->client = child;
  }

  return RET_CONTINUE;
}

static const char* const s_dialog_properties[] = {WIDGET_PROP_ANIM_HINT,
                                                  WIDGET_PROP_OPEN_ANIM_HINT,
                                                  WIDGET_PROP_MOVE_FOCUS_PREV_KEY,
                                                  WIDGET_PROP_MOVE_FOCUS_NEXT_KEY,
                                                  WIDGET_PROP_CLOSE_ANIM_HINT,
                                                  WIDGET_PROP_THEME,
                                                  NULL};

static ret_t dialog_set_prop(widget_t* widget, const char* name, const value_t* v) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_HIGHLIGHT)) {
    dialog->highlight = tk_str_copy(dialog->highlight, value_str(v));

    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELF_LAYOUT)) {
    self_layouter_t* self_layout = widget->self_layout;
    if (self_layouter_default_is_valid(self_layout)) {
      if (self_layouter_get_param_int(self_layout, "x_attr", 0) == X_ATTR_UNDEF) {
        self_layouter_set_param_str(self_layout, "x", "c");
      }

      if (self_layouter_get_param_int(self_layout, "y_attr", 0) == Y_ATTR_UNDEF) {
        self_layouter_set_param_str(self_layout, "y", "m");
      }
    }
    return RET_OK;
  }

  return window_base_set_prop(widget, name, v);
}

static ret_t dialog_get_prop(widget_t* widget, const char* name, value_t* v) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_HIGHLIGHT)) {
    value_set_str(v, dialog->highlight);

    return RET_OK;
  }

  return window_base_get_prop(widget, name, v);
}

static ret_t dialog_on_destroy(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(dialog->highlight);

  return window_base_on_destroy(widget);
}

TK_DECL_VTABLE(dialog) = {.size = sizeof(dialog_t),
                          .type = WIDGET_TYPE_DIALOG,
                          .is_window = TRUE,
                          .clone_properties = s_dialog_properties,
                          .persistent_properties = s_dialog_properties,
                          .parent = TK_PARENT_VTABLE(window_base),
                          .create = dialog_create,
                          .on_add_child = dialog_on_add_child,
                          .on_event = window_base_on_event,
                          .on_paint_self = window_base_on_paint_self,
                          .on_paint_begin = window_base_on_paint_begin,
                          .on_paint_end = window_base_on_paint_end,
                          .set_prop = dialog_set_prop,
                          .get_prop = dialog_get_prop,
                          .on_destroy = dialog_on_destroy};

widget_t* dialog_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return window_base_create(parent, TK_REF_VTABLE(dialog), x, y, w, h);
}

ret_t dialog_set_title(widget_t* widget, const char* title) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL && title != NULL, RET_BAD_PARAMS);

  return widget_set_text_utf8(dialog->title, title);
}

static ret_t dialog_close(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_close_window(widget->parent, widget);
}

static ret_t dialog_idle_close(const idle_info_t* info) {
  return dialog_close(WIDGET(info->ctx));
}

widget_t* dialog_get_title(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, NULL);

  return dialog->title;
}

widget_t* dialog_get_client(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, NULL);

  return dialog->client;
}

dialog_quit_code_t dialog_modal(widget_t* widget) {
#ifdef AWTK_WEB
  log_debug("awtk web not support dialog_modal\n");
  return DIALOG_QUIT_NONE;
#else
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, DIALOG_QUIT_NONE);

  log_debug("%s run\n", __FUNCTION__);

  dialog->quited = FALSE;
  dialog->is_model = TRUE;
  widget_invalidate(widget, NULL);
  main_loop_run(main_loop());

  log_debug("%s quit\n", __FUNCTION__);
  idle_add(dialog_idle_close, widget);

  return (dialog_quit_code_t)(dialog->quit_code);
#endif /*AWTK_WEB*/
}

static bool_t is_dialog_opened(widget_t* widget) {
  int32_t stage = widget_get_prop_int(widget, WIDGET_PROP_STAGE, WINDOW_STAGE_NONE);

  if (stage == WINDOW_STAGE_OPENED) return TRUE;
  if (stage != WINDOW_STAGE_SUSPEND || widget->parent == NULL) return FALSE;

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget->parent, iter, i)
  if (iter == widget) break;
  if (!tk_str_eq(iter->vt->type, WIDGET_TYPE_DIALOG)) continue;
  dialog_t* dialog = DIALOG(iter);
  if (dialog == NULL || !dialog->quited) return FALSE;
  WIDGET_FOR_EACH_CHILD_END()
  return TRUE;
}

ret_t dialog_quit(widget_t* widget, uint32_t code) {
#ifdef AWTK_WEB
  log_debug("awtk web not support dialog_modal\n");
  dialog_close(widget);
#else
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL && !(dialog->quited), RET_BAD_PARAMS);
  return_value_if_fail(is_dialog_opened(widget), RET_BAD_PARAMS);

  dialog->quited = TRUE;
  dialog->quit_code = (dialog_quit_code_t)code;
  main_loop_quit(main_loop());
#endif /*AWTK_WEB*/

  return RET_OK;
}

bool_t dialog_is_quited(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, FALSE);

  return dialog->quited;
}

bool_t dialog_is_modal(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, FALSE);

  return dialog->is_model;
}

widget_t* dialog_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, dialog), NULL);

  return widget;
}
