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

#ifndef AGG_SLIDER_CTRL_INCLUDED
#define AGG_SLIDER_CTRL_INCLUDED

#include "agg_basics.h"
#include "agg_math.h"
#include "agg_ellipse.h"
#include "agg_trans_affine.h"
#include "agg_color_rgba.h"
#include "agg_gsv_text.h"
#include "agg_conv_stroke.h"
#include "agg_path_storage.h"
#include "agg_ctrl.h"


namespace agg
{

    //--------------------------------------------------------slider_ctrl_impl
    class slider_ctrl_impl : public ctrl
    {
    public:
        slider_ctrl_impl(float_t x1, float_t y1, float_t x2, float_t y2, bool flip_y=false);

        void border_width(float_t t, float_t extra=0.0);

        void range(float_t min, float_t max) { m_min = min; m_max = max; }
        void num_steps(unsigned num) { m_num_steps = num; }
        void label(const char* fmt);
        void text_thickness(float_t t) { m_text_thickness = t; }

        bool descending() const { return m_descending; }
        void descending(bool v) { m_descending = v; }

        float_t value() const { return m_value * (m_max - m_min) + m_min; }
        void value(float_t value);

        virtual bool in_rect(float_t x, float_t y) const;
        virtual bool on_mouse_button_down(float_t x, float_t y);
        virtual bool on_mouse_button_up(float_t x, float_t y);
        virtual bool on_mouse_move(float_t x, float_t y, bool button_flag);
        virtual bool on_arrow_keys(bool left, bool right, bool down, bool up);

        // Vertex source interface
        unsigned num_paths() { return 6; };
        void     rewind(unsigned path_id);
        unsigned vertex(float_t* x, float_t* y);

    private:
        void calc_box();
        bool normalize_value(bool preview_value_flag);

        float_t   m_border_width;
        float_t   m_border_extra;
        float_t   m_text_thickness;
        float_t   m_value;
        float_t   m_preview_value;
        float_t   m_min;
        float_t   m_max;
        unsigned m_num_steps;
        bool     m_descending;
        char     m_label[64];
        float_t   m_xs1;
        float_t   m_ys1;
        float_t   m_xs2;
        float_t   m_ys2;
        float_t   m_pdx;
        bool     m_mouse_move;
        float_t   m_vx[32];
        float_t   m_vy[32];

        ellipse  m_ellipse;

        unsigned m_idx;
        unsigned m_vertex;

        gsv_text              m_text;
        conv_stroke<gsv_text> m_text_poly;
        path_storage          m_storage;

    };



    //----------------------------------------------------------slider_ctrl
    template<class ColorT> class slider_ctrl : public slider_ctrl_impl
    {
    public:
        slider_ctrl(float_t x1, float_t y1, float_t x2, float_t y2, bool flip_y=false) :
            slider_ctrl_impl(x1, y1, x2, y2, flip_y),
            m_background_color(rgba(1.0, 0.9, 0.8)),
            m_triangle_color(rgba(0.7, 0.6, 0.6)),
            m_text_color(rgba(0.0, 0.0, 0.0)),
            m_pointer_preview_color(rgba(0.6, 0.4, 0.4, 0.4)),
            m_pointer_color(rgba(0.8, 0.0, 0.0, 0.6))
        {
            m_colors[0] = &m_background_color;
            m_colors[1] = &m_triangle_color;
            m_colors[2] = &m_text_color;
            m_colors[3] = &m_pointer_preview_color;
            m_colors[4] = &m_pointer_color;
            m_colors[5] = &m_text_color;
        }
          

        void background_color(const ColorT& c) { m_background_color = c; }
        void pointer_color(const ColorT& c) { m_pointer_color = c; }

        const ColorT& color(unsigned i) const { return *m_colors[i]; } 

    private:
        slider_ctrl(const slider_ctrl<ColorT>&);
        const slider_ctrl<ColorT>& operator = (const slider_ctrl<ColorT>&);

        ColorT m_background_color;
        ColorT m_triangle_color;
        ColorT m_text_color;
        ColorT m_pointer_preview_color;
        ColorT m_pointer_color;
        ColorT* m_colors[6];
    };





}



#endif

