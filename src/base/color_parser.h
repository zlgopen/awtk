#include "base/color.h"
#include "base/types_def.h"

#ifndef COLOR_PARSER_H
#define COLOR_PARSER_H

BEGIN_C_DECLS

bool_t color_parse(const char* color, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a);
color_t color_parse_simple(const char* color);

END_C_DECLS

#endif /*COLOR_PARSER_H*/
