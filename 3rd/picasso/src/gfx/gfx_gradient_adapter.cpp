/* Picasso - a vector graphics library
 *
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"

#include "gfx_math.h"
#include "gfx_gradient_adapter.h"
#include "gfx_line_generator.h"

namespace gfx {

// gradient functions

// gradient_x
class gradient_x
{
public:
    void init(scalar, scalar, scalar) { }
    static int calculate(int x, int, int) { return x; }
};

// gradient_conic
class gradient_conic
{
public:
    void init(scalar, scalar, scalar) { }
    static int calculate(int x, int y, int d)
    {
        return uround(Fabs(Atan2(INT_TO_SCALAR(y), INT_TO_SCALAR(x))) * INT_TO_SCALAR(d) * _1divPI);
    }
};

class gradient_conic_vg
{
public:
    void init(scalar, scalar, scalar) { }
    static int calculate(int x, int y, int d)
    {
        scalar a = Atan2(INT_TO_SCALAR(y), INT_TO_SCALAR(x));
        if (a < 0) a = _2PI + a;
        return iround(a * INT_TO_SCALAR(d) * _1div2PI);
    }
};


// gradient_radial
class gradient_radial
{
public:
    void init(scalar, scalar, scalar) { }
    static int calculate(int x, int y, int d)
    {
        return (int)fast_sqrt(x*x + y*y);
    }
};

// gradient_radial_focus
class gradient_radial_focus
{
public:
    gradient_radial_focus()
        : m_r(100 * gradient_subpixel_scale)
        , m_fx(0)
        , m_fy(0)
    {
        update_values();
    }

    void init(scalar r, scalar fx, scalar fy)
    {
        m_r = iround(r * gradient_subpixel_scale);
        m_fx = iround(fx * gradient_subpixel_scale);
        m_fy = iround(fy * gradient_subpixel_scale);
        update_values();
    }

    int calculate(int x, int y, int) const
    {
        scalar dx = INT_TO_SCALAR(x - m_fx);
        scalar dy = INT_TO_SCALAR(y - m_fy);
        scalar d2 = dx * m_fy - dy * m_fx;
        scalar d3 = m_r2 * (dx * dx + dy * dy) - d2 * d2;
        return iround((dx * m_fx + dy * m_fy + Sqrt(Fabs(d3))) * m_mul);
    }

private:
    void update_values(void)
    {
        // Calculate the invariant values. In case the focal center
        // lies exactly on the gradient circle the divisor degenerates
        // into zero. In this case we just move the focal center by
        // one subpixel unit possibly in the direction to the origin (0,0)
        // and calculate the values again.

        m_r2 = INT_TO_SCALAR(m_r) * INT_TO_SCALAR(m_r);
        m_fx2 = INT_TO_SCALAR(m_fx) * INT_TO_SCALAR(m_fx);
        m_fy2 = INT_TO_SCALAR(m_fy) * INT_TO_SCALAR(m_fy);
        scalar d = (m_r2 - (m_fx2 + m_fy2));
        if (d == INT_TO_SCALAR(0)) {
            if (m_fx) {
                if (m_fx < 0)
                    ++m_fx;
                else
                    --m_fx;
            }

            if (m_fy) {
                if (m_fy < 0)
                    ++m_fy;
                else
                    --m_fy;
            }

            m_fx2 = INT_TO_SCALAR(m_fx) * INT_TO_SCALAR(m_fx);
            m_fy2 = INT_TO_SCALAR(m_fy) * INT_TO_SCALAR(m_fy);
            d = (m_r2 - (m_fx2 + m_fy2));
        }
        m_mul = m_r / d;
    }

private:
    int m_r;
    int m_fx;
    int m_fy;
    scalar m_r2;
    scalar m_fx2;
    scalar m_fy2;
    scalar m_mul;
};

// gradient adaptors

// gradient once adaptor
template <typename GradientFunc>
class gradient_pad_adaptor
{
public:
    gradient_pad_adaptor(const GradientFunc& gradient)
        : m_gradient(&gradient)
    {
    }

    int calculate(int x, int y, int d) const
    {
        int ret = m_gradient->calculate(x, y, d);
        if (ret < 0) ret = 0;
        if (ret > d) ret = d;
        return ret;
    }

private:
    const GradientFunc* m_gradient;
};

// gradient repeat adaptor
template <typename GradientFunc>
class gradient_repeat_adaptor
{
public:
    gradient_repeat_adaptor(const GradientFunc& gradient)
        : m_gradient(&gradient)
    {
    }

    int calculate(int x, int y, int d) const
    {
        int ret = m_gradient->calculate(x, y, d) % d;
        if (ret < 0) ret += d;
        return ret;
    }

private:
    const GradientFunc* m_gradient;
};

// gradient reflect adaptor
template <typename GradientFunc>
class gradient_reflect_adaptor
{
public:
    gradient_reflect_adaptor(const GradientFunc& gradient)
        : m_gradient(&gradient)
    {
    }

    int calculate(int x, int y, int d) const
    {
        int d2 = d << 1;
        int ret = m_gradient->calculate(x, y, d) % d2;
        if (ret < 0) ret += d2;
        if (ret >= d) ret = d2 - ret;
        return ret;
    }

private:
    const GradientFunc* m_gradient;
};


// gradient wrapper implements
template <typename GradientFunc, typename Adaptor>
class gfx_gradient : public gfx_gradient_wrapper
{
public:
    gfx_gradient()
        : m_gradient()
        , m_adaptor(m_gradient)
    {
    }

    virtual void init(scalar r, scalar x, scalar y)
    {
        m_gradient.init(r, x, y);
    }

    virtual int calculate(int x, int y, int d) const
    {
        return m_adaptor.calculate(x, y, d);
    }

private:
    GradientFunc m_gradient;
    Adaptor m_adaptor;
};

// gfx gradient table

// rgba8 color interpolator
struct color_interpolator
{
public:
    typedef rgba8 color_type;

    color_interpolator(const color_type& c1, const color_type& c2, unsigned len)
        : r(c1.r, c2.r, len)
        , g(c1.g, c2.g, len)
        , b(c1.b, c2.b, len)
        , a(c1.a, c2.a, len)
    {
    }

    void operator ++ ()
    {
        ++r; ++g; ++b; ++a;
    }

    color_type color(void) const
    {
        return color_type(r.y(), g.y(), b.y(), a.y());
    }

private:
    gfx_dda_line_interpolator<14> r;
    gfx_dda_line_interpolator<14> g;
    gfx_dda_line_interpolator<14> b;
    gfx_dda_line_interpolator<14> a;
};

void gfx_gradient_table::build_table(void)
{
    quick_sort(m_color_profile, offset_less);
    m_color_profile.cut_at(remove_duplicates(m_color_profile, offset_equal));

    if (m_color_profile.size() >= 2) {
        unsigned int i;
        unsigned int start = uround(m_color_profile[0].offset * color_table_size);
        unsigned int end = 0;
        color_type c = m_color_profile[0].color;

        for (i = 0; i < start; i++) {
            m_color_table[i] = c;
        }

        for (i = 1; i < m_color_profile.size(); i++) {
            end  = uround(m_color_profile[i].offset * color_table_size);
            color_interpolator ci(m_color_profile[i-1].color,
                                  m_color_profile[i].color, end - start + 1);
            while (start < end) {
                m_color_table[start] = ci.color();
                ++ci;
                ++start;
            }
        }

        c = m_color_profile.last().color;

        for (; end < m_color_table.size(); end++) {
            m_color_table[end] = c;
        }
    }
}

// gfx gradient adapter
void gfx_gradient_adapter::init_linear(int spread, scalar x1, scalar y1, scalar x2, scalar y2)
{
    if (!m_wrapper) {
        switch (spread) {
            case SPREAD_PAD:
                m_wrapper = new gfx_gradient<gradient_x, gradient_pad_adaptor<gradient_x> >;
                break;
            case SPREAD_REPEAT:
                m_wrapper = new gfx_gradient<gradient_x, gradient_repeat_adaptor<gradient_x> >;
                break;
            case SPREAD_REFLECT:
                m_wrapper = new gfx_gradient<gradient_x, gradient_reflect_adaptor<gradient_x> >;
                break;
        };

        if (!m_wrapper)
            return;

        scalar len = calc_distance(x1, y1, x2, y2);

        gfx_trans_affine mtx;
        if (len) {
            if (x2 < x1)
                mtx.rotate(PI - Asin((y2 - y1) / len));
            else
                mtx.rotate(Asin((y2 - y1) / len));
        } else
            len = FLT_TO_SCALAR(2.0f); // len can not be zero

        mtx.translate(x1, y1);

        m_start = FLT_TO_SCALAR(0.0f);
        m_length = len;
        m_matrix = mtx;
    }
}

void gfx_gradient_adapter::init_radial(int spread, scalar x1, scalar y1, scalar radius1,
                                           scalar x2, scalar y2, scalar radius2)
{
    if (!m_wrapper) {
        if ((x1 == x2) && (y1 == y2)) {
            switch (spread) {
                case SPREAD_PAD:
                    m_wrapper = new gfx_gradient<gradient_radial,
                                             gradient_pad_adaptor<gradient_radial> >;
                    break;
                case SPREAD_REPEAT:
                    m_wrapper = new gfx_gradient<gradient_radial,
                                             gradient_repeat_adaptor<gradient_radial> >;
                    break;
                case SPREAD_REFLECT:
                    m_wrapper = new gfx_gradient<gradient_radial,
                                             gradient_reflect_adaptor<gradient_radial> >;
                    break;
            }
        } else {
            switch (spread) {
                case SPREAD_PAD:
                    m_wrapper = new gfx_gradient<gradient_radial_focus,
                                             gradient_pad_adaptor<gradient_radial_focus> >;
                    break;
                case SPREAD_REPEAT:
                    m_wrapper = new gfx_gradient<gradient_radial_focus,
                                             gradient_repeat_adaptor<gradient_radial_focus> >;
                    break;
                case SPREAD_REFLECT:
                    m_wrapper = new gfx_gradient<gradient_radial_focus,
                                             gradient_reflect_adaptor<gradient_radial_focus> >;
                    break;
            }

        }

        if (!m_wrapper)
            return;

        scalar len = Fabs(radius2);
        scalar fx = x1 - x2;
        scalar fy = y1 - y2;

        m_wrapper->init(len, fx, fy);

        if (!len)
            len = FLT_TO_SCALAR(2.0f); // len can not be zero

        gfx_trans_affine mtx;
        mtx.translate(x1 - fx, y1 - fy);


        m_start = Fabs(radius1);
        m_length = len;
        m_matrix = mtx;
    }
}

void gfx_gradient_adapter::init_conic(int spread, scalar x, scalar y, scalar angle)
{
    if (!m_wrapper) {
        if (spread == SPREAD_REFLECT) {
            m_wrapper = new gfx_gradient<gradient_conic, gradient_reflect_adaptor<gradient_conic> >;
        } else {
            m_wrapper = new gfx_gradient<gradient_conic_vg, gradient_pad_adaptor<gradient_conic_vg> >;
        }

        if (!m_wrapper)
            return;

        gfx_trans_affine mtx;
        mtx.translate(x, y);
        mtx.translate(-x, -y);
        mtx.rotate(angle);
        mtx.translate(x, y);

        m_start = INT_TO_SCALAR(0);
        m_length = INT_TO_SCALAR(128);
        m_matrix = mtx;
    }
}

}
