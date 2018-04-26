/* Picasso - a vector graphics library
 *
 * Copyright (C) 2012 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _PICASSO_PRIVATE_H_
#define _PICASSO_PRIVATE_H_

#include "common.h"
#include "convert.h"

namespace picasso {

class graphic_path;

// Font
bool _init_default_font(void);
void _destory_default_font(void);
ps_font* _default_font(void);

// Path
void _path_operation(conv_clipper::clip_op op, const graphic_path& a, const graphic_path& b, graphic_path& r);

// Format
int _byte_pre_color(ps_color_format fmt);
}

// Font Load
bool platform_font_init(void);

void platform_font_shutdown(void);

#endif/*_PICASSO_PRIVATE_H_*/
