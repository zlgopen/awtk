/* Picasso - a vector graphics library
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "math_type.h"

#include "device.h"
#include "gfx_device.h"

namespace picasso {

static device* _global_device = NULL;

bool _init_system_device(void)
{
    _global_device = gfx::gfx_device::create();
    return _global_device != NULL;
}

void _destroy_system_device(void)
{
    delete _global_device;
    _global_device = NULL;
}

bool is_valid_system_device(void)
{
    return _global_device ? true : false;
}

device* get_system_device(void)
{
    return _global_device;
}

}
