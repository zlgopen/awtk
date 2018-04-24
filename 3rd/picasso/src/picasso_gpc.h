/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _PICASSO_GPC_H_
#define _PICASSO_GPC_H_

#include "common.h"
#include "math_type.h"
#include "data_vector.h"
#include "vertex.h"

namespace picasso {

typedef enum {
  GPC_DIFF,                 // difference
  GPC_INT,                  // intersection
  GPC_XOR,                  // exclusive or
  GPC_UNION                 // union
} gpc_op;

typedef struct {
  int num_vertices;         // number of vertices in list
  vertex_s *vertex;         // vertex array pointer
} gpc_vertex_list;

typedef struct {
  int num_contours;         // number of contours in polygon
  int *hole;                // hole / external contour flags
  gpc_vertex_list *contour; // contour vector pointer
} gpc_polygon;

void gpc_polygon_clip(gpc_op set_operation, gpc_polygon *subject_polygon,
                                gpc_polygon *clip_polygon, gpc_polygon *result_polygon);

void gpc_free_polygon(gpc_polygon *polygon);

}
#endif /*_PICASSO_GPC_H_*/
