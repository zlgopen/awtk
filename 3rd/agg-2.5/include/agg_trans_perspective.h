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

#ifndef AGG_TRANS_PERSPECTIVE_INCLUDED
#define AGG_TRANS_PERSPECTIVE_INCLUDED

#include "agg_trans_affine.h"

namespace agg
{
    //=======================================================trans_perspective
    struct trans_perspective
    {
        float_t sx, shy, w0, shx, sy, w1, tx, ty, w2;

        //------------------------------------------------------- Construction
        // Identity matrix
        trans_perspective() : 
            sx (1), shy(0), w0(0), 
            shx(0), sy (1), w1(0), 
            tx (0), ty (0), w2(1) {}

        // Custom matrix
        trans_perspective(float_t v0, float_t v1, float_t v2, 
                          float_t v3, float_t v4, float_t v5,
                          float_t v6, float_t v7, float_t v8) :
           sx (v0), shy(v1), w0(v2), 
           shx(v3), sy (v4), w1(v5), 
           tx (v6), ty (v7), w2(v8) {}

        // Custom matrix from m[9]
        explicit trans_perspective(const float_t* m) :
           sx (m[0]), shy(m[1]), w0(m[2]), 
           shx(m[3]), sy (m[4]), w1(m[5]), 
           tx (m[6]), ty (m[7]), w2(m[8]) {}

        // From affine
        explicit trans_perspective(const trans_affine& a) : 
           sx (a.sx ), shy(a.shy), w0(0), 
           shx(a.shx), sy (a.sy ), w1(0), 
           tx (a.tx ), ty (a.ty ), w2(1) {}

        // Rectangle to quadrilateral
        trans_perspective(float_t x1, float_t y1, float_t x2, float_t y2, 
                          const float_t* quad);

        // Quadrilateral to rectangle
        trans_perspective(const float_t* quad, 
                          float_t x1, float_t y1, float_t x2, float_t y2);

        // Arbitrary quadrilateral transformations
        trans_perspective(const float_t* src, const float_t* dst);

        //-------------------------------------- Quadrilateral transformations
        // The arguments are float_t[8] that are mapped to quadrilaterals:
        // x1,y1, x2,y2, x3,y3, x4,y4
        bool quad_to_quad(const float_t* qs, const float_t* qd);

        bool rect_to_quad(float_t x1, float_t y1, 
                          float_t x2, float_t y2,
                          const float_t* q);

        bool quad_to_rect(const float_t* q,
                          float_t x1, float_t y1, 
                          float_t x2, float_t y2);

        // Map square (0,0,1,1) to the quadrilateral and vice versa
        bool square_to_quad(const float_t* q);
        bool quad_to_square(const float_t* q);


        //--------------------------------------------------------- Operations
        // Reset - load an identity matrix
        const trans_perspective& reset();

        // Invert matrix. Returns false in degenerate case
        bool invert();

        // Direct transformations operations
        const trans_perspective& translate(float_t x, float_t y);
        const trans_perspective& rotate(float_t a);
        const trans_perspective& scale(float_t s);
        const trans_perspective& scale(float_t x, float_t y);

        // Multiply the matrix by another one
        const trans_perspective& multiply(const trans_perspective& m);

        // Multiply "m" by "this" and assign the result to "this"
        const trans_perspective& premultiply(const trans_perspective& m);

        // Multiply matrix to inverse of another one
        const trans_perspective& multiply_inv(const trans_perspective& m);

        // Multiply inverse of "m" by "this" and assign the result to "this"
        const trans_perspective& premultiply_inv(const trans_perspective& m);

        // Multiply the matrix by another one
        const trans_perspective& multiply(const trans_affine& m);

        // Multiply "m" by "this" and assign the result to "this"
        const trans_perspective& premultiply(const trans_affine& m);

        // Multiply the matrix by inverse of another one
        const trans_perspective& multiply_inv(const trans_affine& m);

        // Multiply inverse of "m" by "this" and assign the result to "this"
        const trans_perspective& premultiply_inv(const trans_affine& m);

        //--------------------------------------------------------- Load/Store
        void store_to(float_t* m) const;
        const trans_perspective& load_from(const float_t* m);

        //---------------------------------------------------------- Operators
        // Multiply the matrix by another one
        const trans_perspective& operator *= (const trans_perspective& m)
        {
            return multiply(m);
        }
        const trans_perspective& operator *= (const trans_affine& m)
        {
            return multiply(m);
        }

        // Multiply the matrix by inverse of another one
        const trans_perspective& operator /= (const trans_perspective& m)
        {
            return multiply_inv(m);
        }
        const trans_perspective& operator /= (const trans_affine& m)
        {
            return multiply_inv(m);
        }

        // Multiply the matrix by another one and return
        // the result in a separete matrix.
        trans_perspective operator * (const trans_perspective& m)
        {
            return trans_perspective(*this).multiply(m);
        }
        trans_perspective operator * (const trans_affine& m)
        {
            return trans_perspective(*this).multiply(m);
        }

        // Multiply the matrix by inverse of another one 
        // and return the result in a separete matrix.
        trans_perspective operator / (const trans_perspective& m)
        {
            return trans_perspective(*this).multiply_inv(m);
        }
        trans_perspective operator / (const trans_affine& m)
        {
            return trans_perspective(*this).multiply_inv(m);
        }

        // Calculate and return the inverse matrix
        trans_perspective operator ~ () const
        {
            trans_perspective ret = *this;
            ret.invert();
            return ret;
        }

        // Equal operator with default epsilon
        bool operator == (const trans_perspective& m) const
        {
            return is_equal(m, affine_epsilon);
        }

        // Not Equal operator with default epsilon
        bool operator != (const trans_perspective& m) const
        {
            return !is_equal(m, affine_epsilon);
        }

        //---------------------------------------------------- Transformations
        // Direct transformation of x and y
        void transform(float_t* x, float_t* y) const;

        // Direct transformation of x and y, affine part only
        void transform_affine(float_t* x, float_t* y) const;

        // Direct transformation of x and y, 2x2 matrix only, no translation
        void transform_2x2(float_t* x, float_t* y) const;

        // Inverse transformation of x and y. It works slow because
        // it explicitly inverts the matrix on every call. For massive 
        // operations it's better to invert() the matrix and then use 
        // direct transformations. 
        void inverse_transform(float_t* x, float_t* y) const;


        //---------------------------------------------------------- Auxiliary
        const trans_perspective& from_affine(const trans_affine& a);
        float_t determinant() const;
        float_t determinant_reciprocal() const;

        bool is_valid(float_t epsilon = affine_epsilon) const;
        bool is_identity(float_t epsilon = affine_epsilon) const;
        bool is_equal(const trans_perspective& m, 
                      float_t epsilon = affine_epsilon) const;

        // Determine the major affine parameters. Use with caution 
        // considering possible degenerate cases.
        float_t scale() const;
        float_t rotation() const;
        void   translation(float_t* dx, float_t* dy) const;
        void   scaling(float_t* x, float_t* y) const;
        void   scaling_abs(float_t* x, float_t* y) const;



        //--------------------------------------------------------------------
        class iterator_x
        {
            float_t den;
            float_t den_step;
            float_t nom_x;
            float_t nom_x_step;
            float_t nom_y;
            float_t nom_y_step;

        public:
            float_t x;
            float_t y;

            iterator_x() {}
            iterator_x(float_t px, float_t py, float_t step, const trans_perspective& m) :
                den(px * m.w0 + py * m.w1 + m.w2),
                den_step(m.w0 * step),
                nom_x(px * m.sx + py * m.shx + m.tx),
                nom_x_step(step * m.sx),
                nom_y(px * m.shy + py * m.sy + m.ty),
                nom_y_step(step * m.shy),
                x(nom_x / den),
                y(nom_y / den)
            {}

            void operator ++ ()
            {
                den   += den_step;
                nom_x += nom_x_step;
                nom_y += nom_y_step;
                float_t d = 1.0 / den;
                x = nom_x * d;
                y = nom_y * d;
            }
        };

        //--------------------------------------------------------------------
        iterator_x begin(float_t x, float_t y, float_t step) const
        {
            return iterator_x(x, y, step, *this);
        }
    };









   




    //------------------------------------------------------------------------
    inline bool trans_perspective::square_to_quad(const float_t* q)
    {
        float_t dx = q[0] - q[2] + q[4] - q[6];
        float_t dy = q[1] - q[3] + q[5] - q[7];
        if(dx == 0.0 && dy == 0.0)
        {   
            // Affine case (parallelogram)
            //---------------
            sx  = q[2] - q[0];
            shy = q[3] - q[1];
            w0  = 0.0;
            shx = q[4] - q[2];
            sy  = q[5] - q[3];
            w1  = 0.0;
            tx  = q[0];
            ty  = q[1];
            w2  = 1.0;
        }
        else
        {
            float_t dx1 = q[2] - q[4];
            float_t dy1 = q[3] - q[5];
            float_t dx2 = q[6] - q[4];
            float_t dy2 = q[7] - q[5];
            float_t den = dx1 * dy2 - dx2 * dy1;
            if(den == 0.0)
            {
                // Singular case
                //---------------
                sx = shy = w0 = shx = sy = w1 = tx = ty = w2 = 0.0;
                return false;
            }
            // General case
            //---------------
            float_t u = (dx * dy2 - dy * dx2) / den;
            float_t v = (dy * dx1 - dx * dy1) / den;
            sx  = q[2] - q[0] + u * q[2];
            shy = q[3] - q[1] + u * q[3];
            w0  = u;
            shx = q[6] - q[0] + v * q[6];
            sy  = q[7] - q[1] + v * q[7];
            w1  = v;
            tx  = q[0];
            ty  = q[1];
            w2  = 1.0;
        }
        return true;
    }

    //------------------------------------------------------------------------
    inline bool trans_perspective::invert()
    {
        float_t d0 = sy  * w2 - w1  * ty;
        float_t d1 = w0  * ty - shy * w2;
        float_t d2 = shy * w1 - w0  * sy;
        float_t d  = sx  * d0 + shx * d1 + tx * d2;
        if(d == 0.0) 
        {
            sx = shy = w0 = shx = sy = w1 = tx = ty = w2 = 0.0;
            return false;
        }
        d = 1.0 / d;
        trans_perspective a = *this;
        sx  = d * d0;
        shy = d * d1;
        w0  = d * d2;
        shx = d * (a.w1 *a.tx  - a.shx*a.w2);
        sy  = d * (a.sx *a.w2  - a.w0 *a.tx);
        w1  = d * (a.w0 *a.shx - a.sx *a.w1);
        tx  = d * (a.shx*a.ty  - a.sy *a.tx);
        ty  = d * (a.shy*a.tx  - a.sx *a.ty);
        w2  = d * (a.sx *a.sy  - a.shy*a.shx);
        return true;
    }

    //------------------------------------------------------------------------
    inline bool trans_perspective::quad_to_square(const float_t* q)
    {
        if(!square_to_quad(q)) return false;
        invert();
        return true;
    }

    //------------------------------------------------------------------------
    inline bool trans_perspective::quad_to_quad(const float_t* qs, 
                                                const float_t* qd)
    {
        trans_perspective p;
        if(!  quad_to_square(qs)) return false;
        if(!p.square_to_quad(qd)) return false;
        multiply(p);
        return true;
    }

    //------------------------------------------------------------------------
    inline bool trans_perspective::rect_to_quad(float_t x1, float_t y1, 
                                                float_t x2, float_t y2,
                                                const float_t* q)
    {
        float_t r[8];
        r[0] = r[6] = x1;
        r[2] = r[4] = x2;
        r[1] = r[3] = y1;
        r[5] = r[7] = y2;
        return quad_to_quad(r, q);
    }

    //------------------------------------------------------------------------
    inline bool trans_perspective::quad_to_rect(const float_t* q,
                                                float_t x1, float_t y1, 
                                                float_t x2, float_t y2)
    {
        float_t r[8];
        r[0] = r[6] = x1;
        r[2] = r[4] = x2;
        r[1] = r[3] = y1;
        r[5] = r[7] = y2;
        return quad_to_quad(q, r);
    }

    //------------------------------------------------------------------------
    inline trans_perspective::trans_perspective(float_t x1, float_t y1, 
                                                float_t x2, float_t y2, 
                                                const float_t* quad)
    {
        rect_to_quad(x1, y1, x2, y2, quad);
    }

    //------------------------------------------------------------------------
    inline trans_perspective::trans_perspective(const float_t* quad, 
                                                float_t x1, float_t y1, 
                                                float_t x2, float_t y2)
    {
        quad_to_rect(quad, x1, y1, x2, y2);
    }

    //------------------------------------------------------------------------
    inline trans_perspective::trans_perspective(const float_t* src, 
                                                const float_t* dst) 
    {
        quad_to_quad(src, dst);
    }

    //------------------------------------------------------------------------
    inline const trans_perspective& trans_perspective::reset()
    {
        sx  = 1; shy = 0; w0 = 0; 
        shx = 0; sy  = 1; w1 = 0;
        tx  = 0; ty  = 0; w2 = 1;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const trans_perspective& 
    trans_perspective::multiply(const trans_perspective& a)
    {
        trans_perspective b = *this;
        sx  = a.sx *b.sx  + a.shx*b.shy + a.tx*b.w0;
        shx = a.sx *b.shx + a.shx*b.sy  + a.tx*b.w1;
        tx  = a.sx *b.tx  + a.shx*b.ty  + a.tx*b.w2;
        shy = a.shy*b.sx  + a.sy *b.shy + a.ty*b.w0;
        sy  = a.shy*b.shx + a.sy *b.sy  + a.ty*b.w1;
        ty  = a.shy*b.tx  + a.sy *b.ty  + a.ty*b.w2;
        w0  = a.w0 *b.sx  + a.w1 *b.shy + a.w2*b.w0;
        w1  = a.w0 *b.shx + a.w1 *b.sy  + a.w2*b.w1;
        w2  = a.w0 *b.tx  + a.w1 *b.ty  + a.w2*b.w2;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const trans_perspective& 
    trans_perspective::multiply(const trans_affine& a)
    {
        trans_perspective b = *this;
        sx  = a.sx *b.sx  + a.shx*b.shy + a.tx*b.w0;
        shx = a.sx *b.shx + a.shx*b.sy  + a.tx*b.w1;
        tx  = a.sx *b.tx  + a.shx*b.ty  + a.tx*b.w2;
        shy = a.shy*b.sx  + a.sy *b.shy + a.ty*b.w0;
        sy  = a.shy*b.shx + a.sy *b.sy  + a.ty*b.w1;
        ty  = a.shy*b.tx  + a.sy *b.ty  + a.ty*b.w2;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const trans_perspective& 
    trans_perspective::premultiply(const trans_perspective& b)
    {
        trans_perspective a = *this;
        sx  = a.sx *b.sx  + a.shx*b.shy + a.tx*b.w0;
        shx = a.sx *b.shx + a.shx*b.sy  + a.tx*b.w1;
        tx  = a.sx *b.tx  + a.shx*b.ty  + a.tx*b.w2;
        shy = a.shy*b.sx  + a.sy *b.shy + a.ty*b.w0;
        sy  = a.shy*b.shx + a.sy *b.sy  + a.ty*b.w1;
        ty  = a.shy*b.tx  + a.sy *b.ty  + a.ty*b.w2;
        w0  = a.w0 *b.sx  + a.w1 *b.shy + a.w2*b.w0;
        w1  = a.w0 *b.shx + a.w1 *b.sy  + a.w2*b.w1;
        w2  = a.w0 *b.tx  + a.w1 *b.ty  + a.w2*b.w2;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const trans_perspective& 
    trans_perspective::premultiply(const trans_affine& b)
    {
        trans_perspective a = *this;
        sx  = a.sx *b.sx  + a.shx*b.shy;
        shx = a.sx *b.shx + a.shx*b.sy;
        tx  = a.sx *b.tx  + a.shx*b.ty  + a.tx;
        shy = a.shy*b.sx  + a.sy *b.shy;
        sy  = a.shy*b.shx + a.sy *b.sy;
        ty  = a.shy*b.tx  + a.sy *b.ty  + a.ty;
        w0  = a.w0 *b.sx  + a.w1 *b.shy;
        w1  = a.w0 *b.shx + a.w1 *b.sy;
        w2  = a.w0 *b.tx  + a.w1 *b.ty  + a.w2;
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_perspective& 
    trans_perspective::multiply_inv(const trans_perspective& m)
    {
        trans_perspective t = m;
        t.invert();
        return multiply(t);
    }

    //------------------------------------------------------------------------
    const trans_perspective&
    trans_perspective::multiply_inv(const trans_affine& m)
    {
        trans_affine t = m;
        t.invert();
        return multiply(t);
    }

    //------------------------------------------------------------------------
    const trans_perspective&
    trans_perspective::premultiply_inv(const trans_perspective& m)
    {
        trans_perspective t = m;
        t.invert();
        return *this = t.multiply(*this);
    }

    //------------------------------------------------------------------------
    const trans_perspective&
    trans_perspective::premultiply_inv(const trans_affine& m)
    {
        trans_perspective t(m);
        t.invert();
        return *this = t.multiply(*this);
    }

    //------------------------------------------------------------------------
    inline const trans_perspective& 
    trans_perspective::translate(float_t x, float_t y)
    {
        tx += x;
        ty += y;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const trans_perspective& trans_perspective::rotate(float_t a)
    {
        multiply(trans_affine_rotation(a));
        return *this;
    }

    //------------------------------------------------------------------------
    inline const trans_perspective& trans_perspective::scale(float_t s)
    {
        multiply(trans_affine_scaling(s));
        return *this;
    }

    //------------------------------------------------------------------------
    inline const trans_perspective& trans_perspective::scale(float_t x, float_t y)
    {
        multiply(trans_affine_scaling(x, y));
        return *this;
    }

    //------------------------------------------------------------------------
    inline void trans_perspective::transform(float_t* px, float_t* py) const
    {
        float_t x = *px;
        float_t y = *py;
        float_t m = 1.0 / (x*w0 + y*w1 + w2);
        *px = m * (x*sx  + y*shx + tx);
        *py = m * (x*shy + y*sy  + ty);
    }

    //------------------------------------------------------------------------
    inline void trans_perspective::transform_affine(float_t* x, float_t* y) const
    {
        float_t tmp = *x;
        *x = tmp * sx  + *y * shx + tx;
        *y = tmp * shy + *y * sy  + ty;
    }

    //------------------------------------------------------------------------
    inline void trans_perspective::transform_2x2(float_t* x, float_t* y) const
    {
        float_t tmp = *x;
        *x = tmp * sx  + *y * shx;
        *y = tmp * shy + *y * sy;
    }

    //------------------------------------------------------------------------
    inline void trans_perspective::inverse_transform(float_t* x, float_t* y) const
    {
        trans_perspective t(*this);
        if(t.invert()) t.transform(x, y);
    }

    //------------------------------------------------------------------------
    inline void trans_perspective::store_to(float_t* m) const
    {
        *m++ = sx;  *m++ = shy; *m++ = w0; 
        *m++ = shx; *m++ = sy;  *m++ = w1;
        *m++ = tx;  *m++ = ty;  *m++ = w2;
    }

    //------------------------------------------------------------------------
    inline const trans_perspective& trans_perspective::load_from(const float_t* m)
    {
        sx  = *m++; shy = *m++; w0 = *m++; 
        shx = *m++; sy  = *m++; w1 = *m++;
        tx  = *m++; ty  = *m++; w2 = *m++;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const trans_perspective& 
    trans_perspective::from_affine(const trans_affine& a)
    {
        sx  = a.sx;  shy = a.shy; w0 = 0; 
        shx = a.shx; sy  = a.sy;  w1 = 0;
        tx  = a.tx;  ty  = a.ty;  w2 = 1;
        return *this;
    }

    //------------------------------------------------------------------------
    inline float_t trans_perspective::determinant() const
    {
        return sx  * (sy  * w2 - ty  * w1) +
               shx * (ty  * w0 - shy * w2) +
               tx  * (shy * w1 - sy  * w0);
    }
  
    //------------------------------------------------------------------------
    inline float_t trans_perspective::determinant_reciprocal() const
    {
        return 1.0 / determinant();
    }

    //------------------------------------------------------------------------
    inline bool trans_perspective::is_valid(float_t epsilon) const
    {
        return fabs(sx) > epsilon && fabs(sy) > epsilon && fabs(w2) > epsilon;
    }

    //------------------------------------------------------------------------
    inline bool trans_perspective::is_identity(float_t epsilon) const
    {
        return is_equal_eps(sx,  1.0, epsilon) &&
               is_equal_eps(shy, 0.0, epsilon) &&
               is_equal_eps(w0,  0.0, epsilon) &&
               is_equal_eps(shx, 0.0, epsilon) && 
               is_equal_eps(sy,  1.0, epsilon) &&
               is_equal_eps(w1,  0.0, epsilon) &&
               is_equal_eps(tx,  0.0, epsilon) &&
               is_equal_eps(ty,  0.0, epsilon) &&
               is_equal_eps(w2,  1.0, epsilon);
    }

    //------------------------------------------------------------------------
    inline bool trans_perspective::is_equal(const trans_perspective& m, 
                                            float_t epsilon) const
    {
        return is_equal_eps(sx,  m.sx,  epsilon) &&
               is_equal_eps(shy, m.shy, epsilon) &&
               is_equal_eps(w0,  m.w0,  epsilon) &&
               is_equal_eps(shx, m.shx, epsilon) && 
               is_equal_eps(sy,  m.sy,  epsilon) &&
               is_equal_eps(w1,  m.w1,  epsilon) &&
               is_equal_eps(tx,  m.tx,  epsilon) &&
               is_equal_eps(ty,  m.ty,  epsilon) &&
               is_equal_eps(w2,  m.w2,  epsilon);
    }

    //------------------------------------------------------------------------
    inline float_t trans_perspective::scale() const
    {
        float_t x = 0.707106781 * sx  + 0.707106781 * shx;
        float_t y = 0.707106781 * shy + 0.707106781 * sy;
        return sqrt(x*x + y*y);
    }

    //------------------------------------------------------------------------
    inline float_t trans_perspective::rotation() const
    {
        float_t x1 = 0.0;
        float_t y1 = 0.0;
        float_t x2 = 1.0;
        float_t y2 = 0.0;
        transform(&x1, &y1);
        transform(&x2, &y2);
        return atan2(y2-y1, x2-x1);
    }

    //------------------------------------------------------------------------
    void trans_perspective::translation(float_t* dx, float_t* dy) const
    {
        *dx = tx;
        *dy = ty;
    }

    //------------------------------------------------------------------------
    void trans_perspective::scaling(float_t* x, float_t* y) const
    {
        float_t x1 = 0.0;
        float_t y1 = 0.0;
        float_t x2 = 1.0;
        float_t y2 = 1.0;
        trans_perspective t(*this);
        t *= trans_affine_rotation(-rotation());
        t.transform(&x1, &y1);
        t.transform(&x2, &y2);
        *x = x2 - x1;
        *y = y2 - y1;
    }

    //------------------------------------------------------------------------
    void trans_perspective::scaling_abs(float_t* x, float_t* y) const
    {
        *x = sqrt(sx  * sx  + shx * shx);
        *y = sqrt(shy * shy + sy  * sy);
    }


}

#endif

