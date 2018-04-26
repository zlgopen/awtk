/* Picasso - a vector graphics library
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _CURVE_H_
#define _CURVE_H_

#include "common.h"
#include "vertex.h"
#include "data_vector.h"
#include "graphic_base.h"

namespace picasso {

typedef enum {
    curve_inc,
    curve_div,
} curve_approximation_method;


//curve3_inc
class curve3_inc : public vertex_source
{
public:
    curve3_inc()
        : m_num_steps(0), m_step(0), m_scale(FLT_TO_SCALAR(1.0f))
    { }

    curve3_inc(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3)
        : m_num_steps(0), m_step(0), m_scale(FLT_TO_SCALAR(1.0f))
    {
        init(x1, y1, x2, y2, x3, y3);
    }

    void reset(void)
    {
        m_num_steps = 0;
        m_step = -1;
    }

    void init(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3);

    void approximation_method(curve_approximation_method) {}
    curve_approximation_method approximation_method(void) const { return curve_inc; }

    void approximation_scale(scalar s) { m_scale = s; }
    scalar approximation_scale(void) const { return m_scale; }

    void angle_tolerance(scalar) {}
    scalar angle_tolerance(void) const { return FLT_TO_SCALAR(0.0f); }

    void cusp_limit(scalar) {}
    scalar cusp_limit(void) const { return FLT_TO_SCALAR(0.0f); }

    virtual void rewind(unsigned int id);
    virtual unsigned int vertex(scalar* x, scalar* y);

private:
    int      m_num_steps;
    int      m_step;
    scalar   m_scale;
    scalar   m_start_x;
    scalar   m_start_y;
    scalar   m_end_x;
    scalar   m_end_y;
    scalar   m_fx;
    scalar   m_fy;
    scalar   m_dfx;
    scalar   m_dfy;
    scalar   m_ddfx;
    scalar   m_ddfy;
    scalar   m_saved_fx;
    scalar   m_saved_fy;
    scalar   m_saved_dfx;
    scalar   m_saved_dfy;
};


//curve3_div
class curve3_div : public vertex_source
{
public:
    curve3_div()
        : m_approximation_scale(FLT_TO_SCALAR(1.0f))
        , m_distance_tolerance_square(FLT_TO_SCALAR(0.0f))
        , m_angle_tolerance(FLT_TO_SCALAR(0.0f))
        , m_count(0)
    {
    }

    curve3_div(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3)
        : m_approximation_scale(FLT_TO_SCALAR(1.0f)), m_angle_tolerance(FLT_TO_SCALAR(0.0f)), m_count(0)
    {
        init(x1, y1, x2, y2, x3, y3);
    }

    void reset(void)
    {
        m_points.clear();
        m_count = 0;
    }

    void init(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3);

    void approximation_method(curve_approximation_method) {}
    curve_approximation_method approximation_method() const { return curve_div; }

    void approximation_scale(scalar s) { m_approximation_scale = s; }
    scalar approximation_scale() const { return m_approximation_scale;  }

    void angle_tolerance(scalar a) { m_angle_tolerance = a; }
    scalar angle_tolerance(void) const { return m_angle_tolerance;  }

    void cusp_limit(scalar) {}
    scalar cusp_limit(void) const { return FLT_TO_SCALAR(0.0f); }

    virtual void rewind(unsigned int id)
    {
        m_count = 0;
    }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        if (m_count >= m_points.size())
            return path_cmd_stop;

        const vertex_s& p = m_points[m_count++];
        *x = p.x;  *y = p.y;
        return (m_count == 1) ? path_cmd_move_to : path_cmd_line_to;
    }

private:
    void bezier(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3);
    void recursive_bezier(scalar x1, scalar y1, scalar x2, scalar y2,
                                        scalar x3, scalar y3, unsigned int level);

    scalar m_approximation_scale;
    scalar m_distance_tolerance_square;
    scalar m_angle_tolerance;
    unsigned int m_count;
    pod_bvector<vertex_s> m_points;
};


//curve4_inc
class curve4_inc : public vertex_source
{
public:
    curve4_inc()
        : m_num_steps(0), m_step(0), m_scale(FLT_TO_SCALAR(1.0f))
    {
    }

    curve4_inc(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3,scalar x4, scalar y4)
        : m_num_steps(0), m_step(0), m_scale(FLT_TO_SCALAR(1.0f))
    {
        init(x1, y1, x2, y2, x3, y3, x4, y4);
    }

    void reset(void)
    {
        m_num_steps = 0;
        m_step = -1;
    }

    void init(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3, scalar x4, scalar y4);

    void approximation_method(curve_approximation_method) {}
    curve_approximation_method approximation_method(void) const { return curve_inc; }

    void approximation_scale(scalar s) { m_scale = s; }
    scalar approximation_scale(void) const { return m_scale; }

    void angle_tolerance(scalar) {}
    scalar angle_tolerance(void) const { return FLT_TO_SCALAR(0.0f); }

    void cusp_limit(scalar) {}
    scalar cusp_limit(void) const { return FLT_TO_SCALAR(0.0f); }

    virtual void rewind(unsigned int id);
    virtual unsigned int vertex(scalar* x, scalar* y);

private:
    int      m_num_steps;
    int      m_step;
    scalar   m_scale;
    scalar   m_start_x;
    scalar   m_start_y;
    scalar   m_end_x;
    scalar   m_end_y;
    scalar   m_fx;
    scalar   m_fy;
    scalar   m_dfx;
    scalar   m_dfy;
    scalar   m_ddfx;
    scalar   m_ddfy;
    scalar   m_dddfx;
    scalar   m_dddfy;
    scalar   m_saved_fx;
    scalar   m_saved_fy;
    scalar   m_saved_dfx;
    scalar   m_saved_dfy;
    scalar   m_saved_ddfx;
    scalar   m_saved_ddfy;
};

//curve4_div
class curve4_div : public vertex_source
{
public:
    curve4_div()
        : m_approximation_scale(FLT_TO_SCALAR(1.0f))
        , m_distance_tolerance_square(FLT_TO_SCALAR(0.0f))
        , m_angle_tolerance(FLT_TO_SCALAR(0.0f))
        , m_cusp_limit(FLT_TO_SCALAR(0.0f))
        , m_count(0)
    {
    }

    curve4_div(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3,scalar x4, scalar y4)
        : m_approximation_scale(FLT_TO_SCALAR(1.0f))
        , m_angle_tolerance(FLT_TO_SCALAR(0.0f))
        , m_cusp_limit(FLT_TO_SCALAR(0.0f))
        , m_count(0)
    {
        init(x1, y1, x2, y2, x3, y3, x4, y4);
    }

    void reset(void)
    {
        m_points.clear();
        m_count = 0;
    }

    void init(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3, scalar x4, scalar y4);

    void approximation_method(curve_approximation_method) {}

    curve_approximation_method approximation_method() const { return curve_div; }

    void approximation_scale(scalar s) { m_approximation_scale = s; }
    scalar approximation_scale(void) const { return m_approximation_scale;  }

    void angle_tolerance(scalar a) { m_angle_tolerance = a; }
    scalar angle_tolerance(void) const { return m_angle_tolerance;  }

    void cusp_limit(scalar v)
    {
        m_cusp_limit = (v == FLT_TO_SCALAR(0.0f)) ? FLT_TO_SCALAR(0.0f) : PI - v;
    }

    scalar cusp_limit(void) const
    {
        return (m_cusp_limit == FLT_TO_SCALAR(0.0f)) ? FLT_TO_SCALAR(0.0f) : PI - m_cusp_limit;
    }

    virtual void rewind(unsigned int id)
    {
        m_count = 0;
    }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        if (m_count >= m_points.size())
            return path_cmd_stop;

        const vertex_s& p = m_points[m_count++];
        *x = p.x; *y = p.y;
        return (m_count == 1) ? path_cmd_move_to : path_cmd_line_to;
    }

private:
    void bezier(scalar x1, scalar y1,
                scalar x2, scalar y2,
                scalar x3, scalar y3,
                scalar x4, scalar y4);

    void recursive_bezier(scalar x1, scalar y1,
                          scalar x2, scalar y2,
                          scalar x3, scalar y3,
                          scalar x4, scalar y4,
                          unsigned level);

    scalar m_approximation_scale;
    scalar m_distance_tolerance_square;
    scalar m_angle_tolerance;
    scalar m_cusp_limit;
    unsigned int m_count;
    pod_bvector<vertex_s> m_points;
};

}
#endif    /*_CURVE_H_*/
