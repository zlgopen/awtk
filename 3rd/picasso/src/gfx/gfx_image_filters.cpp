/* Picasso - a vector graphics library
 *
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"

#include "gfx_image_filters.h"
#include "picasso.h"

namespace gfx {

// image_filter
template<typename FilterType>
class image_filter : public image_filter_adapter
{
public:
    image_filter()
    {
        calculate(m_filter);
    }

private:
    FilterType m_filter;
};

// filter_bilinear
class image_filter_bilinear
{
public:
    scalar radius(void) const { return FLT_TO_SCALAR(1.0f); }
    scalar calc_weight(scalar x) const
    {
        return FLT_TO_SCALAR(1.0f) - x;
    }
};

// filter_gaussian
class image_filter_gaussian
{
public:
    scalar radius(void) const { return FLT_TO_SCALAR(2.0f); }
    scalar calc_weight(scalar x) const
    {
        return Exp(-FLT_TO_SCALAR(2.0f) * x * x) * Sqrt(FLT_TO_SCALAR(2.0f) * _1divPI);
    }
};

// filter creater
image_filter_adapter* create_image_filter(int filter)
{
    switch (filter) {
        case FILTER_BILINEAR:
            return new image_filter<image_filter_bilinear>;
        case FILTER_GAUSSIAN:
            return new image_filter<image_filter_gaussian>;
        default:
            //FILTER_NEAREST: no filter
            return 0;
    }
}

}
