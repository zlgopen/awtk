/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _PAINTER_RASTER_H_
#define _PAINTER_RASTER_H_

#include "gfx_image_accessors.h"
#include "gfx_pixfmt_rgb16.h"
#include "gfx_pixfmt_rgb.h"
#include "gfx_pixfmt_rgba.h"
#include "gfx_pixfmt_wrapper.h"
#include "gfx_span_image_filters.h"

namespace gfx {

typedef gfx_span_interpolator_linear interpolator_type;

typedef gfx_alpha_mask_u8 mask_type;

template <typename Pixfmt> struct painter_raster;

//32 bit color bilinear
template <> struct painter_raster<pixfmt_rgba32>
{
    typedef gfx_pixfmt_wrapper<pixfmt_rgba32, mask_type> format;
    typedef format::color_type color_type;
    typedef image_accessor<format> source_type;
    typedef pattern_wrapper<format> pattern_type;
    typedef gfx_span_image_filter_rgba_nb<color_type, source_type, interpolator_type> span_image_filter_type;
    typedef gfx_span_image_filter_rgba_nn_nb<color_type, source_type, interpolator_type> span_image_filter_type_nn;
    typedef gfx_span_image_filter_rgba<color_type, source_type, interpolator_type> span_canvas_filter_type;
    typedef gfx_span_image_filter_rgba_nn<color_type, source_type, interpolator_type> span_canvas_filter_type_nn;
    //pattern repeat
    typedef gfx_span_image_filter_rgba_nb<color_type, pattern_type, interpolator_type> span_image_pattern_type;
    typedef gfx_span_image_filter_rgba_nn_nb<color_type, pattern_type, interpolator_type> span_image_pattern_type_nn;
    typedef gfx_span_image_filter_rgba<color_type, pattern_type, interpolator_type> span_canvas_pattern_type;
    typedef gfx_span_image_filter_rgba_nn<color_type, pattern_type, interpolator_type> span_canvas_pattern_type_nn;
};

template <> struct painter_raster<pixfmt_argb32>
{
    typedef gfx_pixfmt_wrapper<pixfmt_argb32, mask_type> format;
    typedef format::color_type color_type;
    typedef image_accessor<format> source_type;
    typedef pattern_wrapper<format> pattern_type;
    typedef gfx_span_image_filter_rgba_nb<color_type, source_type, interpolator_type> span_image_filter_type;
    typedef gfx_span_image_filter_rgba_nn_nb<color_type, source_type, interpolator_type> span_image_filter_type_nn;
    typedef gfx_span_image_filter_rgba<color_type, source_type, interpolator_type> span_canvas_filter_type;
    typedef gfx_span_image_filter_rgba_nn<color_type, source_type, interpolator_type> span_canvas_filter_type_nn;
    //pattern repeat
    typedef gfx_span_image_filter_rgba_nb<color_type, pattern_type, interpolator_type> span_image_pattern_type;
    typedef gfx_span_image_filter_rgba_nn_nb<color_type, pattern_type, interpolator_type> span_image_pattern_type_nn;
    typedef gfx_span_image_filter_rgba<color_type, pattern_type, interpolator_type> span_canvas_pattern_type;
    typedef gfx_span_image_filter_rgba_nn<color_type, pattern_type, interpolator_type> span_canvas_pattern_type_nn;
};

template <> struct painter_raster<pixfmt_abgr32>
{
    typedef gfx_pixfmt_wrapper<pixfmt_abgr32, mask_type> format;
    typedef format::color_type color_type;
    typedef image_accessor<format> source_type;
    typedef pattern_wrapper<format> pattern_type;
    typedef gfx_span_image_filter_rgba_nb<color_type, source_type, interpolator_type> span_image_filter_type;
    typedef gfx_span_image_filter_rgba_nn_nb<color_type, source_type, interpolator_type> span_image_filter_type_nn;
    typedef gfx_span_image_filter_rgba<color_type, source_type, interpolator_type> span_canvas_filter_type;
    typedef gfx_span_image_filter_rgba_nn<color_type, source_type, interpolator_type> span_canvas_filter_type_nn;
    //pattern repeat
    typedef gfx_span_image_filter_rgba_nb<color_type, pattern_type, interpolator_type> span_image_pattern_type;
    typedef gfx_span_image_filter_rgba_nn_nb<color_type, pattern_type, interpolator_type> span_image_pattern_type_nn;
    typedef gfx_span_image_filter_rgba<color_type, pattern_type, interpolator_type> span_canvas_pattern_type;
    typedef gfx_span_image_filter_rgba_nn<color_type, pattern_type, interpolator_type> span_canvas_pattern_type_nn;
};

template <> struct painter_raster<pixfmt_bgra32>
{
    typedef gfx_pixfmt_wrapper<pixfmt_bgra32, mask_type> format;
    typedef format::color_type color_type;
    typedef image_accessor<format> source_type;
    typedef pattern_wrapper<format> pattern_type;
    typedef gfx_span_image_filter_rgba_nb<color_type, source_type, interpolator_type> span_image_filter_type;
    typedef gfx_span_image_filter_rgba_nn_nb<color_type, source_type, interpolator_type> span_image_filter_type_nn;
    typedef gfx_span_image_filter_rgba<color_type, source_type, interpolator_type> span_canvas_filter_type;
    typedef gfx_span_image_filter_rgba_nn<color_type, source_type, interpolator_type> span_canvas_filter_type_nn;
    //pattern repeat
    typedef gfx_span_image_filter_rgba_nb<color_type, pattern_type, interpolator_type> span_image_pattern_type;
    typedef gfx_span_image_filter_rgba_nn_nb<color_type, pattern_type, interpolator_type> span_image_pattern_type_nn;
    typedef gfx_span_image_filter_rgba<color_type, pattern_type, interpolator_type> span_canvas_pattern_type;
    typedef gfx_span_image_filter_rgba_nn<color_type, pattern_type, interpolator_type> span_canvas_pattern_type_nn;
};

//24 bit color bilinear
template <> struct painter_raster<pixfmt_rgb24>
{
    typedef gfx_pixfmt_wrapper<pixfmt_rgb24, mask_type> format;
    typedef format::color_type color_type;
    typedef image_accessor<format> source_type;
    typedef pattern_wrapper<format> pattern_type;
    typedef gfx_span_image_filter_rgb<color_type, source_type, interpolator_type> span_image_filter_type;
    typedef gfx_span_image_filter_rgb_nn<color_type, source_type, interpolator_type> span_image_filter_type_nn;
    typedef gfx_span_image_filter_rgb<color_type, source_type, interpolator_type> span_canvas_filter_type;
    typedef gfx_span_image_filter_rgb_nn<color_type, source_type, interpolator_type> span_canvas_filter_type_nn;
    //pattern repeat
    typedef gfx_span_image_filter_rgb<color_type, pattern_type, interpolator_type> span_image_pattern_type;
    typedef gfx_span_image_filter_rgb_nn<color_type, pattern_type, interpolator_type> span_image_pattern_type_nn;
    typedef gfx_span_image_filter_rgb<color_type, pattern_type, interpolator_type> span_canvas_pattern_type;
    typedef gfx_span_image_filter_rgb_nn<color_type, pattern_type, interpolator_type> span_canvas_pattern_type_nn;
};

template <> struct painter_raster<pixfmt_bgr24>
{
    typedef gfx_pixfmt_wrapper<pixfmt_bgr24, mask_type> format;
    typedef format::color_type color_type;
    typedef image_accessor<format> source_type;
    typedef pattern_wrapper<format> pattern_type;
    typedef gfx_span_image_filter_rgb<color_type, source_type, interpolator_type> span_image_filter_type;
    typedef gfx_span_image_filter_rgb_nn<color_type, source_type, interpolator_type> span_image_filter_type_nn;
    typedef gfx_span_image_filter_rgb<color_type, source_type, interpolator_type> span_canvas_filter_type;
    typedef gfx_span_image_filter_rgb_nn<color_type, source_type, interpolator_type> span_canvas_filter_type_nn;
    //pattern repeat
    typedef gfx_span_image_filter_rgb<color_type, pattern_type, interpolator_type> span_image_pattern_type;
    typedef gfx_span_image_filter_rgb_nn<color_type, pattern_type, interpolator_type> span_image_pattern_type_nn;
    typedef gfx_span_image_filter_rgb<color_type, pattern_type, interpolator_type> span_canvas_pattern_type;
    typedef gfx_span_image_filter_rgb_nn<color_type, pattern_type, interpolator_type> span_canvas_pattern_type_nn;
};

//16 bit color bilinear
template <> struct painter_raster<pixfmt_rgb565>
{
    typedef gfx_pixfmt_wrapper<pixfmt_rgb565, mask_type> format;
    typedef format::color_type color_type;
    typedef image_accessor<format> source_type;
    typedef pattern_wrapper<format> pattern_type;
    typedef gfx_span_image_filter_rgb16<color_type, source_type, interpolator_type> span_image_filter_type;
    typedef gfx_span_image_filter_rgb16_nn<color_type, source_type, interpolator_type> span_image_filter_type_nn;
    typedef gfx_span_image_filter_rgb16<color_type, source_type, interpolator_type> span_canvas_filter_type;
    typedef gfx_span_image_filter_rgb16_nn<color_type, source_type, interpolator_type> span_canvas_filter_type_nn;
    //pattern repeat
    typedef gfx_span_image_filter_rgb16<color_type, pattern_type, interpolator_type> span_image_pattern_type;
    typedef gfx_span_image_filter_rgb16_nn<color_type, pattern_type, interpolator_type> span_image_pattern_type_nn;
    typedef gfx_span_image_filter_rgb16<color_type, pattern_type, interpolator_type> span_canvas_pattern_type;
    typedef gfx_span_image_filter_rgb16_nn<color_type, pattern_type, interpolator_type> span_canvas_pattern_type_nn;
};

template <> struct painter_raster<pixfmt_rgb555>
{
    typedef gfx_pixfmt_wrapper<pixfmt_rgb555, mask_type> format;
    typedef format::color_type color_type;
    typedef image_accessor<format> source_type;
    typedef pattern_wrapper<format> pattern_type;
    typedef gfx_span_image_filter_rgb16<color_type, source_type, interpolator_type> span_image_filter_type;
    typedef gfx_span_image_filter_rgb16_nn<color_type, source_type, interpolator_type> span_image_filter_type_nn;
    typedef gfx_span_image_filter_rgb16<color_type, source_type, interpolator_type> span_canvas_filter_type;
    typedef gfx_span_image_filter_rgb16_nn<color_type, source_type, interpolator_type> span_canvas_filter_type_nn;
    //pattern repeat
    typedef gfx_span_image_filter_rgb16<color_type, pattern_type, interpolator_type> span_image_pattern_type;
    typedef gfx_span_image_filter_rgb16_nn<color_type, pattern_type, interpolator_type> span_image_pattern_type_nn;
    typedef gfx_span_image_filter_rgb16<color_type, pattern_type, interpolator_type> span_canvas_pattern_type;
    typedef gfx_span_image_filter_rgb16_nn<color_type, pattern_type, interpolator_type> span_canvas_pattern_type_nn;
};

}
#endif /*_PAINTER_RASTER_H_*/
