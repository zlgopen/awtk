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

#ifndef AGG_SCALE_CTRL_INCLUDED
#define AGG_SCALE_CTRL_INCLUDED

#include "agg_basics.h"
#include "agg_math.h"
#include "agg_ellipse.h"
#include "agg_trans_affine.h"
#include "agg_color_rgba.h"
#include "agg_ctrl.h"


namespace agg
{

    //------------------------------------------------------------------------
    class scale_ctrl_impl : public ctrl
    {
        enum move_e
        {
            move_nothing,
            move_value1,
            move_value2,
            move_slider
        };

    public:
        scale_ctrl_impl(float_t x1, float_t y1, float_t x2, float_t y2, bool flip_y=false);

        void border_thickness(float_t t, float_t extra=0.0);
        void resize(float_t x1, float_t y1, float_t x2, float_t y2);
        
        float_t min_delta() const { return m_min_d; }
        void min_delta(float_t d) { m_min_d = d; }
        
        float_t value1() const { return m_value1; }
        void value1(float_t value);

        float_t value2() const { return m_value2; }
        void value2(float_t value);

        void move(float_t d);

        virtual bool in_rect(float_t x, float_t y) const;
        virtual bool on_mouse_button_down(float_t x, float_t y);
        virtual bool on_mouse_button_up(float_t x, float_t y);
        virtual bool on_mouse_move(float_t x, float_t y, bool button_flag);
        virtual bool on_arrow_keys(bool left, bool right, bool down, bool up);

        // Vertex soutce interface
        unsigned num_paths() { return 5; };
        void     rewind(unsigned path_id);
        unsigned vertex(float_t* x, float_t* y);

    private:
        void calc_box();

        float_t   m_border_thickness;
        float_t   m_border_extra;
        float_t   m_value1;
        float_t   m_value2;
        float_t   m_min_d;
        float_t   m_xs1;
        float_t   m_ys1;
        float_t   m_xs2;
        float_t   m_ys2;
        float_t   m_pdx;
        float_t   m_pdy;
        move_e   m_move_what;
        float_t   m_vx[32];
        float_t   m_vy[32];

        ellipse  m_ellipse;

        unsigned m_idx;
        unsigned m_vertex;

    };



    //------------------------------------------------------------------------
    template<class ColorT> class scale_ctrl : public scale_ctrl_impl
    {
    public:
        scale_ctrl(float_t x1, float_t y1, float_t x2, float_t y2, bool flip_y=false) :
            scale_ctrl_impl(x1, y1, x2, y2, flip_y),
            m_background_color(rgba(1.0, 0.9, 0.8)),
            m_border_color(rgba(0.0, 0.0, 0.0)),
            m_pointers_color(rgba(0.8, 0.0, 0.0, 0.8)),
            m_slider_color(rgba(0.2, 0.1, 0.0, 0.6))
        {
            m_colors[0] = &m_background_color;
            m_colors[1] = &m_border_color;
            m_colors[2] = &m_pointers_color;
            m_colors[3] = &m_pointers_color;
            m_colors[4] = &m_slider_color;
        }
          

        void background_color(const ColorT& c) { m_background_color = c; }
        void border_color(const ColorT& c)     { m_border_color = c; }
        void pointers_color(const ColorT& c)   { m_pointers_color = c; }
        void slider_color(const ColorT& c)     { m_slider_color = c; }

        const ColorT& color(unsigned i) const { return *m_colors[i]; } 

    private:
        scale_ctrl(const scale_ctrl<ColorT>&);
        const scale_ctrl<ColorT>& operator = (const scale_ctrl<ColorT>&);

        ColorT m_background_color;
        ColorT m_border_color;
        ColorT m_pointers_color;
        ColorT m_slider_color;
        ColorT* m_colors[5];
    };





}



#endif

