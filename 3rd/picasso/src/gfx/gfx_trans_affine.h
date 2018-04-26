/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_TRANS_AFFINE_H_
#define _GFX_TRANS_AFFINE_H_

#include "common.h"
#include "interfaces.h"

#include "graphic_base.h"

namespace gfx {

const scalar affine_epsilon = FLT_TO_SCALAR(1e-14f);

class gfx_trans_affine : public abstract_trans_affine
{
public:
    gfx_trans_affine()
        : m_sx(FLT_TO_SCALAR(1.0f)), m_shy(FLT_TO_SCALAR(0.0f))
        , m_shx(FLT_TO_SCALAR(0.0f)), m_sy(FLT_TO_SCALAR(1.0f))
        , m_tx(FLT_TO_SCALAR(0.0f)), m_ty(FLT_TO_SCALAR(0.0f))
    {
    }

    gfx_trans_affine(scalar sx, scalar shy, scalar shx, scalar sy, scalar tx, scalar ty)
        : m_sx(sx), m_shy(shy), m_shx(shx), m_sy(sy), m_tx(tx), m_ty(ty)
    {
    }

    virtual ~gfx_trans_affine()
    {
    }

    virtual void sx(scalar v) { m_sx = v; }
    virtual void sy(scalar v) { m_sy = v; }
    virtual scalar sx(void) const { return m_sx; }
    virtual scalar sy(void) const { return m_sy; }
    virtual void shx(scalar v) { m_shx = v; }
    virtual void shy(scalar v) { m_shy = v; }
    virtual scalar shx(void) const { return m_shx; }
    virtual scalar shy(void) const { return m_shy; }
    virtual void tx(scalar v) { m_tx = v; }
    virtual void ty(scalar v) { m_ty = v; }
    virtual scalar tx(void) const { return m_tx; }
    virtual scalar ty(void) const { return m_ty; }

    virtual void translate(scalar x, scalar y)
    {
        m_tx += x;
        m_ty += y;
    }

    virtual void scale(scalar x, scalar y)
    {
        scalar m0 = x;
        scalar m1 = y;
        m_sx  *= m0;
        m_shx *= m0;
        m_tx  *= m0;
        m_shy *= m1;
        m_sy  *= m1;
        m_ty  *= m1;
    }

    virtual void rotate(scalar a)
    {
        scalar ca = Cos(a);
        scalar sa = Sin(a);
        scalar t0 = m_sx  * ca - m_shy * sa;
        scalar t2 = m_shx * ca - m_sy * sa;
        scalar t4 = m_tx  * ca - m_ty * sa;
        m_shy = m_sx  * sa + m_shy * ca;
        m_sy  = m_shx * sa + m_sy * ca;
        m_ty  = m_tx  * sa + m_ty * ca;
        m_sx  = t0;
        m_shx = t2;
        m_tx  = t4;
    }

    virtual void shear(scalar x, scalar y)
    {
        scalar t0 = y * m_shx;
        scalar t1 = y * m_sy;
        scalar t2 = x * m_sx;
        scalar t3 = x * m_shy;
        m_sx += t0;
        m_shy += t1;
        m_shx += t2;
        m_sy += t3;
    }


    virtual void invert(void)
    {
        scalar d = determinant_reciprocal();

        scalar t0 = m_sy * d;
        m_sy  =  m_sx  * d;
        m_shy = -m_shy * d;
        m_shx = -m_shx * d;

        scalar t4 = -m_tx * t0 - m_ty * m_shx;
        m_ty = -m_tx * m_shy - m_ty * m_sy;

        m_sx = t0;
        m_tx = t4;
    }

    virtual void flip_x(void)
    {
        m_sx  = -m_sx;
        m_shy = -m_shy;
        m_tx  = -m_tx;
    }

    virtual void flip_y(void)
    {
        m_shx = -m_shx;
        m_sy  = -m_sy;
        m_ty  = -m_ty;
    }

    virtual void reset(void)
    {
        m_sx = m_sy = FLT_TO_SCALAR(1.0f);
        m_shy = m_shx = m_tx = m_ty = FLT_TO_SCALAR(0.0f);
    }

    virtual void multiply(const abstract_trans_affine* o)
    {
        scalar t0 = m_sx  * o->sx() + m_shy * o->shx();
        scalar t2 = m_shx * o->sx() + m_sy  * o->shx();
        scalar t4 = m_tx  * o->sx() + m_ty  * o->shx() + o->tx();
        m_shy = m_sx  * o->shy() + m_shy * o->sy();
        m_sy  = m_shx * o->shy() + m_sy  * o->sy();
        m_ty  = m_tx  * o->shy() + m_ty  * o->sy() + o->ty();
        m_sx  = t0;
        m_shx = t2;
        m_tx  = t4;
    }

    virtual bool is_identity(void) const
    {
        return is_equal_eps(m_sx,  FLT_TO_SCALAR(1.0f), affine_epsilon) &&
               is_equal_eps(m_shy, FLT_TO_SCALAR(0.0f), affine_epsilon) &&
               is_equal_eps(m_shx, FLT_TO_SCALAR(0.0f), affine_epsilon) &&
               is_equal_eps(m_sy,  FLT_TO_SCALAR(1.0f), affine_epsilon) &&
               is_equal_eps(m_tx,  FLT_TO_SCALAR(0.0f), affine_epsilon) &&
               is_equal_eps(m_ty,  FLT_TO_SCALAR(0.0f), affine_epsilon);
    }

    virtual scalar determinant(void) const
    {
        return m_sx * m_sy - m_shy * m_shx;
    }

    virtual scalar rotation(void) const
    {
        scalar x1 = FLT_TO_SCALAR(0.0f);
        scalar y1 = FLT_TO_SCALAR(0.0f);
        scalar x2 = FLT_TO_SCALAR(1.0f);
        scalar y2 = FLT_TO_SCALAR(0.0f);
        transform(&x1, &y1);
        transform(&x2, &y2);
        return Atan2(y2-y1, x2-x1);
    }

    virtual void translation(scalar* dx, scalar* dy) const
    {
        *dx = m_tx;
        *dy = m_ty;
    }

    virtual void scaling(scalar* x, scalar* y) const
    {
        scalar x1 = FLT_TO_SCALAR(0.0f);
        scalar y1 = FLT_TO_SCALAR(0.0f);
        scalar x2 = FLT_TO_SCALAR(1.0f);
        scalar y2 = FLT_TO_SCALAR(1.0f);
        gfx_trans_affine t(*this);
        t.rotate(-rotation());
        t.transform(&x1, &y1);
        t.transform(&x2, &y2);
        *x = x2 - x1;
        *y = y2 - y1;
    }

    virtual void shearing(scalar* x, scalar* y) const
    {
        //FIXME: need be implements.
    }

    virtual bool is_equal(const abstract_trans_affine* o)
    {
        return is_equal_eps(m_sx,  o->sx(),  affine_epsilon) &&
               is_equal_eps(m_shy, o->shy(), affine_epsilon) &&
               is_equal_eps(m_shx, o->shx(), affine_epsilon) &&
               is_equal_eps(m_sy,  o->sy(),  affine_epsilon) &&
               is_equal_eps(m_tx,  o->tx(),  affine_epsilon) &&
               is_equal_eps(m_ty,  o->ty(),  affine_epsilon);
    }

    virtual void transform(scalar* x, scalar* y) const
    {
        register scalar tmp = *x;
        *x = tmp * m_sx  + *y * m_shx + m_tx;
        *y = tmp * m_shy + *y * m_sy  + m_ty;
    }

    virtual void transform_2x2(scalar* x, scalar* y) const
    {
        register scalar tmp = *x;
        *x = tmp * m_sx  + *y * m_shx;
        *y = tmp * m_shy + *y * m_sy;
    }

    virtual void inverse_transform(scalar* x, scalar* y) const
    {
        register scalar d = determinant_reciprocal();
        register scalar a = (*x - m_tx) * d;
        register scalar b = (*y - m_ty) * d;
        *x = a * m_sy - b * m_shx;
        *y = b * m_sx - a * m_shy;
    }

    // Store matrix to an array [6] of scalar
    virtual void store_to(scalar* m) const
    {
        *m++ = m_sx; *m++ = m_shy; *m++ = m_shx; *m++ = m_sy; *m++ = m_tx; *m++ = m_ty;
    }

    // Load matrix from an array [6] of float
    virtual void load_from(const scalar* m)
    {
        m_sx = *m++; m_shy = *m++; m_shx = *m++; m_sy = *m++; m_tx = *m++; m_ty = *m++;
    }

public:
    // Multiply the matrix by another one
    const gfx_trans_affine& operator *= (const gfx_trans_affine& o)
    {
        multiply(&o);
        return *this;
    }

protected:
    // Calculate the reciprocal of the determinant
    scalar determinant_reciprocal(void) const
    {
        return FLT_TO_SCALAR(1.0f) / (m_sx * m_sy - m_shy * m_shx);
    }

private:
    scalar m_sx;
    scalar m_shy;
    scalar m_shx;
    scalar m_sy;
    scalar m_tx;
    scalar m_ty;
};

// Translation matrix
class gfx_trans_affine_translation : public gfx_trans_affine
{
public:
    gfx_trans_affine_translation(scalar x, scalar y)
        : gfx_trans_affine(FLT_TO_SCALAR(1.0f), FLT_TO_SCALAR(0.0f), FLT_TO_SCALAR(0.0f), FLT_TO_SCALAR(1.0f), x, y)
    {
    }
};

// Scaling matrix. x, y - scale coefficients by X and Y respectively
class gfx_trans_affine_scaling : public gfx_trans_affine
{
public:
    gfx_trans_affine_scaling(scalar x, scalar y)
        : gfx_trans_affine(x, FLT_TO_SCALAR(0.0f), FLT_TO_SCALAR(0.0f), y, FLT_TO_SCALAR(0.0f), FLT_TO_SCALAR(0.0f))
    {
    }

    gfx_trans_affine_scaling(scalar s)
        : gfx_trans_affine(s, FLT_TO_SCALAR(0.0f), FLT_TO_SCALAR(0.0f), s, FLT_TO_SCALAR(0.0f), FLT_TO_SCALAR(0.0f))
    {
    }
};

// Rotation matrix. sin() and cos() are calculated twice for the same angle.
class gfx_trans_affine_rotation : public gfx_trans_affine
{
public:
    gfx_trans_affine_rotation(scalar a)
        : gfx_trans_affine(Cos(a), Sin(a), -(Sin(a)), Cos(a), FLT_TO_SCALAR(0.0f), FLT_TO_SCALAR(0.0f))
    {
    }
};

// Sckewing (shear) matrix
class gfx_trans_affine_skewing : public gfx_trans_affine
{
public:
    gfx_trans_affine_skewing(scalar x, scalar y)
        : gfx_trans_affine(FLT_TO_SCALAR(1.0f), Tan(y), Tan(x),
                           FLT_TO_SCALAR(1.0f), FLT_TO_SCALAR(0.0f), FLT_TO_SCALAR(0.0f))
    {
    }
};

// hack!! Stable matrix
inline gfx_trans_affine stable_matrix(const gfx_trans_affine& o)
{
    if (is_boxer(o.rotation())) {
        scalar tx = Floor(o.tx());
        scalar ty = Floor(o.ty());
        return gfx_trans_affine(o.sx(), o.shy(), o.shx(), o.sy(), SCALAR_TO_FLT(tx), SCALAR_TO_FLT(ty));
    } else  {
        return gfx_trans_affine(o);
    }
}

}
#endif /*_GFX_TRANS_AFFINE_H_*/
