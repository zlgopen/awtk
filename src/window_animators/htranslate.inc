
static ret_t window_animator_open_htranslate_update_percent(window_animator_t* wa) {
  float_t percent = 0.2f + 0.8f * wa->easing(wa->time_percent);
  if (wa->open) {
    wa->percent = percent;
  } else {
    wa->percent = 1.0f - percent;
  }

  return RET_OK;
}

static ret_t window_animator_open_htranslate_draw_prev(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  float_t ratio = wa->ratio;
  widget_t* win = wa->prev_win;
  float_t percent = wa->percent;
  float_t x = win->w * percent;
  float_t w = win->w * (1 - percent);

#ifdef WITH_NANOVG_GPU
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  vgcanvas_draw_image(vg, &(wa->prev_img), x * ratio, win->y * ratio, w * ratio, win->h * ratio, 0,
                      win->y, w, win->h);
#else
  rect_t src;
  rect_t dst;
  src = rect_init(x * ratio, win->y * ratio, w * ratio, win->h * ratio);
  dst = rect_init(0, win->y, w, win->h);
  lcd_draw_image(c->lcd, &(wa->prev_img), &src, &dst);
#endif

  return RET_OK;
}

static ret_t window_animator_open_htranslate_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  float_t ratio = wa->ratio;
  widget_t* win = wa->curr_win;
  float_t percent = wa->percent;
  float_t x = win->w * (1 - percent);
  float_t w = win->w * percent;

#ifdef WITH_NANOVG_GPU
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  vgcanvas_draw_image(vg, &(wa->curr_img), 0, win->y * ratio, w * ratio, win->h * ratio, x, win->y,
                      w, win->h);
#else
  rect_t src;
  rect_t dst;
  src = rect_init(0, win->y * ratio, w * ratio, win->h * ratio);
  dst = rect_init(x, win->y, w, win->h);
  lcd_draw_image(c->lcd, &(wa->curr_img), &src, &dst);
#endif

  return RET_OK;
}

static window_animator_t* window_animator_create_htranslate(bool_t open) {
  window_animator_t* wa = TKMEM_ZALLOC(window_animator_t);
  return_value_if_fail(wa != NULL, NULL);

  if (open) {
    wa->easing = easing_get(EASING_CUBIC_OUT);
    wa->destroy = window_animator_open_destroy;
  } else {
    wa->easing = easing_get(EASING_CUBIC_OUT);
    wa->destroy = window_animator_close_destroy;
  }

  wa->update_percent = window_animator_open_htranslate_update_percent;
  wa->draw_prev_window = window_animator_open_htranslate_draw_prev;
  wa->draw_curr_window = window_animator_open_htranslate_draw_curr;
  wa->begin_frame = window_animator_begin_frame;

  return wa;
}
