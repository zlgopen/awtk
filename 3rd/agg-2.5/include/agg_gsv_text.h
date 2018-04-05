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

#ifndef AGG_GSV_TEXT_INCLUDED
#define AGG_GSV_TEXT_INCLUDED

#include "agg_array.h"
#include "agg_conv_stroke.h"
#include "agg_conv_transform.h"

namespace agg
{


    //---------------------------------------------------------------gsv_text
    //
    // See Implementation agg_gsv_text.cpp 
    //
    class gsv_text
    {
        enum status
        {
            initial,
            next_char,
            start_glyph,
            glyph
        };

    public:
        gsv_text();

        void font(const void* font);
        void flip(bool flip_y) { m_flip = flip_y; }
        void load_font(const char* file);
        void size(float_t height, float_t width=0.0);
        void space(float_t space);
        void line_space(float_t line_space);
        void start_point(float_t x, float_t y);
        void text(const char* text);
        
        float_t text_width();

        void rewind(unsigned path_id);
        unsigned vertex(float_t* x, float_t* y);

    private:
        // not supposed to be copied
        gsv_text(const gsv_text&);
        const gsv_text& operator = (const gsv_text&);

        int16u value(const int8u* p) const
        {
            int16u v;
            if(m_big_endian)
            {
                 *(int8u*)&v      = p[1];
                *((int8u*)&v + 1) = p[0];
            }
            else
            {
                 *(int8u*)&v      = p[0];
                *((int8u*)&v + 1) = p[1];
            }
            return v;
        }

    private:
        float_t          m_x;
        float_t          m_y;
        float_t          m_start_x;
        float_t          m_width;
        float_t          m_height;
        float_t          m_space;
        float_t          m_line_space;
        char            m_chr[2];
        char*           m_text;
        pod_array<char> m_text_buf;
        char*           m_cur_chr;
        const void*     m_font;
        pod_array<char> m_loaded_font;
        status          m_status;
        bool            m_big_endian;
        bool            m_flip;
        int8u*          m_indices;
        int8*           m_glyphs;
        int8*           m_bglyph;
        int8*           m_eglyph;
        float_t          m_w;
        float_t          m_h;
    };




    //--------------------------------------------------------gsv_text_outline
    template<class Transformer = trans_affine> class gsv_text_outline
    {
    public:
        gsv_text_outline(gsv_text& text, const Transformer& trans) :
          m_polyline(text),
          m_trans(m_polyline, trans)
        {
        }

        void width(float_t w) 
        { 
            m_polyline.width(w); 
        }

        void transformer(const Transformer* trans) 
        {
            m_trans->transformer(trans);
        }

        void rewind(unsigned path_id) 
        { 
            m_trans.rewind(path_id); 
            m_polyline.line_join(round_join);
            m_polyline.line_cap(round_cap);
        }

        unsigned vertex(float_t* x, float_t* y)
        {
            return m_trans.vertex(x, y);
        }

    private:
        conv_stroke<gsv_text> m_polyline;
        conv_transform<conv_stroke<gsv_text>, Transformer> m_trans;
    };



}


#endif
