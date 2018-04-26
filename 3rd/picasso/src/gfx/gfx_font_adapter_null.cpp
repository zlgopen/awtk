/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2013 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include <stdio.h>
#include "common.h"
#include "convert.h"
#include "gfx_font_adapter.h"
#include "gfx_rasterizer_scanline.h"
#include "gfx_scanline.h"
#include "gfx_scanline_renderer.h"
#include "gfx_scanline_storage.h"
#include "gfx_trans_affine.h"

#include "graphic_path.h"
#include "graphic_helper.h"
#include "graphic_base.h"

#ifndef ENABLE_FREE_TYPE2

namespace gfx {

class font_adapter_impl
{
public:
    font_adapter_impl()
        : antialias(false)
        , flip_y(false)
        , hinting(false)
        , weight(0)
        , height(0)
        , ascent(0)
        , descent(0)
        , leading(0)
        , units_per_em(0)
        , cur_glyph_index(0)
        , cur_data_size(0)
        , cur_data_type(glyph_type_outline)
        , cur_bound_rect(0,0,0,0)
        , cur_advance_x(0)
        , cur_advance_y(0)

    {
    }

    ~font_adapter_impl()
    {
    }
    
    bool antialias;
    bool flip_y;
    bool hinting;
    scalar weight;
    gfx_trans_affine matrix;
    scalar height;
    scalar ascent;
    scalar descent;
    scalar leading;
    unsigned int units_per_em;
    //font special 
    //current glyph
    unsigned int cur_glyph_index;
    unsigned int cur_data_size;
    glyph_type cur_data_type;
    rect cur_bound_rect;
    scalar cur_advance_x;
    scalar cur_advance_y;
    picasso::graphic_path cur_font_path;
    gfx_scanline_storage_bin cur_font_scanlines_bin;
    gfx_serialized_scanlines_adaptor_bin cur_font_storage_bin;
};

gfx_font_adapter::gfx_font_adapter(const char* name, int charset, scalar height, scalar weight, 
                                bool italic, bool hint, bool flip, bool a, const abstract_trans_affine* mtx)
    :m_impl(new font_adapter_impl)
{
    m_impl->antialias = a;
    m_impl->flip_y = flip;
    m_impl->hinting = hint;
    m_impl->weight = weight;
    m_impl->matrix = *static_cast<gfx_trans_affine*>(const_cast<abstract_trans_affine*>(mtx));
    if (italic)
        m_impl->matrix.shear(-0.4f, 0.0f);

    m_impl->height = height;

}

gfx_font_adapter::~gfx_font_adapter()
{

    delete m_impl;
}

void gfx_font_adapter::active(void)
{
}

void gfx_font_adapter::deactive(void)
{
}

static inline float int26p6_to_flt(int p)
{
    return float(p) / 64.0f;
}

void gfx_font_adapter::add_kerning(unsigned int first, unsigned int second, scalar* x, scalar* y)
{
}

bool gfx_font_adapter::prepare_glyph(unsigned int code)
{
    return false;
}

void gfx_font_adapter::write_glyph_to(byte* buffer)
{
    if (buffer && m_impl->cur_data_size) {
        if (m_impl->cur_data_type == glyph_type_outline) {
            unsigned int count = m_impl->cur_font_path.total_vertices();
            mem_copy(buffer, &count, sizeof(unsigned int));
            buffer += sizeof(unsigned int);
            m_impl->cur_font_path.serialize_to(buffer);
        } else { // mono glyph
            m_impl->cur_font_scanlines_bin.serialize(buffer);
        }
    }
}

void* gfx_font_adapter::create_storage(byte* buf, unsigned int len, scalar x, scalar y)
{
    m_impl->cur_font_storage_bin.init(buf, len, SCALAR_TO_FLT(Ceil(x)), SCALAR_TO_FLT(Ceil(y)));
    return (void*)&m_impl->cur_font_storage_bin;
}

void gfx_font_adapter::destroy_storage(void*)
{
    // do nothing
}

void gfx_font_adapter::translate_storage(void* storage, scalar x, scalar y)
{
    gfx_serialized_scanlines_adaptor_bin* sd = (gfx_serialized_scanlines_adaptor_bin*)storage;
    int ox = sd->x();
    int oy = sd->y();
    sd->setX(ox + SCALAR_TO_INT(Ceil(x)));
    sd->setY(oy + SCALAR_TO_INT(Ceil(y)));
}

scalar gfx_font_adapter::height(void) const
{
    return m_impl->height;
}

scalar gfx_font_adapter::ascent(void) const
{
    return m_impl->ascent;
}

scalar gfx_font_adapter::descent(void) const
{
    return m_impl->descent;
}

scalar gfx_font_adapter::leading(void) const
{
    return m_impl->leading;
}

unsigned int gfx_font_adapter::units_per_em(void) const
{
    return m_impl->units_per_em;
}
unsigned int gfx_font_adapter::glyph_index(void) const
{
    return m_impl->cur_glyph_index;
}

unsigned int gfx_font_adapter::data_size(void) const
{
    return m_impl->cur_data_size;
}

glyph_type gfx_font_adapter::data_type(void) const
{
    return m_impl->cur_data_type;
}

const rect& gfx_font_adapter::bounds(void) const
{
    return m_impl->cur_bound_rect;
}

scalar gfx_font_adapter::advance_x(void) const
{
    return m_impl->cur_advance_x;
}

scalar gfx_font_adapter::advance_y(void) const
{
    return m_impl->cur_advance_y;
}

}

bool platform_font_init(void)
{
    // win32 do nothing
    return true;
}

void platform_font_shutdown(void)
{
    // win32 do nothing
}

#endif
