
static window_animator_t* window_animator_create(window_animator_type_t type, bool_t open) {
  window_animator_t* wa = NULL;

  if (type == WINDOW_ANIMATOR_CENTER_SCALE) {
    wa = window_animator_create_scale(open);
  } else if (type == WINDOW_ANIMATOR_HTRANSLATE) {
    wa = window_animator_create_htranslate(open);
  } else if (type == WINDOW_ANIMATOR_VTRANSLATE) {
    wa = window_animator_create_vtranslate(open);
  } else if (type == WINDOW_ANIMATOR_BOTTOM_TO_TOP) {
    wa = window_animator_create_bottom_to_top(open);
  } else if (type == WINDOW_ANIMATOR_TOP_TO_BOTTOM) {
    wa = window_animator_create_top_to_bottom(open);
  } else if (type == WINDOW_ANIMATOR_FADE) {
    wa = window_animator_create_fade(open);
  }

  return wa;
}

window_animator_t* window_animator_create_for_open(window_animator_type_t type, canvas_t* c,
                                                   widget_t* prev_win, widget_t* curr_win) {
  window_animator_t* wa = window_animator_create(type, TRUE);
  return_value_if_fail(wa != NULL, NULL);

  window_animator_prepare(wa, c, prev_win, curr_win, TRUE);

  return wa;
}

window_animator_t* window_animator_create_for_close(window_animator_type_t type, canvas_t* c,
                                                    widget_t* prev_win, widget_t* curr_win) {
  window_animator_t* wa = window_animator_create(type, FALSE);
  return_value_if_fail(wa != NULL, NULL);

  window_animator_prepare(wa, c, prev_win, curr_win, FALSE);

  return wa;
}
