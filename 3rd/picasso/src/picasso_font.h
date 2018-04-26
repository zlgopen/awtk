/* Picasso - a vector graphics library
 *
 * Copyright (C) 2013 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _PICASSO_FONT_H_
#define _PICASSO_FONT_H_

#include "common.h"
#include "device.h"
#include "interfaces.h"

#include "graphic_path.h"
#include "picasso_global.h"
#include "picasso_matrix.h"
#include "picasso_font_cache.h"

#if ENABLE(LOW_MEMORY)
#define MAX_FONTS 4
#else
#define MAX_FONTS 16
#endif

#define MAX_FONT_NAME_LENGTH 128

namespace picasso {

enum {
    charset_latin,
    charset_unicode,
};

// font desc
class font_desc
{
public:
    font_desc()
        : m_name(0), m_charset(charset_latin)
        , m_height(0), m_weight(0), m_italic(false), m_hint(false), m_flip_y(false)
    {
    }

    font_desc(const char* face_name)
        : m_name(0), m_charset(charset_latin)
        , m_height(0), m_weight(0), m_italic(false), m_hint(false), m_flip_y(false)
    {
        size_t len = MIN(strlen(face_name)+1, MAX_FONT_NAME_LENGTH);
        m_name = (char*)mem_malloc(len);
        strncpy(m_name, face_name, len);
    }

    ~font_desc()
    {
        if (m_name)
            mem_free(m_name);
    }

    font_desc(const font_desc& o)
    {
        size_t len = strlen(o.m_name)+1;
        m_name = (char*)mem_malloc(len);
        strncpy(m_name, o.m_name, len);

        m_charset = o.m_charset;
        m_height = o.m_height;
        m_weight = o.m_weight;
        m_italic = o.m_italic;
        m_hint = o.m_hint;
        m_flip_y = o.m_flip_y;
    }

    font_desc& operator=(const font_desc& o)
    {
        if (this == &o)
            return *this;

        if (m_name)
            mem_free(m_name);

        size_t len = strlen(o.m_name)+1;
        m_name = (char*)mem_malloc(len);
        strncpy(m_name, o.m_name, len);

        m_charset = o.m_charset;
        m_height = o.m_height;
        m_weight = o.m_weight;
        m_italic = o.m_italic;
        m_hint = o.m_hint;
        m_flip_y = o.m_flip_y;

        return *this;
    }

    void set_charset(int charset) { m_charset = charset; }
    void set_height(scalar h) { m_height = h; }
    void set_weight(scalar w) { m_weight = w; }
    void set_italic(bool i) { m_italic = i; }
    void set_hint(bool h) { m_hint = h; }
    void set_flip_y(bool f) { m_flip_y = f; }

    const char* name(void) const { return m_name; }
    int charset(void) const { return m_charset; }
    scalar height(void) const { return m_height; }
    scalar weight(void) const { return m_weight; }
    bool italic(void) const { return m_italic; }
    bool hint(void) const { return m_hint; }
    bool flip_y(void) const { return m_flip_y; }

private:
    char*  m_name;
    int    m_charset;
    scalar m_height;
    scalar m_weight;
    bool   m_italic;
    bool   m_hint;
    bool   m_flip_y;
};

inline bool operator == (const font_desc& a, const font_desc& b)
{
    return (a.charset() == b.charset()) &&
           (a.height() == b.height()) &&
           (a.weight() == b.weight()) &&
           (a.italic() == b.italic()) &&
           (a.hint() == b.hint()) &&
           (a.flip_y() == b.flip_y()) &&
           (strcmp(a.name(), b.name()) == 0);
}


// mono glyph storage
class mono_storage
{
public:
    mono_storage()
        : font(0)
        , storage(0)
    {
    }

    ~mono_storage()
    {
        clear();
    }

    void clear(void)
    {
        if (storage && font)
            font->destroy_storage(storage);

        storage = font = 0;
    }

    void set_font(abstract_font_adapter* f)
    {
        font = f;
    }

    void serialize_from(byte* data, unsigned int size, scalar x, scalar y)
    {
        if (font) {
            if (storage)
                font->destroy_storage(storage);

            storage = font->create_storage(data, size, x, y);
        }
    }

    void translate(scalar x, scalar y)
    {
        if (font && storage)
            font->translate_storage(storage, x, y);
    }

    void* get_storage(void) { return storage; }
private:
    abstract_font_adapter* font;
    void* storage;
};

// font adapter
class font_adapter
{
public:
    font_adapter(const font_desc& desc, const char* signature, const trans_affine& mtx, bool antialias)
        : m_desc(desc)
        , m_cache(new glyph_cache_manager)
        , m_impl(0)
        , m_prev_glyph(0)
        , m_last_glyph(0)
    {
        m_cache->set_signature(signature);
        m_impl = get_system_device()->create_font_adapter(desc.name(), desc.charset(), desc.height(),
                                    desc.weight(), desc.italic(), desc.hint(), desc.flip_y(), antialias, mtx.impl());

        // for mono font
        m_mono_storage.set_font(m_impl);
    }

    ~font_adapter()
    {
        // for mono font
        m_mono_storage.clear();

        get_system_device()->destroy_font_adapter(m_impl);
        delete m_cache;
    }

    scalar height(void) const { return m_impl->height(); }
    scalar ascent(void) const { return m_impl->ascent(); }
    scalar descent(void) const { return m_impl->descent(); }
    scalar leading(void) const { return m_impl->leading(); }
    unsigned int units_per_em(void) const { return m_impl->units_per_em(); }

    const glyph* get_glyph(unsigned int code);

    const char* signature(void) const { return m_cache->signature(); }
    const font_desc& desc(void) const{ return m_desc; }

    bool generate_raster(const glyph* g, scalar x, scalar y);
    void add_kerning(scalar* x, scalar* y);
    graphic_path& path_adaptor(void) { return m_path_adaptor; }
    mono_storage& mono_adaptor(void) { return m_mono_storage; }
public:
    void active(void);
    void deactive(void);

    static bool create_signature(const font_desc&, const trans_affine& m, bool a, char* recv_sig);
private:
    font_adapter(const font_adapter&);
    font_adapter& operator=(const font_adapter&);

    font_desc m_desc;
    glyph_cache_manager * m_cache;
    abstract_font_adapter * m_impl;
    graphic_path m_path_adaptor;
    mono_storage m_mono_storage;
    const glyph* m_prev_glyph;
    const glyph* m_last_glyph;
};


// font engine
class font_engine
{
public:
    font_engine(unsigned int max_fonts = MAX_FONTS);
    ~font_engine();

    void set_antialias(bool b);
    void set_transform(const trans_affine& mtx);

    bool create_font(const font_desc& desc);

    bool stamp_change(void) const { return m_stamp_change; }
    bool antialias(void) const { return m_antialias; }
    font_adapter* current_font(void) const { return m_current; }

    static bool initialize(void);
    static void shutdown(void);
private:
    font_engine(const font_engine&);
    font_engine& operator=(const font_engine&);

    int find_font(const char* font_signature);

    font_adapter** m_fonts;
    font_adapter* m_current;
    unsigned int m_max_fonts;
    unsigned int m_num_fonts;
    char* m_signature;
    trans_affine m_affine;
    bool m_stamp_change;
    bool m_antialias;
};

}
#endif /*_PICASSO_FONT_H_*/
