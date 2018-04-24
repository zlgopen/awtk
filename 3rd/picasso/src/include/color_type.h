/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _COLOR_TYPE_H_
#define _COLOR_TYPE_H_

#include "common.h"
#include "math_type.h"
#include "graphic_base.h"

namespace picasso {

// supported byte orders for RGB and RGBA pixel formats
struct order_rgb  { enum { R=0, G=1, B=2, rgb_tag }; }; // order_rgb
struct order_bgr  { enum { B=0, G=1, R=2, rgb_tag }; }; // order_bgr
struct order_rgba { enum { R=0, G=1, B=2, A=3, rgba_tag }; }; // order_rgba
struct order_argb { enum { A=0, R=1, G=2, B=3, rgba_tag }; }; // order_argb
struct order_abgr { enum { A=0, B=1, G=2, R=3, rgba_tag }; }; // order_abgr
struct order_bgra { enum { B=0, G=1, R=2, A=3, rgba_tag }; }; // order_bgra
//packed color order
//packed color is bitset not index
struct order_rgb565 { enum {R=5, G=6, B=5, rgbp_tag }; }; // order_rgb565
struct order_rgb555 { enum {R=5, G=5, B=5, rgbp_tag }; }; // order_rgb555

// pixfmt type
typedef enum {
    pix_fmt_rgba,
    pix_fmt_argb,
    pix_fmt_abgr,
    pix_fmt_bgra,
    pix_fmt_rgb,
    pix_fmt_bgr,
    pix_fmt_rgb565,
    pix_fmt_rgb555,
} pix_fmt;

// color rgba 0 ~ 1
struct rgba
{
    scalar r;
    scalar g;
    scalar b;
    scalar a;

    rgba() 
        : r(0), g(0), b(0), a(0)  
    {
    }

    rgba(scalar _r, scalar _g, scalar _b, scalar _a = FLT_TO_SCALAR(1.0f))
        : r(_r), g(_g), b(_b), a(_a)
    {
    }
};

// color rgba8 0 ~ 255
struct rgba8
{
    typedef byte value_type;
    typedef uint32_t calc_type;
    typedef int32_t long_type;

    enum {
        base_shift = 8,
        base_scale = 1 << base_shift,
        base_mask  = base_scale - 1
    };

    value_type r;
    value_type g;
    value_type b;
    value_type a;

    rgba8() 
        : r(0), g(0), b(0), a(0)
    {
    }

    rgba8(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a = base_mask)
        : r(_r), g(_g), b(_b), a(_a) 
    {
    }

    rgba8(const rgba& c) 
        : r((value_type)uround(c.r * scalar(base_mask))) 
        , g((value_type)uround(c.g * scalar(base_mask))) 
        , b((value_type)uround(c.b * scalar(base_mask))) 
        , a((value_type)uround(c.a * scalar(base_mask))) 
    {
    }

};

}

using picasso::rgba;
using picasso::rgba8;

#endif /*_COLOR_TYPE_H_*/
