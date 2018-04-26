/* Picasso - a vector graphics library
 *
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_GRADIENT_ADAPTER_H_
#define _GFX_GRADIENT_ADAPTER_H_

#include "common.h"
#include "interfaces.h"

#include "gfx_pixfmt_wrapper.h"
#include "gfx_span_generator.h"
#include "gfx_trans_affine.h"

#include "picasso_gradient.h"

namespace gfx {

// gradient wrapper interface
class gfx_gradient_wrapper
{
public:
    gfx_gradient_wrapper() { }
    virtual ~gfx_gradient_wrapper() { }
    virtual void init(scalar r, scalar x, scalar y) = 0;
    virtual int calculate(int x, int y, int d) const = 0;
};

// gradient table
class gfx_gradient_table
{
public:
    typedef rgba8 color_type;

    enum {
        color_table_size = 256,
    };
private:
    struct color_point {
        scalar offset;
        color_type color;

        color_point() : offset(INT_TO_SCALAR(0)) { }
        color_point(scalar off, const color_type& c)
            : offset(off), color(c)
        {
            if (offset < FLT_TO_SCALAR(0.0f))
                offset = FLT_TO_SCALAR(0.0f);

            if (offset > FLT_TO_SCALAR(1.0f))
                offset = FLT_TO_SCALAR(1.0f);
        }
    };

    static bool offset_less(const color_point& a, const color_point& b)
    {
        return a.offset < b.offset;
    }

    static bool offset_equal(const color_point& a, const color_point& b)
    {
        return a.offset == b.offset;
    }

public:
    gfx_gradient_table()
        : m_color_table(color_table_size)
    {
    }

    void build_table(void);

    void remove_all(void)
    {
        m_color_profile.clear();
    }

    void add_color(scalar offset, const color_type& color)
    {
        m_color_profile.add(color_point(offset, color));
    }

    static unsigned int size(void)
    {
        return color_table_size;
    }

    const color_type& operator [] (unsigned int i) const
    {
        return m_color_table[i];
    }

private:
    typedef pod_bvector<color_point, 4> color_profile_type;
    typedef pod_array<color_type> color_table_type;

    color_profile_type m_color_profile;
    color_table_type m_color_table;
};

// span_gradient
template <typename ColorType>
class gfx_span_gradient
{
public:
    typedef ColorType color_type;
    typedef gfx_gradient_table color_func;
    typedef gfx_span_interpolator_linear interpolator_type;
    typedef gfx_gradient_wrapper gradient_type;

    enum {
        downscale_shift = interpolator_type::subpixel_shift - gradient_subpixel_shift,
    };

    gfx_span_gradient(interpolator_type& inter, const gradient_type& gradient_function,
                                        const color_func& color_function, scalar d1, scalar d2)
        : m_interpolator(&inter)
        , m_gradient_function(&gradient_function)
        , m_color_function(&color_function)
        , m_d1(iround(d1 * gradient_subpixel_scale))
        , m_d2(iround(d2 * gradient_subpixel_scale))
    {
    }

    void prepare(void)
    {
        // do nothing, scanline raster needed.
    }

    void generate(color_type* span, int x, int y, unsigned int len)
    {
        int dd = m_d2 - m_d1;
        if (dd < 1)
            dd = 1;

        m_interpolator->begin(INT_TO_SCALAR(x)+FLT_TO_SCALAR(0.5f),
                              INT_TO_SCALAR(y)+FLT_TO_SCALAR(0.5f), len);
        do {
            m_interpolator->coordinates(&x, &y);
            int d = m_gradient_function->calculate(x >> downscale_shift,
                    y >> downscale_shift, m_d2);
            d = ((d - m_d1) * (int)m_color_function->size()) / dd;

            if (d < 0)
                d = 0;

            if (d >= (int)m_color_function->size())
                d = m_color_function->size() - 1;

            *span++ = (*m_color_function)[d];
            ++(*m_interpolator);
        } while(--len);
    }

private:
    interpolator_type* m_interpolator;
    const gradient_type* m_gradient_function;
    const color_func* m_color_function;
    int m_d1;
    int m_d2;
};

// gradient adaptor
class gfx_gradient_adapter : public abstract_gradient_adapter
{
public:
    gfx_gradient_adapter()
        : m_wrapper(0)
        , m_start(0)
        , m_length(0)
        , m_build(false)
    {
    }

    virtual ~gfx_gradient_adapter()
    {
        if (m_wrapper)
            delete m_wrapper;
    }

    virtual void init_linear(int spread, scalar x1, scalar y1, scalar x2, scalar y2);

    virtual void init_radial(int spread, scalar x1, scalar y1, scalar radius1,
                                               scalar x2, scalar y2, scalar radius2);

    virtual void init_conic(int spread, scalar x, scalar y, scalar angle);

    virtual void add_color_stop(scalar offset, const picasso::rgba& c)
    {
        m_colors.add_color(SCALAR_TO_FLT(offset), rgba8(c));
        m_build = false;
    }

    virtual void clear_stops(void)
    {
        m_colors.remove_all();
        m_build = false;
    }

    virtual void transform(const abstract_trans_affine* mtx)
    {
        register const gfx_trans_affine* m = static_cast<const gfx_trans_affine*>(mtx);
        m_matrix *= (*const_cast<gfx_trans_affine*>(m));
    }

    void build(void)
    {
        if (!m_build) {
            m_colors.build_table();
            m_build = true;
        }
    }

    gfx_gradient_wrapper* wrapper(void) { return m_wrapper; }
    scalar start(void) { return m_start; }
    scalar length(void) { return m_length; }
    gfx_gradient_table& colors(void) { return m_colors; }
    gfx_trans_affine& matrix(void) { return m_matrix; }
private:
    gfx_gradient_wrapper* m_wrapper;
    scalar m_start;
    scalar m_length;
    bool m_build;
    gfx_trans_affine m_matrix;
    gfx_gradient_table m_colors;
};

}
#endif /*_GFX_GRADIENT_ADAPTER_H_*/
