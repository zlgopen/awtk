/* Picasso - a vector graphics library
 *
 * Copyright (C) 2013 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _PICASSO_FONT_CACHE_H_
#define _PICASSO_FONT_CACHE_H_

#include "common.h"
#include "data_vector.h"
#include "device.h"
#include "interfaces.h"

namespace picasso {

class glyph_cache_manager
{
    enum {
        block_size = 16384-16
    };

public:
    glyph_cache_manager()
        : m_allocator(block_size)
        , m_signature(0)
    {
        memset(m_glyphs, 0, sizeof(m_glyphs));
    }

    ~glyph_cache_manager()
    {
        // block_allocator will free all memory.
    }

    void set_signature(const char* font_signature)
    {
        m_signature = (char*)m_allocator.allocate((unsigned int)strlen(font_signature)+1);
        strcpy(m_signature, font_signature);
    }

    const char* signature(void) const
    {
        return m_signature;
    }

    const glyph* find_glyph(unsigned int code) const
    {
        unsigned int msb = (code >> 8) & 0xFF;
        if (m_glyphs[msb]) { // find row
            return m_glyphs[msb][code & 0xFF];
        }
        return 0;
    }

    glyph* cache_glyph(unsigned int code, unsigned int index, unsigned int data_size, glyph_type data_type,
                                            const rect& bounds, scalar height, scalar advance_x, scalar advance_y)
    {
        unsigned int msb = (code >> 8) & 0xFF;
        if (m_glyphs[msb] == 0) { // cache row is empty.
            // alloc cache row.
            m_glyphs[msb] = (glyph**)m_allocator.allocate(sizeof(glyph*) * 256, sizeof(glyph*));
            memset(m_glyphs[msb], 0, sizeof(glyph*) * 256);
        }

        unsigned int lsb = code & 0xFF;
        if (m_glyphs[msb][lsb])
            return 0; // already exists.

        glyph* g = (glyph*)m_allocator.allocate(sizeof(glyph), sizeof(int));

        g->code         = code;
        g->index        = index;
        g->data         = m_allocator.allocate(data_size);
        g->data_size    = data_size;
        g->type         = data_type;
        g->bounds       = bounds;
        g->height        = height;
        g->advance_x    = advance_x;
        g->advance_y    = advance_y;
        return m_glyphs[msb][lsb] = g;
    }

private:
    glyph_cache_manager(const glyph_cache_manager&);
    glyph_cache_manager& operator=(const glyph_cache_manager&);

    block_allocator m_allocator;
    glyph**         m_glyphs[256];
    char*           m_signature;
};

}
#endif /*_PICASSO_FONT_CACHE_H_*/
