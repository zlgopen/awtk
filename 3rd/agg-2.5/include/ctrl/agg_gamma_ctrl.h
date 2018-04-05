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

#ifndef AGG_GAMMA_CTRL_INCLUDED
#define AGG_GAMMA_CTRL_INCLUDED

#include "agg_basics.h"
#include "agg_gamma_spline.h"
#include "agg_ellipse.h"
#include "agg_conv_stroke.h"
#include "agg_gsv_text.h"
#include "agg_trans_affine.h"
#include "agg_color_rgba.h"
#include "agg_ctrl.h"

namespace agg
{
    //------------------------------------------------------------------------
    // Class that can be used to create an interactive control to set up 
    // gamma arrays.
    //------------------------------------------------------------------------
    class gamma_ctrl_impl : public ctrl
    {
    public:
        gamma_ctrl_impl(float_t x1, float_t y1, float_t x2, float_t y2, bool flip_y=false);

        // Set other parameters
        void border_width(float_t t, float_t extra=0.0);
        void curve_width(float_t t)         { m_curve_width = t; }
        void grid_width(float_t t)          { m_grid_width = t; }
        void text_thickness(float_t t)      { m_text_thickness = t; }
        void text_size(float_t h, float_t w=0.0);
        void point_size(float_t s)              { m_point_size = s; }

        // Event handlers. Just call them if the respective events
        // in your system occure. The functions return true if redrawing
        // is required.
        virtual bool in_rect(float_t x, float_t y) const;
        virtual bool on_mouse_button_down(float_t x, float_t y);
        virtual bool on_mouse_button_up(float_t x, float_t y);
        virtual bool on_mouse_move(float_t x, float_t y, bool button_flag);
        virtual bool on_arrow_keys(bool left, bool right, bool down, bool up);

        void change_active_point();

        // A copy of agg::gamma_spline interface
        void  values(float_t kx1, float_t ky1, float_t kx2, float_t ky2);
        void  values(float_t* kx1, float_t* ky1, float_t* kx2, float_t* ky2) const;
        const unsigned char* gamma() const { return m_gamma_spline.gamma(); }
        float_t y(float_t x) const { return m_gamma_spline.y(x); }
        float_t operator() (float_t x) const { return m_gamma_spline.y(x); }
        const gamma_spline& get_gamma_spline() const { return m_gamma_spline; }

        // Vertex soutce interface
        unsigned num_paths() { return 7; }
        void     rewind(unsigned idx);
        unsigned vertex(float_t* x, float_t* y);

    private:
        void calc_spline_box();
        void calc_points();
        void calc_values();

        gamma_spline  m_gamma_spline;
        float_t m_border_width;
        float_t m_border_extra;
        float_t m_curve_width;
        float_t m_grid_width;
        float_t m_text_thickness;
        float_t m_point_size;
        float_t m_text_height;
        float_t m_text_width;
        float_t m_xc1;
        float_t m_yc1;
        float_t m_xc2;
        float_t m_yc2;
        float_t m_xs1;
        float_t m_ys1;
        float_t m_xs2;
        float_t m_ys2;
        float_t m_xt1;
        float_t m_yt1;
        float_t m_xt2;
        float_t m_yt2;
        conv_stroke<gamma_spline> m_curve_poly;
        ellipse                   m_ellipse;
        gsv_text                  m_text;
        conv_stroke<gsv_text>     m_text_poly;
        unsigned m_idx;
        unsigned m_vertex;
        float_t   m_vx[32];
        float_t   m_vy[32];
        float_t   m_xp1;
        float_t   m_yp1;
        float_t   m_xp2;
        float_t   m_yp2;
        bool     m_p1_active;
        unsigned m_mouse_point;
        float_t   m_pdx;
        float_t   m_pdy;
    };



    template<class ColorT> class gamma_ctrl : public gamma_ctrl_impl
    {
    public:
        gamma_ctrl(float_t x1, float_t y1, float_t x2, float_t y2, bool flip_y=false) :
            gamma_ctrl_impl(x1, y1, x2, y2, flip_y),
            m_background_color(rgba(1.0, 1.0, 0.9)),
            m_border_color(rgba(0.0, 0.0, 0.0)),
            m_curve_color(rgba(0.0, 0.0, 0.0)),
            m_grid_color(rgba(0.2, 0.2, 0.0)),
            m_inactive_pnt_color(rgba(0.0, 0.0, 0.0)),
            m_active_pnt_color(rgba(1.0, 0.0, 0.0)),
            m_text_color(rgba(0.0, 0.0, 0.0))
        {
            m_colors[0] = &m_background_color;
            m_colors[1] = &m_border_color;
            m_colors[2] = &m_curve_color;
            m_colors[3] = &m_grid_color;
            m_colors[4] = &m_inactive_pnt_color;
            m_colors[5] = &m_active_pnt_color;
            m_colors[6] = &m_text_color;
        }

        // Set colors
        void background_color(const ColorT& c)   { m_background_color = c; }
        void border_color(const ColorT& c)       { m_border_color = c; }
        void curve_color(const ColorT& c)        { m_curve_color = c; }
        void grid_color(const ColorT& c)         { m_grid_color = c; }
        void inactive_pnt_color(const ColorT& c) { m_inactive_pnt_color = c; }
        void active_pnt_color(const ColorT& c)   { m_active_pnt_color = c; }
        void text_color(const ColorT& c)         { m_text_color = c; }
        const ColorT& color(unsigned i) const { return *m_colors[i]; } 

    private:
        gamma_ctrl(const gamma_ctrl<ColorT>&);
        const gamma_ctrl<ColorT>& operator = (const gamma_ctrl<ColorT>&);

        ColorT  m_background_color;
        ColorT  m_border_color;
        ColorT  m_curve_color;
        ColorT  m_grid_color;
        ColorT  m_inactive_pnt_color;
        ColorT  m_active_pnt_color;
        ColorT  m_text_color;
        ColorT* m_colors[7];
    };


}

#endif
