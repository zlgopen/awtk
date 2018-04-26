/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "device.h"
#include "interfaces.h"

#include "picasso.h"
#include "picasso_global.h"
#include "picasso_raster_adapter.h"
#include "picasso_objects.h"
#include "picasso_matrix.h"

namespace picasso {

raster_adapter::raster_adapter()
{
    m_impl = get_system_device()->create_raster_adapter();
}

raster_adapter::~raster_adapter()
{
    get_system_device()->destroy_raster_adapter(m_impl);
}

void raster_adapter::set_gamma_power(scalar g)
{
    m_impl->set_gamma_power(g);
}

void raster_adapter::set_antialias(bool b)
{
    m_impl->set_antialias(b);
}

void raster_adapter::reset(void)
{
    m_impl->reset();
}

bool raster_adapter::is_empty(void)
{
    return m_impl->is_empty();
}

void raster_adapter::add_shape(const vertex_source& vs, unsigned int id)
{
    m_impl->add_shape(vs, id);
}

void raster_adapter::set_stroke_dashes(scalar start, const scalar* dashes, unsigned int num)
{
    m_impl->set_stroke_dashes(start, dashes, num);
}

void raster_adapter::set_stroke_attr_val(int idx, scalar val)
{
    m_impl->set_stroke_attr_val(idx, val);
}

void raster_adapter::set_stroke_attr(int idx, int val)
{
    m_impl->set_stroke_attr(idx, val);
}

void raster_adapter::set_fill_attr(int idx, int val)
{
    m_impl->set_fill_attr(idx, val);
}

void raster_adapter::set_raster_method(unsigned int m)
{
    m_impl->set_raster_method(m);
}

void raster_adapter::set_transform(const trans_affine& mtx)
{
    m_impl->set_transform(mtx.impl());
}

void raster_adapter::commit(void)
{
    m_impl->commit();
}

//static methods
bool raster_adapter::fill_contents_point(const vertex_source& vs, scalar x, scalar y, filling_rule rule)
{
    bool ret = false;
    abstract_raster_adapter* rs = get_system_device()->create_raster_adapter();
    trans_affine mtx;
    if (rs) {
        rs->set_raster_method(raster_fill);
        rs->set_fill_attr(FIA_FILL_RULE, rule);
        rs->set_transform(mtx.impl());
        rs->add_shape(vs, 0);
        rs->commit();
        ret = rs->contains(x, y);
    }
    get_system_device()->destroy_raster_adapter(rs);
    return ret;
}

bool raster_adapter::stroke_contents_point(const vertex_source& vs, scalar x, scalar y, scalar w)
{
    bool ret = false;
    abstract_raster_adapter* rs = get_system_device()->create_raster_adapter();
    trans_affine mtx;
    if (rs) {
        rs->set_raster_method(raster_stroke);
        rs->set_stroke_attr_val(STA_WIDTH, w);
        rs->set_transform(mtx.impl());
        rs->add_shape(vs, 0);
        rs->commit();
        ret = rs->contains(x, y);
    }
    get_system_device()->destroy_raster_adapter(rs);
    return ret;
}

}
