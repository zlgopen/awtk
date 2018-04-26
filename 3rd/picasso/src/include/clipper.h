/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _CLIPPER_H_
#define _CLIPPER_H_

#include "common.h"
#include "math_type.h"
#include "data_vector.h"
#include "vertex.h"

namespace picasso {

typedef enum {
    POLY_UNION,            // union
    POLY_INTERSECT,        // intersection
    POLY_XOR,              // exclusive or
    POLY_DIFF,             // difference
} polygon_op;

typedef struct {
    int num_vertices;     // number of vertices in list
    vertex_s *vertex;     // vertex array pointer
} vertex_list;

typedef struct {
    int num_contours;     // number of contours in polygon
    vertex_list *contour; // contour vector pointer
} polygon;

void polygon_clip(polygon_op set_operation, polygon *subject_polygon,
                                polygon *clip_polygon, polygon *result_polygon);

void free_polygon(polygon *polygon);

}
#endif /*_CLIPPER_H_*/
