#include "tkc/rect.h"
#include "base/bitmap.h"
#include "base/events.h"
#include "base/widget.h"

#ifndef COMMON_H
#define COMMON_H

void bitmap_dump(bitmap_t* b);
void bitmap_check(bitmap_t* b, rect_t* r, rgba_t e);

ret_t widget_log_events(void* ctx, event_t* e);
void assert_str_eq(const wchar_t* wstr, const char* utf8);

#endif /*COMMON_H*/
