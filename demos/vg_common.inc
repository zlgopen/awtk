
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
  vgcanvas_set_font(vg, TK_DEFAULT_FONT);
  w = vgcanvas_measure_text(vg, text);

  vgcanvas_save(vg);
  vgcanvas_translate(vg, w / 2, h / 2);
  vgcanvas_rotate(vg, 3.14 / 4);
  vgcanvas_translate(vg, -w / 2, -h / 2);

  vgcanvas_fill_text(vg, text, 10, 10, 100);
  log_debug("text=%s w=%f\n", text, w);
  vgcanvas_restore(vg);
}
