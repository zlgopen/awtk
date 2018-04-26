/* Picasso - a vector graphics library
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_RASTER_ADAPTER_H_
#define _GFX_RASTER_ADAPTER_H_

#include "common.h"
#include "interfaces.h"

#include "gfx_rasterizer_scanline.h"
#include "gfx_trans_affine.h"

namespace gfx {

class gfx_raster_adapter_impl;

class gfx_raster_adapter : public abstract_raster_adapter
{
public:
    gfx_raster_adapter();
    virtual ~gfx_raster_adapter();

    virtual void set_gamma_power(scalar g);
    virtual void set_antialias(bool b);
    virtual void set_transform(const abstract_trans_affine* mtx);
    virtual void set_raster_method(unsigned int m);

    virtual void add_shape(const vertex_source& vs, unsigned int id);
    virtual void reset(void);

    virtual void set_stroke_dashes(scalar start, const scalar* dashes, unsigned int num);
    virtual void set_stroke_attr(int idx, int val);
    virtual void set_stroke_attr_val(int idx, scalar val);
    virtual void set_fill_attr(int idx, int val);

    virtual void commit(void);
    virtual bool is_empty(void);
    virtual bool contains(scalar x, scalar y);

    unsigned int raster_method(void) const;
    gfx_rasterizer_scanline_aa<>& stroke_impl(void) { return m_sraster; }
    gfx_rasterizer_scanline_aa<>& fill_impl(void) { return m_fraster; }
    gfx_trans_affine transformation(void) const;
private:
    void setup_stroke_raster(void);
    void setup_fill_raster(void);

    gfx_raster_adapter_impl * m_impl;
    gfx_rasterizer_scanline_aa<> m_sraster;
    gfx_rasterizer_scanline_aa<> m_fraster;
};

}
#endif /*_GFX_RASTER_ADAPTER_H_*/
