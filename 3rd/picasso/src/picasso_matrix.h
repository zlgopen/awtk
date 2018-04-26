/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _PICASSO_MATRIX_H_
#define _PICASSO_MATRIX_H_

#include "common.h"
#include "device.h"
#include "interfaces.h"

namespace picasso {

class trans_affine
{
public:
    trans_affine();
    trans_affine(scalar sx, scalar shy, scalar shx, scalar sy, scalar tx, scalar ty);

    trans_affine(const trans_affine& o);
    trans_affine& operator=(const trans_affine& o);

    ~trans_affine();
public:
    void sx(scalar v);
    void sy(scalar v);
    scalar sx(void) const;
    scalar sy(void) const;
    void shx(scalar v);
    void shy(scalar v);
    scalar shx(void) const;
    scalar shy(void) const;
    void tx(scalar v);
    void ty(scalar v);
    scalar tx(void) const;
    scalar ty(void) const;
public:
    const trans_affine& translate(scalar x, scalar y);
    const trans_affine& scale(scalar x, scalar y);
    const trans_affine& rotate(scalar a);
    const trans_affine& shear(scalar x, scalar y);

    const trans_affine& invert(void);
    const trans_affine& flip_x(void);
    const trans_affine& flip_y(void);
    const trans_affine& reset(void);
    const trans_affine& multiply(const trans_affine& o);

    bool is_identity(void) const;
    scalar determinant(void) const;
    scalar rotation(void) const;
    void translation(scalar* dx, scalar* dy) const;
    void scaling(scalar* x, scalar* y) const;
    void shearing(scalar* x, scalar* y) const;

    void transform(scalar* x, scalar* y) const;
    void transform_2x2(scalar* x, scalar* y) const;
    void inverse_transform(scalar* x, scalar* y) const;

    void store_to(scalar* m) const;
    void load_from(const scalar* m);
public:
    const trans_affine& operator *= (const trans_affine& o) { return multiply(o); }
    abstract_trans_affine* impl(void) const { return m_impl; }
private:
    friend bool operator == (const trans_affine& a, const trans_affine& b);
    abstract_trans_affine* m_impl;
};

inline bool operator == (const trans_affine& a, const trans_affine& b)
{
    return a.impl()->is_equal(b.impl());
}

inline bool operator != (const trans_affine& a, const trans_affine& b)
{
    return !a.impl()->is_equal(b.impl());
}

inline trans_affine operator * (const trans_affine& a, const trans_affine& b)
{
    return trans_affine(a).multiply(b);
}

}
#endif/*_PICASSO_MATRIX_H_*/
