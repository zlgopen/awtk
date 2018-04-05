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

#ifndef AGG_ROUNDED_RECT_INCLUDED
#define AGG_ROUNDED_RECT_INCLUDED

#include "agg_basics.h"
#include "agg_arc.h"

namespace agg
{
    //------------------------------------------------------------rounded_rect
    //
    // See Implemantation agg_rounded_rect.cpp
    //
    class rounded_rect
    {
    public:
        rounded_rect() {}
        rounded_rect(float_t x1, float_t y1, float_t x2, float_t y2, float_t r);

        void rect(float_t x1, float_t y1, float_t x2, float_t y2);
        void radius(float_t r);
        void radius(float_t rx, float_t ry);
        void radius(float_t rx_bottom, float_t ry_bottom, float_t rx_top, float_t ry_top);
        void radius(float_t rx1, float_t ry1, float_t rx2, float_t ry2, 
                    float_t rx3, float_t ry3, float_t rx4, float_t ry4);
        void normalize_radius();

        void approximation_scale(float_t s) { m_arc.approximation_scale(s); }
        float_t approximation_scale() const { return m_arc.approximation_scale(); }

        void rewind(unsigned);
        unsigned vertex(float_t* x, float_t* y);

    private:
        float_t m_x1;
        float_t m_y1;
        float_t m_x2;
        float_t m_y2;
        float_t m_rx1;
        float_t m_ry1;
        float_t m_rx2;
        float_t m_ry2;
        float_t m_rx3;
        float_t m_ry3;
        float_t m_rx4;
        float_t m_ry4;
        unsigned m_status;
        arc      m_arc;
    };

}

#endif

