/* Picasso - a vector graphics library
 *
 * Copyright (C) 2013 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _PICASSO_GRADIENT_H_
#define _PICASSO_GRADIENT_H_

#include "common.h"
#include "device.h"
#include "interfaces.h"

enum {
    SPREAD_PAD,
    SPREAD_REPEAT,
    SPREAD_REFLECT,
};

namespace picasso {

class trans_affine;

class gradient_adapter
{
public:
    gradient_adapter();
    ~gradient_adapter();

    void init_linear(int spread, scalar x1, scalar y1, scalar x2, scalar y2);
    void init_radial(int spread, scalar x1, scalar y1, scalar radius1, scalar x2, scalar y2, scalar radius2);
    void init_conic(int spread, scalar x, scalar y, scalar angle);

    void add_color_stop(scalar offset, const rgba& c);
    void clear_stops(void);

    void transform(const trans_affine& mtx);
public:
    abstract_gradient_adapter* impl(void) const { return m_impl; }
private:
    gradient_adapter(const gradient_adapter&);
    gradient_adapter& operator=(const gradient_adapter&);
    abstract_gradient_adapter* m_impl;
};

}
#endif /*_PICASSO_GRADIENT_H_*/
