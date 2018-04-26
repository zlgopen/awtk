/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_DEVICE_H_
#define _GFX_DEVICE_H_

#include "common.h"
#include "device.h"
#include "interfaces.h"

namespace gfx {

class gfx_device : public device
{
public:
    static gfx_device* create(void);
    virtual ~gfx_device();

    virtual abstract_trans_affine* create_trans_affine(scalar sx, scalar shy,
                                        scalar shx, scalar sy, scalar tx, scalar ty);
    virtual void destroy_trans_affine(abstract_trans_affine* m);

    virtual abstract_painter* create_painter(pix_fmt fmt);
    virtual void destroy_painter(abstract_painter* p);

    virtual abstract_raster_adapter* create_raster_adapter(void);
    virtual void destroy_raster_adapter(abstract_raster_adapter* d);

    virtual abstract_rendering_buffer* create_rendering_buffer(byte* buf,
                                    unsigned int width, unsigned int height, int stride);
    virtual void destroy_rendering_buffer(abstract_rendering_buffer* b);

    virtual abstract_mask_layer* create_mask_layer(byte* buf,
                                    unsigned int width, unsigned int height, int stride);
    virtual void destroy_mask_layer(abstract_mask_layer* m);

    virtual abstract_gradient_adapter* create_gradient_adapter(void);
    virtual void destroy_gradient_adapter(abstract_gradient_adapter* g);


    virtual abstract_font_adapter* create_font_adapter(const char* name, int charset,
                                    scalar height, scalar weight, bool italic, bool hint, bool flip,
                                    bool antialias, const abstract_trans_affine* mtx);
    virtual void destroy_font_adapter(abstract_font_adapter* f);
protected:
    gfx_device();

};

}
#endif /*_GFX_DEVICE_H_*/
