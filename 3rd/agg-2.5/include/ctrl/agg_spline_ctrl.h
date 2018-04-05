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

#ifndef AGG_SPLINE_CTRL_INCLUDED
#define AGG_SPLINE_CTRL_INCLUDED

#include "agg_basics.h"
#include "agg_ellipse.h"
#include "agg_bspline.h"
#include "agg_conv_stroke.h"
#include "agg_path_storage.h"
#include "agg_trans_affine.h"
#include "agg_color_rgba.h"
#include "agg_ctrl.h"

namespace agg
{

    //------------------------------------------------------------------------
    // Class that can be used to create an interactive control to set up 
    // gamma arrays.
    //------------------------------------------------------------------------
    class spline_ctrl_impl : public ctrl
    {
    public:
        spline_ctrl_impl(float_t x1, float_t y1, float_t x2, float_t y2, 
                         unsigned num_pnt, bool flip_y=false);

        // Set other parameters
        void border_width(float_t t, float_t extra=0.0);
        void curve_width(float_t t) { m_curve_width = t; }
        void point_size(float_t s)  { m_point_size = s; }

        // Event handlers. Just call them if the respective events
        // in your system occure. The functions return true if redrawing
        // is required.
        virtual bool in_rect(float_t x, float_t y) const;
        virtual bool on_mouse_button_down(float_t x, float_t y);
        virtual bool on_mouse_button_up(float_t x, float_t y);
        virtual bool on_mouse_move(float_t x, float_t y, bool button_flag);
        virtual bool on_arrow_keys(bool left, bool right, bool down, bool up);

        void active_point(int i);

        const float_t* spline()  const { return m_spline_values;  }
        const int8u*  spline8() const { return m_spline_values8; }
        float_t value(float_t x) const;
        void   value(unsigned idx, float_t y);
        void   point(unsigned idx, float_t x, float_t y);
        void   x(unsigned idx, float_t x) { m_xp[idx] = x; }
        void   y(unsigned idx, float_t y) { m_yp[idx] = y; }
        float_t x(unsigned idx) const { return m_xp[idx]; }
        float_t y(unsigned idx) const { return m_yp[idx]; }
        void  update_spline();

        // Vertex soutce interface
        unsigned num_paths() { return 5; }
        void     rewind(unsigned path_id);
        unsigned vertex(float_t* x, float_t* y);

    private:
        void calc_spline_box();
        void calc_curve();
        float_t calc_xp(unsigned idx);
        float_t calc_yp(unsigned idx);
        void set_xp(unsigned idx, float_t val);
        void set_yp(unsigned idx, float_t val);

        unsigned m_num_pnt;
        float_t   m_xp[32];
        float_t   m_yp[32];
        bspline  m_spline;
        float_t   m_spline_values[256];
        int8u    m_spline_values8[256];
        float_t   m_border_width;
        float_t   m_border_extra;
        float_t   m_curve_width;
        float_t   m_point_size;
        float_t   m_xs1;
        float_t   m_ys1;
        float_t   m_xs2;
        float_t   m_ys2;
        path_storage              m_curve_pnt;
        conv_stroke<path_storage> m_curve_poly;
        ellipse                   m_ellipse;
        unsigned m_idx;
        unsigned m_vertex;
        float_t   m_vx[32];
        float_t   m_vy[32];
        int      m_active_pnt;
        int      m_move_pnt;
        float_t   m_pdx;
        float_t   m_pdy;
        const trans_affine* m_mtx;
    };


    template<class ColorT> class spline_ctrl : public spline_ctrl_impl
    {
    public:
        spline_ctrl(float_t x1, float_t y1, float_t x2, float_t y2, 
                    unsigned num_pnt, bool flip_y=false) :
            spline_ctrl_impl(x1, y1, x2, y2, num_pnt, flip_y),
            m_background_color(rgba(1.0, 1.0, 0.9)),
            m_border_color(rgba(0.0, 0.0, 0.0)),
            m_curve_color(rgba(0.0, 0.0, 0.0)),
            m_inactive_pnt_color(rgba(0.0, 0.0, 0.0)),
            m_active_pnt_color(rgba(1.0, 0.0, 0.0))
        {
            m_colors[0] = &m_background_color;
            m_colors[1] = &m_border_color;
            m_colors[2] = &m_curve_color;
            m_colors[3] = &m_inactive_pnt_color;
            m_colors[4] = &m_active_pnt_color;
        }

        // Set colors
        void background_color(const ColorT& c)   { m_background_color = c; }
        void border_color(const ColorT& c)       { m_border_color = c; }
        void curve_color(const ColorT& c)        { m_curve_color = c; }
        void inactive_pnt_color(const ColorT& c) { m_inactive_pnt_color = c; }
        void active_pnt_color(const ColorT& c)   { m_active_pnt_color = c; }
        const ColorT& color(unsigned i) const { return *m_colors[i]; } 

    private:
        spline_ctrl(const spline_ctrl<ColorT>&);
        const spline_ctrl<ColorT>& operator = (const spline_ctrl<ColorT>&);

        ColorT  m_background_color;
        ColorT  m_border_color;
        ColorT  m_curve_color;
        ColorT  m_inactive_pnt_color;
        ColorT  m_active_pnt_color;
        ColorT* m_colors[5];
    };




}


#endif
