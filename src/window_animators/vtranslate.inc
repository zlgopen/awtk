
static ret_t window_animator_open_vtranslate_update_percent(window_animator_t* wa) {
  float_t percent = 0.2f + 0.8f * wa->easing(wa->time_percent);
  if (wa->open) {
    wa->percent = percent;
  } else {
    wa->percent = 1.0f - percent;
  }

  return RET_OK;
}

static ret_t window_animator_open_vtranslate_draw_prev(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  float_t ratio = wa->ratio;
  widget_t* win = wa->prev_win;
  float_t percent = wa->percent;
  float_t y = win->h * percent;
  float_t h = win->h * (1 - percent);

#ifdef WITH_NANOVG
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  vgcanvas_draw_image(vg, &(wa->prev_img), win->x * ratio, y * ratio, win->w * ratio, h * ratio,
                      win->x, 0, win->w, h);
#else
  rect_t src;
  rect_t dst;
  rect_init(src, win->x * ratio, y * ratio, win->w * ratio, h * ratio);
  rect_init(dst, win->x, 0, win->w, h);
  lcd_draw_image(c->lcd, &(wa->prev_img), &src, &dst);
#endif

  return RET_OK;
}

static ret_t window_animator_open_vtranslate_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  float_t ratio = wa->ratio;
  widget_t* win = wa->curr_win;
  float_t percent = wa->percent;
  float_t y = win->h * (1 - percent);
  float_t h = win->h * percent;

#ifdef WITH_NANOVG
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  vgcanvas_draw_image(vg, &(wa->curr_img), win->x, 0, win->w * ratio, h * ratio, win->x, y, win->w,
                      h);
#else
  rect_t src;
  rect_t dst;
  rect_init(src, win->x, 0, win->w * ratio, h * ratio);
  rect_init(dst, win->x, y, win->w, h);
  lcd_draw_image(c->lcd, &(wa->curr_img), &src, &dst);
#endif

  return RET_OK;
}

static window_animator_t* window_animator_create_vtranslate(bool_t open) {
  window_animator_t* wa = TKMEM_ZALLOC(window_animator_t);
  return_value_if_fail(wa != NULL, NULL);

  if (open) {
    wa->easing = easing_get(EASING_QUADRATIC_IN);
    wa->destroy = window_animator_open_destroy;
  } else {
    wa->easing = easing_get(EASING_QUADRATIC_IN);
    wa->destroy = window_animator_close_destroy;
  }

  wa->update_percent = window_animator_open_vtranslate_update_percent;
  wa->draw_prev_window = window_animator_open_vtranslate_draw_prev;
  wa->draw_curr_window = window_animator_open_vtranslate_draw_curr;

  return wa;
}
