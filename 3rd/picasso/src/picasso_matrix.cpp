/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "device.h"
#include "interfaces.h"

#include "picasso_global.h"
#include "picasso_matrix.h"

namespace picasso {

trans_affine::trans_affine()
{
    //Identity matrix
    m_impl = get_system_device()->create_trans_affine(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
}

trans_affine::trans_affine(scalar sx, scalar shy, scalar shx, scalar sy, scalar tx, scalar ty)
{
    //Custom matrix
    m_impl = get_system_device()->create_trans_affine(sx, shy, shx, sy, tx, ty);
}

trans_affine::trans_affine(const trans_affine& o)
{
    m_impl = get_system_device()->create_trans_affine(o.sx(), o.shy(), o.shx(), o.sy(), o.tx(), o.ty());
}

trans_affine& trans_affine::operator=(const trans_affine& o)
{
    if (this == &o)
        return *this;

    get_system_device()->destroy_trans_affine(m_impl);
    m_impl = get_system_device()->create_trans_affine(o.sx(), o.shy(), o.shx(), o.sy(), o.tx(), o.ty());

    return *this;
}

trans_affine::~trans_affine()
{
    get_system_device()->destroy_trans_affine(m_impl);
}

void trans_affine::sx(scalar v)
{
    m_impl->sx(v);
}

void trans_affine::sy(scalar v)
{
    m_impl->sy(v);
}

scalar trans_affine::sx(void) const
{
    return m_impl->sx();
}

scalar trans_affine::sy(void) const
{
    return m_impl->sy();
}

void trans_affine::shx(scalar v)
{
    m_impl->shx(v);
}

void trans_affine::shy(scalar v)
{
    m_impl->shy(v);
}

scalar trans_affine::shx(void) const
{
    return m_impl->shx();
}

scalar trans_affine::shy(void) const
{
    return m_impl->shy();
}

void trans_affine::tx(scalar v)
{
    m_impl->tx(v);
}

void trans_affine::ty(scalar v)
{
    m_impl->ty(v);
}

scalar trans_affine::tx(void) const
{
    return m_impl->tx();
}

scalar trans_affine::ty(void) const
{
    return m_impl->ty();
}

void trans_affine::transform(scalar* x, scalar* y) const
{
    m_impl->transform(x, y);
}

void trans_affine::transform_2x2(scalar* x, scalar* y) const
{
    m_impl->transform_2x2(x, y);
}

void trans_affine::inverse_transform(scalar* x, scalar* y) const
{
    m_impl->inverse_transform(x, y);
}

const trans_affine& trans_affine::translate(scalar x, scalar y)
{
    m_impl->translate(x, y);
    return *this;
}

const trans_affine& trans_affine::scale(scalar x, scalar y)
{
    m_impl->scale(x, y);
    return *this;
}

const trans_affine& trans_affine::rotate(scalar a)
{
    m_impl->rotate(a);
    return *this;
}

const trans_affine& trans_affine::shear(scalar x, scalar y)
{
    m_impl->shear(x, y);
    return *this;
}

const trans_affine& trans_affine::invert(void)
{
    m_impl->invert();
    return *this;
}

const trans_affine& trans_affine::flip_x(void)
{
    m_impl->flip_x();
    return *this;
}

const trans_affine& trans_affine::flip_y(void)
{
    m_impl->flip_y();
    return *this;
}

const trans_affine& trans_affine::reset(void)
{
    m_impl->reset();
    return *this;
}

const trans_affine& trans_affine::multiply(const trans_affine& o)
{
    m_impl->multiply(o.m_impl);
    return *this;
}

bool trans_affine::is_identity(void) const
{
    return m_impl->is_identity();
}

scalar trans_affine::determinant(void) const
{
    return m_impl->determinant();
}

scalar trans_affine::rotation(void) const
{
    return m_impl->rotation();
}

void trans_affine::translation(scalar* dx, scalar* dy) const
{
    m_impl->translation(dx, dy);
}

void trans_affine::scaling(scalar* x, scalar* y) const
{
    m_impl->scaling(x, y);
}

void trans_affine::shearing(scalar* x, scalar* y) const
{
    m_impl->shearing(x, y);
}

void trans_affine::store_to(scalar* m) const
{
    m_impl->store_to(m);
}

void trans_affine::load_from(const scalar* m)
{
    m_impl->load_from(m);
}

}
