/* Picasso - a vector graphics library
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "math_type.h"
#include "data_vector.h"
#include "geometry.h"
#include "graphic_base.h"
#include "graphic_path.h"

#include "picasso_matrix.h"

namespace picasso {

#define DEFAULT_VERTEICES  (8)

class graphic_path_impl
{
public:
    graphic_path_impl()
    {
    }

    graphic_path_impl(unsigned int size)
        : m_vertices(size)
        , m_cmds(size)
    {
    }

    void remove_all(void)
    {
        m_vertices.clear();
        m_cmds.clear();
    }

    void add_vertex(scalar x, scalar y, unsigned int cmd)
    {
        if (m_vertices.is_full()) {
            m_vertices.resize(m_vertices.capacity() << 1);
            m_cmds.resize(m_cmds.capacity() << 1);
        }

        m_vertices.push_back(vertex_s(x, y));
        m_cmds.push_back(cmd);
    }

    void modify_vertex(unsigned int idx, scalar x, scalar y)
    {
        vertex_s & v = m_vertices[idx];
        v.x = x;
        v.y = y;
    }

    void modify_vertex(unsigned int idx, scalar x, scalar y, unsigned int cmd)
    {
        vertex_s & v = m_vertices[idx];
        v.x = x;
        v.y = y;
        m_cmds[idx] = cmd;
    }

    void modify_command(unsigned int idx, unsigned int cmd)
    {
        m_cmds[idx] = cmd;
    }

    void swap_vertices(unsigned int v1, unsigned int v2)
    {
        vertex_s t = m_vertices[v1];
        unsigned int c = m_cmds[v1];

        m_vertices[v1] = m_vertices[v2];
        m_cmds[v1] = m_cmds[v2];

        m_vertices[v2] = t;
        m_cmds[v2] = c;
    }

    unsigned int last_command(void) const
    {
        return m_cmds.size() ? m_cmds[m_cmds.size() - 1] : path_cmd_stop;
    }

    unsigned int last_vertex(scalar* x, scalar* y) const
    {
        if (m_vertices.size() == 0) {
            *x = *y = FLT_TO_SCALAR(0.0f);
            return path_cmd_stop;
        }
        return vertex(m_vertices.size() - 1, x, y);
    }

    unsigned int prev_vertex(scalar* x, scalar* y) const
    {
        if (m_vertices.size() < 2) {
            *x = *y = FLT_TO_SCALAR(0.0f);
            return path_cmd_stop;
        }
        return vertex(m_vertices.size() - 2, x, y);
    }

    scalar last_x(void) const
    {
        return m_vertices.size() ? m_vertices[m_vertices.size() - 1].x : FLT_TO_SCALAR(0.0f);
    }

    scalar last_y(void) const
    {
        return m_vertices.size() ? m_vertices[m_vertices.size() - 1].y : FLT_TO_SCALAR(0.0f);
    }

    unsigned int total_vertices(void) const
    {
        return m_vertices.size();
    }

    unsigned int total_byte_size(void) const
    {
        return m_vertices.size() * sizeof(vertex_s) + m_cmds.size() * sizeof(unsigned int);
    }

    unsigned int vertex(unsigned int idx, scalar* x, scalar* y) const
    {
        const vertex_s & v = m_vertices[idx];
        *x = v.x;
        *y = v.y;
        return m_cmds[idx];
    }

    unsigned int command(unsigned int idx) const
    {
        return m_cmds[idx];
    }

private:
    friend class graphic_path;
    pod_vector<vertex_s> m_vertices;
    pod_vector<unsigned int> m_cmds;
};


// graphic path special
graphic_path::graphic_path()
    : m_impl(new graphic_path_impl(DEFAULT_VERTEICES))
    , m_iterator(0)
    , m_shape(shape_polygon)
{
}

graphic_path::~graphic_path()
{
    delete m_impl;
}

graphic_path::graphic_path(const graphic_path& o)
{
    m_impl = new graphic_path_impl(*(o.m_impl));
    m_iterator = o.m_iterator;
    m_shape = o.m_shape;
}

graphic_path& graphic_path::operator=(const graphic_path& o)
{
    if (this == &o)
        return *this;

    delete m_impl;
    m_impl = new graphic_path_impl(*(o.m_impl));
    m_iterator = o.m_iterator;
    m_shape = o.m_shape;

    return *this;
}

void graphic_path::remove_all(void)
{
    m_impl->remove_all();
    m_iterator = 0;
    m_shape = shape_polygon;
}

void graphic_path::free_all(void)
{
    remove_all();
}

unsigned int graphic_path::start_new_path(void)
{
    if (!is_stop(m_impl->last_command())) {
        m_impl->add_vertex(FLT_TO_SCALAR(0.0f), FLT_TO_SCALAR(0.0f), path_cmd_stop);
    }
    return m_impl->total_vertices();
}

void graphic_path::move_to(scalar x, scalar y)
{
    m_impl->add_vertex(x, y, path_cmd_move_to);
}

void graphic_path::move_rel(scalar dx, scalar dy)
{
    rel_to_abs(&dx, &dy);
    m_impl->add_vertex(dx, dy, path_cmd_move_to);
}

void graphic_path::line_to(scalar x, scalar y)
{
    m_impl->add_vertex(x, y, path_cmd_line_to);
}

void graphic_path::line_rel(scalar dx, scalar dy)
{
    rel_to_abs(&dx, &dy);
    m_impl->add_vertex(dx, dy, path_cmd_line_to);
}

void graphic_path::hline_to(scalar x)
{
    m_impl->add_vertex(x, last_y(), path_cmd_line_to);
}

void graphic_path::hline_rel(scalar dx)
{
    scalar dy = 0;
    rel_to_abs(&dx, &dy);
    m_impl->add_vertex(dx, dy, path_cmd_line_to);
}

void graphic_path::vline_to(scalar y)
{
    m_impl->add_vertex(last_x(), y, path_cmd_line_to);
}

void graphic_path::vline_rel(scalar dy)
{
    scalar dx = 0;
    rel_to_abs(&dx, &dy);
    m_impl->add_vertex(dx, dy, path_cmd_line_to);
}

void graphic_path::arc_to(scalar rx, scalar ry, scalar angle,
        bool large_arc_flag, bool sweep_flag, scalar x, scalar y)
{
    if (m_impl->total_vertices() && is_vertex(m_impl->last_command())) {
        const scalar epsilon = FLT_TO_SCALAR(1e-30f);
        scalar x0 = 0.0f;
        scalar y0 = 0.0f;
        m_impl->last_vertex(&x0, &y0);

        rx = Fabs(rx);
        ry = Fabs(ry);

        // Ensure radii are valid
        //-------------------------
        if (rx < epsilon || ry < epsilon) {
            line_to(x, y);
            return;
        }

        if (calc_distance(x0, y0, x, y) < epsilon) {
            // If the endpoints (x, y) and (x0, y0) are identical, then this
            // is equivalent to omitting the elliptical arc segment entirely.
            return;
        }

        picasso::bezier_arc_svg a(x0, y0, rx, ry, angle, large_arc_flag, sweep_flag, x, y);
        if (a.radii_ok()) {
            join_path(a);
        } else {
            line_to(x, y);
        }
    } else {
        move_to(x, y);
    }
}

void graphic_path::arc_rel(scalar rx, scalar ry, scalar angle,
        bool large_arc_flag, bool sweep_flag, scalar dx, scalar dy)
{
    rel_to_abs(&dx, &dy);
    arc_to(rx, ry, angle, large_arc_flag, sweep_flag, dx, dy);
}

void graphic_path::curve3(scalar x_ctrl, scalar y_ctrl, scalar x_to, scalar y_to)
{
    m_impl->add_vertex(x_ctrl, y_ctrl, path_cmd_curve3);
    m_impl->add_vertex(x_to, y_to, path_cmd_curve3);
}

void graphic_path::curve3_rel(scalar dx_ctrl, scalar dy_ctrl, scalar dx_to, scalar dy_to)
{
    rel_to_abs(&dx_ctrl, &dy_ctrl);
    rel_to_abs(&dx_to, &dy_to);
    m_impl->add_vertex(dx_ctrl, dy_ctrl, path_cmd_curve3);
    m_impl->add_vertex(dx_to, dy_to, path_cmd_curve3);
}

void graphic_path::curve3(scalar x_to, scalar y_to)
{
    scalar x0;
    scalar y0;
    if (is_vertex(m_impl->last_vertex(&x0, &y0))) {
        scalar x_ctrl;
        scalar y_ctrl;
        unsigned int cmd = m_impl->prev_vertex(&x_ctrl, &y_ctrl);
        if (is_curve(cmd)) {
            x_ctrl = x0 + x0 - x_ctrl;
            y_ctrl = y0 + y0 - y_ctrl;
        } else {
            x_ctrl = x0;
            y_ctrl = y0;
        }
        curve3(x_ctrl, y_ctrl, x_to, y_to);
    }
}

void graphic_path::curve3_rel(scalar dx_to, scalar dy_to)
{
    rel_to_abs(&dx_to, &dy_to);
    curve3(dx_to, dy_to);
}

void graphic_path::curve4(scalar x_ctrl1, scalar y_ctrl1, scalar x_ctrl2, scalar y_ctrl2, scalar x_to, scalar y_to)
{
    m_impl->add_vertex(x_ctrl1, y_ctrl1, path_cmd_curve4);
    m_impl->add_vertex(x_ctrl2, y_ctrl2, path_cmd_curve4);
    m_impl->add_vertex(x_to, y_to, path_cmd_curve4);
}

void graphic_path::curve4_rel(scalar dx_ctrl1, scalar dy_ctrl1, scalar dx_ctrl2,
                                                        scalar dy_ctrl2, scalar dx_to, scalar dy_to)
{
    rel_to_abs(&dx_ctrl1, &dy_ctrl1);
    rel_to_abs(&dx_ctrl2, &dy_ctrl2);
    rel_to_abs(&dx_to, &dy_to);
    m_impl->add_vertex(dx_ctrl1, dy_ctrl1, path_cmd_curve4);
    m_impl->add_vertex(dx_ctrl2, dy_ctrl2, path_cmd_curve4);
    m_impl->add_vertex(dx_to, dy_to, path_cmd_curve4);
}

void graphic_path::curve4(scalar x_ctrl2, scalar y_ctrl2, scalar x_to, scalar y_to)
{
    scalar x0;
    scalar y0;
    if (is_vertex(last_vertex(&x0, &y0))) {
        scalar x_ctrl1;
        scalar y_ctrl1;
        unsigned int cmd = prev_vertex(&x_ctrl1, &y_ctrl1);
        if (is_curve(cmd)) {
            x_ctrl1 = x0 + x0 - x_ctrl1;
            y_ctrl1 = y0 + y0 - y_ctrl1;
        } else {
            x_ctrl1 = x0;
            y_ctrl1 = y0;
        }
        curve4(x_ctrl1, y_ctrl1, x_ctrl2, y_ctrl2, x_to, y_to);
    }
}

void graphic_path::curve4_rel(scalar dx_ctrl2, scalar dy_ctrl2, scalar dx_to, scalar dy_to)
{
    rel_to_abs(&dx_ctrl2, &dy_ctrl2);
    rel_to_abs(&dx_to, &dy_to);
    curve4(dx_ctrl2, dy_ctrl2, dx_to, dy_to);
}

void graphic_path::end_poly(unsigned int flags)
{
    if (is_vertex(m_impl->last_command())) {
        m_impl->add_vertex(FLT_TO_SCALAR(0.0f), FLT_TO_SCALAR(0.0f), path_cmd_end_poly | flags);
    }
}

void graphic_path::close_polygon(unsigned int flags)
{
    end_poly(path_flags_close | flags);
}

scalar graphic_path::last_x(void) const
{
    return m_impl->last_x();
}

scalar graphic_path::last_y(void) const
{
    return m_impl->last_y();
}

unsigned int graphic_path::last_vertex(scalar* x, scalar* y) const
{
    return m_impl->last_vertex(x, y);
}

unsigned int graphic_path::prev_vertex(scalar* x, scalar* y) const
{
    return m_impl->prev_vertex(x, y);
}

unsigned int graphic_path::total_vertices(void) const
{
    return m_impl->total_vertices();
}

unsigned int graphic_path::total_byte_size(void) const
{
    return m_impl->total_byte_size();
}

void graphic_path::rel_to_abs(scalar* x, scalar* y) const
{
    if (m_impl->total_vertices()) {
        scalar x2;
        scalar y2;
        if (is_vertex(m_impl->last_vertex(&x2, &y2))) {
            *x += x2;
            *y += y2;
        }
    }
}

unsigned int graphic_path::vertex(unsigned int idx, scalar* x, scalar* y) const
{
    return m_impl->vertex(idx, x, y);
}

unsigned int graphic_path::command(unsigned int idx) const
{
    return m_impl->command(idx);
}

void graphic_path::modify_vertex(unsigned int idx, scalar x, scalar y)
{
    m_impl->modify_vertex(idx, x, y);
}

void graphic_path::modify_vertex(unsigned int idx, scalar x, scalar y, unsigned int cmd)
{
    m_impl->modify_vertex(idx, x, y, cmd);
}

void graphic_path::modify_command(unsigned int idx, unsigned int cmd)
{
    m_impl->modify_command(idx, cmd);
}

void graphic_path::rewind(unsigned int id)
{
    m_iterator = id;
}

unsigned int graphic_path::vertex(scalar* x, scalar* y)
{
    if (m_iterator >= m_impl->total_vertices())
        return path_cmd_stop;
    return m_impl->vertex(m_iterator++, x, y);
}

void graphic_path::add_vertex(scalar x, scalar y, unsigned int cmd)
{
    m_impl->add_vertex(x, y, cmd);
}

unsigned int graphic_path::arrange_polygon_orientation(unsigned int start, unsigned int flag_orientation)
{
    if (flag_orientation == path_flags_none)
        return start;

    // Skip all non-vertices at the beginning
    while (start < m_impl->total_vertices() &&
            !is_vertex(m_impl->command(start))) ++start;

    // Skip all insignificant move_to
    while (start+1 < m_impl->total_vertices() &&
            is_move_to(m_impl->command(start)) &&
            is_move_to(m_impl->command(start+1))) ++start;

    // Find the last vertex
    unsigned int end = start + 1;
    while (end < m_impl->total_vertices() &&
            !is_next_poly(m_impl->command(end))) ++end;

    if (end - start > 2) {
        if (perceive_polygon_orientation(start, end) != flag_orientation) {
            // Invert polygon, set orientation flag, and skip all end_poly
            invert_polygon(start, end);
            unsigned int cmd;
            while (end < m_impl->total_vertices() &&
                    is_end_poly(cmd = m_impl->command(end)))
            {
                m_impl->modify_command(end++, set_orientation(cmd, flag_orientation));
            }
        }
    }
    return end;
}

unsigned int graphic_path::arrange_orientations(unsigned int start, unsigned int flag_orientation)
{
    if (flag_orientation != path_flags_none) {
        while (start < m_impl->total_vertices()) {
            start = arrange_polygon_orientation(start, flag_orientation);
            if(is_stop(m_impl->command(start))) {
                ++start;
                break;
            }
        }
    }
    return start;
}

void graphic_path::arrange_orientations_all_paths(unsigned int flag_orientation)
{
    if (flag_orientation != path_flags_none) {
        unsigned int start = 0;
        while (start < m_impl->total_vertices()) {
            start = arrange_orientations(start, flag_orientation);
        }
    }
}

unsigned int graphic_path::perceive_polygon_orientation(unsigned int start, unsigned int end)
{
    // Calculate signed area (scalar area to be exact)
    //---------------------
    unsigned int np = end - start;
    scalar area = FLT_TO_SCALAR(0.0f);
    unsigned int i;
    for (i = 0; i < np; i++) {
        scalar x1, y1, x2, y2;
        m_impl->vertex(start + i,            &x1, &y1);
        m_impl->vertex(start + (i + 1) % np, &x2, &y2);
        area += x1 * y2 - y1 * x2;
    }
    return (area < FLT_TO_SCALAR(0.0f)) ? path_flags_cw : path_flags_ccw;
}

void graphic_path::invert_polygon(unsigned int start, unsigned int end)
{
    unsigned int i;
    unsigned int tmp_cmd = m_impl->command(start);

    --end; // Make "end" inclusive

    // Shift all commands to one position
    for (i = start; i < end; i++) {
        m_impl->modify_command(i, m_impl->command(i + 1));
    }

    // Assign starting command to the ending command
    m_impl->modify_command(end, tmp_cmd);

    // Reverse the polygon
    while(end > start) {
        m_impl->swap_vertices(start++, end--);
    }
}

void graphic_path::invert_polygon(unsigned int start)
{
    // Skip all non-vertices at the beginning
    while(start < m_impl->total_vertices() &&
            !is_vertex(m_impl->command(start))) ++start;

    // Skip all insignificant move_to
    while(start+1 < m_impl->total_vertices() &&
            is_move_to(m_impl->command(start)) &&
            is_move_to(m_impl->command(start+1))) ++start;

    // Find the last vertex
    unsigned int end = start + 1;
    while (end < m_impl->total_vertices() &&
            !is_next_poly(m_impl->command(end))) ++end;

    invert_polygon(start, end);
}

void graphic_path::flip_x(scalar x1, scalar x2)
{
    scalar x, y;
    for (unsigned int i = 0; i < m_impl->total_vertices(); i++) {
        unsigned int cmd = m_impl->vertex(i, &x, &y);
        if(is_vertex(cmd)) {
            m_impl->modify_vertex(i, x2 - x + x1, y);
        }
    }
}

void graphic_path::flip_y(scalar y1, scalar y2)
{
    scalar x, y;
    for (unsigned int i = 0; i < m_impl->total_vertices(); i++) {
        unsigned int cmd = m_impl->vertex(i, &x, &y);
        if (is_vertex(cmd)) {
            m_impl->modify_vertex(i, x, y2 - y + y1);
        }
    }
}

void graphic_path::translate(scalar dx, scalar dy, unsigned int id)
{
    unsigned int num_ver = m_impl->total_vertices();
    for(unsigned int path_id = id; path_id < num_ver; path_id++) {
        scalar x, y;
        unsigned int cmd = m_impl->vertex(path_id, &x, &y);
        if (is_stop(cmd))
            break;

        if (is_vertex(cmd)) {
            x += dx;
            y += dy;
            m_impl->modify_vertex(path_id, x, y);
        }
    }
}

void graphic_path::translate_all_paths(scalar dx, scalar dy)
{
    unsigned int idx;
    unsigned int num_ver = m_impl->total_vertices();
    for (idx = 0; idx < num_ver; idx++) {
        scalar x, y;
        if (is_vertex(m_impl->vertex(idx, &x, &y))) {
            x += dx;
            y += dy;
            m_impl->modify_vertex(idx, x, y);
        }
    }
}

void graphic_path::transform(const trans_affine& trans, unsigned int id)
{
    unsigned int num_ver = m_impl->total_vertices();
    for (unsigned int path_id = id; path_id < num_ver; path_id++) {
        scalar x, y;
        unsigned int cmd = m_impl->vertex(path_id, &x, &y);
        if (is_stop(cmd))
            break;

        if (is_vertex(cmd)) {
            trans.transform(&x, &y);
            m_impl->modify_vertex(path_id, x, y);
        }
    }
}

void graphic_path::transform_all_paths(const trans_affine& trans)
{
    unsigned int idx;
    unsigned int num_ver = m_impl->total_vertices();
    for (idx = 0; idx < num_ver; idx++) {
        scalar x, y;
        if (is_vertex(m_impl->vertex(idx, &x, &y))) {
            trans.transform(&x, &y);
            m_impl->modify_vertex(idx, x, y);
        }
    }
}

void graphic_path::join_path(vertex_source& vs, unsigned int id)
{
    scalar x = 0, y = 0;
    unsigned int cmd;
    vs.rewind(id);
    cmd = vs.vertex(&x, &y);
    if (!is_stop(cmd)) {
        if (is_vertex(cmd)) {
            scalar x0, y0;
            unsigned int cmd0 = last_vertex(&x0, &y0);
            if (is_vertex(cmd0)) {
                if (calc_distance(x, y, x0, y0) > FLT_TO_SCALAR(vertex_dist_epsilon)) {
                    if (is_move_to(cmd)) {
                        cmd = path_cmd_line_to;
                    }
                    m_impl->add_vertex(x, y, cmd);
                }
            } else {
                if (is_stop(cmd0)) {
                    cmd = path_cmd_move_to;
                } else {
                    if (is_move_to(cmd))
                        cmd = path_cmd_line_to;
                }
                m_impl->add_vertex(x, y, cmd);
            }
        }

        while (!is_stop(cmd = vs.vertex(&x, &y))) {
            m_impl->add_vertex(x, y, is_move_to(cmd) ? path_cmd_line_to : cmd);
        }
    }
}

void graphic_path::concat_path(vertex_source& vs, unsigned int id)
{
    scalar x = 0, y = 0;
    unsigned int cmd;
    vs.rewind(id);
    while (!is_stop(cmd = vs.vertex(&x, &y))) {
        m_impl->add_vertex(x, y, cmd);
    }
}

void graphic_path::serialize_to(byte* buffer)
{
    unsigned int num = m_impl->total_vertices();
    mem_copy(buffer, m_impl->m_vertices.data(), num * sizeof(vertex_s));
    buffer += num * sizeof(vertex_s);
    mem_copy(buffer, m_impl->m_cmds.data(), num * sizeof(unsigned int));
}

void graphic_path::serialize_from(unsigned int num, byte* buffer, unsigned int buf_len)
{
    //FIXME: paramer check and buf_len!
    remove_all();
    m_impl->m_vertices.resize(num);
    m_impl->m_vertices.set_data(num, (vertex_s*)buffer);
    buffer += num * sizeof(vertex_s);
    m_impl->m_cmds.resize(num);
    m_impl->m_cmds.set_data(num, (unsigned int*)buffer);
}

}

