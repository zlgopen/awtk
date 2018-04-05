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

#ifndef AGG_BSPLINE_INCLUDED
#define AGG_BSPLINE_INCLUDED

#include "agg_array.h"

namespace agg
{
    //----------------------------------------------------------------bspline
    // A very simple class of Bi-cubic Spline interpolation.
    // First call init(num, x[], y[]) where num - number of source points, 
    // x, y - arrays of X and Y values respectively. Here Y must be a function 
    // of X. It means that all the X-coordinates must be arranged in the ascending
    // order. 
    // Then call get(x) that calculates a value Y for the respective X. 
    // The class supports extrapolation, i.e. you can call get(x) where x is
    // outside the given with init() X-range. Extrapolation is a simple linear 
    // function.
    //
    //  See Implementation agg_bspline.cpp
    //------------------------------------------------------------------------
    class bspline 
    {
    public:
        bspline();
        bspline(int num);
        bspline(int num, const float_t* x, const float_t* y);

        void   init(int num);
        void   add_point(float_t x, float_t y);
        void   prepare();

        void   init(int num, const float_t* x, const float_t* y);

        float_t get(float_t x) const;
        float_t get_stateful(float_t x) const;
    
    private:
        bspline(const bspline&);
        const bspline& operator = (const bspline&);

        static void bsearch(int n, const float_t *x, float_t x0, int *i);
        float_t extrapolation_left(float_t x) const;
        float_t extrapolation_right(float_t x) const;
        float_t interpolation(float_t x, int i) const;

        int               m_max;
        int               m_num;
        float_t*           m_x;
        float_t*           m_y;
        pod_array<float_t> m_am;
        mutable int       m_last_idx;
    };


}

#endif
