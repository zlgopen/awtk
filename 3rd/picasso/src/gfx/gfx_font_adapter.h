/* Picasso - a vector graphics library
 *
 * Copyright (C) 2013 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_FONT_ADAPTER_H_
#define _GFX_FONT_ADAPTER_H_

#include "common.h"
#include "interfaces.h"

#include "picasso_font.h"

namespace gfx {

class font_adapter_impl;

class gfx_font_adapter : public abstract_font_adapter
{
public:
    gfx_font_adapter(const char* name, int charset, scalar height, scalar weight,
                    bool italic, bool hint, bool flip, bool antialias, const abstract_trans_affine* mtx);
    virtual ~gfx_font_adapter();

    virtual scalar height(void) const;
    virtual scalar ascent(void) const;
    virtual scalar descent(void) const;
    virtual scalar leading(void) const;
    virtual unsigned int units_per_em(void) const;

    virtual void active(void);
    virtual void deactive(void);

    virtual bool prepare_glyph(unsigned int code);
    virtual void write_glyph_to(byte* buffer);
    virtual void add_kerning(unsigned int f, unsigned int s, scalar* x, scalar* y);

    virtual unsigned int glyph_index(void) const;
    virtual unsigned int data_size(void) const;
    virtual glyph_type data_type(void) const;
    virtual const rect& bounds(void) const;
    virtual scalar advance_x(void) const;
    virtual scalar advance_y(void) const;

    virtual void* create_storage(byte* buf, unsigned int len, scalar x, scalar y);
    virtual void destroy_storage(void*);
    virtual void translate_storage(void*, scalar x, scalar y);
private:
    void load_kerning_pairs(void);
    void sort_kerning_pairs(void);
    font_adapter_impl* m_impl;
};

}
#endif /*_GFX_FONT_ADAPTER_H_*/
