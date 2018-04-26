/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "device.h"
#include "interfaces.h"

#include "picasso_global.h"
#include "picasso_mask.h"

namespace picasso {

mask_layer::mask_layer(byte* buf, unsigned int width, unsigned int height, int stride, int type)
    : m_impl(0)
{
    attach(buf, width, height, stride);
    set_mask_type(type);
}

mask_layer::~mask_layer()
{
    if (m_impl)
        get_system_device()->destroy_mask_layer(m_impl);
}

void mask_layer::attach(byte* buf, unsigned int width, unsigned int height, int stride)
{
    if (m_impl)
        get_system_device()->destroy_mask_layer(m_impl);

    m_impl = get_system_device()->create_mask_layer(buf, width, height, stride);
}

void mask_layer::set_mask_type(int type)
{
    if (m_impl)
        m_impl->set_mask_type(type);
}

void mask_layer::add_filter_color(const rgba& c)
{
    if (m_impl)
        m_impl->add_filter_color(c);
}

void mask_layer::clear_filter_colors(void)
{
    if (m_impl)
        m_impl->clear_filter_colors();
}

}
