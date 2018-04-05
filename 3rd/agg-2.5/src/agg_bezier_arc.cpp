//----------------------------------------------------------------------------
// Anti-Grain Geometry (AGG) - Version 2.5
// A high quality rendering engine for C++
// Copyright (C) 2002-2006 Maxim Shemanarev
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://antigrain.com
// 
// AGG is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// AGG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with AGG; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
// MA 02110-1301, USA.
//----------------------------------------------------------------------------

#include <math.h>
#include "agg_bezier_arc.h"


namespace agg
{

    // This epsilon is used to prevent us from adding degenerate curves 
    // (converging to a single point).
    // The value isn't very critical. Function arc_to_bezier() has a limit 
    // of the sweep_angle. If fabs(sweep_angle) exceeds pi/2 the curve 
    // becomes inaccurate. But slight exceeding is quite appropriate.
    //-------------------------------------------------bezier_arc_angle_epsilon
    const float_t bezier_arc_angle_epsilon = 0.01;

    //------------------------------------------------------------arc_to_bezier
    void arc_to_bezier(float_t cx, float_t cy, float_t rx, float_t ry, 
                       float_t start_angle, float_t sweep_angle,
                       float_t* curve)
    {
        float_t x0 = cos(sweep_angle / 2.0);
        float_t y0 = sin(sweep_angle / 2.0);
        float_t tx = (1.0 - x0) * 4.0 / 3.0;
        float_t ty = y0 - tx * x0 / y0;
        float_t px[4];
        float_t py[4];
        px[0] =  x0;
        py[0] = -y0;
        px[1] =  x0 + tx;
        py[1] = -ty;
        px[2] =  x0 + tx;
        py[2] =  ty;
        px[3] =  x0;
        py[3] =  y0;

        float_t sn = sin(start_angle + sweep_angle / 2.0);
        float_t cs = cos(start_angle + sweep_angle / 2.0);

        unsigned i;
        for(i = 0; i < 4; i++)
        {
            curve[i * 2]     = cx + rx * (px[i] * cs - py[i] * sn);
            curve[i * 2 + 1] = cy + ry * (px[i] * sn + py[i] * cs);
        }
    }



    //------------------------------------------------------------------------
    void bezier_arc::init(float_t x,  float_t y, 
                          float_t rx, float_t ry, 
                          float_t start_angle, 
                          float_t sweep_angle)
    {
        start_angle = fmod(start_angle, 2.0 * pi);
        if(sweep_angle >=  2.0 * pi) sweep_angle =  2.0 * pi;
        if(sweep_angle <= -2.0 * pi) sweep_angle = -2.0 * pi;

        if(fabs(sweep_angle) < 1e-10)
        {
            m_num_vertices = 4;
            m_cmd = path_cmd_line_to;
            m_vertices[0] = x + rx * cos(start_angle);
            m_vertices[1] = y + ry * sin(start_angle);
            m_vertices[2] = x + rx * cos(start_angle + sweep_angle);
            m_vertices[3] = y + ry * sin(start_angle + sweep_angle);
            return;
        }

        float_t total_sweep = 0.0;
        float_t local_sweep = 0.0;
        float_t prev_sweep;
        m_num_vertices = 2;
        m_cmd = path_cmd_curve4;
        bool done = false;
        do
        {
            if(sweep_angle < 0.0)
            {
                prev_sweep  = total_sweep;
                local_sweep = -pi * 0.5;
                total_sweep -= pi * 0.5;
                if(total_sweep <= sweep_angle + bezier_arc_angle_epsilon)
                {
                    local_sweep = sweep_angle - prev_sweep;
                    done = true;
                }
            }
            else
            {
                prev_sweep  = total_sweep;
                local_sweep =  pi * 0.5;
                total_sweep += pi * 0.5;
                if(total_sweep >= sweep_angle - bezier_arc_angle_epsilon)
                {
                    local_sweep = sweep_angle - prev_sweep;
                    done = true;
                }
            }

            arc_to_bezier(x, y, rx, ry, 
                          start_angle, 
                          local_sweep, 
                          m_vertices + m_num_vertices - 2);

            m_num_vertices += 6;
            start_angle += local_sweep;
        }
        while(!done && m_num_vertices < 26);
    }




    //--------------------------------------------------------------------
    void bezier_arc_svg::init(float_t x0, float_t y0, 
                              float_t rx, float_t ry, 
                              float_t angle,
                              bool large_arc_flag,
                              bool sweep_flag,
                              float_t x2, float_t y2)
    {
        m_radii_ok = true;

        if(rx < 0.0) rx = -rx;
        if(ry < 0.0) ry = -rx;

        // Calculate the middle point between 
        // the current and the final points
        //------------------------
        float_t dx2 = (x0 - x2) / 2.0;
        float_t dy2 = (y0 - y2) / 2.0;

        float_t cos_a = cos(angle);
        float_t sin_a = sin(angle);

        // Calculate (x1, y1)
        //------------------------
        float_t x1 =  cos_a * dx2 + sin_a * dy2;
        float_t y1 = -sin_a * dx2 + cos_a * dy2;

        // Ensure radii are large enough
        //------------------------
        float_t prx = rx * rx;
        float_t pry = ry * ry;
        float_t px1 = x1 * x1;
        float_t py1 = y1 * y1;

        // Check that radii are large enough
        //------------------------
        float_t radii_check = px1/prx + py1/pry;
        if(radii_check > 1.0) 
        {
            rx = sqrt(radii_check) * rx;
            ry = sqrt(radii_check) * ry;
            prx = rx * rx;
            pry = ry * ry;
            if(radii_check > 10.0) m_radii_ok = false;
        }

        // Calculate (cx1, cy1)
        //------------------------
        float_t sign = (large_arc_flag == sweep_flag) ? -1.0 : 1.0;
        float_t sq   = (prx*pry - prx*py1 - pry*px1) / (prx*py1 + pry*px1);
        float_t coef = sign * sqrt((sq < 0) ? 0 : sq);
        float_t cx1  = coef *  ((rx * y1) / ry);
        float_t cy1  = coef * -((ry * x1) / rx);

        //
        // Calculate (cx, cy) from (cx1, cy1)
        //------------------------
        float_t sx2 = (x0 + x2) / 2.0;
        float_t sy2 = (y0 + y2) / 2.0;
        float_t cx = sx2 + (cos_a * cx1 - sin_a * cy1);
        float_t cy = sy2 + (sin_a * cx1 + cos_a * cy1);

        // Calculate the start_angle (angle1) and the sweep_angle (dangle)
        //------------------------
        float_t ux =  (x1 - cx1) / rx;
        float_t uy =  (y1 - cy1) / ry;
        float_t vx = (-x1 - cx1) / rx;
        float_t vy = (-y1 - cy1) / ry;
        float_t p, n;

        // Calculate the angle start
        //------------------------
        n = sqrt(ux*ux + uy*uy);
        p = ux; // (1 * ux) + (0 * uy)
        sign = (uy < 0) ? -1.0 : 1.0;
        float_t v = p / n;
        if(v < -1.0) v = -1.0;
        if(v >  1.0) v =  1.0;
        float_t start_angle = sign * acos(v);

        // Calculate the sweep angle
        //------------------------
        n = sqrt((ux*ux + uy*uy) * (vx*vx + vy*vy));
        p = ux * vx + uy * vy;
        sign = (ux * vy - uy * vx < 0) ? -1.0 : 1.0;
        v = p / n;
        if(v < -1.0) v = -1.0;
        if(v >  1.0) v =  1.0;
        float_t sweep_angle = sign * acos(v);
        if(!sweep_flag && sweep_angle > 0) 
        {
            sweep_angle -= pi * 2.0;
        } 
        else 
        if (sweep_flag && sweep_angle < 0) 
        {
            sweep_angle += pi * 2.0;
        }

        // We can now build and transform the resulting arc
        //------------------------
        m_arc.init(0.0, 0.0, rx, ry, start_angle, sweep_angle);
        trans_affine mtx = trans_affine_rotation(angle);
        mtx *= trans_affine_translation(cx, cy);
        
        for(unsigned i = 2; i < m_arc.num_vertices()-2; i += 2)
        {
            mtx.transform(m_arc.vertices() + i, m_arc.vertices() + i + 1);
        }

        // We must make sure that the starting and ending points
        // exactly coincide with the initial (x0,y0) and (x2,y2)
        m_arc.vertices()[0] = x0;
        m_arc.vertices()[1] = y0;
        if(m_arc.num_vertices() > 2)
        {
            m_arc.vertices()[m_arc.num_vertices() - 2] = x2;
            m_arc.vertices()[m_arc.num_vertices() - 1] = y2;
        }
    }


}
