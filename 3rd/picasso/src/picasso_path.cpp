/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "device.h"
#include "geometry.h"
#include "convert.h"
#include "graphic_path.h"
#include "graphic_helper.h"

#include "picasso.h"
#include "picasso_global.h"
#include "picasso_matrix.h"
#include "picasso_objects.h"

namespace picasso {
//inner functions
static ps_rect _path_bounding_rect(const graphic_path& path)
{
    ps_rect r = {1, 1, 0, 0};
    scalar x1 = 1, y1 = 1, x2 = 0 ,y2 = 0;
    if (bounding_rect(const_cast<graphic_path&>(path), 0, &x1, &y1, &x2, &y2)) {
        r.x = x1;  r.y = y1;  r.w = x2-x1;  r.h = y2-y1;
    }
    return r;
}

void _path_operation(conv_clipper::clip_op op, const graphic_path& a, const graphic_path& b, graphic_path& r)
{
    conv_clipper cliper(a, b, op);
    cliper.rewind(0);
    r.remove_all();
    scalar x = 0, y = 0;
    unsigned int cmd = 0;
    while (!is_stop(cmd = cliper.vertex(&x, &y))) {
        r.add_vertex(x, y, cmd);
    }
}

}

#ifdef __cplusplus
extern "C" {
#endif

ps_path* PICAPI ps_path_create(void)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    ps_path *p = (ps_path*)mem_malloc(sizeof(ps_path));
    if (p) {
        p->refcount = 1;
        new ((void*)&(p->path)) picasso::graphic_path;
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_path* PICAPI ps_path_create_copy(const ps_path* path)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!path) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    ps_path *p = (ps_path*)mem_malloc(sizeof(ps_path));
    if (p) {
        p->refcount = 1;
        new ((void*)&(p->path)) picasso::graphic_path;
        p->path = path->path;
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_path* PICAPI ps_path_ref(ps_path* path)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!path) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    path->refcount++;
    global_status = STATUS_SUCCEED;
    return path;
}

void PICAPI ps_path_unref(ps_path* path)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    path->refcount--;
    if (path->refcount <= 0) {
        (&path->path)->picasso::graphic_path::~graphic_path();
        mem_free(path);
    }
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_path_move_to(ps_path* path, const ps_point* p)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path || !p) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    path->path.move_to(FLT_TO_SCALAR(p->x), FLT_TO_SCALAR(p->y));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_path_line_to(ps_path* path, const ps_point* p)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path || !p) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    path->path.line_to(FLT_TO_SCALAR(p->x), FLT_TO_SCALAR(p->y));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_path_tangent_arc_to(ps_path* path, float r, const ps_point* tp, const ps_point* ep)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path || !tp || !ep || r < 0) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ps_point sp;
    sp.x = SCALAR_TO_FLT(path->path.last_x());
    sp.y = SCALAR_TO_FLT(path->path.last_y());
    if ((tp->x == sp.x && tp->y == sp.y) || (tp->x == ep->x && tp->y == ep->y) || r == 0.f) {
        ps_path_line_to(path, tp);
        global_status = STATUS_SUCCEED;
        return;
    }
    ps_point p1p0 = {(sp.x - tp->x), (sp.y - tp->y)};
    ps_point p1p2 = {(ep->x - tp->x), (ep->y - tp->y)};
    float p1p0_length = sqrtf(p1p0.x * p1p0.x + p1p0.y * p1p0.y);
    float p1p2_length = sqrtf(p1p2.x * p1p2.x + p1p2.y * p1p2.y);

    float cos_phi = (p1p0.x * p1p2.x + p1p0.y * p1p2.y) / (p1p0_length * p1p2_length);
    // all points on a line logic
    if (cos_phi == -1.0f) {
        ps_path_line_to(path, tp);
        global_status = STATUS_SUCCEED;
        return;
    }

    if (cos_phi == 1.0f) {
        // add infinite far away point
        unsigned int max_length = 65535;
        float factor_max = max_length / p1p0_length;
        ps_point np  = {(sp.x + factor_max * p1p0.x), (sp.y + factor_max * p1p0.y)};
        ps_path_line_to(path, &np);
        global_status = STATUS_SUCCEED;
        return;
    }


    float tangent = r / tanf(acosf(cos_phi) / 2);
    float factor_p1p0 = tangent / p1p0_length;
    ps_point t_p1p0 = {(tp->x + factor_p1p0 * p1p0.x), (tp->y + factor_p1p0 * p1p0.y)};

    ps_point orth_p1p0 = {p1p0.y, -p1p0.x};
    float orth_p1p0_length = sqrtf(orth_p1p0.x * orth_p1p0.x + orth_p1p0.y * orth_p1p0.y);
    float factor_ra = r / orth_p1p0_length;

    // angle between orth_p1p0 and p1p2 to get the right vector orthographic to p1p0
    float cos_alpha = (orth_p1p0.x * p1p2.x + orth_p1p0.y * p1p2.y) / (orth_p1p0_length * p1p2_length);
    if (cos_alpha < 0.f) {
        orth_p1p0.x = -orth_p1p0.x;
        orth_p1p0.y = -orth_p1p0.y;
    }

    ps_point p = {(t_p1p0.x + factor_ra * orth_p1p0.x), (t_p1p0.y + factor_ra * orth_p1p0.y)};

    // calculate angles for addArc
    orth_p1p0.x = -orth_p1p0.x;
    orth_p1p0.y = -orth_p1p0.y;
    float sa = acosf(orth_p1p0.x / orth_p1p0_length);
    if (orth_p1p0.y < 0.f)
        sa = 2 * PI - sa;

    // clockwise logic
    ps_bool clockwise = True;

    float factor_p1p2 = tangent / p1p2_length;
    ps_point t_p1p2 = {(tp->x + factor_p1p2 * p1p2.x), (tp->y + factor_p1p2 * p1p2.y)};
    ps_point orth_p1p2 = {(t_p1p2.x - p.x),(t_p1p2.y - p.y)};
    float orth_p1p2_length = sqrtf(orth_p1p2.x * orth_p1p2.x + orth_p1p2.y * orth_p1p2.y);
    float ea = acosf(orth_p1p2.x / orth_p1p2_length);
    if (orth_p1p2.y < 0)
        ea = 2 * PI - ea;
    if ((sa > ea) && ((sa - ea) < PI))
        clockwise = False;
    if ((sa < ea) && ((ea - sa) > PI))
        clockwise = False;

    ps_path_line_to(path, &t_p1p0);

    ps_path_add_arc(path, &p, r, sa, ea, clockwise);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_path_arc_to(ps_path* path, float rx, float ry, float a, ps_bool large, ps_bool cw, const ps_point* ep)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path || !ep || rx <= 0.0 || ry <= 0.0) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    scalar x1 = path->path.last_x();
    scalar y1 = path->path.last_y();

    picasso::bezier_arc_svg arc(x1, y1, FLT_TO_SCALAR(rx), FLT_TO_SCALAR(ry), FLT_TO_SCALAR(a),
                            (large ? true : false), (cw ? true : false), FLT_TO_SCALAR(ep->x), FLT_TO_SCALAR(ep->y));
    picasso::conv_curve cr(arc);
    if (picasso::_is_closed_path(path->path))
        path->path.concat_path(cr, 0);
    else
        path->path.join_path(cr, 0);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_path_bezier_to(ps_path* path, const ps_point* cp1, const ps_point* cp2, const ps_point* ep)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path || !cp1 || !cp2 || !ep) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::curve4 c(path->path.last_x(), path->path.last_y(), FLT_TO_SCALAR(cp1->x), FLT_TO_SCALAR(cp1->y),
                    FLT_TO_SCALAR(cp2->x), FLT_TO_SCALAR(cp2->y), FLT_TO_SCALAR(ep->x), FLT_TO_SCALAR(ep->y));
    if (picasso::_is_closed_path(path->path))
        path->path.concat_path(c, 0);
    else
        path->path.join_path(c, 0);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_path_quad_to(ps_path* path, const ps_point* cp, const ps_point* ep)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path || !cp || !ep) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::curve3 c(path->path.last_x(), path->path.last_y(), FLT_TO_SCALAR(cp->x),
                    FLT_TO_SCALAR(cp->y), FLT_TO_SCALAR(ep->x), FLT_TO_SCALAR(ep->y));

    if (picasso::_is_closed_path(path->path))
        path->path.concat_path(c, 0);
    else
        path->path.join_path(c, 0);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_path_sub_close(ps_path* path)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    path->path.close_polygon();
    global_status = STATUS_SUCCEED;
}

float PICAPI ps_path_get_length(const ps_path* path)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return (0.0);
    }

    if (!path) {
        global_status = STATUS_INVALID_ARGUMENT;
        return (0.0);
    }

    global_status = STATUS_SUCCEED;
    return SCALAR_TO_FLT(picasso::path_length(const_cast<ps_path*>(path)->path, 0));
}

unsigned int PICAPI ps_path_get_vertex_count(const ps_path* path)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!path) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    global_status = STATUS_SUCCEED;
    return path->path.total_vertices();
}

ps_path_cmd PICAPI ps_path_get_vertex(const ps_path* path, unsigned int index, ps_point* point)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return PATH_CMD_STOP;
    }

    if (!path || !point || (index > path->path.total_vertices()-1)) {
        global_status = STATUS_INVALID_ARGUMENT;
        return PATH_CMD_STOP;
    }

    scalar x = 0, y = 0;
    unsigned int cmd = path->path.vertex(index, &x, &y);
    point->x = SCALAR_TO_FLT(x);
    point->y = SCALAR_TO_FLT(y);
    global_status = STATUS_SUCCEED;
    return (ps_path_cmd)cmd;
}

void PICAPI ps_path_clear(ps_path* path)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    path->path.free_all();
    global_status = STATUS_SUCCEED;
}

ps_bool PICAPI ps_path_is_empty(const ps_path* path)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return False;
    }

    if (!path) {
        global_status = STATUS_INVALID_ARGUMENT;
        return False;
    }

    global_status = STATUS_SUCCEED;
    return (!path->path.total_vertices()) ? True : False;
}

ps_bool PICAPI ps_path_bounding_rect(const ps_path* path, ps_rect* rect)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return False;
    }

    if (!path || !rect) {
        global_status = STATUS_INVALID_ARGUMENT;
        return False;
    }
    global_status = STATUS_SUCCEED;
    *rect = picasso::_path_bounding_rect(path->path);
    return True;
}

ps_bool PICAPI ps_path_stroke_contains(const ps_path* path, const ps_point* p, float width)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return False;
    }

    if (!path || !p) {
        global_status = STATUS_INVALID_ARGUMENT;
        return False;
    }

    if (!path->path.total_vertices()) {
        global_status = STATUS_SUCCEED;
        return False;
    }

    ps_rect br = picasso::_path_bounding_rect(path->path);
    if ((p->x < br.x) || (p->y < br.y) || (p->x > (br.x+br.w)) || (p->y > (br.y+br.h))) {
        //out of bounding rect
        global_status = STATUS_SUCCEED;
        return False;
    }

    global_status = STATUS_SUCCEED;
    return picasso::raster_adapter::stroke_contents_point(path->path,
                FLT_TO_SCALAR(p->x), FLT_TO_SCALAR(p->y), FLT_TO_SCALAR(width)) ? True : False;
}

ps_bool PICAPI ps_path_contains(const ps_path* path, const ps_point* p, ps_fill_rule rule)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return False;
    }

    if (!path || !p) {
        global_status = STATUS_INVALID_ARGUMENT;
        return False;
    }

    if (!path->path.total_vertices()) {
        global_status = STATUS_SUCCEED;
        return False;
    }

    ps_rect br = picasso::_path_bounding_rect(path->path);
    if ((p->x < br.x) || (p->y < br.y) || (p->x > (br.x+br.w)) || (p->y > (br.y+br.h))) {
        //out of bounding rect
        global_status = STATUS_SUCCEED;
        return False;
    }

    global_status = STATUS_SUCCEED;
    return picasso::raster_adapter::fill_contents_point(path->path,
                FLT_TO_SCALAR(p->x), FLT_TO_SCALAR(p->y), (picasso::filling_rule)rule) ? True : False;
}

void PICAPI ps_path_add_line(ps_path* path, const ps_point* p1, const ps_point* p2)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path || !p1 || !p2) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    path->path.move_to(FLT_TO_SCALAR(p1->x), FLT_TO_SCALAR(p1->y));
    path->path.line_to(FLT_TO_SCALAR(p2->x), FLT_TO_SCALAR(p2->y));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_path_add_arc(ps_path* path, const ps_point* cp, float r, float sa, float ea, ps_bool cw)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path || !cp) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    if (r <= 0.0f) {
        global_status = STATUS_SUCCEED;
        return;
    }

    picasso::arc a(FLT_TO_SCALAR(cp->x), FLT_TO_SCALAR(cp->y), FLT_TO_SCALAR(r), FLT_TO_SCALAR(r),
                FLT_TO_SCALAR(sa), FLT_TO_SCALAR(ea), (cw ? true : false));

    if (picasso::_is_closed_path(path->path))
        path->path.concat_path(a, 0);
    else
        path->path.join_path(a, 0);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_path_add_rect(ps_path* path, const ps_rect* r)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path || !r) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    path->path.move_to(FLT_TO_SCALAR(r->x), FLT_TO_SCALAR(r->y));
    path->path.hline_rel(FLT_TO_SCALAR(r->w));
    path->path.vline_rel(FLT_TO_SCALAR(r->h));
    path->path.hline_rel(-FLT_TO_SCALAR(r->w));
    path->path.end_poly();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_path_add_ellipse(ps_path* path, const ps_rect* r)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path || !r) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::ellipse e(FLT_TO_SCALAR(r->x+r->w/2), FLT_TO_SCALAR(r->y+r->h/2),
                            FLT_TO_SCALAR(r->w/2), FLT_TO_SCALAR(r->h/2));
    if (picasso::_is_closed_path(path->path))
        path->path.concat_path(e, 0);
    else
        path->path.join_path(e, 0);
    global_status = STATUS_SUCCEED;
}


void PICAPI ps_path_add_rounded_rect(ps_path*path, const ps_rect* r, float ltx, float lty, float rtx, float rty,
                                                                        float lbx, float lby, float rbx, float rby)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path || !r) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::rounded_rect rr;
    rr.rect(FLT_TO_SCALAR(r->x), FLT_TO_SCALAR(r->y), FLT_TO_SCALAR(r->x+r->w), FLT_TO_SCALAR(r->y+r->h));
    rr.radius(FLT_TO_SCALAR(ltx), FLT_TO_SCALAR(lty), FLT_TO_SCALAR(rtx), FLT_TO_SCALAR(rty),
                FLT_TO_SCALAR(lbx), FLT_TO_SCALAR(lby), FLT_TO_SCALAR(rbx), FLT_TO_SCALAR(rby));
    rr.normalize_radius();
    if (picasso::_is_closed_path(path->path))
        path->path.concat_path(rr, 0);
    else
        path->path.join_path(rr, 0);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_path_add_sub_path(ps_path* path, const ps_path* p)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!path || !p) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    if (picasso::_is_closed_path(path->path))
        path->path.concat_path(const_cast<ps_path*>(p)->path, 0);
    else
        path->path.join_path(const_cast<ps_path*>(p)->path, 0);

    global_status = STATUS_SUCCEED;
}

void PICAPI ps_path_clipping(ps_path* r, ps_path_operation op, const ps_path* a, const ps_path* b)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!r) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    if (!a || !a->path.total_vertices() || !picasso::_is_closed_path(a->path)) {//invalid a
        r->path = b->path;
        global_status = STATUS_SUCCEED;
        return;
    }

    if (!b || !b->path.total_vertices() || !picasso::_is_closed_path(b->path)) {//invalid b
        r->path = a->path;
        global_status = STATUS_SUCCEED;
        return;
    }

    picasso::_path_operation((picasso::conv_clipper::clip_op)op, a->path, b->path, r->path);

    global_status = STATUS_SUCCEED;
}

#ifdef __cplusplus
}
#endif
