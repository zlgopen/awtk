
static ret_t window_animator_open_top_to_bottom_update_percent(window_animator_t* wa) {
  if (wa->open) {
    wa->percent = wa->easing(wa->time_percent);
  } else {
    wa->percent = 1.0f - wa->easing(wa->time_percent);
  }

  return RET_OK;
}

static ret_t window_animator_open_top_to_bottom_draw_prev(window_animator_t* wa) {
  rect_t src;
  rect_t dst;
  canvas_t* c = wa->canvas;
  float_t ratio = wa->ratio;
  widget_t* win = wa->prev_win;

  src = rect_init(win->x * ratio, win->y * ratio, win->w * ratio, win->h * ratio);
  dst = rect_init(win->x, win->y, win->w, win->h);
#ifndef WITH_NANOVG_GPU
  if (wa->percent > 0 && !lcd_is_swappable(c->lcd)) {
    wh_t y = wa->percent * wa->curr_win->h;
    wh_t h = wa->curr_win->h - y;

    if (h == 0) {
      return RET_OK;
    }

    win = wa->curr_win;
    src = rect_init(win->x * ratio, y * ratio, win->w * ratio, h * ratio);
    dst = rect_init(win->x, y, win->w, h);
  }
#endif

  lcd_draw_image(c->lcd, &(wa->prev_img), &src, &dst);

  return RET_OK;
}

static ret_t window_animator_open_top_to_bottom_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  float_t ratio = wa->ratio;
  widget_t* win = wa->curr_win;
  float_t percent = wa->percent;
  float_t alpha = percent;

#ifdef WITH_NANOVG_GPU
  float_t h = win->h * percent;
  float_t y = win->h - h;
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  vgcanvas_set_global_alpha(vg, alpha);
  vgcanvas_draw_image(vg, &(wa->curr_img), win->x * ratio, y * ratio, win->w * ratio, h * ratio,
                      win->x, 0, win->w, h);
#else
  rect_t src;
  rect_t dst;
  int32_t h = win->h * percent;
  int32_t y = win->h - h;
  (void)alpha;
  src = rect_init(win->x * ratio, y, win->w * ratio, h);
  dst = rect_init(win->x, 0, win->w, h);
  lcd_set_global_alpha(c->lcd, 0xff);
  lcd_draw_image(c->lcd, &(wa->curr_img), &src, &dst);
#endif

  return RET_OK;
}

static window_animator_t* window_animator_create_top_to_bottom(bool_t open) {
  window_animator_t* wa = TKMEM_ZALLOC(window_animator_t);
  return_value_if_fail(wa != NULL, NULL);

  if (open) {
    wa->easing = easing_get(EASING_CUBIC_OUT);
    wa->destroy = window_animator_open_destroy;
  } else {
    wa->easing = easing_get(EASING_CUBIC_IN);
    wa->destroy = window_animator_close_destroy;
  }

  wa->update_percent = window_animator_open_top_to_bottom_update_percent;
  wa->draw_prev_window = window_animator_open_top_to_bottom_draw_prev;
  wa->draw_curr_window = window_animator_open_top_to_bottom_draw_curr;
  wa->begin_frame = window_animator_begin_frame_overlap;

  return wa;
}
