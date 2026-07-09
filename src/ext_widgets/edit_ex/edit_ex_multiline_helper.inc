/**
 * File:   edit_ex_multiline_helper.inc
 * Author: AWTK Develop Team
 * Brief:  edit_ex¶àÐÐ±à¼­°ïÖúº¯Êý
 *
 * Copyright (c) 2026 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2026-07-07 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_EDIT_EX_MULTILINE_HELPER_INC
#define TK_EDIT_EX_MULTILINE_HELPER_INC

#include "tkc/types_def.h"

BEGIN_C_DECLS

#include "base/window.h"
#include "widgets/popup.h"
#include "base/window_manager.h"
#include "ext_widgets/edit_ex/edit_ex.h"
#include "ext_widgets/mledit/mledit.h"

static ret_t edit_ex_close_multiline_popup(edit_ex_t* edit_ex);

static ret_t edit_ex_close_multiline_popup_on_screen_resize(void* ctx, event_t* e) {
  edit_ex_t* edit_ex = EDIT_EX(ctx);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  edit_ex_close_multiline_popup(edit_ex);

  return RET_OK;
}

inline static ret_t edit_ex_deinit_multiline_popup(edit_ex_t* edit_ex) {
  widget_t* mledit = widget_lookup_by_type(edit_ex->multiline_popup, WIDGET_TYPE_MLEDIT, TRUE);
  widget_set_text(WIDGET(edit_ex), widget_get_text(mledit));
  widget_off_by_ctx(edit_ex->multiline_popup, edit_ex);
  widget_off_by_func(window_manager(), EVT_RESIZE, edit_ex_close_multiline_popup_on_screen_resize,
                     edit_ex);
  widget_off_by_func(window_manager(), EVT_MOVE_RESIZE,
                     edit_ex_close_multiline_popup_on_screen_resize, edit_ex);
  edit_ex->multiline_popup = NULL;
  return RET_OK;
}

inline static ret_t edit_ex_close_multiline_popup(edit_ex_t* edit_ex) {
  if (edit_ex->multiline_popup != NULL) {
    window_close(edit_ex->multiline_popup);
    return edit_ex_deinit_multiline_popup(edit_ex);
  }
  return RET_OK;
}

static ret_t edit_ex_multiline_popup_on_close(void* ctx, event_t* e) {
  edit_ex_t* edit_ex = EDIT_EX(ctx);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  edit_ex_deinit_multiline_popup(edit_ex);

  return RET_OK;
}

inline static void edit_ex_open_multiline_popup_prepare_props(edit_ex_t* edit_ex) {
  widget_t* widget = WIDGET(edit_ex);
  const char* theme = NULL;
  const char* applet_name = NULL;
  widget_t* window_for_edit_ex = NULL;

  window_for_edit_ex = widget_get_window(widget);

  applet_name = widget_get_prop_str(window_for_edit_ex, WIDGET_PROP_APPLET_NAME, NULL);
  widget_set_prop_str(edit_ex->multiline_popup, WIDGET_PROP_APPLET_NAME, applet_name);

  theme = widget_get_prop_str(window_for_edit_ex, WIDGET_PROP_THEME, NULL);
  widget_set_prop_str(edit_ex->multiline_popup, WIDGET_PROP_THEME, theme);

  widget_set_prop_bool(edit_ex->multiline_popup, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE, TRUE);
}

static ret_t edit_ex_open_multiline_popup(edit_ex_t* edit_ex) {
  point_t p = {0, 0};
  widget_t* widget = WIDGET(edit_ex);
  widget_t* mledit = NULL;

  if (!edit_ex->multiline) {
    return RET_OK;
  }
  if (edit_ex->multiline_popup != NULL) {
    return RET_OK;
  }

  widget_set_focused(widget, FALSE);

  edit_ex->multiline_popup = popup_create(NULL, 0, 0, widget->w, widget->h);
  return_value_if_fail(edit_ex->multiline_popup != NULL, RET_OOM);

  mledit = mledit_create(edit_ex->multiline_popup, 0, 0, 0, 0);
  return_value_if_fail(mledit != NULL, (edit_ex_close_multiline_popup(edit_ex), RET_OOM));

  popup_set_parent_widget_by_create(edit_ex->multiline_popup, widget);
  edit_ex_open_multiline_popup_prepare_props(edit_ex);

  widget_to_screen(widget, &p);
  widget_move(edit_ex->multiline_popup, p.x, p.y);
  widget_set_self_layout(edit_ex->multiline_popup, "flex(h=flex)");

  widget_use_style(mledit, widget_get_prop_str(widget, WIDGET_PROP_STYLE, "default"));
  widget_set_prop_bool(mledit, MLEDIT_PROP_AUTO_ADJUST_HEIGHT, TRUE);
  widget_set_self_layout(mledit, "default(w=100%)");
  widget_set_text(mledit, widget_get_text(widget));
  widget_set_focused(mledit, TRUE);
  text_edit_select_all(MLEDIT(mledit)->model);

  widget_on(edit_ex->multiline_popup, EVT_WINDOW_CLOSE, edit_ex_multiline_popup_on_close, widget);

  widget_on(window_manager(), EVT_RESIZE, edit_ex_close_multiline_popup_on_screen_resize, widget);
  widget_on(window_manager(), EVT_MOVE_RESIZE, edit_ex_close_multiline_popup_on_screen_resize,
            widget);

  return RET_OK;
}

END_C_DECLS

#endif /*TK_EDIT_EX_MULTILINE_HELPER_INC*/
