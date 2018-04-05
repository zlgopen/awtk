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

#ifndef AGG_TRANS_AFFINE_INCLUDED
#define AGG_TRANS_AFFINE_INCLUDED

#include <math.h>
#include "agg_basics.h"

namespace agg
{
    const float_t affine_epsilon = 1e-14; 

    //============================================================trans_affine
    //
    // See Implementation agg_trans_affine.cpp
    //
    // Affine transformation are linear transformations in Cartesian coordinates
    // (strictly speaking not only in Cartesian, but for the beginning we will 
    // think so). They are rotation, scaling, translation and skewing.  
    // After any affine transformation a line segment remains a line segment 
    // and it will never become a curve. 
    //
    // There will be no math about matrix calculations, since it has been 
    // described many times. Ask yourself a very simple question:
    // "why do we need to understand and use some matrix stuff instead of just 
    // rotating, scaling and so on". The answers are:
    //
    // 1. Any combination of transformations can be done by only 4 multiplications
    //    and 4 additions in floating point.
    // 2. One matrix transformation is equivalent to the number of consecutive
    //    discrete transformations, i.e. the matrix "accumulates" all transformations 
    //    in the order of their settings. Suppose we have 4 transformations: 
    //       * rotate by 30 degrees,
    //       * scale X to 2.0, 
    //       * scale Y to 1.5, 
    //       * move to (100, 100). 
    //    The result will depend on the order of these transformations, 
    //    and the advantage of matrix is that the sequence of discret calls:
    //    rotate(30), scaleX(2.0), scaleY(1.5), move(100,100) 
    //    will have exactly the same result as the following matrix transformations:
    //   
    //    affine_matrix m;
    //    m *= rotate_matrix(30); 
    //    m *= scaleX_matrix(2.0);
    //    m *= scaleY_matrix(1.5);
    //    m *= move_matrix(100,100);
    //
    //    m.transform_my_point_at_last(x, y);
    //
    // What is the good of it? In real life we will set-up the matrix only once
    // and then transform many points, let alone the convenience to set any 
    // combination of transformations.
    //
    // So, how to use it? Very easy - literally as it's shown above. Not quite,
    // let us write a correct example:
    //
    // agg::trans_affine m;
    // m *= agg::trans_affine_rotation(30.0 * 3.1415926 / 180.0);
    // m *= agg::trans_affine_scaling(2.0, 1.5);
    // m *= agg::trans_affine_translation(100.0, 100.0);
    // m.transform(&x, &y);
    //
    // The affine matrix is all you need to perform any linear transformation,
    // but all transformations have origin point (0,0). It means that we need to 
    // use 2 translations if we want to rotate someting around (100,100):
    // 
    // m *= agg::trans_affine_translation(-100.0, -100.0);         // move to (0,0)
    // m *= agg::trans_affine_rotation(30.0 * 3.1415926 / 180.0);  // rotate
    // m *= agg::trans_affine_translation(100.0, 100.0);           // move back to (100,100)
    //----------------------------------------------------------------------
    struct trans_affine
    {
        float_t sx, shy, shx, sy, tx, ty;

        //------------------------------------------ Construction
        // Identity matrix
        trans_affine() :
            sx(1.0), shy(0.0), shx(0.0), sy(1.0), tx(0.0), ty(0.0)
        {}

        // Custom matrix. Usually used in derived classes
        trans_affine(float_t v0, float_t v1, float_t v2, 
                     float_t v3, float_t v4, float_t v5) :
            sx(v0), shy(v1), shx(v2), sy(v3), tx(v4), ty(v5)
        {}

        // Custom matrix from m[6]
        explicit trans_affine(const float_t* m) :
            sx(m[0]), shy(m[1]), shx(m[2]), sy(m[3]), tx(m[4]), ty(m[5])
        {}

        // Rectangle to a parallelogram.
        trans_affine(float_t x1, float_t y1, float_t x2, float_t y2, 
                     const float_t* parl)
        {
            rect_to_parl(x1, y1, x2, y2, parl);
        }

        // Parallelogram to a rectangle.
        trans_affine(const float_t* parl, 
                     float_t x1, float_t y1, float_t x2, float_t y2)
        {
            parl_to_rect(parl, x1, y1, x2, y2);
        }

        // Arbitrary parallelogram transformation.
        trans_affine(const float_t* src, const float_t* dst)
        {
            parl_to_parl(src, dst);
        }

        //---------------------------------- Parellelogram transformations
        // transform a parallelogram to another one. Src and dst are 
        // pointers to arrays of three points (float_t[6], x1,y1,...) that 
        // identify three corners of the parallelograms assuming implicit 
        // fourth point. The arguments are arrays of float_t[6] mapped 
        // to x1,y1, x2,y2, x3,y3  where the coordinates are:
        //        *-----------------*
        //       /          (x3,y3)/
        //      /                 /
        //     /(x1,y1)   (x2,y2)/
        //    *-----------------*
        const trans_affine& parl_to_parl(const float_t* src, 
                                         const float_t* dst);

        const trans_affine& rect_to_parl(float_t x1, float_t y1, 
                                         float_t x2, float_t y2, 
                                         const float_t* parl);

        const trans_affine& parl_to_rect(const float_t* parl, 
                                         float_t x1, float_t y1, 
                                         float_t x2, float_t y2);


        //------------------------------------------ Operations
        // Reset - load an identity matrix
        const trans_affine& reset();

        // Direct transformations operations
        const trans_affine& translate(float_t x, float_t y);
        const trans_affine& rotate(float_t a);
        const trans_affine& scale(float_t s);
        const trans_affine& scale(float_t x, float_t y);

        // Multiply matrix to another one
        const trans_affine& multiply(const trans_affine& m);

        // Multiply "m" to "this" and assign the result to "this"
        const trans_affine& premultiply(const trans_affine& m);

        // Multiply matrix to inverse of another one
        const trans_affine& multiply_inv(const trans_affine& m);

        // Multiply inverse of "m" to "this" and assign the result to "this"
        const trans_affine& premultiply_inv(const trans_affine& m);

        // Invert matrix. Do not try to invert degenerate matrices, 
        // there's no check for validity. If you set scale to 0 and 
        // then try to invert matrix, expect unpredictable result.
        const trans_affine& invert();

        // Mirroring around X
        const trans_affine& flip_x();

        // Mirroring around Y
        const trans_affine& flip_y();

        //------------------------------------------- Load/Store
        // Store matrix to an array [6] of float_t
        void store_to(float_t* m) const
        {
            *m++ = sx; *m++ = shy; *m++ = shx; *m++ = sy; *m++ = tx; *m++ = ty;
        }

        // Load matrix from an array [6] of float_t
        const trans_affine& load_from(const float_t* m)
        {
            sx = *m++; shy = *m++; shx = *m++; sy = *m++; tx = *m++;  ty = *m++;
            return *this;
        }

        //------------------------------------------- Operators
        
        // Multiply the matrix by another one
        const trans_affine& operator *= (const trans_affine& m)
        {
            return multiply(m);
        }

        // Multiply the matrix by inverse of another one
        const trans_affine& operator /= (const trans_affine& m)
        {
            return multiply_inv(m);
        }

        // Multiply the matrix by another one and return
        // the result in a separete matrix.
        trans_affine operator * (const trans_affine& m)
        {
            return trans_affine(*this).multiply(m);
        }

        // Multiply the matrix by inverse of another one 
        // and return the result in a separete matrix.
        trans_affine operator / (const trans_affine& m)
        {
            return trans_affine(*this).multiply_inv(m);
        }

        // Calculate and return the inverse matrix
        trans_affine operator ~ () const
        {
            trans_affine ret = *this;
            return ret.invert();
        }

        // Equal operator with default epsilon
        bool operator == (const trans_affine& m) const
        {
            return is_equal(m, affine_epsilon);
        }

        // Not Equal operator with default epsilon
        bool operator != (const trans_affine& m) const
        {
            return !is_equal(m, affine_epsilon);
        }

        //-------------------------------------------- Transformations
        // Direct transformation of x and y
        void transform(float_t* x, float_t* y) const;

        // Direct transformation of x and y, 2x2 matrix only, no translation
        void transform_2x2(float_t* x, float_t* y) const;

        // Inverse transformation of x and y. It works slower than the 
        // direct transformation. For massive operations it's better to 
        // invert() the matrix and then use direct transformations. 
        void inverse_transform(float_t* x, float_t* y) const;

        //-------------------------------------------- Auxiliary
        // Calculate the determinant of matrix
        float_t determinant() const
        {
            return sx * sy - shy * shx;
        }

        // Calculate the reciprocal of the determinant
        float_t determinant_reciprocal() const
        {
            return 1.0 / (sx * sy - shy * shx);
        }

        // Get the average scale (by X and Y). 
        // Basically used to calculate the approximation_scale when
        // decomposinting curves into line segments.
        float_t scale() const;

        // Check to see if the matrix is not degenerate
        bool is_valid(float_t epsilon = affine_epsilon) const;

        // Check to see if it's an identity matrix
        bool is_identity(float_t epsilon = affine_epsilon) const;

        // Check to see if two matrices are equal
        bool is_equal(const trans_affine& m, float_t epsilon = affine_epsilon) const;

        // Determine the major parameters. Use with caution considering 
        // possible degenerate cases.
        float_t rotation() const;
        void   translation(float_t* dx, float_t* dy) const;
        void   scaling(float_t* x, float_t* y) const;
        void   scaling_abs(float_t* x, float_t* y) const;
    };

    //------------------------------------------------------------------------
    inline void trans_affine::transform(float_t* x, float_t* y) const
    {
        register float_t tmp = *x;
        *x = tmp * sx  + *y * shx + tx;
        *y = tmp * shy + *y * sy  + ty;
    }

    //------------------------------------------------------------------------
    inline void trans_affine::transform_2x2(float_t* x, float_t* y) const
    {
        register float_t tmp = *x;
        *x = tmp * sx  + *y * shx;
        *y = tmp * shy + *y * sy;
    }

    //------------------------------------------------------------------------
    inline void trans_affine::inverse_transform(float_t* x, float_t* y) const
    {
        register float_t d = determinant_reciprocal();
        register float_t a = (*x - tx) * d;
        register float_t b = (*y - ty) * d;
        *x = a * sy - b * shx;
        *y = b * sx - a * shy;
    }

    //------------------------------------------------------------------------
    inline float_t trans_affine::scale() const
    {
        float_t x = 0.707106781 * sx  + 0.707106781 * shx;
        float_t y = 0.707106781 * shy + 0.707106781 * sy;
        return sqrt(x*x + y*y);
    }

    //------------------------------------------------------------------------
    inline const trans_affine& trans_affine::translate(float_t x, float_t y) 
    { 
        tx += x;
        ty += y; 
        return *this;
    }

    //------------------------------------------------------------------------
    inline const trans_affine& trans_affine::rotate(float_t a) 
    {
        float_t ca = cos(a); 
        float_t sa = sin(a);
        float_t t0 = sx  * ca - shy * sa;
        float_t t2 = shx * ca - sy * sa;
        float_t t4 = tx  * ca - ty * sa;
        shy = sx  * sa + shy * ca;
        sy  = shx * sa + sy * ca; 
        ty  = tx  * sa + ty * ca;
        sx  = t0;
        shx = t2;
        tx  = t4;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const trans_affine& trans_affine::scale(float_t x, float_t y) 
    {
        float_t mm0 = x; // Possible hint for the optimizer
        float_t mm3 = y; 
        sx  *= mm0;
        shx *= mm0;
        tx  *= mm0;
        shy *= mm3;
        sy  *= mm3;
        ty  *= mm3;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const trans_affine& trans_affine::scale(float_t s) 
    {
        float_t m = s; // Possible hint for the optimizer
        sx  *= m;
        shx *= m;
        tx  *= m;
        shy *= m;
        sy  *= m;
        ty  *= m;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const trans_affine& trans_affine::premultiply(const trans_affine& m)
    {
        trans_affine t = m;
        return *this = t.multiply(*this);
    }

    //------------------------------------------------------------------------
    inline const trans_affine& trans_affine::multiply_inv(const trans_affine& m)
    {
        trans_affine t = m;
        t.invert();
        return multiply(t);
    }

    //------------------------------------------------------------------------
    inline const trans_affine& trans_affine::premultiply_inv(const trans_affine& m)
    {
        trans_affine t = m;
        t.invert();
        return *this = t.multiply(*this);
    }

    //------------------------------------------------------------------------
    inline void trans_affine::scaling_abs(float_t* x, float_t* y) const
    {
        // Used to calculate scaling coefficients in image resampling. 
        // When there is considerable shear this method gives us much
        // better estimation than just sx, sy.
        *x = sqrt(sx  * sx  + shx * shx);
        *y = sqrt(shy * shy + sy  * sy);
    }

    //====================================================trans_affine_rotation
    // Rotation matrix. sin() and cos() are calculated twice for the same angle.
    // There's no harm because the performance of sin()/cos() is very good on all
    // modern processors. Besides, this operation is not going to be invoked too 
    // often.
    class trans_affine_rotation : public trans_affine
    {
    public:
        trans_affine_rotation(float_t a) : 
          trans_affine(cos(a), sin(a), -sin(a), cos(a), 0.0, 0.0)
        {}
    };

    //====================================================trans_affine_scaling
    // Scaling matrix. x, y - scale coefficients by X and Y respectively
    class trans_affine_scaling : public trans_affine
    {
    public:
        trans_affine_scaling(float_t x, float_t y) : 
          trans_affine(x, 0.0, 0.0, y, 0.0, 0.0)
        {}

        trans_affine_scaling(float_t s) : 
          trans_affine(s, 0.0, 0.0, s, 0.0, 0.0)
        {}
    };

    //================================================trans_affine_translation
    // Translation matrix
    class trans_affine_translation : public trans_affine
    {
    public:
        trans_affine_translation(float_t x, float_t y) : 
          trans_affine(1.0, 0.0, 0.0, 1.0, x, y)
        {}
    };

    //====================================================trans_affine_skewing
    // Sckewing (shear) matrix
    class trans_affine_skewing : public trans_affine
    {
    public:
        trans_affine_skewing(float_t x, float_t y) : 
          trans_affine(1.0, tan(y), tan(x), 1.0, 0.0, 0.0)
        {}
    };


    //===============================================trans_affine_line_segment
    // Rotate, Scale and Translate, associating 0...dist with line segment 
    // x1,y1,x2,y2
    class trans_affine_line_segment : public trans_affine
    {
    public:
        trans_affine_line_segment(float_t x1, float_t y1, float_t x2, float_t y2, 
                                  float_t dist)
        {
            float_t dx = x2 - x1;
            float_t dy = y2 - y1;
            if(dist > 0.0)
            {
                multiply(trans_affine_scaling(sqrt(dx * dx + dy * dy) / dist));
            }
            multiply(trans_affine_rotation(atan2(dy, dx)));
            multiply(trans_affine_translation(x1, y1));
        }
    };


    //============================================trans_affine_reflection_unit
    // Reflection matrix. Reflect coordinates across the line through 
    // the origin containing the unit vector (ux, uy).
    // Contributed by John Horigan
    class trans_affine_reflection_unit : public trans_affine
    {
    public:
        trans_affine_reflection_unit(float_t ux, float_t uy) :
          trans_affine(2.0 * ux * ux - 1.0, 
                       2.0 * ux * uy, 
                       2.0 * ux * uy, 
                       2.0 * uy * uy - 1.0, 
                       0.0, 0.0)
        {}
    };


    //=================================================trans_affine_reflection
    // Reflection matrix. Reflect coordinates across the line through 
    // the origin at the angle a or containing the non-unit vector (x, y).
    // Contributed by John Horigan
    class trans_affine_reflection : public trans_affine_reflection_unit
    {
    public:
        trans_affine_reflection(float_t a) :
          trans_affine_reflection_unit(cos(a), sin(a))
        {}


        trans_affine_reflection(float_t x, float_t y) :
          trans_affine_reflection_unit(x / sqrt(x * x + y * y), y / sqrt(x * x + y * y))
        {}
    };

}


#endif

