
#ifndef TK_SVG_COLOR_H
#define TK_SVG_COLOR_H

#include "tkc/color.h"

BEGIN_C_DECLS

typedef enum _svg_color_type_t {
  SVG_COLOR_NULL = 0,
  SVG_COLOR_SOLID,
  SVG_COLOR_LINEAR_GRADIENT,
} svg_color_type_t;

#pragma pack(push, 1)
typedef struct _svg_color_linear_gradient_t {
  float sx;
  float sy;
  float ex;
  float ey;
  color_t icolor;
  color_t ocolor;
} svg_color_linear_gradient_t;

typedef union _svg_color_t {
  color_t solid;
  svg_color_linear_gradient_t linear_gradient;
} svg_color_t;
#pragma pack(pop)

uint32_t svg_color_size(svg_color_type_t type);

svg_color_t* svg_color_solid_init(svg_color_t* c, color_t solid);

svg_color_t* svg_color_linear_gradient_init(svg_color_t* c, float sx, float sy, float ex, float ey,
                                            color_t icolor, color_t ocolor);

END_C_DECLS

#endif /*TK_SVG_COLOR_H*/
