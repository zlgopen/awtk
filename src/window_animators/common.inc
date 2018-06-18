#include "base/utils.h"

static window_animator_t* window_animator_create(const char* type, bool_t open) {
  window_animator_t* wa = NULL;
  if (type == NULL || *type == '\0') {
    return NULL;
  } else if (tk_str_eq(type, WINDOW_ANIMATOR_CENTER_SCALE)) {
    wa = window_animator_create_scale(open);
  } else if (tk_str_eq(type, WINDOW_ANIMATOR_HTRANSLATE)) {
    wa = window_animator_create_htranslate(open);
  } else if (tk_str_eq(type, WINDOW_ANIMATOR_VTRANSLATE)) {
    wa = window_animator_create_vtranslate(open);
  } else if (tk_str_eq(type, WINDOW_ANIMATOR_BOTTOM_TO_TOP)) {
    wa = window_animator_create_bottom_to_top(open);
  } else if (tk_str_eq(type, WINDOW_ANIMATOR_TOP_TO_BOTTOM)) {
    wa = window_animator_create_top_to_bottom(open);
  } else if (tk_str_eq(type, WINDOW_ANIMATOR_FADE)) {
    wa = window_animator_create_fade(open);
  }

  return wa;
}

window_animator_t* window_animator_create_for_open(const char* type, canvas_t* c,
                                                   widget_t* prev_win, widget_t* curr_win) {
  window_animator_t* wa = window_animator_create(type, TRUE);
  return_value_if_fail(wa != NULL, NULL);

  window_animator_prepare(wa, c, prev_win, curr_win, TRUE);

  return wa;
}

window_animator_t* window_animator_create_for_close(const char* type, canvas_t* c,
                                                    widget_t* prev_win, widget_t* curr_win) {
  window_animator_t* wa = window_animator_create(type, FALSE);
  return_value_if_fail(wa != NULL, NULL);

  window_animator_prepare(wa, c, prev_win, curr_win, FALSE);

  return wa;
}
