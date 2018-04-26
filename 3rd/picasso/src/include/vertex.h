/* Picasso - a vector graphics library
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _VERTEX_OBJECT_H_
#define _VERTEX_OBJECT_H_

#include "math_type.h"

namespace picasso {

struct vertex_s
{
    scalar x;
    scalar y;

    vertex_s() : x(0), y(0){ }
    vertex_s(scalar _x, scalar _y) : x(_x), y(_y){ }
};

class vertex_source
{
public:
    virtual ~vertex_source() {}

    virtual void rewind(unsigned int id) = 0;
    virtual unsigned int vertex(scalar* x, scalar* y) = 0;
};

class vertex_container : public vertex_source
{
public:
    virtual ~vertex_container() {}

    virtual void add_vertex(scalar x, scalar y, unsigned int cmd) = 0;
    virtual void remove_all(void) = 0;
};

}

#endif /*_VERTEX_OBJECT_H_*/

