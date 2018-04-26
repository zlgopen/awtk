/* Picasso - a vector graphics library
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GRAPHIC_PATH_H_
#define _GRAPHIC_PATH_H_

#include "common.h"
#include "vertex.h"
#include "graphic_base.h"

namespace picasso {

class trans_affine;
class graphic_path_impl;

class graphic_path : public vertex_container
{
public:
    typedef enum {
        shape_polygon       = 0,
        shape_rectangle     = 1,
        shape_ellipse       = 2,
        shape_rounded_rect  = 3,
    } shape_type;

    graphic_path();
    graphic_path(const graphic_path& o);

    virtual ~graphic_path();

    graphic_path& operator=(const graphic_path& o);

    void free_all(void);

    //Make path functions
    unsigned int start_new_path(void);

    void move_to(scalar x, scalar y);
    void move_rel(scalar dx, scalar dy);

    void line_to(scalar x, scalar y);
    void line_rel(scalar dx, scalar dy);

    void hline_to(scalar x);
    void hline_rel(scalar dx);

    void vline_to(scalar y);
    void vline_rel(scalar dy);

    void arc_to(scalar rx, scalar ry, scalar angle,
                bool large_arc_flag, bool sweep_flag, scalar x, scalar y);

    void arc_rel(scalar rx, scalar ry, scalar angle,
                bool large_arc_flag, bool sweep_flag, scalar dx, scalar dy);

    void curve3(scalar x_ctrl, scalar y_ctrl, scalar x_to,   scalar y_to);
    void curve3_rel(scalar dx_ctrl, scalar dy_ctrl, scalar dx_to,   scalar dy_to);

    void curve3(scalar x_to, scalar y_to);
    void curve3_rel(scalar dx_to, scalar dy_to);

    void curve4(scalar x_ctrl1, scalar y_ctrl1, scalar x_ctrl2, scalar y_ctrl2, scalar x_to, scalar y_to);
    void curve4_rel(scalar dx_ctrl1, scalar dy_ctrl1, scalar dx_ctrl2, scalar dy_ctrl2, scalar dx_to, scalar dy_to);

    void curve4(scalar x_ctrl2, scalar y_ctrl2, scalar x_to, scalar y_to);
    void curve4_rel(scalar x_ctrl2, scalar y_ctrl2, scalar x_to, scalar y_to);

    void end_poly(unsigned int flags = path_flags_close);
    void close_polygon(unsigned int flags = path_flags_none);

    // Accessors
    scalar last_x(void) const;
    scalar last_y(void) const;

    unsigned int last_vertex(scalar* x, scalar* y) const;
    unsigned int prev_vertex(scalar* x, scalar* y) const;

    unsigned int total_vertices(void) const;
    unsigned int total_byte_size(void) const;
    void rel_to_abs(scalar* x, scalar* y) const;

    unsigned int vertex(unsigned int idx, scalar* x, scalar* y) const;
    unsigned int command(unsigned int idx) const;

    void modify_vertex(unsigned int idx, scalar x, scalar y);
    void modify_vertex(unsigned int idx, scalar x, scalar y, unsigned int cmd);
    void modify_command(unsigned int idx, unsigned int cmd);

    unsigned int arrange_polygon_orientation(unsigned int start, unsigned int flag_orientation);
    unsigned int arrange_orientations(unsigned int start, unsigned int flag_orientation);
    void arrange_orientations_all_paths(unsigned int flag_orientation);
    void invert_polygon(unsigned int start);

    void flip_x(scalar x1, scalar x2);
    void flip_y(scalar y1, scalar y2);

    void translate(scalar dx, scalar dy, unsigned int id = 0);
    void translate_all_paths(scalar dx, scalar dy);

    void transform(const trans_affine& trans, unsigned int id = 0);
    void transform_all_paths(const trans_affine& trans);

    // path calc
    void join_path(vertex_source& vs, unsigned int id = 0);
    void concat_path(vertex_source& vs, unsigned int id = 0);

    // vertex source interface
    virtual void rewind(unsigned int id = 0);
    virtual unsigned int vertex(scalar* x, scalar* y);
    virtual void add_vertex(scalar x, scalar y, unsigned int cmd);
    virtual void remove_all(void);

    // sheap type
    void set_shape(shape_type s) { m_shape = s; }
    shape_type get_shape(void) const { return (shape_type)m_shape; }

    // serialize
    void serialize_to(byte* buffer);
    void serialize_from(unsigned int num, byte* buffer, unsigned int buf_len);
private:
    unsigned int perceive_polygon_orientation(unsigned int start, unsigned int end);
    void invert_polygon(unsigned int start, unsigned int end);
    graphic_path_impl *m_impl;
    unsigned int m_iterator;
    unsigned int m_shape;
};

inline bool operator != (const graphic_path& a, const graphic_path& b)
{
    if (a.total_vertices() != b.total_vertices())
        return true;  // total vertices is not same , the path is not same

    unsigned int num = a.total_vertices(); //because a total_vertices is same to b total_vertices.
    scalar x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    const_cast<graphic_path&>(a).rewind();
    const_cast<graphic_path&>(b).rewind();
    for (unsigned int i = 0; i < num; i++) {
        unsigned int cmd1 = a.vertex(i, &x1, &y1);
        unsigned int cmd2 = b.vertex(i, &x2, &y2);
        if ((cmd1 != cmd2) || (x1 != x2) || (y1 != y2))
            return true; // this vertex is not same.
    }
    return false;
}

inline bool _is_closed_path(const graphic_path& path)
{
    scalar x, y;
    unsigned int flag;
    if (!path.total_vertices())
        return true;

    flag = path.last_vertex(&x, &y);
    if (flag & path_flags_close)
        return true;

    return false;
}

}
#endif /*_GRAPHIC_PATH_H_*/

