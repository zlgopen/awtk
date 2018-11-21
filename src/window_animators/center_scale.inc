
static ret_t window_animator_open_scale_update_percent(window_animator_t* wa) {
  if (wa->open) {
    wa->percent = 0.9f + 0.1f * wa->easing(wa->time_percent);
  } else {
    wa->percent = 1.0f - 0.1f * wa->easing(wa->time_percent);
  }

  return RET_OK;
}

static ret_t window_animator_open_scale_draw_prev(window_animator_t* wa) {
  rect_t src;
  rect_t dst;
  canvas_t* c = wa->canvas;
  float_t ratio = wa->ratio;
  widget_t* win = wa->prev_win;

  src = rect_init(win->x * ratio, win->y * ratio, win->w * ratio, win->h * ratio);
  dst = rect_init(win->x, win->y, win->w, win->h);
#ifndef WITH_NANOVG_GPU
  if (wa->percent > 0 && !lcd_is_swappable(c->lcd)) {
    win = wa->curr_win;
    src = rect_init(win->x * ratio, win->y * ratio, win->w * ratio, win->h * ratio);
    dst = rect_init(win->x, win->y, win->w, win->h);
  }
#endif /*WITH_NANOVG_GPU*/
  lcd_draw_image(c->lcd, &(wa->prev_img), &src, &dst);

  return RET_OK;
}

static ret_t window_animator_open_scale_draw_curr(window_animator_t* wa) {
  rect_t src;
  rect_t dst;
  canvas_t* c = wa->canvas;
  float_t ratio = wa->ratio;
  widget_t* win = wa->curr_win;
  float_t scale = wa->percent;
  float_t alpha = wa->open ? wa->time_percent : 1 - wa->time_percent;
  src = rect_init(win->x * ratio, win->y * ratio, win->w * ratio, win->h * ratio);

#ifdef WITH_NANOVG_GPU
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  dst = rect_init(0, 0, win->w, win->h);
  vgcanvas_save(vg);
  vgcanvas_translate(vg, win->x, win->y);
  vgcanvas_translate(vg, win->w >> 1, win->h >> 1);
  vgcanvas_scale(vg, scale, scale);
  vgcanvas_translate(vg, -win->w >> 1, -win->h >> 1);
  vgcanvas_set_global_alpha(vg, alpha);
  vgcanvas_draw_image(vg, &(wa->curr_img), src.x, src.y, src.w, src.h, dst.x, dst.y, dst.w, dst.h);
  vgcanvas_restore(vg);
#else
  (void)alpha;
  dst = rect_init(0, 0, win->w * scale, win->h * scale);
  dst.x = win->x + ((win->w - dst.w) >> 1);
  dst.y = win->y + ((win->h - dst.h) >> 1);
  if (alpha > 0) {
    lcd_set_global_alpha(c->lcd, alpha * 0xff);
    lcd_draw_image(c->lcd, &(wa->curr_img), &src, &dst);
  }
#endif /*WITH_NANOVG_GPU*/

  return RET_OK;
}

static window_animator_t* window_animator_create_scale(bool_t open) {
  window_animator_t* wa = TKMEM_ZALLOC(window_animator_t);
  return_value_if_fail(wa != NULL, NULL);

  if (open) {
    wa->easing = easing_get(EASING_QUADRATIC_IN);
    wa->destroy = window_animator_open_destroy;
  } else {
    wa->easing = easing_get(EASING_QUADRATIC_IN);
    wa->destroy = window_animator_close_destroy;
  }

  wa->update_percent = window_animator_open_scale_update_percent;
  wa->draw_prev_window = window_animator_open_scale_draw_prev;
  wa->draw_curr_window = window_animator_open_scale_draw_curr;
  wa->begin_frame = window_animator_begin_frame_overlap;

  return wa;
}
