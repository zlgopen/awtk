/* Picasso - a vector graphics library
 *
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_IMAGE_FILTERS_H_
#define _GFX_IMAGE_FILTERS_H_

#include "common.h"
#include "data_vector.h"
#include "interfaces.h"
#include "graphic_base.h"

namespace gfx {

// image filter adapter
class image_filter_adapter
{
public:
    image_filter_adapter()
        : m_radius(FLT_TO_SCALAR(0.0f))
        , m_start(0)
        , m_diameter(0)
    {
    }

    template <typename FilterType>
    void calculate(const FilterType& filter, bool normalization = true)
    {
        scalar r = filter.radius();
        realloc_filter_lut(r);
        unsigned int pivot = diameter() << (image_subpixel_shift - 1);

        for (unsigned int i = 0; i < pivot; i++) {
            scalar x = INT_TO_SCALAR(i) / INT_TO_SCALAR(image_subpixel_scale);
            scalar y = filter.calc_weight(x);
            m_weight_array[pivot + i] = m_weight_array[pivot - i] = (int16_t)iround(y * image_filter_scale);
        }

        unsigned int end = (diameter() << image_subpixel_shift) - 1;
        m_weight_array[0] = m_weight_array[end];

        if (normalization)
            normalize();
    }

    int start(void) const { return m_start; }
    scalar radius(void) const { return m_radius; }
    unsigned int diameter(void) const { return m_diameter; }
    const int16_t* weight_array(void) const { return &m_weight_array[0]; }

    void normalize(void)
    {
        int flip = 1;

        for (unsigned int i = 0; i < image_subpixel_scale; i++) {
            for (;;) {
                int sum = 0;
                unsigned int j;
                for (j = 0; j < m_diameter; j++) {
                    sum += m_weight_array[j * image_subpixel_scale + i];
                }

                if (sum == image_filter_scale)
                    break; // break loop

                scalar k = INT_TO_SCALAR(image_filter_scale) / INT_TO_SCALAR(sum);
                sum = 0;
                for (j = 0; j < m_diameter; j++) {
                    sum += m_weight_array[j * image_subpixel_scale + i] =
                        iround(m_weight_array[j * image_subpixel_scale + i] * k);
                }

                sum -= image_filter_scale;
                int inc = (sum > 0) ? -1 : 1;

                for (j = 0; j < m_diameter && sum; j++) {
                    flip ^= 1;
                    unsigned int idx = flip ? m_diameter / 2 + j / 2 : m_diameter / 2 - j / 2;
                    int v = m_weight_array[idx * image_subpixel_scale + i];
                    if (v < image_filter_scale) {
                        m_weight_array[idx * image_subpixel_scale + i] += inc;
                        sum += inc;
                    }
                }
            }
        }

        unsigned int pivot = m_diameter << (image_subpixel_shift - 1);

        for (unsigned int i = 0; i < pivot; i++) {
            m_weight_array[pivot + i] = m_weight_array[pivot - i];
        }

        unsigned int end = (diameter() << image_subpixel_shift) - 1;
        m_weight_array[0] = m_weight_array[end];
    }

private:
    image_filter_adapter(const image_filter_adapter&);
    image_filter_adapter& operator=(const image_filter_adapter&);

    void realloc_filter_lut(scalar radius)
    {
        m_radius = radius;
        m_diameter = (unsigned int)Ceil(radius) * 2;
        m_start = -(int)(m_diameter / 2 - 1);
        unsigned int size = m_diameter << image_subpixel_shift;

        if (size > m_weight_array.size())
            m_weight_array.resize(size);
    }

    scalar m_radius;
    int m_start;
    unsigned int m_diameter;
    pod_array<int16_t> m_weight_array;
};


// filter creater
image_filter_adapter* create_image_filter(int filter);

}
#endif /*_GFX_IMAGE_FILTERS_H_*/
