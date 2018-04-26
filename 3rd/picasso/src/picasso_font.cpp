/* Picasso - a vector graphics library
 *
 * Copyright (C) 2013 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include <stdio.h>
#include "common.h"
#include "device.h"
#include "graphic_path.h"
#include "geometry.h"
#include "convert.h"

#include "picasso_global.h"
#include "picasso_private.h"
#include "picasso_painter.h"
#include "picasso_font.h"

namespace picasso {

font_engine::font_engine(unsigned int max_fonts)
    : m_fonts(pod_allocator<font_adapter*>::allocate(max_fonts))
    , m_current(0)
    , m_max_fonts(max_fonts)
    , m_num_fonts(0)
    , m_signature(0)
    , m_stamp_change(false)
    , m_antialias(false)
{
    memset(m_fonts, 0, sizeof(font_adapter*) * max_fonts);
    m_signature = (char*)mem_calloc(1, MAX_FONT_NAME_LENGTH + 128);
}

font_engine::~font_engine()
{
    if (m_signature)
        mem_free(m_signature);

    for (unsigned int i = 0; i < m_num_fonts; ++i)
        delete m_fonts[i];

    pod_allocator<font_adapter*>::deallocate(m_fonts, m_max_fonts);
}

void font_engine::set_antialias(bool b)
{
    if (m_antialias != b) {
        m_antialias = b;
        m_stamp_change = true;
    }
}

void font_engine::set_transform(const trans_affine& mtx)
{
    if (m_affine != mtx){
        m_affine = mtx;
        m_stamp_change = true;
    }
}

int font_engine::find_font(const char* font_signature)
{
    for (unsigned int i = 0; i < m_num_fonts; i++) {
        if(strcmp(m_fonts[i]->signature(), font_signature) == 0)
            return (int)i;
    }
    return -1;
}

bool font_engine::create_font(const font_desc& desc)
{
    if (!font_adapter::create_signature(desc, m_affine, m_antialias, m_signature))
        return false;

    if (m_current)
        m_current->deactive();

    int idx = find_font(m_signature);
    if (idx >= 0) {
        m_current = m_fonts[idx];
    } else {
        if (m_num_fonts >= m_max_fonts) {
            delete m_fonts[0];
            mem_copy(m_fonts, m_fonts + 1, (m_max_fonts - 1) * sizeof(font_adapter*));
            m_num_fonts = m_max_fonts - 1;
        }

        m_fonts[m_num_fonts] = new font_adapter(desc, m_signature, m_affine, m_antialias);
        m_current = m_fonts[m_num_fonts];
        m_num_fonts++;
    }

    m_current->active();
    m_stamp_change = false;
    return true;
}

bool font_engine::initialize(void)
{
    return platform_font_init();
}

void font_engine::shutdown(void)
{
    platform_font_shutdown();
}

// font adapter
bool font_adapter::create_signature(const font_desc& desc, const trans_affine& mtx, bool anti, char* recv_sig)
{
    if (!recv_sig)
        return false; //out of memory.

    recv_sig[0] = 0;

    sprintf(recv_sig,
            "%s,%d,%d,%d,%d,%d,%d,%d-",
            desc.name(),
            desc.charset(),
            (int)desc.height(),
            (int)desc.weight(),
            (int)desc.italic(),
            (int)desc.hint(),
            (int)desc.flip_y(),
            (int)anti);

    char mbuf[64] = {0};
    sprintf(mbuf,
            "%08X%08X%08X%08X%08X%08X",
            fxmath::flt_to_fixed(SCALAR_TO_FLT(mtx.sx())),
            fxmath::flt_to_fixed(SCALAR_TO_FLT(mtx.sy())),
            fxmath::flt_to_fixed(SCALAR_TO_FLT(mtx.shx())),
            fxmath::flt_to_fixed(SCALAR_TO_FLT(mtx.shy())),
            fxmath::flt_to_fixed(SCALAR_TO_FLT(mtx.tx())),
            fxmath::flt_to_fixed(SCALAR_TO_FLT(mtx.ty())));

    strcat(recv_sig, mbuf);
    return true;
}

void font_adapter::active(void)
{
    m_impl->active();
    m_prev_glyph = m_last_glyph = 0;
}

void font_adapter::deactive(void)
{
    m_prev_glyph = m_last_glyph = 0;
    m_impl->deactive();
}

void font_adapter::add_kerning(scalar* x, scalar* y)
{
    if (m_prev_glyph && m_last_glyph)
        m_impl->add_kerning(m_prev_glyph->index, m_last_glyph->index, x, y);
}

const glyph* font_adapter::get_glyph(unsigned int code)
{
    const glyph* gl = m_cache->find_glyph(code);
    if (gl) {
        m_prev_glyph = m_last_glyph;
        m_last_glyph = gl;
        return gl;
    } else {
        if (m_impl->prepare_glyph(code)) {
            m_prev_glyph = m_last_glyph;
            gl = m_cache->cache_glyph(code,
                                 m_impl->glyph_index(),
                                 m_impl->data_size(),
                                 m_impl->data_type(),
                                 m_impl->bounds(),
                                 m_impl->height(),
                                 m_impl->advance_x(),
                                 m_impl->advance_y());
            m_impl->write_glyph_to(gl->data);
            m_last_glyph = gl;
            return gl;
        }
    }
    return 0;
}

bool font_adapter::generate_raster(const glyph* g, scalar x, scalar y)
{
    if (g) {
        if (g->type == glyph_type_mono) {
            m_mono_storage.serialize_from(g->data, g->data_size, x, y);
            return true;
        } else {
            unsigned int count = 0;
            unsigned int offset = sizeof(unsigned int);
            mem_copy(&count, g->data, offset);
            m_path_adaptor.serialize_from(count, g->data+offset, g->data_size-offset);
            m_path_adaptor.translate(x, y);
        }
        return true;
    } else
        return false;
}

}
