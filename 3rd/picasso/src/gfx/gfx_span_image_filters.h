/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_SPAN_IMAGE_FILTERS_H_
#define _GFX_SPAN_IMAGE_FILTERS_H_

#include "common.h"
#include "graphic_base.h"

#include "gfx_image_filters.h"
#include "gfx_span_generator.h"

namespace gfx {

// span image filter
template <typename ColorType, typename Source, typename Interpolator>
class gfx_span_image_filter
{
public:
    typedef Source source_type;
    typedef Interpolator interpolator_type;

    explicit gfx_span_image_filter(source_type& src,
        interpolator_type& interpolator, const image_filter_adapter* filter)
        : m_src(&src)
        , m_interpolator(&interpolator)
        , m_filter(filter)
        , m_dx_flt(FLT_TO_SCALAR(0.5f))
        , m_dy_flt(FLT_TO_SCALAR(0.5f))
        , m_dx_int(image_subpixel_scale / 2)
        , m_dy_int(image_subpixel_scale / 2)
    {
    }
    
    void prepare(void) { /* do nothing, scanline raster needed. */ }

    void attach(source_type& v) { m_src = &v; }

    interpolator_type& interpolator(void) { return *m_interpolator; }
    source_type& source(void) { return *m_src; }
    const image_filter_adapter& filter(void) const { return *m_filter; }
    int filter_dx_int(void) const { return m_dx_int; }
    int filter_dy_int(void) const { return m_dy_int; }
    scalar filter_dx_flt(void) const { return m_dx_flt; }
    scalar filter_dy_flt(void) const { return m_dy_flt; }

    void interpolator(interpolator_type& v)  { m_interpolator = &v; }
    void filter(const image_filter_adapter& v)   { m_filter = &v; }
    void filter_offset(scalar d) { filter_offset(d, d); }
    void filter_offset(scalar dx, scalar dy)
    {
        m_dx_flt = dx;
        m_dy_flt = dy;
        m_dx_int = iround(dx * image_subpixel_scale);
        m_dy_int = iround(dy * image_subpixel_scale);
    }

private:
    source_type* m_src;
    interpolator_type* m_interpolator;
    const image_filter_adapter* m_filter;
    scalar m_dx_flt;
    scalar m_dy_flt;
    unsigned int m_dx_int;
    unsigned int m_dy_int;
};


// rgba color format filters
// span image filter rgba
template <typename ColorType, typename Source, typename Interpolator> 
class gfx_span_image_filter_rgba : public gfx_span_image_filter<ColorType, Source, Interpolator>
{
public:
    typedef ColorType color_type;
    typedef Source source_type;
    typedef typename source_type::order_type order_type;
    typedef Interpolator interpolator_type;
    typedef gfx_span_image_filter<color_type, source_type, interpolator_type> base_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    explicit gfx_span_image_filter_rgba(source_type& src, 
        interpolator_type& inter, const image_filter_adapter& filter)
        : base_type(src, inter, &filter) 
    {
    }

    void generate(color_type* span, int x, int y, unsigned int len)
    {
        base_type::interpolator().begin(x + base_type::filter_dx_flt(), 
                                        y + base_type::filter_dy_flt(), len);

        int fg[4];
        const value_type *fg_ptr;

        unsigned int diameter = base_type::filter().diameter();
        int start = base_type::filter().start();
        const int16_t* weight_array = base_type::filter().weight_array();

        int x_count; 
        int weight_y;

        do {
            base_type::interpolator().coordinates(&x, &y);

            x -= base_type::filter_dx_int();
            y -= base_type::filter_dy_int();

            int x_hr = x; 
            int y_hr = y; 

            int x_lr = x_hr >> image_subpixel_shift;
            int y_lr = y_hr >> image_subpixel_shift;

            fg[0] = fg[1] = fg[2] = fg[3] = image_filter_scale / 2;

            int x_fract = x_hr & image_subpixel_mask;
            unsigned int y_count = diameter;

            y_hr = image_subpixel_mask - (y_hr & image_subpixel_mask);
            fg_ptr = (const value_type*)base_type::source().span(x_lr + start, y_lr + start, diameter);

            for (;;) {
                x_count = diameter;
                weight_y = weight_array[y_hr];
                x_hr = image_subpixel_mask - x_fract;
                for (;;) {
                    int weight = (weight_y * weight_array[x_hr] + 
                                 image_filter_scale / 2) >> 
                                 image_filter_shift;

                    fg[0] += weight * *fg_ptr++;
                    fg[1] += weight * *fg_ptr++;
                    fg[2] += weight * *fg_ptr++;
                    fg[3] += weight * *fg_ptr;

                    if (--x_count == 0)
                        break;

                    x_hr += image_subpixel_scale;
                    fg_ptr = (const value_type*)base_type::source().next_x();
                }

                if (--y_count == 0)
                    break;

                y_hr += image_subpixel_scale;
                fg_ptr = (const value_type*)base_type::source().next_y();
            }

            fg[0] >>= image_filter_shift;
            fg[1] >>= image_filter_shift;
            fg[2] >>= image_filter_shift;
            fg[3] >>= image_filter_shift;

            if (fg[0] < 0) fg[0] = 0;
            if (fg[1] < 0) fg[1] = 0;
            if (fg[2] < 0) fg[2] = 0;
            if (fg[3] < 0) fg[3] = 0;

            if (fg[order_type::A] > base_mask)         fg[order_type::A] = base_mask;
            if (fg[order_type::R] > fg[order_type::A]) fg[order_type::R] = fg[order_type::A];
            if (fg[order_type::G] > fg[order_type::A]) fg[order_type::G] = fg[order_type::A];
            if (fg[order_type::B] > fg[order_type::A]) fg[order_type::B] = fg[order_type::A];

            span->r = (value_type)fg[order_type::R];
            span->g = (value_type)fg[order_type::G];
            span->b = (value_type)fg[order_type::B];
            span->a = (value_type)fg[order_type::A];
            ++span;
            ++base_type::interpolator();

        } while(--len);
    }
};

// span image filter rgba no blending
template <typename ColorType, typename Source, typename Interpolator> 
class gfx_span_image_filter_rgba_nb : public gfx_span_image_filter<ColorType, Source, Interpolator>
{
public:
    typedef ColorType color_type;
    typedef Source source_type;
    typedef typename source_type::order_type order_type;
    typedef Interpolator interpolator_type;
    typedef gfx_span_image_filter<color_type, source_type, interpolator_type> base_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    explicit gfx_span_image_filter_rgba_nb(source_type& src, 
        interpolator_type& inter, const image_filter_adapter& filter)
        : base_type(src, inter, &filter) 
    {
    }

    void generate(color_type* span, int x, int y, unsigned int len)
    {
        base_type::interpolator().begin(x + base_type::filter_dx_flt(), 
                                        y + base_type::filter_dy_flt(), len);

        int fg[4];
        const value_type *fg_ptr;

        unsigned int diameter = base_type::filter().diameter();
        int start = base_type::filter().start();
        const int16_t* weight_array = base_type::filter().weight_array();

        int x_count; 
        int weight_y;

        do {
            base_type::interpolator().coordinates(&x, &y);

            x -= base_type::filter_dx_int();
            y -= base_type::filter_dy_int();

            int x_hr = x; 
            int y_hr = y; 

            int x_lr = x_hr >> image_subpixel_shift;
            int y_lr = y_hr >> image_subpixel_shift;

            fg[0] = fg[1] = fg[2] = fg[3] = image_filter_scale / 2;

            int x_fract = x_hr & image_subpixel_mask;
            unsigned int y_count = diameter;

            y_hr = image_subpixel_mask - (y_hr & image_subpixel_mask);
            fg_ptr = (const value_type*)base_type::source().span(x_lr + start, y_lr + start, diameter);

            for (;;) {
                x_count = diameter;
                weight_y = weight_array[y_hr];
                x_hr = image_subpixel_mask - x_fract;
                for (;;) {
                    int weight = (weight_y * weight_array[x_hr] + 
                                 image_filter_scale / 2) >> 
                                 image_filter_shift;

                    fg[0] += weight * *fg_ptr++;
                    fg[1] += weight * *fg_ptr++;
                    fg[2] += weight * *fg_ptr++;
                    fg[3] += weight * *fg_ptr;

                    if (--x_count == 0)
                        break;

                    x_hr  += image_subpixel_scale;
                    fg_ptr = (const value_type*)base_type::source().next_x();
                }

                if (--y_count == 0)
                    break;

                y_hr += image_subpixel_scale;
                fg_ptr = (const value_type*)base_type::source().next_y();
            }

            fg[0] >>= image_filter_shift;
            fg[1] >>= image_filter_shift;
            fg[2] >>= image_filter_shift;
            fg[3] >>= image_filter_shift;

            if (fg[0] < 0) fg[0] = 0;
            if (fg[1] < 0) fg[1] = 0;
            if (fg[2] < 0) fg[2] = 0;
            if (fg[3] < 0) fg[3] = 0;

            span->r = (value_type)fg[order_type::R];
            span->g = (value_type)fg[order_type::G];
            span->b = (value_type)fg[order_type::B];
            span->a = base_mask;
            ++span;
            ++base_type::interpolator();
        } while(--len);
    }
};


//span image filter rgba nearest 
template <typename ColorType, typename Source, typename Interpolator> 
class gfx_span_image_filter_rgba_nn : public gfx_span_image_filter<ColorType, Source, Interpolator>
{
public:
    typedef ColorType color_type;
    typedef Source source_type;
    typedef typename source_type::order_type order_type;
    typedef Interpolator interpolator_type;
    typedef gfx_span_image_filter<color_type, source_type, interpolator_type> base_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    explicit gfx_span_image_filter_rgba_nn(source_type& src, interpolator_type& inter)
        : base_type(src, inter, 0) 
    {
    }

    void generate(color_type* span, int x, int y, unsigned int len)
    {
        base_type::interpolator().begin(x + base_type::filter_dx_flt(), 
                                        y + base_type::filter_dy_flt(), len);
        do {
            base_type::interpolator().coordinates(&x, &y);
            const value_type* fg_ptr = (const value_type*)
                base_type::source().span(x >> image_subpixel_shift, 
                                         y >> image_subpixel_shift, 1);

            span->r = fg_ptr[order_type::R];
            span->g = fg_ptr[order_type::G];
            span->b = fg_ptr[order_type::B];
            span->a = fg_ptr[order_type::A];
            ++span;
            ++base_type::interpolator();

        } while(--len);
    }
};

//span image filter rgba nearest no blending
template <typename ColorType, typename Source, typename Interpolator> 
class gfx_span_image_filter_rgba_nn_nb : public gfx_span_image_filter<ColorType, Source, Interpolator>
{
public:
    typedef ColorType color_type;
    typedef Source source_type;
    typedef typename source_type::order_type order_type;
    typedef Interpolator interpolator_type;
    typedef gfx_span_image_filter<color_type, source_type, interpolator_type> base_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    explicit gfx_span_image_filter_rgba_nn_nb(source_type& src, interpolator_type& inter)
        : base_type(src, inter, 0) 
    {
    }

    void generate(color_type* span, int x, int y, unsigned int len)
    {
        base_type::interpolator().begin(x + base_type::filter_dx_flt(), 
                                        y + base_type::filter_dy_flt(), len);
        do {
            base_type::interpolator().coordinates(&x, &y);
            const value_type* fg_ptr = (const value_type*)
                base_type::source().span(x >> image_subpixel_shift, 
                                         y >> image_subpixel_shift, 1);

            span->r = fg_ptr[order_type::R];
            span->g = fg_ptr[order_type::G];
            span->b = fg_ptr[order_type::B];
            span->a = base_mask;
            ++span;
            ++base_type::interpolator();

        } while(--len);
    }
};


// span image filter rgb
template <typename ColorType, typename Source, typename Interpolator> 
class gfx_span_image_filter_rgb : public gfx_span_image_filter<ColorType, Source, Interpolator>
{
public:
    typedef ColorType color_type;
    typedef Source source_type;
    typedef typename source_type::order_type order_type;
    typedef Interpolator interpolator_type;
    typedef gfx_span_image_filter<color_type, source_type, interpolator_type> base_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    explicit gfx_span_image_filter_rgb(source_type& src, 
        interpolator_type& inter, const image_filter_adapter& filter)
        : base_type(src, inter, &filter) 
    {
    }

    void generate(color_type* span, int x, int y, unsigned int len)
    {
        base_type::interpolator().begin(x + base_type::filter_dx_flt(), 
                                        y + base_type::filter_dy_flt(), len);

        int fg[3];
        const value_type *fg_ptr;

        unsigned int diameter = base_type::filter().diameter();
        int start = base_type::filter().start();
        const int16_t* weight_array = base_type::filter().weight_array();

        int x_count; 
        int weight_y;

        do {
            base_type::interpolator().coordinates(&x, &y);

            x -= base_type::filter_dx_int();
            y -= base_type::filter_dy_int();

            int x_hr = x; 
            int y_hr = y; 

            int x_lr = x_hr >> image_subpixel_shift;
            int y_lr = y_hr >> image_subpixel_shift;

            fg[0] = fg[1] = fg[2] = image_filter_scale / 2;

            int x_fract = x_hr & image_subpixel_mask;
            unsigned int y_count = diameter;

            y_hr = image_subpixel_mask - (y_hr & image_subpixel_mask);
            fg_ptr = (const value_type*)base_type::source().span(x_lr + start, y_lr + start, diameter);

            for (;;) {
                x_count  = diameter;
                weight_y = weight_array[y_hr];
                x_hr = image_subpixel_mask - x_fract;

                for (;;) {
                    int weight = (weight_y * weight_array[x_hr] + 
                                 image_filter_scale / 2) >> 
                                 image_filter_shift;

                    fg[0] += weight * *fg_ptr++;
                    fg[1] += weight * *fg_ptr++;
                    fg[2] += weight * *fg_ptr;

                    if (--x_count == 0)
                        break;

                    x_hr += image_subpixel_scale;
                    fg_ptr = (const value_type*)base_type::source().next_x();
                }

                if (--y_count == 0)
                    break;

                y_hr += image_subpixel_scale;
                fg_ptr = (const value_type*)base_type::source().next_y();
            }

            fg[0] >>= image_filter_shift;
            fg[1] >>= image_filter_shift;
            fg[2] >>= image_filter_shift;

            if (fg[0] < 0) fg[0] = 0;
            if (fg[1] < 0) fg[1] = 0;
            if (fg[2] < 0) fg[2] = 0;

            if (fg[order_type::R] > base_mask) fg[order_type::R] = base_mask;
            if (fg[order_type::G] > base_mask) fg[order_type::G] = base_mask;
            if (fg[order_type::B] > base_mask) fg[order_type::B] = base_mask;

            span->r = (value_type)fg[order_type::R];
            span->g = (value_type)fg[order_type::G];
            span->b = (value_type)fg[order_type::B];
            span->a = base_mask;

            ++span;
            ++base_type::interpolator();

        } while(--len);
    }
};

// span image filter rgb nearest
template <typename ColorType, typename Source, typename Interpolator> 
class gfx_span_image_filter_rgb_nn : public gfx_span_image_filter<ColorType, Source, Interpolator>
{
public:
    typedef ColorType color_type;
    typedef Source source_type;
    typedef typename source_type::order_type order_type;
    typedef Interpolator interpolator_type;
    typedef gfx_span_image_filter<color_type, source_type, interpolator_type> base_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    explicit gfx_span_image_filter_rgb_nn(source_type& src, interpolator_type& inter)
        : base_type(src, inter, 0) 
    {
    }

    void generate(color_type* span, int x, int y, unsigned int len)
    {
        base_type::interpolator().begin(x + base_type::filter_dx_flt(), 
                                        y + base_type::filter_dy_flt(), len);
        do {
            base_type::interpolator().coordinates(&x, &y);
            const value_type* fg_ptr = (const value_type*)
                base_type::source().span(x >> image_subpixel_shift, 
                                         y >> image_subpixel_shift, 1);

            span->r = fg_ptr[order_type::R];
            span->g = fg_ptr[order_type::G];
            span->b = fg_ptr[order_type::B];
            span->a = base_mask;
            ++span;
            ++base_type::interpolator();

        } while(--len);
    }
};


// span image filter rgb16
template <typename ColorType, typename Source, typename Interpolator> 
class gfx_span_image_filter_rgb16 : public gfx_span_image_filter<ColorType, Source, Interpolator>
{
public:
    typedef ColorType color_type;
    typedef Source source_type;
    typedef typename source_type::order_type order_type;
    typedef typename source_type::pixel_type pixel_type;
    typedef Interpolator interpolator_type;
    typedef gfx_span_image_filter<color_type, source_type, interpolator_type> base_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
        color_mask = 0xFFFFFFFF,
        r_mask = (color_mask >> (order_type::G + order_type::B)) << (order_type::G + order_type::B),
        g_mask = (((color_mask) & (~r_mask)) >> (order_type::B)) << (order_type::B),
        b_mask = ((color_mask) & (~(r_mask | g_mask))),
    };

    explicit gfx_span_image_filter_rgb16(source_type& src,
        interpolator_type& inter, const image_filter_adapter& filter)
        : base_type(src, inter, &filter) 
    {
    }

    void generate(color_type* span, int x, int y, unsigned int len)
    {
        base_type::interpolator().begin(x + base_type::filter_dx_flt(), 
                                        y + base_type::filter_dy_flt(), len);

        int fg[3];
        const value_type *fg_ptr;

        unsigned int diameter = base_type::filter().diameter();
        int start = base_type::filter().start();
        const int16_t* weight_array = base_type::filter().weight_array();

        int x_count; 
        int weight_y;

        pixel_type rgb_pixel;

        do {
            base_type::interpolator().coordinates(&x, &y);

            x -= base_type::filter_dx_int();
            y -= base_type::filter_dy_int();

            int x_hr = x; 
            int y_hr = y; 

            int x_lr = x_hr >> image_subpixel_shift;
            int y_lr = y_hr >> image_subpixel_shift;

            fg[0] = fg[1] = fg[2] = image_filter_scale >> 1;

            int x_fract = x_hr & image_subpixel_mask;
            unsigned int y_count = diameter;

            y_hr = image_subpixel_mask - (y_hr & image_subpixel_mask);
            fg_ptr = (const value_type*)base_type::source().span(x_lr + start, y_lr + start, diameter);
            rgb_pixel = *reinterpret_cast<const pixel_type*>(fg_ptr);

            for (;;) {
                x_count = diameter;
                weight_y = weight_array[y_hr];
                x_hr = image_subpixel_mask - x_fract;

                for (;;) {
                    int weight = (weight_y * weight_array[x_hr] + 
                                 (image_filter_scale>>1)) >> 
                                 image_filter_shift;

                    fg[0] += weight * ((rgb_pixel & r_mask) >> (order_type::G + order_type::B));
                    fg[1] += weight * ((rgb_pixel & g_mask) >> (order_type::B));
                    fg[2] += weight * (rgb_pixel & b_mask);
                    fg_ptr += 2;

                    if (--x_count == 0)
                        break;

                    x_hr += image_subpixel_scale;
                    fg_ptr = (const value_type*)base_type::source().next_x();
                    rgb_pixel = *reinterpret_cast<const pixel_type*>(fg_ptr);
                }

                if (--y_count == 0)
                    break;

                y_hr += image_subpixel_scale;
                fg_ptr = (const value_type*)base_type::source().next_y();
                rgb_pixel = *reinterpret_cast<const pixel_type*>(fg_ptr);
            }

            fg[0] >>= image_filter_shift;
            fg[1] >>= image_filter_shift;
            fg[2] >>= image_filter_shift;

            if (fg[0] < 0) fg[0] = 0;
            if (fg[1] < 0) fg[1] = 0;
            if (fg[2] < 0) fg[2] = 0;

            if (fg[0] > (int)base_mask) fg[0] = base_mask;
            if (fg[1] > (int)base_mask) fg[1] = base_mask;
            if (fg[2] > (int)base_mask) fg[2] = base_mask;

            span->r = (value_type)(fg[0]<<(base_shift-order_type::R));
            span->g = (value_type)(fg[1]<<(base_shift-order_type::G));
            span->b = (value_type)(fg[2]<<(base_shift-order_type::B));
            span->a = base_mask;

            ++span;
            ++base_type::interpolator();

        } while(--len);
    }
};

//span image filter rgb16 nearest 
template <typename ColorType, typename Source, typename Interpolator> 
class gfx_span_image_filter_rgb16_nn : public gfx_span_image_filter<ColorType, Source, Interpolator>
{
public:
    typedef ColorType color_type;
    typedef Source source_type;
    typedef typename source_type::order_type order_type;
    typedef typename source_type::pixel_type pixel_type;
    typedef Interpolator interpolator_type;
    typedef gfx_span_image_filter<color_type, source_type, interpolator_type> base_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
        color_mask = 0xFFFFFFFF,
        r_mask = (color_mask >> (order_type::G + order_type::B)) << (order_type::G + order_type::B),
        g_mask = (((color_mask) & (~r_mask)) >> (order_type::B)) << (order_type::B),
        b_mask = ((color_mask) & (~(r_mask | g_mask))),
    };

    explicit gfx_span_image_filter_rgb16_nn(source_type& src, interpolator_type& inter)
        : base_type(src, inter, 0) 
    {
    }

    void generate(color_type* span, int x, int y, unsigned int len)
    {
        base_type::interpolator().begin(x + base_type::filter_dx_flt(), 
                                        y + base_type::filter_dy_flt(), len);
        do {
            base_type::interpolator().coordinates(&x, &y);
            const value_type* fg_ptr = (const value_type*)
                base_type::source().span(x >> image_subpixel_shift, 
                                         y >> image_subpixel_shift, 1);

            register pixel_type rgb = *reinterpret_cast<const pixel_type*>(fg_ptr);
            span->r = (rgb & r_mask) >> (base_shift-(16-order_type::R-order_type::G-order_type::B));
            span->g = (rgb & g_mask) >> (order_type::G+order_type::B-base_shift);
            span->b = (rgb & b_mask) << (base_shift-order_type::B);
            span->a = base_mask;
            ++span;
            ++base_type::interpolator();

        } while(--len);
    }
};

}
#endif /*_GFX_SPAN_IMAGE_FILTERS_H_*/
