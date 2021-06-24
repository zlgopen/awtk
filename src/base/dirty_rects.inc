#include "base/widget_vtable.h"
#include "base/dirty_rects.h"

static inline ret_t dirty_rects_dump(dirty_rects_t* dirty_rects) {
  uint32_t i = 0;
  rect_t* iter = NULL;
  return_value_if_fail(dirty_rects != NULL, RET_BAD_PARAMS);

  iter = &(dirty_rects->max);
  log_debug("max:");
  log_debug("(%d %d %d %d)", iter->x, iter->y, iter->w, iter->h);
  log_debug("\n");

  log_debug("===========================================================\n");
  for (i = 0; i < dirty_rects->nr; i++) {
    rect_t* iter = dirty_rects->rects + i;
    log_debug("(%d %d %d %d)", iter->x, iter->y, iter->w, iter->h);
  }
  log_debug("\n");
  log_debug("===========================================================\n\n");

  return RET_OK;
}

static inline ret_t dirty_rects_paint(dirty_rects_t* dirty_rects, widget_t* widget, canvas_t* c,
                                      widget_on_paint_t on_paint) {
  uint32_t cost = 0;
  rect_t* iter = NULL;
  uint64_t start = time_now_us();
  return_value_if_fail(dirty_rects != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget != NULL && c != NULL && on_paint != NULL, RET_BAD_PARAMS);

  if (dirty_rects->disable_multiple) {
    iter = &(dirty_rects->max);
    widget_paint_with_clip(widget, iter, c, on_paint);
    if (dirty_rects->profile) {
      cost = time_now_us() - start;
      log_debug("paint max rect(%d %d %d %d) cost=%u\n", iter->x, iter->y, iter->w, iter->h, cost);
    }

    if (dirty_rects->debug) {
      canvas_set_stroke_color_str(c, "red");
      canvas_stroke_rect(c, iter->x + 1, iter->y + 1, iter->w - 2, iter->h - 2);
    }
  } else {
    uint32_t i = 0;

    if (dirty_rects->debug) {
      dirty_rects_dump(dirty_rects);
    }

    for (i = 0; i < dirty_rects->nr; i++) {
      rect_t* iter = dirty_rects->rects + i;
      uint64_t start1 = time_now_us();
      widget_paint_with_clip(widget, iter, c, on_paint);

      if (dirty_rects->debug) {
        canvas_set_stroke_color_str(c, "red");
        canvas_stroke_rect(c, iter->x + 1, iter->y + 1, iter->w - 2, iter->h - 2);
      }

      if (dirty_rects->profile) {
        cost = time_now_us() - start1;
        log_debug("paint %u rect(%d %d %d %d) cost=%u\n", i, iter->x, iter->y, iter->w, iter->h,
                  cost);
      }
    }

    if (dirty_rects->profile) {
      iter = &(dirty_rects->max);
      cost = time_now_us() - start;
      log_debug("paint total rect(%d %d %d %d) cost=%u\n", iter->x, iter->y, iter->w, iter->h,
                cost);
    }
  }

  if (dirty_rects->profile) {
    log_debug("\n\n");
  }

  return RET_OK;
}
