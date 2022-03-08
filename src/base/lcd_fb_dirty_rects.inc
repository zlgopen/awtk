#include "base/lcd_fb_dirty_rects.h"

typedef struct _lcd_fb_info_t {
  uint8_t* fb;
  dirty_rects_t dirty_rects;
} lcd_fb_info_t;

const dirty_rects_t* lcd_fb_dirty_rects_get_dirty_rects_by_fb(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, uint8_t* fb) {
  lcd_fb_info_t* fb_info = NULL;
  return_value_if_fail(lcd_fb_dirty_rects != NULL && fb != NULL, NULL);
  fb_info = (lcd_fb_info_t*)darray_find(&(lcd_fb_dirty_rects->fb_dirty_list), fb);
  if (fb_info != NULL) {
    return (const dirty_rects_t*)&(fb_info->dirty_rects);
  }
  return NULL;
}

ret_t lcd_fb_dirty_rects_reset_dirty_rects_by_fb(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, uint8_t* fb) {
  lcd_fb_info_t* fb_info = NULL;
  return_value_if_fail(lcd_fb_dirty_rects != NULL && fb != NULL, RET_BAD_PARAMS);
  fb_info = (lcd_fb_info_t*)darray_find(&(lcd_fb_dirty_rects->fb_dirty_list), fb);
  if (fb_info != NULL) {
    return dirty_rects_reset(&(fb_info->dirty_rects));
  }
  return RET_NOT_FOUND;
}

ret_t lcd_fb_dirty_rects_update_all_fb_dirty_rect(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, const rect_t* dirty_rect) {
  uint32_t i;
  lcd_fb_info_t* fb_info = NULL;
  return_value_if_fail(lcd_fb_dirty_rects != NULL, RET_BAD_PARAMS);
  for (i = 0; i < lcd_fb_dirty_rects->fb_dirty_list.size; i++) {
    fb_info = (lcd_fb_info_t*)darray_get(&(lcd_fb_dirty_rects->fb_dirty_list), i);
    if (fb_info != NULL) {
      if (dirty_rect == NULL) {
        rect_t r = rect_init(0, 0, lcd_fb_dirty_rects->lcd_w,  lcd_fb_dirty_rects->lcd_h);
        dirty_rects_add(&(fb_info->dirty_rects), (const rect_t*)&r);
      } else {
        dirty_rects_add(&(fb_info->dirty_rects), dirty_rect);
      }
    }
  }
  return RET_OK;
}

ret_t lcd_fb_dirty_rects_update_all_fb_dirty_rects(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, const dirty_rects_t* dirty_rects) {
  uint32_t i, j;
  lcd_fb_info_t* fb_info = NULL;
  return_value_if_fail(lcd_fb_dirty_rects != NULL, RET_BAD_PARAMS);
  for (i = 0; i < lcd_fb_dirty_rects->fb_dirty_list.size; i++) {
    fb_info = (lcd_fb_info_t*)darray_get(&(lcd_fb_dirty_rects->fb_dirty_list), i);
    if (fb_info != NULL) {
      if (dirty_rects == NULL) {
        rect_t r = rect_init(0, 0, lcd_fb_dirty_rects->lcd_w,  lcd_fb_dirty_rects->lcd_h);
        dirty_rects_add(&(fb_info->dirty_rects), (const rect_t*)&r);
      } else {
        if (dirty_rects->disable_multiple) {
          dirty_rects_add(&(fb_info->dirty_rects), &(dirty_rects->max));
        } else {
          for (j = 0; j < dirty_rects->nr; j++) {
            const rect_t* iter = dirty_rects->rects + j;
            dirty_rects_add(&(fb_info->dirty_rects), iter);
          }
        }
      }
    }
  }
  return RET_OK;
}

ret_t lcd_fb_dirty_rects_add_fb_info(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, uint8_t* fb) {
  lcd_fb_info_t* fb_info = NULL;
  return_value_if_fail(lcd_fb_dirty_rects != NULL && fb != NULL, RET_BAD_PARAMS);
  fb_info = darray_find(&(lcd_fb_dirty_rects->fb_dirty_list), fb);
  if (fb_info == NULL) {
    rect_t r = rect_init(0, 0, lcd_fb_dirty_rects->lcd_w,  lcd_fb_dirty_rects->lcd_h);
    fb_info = TKMEM_ZALLOC(lcd_fb_info_t);
    return_value_if_fail(fb_info != NULL, RET_OOM);
    fb_info->fb = fb;
    dirty_rects_init(&(fb_info->dirty_rects));
    dirty_rects_add(&(fb_info->dirty_rects), &r);
    darray_push(&(lcd_fb_dirty_rects->fb_dirty_list), fb_info);
  }
  return RET_OK;
}

static int lcd_fb_dirty_rects_fb_dirty_list_cmp_by_fb(const void* a, const void* b) {
  lcd_fb_info_t* fb_info1 = (lcd_fb_info_t*)a;
  return tk_pointer_to_int(fb_info1->fb) - tk_pointer_to_int(b);
}

static ret_t lcd_fb_dirty_rects_fb_dirty_list_destroy(void* data) {
  lcd_fb_info_t* fb_info = (lcd_fb_info_t*)data;
  dirty_rects_deinit(&(fb_info->dirty_rects));
  TKMEM_FREE(data);
  return RET_OK;
}

ret_t lcd_fb_dirty_rects_reinit(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, wh_t lcd_w, wh_t lcd_h) {
  int32_t i;
  return_value_if_fail(lcd_fb_dirty_rects != NULL, RET_BAD_PARAMS);
  lcd_fb_dirty_rects->lcd_w = lcd_w;
  lcd_fb_dirty_rects->lcd_h = lcd_h;
  for (i = 0; i < lcd_fb_dirty_rects->fb_dirty_list.size; i++) {
    lcd_fb_info_t* fb_info = (lcd_fb_info_t*)darray_get(&(lcd_fb_dirty_rects->fb_dirty_list), i);
    if (fb_info != NULL) {
      rect_t r = rect_init(0, 0, lcd_fb_dirty_rects->lcd_w,  lcd_fb_dirty_rects->lcd_h);
      dirty_rects_reset(&(fb_info->dirty_rects));
      dirty_rects_add(&(fb_info->dirty_rects), &r);
    }
  }
  return RET_OK;
}

ret_t lcd_fb_dirty_rects_init(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, wh_t lcd_w, wh_t lcd_h) {
  darray_t* tmp = NULL;
  return_value_if_fail(lcd_fb_dirty_rects != NULL, RET_BAD_PARAMS);
  lcd_fb_dirty_rects->lcd_w = lcd_w;
  lcd_fb_dirty_rects->lcd_h = lcd_h;
  tmp = darray_init(&(lcd_fb_dirty_rects->fb_dirty_list), 4, lcd_fb_dirty_rects_fb_dirty_list_destroy, lcd_fb_dirty_rects_fb_dirty_list_cmp_by_fb);
  return_value_if_fail(tmp != NULL, RET_FAIL);
  return RET_OK;
}

ret_t lcd_fb_dirty_rects_deinit(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects) {
  return_value_if_fail(lcd_fb_dirty_rects != NULL, RET_BAD_PARAMS);
  return darray_deinit(&(lcd_fb_dirty_rects->fb_dirty_list));
}
