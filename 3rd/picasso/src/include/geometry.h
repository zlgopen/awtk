/* Picasso - a vector graphics library
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "common.h"
#include "vertex.h"
#include "curve.h"

namespace picasso {

// Arc geometry
class arc : public vertex_source
{
public:
    arc()
        : m_x(FLT_TO_SCALAR(0)), m_y(FLT_TO_SCALAR(0))
        , m_rx(FLT_TO_SCALAR(0)), m_ry(FLT_TO_SCALAR(0))
        , m_start(FLT_TO_SCALAR(0)), m_end(FLT_TO_SCALAR(0))
        , m_scale(FLT_TO_SCALAR(1.0f)), m_da(FLT_TO_SCALAR(0)), m_angle(FLT_TO_SCALAR(0))
        , m_path_cmd(0), m_ccw(false), m_initialized(false)
    {
    }

    arc(scalar x,  scalar y, scalar rx, scalar ry, scalar a1, scalar a2, bool ccw = true)
        : m_x(x), m_y(y), m_rx(rx), m_ry(ry)
        , m_start(FLT_TO_SCALAR(0)), m_end(FLT_TO_SCALAR(0))
        , m_scale(FLT_TO_SCALAR(1.0f)), m_da(FLT_TO_SCALAR(0)), m_angle(FLT_TO_SCALAR(0))
        , m_path_cmd(0), m_ccw(false), m_initialized(false)
    {
        normalize(a1, a2, ccw);
    }

    void init(scalar x,  scalar y, scalar rx, scalar ry, scalar a1, scalar a2, bool ccw = true)
    {
        m_x = x;  m_y = y;
        m_rx = rx; m_ry = ry;
        normalize(a1, a2, ccw);
    }

    void approximation_scale(scalar s)
    {
        m_scale = s;
        if (m_initialized) {
            normalize(m_start, m_end, m_ccw);
        }
    }

    scalar approximation_scale(void) const
    {
        return m_scale;
    }

    virtual void rewind(unsigned int id)
    {
        m_path_cmd = path_cmd_move_to;
        m_angle = m_start;
    }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        if (is_stop(m_path_cmd))
            return path_cmd_stop;

        if ((m_angle < (m_end - m_da/4)) != m_ccw) {
            *x = m_x + Cos(m_end) * m_rx;
            *y = m_y + Sin(m_end) * m_ry;
            m_path_cmd = path_cmd_stop;
            return path_cmd_line_to;
        }

        *x = m_x + Cos(m_angle) * m_rx;
        *y = m_y + Sin(m_angle) * m_ry;

        m_angle += m_da;

        unsigned int pf = m_path_cmd;
        m_path_cmd = path_cmd_line_to;
        return pf;
    }

private:
    void normalize(scalar a1, scalar a2, bool ccw)
    {
        scalar ra = (Fabs(m_rx) + Fabs(m_ry)) / 2;
        m_da = Acos(ra / (ra + FLT_TO_SCALAR(0.125f) / m_scale)) * 2;
        if (ccw) {
            while(a2 < a1) a2 += _2PI;
        } else {
            while(a1 < a2) a1 += _2PI;
            m_da = -m_da;
        }
        m_ccw   = ccw;
        m_start = a1;
        m_end   = a2;
        m_initialized = true;
    }

    scalar   m_x;
    scalar   m_y;
    scalar   m_rx;
    scalar   m_ry;
    scalar   m_start;
    scalar   m_end;
    scalar   m_scale;
    scalar   m_da;
    scalar m_angle;
    unsigned int m_path_cmd;
    bool     m_ccw;
    bool     m_initialized;
};

// Bezier arc geometry
class bezier_arc : public vertex_source
{
public:
    enum {
        vertex_max_num = 26,
    };

    bezier_arc()
        : m_vertex(vertex_max_num), m_num_vertices(0), m_cmd(path_cmd_line_to)
    {
    }

    bezier_arc(scalar x,  scalar y, scalar rx, scalar ry, scalar start_angle, scalar sweep_angle)
    {
        init(x, y, rx, ry, start_angle, sweep_angle);
    }

    void init(scalar x,  scalar y, scalar rx, scalar ry, scalar start_angle, scalar sweep_angle)
    {
        start_angle = Fmod(start_angle, _2PI);
        if (sweep_angle >= _2PI)
            sweep_angle = _2PI;

        if (sweep_angle <= -_2PI)
            sweep_angle = -_2PI;

        if (Fabs(sweep_angle) < FLT_TO_SCALAR(1e-10f)) {
            m_num_vertices = 4;
            m_cmd = path_cmd_line_to;
            m_vertices[0] = x + rx * Cos(start_angle);
            m_vertices[1] = y + ry * Sin(start_angle);
            m_vertices[2] = x + rx * Cos(start_angle + sweep_angle);
            m_vertices[3] = y + ry * Sin(start_angle + sweep_angle);
            return;
        }

        scalar total_sweep = FLT_TO_SCALAR(0.0f);
        scalar local_sweep = FLT_TO_SCALAR(0.0f);
        scalar prev_sweep;
        m_num_vertices = 2;
        m_cmd = path_cmd_curve4;
        bool done = false;
        do {
            if (sweep_angle < FLT_TO_SCALAR(0.0f)) {
                prev_sweep  = total_sweep;
                local_sweep = -_PIdiv2;
                total_sweep -= _PIdiv2;
                if (total_sweep <= sweep_angle + FLT_TO_SCALAR(0.01f)) {
                    local_sweep = sweep_angle - prev_sweep;
                    done = true;
                }
            } else {
                prev_sweep  = total_sweep;
                local_sweep =  _PIdiv2;
                total_sweep += _PIdiv2;
                if (total_sweep >= sweep_angle - FLT_TO_SCALAR(0.01f)) {
                    local_sweep = sweep_angle - prev_sweep;
                    done = true;
                }
            }

            arc_to_bezier(x, y, rx, ry, start_angle, local_sweep, m_vertices + m_num_vertices - 2);

            m_num_vertices += 6;
            start_angle += local_sweep;
        } while (!done && (m_num_vertices < vertex_max_num));
    }

    unsigned int num_vertices(void) const { return m_num_vertices; }
    const scalar* vertices(void) const { return m_vertices; }
    scalar* vertices(void) { return m_vertices; }

    virtual void rewind(unsigned int id)
    {
        m_vertex = 0;
    }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        if (m_vertex >= m_num_vertices)
            return path_cmd_stop;

        *x = m_vertices[m_vertex];
        *y = m_vertices[m_vertex + 1];
        m_vertex += 2;
        return (m_vertex == 2) ? path_cmd_move_to : m_cmd;
    }

private:
    void arc_to_bezier(scalar cx, scalar cy, scalar rx, scalar ry, scalar start_angle, scalar sweep_angle, scalar* curve)
    {
        scalar x0 = Cos(sweep_angle / FLT_TO_SCALAR(2.0f));
        scalar y0 = Sin(sweep_angle / FLT_TO_SCALAR(2.0f));
        scalar tx = (FLT_TO_SCALAR(1.0f) - x0) * FLT_TO_SCALAR(4.0f) / FLT_TO_SCALAR(3.0f);
        scalar ty = y0 - tx * x0 / y0;
        scalar px[4];
        scalar py[4];
        px[0] =  x0;
        py[0] = -y0;
        px[1] =  x0 + tx;
        py[1] = -ty;
        px[2] =  x0 + tx;
        py[2] =  ty;
        px[3] =  x0;
        py[3] =  y0;

        scalar sn = Sin(start_angle + sweep_angle / FLT_TO_SCALAR(2.0f));
        scalar cs = Cos(start_angle + sweep_angle / FLT_TO_SCALAR(2.0f));

        for (unsigned int i = 0; i < 4; i++) {
            curve[i * 2]     = cx + rx * (px[i] * cs - py[i] * sn);
            curve[i * 2 + 1] = cy + ry * (px[i] * sn + py[i] * cs);
        }
    }

    unsigned int m_vertex;
    unsigned int m_num_vertices;
    scalar        m_vertices[vertex_max_num];
    unsigned int m_cmd;
};

// Bezier arc SVG style geometry
class bezier_arc_svg : public vertex_source
{
public:
    bezier_arc_svg()
        : m_radii_ok(false)
    {
    }

    bezier_arc_svg(scalar x1, scalar y1, scalar rx, scalar ry,
            scalar angle, bool large_arc_flag, bool sweep_flag, scalar x2, scalar y2)
        : m_radii_ok(false)
    {
        init(x1, y1, rx, ry, angle, large_arc_flag, sweep_flag, x2, y2);
    }

    bool radii_ok(void) const { return m_radii_ok; }

    void init(scalar x0, scalar y0, scalar rx, scalar ry,
            scalar angle, bool large_arc_flag, bool sweep_flag, scalar x2, scalar y2)
    {
        m_radii_ok = true;

        if (rx < FLT_TO_SCALAR(0.0f)) rx = -rx;
        if (ry < FLT_TO_SCALAR(0.0f)) ry = -rx;

        // Calculate the middle point between
        // the current and the final points
        //------------------------
        scalar dx2 = (x0 - x2) / FLT_TO_SCALAR(2.0f);
        scalar dy2 = (y0 - y2) / FLT_TO_SCALAR(2.0f);

        scalar cos_a = Cos(angle);
        scalar sin_a = Sin(angle);

        // Calculate (x1, y1)
        //------------------------
        scalar x1 =  cos_a * dx2 + sin_a * dy2;
        scalar y1 = -sin_a * dx2 + cos_a * dy2;

        // Ensure radii are large enough
        //------------------------
        scalar prx = rx * rx;
        scalar pry = ry * ry;
        scalar px1 = x1 * x1;
        scalar py1 = y1 * y1;

        // Check that radii are large enough
        //------------------------
        scalar radii_check = px1/prx + py1/pry;
        if (radii_check > FLT_TO_SCALAR(1.0f)) {
            rx = Sqrt(radii_check) * rx;
            ry = Sqrt(radii_check) * ry;
            prx = rx * rx;
            pry = ry * ry;
            if (radii_check > FLT_TO_SCALAR(10.0f)) m_radii_ok = false;
        }

        // Calculate (cx1, cy1)
        //------------------------
        scalar sign = (large_arc_flag == sweep_flag) ? -FLT_TO_SCALAR(1.0f) : FLT_TO_SCALAR(1.0f);
        scalar sq   = (prx*pry - prx*py1 - pry*px1) / (prx*py1 + pry*px1);
        scalar coef = sign * Sqrt((sq < 0) ? 0 : sq);
        scalar cx1  = coef *  ((rx * y1) / ry);
        scalar cy1  = coef * -((ry * x1) / rx);

        //
        // Calculate (cx, cy) from (cx1, cy1)
        //------------------------
        scalar sx2 = (x0 + x2) / FLT_TO_SCALAR(2.0f);
        scalar sy2 = (y0 + y2) / FLT_TO_SCALAR(2.0f);
        scalar cx = sx2 + (cos_a * cx1 - sin_a * cy1);
        scalar cy = sy2 + (sin_a * cx1 + cos_a * cy1);

        // Calculate the start_angle (angle1) and the sweep_angle (dangle)
        //------------------------
        scalar ux =  (x1 - cx1) / rx;
        scalar uy =  (y1 - cy1) / ry;
        scalar vx = (-x1 - cx1) / rx;
        scalar vy = (-y1 - cy1) / ry;
        scalar p, n;

        // Calculate the angle start
        //------------------------
        n = Sqrt(ux*ux + uy*uy);
        p = ux; // (1 * ux) + (0 * uy)
        sign = (uy < 0) ? -FLT_TO_SCALAR(1.0f) : FLT_TO_SCALAR(1.0f);
        scalar v = p / n;
        if (v < -FLT_TO_SCALAR(1.0f)) v = -FLT_TO_SCALAR(1.0f);
        if (v > FLT_TO_SCALAR(1.0f)) v = FLT_TO_SCALAR(1.0f);
        scalar start_angle = sign * Acos(v);

        // Calculate the sweep angle
        //------------------------
        n = Sqrt((ux*ux + uy*uy) * (vx*vx + vy*vy));
        p = ux * vx + uy * vy;
        sign = (ux * vy - uy * vx < 0) ? -FLT_TO_SCALAR(1.0f) : FLT_TO_SCALAR(1.0f);
        v = p / n;
        if (v < -FLT_TO_SCALAR(1.0f)) v = -FLT_TO_SCALAR(1.0f);
        if (v > FLT_TO_SCALAR(1.0f)) v = FLT_TO_SCALAR(1.0f);
        scalar sweep_angle = sign * Acos(v);
        if (!sweep_flag && sweep_angle > 0) {
            sweep_angle -= _2PI;
        } else {
            if (sweep_flag && sweep_angle < 0) {
                sweep_angle += _2PI;
            }
        }

        // We can now build and transform the resulting arc
        //------------------------
        m_arc.init(FLT_TO_SCALAR(0.0f), FLT_TO_SCALAR(0.0f), rx, ry, start_angle, sweep_angle);

        scalar sx = Cos(angle);
        scalar shy = Sin(angle);
        scalar shx = -Sin(angle);
        scalar sy = Cos(angle);
           scalar tx = cx;
        scalar ty = cy;

        for (unsigned i = 2; i < m_arc.num_vertices()-2; i += 2) {
            scalar *x = m_arc.vertices() + i;
            scalar *y = m_arc.vertices() + i + 1;

            // matrix transform
            scalar tmp = *x;
            *x = tmp * sx  + *y * shx + tx;
            *y = tmp * shy + *y * sy  + ty;
        }

        // We must make sure that the starting and ending points
        // exactly coincide with the initial (x0,y0) and (x2,y2)
        m_arc.vertices()[0] = x0;
        m_arc.vertices()[1] = y0;
        if (m_arc.num_vertices() > 2) {
            m_arc.vertices()[m_arc.num_vertices() - 2] = x2;
            m_arc.vertices()[m_arc.num_vertices() - 1] = y2;
        }
    }

    unsigned int num_vertices(void) const { return m_arc.num_vertices(); }
    const scalar* vertices(void) const { return m_arc.vertices(); }
    scalar* vertices(void) { return m_arc.vertices(); }

    virtual void rewind(unsigned int id)
    {
        m_arc.rewind(0);
    }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        return m_arc.vertex(x, y);
    }

private:
    bezier_arc m_arc;
    bool       m_radii_ok;
};

// Rounded rectangle geometry
class rounded_rect : public vertex_source
{
public:
    rounded_rect()
        : m_x1(0), m_y1(0), m_x2(0), m_y2(0)
        , m_rx1(0), m_ry1(0), m_rx2(0), m_ry2(0)
        , m_rx3(0), m_ry3(0), m_rx4(0), m_ry4(0)
        , m_status(0)
    {
    }

    rounded_rect(scalar x1, scalar y1, scalar x2, scalar y2, scalar r)
        : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
        , m_rx1(r), m_ry1(r), m_rx2(r), m_ry2(r)
        , m_rx3(r), m_ry3(r), m_rx4(r), m_ry4(r)
        , m_status(0)
    {
        if (x1 > x2) {
            m_x1 = x2;
            m_x2 = x1;
        }

        if (y1 > y2) {
            m_y1 = y2;
            m_y2 = y1;
        }
    }

    void rect(scalar x1, scalar y1, scalar x2, scalar y2)
    {
        m_x1 = x1;
        m_y1 = y1;
        m_x2 = x2;
        m_y2 = y2;

        if (x1 > x2) {
            m_x1 = x2;
            m_x2 = x1;
        }

        if (y1 > y2) {
            m_y1 = y2;
            m_y2 = y1;
        }
    }

    void radius(scalar rx1, scalar ry1, scalar rx2, scalar ry2,
                scalar rx3, scalar ry3, scalar rx4, scalar ry4)
    {
        m_rx1 = rx1; m_ry1 = ry1; m_rx2 = rx2; m_ry2 = ry2;
        m_rx3 = rx3; m_ry3 = ry3; m_rx4 = rx4; m_ry4 = ry4;
    }

    void normalize_radius(void)
    {
        scalar dx = Fabs(m_y2 - m_y1);
        scalar dy = Fabs(m_x2 - m_x1);

        scalar k = FLT_TO_SCALAR(1.0f);
        scalar t;
        t = dx / (m_rx1 + m_rx2); if(t < k) k = t;
        t = dx / (m_rx3 + m_rx4); if(t < k) k = t;
        t = dy / (m_ry1 + m_ry2); if(t < k) k = t;
        t = dy / (m_ry3 + m_ry4); if(t < k) k = t;

        if (k < FLT_TO_SCALAR(1.0f)) {
            m_rx1 *= k; m_ry1 *= k; m_rx2 *= k; m_ry2 *= k;
            m_rx3 *= k; m_ry3 *= k; m_rx4 *= k; m_ry4 *= k;
        }
    }

    void approximation_scale(scalar s)
    {
        m_arc.approximation_scale(s);
    }

    scalar approximation_scale(void) const
    {
        return m_arc.approximation_scale();
    }

    virtual void rewind(unsigned int id)
    {
        m_status = 0;
    }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        unsigned int cmd = path_cmd_stop;
        switch(m_status)
        {
            case 0:
                m_arc.init(m_x1 + m_rx1, m_y1 + m_ry1, m_rx1, m_ry1, PI, PI+_PIdiv2);
                m_arc.rewind(0);
                m_status++;

            case 1:
                cmd = m_arc.vertex(x, y);
                if (is_stop(cmd))
                    m_status++;
                else
                    return cmd;

            case 2:
                m_arc.init(m_x2 - m_rx2, m_y1 + m_ry2, m_rx2, m_ry2, PI+_PIdiv2, FLT_TO_SCALAR(0.0f));
                m_arc.rewind(0);
                m_status++;

            case 3:
                cmd = m_arc.vertex(x, y);
                if (is_stop(cmd))
                    m_status++;
                else
                    return path_cmd_line_to;

            case 4:
                m_arc.init(m_x2 - m_rx3, m_y2 - m_ry3, m_rx3, m_ry3, FLT_TO_SCALAR(0.0f), _PIdiv2);
                m_arc.rewind(0);
                m_status++;

            case 5:
                cmd = m_arc.vertex(x, y);
                if (is_stop(cmd))
                    m_status++;
                else
                    return path_cmd_line_to;

            case 6:
                m_arc.init(m_x1 + m_rx4, m_y2 - m_ry4, m_rx4, m_ry4, _PIdiv2, PI);
                m_arc.rewind(0);
                m_status++;

            case 7:
                cmd = m_arc.vertex(x, y);
                if (is_stop(cmd))
                    m_status++;
                else
                    return path_cmd_line_to;

            case 8:
                cmd = path_cmd_end_poly | path_flags_close | path_flags_ccw;
                m_status++;
                break;
        }
        return cmd;
    }

private:
    scalar m_x1;
    scalar m_y1;
    scalar m_x2;
    scalar m_y2;
    scalar m_rx1;
    scalar m_ry1;
    scalar m_rx2;
    scalar m_ry2;
    scalar m_rx3;
    scalar m_ry3;
    scalar m_rx4;
    scalar m_ry4;
    unsigned int m_status;
    arc m_arc;
};

// Ellipse geometry
class ellipse : public vertex_source
{
public:
    ellipse()
        : m_x(FLT_TO_SCALAR(0.0f)), m_y(FLT_TO_SCALAR(0.0f)), m_rx(FLT_TO_SCALAR(1.0f)), m_ry(FLT_TO_SCALAR(1.0f))
        , m_scale(FLT_TO_SCALAR(1.0f)), m_num(4), m_step(0), m_cw(false)
    {
    }

    ellipse(scalar x, scalar y, scalar rx, scalar ry, unsigned int num_steps = 0, bool cw = false)
        : m_x(x), m_y(y), m_rx(rx), m_ry(ry), m_scale(FLT_TO_SCALAR(1.0f))
        , m_num(num_steps), m_step(0), m_cw(cw)
    {
        if (0 == m_num)
            calc_num_steps();
    }

    void init(scalar x, scalar y, scalar rx, scalar ry, unsigned int num_steps = 0, bool cw = false)
    {
        m_x = x;
        m_y = y;
        m_rx = rx;
        m_ry = ry;
        m_num = num_steps;
        m_step = 0;
        m_cw = cw;
        if (0 == m_num)
            calc_num_steps();
    }

    void approximation_scale(scalar scale)
    {
        m_scale = scale;
        calc_num_steps();
    }

    scalar approximation_scale(void) const
    {
        return m_scale;
    }

    virtual void rewind(unsigned int id)
    {
        m_step = 0;
    }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        if (m_step == m_num) {
            ++m_step;
            return path_cmd_end_poly | path_flags_close | path_flags_ccw;
        }

        if (m_step > m_num)
            return path_cmd_stop;

        scalar angle = scalar(m_step) / scalar(m_num) * _2PI;

        if (m_cw)
            angle = _2PI - angle;

        *x = m_x + Cos(angle) * m_rx;
        *y = m_y + Sin(angle) * m_ry;
        m_step++;
        return ((m_step == 1) ? path_cmd_move_to : path_cmd_line_to);
    }

private:
    void calc_num_steps(void)
    {
        scalar ra = (Fabs(m_rx) + Fabs(m_ry)) / 2;
        scalar da = Acos(ra / (ra + FLT_TO_SCALAR(0.125f) / m_scale)) * 2;
        m_num = uround(_2PI / da);
    }

    scalar m_x;
    scalar m_y;
    scalar m_rx;
    scalar m_ry;
    scalar m_scale;
    unsigned int m_num;
    unsigned int m_step;
    bool m_cw;
};

// Curve3 geometry
class curve3 : public vertex_source
{
public:
    curve3() : m_approximation_method(curve_div) {}
    curve3(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3)
        : m_approximation_method(curve_div)
    {
        init(x1, y1, x2, y2, x3, y3);
    }

    void reset(void)
    {
        m_curve_inc.reset();
        m_curve_div.reset();
    }

    void init(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3)
    {
        if (m_approximation_method == curve_inc) {
            m_curve_inc.init(x1, y1, x2, y2, x3, y3);
        } else {
            m_curve_div.init(x1, y1, x2, y2, x3, y3);
        }
    }

    void approximation_method(curve_approximation_method v)
    {
        m_approximation_method = v;
    }

    curve_approximation_method approximation_method(void) const
    {
        return m_approximation_method;
    }

    void approximation_scale(scalar s)
    {
        m_curve_inc.approximation_scale(s);
        m_curve_div.approximation_scale(s);
    }

    scalar approximation_scale(void) const
    {
           return m_curve_inc.approximation_scale();
    }

    void angle_tolerance(scalar v)
    {
        m_curve_div.angle_tolerance(v);
    }

    scalar angle_tolerance() const
    {
        return m_curve_div.angle_tolerance();
    }

    void cusp_limit(scalar v)
    {
        m_curve_div.cusp_limit(v);
    }

    scalar cusp_limit() const
    {
        return m_curve_div.cusp_limit();
    }

    virtual void rewind(unsigned int id)
    {
        if (m_approximation_method == curve_inc) {
            m_curve_inc.rewind(id);
        } else {
            m_curve_div.rewind(id);
        }
    }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        if (m_approximation_method == curve_inc) {
            return m_curve_inc.vertex(x, y);
        }
        return m_curve_div.vertex(x, y);
    }

private:
    curve3_inc m_curve_inc;
    curve3_div m_curve_div;
    curve_approximation_method m_approximation_method;
};


// Curve4 geometry
class curve4 : public vertex_source
{
public:
    curve4() : m_approximation_method(curve_div) {}

    curve4(scalar x1, scalar y1, scalar x2, scalar y2,
                scalar x3, scalar y3, scalar x4, scalar y4)
        : m_approximation_method(curve_div)
    {
        init(x1, y1, x2, y2, x3, y3, x4, y4);
    }

    void reset(void)
    {
        m_curve_inc.reset();
        m_curve_div.reset();
    }

    void init(scalar x1, scalar y1, scalar x2, scalar y2,
                            scalar x3, scalar y3, scalar x4, scalar y4)
    {
        if (m_approximation_method == curve_inc) {
            m_curve_inc.init(x1, y1, x2, y2, x3, y3, x4, y4);
        } else {
            m_curve_div.init(x1, y1, x2, y2, x3, y3, x4, y4);
        }
    }

    void approximation_method(curve_approximation_method v)
    {
        m_approximation_method = v;
    }

    curve_approximation_method approximation_method(void) const
    {
        return m_approximation_method;
    }

    void approximation_scale(scalar s)
    {
        m_curve_inc.approximation_scale(s);
        m_curve_div.approximation_scale(s);
    }

    scalar approximation_scale(void) const
    {
           return m_curve_inc.approximation_scale();
    }

    void angle_tolerance(scalar v)
    {
        m_curve_div.angle_tolerance(v);
    }

    scalar angle_tolerance() const
    {
        return m_curve_div.angle_tolerance();
    }

    void cusp_limit(scalar v)
    {
        m_curve_div.cusp_limit(v);
    }

    scalar cusp_limit() const
    {
        return m_curve_div.cusp_limit();
    }

    virtual void rewind(unsigned int id)
    {
        if (m_approximation_method == curve_inc) {
            m_curve_inc.rewind(id);
        } else {
            m_curve_div.rewind(id);
        }
    }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        if (m_approximation_method == curve_inc) {
            return m_curve_inc.vertex(x, y);
        }
        return m_curve_div.vertex(x, y);
    }

private:
    curve4_inc m_curve_inc;
    curve4_div m_curve_div;
    curve_approximation_method m_approximation_method;
};

}

#endif /*_GEOMETRY_H_*/

