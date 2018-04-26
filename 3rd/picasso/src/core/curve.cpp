/* Picasso - a vector graphics library
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "math_type.h"
#include "curve.h"

namespace picasso {

const scalar curve_distance_epsilon          = FLT_TO_SCALAR(1e-30f);
const scalar curve_collinearity_epsilon      = FLT_TO_SCALAR(1e-30f);
const scalar curve_angle_tolerance_epsilon   = FLT_TO_SCALAR(0.01f);

enum curve_recursion_limit
{
    curve_recursion_limit = 32,
};

// curve3_inc
void curve3_inc::init(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3)
{
    m_start_x = x1;
    m_start_y = y1;
    m_end_x   = x3;
    m_end_y   = y3;

    scalar dx1 = x2 - x1;
    scalar dy1 = y2 - y1;
    scalar dx2 = x3 - x2;
    scalar dy2 = y3 - y2;

    scalar len = Sqrt(dx1 * dx1 + dy1 * dy1) + Sqrt(dx2 * dx2 + dy2 * dy2);

    m_num_steps = uround(len * FLT_TO_SCALAR(0.25f) * m_scale);

    if (m_num_steps < 4) {
        m_num_steps = 4;
    }

    scalar subdivide_step  = FLT_TO_SCALAR(1.0f) / m_num_steps;
    scalar subdivide_step2 = subdivide_step * subdivide_step;

    scalar tmpx = (x1 - x2 * FLT_TO_SCALAR(2.0f) + x3) * subdivide_step2;
    scalar tmpy = (y1 - y2 * FLT_TO_SCALAR(2.0f) + y3) * subdivide_step2;

    m_saved_fx = m_fx = x1;
    m_saved_fy = m_fy = y1;

    m_saved_dfx = m_dfx = tmpx + (x2 - x1) * (FLT_TO_SCALAR(2.0f) * subdivide_step);
    m_saved_dfy = m_dfy = tmpy + (y2 - y1) * (FLT_TO_SCALAR(2.0f) * subdivide_step);

    m_ddfx = tmpx * FLT_TO_SCALAR(2.0f);
    m_ddfy = tmpy * FLT_TO_SCALAR(2.0f);

    m_step = m_num_steps;
}

void curve3_inc::rewind(unsigned int)
{
    if (m_num_steps == 0) {
        m_step = -1;
        return;
    }
    m_step = m_num_steps;
    m_fx   = m_saved_fx;
    m_fy   = m_saved_fy;
    m_dfx  = m_saved_dfx;
    m_dfy  = m_saved_dfy;
}

unsigned int curve3_inc::vertex(scalar* x, scalar* y)
{
    if (m_step < 0)
        return path_cmd_stop;

    if (m_step == m_num_steps) {
        *x = m_start_x;
        *y = m_start_y;
        --m_step;
        return path_cmd_move_to;
    }

    if (m_step == 0) {
        *x = m_end_x;
        *y = m_end_y;
        --m_step;
        return path_cmd_line_to;
    }

    m_fx  += m_dfx;
    m_fy  += m_dfy;
    m_dfx += m_ddfx;
    m_dfy += m_ddfy;
    *x = m_fx;
    *y = m_fy;
    --m_step;
    return path_cmd_line_to;
}


//curve3_div
void curve3_div::init(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3)
{
    m_points.clear();
    m_distance_tolerance_square = FLT_TO_SCALAR(0.5f) / m_approximation_scale;
    m_distance_tolerance_square *= m_distance_tolerance_square;
    bezier(x1, y1, x2, y2, x3, y3);
    m_count = 0;
}

//------------------------------------------------------------------------
void curve3_div::recursive_bezier(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3, unsigned int level)
{
    if (level > curve_recursion_limit) {
        return;
    }

    // Calculate all the mid-points of the line segments
    //----------------------
    scalar x12   = (x1 + x2) / 2;
    scalar y12   = (y1 + y2) / 2;
    scalar x23   = (x2 + x3) / 2;
    scalar y23   = (y2 + y3) / 2;
    scalar x123  = (x12 + x23) / 2;
    scalar y123  = (y12 + y23) / 2;

    scalar dx = x3-x1;
    scalar dy = y3-y1;
    scalar d = Fabs(((x2 - x3) * dy - (y2 - y3) * dx));
    scalar da;

    if (d > curve_collinearity_epsilon) {
        // Regular case
        //-----------------
        if (d * d <= m_distance_tolerance_square * (dx*dx + dy*dy)) {
            // If the curvature doesn't exceed the distance_tolerance value
            // we tend to finish subdivisions.
            //----------------------
            if (m_angle_tolerance < curve_angle_tolerance_epsilon) {
                m_points.add(vertex_s(x123, y123));
                return;
            }

            // Angle & Cusp Condition
            //----------------------
            da = Fabs(Atan2(y3 - y2, x3 - x2) - Atan2(y2 - y1, x2 - x1));
            if (da >= PI)
                da = _2PI - da;

            if (da < m_angle_tolerance) {
                // Finally we can stop the recursion
                //----------------------
                m_points.add(vertex_s(x123, y123));
                return;
            }
        }
    } else {
        // Collinear case
        //------------------
        da = dx*dx + dy*dy;
        if (da == 0) {
            d = calc_sq_distance(x1, y1, x2, y2);
        } else {
            d = ((x2 - x1)*dx + (y2 - y1)*dy) / da;
            if (d > 0 && d < 1) {
                // Simple collinear case, 1---2---3
                // We can leave just two endpoints
                return;
            }

            if (d <= 0)
                d = calc_sq_distance(x2, y2, x1, y1);
            else if (d >= 1)
                d = calc_sq_distance(x2, y2, x3, y3);
            else
                d = calc_sq_distance(x2, y2, x1 + d*dx, y1 + d*dy);
        }

        if (d < m_distance_tolerance_square) {
            m_points.add(vertex_s(x2, y2));
            return;
        }
    }

    // Continue subdivision
    //----------------------
    recursive_bezier(x1, y1, x12, y12, x123, y123, level + 1);
    recursive_bezier(x123, y123, x23, y23, x3, y3, level + 1);
}

//------------------------------------------------------------------------
void curve3_div::bezier(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3)
{
    m_points.add(vertex_s(x1, y1));
    recursive_bezier(x1, y1, x2, y2, x3, y3, 0);
    m_points.add(vertex_s(x3, y3));
}

//curve4_inc
void curve4_inc::init(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3, scalar x4, scalar y4)
{
    m_start_x = x1;
    m_start_y = y1;
    m_end_x   = x4;
    m_end_y   = y4;

    scalar dx1 = x2 - x1;
    scalar dy1 = y2 - y1;
    scalar dx2 = x3 - x2;
    scalar dy2 = y3 - y2;
    scalar dx3 = x4 - x3;
    scalar dy3 = y4 - y3;

    scalar len = (Sqrt(dx1 * dx1 + dy1 * dy1) +
                  Sqrt(dx2 * dx2 + dy2 * dy2) +
                  Sqrt(dx3 * dx3 + dy3 * dy3)) * FLT_TO_SCALAR(0.25f) * m_scale;

    m_num_steps = uround(len);

    if (m_num_steps < 4) {
        m_num_steps = 4;
    }

    scalar subdivide_step  = FLT_TO_SCALAR(1.0f) / m_num_steps;
    scalar subdivide_step2 = subdivide_step * subdivide_step;
    scalar subdivide_step3 = subdivide_step * subdivide_step * subdivide_step;

    scalar pre1 = FLT_TO_SCALAR(3.0f) * subdivide_step;
    scalar pre2 = FLT_TO_SCALAR(3.0f) * subdivide_step2;
    scalar pre4 = FLT_TO_SCALAR(6.0f) * subdivide_step2;
    scalar pre5 = FLT_TO_SCALAR(6.0f) * subdivide_step3;

    scalar tmp1x = x1 - x2 * FLT_TO_SCALAR(2.0f) + x3;
    scalar tmp1y = y1 - y2 * FLT_TO_SCALAR(2.0f) + y3;

    scalar tmp2x = (x2 - x3) * FLT_TO_SCALAR(3.0f) - x1 + x4;
    scalar tmp2y = (y2 - y3) * FLT_TO_SCALAR(3.0f) - y1 + y4;

    m_saved_fx = m_fx = x1;
    m_saved_fy = m_fy = y1;

    m_saved_dfx = m_dfx = (x2 - x1) * pre1 + tmp1x * pre2 + tmp2x * subdivide_step3;
    m_saved_dfy = m_dfy = (y2 - y1) * pre1 + tmp1y * pre2 + tmp2y * subdivide_step3;

    m_saved_ddfx = m_ddfx = tmp1x * pre4 + tmp2x * pre5;
    m_saved_ddfy = m_ddfy = tmp1y * pre4 + tmp2y * pre5;

    m_dddfx = tmp2x * pre5;
    m_dddfy = tmp2y * pre5;

    m_step = m_num_steps;
}

void curve4_inc::rewind(unsigned int)
{
    if (m_num_steps == 0) {
        m_step = -1;
        return;
    }
    m_step = m_num_steps;
    m_fx   = m_saved_fx;
    m_fy   = m_saved_fy;
    m_dfx  = m_saved_dfx;
    m_dfy  = m_saved_dfy;
    m_ddfx = m_saved_ddfx;
    m_ddfy = m_saved_ddfy;
}

unsigned int curve4_inc::vertex(scalar* x, scalar* y)
{
    if (m_step < 0)
        return path_cmd_stop;

    if (m_step == m_num_steps) {
        *x = m_start_x;
        *y = m_start_y;
        --m_step;
        return path_cmd_move_to;
    }

    if (m_step == 0) {
        *x = m_end_x;
        *y = m_end_y;
        --m_step;
        return path_cmd_line_to;
    }

    m_fx   += m_dfx;
    m_fy   += m_dfy;
    m_dfx  += m_ddfx;
    m_dfy  += m_ddfy;
    m_ddfx += m_dddfx;
    m_ddfy += m_dddfy;

    *x = m_fx;
    *y = m_fy;
    --m_step;
    return path_cmd_line_to;
}

//curve4_div
void curve4_div::init(scalar x1, scalar y1,
        scalar x2, scalar y2,
        scalar x3, scalar y3,
        scalar x4, scalar y4)
{
    m_points.clear();
    m_distance_tolerance_square = FLT_TO_SCALAR(0.5f) / m_approximation_scale;
    m_distance_tolerance_square *= m_distance_tolerance_square;
    bezier(x1, y1, x2, y2, x3, y3, x4, y4);
    m_count = 0;
}

void curve4_div::recursive_bezier(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3, scalar x4, scalar y4, unsigned int level)
{
    if (level > curve_recursion_limit) {
        return;
    }

    // Calculate all the mid-points of the line segments
    //----------------------
    scalar x12   = (x1 + x2) / 2;
    scalar y12   = (y1 + y2) / 2;
    scalar x23   = (x2 + x3) / 2;
    scalar y23   = (y2 + y3) / 2;
    scalar x34   = (x3 + x4) / 2;
    scalar y34   = (y3 + y4) / 2;
    scalar x123  = (x12 + x23) / 2;
    scalar y123  = (y12 + y23) / 2;
    scalar x234  = (x23 + x34) / 2;
    scalar y234  = (y23 + y34) / 2;
    scalar x1234 = (x123 + x234) / 2;
    scalar y1234 = (y123 + y234) / 2;


    // Try to approximate the full cubic curve by a single straight line
    //------------------
    scalar dx = x4-x1;
    scalar dy = y4-y1;

    scalar d2 = Fabs(((x2 - x4) * dy - (y2 - y4) * dx));
    scalar d3 = Fabs(((x3 - x4) * dy - (y3 - y4) * dx));
    scalar da1, da2, k;

    switch((int(d2 > curve_collinearity_epsilon) << 1) + int(d3 > curve_collinearity_epsilon))
    {
    case 0:
        // All collinear OR p1==p4
        //----------------------
        k = dx*dx + dy*dy;
        if (k == 0) {
            d2 = calc_sq_distance(x1, y1, x2, y2);
            d3 = calc_sq_distance(x4, y4, x3, y3);
        } else {
            k   = 1 / k;
            da1 = x2 - x1;
            da2 = y2 - y1;
            d2  = k * (da1*dx + da2*dy);
            da1 = x3 - x1;
            da2 = y3 - y1;
            d3  = k * (da1*dx + da2*dy);
            if (d2 > 0 && d2 < 1 && d3 > 0 && d3 < 1) {
                // Simple collinear case, 1---2---3---4
                // We can leave just two endpoints
                return;
            }

            if (d2 <= 0)
                d2 = calc_sq_distance(x2, y2, x1, y1);
            else if(d2 >= 1)
                d2 = calc_sq_distance(x2, y2, x4, y4);
            else
                d2 = calc_sq_distance(x2, y2, x1 + d2*dx, y1 + d2*dy);

            if (d3 <= 0)
                d3 = calc_sq_distance(x3, y3, x1, y1);
            else if(d3 >= 1)
                d3 = calc_sq_distance(x3, y3, x4, y4);
            else
                d3 = calc_sq_distance(x3, y3, x1 + d3*dx, y1 + d3*dy);
        }

        if (d2 > d3) {
            if (d2 < m_distance_tolerance_square) {
                m_points.add(vertex_s(x2, y2));
                return;
            }
        } else {
            if (d3 < m_distance_tolerance_square) {
                m_points.add(vertex_s(x3, y3));
                return;
            }
        }
        break;

    case 1:
        // p1,p2,p4 are collinear, p3 is significant
        //----------------------
        if (d3 * d3 <= m_distance_tolerance_square * (dx*dx + dy*dy)) {
            if (m_angle_tolerance < curve_angle_tolerance_epsilon) {
                m_points.add(vertex_s(x23, y23));
                return;
            }

            // Angle Condition
            //----------------------
            da1 = Fabs(Atan2(y4 - y3, x4 - x3) - Atan2(y3 - y2, x3 - x2));
            if (da1 >= PI)
                da1 = _2PI - da1;

            if (da1 < m_angle_tolerance) {
                m_points.add(vertex_s(x2, y2));
                m_points.add(vertex_s(x3, y3));
                return;
            }

            if (m_cusp_limit != 0.0) {
                if (da1 > m_cusp_limit) {
                    m_points.add(vertex_s(x3, y3));
                    return;
                }
            }
        }
        break;

    case 2:
        // p1,p3,p4 are collinear, p2 is significant
        //----------------------
        if (d2 * d2 <= m_distance_tolerance_square * (dx*dx + dy*dy)) {
            if (m_angle_tolerance < curve_angle_tolerance_epsilon) {
                m_points.add(vertex_s(x23, y23));
                return;
            }

            // Angle Condition
            //----------------------
            da1 = Fabs(Atan2(y3 - y2, x3 - x2) - Atan2(y2 - y1, x2 - x1));
            if (da1 >= PI)
                da1 = _2PI - da1;

            if (da1 < m_angle_tolerance) {
                m_points.add(vertex_s(x2, y2));
                m_points.add(vertex_s(x3, y3));
                return;
            }

            if (m_cusp_limit != 0.0) {
                if (da1 > m_cusp_limit) {
                    m_points.add(vertex_s(x2, y2));
                    return;
                }
            }
        }
        break;

    case 3:
        // Regular case
        //-----------------
        if ((d2 + d3)*(d2 + d3) <= m_distance_tolerance_square * (dx*dx + dy*dy)) {
            // If the curvature doesn't exceed the distance_tolerance value
            // we tend to finish subdivisions.
            //----------------------
            if (m_angle_tolerance < curve_angle_tolerance_epsilon) {
                m_points.add(vertex_s(x23, y23));
                return;
            }

            // Angle & Cusp Condition
            //----------------------
            k   = Atan2(y3 - y2, x3 - x2);
            da1 = Fabs(k - Atan2(y2 - y1, x2 - x1));
            da2 = Fabs(Atan2(y4 - y3, x4 - x3) - k);
            if (da1 >= PI)
                da1 = _2PI - da1;
            if (da2 >= PI)
                da2 = _2PI - da2;

            if (da1 + da2 < m_angle_tolerance) {
                // Finally we can stop the recursion
                //----------------------
                m_points.add(vertex_s(x23, y23));
                return;
            }

            if (m_cusp_limit != 0.0) {
                if (da1 > m_cusp_limit) {
                    m_points.add(vertex_s(x2, y2));
                    return;
                }

                if (da2 > m_cusp_limit) {
                    m_points.add(vertex_s(x3, y3));
                    return;
                }
            }
        }
        break;
    }

    // Continue subdivision
    //----------------------
    recursive_bezier(x1, y1, x12, y12, x123, y123, x1234, y1234, level + 1);
    recursive_bezier(x1234, y1234, x234, y234, x34, y34, x4, y4, level + 1);
}

void curve4_div::bezier(scalar x1, scalar y1, scalar x2, scalar y2, scalar x3, scalar y3, scalar x4, scalar y4)
{
    m_points.add(vertex_s(x1, y1));
    recursive_bezier(x1, y1, x2, y2, x3, y3, x4, y4, 0);
    m_points.add(vertex_s(x4, y4));
}

}
