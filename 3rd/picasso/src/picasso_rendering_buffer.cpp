/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "device.h"
#include "interfaces.h"
#include "graphic_path.h"

#include "picasso.h"
#include "picasso_global.h"
#include "picasso_rendering_buffer.h"
#include "picasso_objects.h"

namespace picasso {

rendering_buffer::rendering_buffer()
    : m_impl(0)
{
}

rendering_buffer::rendering_buffer(byte* buf, unsigned int width, unsigned int height, int stride)
    : m_impl(0)
{
    attach(buf, width, height, stride);
}

rendering_buffer::~rendering_buffer()
{
    if (m_impl)
        get_system_device()->destroy_rendering_buffer(m_impl);
}

void rendering_buffer::attach(byte* buf, unsigned int width, unsigned int height, int stride)
{
    if (m_impl)
        get_system_device()->destroy_rendering_buffer(m_impl);

    m_impl = get_system_device()->create_rendering_buffer(buf, width, height, stride);
}

void rendering_buffer::replace(byte* buf, unsigned int width, unsigned int height, int stride)
{
    m_impl->init(buf, width, height, stride);
}

bool rendering_buffer::is_empty(void)
{
    return !m_impl;
}

unsigned int rendering_buffer::width(void) const 
{ 
    if (m_impl)
        return m_impl->width(); 

    return 0;
}

unsigned int rendering_buffer::height(void) const 
{ 
    if (m_impl)
        return m_impl->height(); 

    return 0;
}

int rendering_buffer::stride(void) const 
{ 
    if (m_impl)
        return m_impl->stride(); 

    return 0;
}

byte * rendering_buffer::buffer(void) const
{
    if (m_impl)
        return m_impl->buffer(); 

    return 0;
}

bool rendering_buffer::is_transparent(void) const 
{ 
    if (m_impl)
        return m_impl->is_transparent();

    return false; // defalt false
} 

void rendering_buffer::set_transparent(bool b) 
{
    if (m_impl)
        m_impl->set_transparent(b);
}

bool rendering_buffer::has_color_channel(void) const 
{
    if (m_impl)
        return m_impl->has_color_channel();

    return false; // default false
} 

void rendering_buffer::clear_color_channel(void) 
{
    if (m_impl)
        m_impl->clear_color_channel();
}

void rendering_buffer::set_color_channel(const rgba& c) 
{
    if (m_impl)
        m_impl->set_color_channel(c);
}

rgba rendering_buffer::get_color_channel(void) const 
{ 
    if (m_impl)
        return m_impl->get_color_channel();

    return rgba(0,0,0,0); 
}

}
