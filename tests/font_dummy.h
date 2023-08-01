#ifndef TK_FONT_DUMMY_H
#define TK_FONT_DUMMY_H

#include "base/font.h"
#include "base/font_loader.h"

BEGIN_C_DECLS

ret_t font_dummy_init();
font_t* font_dummy_0(const char* name, uint16_t size);
font_t* font_dummy_1(const char* name, uint16_t size);
font_t* font_dummy_2(const char* name, uint16_t size);
font_t* font_dummy_default(const char* name, uint16_t size);

font_loader_t* default_font_loader(void);

END_C_DECLS

#endif /*TK_FONT_DUMMY_H*/
