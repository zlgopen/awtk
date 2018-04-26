/* Picasso - a vector graphics library
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _PICASSO_RENDERING_BUFFER_H_
#define _PICASSO_RENDERING_BUFFER_H_

#include "common.h"
#include "device.h"
#include "color_type.h"
#include "interfaces.h"

namespace picasso {

class rendering_buffer
{
public:
    rendering_buffer();
    rendering_buffer(byte* buf, unsigned int width, unsigned int height, int stride);
    ~rendering_buffer();
public:
    void attach(byte* buf, unsigned int width, unsigned int height, int stride);
    void replace(byte* buf, unsigned int width, unsigned int height, int stride);
    bool is_empty(void);

    byte * buffer(void) const;
    unsigned int width(void) const;
    unsigned int height(void) const;
    int stride(void) const;

    bool is_transparent(void) const;
    void set_transparent(bool b);

    bool has_color_channel(void) const;
    void clear_color_channel(void);
    void set_color_channel(const rgba& c);
    rgba get_color_channel(void) const;

    abstract_rendering_buffer* impl(void) const { return m_impl; }
private:
    friend class painter;
    rendering_buffer(const rendering_buffer& o);
    rendering_buffer& operator=(const rendering_buffer& o);
    abstract_rendering_buffer * m_impl;
};

}
#endif/*_PICASSO_RENDERING_BUFFER_H_*/
