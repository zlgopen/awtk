/* Picasso - a vector graphics library
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _CONVERT_H_
#define _CONVERT_H_

#include "common.h"
#include "clipper.h"
#include "vertex.h"
#include "vertex_dist.h"
#include "data_vector.h"
#include "geometry.h"
#include "graphic_base.h"
#include "interfaces.h"
#include "picasso_matrix.h"

namespace picasso {

// Convert curve
class conv_curve : public vertex_source
{
public:
    conv_curve(const vertex_source& v)
        : m_source(const_cast<vertex_source*>(&v))
    {
    }

    void approximation_scale(scalar s)
    {
        m_curve3.approximation_scale(s);
        m_curve4.approximation_scale(s);
    }

    scalar approximation_scale(void) const
    {
        return m_curve4.approximation_scale();
    }

    virtual void rewind(unsigned int id)
    {
        m_source->rewind(id);
        m_last_x = FLT_TO_SCALAR(0.0f);
        m_last_y = FLT_TO_SCALAR(0.0f);
        m_curve3.reset();
        m_curve4.reset();
    }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        if (!is_stop(m_curve3.vertex(x, y))) {
            m_last_x = *x;
            m_last_y = *y;
            return path_cmd_line_to;
        }

        if (!is_stop(m_curve4.vertex(x, y))) {
            m_last_x = *x;
            m_last_y = *y;
            return path_cmd_line_to;
        }

        scalar ct2_x = FLT_TO_SCALAR(0.0f);
        scalar ct2_y = FLT_TO_SCALAR(0.0f);
        scalar end_x = FLT_TO_SCALAR(0.0f);
        scalar end_y = FLT_TO_SCALAR(0.0f);

        unsigned int cmd = m_source->vertex(x, y);
        switch(cmd) {
            case path_cmd_curve3:
                m_source->vertex(&end_x, &end_y);
                m_curve3.init(m_last_x, m_last_y, *x, *y, end_x, end_y);
                m_curve3.vertex(x, y);
                m_curve3.vertex(x, y);
                cmd = path_cmd_line_to;
                break;
            case path_cmd_curve4:
                m_source->vertex(&ct2_x, &ct2_y);
                m_source->vertex(&end_x, &end_y);
                m_curve4.init(m_last_x, m_last_y, *x, *y, ct2_x, ct2_y, end_x, end_y);
                m_curve4.vertex(x, y);
                m_curve4.vertex(x, y);
                cmd = path_cmd_line_to;
                break;
        }
        m_last_x = *x;
        m_last_y = *y;
        return cmd;
    }

private:
    conv_curve(const conv_curve&);
    conv_curve& operator=(const conv_curve&);

    vertex_source* m_source;
    scalar m_last_x;
    scalar m_last_y;
    curve3 m_curve3;
    curve4 m_curve4;
};

// Convert transformer
class conv_transform : public vertex_source
{
public:
    conv_transform(const vertex_source& v, const trans_affine& m)
        : m_source(const_cast<vertex_source*>(&v)), m_trans(m.impl())
    {
    }

    conv_transform(const vertex_source& v, const abstract_trans_affine* m)
        : m_source(const_cast<vertex_source*>(&v)), m_trans(m)
    {
    }

    void transformer(const trans_affine& t) { m_trans = t.impl(); }

    virtual void rewind(unsigned int id) { m_source->rewind(id); }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        unsigned int cmd = m_source->vertex(x, y);
        if (is_vertex(cmd)) {
            m_trans->transform(x, y);
        }
        return cmd;
    }
private:
    conv_transform(const conv_transform&);
    conv_transform& operator=(const conv_transform&);

    vertex_source* m_source;
    const abstract_trans_affine* m_trans;
};

// Convert clipper
class conv_clipper : public vertex_source
{
public:
    typedef enum {
        clip_union,
        clip_intersect,
        clip_xor,
        clip_diff,
    } clip_op;

    typedef enum {
        status_move_to,
        status_line_to,
        status_stop,
    } status;

    typedef struct {
        int num_vertices;
        vertex_s* vertices;
    } contour_header;

    conv_clipper(const vertex_source& a, const vertex_source& b, clip_op op)
        : m_src_a(const_cast<vertex_source*>(&a))
        , m_src_b(const_cast<vertex_source*>(&b))
        , m_status(status_move_to)
        , m_vertex(-1)
        , m_contour(-1)
        , m_operation(op)
    {
        memset(&m_poly_a, 0, sizeof(m_poly_a));
        memset(&m_poly_b, 0, sizeof(m_poly_b));
        memset(&m_result, 0, sizeof(m_result));
    }

    virtual ~conv_clipper()
    {
        free_all();
    }

    virtual void rewind(unsigned int id)
    {
        free_result();
        m_src_a->rewind(id);
        m_src_b->rewind(id);
        add(m_poly_a, *m_src_a);
        add(m_poly_b, *m_src_b);

        switch(m_operation)
        {
           case clip_union:
               polygon_clip(POLY_UNION, &m_poly_a, &m_poly_b, &m_result);
               break;
           case clip_intersect:
               polygon_clip(POLY_INTERSECT, &m_poly_a, &m_poly_b, &m_result);
               break;
           case clip_xor:
               polygon_clip(POLY_XOR, &m_poly_a, &m_poly_b, &m_result);
               break;
           case clip_diff:
               polygon_clip(POLY_DIFF, &m_poly_a, &m_poly_b, &m_result);
               break;
        }

        m_status = status_move_to;
        m_contour = -1;
        m_vertex = -1;
    }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        if (m_status == status_move_to) {
            if (next_contour()) {
                if (next_vertex(x, y)) {
                    m_status = status_line_to;
                    return path_cmd_move_to;
                }
                m_status = status_stop;
                return path_cmd_end_poly | path_flags_close;
            }
        } else {
            if (next_vertex(x, y)) {
                return path_cmd_line_to;
            } else {
                m_status = status_move_to;
            }
            return path_cmd_end_poly | path_flags_close;
        }
        return path_cmd_stop;
    }
private:
    void free_result(void)
    {
        if (m_result.contour) {
            free_polygon(&m_result);
        }
        memset(&m_result, 0, sizeof(m_result));
    }

    void free_all(void)
    {
        clear_polygon(m_poly_a);
        clear_polygon(m_poly_b);
        free_result();
    }

    void clear_polygon(polygon& p)
    {
        for (int i = 0; i < p.num_contours; i++) {
            pod_allocator<vertex_s>::deallocate(p.contour[i].vertex,
                                                  p.contour[i].num_vertices);
        }
        pod_allocator<vertex_list>::deallocate(p.contour, p.num_contours);
        memset(&p, 0, sizeof(polygon));
    }

    bool next_contour(void)
    {
        if (++m_contour < m_result.num_contours) {
            m_vertex = -1;
            return true;
        }
        return false;
    }

    bool next_vertex(scalar* x, scalar* y)
    {
        const vertex_list& vlist = m_result.contour[m_contour];
        if (++m_vertex < vlist.num_vertices) {
            const vertex_s& v = vlist.vertex[m_vertex];
            *x = v.x;
            *y = v.y;
            return true;
        }
        return false;
    }

    void add(polygon& p, vertex_source& src)
    {
        unsigned int cmd;
        scalar x, y;
        scalar start_x = FLT_TO_SCALAR(0.0f);
        scalar start_y = FLT_TO_SCALAR(0.0f);
        bool line_to = false;
        unsigned int orientation = 0;

        m_contour_accumulator.clear();

        while(!is_stop(cmd = src.vertex(&x, &y))) {
            if (is_vertex(cmd)) {
                if (is_move_to(cmd)) {
                    if (line_to) {
                        end_contour(orientation);
                        orientation = 0;
                    }
                    start_contour();
                    start_x = x;
                    start_y = y;
                }
                add_vertex(x, y);
                line_to = true;
            } else {
                if (is_end_poly(cmd)) {
                    orientation = get_orientation(cmd);
                    if (line_to && is_closed(cmd)) {
                        add_vertex(start_x, start_y);
                    }
                }
            }
        }

        if (line_to) {
            end_contour(orientation);
        }
        make_polygon(p);
    }

    void add_vertex(scalar x, scalar y)
    {
        vertex_s v;
        v.x = x;
        v.y = y;
        m_vertex_accumulator.add(v);
    }

    void start_contour(void)
    {
        contour_header h;
        memset(&h, 0, sizeof(h));
        m_contour_accumulator.add(h);
        m_vertex_accumulator.clear();
    }

    void end_contour(unsigned int orientation)
    {
        if (m_contour_accumulator.size()) {
            if (m_vertex_accumulator.size() > 2) {
                contour_header& h = m_contour_accumulator[m_contour_accumulator.size() - 1];

                h.num_vertices = m_vertex_accumulator.size();
                h.vertices = pod_allocator<vertex_s>::allocate(h.num_vertices);
                vertex_s* d = h.vertices;
                for (int i = 0; i < h.num_vertices; i++) {
                    const vertex_s& s = m_vertex_accumulator[i];
                    d->x = s.x;
                    d->y = s.y;
                    ++d;
                }
            } else {
                m_vertex_accumulator.remove_last();
            }
        }
    }

    void make_polygon(polygon& p)
    {
        clear_polygon(p);
        if (m_contour_accumulator.size()) {
            p.num_contours = m_contour_accumulator.size();
            p.contour = pod_allocator<vertex_list>::allocate(p.num_contours);

            vertex_list* pv = p.contour;
            for (int i = 0; i < p.num_contours; i++) {
                const contour_header& h = m_contour_accumulator[i];
                pv->num_vertices = h.num_vertices;
                pv->vertex = h.vertices;
                ++pv;
            }
        }
    }
private:
    conv_clipper(const conv_clipper&);
    conv_clipper& operator=(const conv_clipper&);

    vertex_source* m_src_a;
    vertex_source* m_src_b;
    status m_status;
    int m_vertex;
    int m_contour;
    clip_op m_operation;
    pod_bvector<vertex_s> m_vertex_accumulator;
    pod_bvector<contour_header> m_contour_accumulator;
    polygon m_poly_a;
    polygon m_poly_b;
    polygon m_result;
};

// Convert line generator abstract
class conv_line_generator : public vertex_container
{
public:
    typedef enum {
        status_initial,
        status_accumulate,
        status_generate
    } status;

    conv_line_generator(const vertex_source& v)
        : m_source(const_cast<vertex_source*>(&v))
        , m_status(status_initial)
        , m_last_cmd(path_cmd_stop)
        , m_start_x(FLT_TO_SCALAR(0.0f))
        , m_start_y(FLT_TO_SCALAR(0.0f))
    {
    }

    virtual void rewind(unsigned int id)
    {
        m_source->rewind(id);
        m_status = status_initial;
    }

    virtual unsigned int vertex(scalar* x, scalar* y)
    {
        unsigned int cmd = path_cmd_stop;
        bool done = false;
        while (!done)
        {
            switch(m_status)
            {
            case status_initial:
                m_last_cmd = m_source->vertex(&m_start_x, &m_start_y);
                m_status = status_accumulate;

            case status_accumulate:
                if (is_stop(m_last_cmd))
                    return path_cmd_stop;

                remove_all();
                add_vertex(m_start_x, m_start_y, path_cmd_move_to);

                while (true)
                {
                    cmd = m_source->vertex(x, y);
                    if (is_vertex(cmd)) {
                        m_last_cmd = cmd;
                        if (is_move_to(cmd)) {
                            m_start_x = *x;
                            m_start_y = *y;
                            break;
                        }
                        add_vertex(*x, *y, cmd);
                    } else {
                        if (is_stop(cmd)) {
                            m_last_cmd = path_cmd_stop;
                            break;
                        }

                        if (is_end_poly(cmd)) {
                            add_vertex(*x, *y, cmd);
                            break;
                        }
                    }
                }
                reset();
                m_status = status_generate;

            case status_generate:
                cmd = get_vertex(x, y);
                if (is_stop(cmd)) {
                    m_status = status_accumulate;
                    break;
                }
                done = true;
                break;
            }
        }
        return cmd;
    }

protected:
    virtual void reset(void) = 0;
    virtual unsigned int get_vertex(scalar* x, scalar* y) = 0;

private:
    conv_line_generator(const conv_line_generator&);
    conv_line_generator& operator=(const conv_line_generator&);

    vertex_source* m_source;
    status m_status;
    unsigned int m_last_cmd;
    scalar m_start_x;
    scalar m_start_y;
};

// Convert dash
class conv_dash : public conv_line_generator
{
public:
    typedef enum {
        initial,
        ready,
        polyline,
        stop,
    } status;

    enum { max_dashes = 32 };

    conv_dash(const vertex_source& v)
        : conv_line_generator(v)
        , m_total_dash_len(FLT_TO_SCALAR(0.0f))
        , m_num_dashes(0)
        , m_dash_start(FLT_TO_SCALAR(0.0f))
        , m_shorten(FLT_TO_SCALAR(0.0f))
        , m_curr_dash_start(FLT_TO_SCALAR(0.0f))
        , m_curr_rest(FLT_TO_SCALAR(0.0f))
        , m_curr_dash(0)
        , m_v1(0)
        , m_v2(0)
        , m_status(initial)
        , m_closed(0)
        , m_src_vertex(0)
    {
        memset(m_dashes, 0, max_dashes);
    }

    void dash_start(scalar start)
    {
        m_dash_start = start;
        calc_dash_start(Fabs(start));
    }

    void add_dash(scalar dash_len, scalar gap_len)
    {
        if (m_num_dashes < max_dashes) {
            m_total_dash_len += dash_len + gap_len;
            m_dashes[m_num_dashes++] = dash_len;
            m_dashes[m_num_dashes++] = gap_len;
        }
    }

public:
    virtual void reset(void)
    {
        if (m_status == initial) {
            m_src_vertices.close(m_closed != 0);
            shorten_path(m_src_vertices, m_shorten, m_closed);
        }
        m_status = ready;
        m_src_vertex = 0;
    }

    virtual unsigned int get_vertex(scalar* x, scalar* y)
    {
        unsigned int cmd = path_cmd_move_to;
        while (!is_stop(cmd))
        {
            switch(m_status)
            {
            case initial:
                 reset();

            case ready:
                if (m_num_dashes < 2 || m_src_vertices.size() < 2) {
                    cmd = path_cmd_stop;
                    break;
                }

                m_status = polyline;
                m_src_vertex = 1;
                m_v1 = &m_src_vertices[0];
                m_v2 = &m_src_vertices[1];
                m_curr_rest = m_v1->d;
                *x = m_v1->x;
                *y = m_v1->y;

                if (m_dash_start >= FLT_TO_SCALAR(0.0f))
                    calc_dash_start(m_dash_start);

                return path_cmd_move_to;

            case polyline:
                {
                    scalar dash_rest = m_dashes[m_curr_dash] - m_curr_dash_start;

                    unsigned int cmd = (m_curr_dash & 1) ? path_cmd_move_to : path_cmd_line_to;

                    if (m_curr_rest > dash_rest) {
                        m_curr_rest -= dash_rest;
                        ++m_curr_dash;

                        if (m_curr_dash >= m_num_dashes)
                            m_curr_dash = 0;

                        m_curr_dash_start = FLT_TO_SCALAR(0.0f);
                        *x = m_v2->x - (m_v2->x - m_v1->x) * m_curr_rest / m_v1->d;
                        *y = m_v2->y - (m_v2->y - m_v1->y) * m_curr_rest / m_v1->d;
                    } else {
                        m_curr_dash_start += m_curr_rest;
                        *x = m_v2->x;
                        *y = m_v2->y;
                        ++m_src_vertex;
                        m_v1 = m_v2;
                        m_curr_rest = m_v1->d;

                        if (m_closed) {
                            if (m_src_vertex > m_src_vertices.size()) {
                                m_status = stop;
                            } else {
                                m_v2 = &m_src_vertices
                                [(m_src_vertex >= m_src_vertices.size()) ? 0 : m_src_vertex];
                            }
                        } else {
                            if (m_src_vertex >= m_src_vertices.size()) {
                                m_status = stop;
                            } else {
                                m_v2 = &m_src_vertices[m_src_vertex];
                            }
                        }
                    }
                    return cmd;
                }
                break;
            case stop:
                cmd = path_cmd_stop;
                break;
            }
        }
        return path_cmd_stop;
    }

    virtual void remove_all(void)
    {
        m_status = initial;
        m_src_vertices.clear();
        m_closed = 0;
    }

    virtual void add_vertex(scalar x, scalar y, unsigned int cmd)
    {
        m_status = initial;
        if (is_move_to(cmd)) {
            m_src_vertices.modify_last(vertex_dist(x, y));
        } else {
            if (is_vertex(cmd)) {
                m_src_vertices.add(vertex_dist(x, y));
            } else {
                m_closed = get_close_flag(cmd);
            }
        }
    }

private:
    void calc_dash_start(scalar ds)
    {
        m_curr_dash = 0;
        m_curr_dash_start = FLT_TO_SCALAR(0.0f);

        while (ds > FLT_TO_SCALAR(0.0f))
        {
            if (ds > m_dashes[m_curr_dash]) {
                ds -= m_dashes[m_curr_dash];
                ++m_curr_dash;
                m_curr_dash_start = FLT_TO_SCALAR(0.0f);

                if (m_curr_dash >= m_num_dashes)
                    m_curr_dash = 0;
            } else {
                m_curr_dash_start = ds;
                ds = FLT_TO_SCALAR(0.0f);
            }
        }
    }

private:
    conv_dash(const conv_dash&);
    conv_dash& operator=(const conv_dash&);

    typedef vertex_sequence<vertex_dist, 6> vertex_storage;

    // dash property
    scalar m_dashes[max_dashes];
    scalar m_total_dash_len;
    unsigned int m_num_dashes;
    scalar m_dash_start;
    scalar m_shorten;
    scalar m_curr_dash_start;
    scalar m_curr_rest;
    unsigned int m_curr_dash;
    const vertex_dist* m_v1;
    const vertex_dist* m_v2;

    // dash status
    vertex_storage m_src_vertices;
    status m_status;
    unsigned int m_closed;
    unsigned int m_src_vertex;
};

// Convert dash
class conv_stroke : public conv_line_generator
{
public:
    typedef enum {
        initial,
        ready,
        cap1,
        cap2,
        outline1,
        close_first,
        outline2,
        out_vertices,
        end_poly1,
        end_poly2,
        stop
    } status;

    conv_stroke(const vertex_source& v)
        : conv_line_generator(v)
        , m_width(FLT_TO_SCALAR(0.5f))
        , m_width_abs(FLT_TO_SCALAR(0.5f))
        , m_width_eps(FLT_TO_SCALAR(0.5f/1024.0f))
        , m_width_sign(1)
        , m_miter_limit(FLT_TO_SCALAR(4.0f))
        , m_inner_miter_limit(FLT_TO_SCALAR(1.01f))
        , m_approx_scale(FLT_TO_SCALAR(1.0f))
        , m_line_cap(butt_cap)
        , m_line_join(miter_join)
        , m_inner_join(inner_miter)
        , m_shorten(FLT_TO_SCALAR(0.0f))
        , m_status(initial)
        , m_prev_status(initial)
        , m_closed(0)
        , m_src_vertex(0)
        , m_out_vertex(0)
    {
    }

    void set_line_cap(line_cap lc) { m_line_cap = lc; }
    void set_line_join(line_join lj) { m_line_join = lj; }
    void set_inner_join(inner_join ij) { m_inner_join = ij; }

    line_cap get_line_cap(void) const { return m_line_cap; }
    line_join get_line_join(void) const { return m_line_join; }
    inner_join get_inner_join(void) const { return m_inner_join; }

    void set_miter_limit(scalar ml) { m_miter_limit = ml; }
    void set_inner_miter_limit(scalar ml) { m_inner_miter_limit = ml; }

    void set_width(scalar w)
    {
        m_width = w * FLT_TO_SCALAR(0.5f);
        if (m_width < 0) {
            m_width_abs  = -m_width;
            m_width_sign = -1;
        } else {
            m_width_abs  = m_width;
            m_width_sign = 1;
        }
        m_width_eps = m_width / FLT_TO_SCALAR(1024.0f);
    }

    void set_miter_limit_theta(scalar t)
    {
        m_miter_limit = FLT_TO_SCALAR(1.0f) / Sin(t * FLT_TO_SCALAR(0.5f));
    }

    scalar get_width(void) const { return m_width * FLT_TO_SCALAR(2.0f); }
    scalar get_miter_limit(void) const { return m_miter_limit; }
    scalar get_inner_miter_limit(void) const { return m_inner_miter_limit; }

public:
    virtual void remove_all(void)
    {
        m_src_vertices.clear();
        m_status = initial;
        m_closed = 0;
    }

    virtual void add_vertex(scalar x, scalar y, unsigned int cmd)
    {
        m_status = initial;

        if (is_move_to(cmd)) {
            m_src_vertices.modify_last(vertex_dist(x, y));
        } else {
            if (is_vertex(cmd)) {
                m_src_vertices.add(vertex_dist(x, y));
            } else {
                m_closed = get_close_flag(cmd);
            }
        }
    }

    virtual void reset(void)
    {
        if (m_status == initial) {
            m_src_vertices.close(m_closed != 0);
            shorten_path(m_src_vertices, m_shorten, m_closed);

            if (m_src_vertices.size() < 3)
                m_closed = 0;
        }

        m_status = ready;
        m_src_vertex = 0;
        m_out_vertex = 0;
    }

    virtual unsigned int get_vertex(scalar* x, scalar* y)
    {
        unsigned int cmd = path_cmd_line_to;
        while (!is_stop(cmd))
        {
            switch(m_status)
            {
            case initial:
                reset();

            case ready:
                if (m_src_vertices.size() < 2 + (unsigned int)(m_closed != 0)) {
                    cmd = path_cmd_stop;
                    break;
                }
                m_status = m_closed ? outline1 : cap1;
                cmd = path_cmd_move_to;
                m_src_vertex = 0;
                m_out_vertex = 0;
                break;
            case cap1:
                calc_cap(m_out_vertices,
                         m_src_vertices[0],
                         m_src_vertices[1],
                         m_src_vertices[0].d);

                m_src_vertex = 1;
                m_prev_status = outline1;
                m_status = out_vertices;
                m_out_vertex = 0;
                break;
            case cap2:
                calc_cap(m_out_vertices,
                         m_src_vertices[m_src_vertices.size() - 1],
                         m_src_vertices[m_src_vertices.size() - 2],
                         m_src_vertices[m_src_vertices.size() - 2].d);

                m_prev_status = outline2;
                m_status = out_vertices;
                m_out_vertex = 0;
                break;
            case outline1:
                if (m_closed) {
                    if (m_src_vertex >= m_src_vertices.size()) {
                        m_prev_status = close_first;
                        m_status = end_poly1;
                        break;
                    }
                } else {
                    if (m_src_vertex >= m_src_vertices.size() - 1) {
                        m_status = cap2;
                        break;
                    }
                }

                calc_join(m_out_vertices,
                          m_src_vertices.prev(m_src_vertex),
                          m_src_vertices.curr(m_src_vertex),
                          m_src_vertices.next(m_src_vertex),
                          m_src_vertices.prev(m_src_vertex).d,
                          m_src_vertices.curr(m_src_vertex).d);

                ++m_src_vertex;
                m_prev_status = m_status;
                m_status = out_vertices;
                m_out_vertex = 0;
                break;
            case close_first:
                m_status = outline2;
                cmd = path_cmd_move_to;

            case outline2:
                if (m_src_vertex <= (unsigned int)(m_closed == 0)) {
                    m_status = end_poly2;
                    m_prev_status = stop;
                    break;
                }

                --m_src_vertex;
                calc_join(m_out_vertices,
                          m_src_vertices.next(m_src_vertex),
                          m_src_vertices.curr(m_src_vertex),
                          m_src_vertices.prev(m_src_vertex),
                          m_src_vertices.curr(m_src_vertex).d,
                          m_src_vertices.prev(m_src_vertex).d);

                m_prev_status = m_status;
                m_status = out_vertices;
                m_out_vertex = 0;
                break;
            case out_vertices:
                if (m_out_vertex >= m_out_vertices.size()) {
                    m_status = m_prev_status;
                } else {
                    const vertex_s& v = m_out_vertices[m_out_vertex++];
                    *x = v.x;
                    *y = v.y;
                    return cmd;
                }
                break;
            case end_poly1:
                m_status = m_prev_status;
                return path_cmd_end_poly | path_flags_close | path_flags_ccw;

            case end_poly2:
                m_status = m_prev_status;
                return path_cmd_end_poly | path_flags_close | path_flags_cw;

            case stop:
                cmd = path_cmd_stop;
                break;
            }
        }
        return cmd;
    }

private:
    typedef vertex_sequence<vertex_dist, 6> vertex_storage;
    typedef pod_bvector<vertex_s, 6> coord_storage;

    void calc_cap(coord_storage& cs, const vertex_dist& v0, const vertex_dist& v1, scalar len)
    {
        cs.clear();

        scalar dx1 = (v1.y - v0.y) / len;
        scalar dy1 = (v1.x - v0.x) / len;
        scalar dx2 = 0;
        scalar dy2 = 0;

        dx1 *= m_width;
        dy1 *= m_width;

        if (m_line_cap != round_cap) {
            if (m_line_cap == square_cap) {
                dx2 = dy1 * m_width_sign;
                dy2 = dx1 * m_width_sign;
            }
            add_coord_vertex(cs, v0.x - dx1 - dx2, v0.y + dy1 - dy2);
            add_coord_vertex(cs, v0.x + dx1 - dx2, v0.y - dy1 - dy2);
        } else {
            int i;
            scalar a1;
            scalar da = Acos(m_width_abs / (m_width_abs + FLT_TO_SCALAR(0.125f) / m_approx_scale)) * 2;
            int n = (int)iround(PI / da); // FIXME:need round ?

            da = PI / (n + 1);
            add_coord_vertex(cs, v0.x - dx1, v0.y + dy1);
            if (m_width_sign > 0) {
                a1 = Atan2(dy1, -dx1);
                a1 += da;
                for (i = 0; i < n; i++) {
                    add_coord_vertex(cs, v0.x + Cos(a1) * m_width, v0.y + Sin(a1) * m_width);
                    a1 += da;
                }
            } else {
                a1 = Atan2(-dy1, dx1);
                a1 -= da;
                for (i = 0; i < n; i++) {
                    add_coord_vertex(cs, v0.x + Cos(a1) * m_width, v0.y + Sin(a1) * m_width);
                    a1 -= da;
                }
            }
            add_coord_vertex(cs, v0.x + dx1, v0.y - dy1);
        }
    }

    void calc_join(coord_storage& cs, const vertex_dist& v0,
                   const vertex_dist& v1, const vertex_dist& v2, scalar len1, scalar len2)
    {
        scalar dx1 = m_width * (v1.y - v0.y) / len1;
        scalar dy1 = m_width * (v1.x - v0.x) / len1;
        scalar dx2 = m_width * (v2.y - v1.y) / len2;
        scalar dy2 = m_width * (v2.x - v1.x) / len2;

        cs.clear();

        scalar cp = cross_product(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);
        if ((cp != 0) && ((cp > 0) == (m_width > 0))) {
            // inner join
            scalar limit = ((len1 < len2) ? len1 : len2) / m_width_abs;
            if (limit < m_inner_miter_limit) {
                limit = m_inner_miter_limit;
            }

            switch (m_inner_join)
            {
                case inner_miter:
                    calc_miter(cs, v0, v1, v2, dx1, dy1, dx2, dy2,
                               miter_join_revert, limit, 0);
                    break;
                case inner_jag:
                case inner_round:
                    cp = (dx1-dx2) * (dx1-dx2) + (dy1-dy2) * (dy1-dy2);
                    if (cp < len1 * len1 && cp < len2 * len2) {
                        calc_miter(cs, v0, v1, v2, dx1, dy1, dx2, dy2,
                                   miter_join_revert, limit, 0);
                    } else {
                        if (m_inner_join == inner_jag) {
                            add_coord_vertex(cs, v1.x + dx1, v1.y - dy1);
                            add_coord_vertex(cs, v1.x, v1.y);
                            add_coord_vertex(cs, v1.x + dx2, v1.y - dy2);
                        } else {
                            add_coord_vertex(cs, v1.x + dx1, v1.y - dy1);
                            add_coord_vertex(cs, v1.x, v1.y);

                            calc_arc(cs, v1.x, v1.y, dx2, -dy2, dx1, -dy1);
                            add_coord_vertex(cs, v1.x, v1.y);
                            add_coord_vertex(cs, v1.x + dx2, v1.y - dy2);
                        }
                    }
                    break;
                default: // inner_bevel
                    add_coord_vertex(cs, v1.x + dx1, v1.y - dy1);
                    add_coord_vertex(cs, v1.x + dx2, v1.y - dy2);
                    break;
            }
        } else {
            // outer join
            scalar dx = (dx1 + dx2) / 2;
            scalar dy = (dy1 + dy2) / 2;
            scalar dbevel = Sqrt(dx * dx + dy * dy);

            if (m_line_join == round_join || m_line_join == bevel_join) {
                if (m_approx_scale * (m_width_abs - dbevel) < m_width_eps) {
                    if (calc_intersection(v0.x + dx1, v0.y - dy1, v1.x + dx1, v1.y - dy1,
                                          v1.x + dx2, v1.y - dy2, v2.x + dx2, v2.y - dy2, &dx, &dy))
                    {
                        add_coord_vertex(cs, dx, dy);
                    } else {
                        add_coord_vertex(cs, v1.x + dx1, v1.y - dy1);
                    }
                    return;
                }
            }

            switch (m_line_join)
            {
                case miter_join:
                case miter_join_revert:
                case miter_join_round:
                    calc_miter(cs, v0, v1, v2, dx1, dy1, dx2, dy2,
                               m_line_join, m_miter_limit, dbevel);
                    break;
                case round_join:
                    calc_arc(cs, v1.x, v1.y, dx1, -dy1, dx2, -dy2);
                    break;
                default: // Bevel join
                    add_coord_vertex(cs, v1.x + dx1, v1.y - dy1);
                    add_coord_vertex(cs, v1.x + dx2, v1.y - dy2);
                    break;
            }
        }
    }

    void calc_arc(coord_storage& cs, scalar x, scalar y,
                  scalar dx1, scalar dy1, scalar dx2, scalar dy2)
    {
        scalar a1 = Atan2(dy1 * m_width_sign, dx1 * m_width_sign);
        scalar a2 = Atan2(dy2 * m_width_sign, dx2 * m_width_sign);
        scalar da = a1 - a2;
        int i, n;

        da = Acos(m_width_abs / (m_width_abs + FLT_TO_SCALAR(0.125f) / m_approx_scale)) * 2;

        add_coord_vertex(cs, x + dx1, y + dy1);
        if (m_width_sign > 0) {
            if (a1 > a2)
                a2 += _2PI;
            n = (int)iround((a2 - a1) / da); // FIXME:need round ?
            da = (a2 - a1) / (n + 1);
            a1 += da;
            for (i = 0; i < n; i++) {
                add_coord_vertex(cs, x + Cos(a1) * m_width, y + Sin(a1) * m_width);
                a1 += da;
            }
        } else {
            if (a1 < a2)
                a2 -= _2PI;
            n = (int)iround((a1 - a2) / da); // FIXME:need round ?
            da = (a1 - a2) / (n + 1);
            a1 -= da;
            for (i = 0; i < n; i++) {
                add_coord_vertex(cs, x + Cos(a1) * m_width, y + Sin(a1) * m_width);
                a1 -= da;
            }
        }
        add_coord_vertex(cs, x + dx2, y + dy2);
    }

    void calc_miter(coord_storage& cs, const vertex_dist& v0, const vertex_dist& v1,
                    const vertex_dist& v2, scalar dx1, scalar dy1, scalar dx2, scalar dy2,
                    line_join lj, scalar mlimit, scalar dbevel)
    {
        scalar xi = v1.x;
        scalar yi = v1.y;
        scalar di = FLT_TO_SCALAR(1.0f);
        scalar lim = m_width_abs * mlimit;
        bool miter_limit_exceeded = true; // Assume the worst
        bool intersection_failed  = true; // Assume the worst

        if (calc_intersection(v0.x + dx1, v0.y - dy1, v1.x + dx1, v1.y - dy1,
                              v1.x + dx2, v1.y - dy2, v2.x + dx2, v2.y - dy2, &xi, &yi))
        {
            // calculation of the intersection succeeded
            di = calc_distance(v1.x, v1.y, xi, yi);
            if (di <= lim) {
                // inside the miter limit
                add_coord_vertex(cs, xi, yi);
                miter_limit_exceeded = false;
            }
            intersection_failed = false;
        } else {
            // calculation of the intersection failed, most probably
            // the three points lie one straight line.
            // first check if v0 and v2 lie on the opposite sides of vector:
            // (v1.x, v1.y) -> (v1.x+dx1, v1.y-dy1), that is, the perpendicular
            // to the line determined by vertices v0 and v1.
            // this condition determines whether the next line segments continues
            // the previous one or goes back.
            //----------------
            scalar x2 = v1.x + dx1;
            scalar y2 = v1.y - dy1;
            if ((cross_product(v0.x, v0.y, v1.x, v1.y, x2, y2) < 0.0) ==
                (cross_product(v1.x, v1.y, v2.x, v2.y, x2, y2) < 0.0))
            {
                // this case means that the next segment continues
                // the previous one (straight line)
                add_coord_vertex(cs, v1.x + dx1, v1.y - dy1);
                miter_limit_exceeded = false;
            }
        }

        if (miter_limit_exceeded) {
            // miter limit exceeded
            switch (lj)
            {
                case miter_join_revert:
                    add_coord_vertex(cs, v1.x + dx1, v1.y - dy1);
                    add_coord_vertex(cs, v1.x + dx2, v1.y - dy2);
                    break;
                case miter_join_round:
                    calc_arc(cs, v1.x, v1.y, dx1, -dy1, dx2, -dy2);
                    break;
                default:
                    // if no miter-revert, calculate new dx1, dy1, dx2, dy2
                    if (intersection_failed) {
                        mlimit *= m_width_sign;
                        add_coord_vertex(cs, v1.x + dx1 + dy1 * mlimit,
                                                    v1.y - dy1 + dx1 * mlimit);
                        add_coord_vertex(cs, v1.x + dx2 - dy2 * mlimit,
                                                    v1.y - dy2 - dx2 * mlimit);
                    } else {
                        scalar x1 = v1.x + dx1;
                        scalar y1 = v1.y - dy1;
                        scalar x2 = v1.x + dx2;
                        scalar y2 = v1.y - dy2;
                        di = (lim - dbevel) / (di - dbevel);
                        add_coord_vertex(cs, x1 + (xi - x1) * di, y1 + (yi - y1) * di);
                        add_coord_vertex(cs, x2 + (xi - x2) * di, y2 + (yi - y2) * di);
                    }
                    break;
            }
        }
    }

    void add_coord_vertex(coord_storage& cs, scalar x, scalar y)
    {
        cs.add(vertex_s(x, y));
    }
private:
    conv_stroke(const conv_stroke&);
    conv_stroke& operator=(const conv_stroke&);

    // stroke property
    scalar m_width;
    scalar m_width_abs;
    scalar m_width_eps;
    int m_width_sign;
    scalar m_miter_limit;
    scalar m_inner_miter_limit;
    scalar m_approx_scale;
    line_cap m_line_cap;
    line_join m_line_join;
    inner_join m_inner_join;

    // stroke status
    vertex_storage m_src_vertices;
    coord_storage m_out_vertices;
    scalar m_shorten;
    status m_status;
    status m_prev_status;
    unsigned int m_closed;
    unsigned int m_src_vertex;
    unsigned int m_out_vertex;
};

}
#endif /*_CONVERT_H_*/

