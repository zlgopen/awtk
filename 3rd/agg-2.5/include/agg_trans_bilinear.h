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

#ifndef AGG_TRANS_BILINEAR_INCLUDED
#define AGG_TRANS_BILINEAR_INCLUDED

#include "agg_basics.h"
#include "agg_simul_eq.h"

namespace agg
{

    //==========================================================trans_bilinear
    class trans_bilinear
    {
    public:
        //--------------------------------------------------------------------
        trans_bilinear() : m_valid(false) {}

        //--------------------------------------------------------------------
        // Arbitrary quadrangle transformations
        trans_bilinear(const float_t* src, const float_t* dst) 
        {
            quad_to_quad(src, dst);
        }


        //--------------------------------------------------------------------
        // Direct transformations 
        trans_bilinear(float_t x1, float_t y1, float_t x2, float_t y2, 
                       const float_t* quad)
        {
            rect_to_quad(x1, y1, x2, y2, quad);
        }


        //--------------------------------------------------------------------
        // Reverse transformations 
        trans_bilinear(const float_t* quad, 
                       float_t x1, float_t y1, float_t x2, float_t y2)
        {
            quad_to_rect(quad, x1, y1, x2, y2);
        }


        //--------------------------------------------------------------------
        // Set the transformations using two arbitrary quadrangles.
        void quad_to_quad(const float_t* src, const float_t* dst)
        {
            float_t left[4][4];
            float_t right[4][2];

            unsigned i;
            for(i = 0; i < 4; i++)
            {
                unsigned ix = i * 2;
                unsigned iy = ix + 1;
                left[i][0] = 1.0;
                left[i][1] = src[ix] * src[iy];
                left[i][2] = src[ix];
                left[i][3] = src[iy];

                right[i][0] = dst[ix];
                right[i][1] = dst[iy];
            }
            m_valid = simul_eq<4, 2>::solve(left, right, m_mtx);
        }


        //--------------------------------------------------------------------
        // Set the direct transformations, i.e., rectangle -> quadrangle
        void rect_to_quad(float_t x1, float_t y1, float_t x2, float_t y2, 
                          const float_t* quad)
        {
            float_t src[8];
            src[0] = src[6] = x1;
            src[2] = src[4] = x2;
            src[1] = src[3] = y1;
            src[5] = src[7] = y2;
            quad_to_quad(src, quad);
        }


        //--------------------------------------------------------------------
        // Set the reverse transformations, i.e., quadrangle -> rectangle
        void quad_to_rect(const float_t* quad, 
                          float_t x1, float_t y1, float_t x2, float_t y2)
        {
            float_t dst[8];
            dst[0] = dst[6] = x1;
            dst[2] = dst[4] = x2;
            dst[1] = dst[3] = y1;
            dst[5] = dst[7] = y2;
            quad_to_quad(quad, dst);
        }

        //--------------------------------------------------------------------
        // Check if the equations were solved successfully
        bool is_valid() const { return m_valid; }

        //--------------------------------------------------------------------
        // Transform a point (x, y)
        void transform(float_t* x, float_t* y) const
        {
            float_t tx = *x;
            float_t ty = *y;
            float_t xy = tx * ty;
            *x = m_mtx[0][0] + m_mtx[1][0] * xy + m_mtx[2][0] * tx + m_mtx[3][0] * ty;
            *y = m_mtx[0][1] + m_mtx[1][1] * xy + m_mtx[2][1] * tx + m_mtx[3][1] * ty;
        }


        //--------------------------------------------------------------------
        class iterator_x
        {
            float_t inc_x;
            float_t inc_y;

        public:
            float_t x;
            float_t y;

            iterator_x() {}
            iterator_x(float_t tx, float_t ty, float_t step, const float_t m[4][2]) :
                inc_x(m[1][0] * step * ty + m[2][0] * step),
                inc_y(m[1][1] * step * ty + m[2][1] * step),
                x(m[0][0] + m[1][0] * tx * ty + m[2][0] * tx + m[3][0] * ty),
                y(m[0][1] + m[1][1] * tx * ty + m[2][1] * tx + m[3][1] * ty)
            {
            }

            void operator ++ ()
            {
                x += inc_x;
                y += inc_y;
            }
        };

        iterator_x begin(float_t x, float_t y, float_t step) const
        {
            return iterator_x(x, y, step, m_mtx);
        }

    private:
        float_t m_mtx[4][2];
        bool   m_valid;
    };

}

#endif
