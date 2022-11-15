#include "svg_color.h"

uint32_t svg_color_size(svg_color_type_t type) {
  uint32_t size = 0;
  switch (type) {
    case SVG_COLOR_NULL: {
      break;
    }
    case SVG_COLOR_SOLID: {
      size = sizeof(color_t);
      break;
    }
    case SVG_COLOR_LINEAR_GRADIENT: {
      size = sizeof(svg_color_linear_gradient_t);
      break;
    }
    default: {
      assert(!"not supported type");
      break;
    }
  }

  return size;
}

svg_color_t* svg_color_solid_init(svg_color_t* c, color_t solid) {
  c->solid = solid;
  return c;
}

svg_color_t* svg_color_linear_gradient_init(svg_color_t* c, float sx, float sy, float ex, float ey,
                                            color_t icolor, color_t ocolor) {
  c->linear_gradient.sx = sx;
  c->linear_gradient.sy = sy;
  c->linear_gradient.ex = ex;
  c->linear_gradient.ey = ey;
  c->linear_gradient.icolor = icolor;
  c->linear_gradient.ocolor = ocolor;
  return c;
}
