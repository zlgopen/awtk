/*adapted from :
 * http://rosettacode.org/wiki/Xiaolin_Wu%27s_line_algorithm#C
 */

#define plot_(X, Y, D)                                            \
  p.x = (X);                                                      \
  p.y = (Y);                                                      \
  if (p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax) { \
    color->rgba.a = (D)*0xff;                                     \
    lcd_draw_points(lcd, &p, 1);                                  \
  }

#define ipart_(X) ((int)(X))
#define round_(X) ((int)(((double)(X)) + 0.5))
#define fpart_(X) (((double)(X)) - (double)ipart_(X))
#define rfpart_(X) (1.0 - fpart_(X))
#define fabs(xx) ((xx) < 0 ? -(xx) : (xx))

#define swap_(a, b)       \
  do {                    \
    unsigned int tmp = a; \
    a = b;                \
    b = tmp;              \
  } while (0)

void draw_line(canvas_t* c, int x1, int y1, int x2, int y2) {
  point_t p;
  lcd_t* lcd = c->lcd;
  xy_t xmin = c->clip_left;
  xy_t xmax = c->clip_right;
  xy_t ymin = c->clip_top;
  xy_t ymax = c->clip_bottom;
  color_t* color = &(c->lcd->stroke_color);
  double dx = (double)x2 - (double)x1;
  double dy = (double)y2 - (double)y1;

  if (fabs(dx) > fabs(dy)) {
    if (x2 < x1) {
      swap_(x1, x2);
      swap_(y1, y2);
    }
    double gradient = dy / dx;
    double xend = round_(x1);
    double yend = y1 + gradient * (xend - x1);
    double xgap = rfpart_(x1 + 0.5);
    int xpxl1 = xend;
    int ypxl1 = ipart_(yend);
    plot_(xpxl1, ypxl1, rfpart_(yend) * xgap);
    plot_(xpxl1, ypxl1 + 1, fpart_(yend) * xgap);
    double intery = yend + gradient;

    xend = round_(x2);
    yend = y2 + gradient * (xend - x2);
    xgap = fpart_(x2 + 0.5);
    int xpxl2 = xend;
    int ypxl2 = ipart_(yend);
    plot_(xpxl2, ypxl2, rfpart_(yend) * xgap);
    plot_(xpxl2, ypxl2 + 1, fpart_(yend) * xgap);

    int x;
    for (x = xpxl1 + 1; x <= (xpxl2 - 1); x++) {
      plot_(x, ipart_(intery), rfpart_(intery));
      plot_(x, ipart_(intery) + 1, fpart_(intery));
      intery += gradient;
    }
  } else {
    if (y2 < y1) {
      swap_(x1, x2);
      swap_(y1, y2);
    }
    double gradient = dx / dy;
    double yend = round_(y1);
    double xend = x1 + gradient * (yend - y1);
    double ygap = rfpart_(y1 + 0.5);
    int ypxl1 = yend;
    int xpxl1 = ipart_(xend);
    plot_(xpxl1, ypxl1, rfpart_(xend) * ygap);
    plot_(xpxl1, ypxl1 + 1, fpart_(xend) * ygap);
    double interx = xend + gradient;

    yend = round_(y2);
    xend = x2 + gradient * (yend - y2);
    ygap = fpart_(y2 + 0.5);
    int ypxl2 = yend;
    int xpxl2 = ipart_(xend);
    plot_(xpxl2, ypxl2, rfpart_(xend) * ygap);
    plot_(xpxl2, ypxl2 + 1, fpart_(xend) * ygap);

    int y;
    for (y = ypxl1 + 1; y <= (ypxl2 - 1); y++) {
      plot_(ipart_(interx), y, rfpart_(interx));
      plot_(ipart_(interx) + 1, y, fpart_(interx));
      interx += gradient;
    }
  }
}
#undef swap_
#undef plot_
#undef ipart_
#undef fpart_
#undef round_
#undef rfpart_
