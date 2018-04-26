/* Picasso - a vector graphics library
 *
 * Copyright (C) 2009 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _PICASSO_GLOBAL_H_
#define _PICASSO_GLOBAL_H_

#include "common.h"
#include "math_type.h"
#include "data_vector.h"
#include "fixedopt.h"

#include "picasso.h"

//this can be replace by hw buffer!
#define BufferAlloc(n)         mem_malloc(n)
#define BuffersAlloc(n, s)     mem_calloc(n, s)
#define BufferFree(p)          mem_free(p)
#define BufferCopy(d, s, n)    mem_copy(d, s, n)

#define MAX(x, y)    (((x) > (y))?(x):(y))
#define MIN(x, y)    (((x) < (y))?(x):(y))
#define ABS(x)       (((x) < 0)?(-(x)):(x))


// global error code
extern "C" ps_status global_status;

#endif /*_PICASSO_GLOBAL_H_*/
