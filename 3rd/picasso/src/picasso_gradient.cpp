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
#include "picasso_gradient.h"

namespace picasso {

gradient_adapter::gradient_adapter()
    : m_impl(0)
{
    m_impl = get_system_device()->create_gradient_adapter();
}

gradient_adapter::~gradient_adapter()
{
    get_system_device()->destroy_gradient_adapter(m_impl);
}

void gradient_adapter::init_linear(int spread, scalar x1, scalar y1, scalar x2, scalar y2)
{
    if (m_impl)
        m_impl->init_linear(spread, x1, y1, x2, y2);
}

void gradient_adapter::init_radial(int spread, scalar x1, scalar y1, scalar radius1,
                                                            scalar x2, scalar y2, scalar radius2)
{
    if (m_impl)
        m_impl->init_radial(spread, x1, y1, radius1, x2, y2, radius2);
}

void gradient_adapter::init_conic(int spread, scalar x, scalar y, scalar angle)
{
    if (m_impl)
        m_impl->init_conic(spread, x, y, angle);
}

void gradient_adapter::add_color_stop(scalar offset, const rgba& c)
{
    if (m_impl)
        m_impl->add_color_stop(offset, c);
}

void gradient_adapter::clear_stops(void)
{
    if (m_impl)
        m_impl->clear_stops();
}

void gradient_adapter::transform(const trans_affine& mtx)
{
    if (m_impl)
        m_impl->transform(mtx.impl());
}

}
