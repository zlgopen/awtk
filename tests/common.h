#include "base/rect.h"
#include "base/bitmap.h"

#ifndef COMMON_H
#define COMMON_H

void bitmap_dump(bitmap_t* b);
void bitmap_check(bitmap_t* b, rect_t* r, rgba_t e);

#endif /*COMMON_H*/
