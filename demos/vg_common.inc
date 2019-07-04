
static inline ret_t on_paint_rect(void* ctx, event_t* e) {
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  canvas_set_fill_color(c, color_init(0, 0, 0xff, 0));
  canvas_fill_rect(c, 10, 10, 100, 100);

  return RET_OK;
}

static inline ret_t on_paint_patch9(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;

  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, 200, 30);
  canvas_draw_image_patch9(c, &img, &r);

  r = rect_init(10, 70, 200, 60);
  canvas_draw_image_patch9(c, &img, &r);

  r = rect_init(10, 150, 20, 60);
  canvas_draw_image_patch9(c, &img, &r);

  return RET_OK;
}

static inline ret_t on_paint_default(void* ctx, event_t* e) {
  rect_t s;
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "earth", &img);

  s = rect_init(0, 0, img.w, img.h);
  r = rect_init(10, 220, img.w / 2, img.h / 2);
  canvas_draw_image(c, &img, &s, &r);

  s = rect_init(0, 0, img.w, img.h);
  r = rect_init(60, 220, img.w, img.h);
  canvas_draw_image(c, &img, &s, &r);

  s = rect_init(0, 0, img.w, img.h);
  r = rect_init(160, 220, img.w * 2, img.h * 2);
  canvas_draw_image(c, &img, &s, &r);

  return RET_OK;
}

static inline ret_t on_paint_line(void* ctx, event_t* e) {
  color_t fill_color = color_init(0xe0, 0xe0, 0xe0, 0xff);
  color_t stroke_color = color_init(0x80, 0x80, 0x80, 0xff);
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  canvas_set_fill_color(c, fill_color);
  canvas_set_stroke_color(c, stroke_color);

  canvas_set_stroke_color(c, stroke_color);
  canvas_draw_line(c, 10, 10, 100, 300);

  return RET_OK;
}

static inline ret_t on_paint_points(void* ctx, event_t* e) {
  point_t points[] = {{100, 100}, {100, 101}, {100, 102}, {100, 103}};
  color_t fill_color = color_init(0xe0, 0xe0, 0xe0, 0xff);
  color_t stroke_color = color_init(0x80, 0x80, 0x80, 0xff);
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  canvas_set_fill_color(c, fill_color);

  canvas_set_stroke_color(c, stroke_color);
  canvas_draw_points(c, points, ARRAY_SIZE(points));

  return RET_OK;
}

static inline ret_t on_paint_patch3_h(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, 100, 60);
  canvas_draw_image_patch3_x(c, &img, &r);

  r = rect_init(10, 80, 10, 60);
  canvas_draw_image_patch3_x(c, &img, &r);

  r = rect_init(10, 160, 300, 50);
  canvas_draw_image_patch3_x(c, &img, &r);

  r = rect_init(10, 220, 300, 20);
  canvas_draw_image_patch3_x(c, &img, &r);

  return RET_OK;
}

static inline ret_t on_paint_patch3_v(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, 60, 100);
  canvas_draw_image_patch3_x(c, &img, &r);

  r = rect_init(80, 10, 60, 10);
  canvas_draw_image_patch3_x(c, &img, &r);

  r = rect_init(160, 10, 50, 300);
  canvas_draw_image_patch3_x(c, &img, &r);

  r = rect_init(220, 10, 20, 300);
  canvas_draw_image_patch3_x(c, &img, &r);

  return RET_OK;
}

static inline ret_t on_paint_repeat_x(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, 10, img.h);
  canvas_draw_image_repeat_x(c, &img, &r);

  r = rect_init(10, 60, img.w, img.h);
  canvas_draw_image_repeat_x(c, &img, &r);

  r = rect_init(10, 160, img.w * 2, img.h * 2);
  canvas_draw_image_repeat_x(c, &img, &r);

  return RET_OK;
}

static inline ret_t on_paint_repeat_y(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, img.w, 10);
  canvas_draw_image_repeat_y(c, &img, &r);

  r = rect_init(90, 10, img.w, img.h);
  canvas_draw_image_repeat_y(c, &img, &r);

  r = rect_init(160, 10, img.w * 2, img.h * 2);
  canvas_draw_image_repeat_y(c, &img, &r);

  return RET_OK;
}

static inline ret_t on_paint_repeat(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, img.w / 2, img.h / 2);
  canvas_draw_image_repeat(c, &img, &r);

  r = rect_init(50, 50, img.w, img.h);
  canvas_draw_image_repeat(c, &img, &r);

  r = rect_init(100, 100, img.w * 2 + 10, img.h * 2 + 10);
  canvas_draw_image_repeat(c, &img, &r);

  return RET_OK;
}

static inline ret_t on_paint_scale_x(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, img.w, img.h / 2);
  canvas_draw_image_scale_w(c, &img, &r);

  r = rect_init(50, 50, img.w, img.h);
  canvas_draw_image_scale_w(c, &img, &r);

  r = rect_init(100, 100, img.w * 2, img.h);
  canvas_draw_image_scale_w(c, &img, &r);

  return RET_OK;
}

static inline ret_t on_paint_scale_y(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, img.w / 2, img.h);
  canvas_draw_image_scale_h(c, &img, &r);

  r = rect_init(50, 50, img.w, img.h);
  canvas_draw_image_scale_h(c, &img, &r);

  r = rect_init(100, 100, img.w, img.h * 2);
  canvas_draw_image_scale_h(c, &img, &r);

  return RET_OK;
}

static inline ret_t on_paint_scale(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, img.w / 2, img.h);
  canvas_draw_image_scale(c, &img, &r);

  r = rect_init(50, 50, img.w, img.h);
  canvas_draw_image_scale(c, &img, &r);

  r = rect_init(100, 100, img.w, img.h * 2);
  canvas_draw_image_scale(c, &img, &r);

  return RET_OK;
}

static inline ret_t on_paint_center(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, img.w / 2, img.h);
  canvas_draw_image_center(c, &img, &r);

  r = rect_init(50, 50, img.w, img.h);
  canvas_draw_image_center(c, &img, &r);

  r = rect_init(100, 100, img.w, img.h * 2);
  canvas_draw_image_center(c, &img, &r);

  return RET_OK;
}

static void draw_basic_shapes(vgcanvas_t* vg, bool_t stroke) {
  vgcanvas_save(vg);

  vgcanvas_translate(vg, 5, 5);
  vgcanvas_rounded_rect(vg, 0, 0, 60, 40, 5);
  if (stroke)
    vgcanvas_stroke(vg);
  else
    vgcanvas_fill(vg);

  vgcanvas_translate(vg, 65, 0);
  vgcanvas_rounded_rect(vg, 0, 0, 60, 40, 1);
  if (stroke)
    vgcanvas_stroke(vg);
  else
    vgcanvas_fill(vg);

  vgcanvas_translate(vg, 65, 0);
  vgcanvas_ellipse(vg, 30, 20, 30, 20);
  if (stroke)
    vgcanvas_stroke(vg);
  else
    vgcanvas_fill(vg);

  vgcanvas_begin_path(vg);
  vgcanvas_translate(vg, 65, 0);
  vgcanvas_arc(vg, 20, 20, 20, 0, 2 * 3.15, FALSE);
  if (stroke)
    vgcanvas_stroke(vg);
  else
    vgcanvas_fill(vg);

  vgcanvas_begin_path(vg);
  vgcanvas_translate(vg, 50, 0);
  vgcanvas_move_to(vg, 0, 0);
  vgcanvas_line_to(vg, 40, 0);
  vgcanvas_line_to(vg, 40, 40);
  vgcanvas_close_path(vg);
  if (stroke)
    vgcanvas_stroke(vg);
  else
    vgcanvas_fill(vg);

  vgcanvas_begin_path(vg);
  vgcanvas_restore(vg);
}

static void stroke_lines(vgcanvas_t* vg) {
  vgcanvas_save(vg);
  vgcanvas_set_stroke_color(vg, color_init(0, 0, 0xff, 0xff));
  vgcanvas_move_to(vg, 0, 0);
  vgcanvas_line_to(vg, 40, 40);

  vgcanvas_translate(vg, 40, 0);
  vgcanvas_move_to(vg, 0, 0);
  vgcanvas_quad_to(vg, 40, 0, 40, 40);

  vgcanvas_translate(vg, 40, 0);
  vgcanvas_move_to(vg, 0, 0);
  vgcanvas_bezier_to(vg, 20, 0, 20, 40, 40, 40);

  vgcanvas_stroke(vg);

  vgcanvas_translate(vg, 40, 0);
  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, 20, 20, 20, 0, 3.14, TRUE);
  vgcanvas_stroke(vg);

  vgcanvas_translate(vg, 40, 0);
  vgcanvas_set_line_width(vg, 2);
  vgcanvas_set_line_cap(vg, "round");
  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, 20, 20, 20, 0, 3.14 / 2, FALSE);
  vgcanvas_stroke(vg);

  vgcanvas_stroke(vg);
  vgcanvas_restore(vg);
}

static void draw_image(vgcanvas_t* vg) {
  bitmap_t img;

  vgcanvas_save(vg);

  image_manager_get_bitmap(image_manager(), "bricks", &img);

  vgcanvas_translate(vg, (vg->w - img.w) / 2, 0);

  vgcanvas_translate(vg, img.w >> 1, img.h >> 1);
  vgcanvas_rotate(vg, 3.14 / 4);
  vgcanvas_translate(vg, -img.w >> 1, -img.h >> 1);

  vgcanvas_scale(vg, 1.5, 1.5);
  vgcanvas_draw_image(vg, &img, 0, 0, img.w, img.h, 0, 0, img.w, img.h);
  vgcanvas_restore(vg);

  return;
}

static void draw_matrix(vgcanvas_t* vg) {
  float_t w = 50;
  float_t h = 50;

  vgcanvas_save(vg);
  vgcanvas_translate(vg, w / 2, h / 2);
  vgcanvas_rotate(vg, 3.14 / 4);
  vgcanvas_translate(vg, -w / 2, -h / 2);

  vgcanvas_rect(vg, 0, 0, w, h);
  vgcanvas_fill(vg);
  vgcanvas_restore(vg);
}

static void draw_text(vgcanvas_t* vg) {
  float_t h = 20;
  const char* text = "Hello AWTK";
  float_t w = 100;

  vgcanvas_set_font_size(vg, 20);
  vgcanvas_set_font(vg, NULL);
  w = vgcanvas_measure_text(vg, text);

  vgcanvas_save(vg);
  vgcanvas_translate(vg, w / 2, h / 2);
  vgcanvas_rotate(vg, 3.14 / 4);
  vgcanvas_translate(vg, -w / 2, -h / 2);

  vgcanvas_fill_text(vg, text, 10, 10, 100);
  log_debug("text=%s w=%f\n", text, w);
  vgcanvas_restore(vg);
}

static inline ret_t on_paint_text(void* ctx, event_t* e) {
  int32_t x = 0;
  int32_t y = 0;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  color_t color = color_init(0xff, 0, 0, 0xff);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);

  color = color_init(0xff, 0, 0, 0xff);
  canvas_set_stroke_color(c, color);
  canvas_stroke_rect(c, x, y, 100, 30);

  color = color_init(0xff, 0, 0, 0xff);
  canvas_set_text_color(c, color);
  canvas_set_font(c, NULL, 30);

  vgcanvas_set_fill_color(vg, color);
  vgcanvas_fill_text(vg, "ABC", 0, 0, 100);
  /*
    vgcanvas_translate(vg, x, y);
    canvas_draw_text(c, L"abc", 3, 0, 0);
  */
  return RET_OK;
}

static inline ret_t on_paint_vg(void* ctx, event_t* e) {
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);

  vgcanvas_save(vg);
  vgcanvas_set_line_width(vg, 1);
  vgcanvas_set_stroke_color(vg, color_init(0, 0xff, 0, 0xff));
  vgcanvas_set_fill_color(vg, color_init(0xff, 0, 0, 0xff));

  draw_basic_shapes(vg, FALSE);
  vgcanvas_translate(vg, 0, 50);
  draw_basic_shapes(vg, TRUE);
  vgcanvas_translate(vg, 0, 50);
  stroke_lines(vg);
  vgcanvas_translate(vg, 0, 50);
  draw_image(vg);

  vgcanvas_translate(vg, 50, 100);
  draw_matrix(vg);
  vgcanvas_translate(vg, 0, 100);

  draw_text(vg);
  vgcanvas_restore(vg);

  return RET_OK;
}

static inline ret_t on_paint_global_alpha(void* ctx, event_t* e) {
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  color_t fill = color_init(0x00, 0xff, 0x00, 0xff);
  color_t stroke = color_init(0x00, 0x00, 0xff, 0xff);

  vgcanvas_save(vg);

  image_manager_get_bitmap(image_manager(), "bricks", &img);
  vgcanvas_set_global_alpha(vg, 1);
  vgcanvas_draw_image(vg, &img, 0, 0, img.w, img.h, 0, 0, img.w, img.h);

  vgcanvas_set_global_alpha(vg, 0.5);

  vgcanvas_set_fill_color(vg, fill);
  vgcanvas_rect(vg, 0, 0, 20, 20);
  vgcanvas_fill(vg);

  vgcanvas_set_stroke_color(vg, stroke);
  vgcanvas_set_line_width(vg, 5);
  vgcanvas_rect(vg, 20, 20, 20, 20);
  vgcanvas_stroke(vg);

  vgcanvas_translate(vg, 15, 15);
  vgcanvas_draw_image(vg, &img, 0, 0, img.w, img.h, 0, 0, img.w, img.h);

  vgcanvas_restore(vg);

  return RET_OK;
}

static inline ret_t on_paint_vg_simple(void* ctx, event_t* e) {
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  image_manager_get_bitmap(image_manager(), "rgba", &img);

  canvas_set_fill_color(c, color_init(0x11, 0x22, 0x33, 0xff));
  canvas_fill_rect(c, 0, 0, 10, 10);

  rect_t s = rect_init(0, 0, img.w, img.h);
  rect_t r = rect_init(10, 10, img.w, img.h);
  canvas_draw_image(c, &img, &s, &r);

  vgcanvas_save(vg);
  vgcanvas_translate(vg, 0, 100);

  vgcanvas_set_line_width(vg, 1);
  vgcanvas_set_fill_color(vg, color_init(0xff, 0, 0, 0xff));
  vgcanvas_rect(vg, 5, 5, 100, 100);
  vgcanvas_fill(vg);

  vgcanvas_set_fill_color(vg, color_init(0, 0xff, 0, 0xff));
  vgcanvas_rect(vg, 110, 5, 100, 100);
  vgcanvas_fill(vg);

  vgcanvas_set_fill_color(vg, color_init(0, 0, 0xff, 0xff));
  vgcanvas_rect(vg, 215, 5, 100, 100);
  vgcanvas_fill(vg);

  vgcanvas_translate(vg, 0, 105);
  vgcanvas_set_stroke_color(vg, color_init(0xff, 0, 0, 0xff));
  vgcanvas_rect(vg, 5, 5, 100, 100);
  vgcanvas_stroke(vg);

  vgcanvas_set_stroke_color(vg, color_init(0, 0xff, 0, 0xff));
  vgcanvas_rect(vg, 110, 5, 100, 100);
  vgcanvas_stroke(vg);

  vgcanvas_set_stroke_color(vg, color_init(0, 0, 0xff, 0xff));
  vgcanvas_rect(vg, 215, 5, 100, 100);
  vgcanvas_stroke(vg);

  vgcanvas_translate(vg, 0, 128);
  image_manager_get_bitmap(image_manager(), "rgb", &img);
  vgcanvas_draw_image(vg, &img, 0, 0, img.w, img.h, 0, 0, img.w, img.h);

  image_manager_get_bitmap(image_manager(), "rgba", &img);
  vgcanvas_draw_image(vg, &img, 0, 0, img.w, img.h, 100, 0, img.w, img.h);

  image_manager_get_bitmap(image_manager(), "switch", &img);
  vgcanvas_draw_image(vg, &img, 0, 0, img.w, img.h, 200, 0, img.w, img.h);

  vgcanvas_translate(vg, 0, 60);
  vgcanvas_rounded_rect(vg, 0, 0, img.w, img.h, 5);
  vgcanvas_paint(vg, FALSE, &img);

  vg = canvas_get_vgcanvas(c);
  vgcanvas_translate(vg, 160, 0);
  vgcanvas_rounded_rect(vg, 0, 0, img.w, img.h, 5);
  vgcanvas_paint(vg, FALSE, &img);

  vgcanvas_restore(vg);

  return RET_OK;
}

static inline ret_t on_timer(const timer_info_t* timer) {
  widget_t* widget = WIDGET(timer->ctx);

  widget_invalidate(widget, NULL);

  return RET_REPEAT;
}
