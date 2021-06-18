/**
 * File:   vgcanvas_asset_manager.c
 * Author: AWTK Develop Team
 * Brief:  vgcanvas asset manger.
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-04-16 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#include "base/vgcanvas.h"
#include "base/system_info.h"
#include "tkc/color_parser.h"
#include "tkc/mem.h"
#include "tkc/darray.h"
#include "tkc/utils.h"
#include "base/image_manager.h"
#include "base/vgcanvas_asset_manager.h"

#ifdef WITH_VGCANVAS

#define VGCANVAS_ASSET_MANAGER_DARRAY_INIT_NUMBER 20

typedef struct _vgcanvas_asset_ctx_t {
  void* ctx;
  uint32_t vg_id;
  vgcanvas_asset_manager_t* vgcanvas_asset_manager;
} vgcanvas_asset_ctx_t;

typedef struct _vgcanvas_asset_image_ctx_t {
  vgcanvas_asset_ctx_t base;
  vgcanvas_asset_manager_bitmap_destroy_t bitmap_destroy;
} vgcanvas_asset_image_ctx_t;

typedef struct _vgcanvas_asset_font_ctx_t {
  vgcanvas_asset_ctx_t base;
  char* font_name;
  vgcanvas_asset_manager_font_destroy_t font_destroy;
} vgcanvas_asset_font_ctx_t;

typedef struct _vgcanvas_asset_font_t {
  char* font_name;
  darray_t font_ctx_list;
} vgcanvas_asset_font_t;

typedef struct _vgcanvas_asset_image_t {
  char* image_name;
  darray_t image_ctx_list;
  vgcanvas_asset_manager_t* vgcanvas_asset_manager;
} vgcanvas_asset_image_t;

typedef struct _vgcanvas_asset_vg_t {
  void* vg;
  uint32_t vg_id;
  vgcanvas_asset_manager_font_destroy_t font_destroy;
  vgcanvas_asset_manager_bitmap_destroy_t bitmap_destroy;
} vgcanvas_asset_vg_t;


static vgcanvas_asset_manager_t* s_vgcanvas_asset_manager;

static int vgcanvas_asset_manager_vg_list_compare_by_id(const void* a, const void* b);
static void* vgcanvas_asset_manager_darray_find_index(darray_t* darray, tk_compare_t compare, void* data);

static int vgcanvas_asset_manager_ctx_list_compare(const void* a, const void* b) {
  uint32_t vg_id = tk_pointer_to_int(b);
  vgcanvas_asset_ctx_t* ctx = (vgcanvas_asset_ctx_t*)a;
  if (ctx->vg_id == vg_id) {
    return 0;
  }
  return 1;
}

static ret_t vgcanvas_asset_manager_font_list_destroy(void* data) {
  vgcanvas_asset_font_t* font = (vgcanvas_asset_font_t*)data;
  darray_deinit(&(font->font_ctx_list));
  if (font->font_name != NULL) {
    TKMEM_FREE(font->font_name);
  }
  TKMEM_FREE(font);
  return RET_OK;
}

static int vgcanvas_asset_manager_font_list_compare(const void* a, const void* b) {
  const char* font_name = (const char*)b;
  vgcanvas_asset_font_t* font = (vgcanvas_asset_font_t*)a;
  if (tk_str_eq(font->font_name, font_name)) {
    return 0;
  }
  return 1;
}

static ret_t vgcanvas_asset_manager_font_ctx_list_destroy(void* data) {
  vgcanvas_asset_font_ctx_t* font_ctx = (vgcanvas_asset_font_ctx_t*)data;
  vgcanvas_asset_manager_t* vgcanvas_asset_manager = font_ctx->base.vgcanvas_asset_manager;
  vgcanvas_asset_vg_t* vg_data = (vgcanvas_asset_vg_t*)vgcanvas_asset_manager_darray_find_index(&(vgcanvas_asset_manager->vg_list), vgcanvas_asset_manager_vg_list_compare_by_id, tk_pointer_from_int(font_ctx->base.vg_id));
  if (vg_data != NULL) {
    font_ctx->font_destroy(vg_data->vg, font_ctx->font_name, font_ctx->base.ctx);
  } else {
    font_ctx->font_destroy(NULL, font_ctx->font_name, font_ctx->base.ctx);
  }
  TKMEM_FREE(font_ctx);
  return 1;
}

static ret_t vgcanvas_asset_manager_image_list_destroy(void* data) {
  vgcanvas_asset_image_t* image = (vgcanvas_asset_image_t*)data;
  darray_deinit(&(image->image_ctx_list));  
  if (image->image_name != NULL) {
    TKMEM_FREE(image->image_name)
  }
  TKMEM_FREE(image);
  return RET_OK;
}

static int vgcanvas_asset_manager_image_list_compare(const void* a, const void* b) {
  bitmap_t* img = (bitmap_t*)b;
  vgcanvas_asset_image_t* image = (vgcanvas_asset_image_t*)a;
  if (tk_str_eq(image->image_name, img->name)) {
    return 0;
  }
  return 1;
}

static ret_t vgcanvas_asset_manager_image_ctx_list_destroy(void* data) {
  vgcanvas_asset_image_ctx_t* ctx = (vgcanvas_asset_image_ctx_t*)data;
  vgcanvas_asset_manager_t* vgcanvas_asset_manager = ctx->base.vgcanvas_asset_manager;
  vgcanvas_asset_vg_t* vg_data = (vgcanvas_asset_vg_t*)vgcanvas_asset_manager_darray_find_index(&(vgcanvas_asset_manager->vg_list), vgcanvas_asset_manager_vg_list_compare_by_id, tk_pointer_from_int(ctx->base.vg_id));
  if (vg_data != NULL) {
    ctx->bitmap_destroy(vg_data->vg, ctx->base.ctx);
  } else {
    ctx->bitmap_destroy(NULL, ctx->base.ctx);
  }
  TKMEM_FREE(ctx);
  return 1;
}

static int vgcanvas_asset_manager_vg_list_compare_by_id(const void* a, const void* b) {
  vgcanvas_asset_vg_t* vgcanvas_asset_vg = (vgcanvas_asset_vg_t*)a;
  if (vgcanvas_asset_vg->vg_id == tk_pointer_to_int(b)) {
    return 0;
  }
  return 1;
}

static int vgcanvas_asset_manager_vg_list_compare(const void* a, const void* b) {
  vgcanvas_asset_vg_t* vgcanvas_asset_vg = (vgcanvas_asset_vg_t*)a;
  if (vgcanvas_asset_vg->vg == (vgcanvas_t*)b) {
    return 0;
  }
  return 1;
}

static void* vgcanvas_asset_manager_darray_find_index(darray_t* darray, tk_compare_t compare, void* data) {
  int32_t i = 0;
  int32_t size = 0;
  void** elms = NULL;
  return_value_if_fail(darray != NULL, NULL);

  elms = darray->elms;
  size = darray->size;
  for (i = 0; i < size; i++) {
    void* iter = elms[i];
    if (compare(iter, data) == 0) {
      return iter;
    }
  }

  return NULL;
}

static ret_t vgcanvas_asset_manager_on_bitmap_destroy(bitmap_t* img) {
  vgcanvas_asset_image_t* image = (vgcanvas_asset_image_t*)img->specific;
  vgcanvas_asset_manager_t* vgcanvas_asset_manager = image->vgcanvas_asset_manager;
  darray_remove(&(vgcanvas_asset_manager->image_list), image);
  return RET_OK;
}



vgcanvas_asset_manager_t* vgcanvas_asset_manager(void) {
  return s_vgcanvas_asset_manager;
}

ret_t vgcanvas_asset_manager_set(vgcanvas_asset_manager_t* vgcanvas_asset_manager) {
  s_vgcanvas_asset_manager = vgcanvas_asset_manager;
  return RET_OK;
}

vgcanvas_asset_manager_t* vgcanvas_asset_manager_create(void) {
  vgcanvas_asset_manager_t* vgcanvas_asset_manager = TKMEM_ZALLOC(vgcanvas_asset_manager_t);
  darray_init(&(vgcanvas_asset_manager->vg_list), VGCANVAS_ASSET_MANAGER_DARRAY_INIT_NUMBER, default_destroy, NULL);
  darray_init(&(vgcanvas_asset_manager->font_list), VGCANVAS_ASSET_MANAGER_DARRAY_INIT_NUMBER, vgcanvas_asset_manager_font_list_destroy, NULL);
  darray_init(&(vgcanvas_asset_manager->image_list), VGCANVAS_ASSET_MANAGER_DARRAY_INIT_NUMBER, vgcanvas_asset_manager_image_list_destroy, NULL);
  return vgcanvas_asset_manager;
}

ret_t vgcanvas_asset_manager_destroy(vgcanvas_asset_manager_t* vgcanvas_asset_manager) {
  return_value_if_fail(vgcanvas_asset_manager != NULL, RET_BAD_PARAMS);
  darray_deinit(&(vgcanvas_asset_manager->vg_list));
  darray_deinit(&(vgcanvas_asset_manager->font_list));
  darray_deinit(&(vgcanvas_asset_manager->image_list));
  TKMEM_FREE(vgcanvas_asset_manager);
  return RET_OK;
}

const void* vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, bitmap_t* img, ret_t* reuslt) {
  int32_t i = 0;
  int32_t size = 0;
  darray_t* darray = NULL;
  vgcanvas_asset_vg_t* vg_data = NULL;
  *reuslt = RET_FAIL;
  return_value_if_fail(vgcanvas_asset_manager != NULL, NULL);
  darray = &(vgcanvas_asset_manager->image_list);
  size = darray->size;
  
  vg_data = (vgcanvas_asset_vg_t*)vgcanvas_asset_manager_darray_find_index(&(vgcanvas_asset_manager->vg_list), vgcanvas_asset_manager_vg_list_compare, vg);
  if (vg_data != NULL) {
    for (i = 0; i < size; i++) {
      vgcanvas_asset_image_t* image = (vgcanvas_asset_image_t*)darray_get(darray, i);
      if (tk_str_eq(image->image_name, img->name)) {
        int32_t image_ctx_id = darray_find_index(&(image->image_ctx_list), tk_pointer_from_int(vg_data->vg_id));
        if (image_ctx_id >= 0) {
          vgcanvas_asset_ctx_t* image_ctx = (vgcanvas_asset_ctx_t*)darray_get(&(image->image_ctx_list), image_ctx_id);
          *reuslt = RET_OK;
          return (const void*)(image_ctx->ctx);
        }
        break;
      }
    }
  }
  return NULL;
}

ret_t vgcanvas_asset_manager_add_image(vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, bitmap_t* img, void* specific) {
  vgcanvas_asset_image_ctx_t* ctx = NULL;
  vgcanvas_asset_vg_t* vg_data = NULL;
  vgcanvas_asset_image_t* image = NULL;
  return_value_if_fail(vgcanvas_asset_manager != NULL && vg != NULL && img != NULL, RET_BAD_PARAMS);
  
  vg_data = (vgcanvas_asset_vg_t*)vgcanvas_asset_manager_darray_find_index(&(vgcanvas_asset_manager->vg_list), vgcanvas_asset_manager_vg_list_compare, vg);
  return_value_if_fail(vg_data != NULL, RET_BAD_PARAMS);

  ctx = TKMEM_ZALLOC(vgcanvas_asset_image_ctx_t);
  return_value_if_fail(ctx != NULL, RET_OOM);
  ctx->base.ctx = specific;
  ctx->base.vg_id = vg_data->vg_id;
  ctx->base.vgcanvas_asset_manager = vgcanvas_asset_manager;
  ctx->bitmap_destroy = vg_data->bitmap_destroy;

  image = (vgcanvas_asset_image_t*)vgcanvas_asset_manager_darray_find_index(&(vgcanvas_asset_manager->image_list), vgcanvas_asset_manager_image_list_compare, img);
  if (image == NULL) {
    image = TKMEM_ZALLOC(vgcanvas_asset_image_t);
    goto_error_if_fail(image != NULL);
    
    image->image_name = tk_str_copy(NULL, img->name);
    image->vgcanvas_asset_manager = vgcanvas_asset_manager;
    darray_init(&(image->image_ctx_list), VGCANVAS_ASSET_MANAGER_DARRAY_INIT_NUMBER, vgcanvas_asset_manager_image_ctx_list_destroy, vgcanvas_asset_manager_ctx_list_compare);

    img->specific = image;
    img->specific_destroy = vgcanvas_asset_manager_on_bitmap_destroy;
    image_manager_update_specific(img->image_manager, img);
    darray_push(&(vgcanvas_asset_manager->image_list), image);
  }
  darray_push(&(image->image_ctx_list), ctx);
  return RET_OK;
error :
  if (ctx != NULL) {
    TKMEM_FREE(ctx);
  }
  if (image != NULL) {
    TKMEM_FREE(image);
  }
  return RET_OOM;
}

const void* vgcanvas_asset_manager_get_font_ctx(vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, const char* font_name, ret_t* reuslt) {
  int32_t i = 0;
  int32_t size = 0;
  darray_t* darray = NULL;
  vgcanvas_asset_vg_t* vg_data = NULL;
  *reuslt = RET_FAIL;
  return_value_if_fail(vgcanvas_asset_manager != NULL, NULL);
  darray = &(vgcanvas_asset_manager->font_list);
  size = darray->size;
  
  vg_data = (vgcanvas_asset_vg_t*)vgcanvas_asset_manager_darray_find_index(&(vgcanvas_asset_manager->vg_list), vgcanvas_asset_manager_vg_list_compare, vg);
  if (vg_data != NULL) {
    for (i = 0; i < size; i++) {
      vgcanvas_asset_font_t* font = (vgcanvas_asset_font_t*)darray_get(darray, i);
      if (tk_str_eq(font->font_name, font_name)) {
        int32_t ctx_id = darray_find_index(&(font->font_ctx_list), tk_pointer_from_int(vg_data->vg_id));
        if (ctx_id >= 0) {
          vgcanvas_asset_ctx_t* ctx = (vgcanvas_asset_ctx_t*)darray_get(&(font->font_ctx_list), ctx_id);
          *reuslt = RET_OK;
          return (const void*)(ctx->ctx);
        }
        break;
      }
    }
  }
  return NULL;
}

ret_t vgcanvas_asset_manager_add_font(vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, const char* font_name, void* ctx) {
  vgcanvas_asset_font_t* font = NULL;
  vgcanvas_asset_vg_t* vg_data = NULL;
  vgcanvas_asset_font_ctx_t* font_ctx = NULL;
  return_value_if_fail(vgcanvas_asset_manager != NULL && vg != NULL && font_name != NULL, RET_BAD_PARAMS);

  vg_data = (vgcanvas_asset_vg_t*)vgcanvas_asset_manager_darray_find_index(&(vgcanvas_asset_manager->vg_list), vgcanvas_asset_manager_vg_list_compare, vg);
  return_value_if_fail(vg_data != NULL, RET_BAD_PARAMS);

  font_ctx = TKMEM_ZALLOC(vgcanvas_asset_font_ctx_t);
  return_value_if_fail(font_ctx != NULL, RET_OOM);
  font_ctx->base.ctx = ctx;
  font_ctx->base.vg_id = vg_data->vg_id;
  font_ctx->base.vgcanvas_asset_manager = vgcanvas_asset_manager;
  font_ctx->font_destroy = vg_data->font_destroy;

  font = (vgcanvas_asset_font_t*)vgcanvas_asset_manager_darray_find_index(&(vgcanvas_asset_manager->font_list), vgcanvas_asset_manager_font_list_compare, (void*)font_name);
  if (font == NULL) {
    font = TKMEM_ZALLOC(vgcanvas_asset_font_t);
    goto_error_if_fail(font != NULL);
    font->font_name = tk_str_copy(NULL, font_name);
    goto_error_if_fail(font->font_name != NULL);
    darray_init(&(font->font_ctx_list), VGCANVAS_ASSET_MANAGER_DARRAY_INIT_NUMBER, vgcanvas_asset_manager_font_ctx_list_destroy, vgcanvas_asset_manager_ctx_list_compare);
    darray_push(&(vgcanvas_asset_manager->font_list), font);
  }
  font_ctx->font_name = font->font_name;
  darray_push(&(font->font_ctx_list), font_ctx);
  return RET_OK;
error :
  if (font_ctx != NULL) {
    TKMEM_FREE(font_ctx);
  }
  if (font != NULL) {
    TKMEM_FREE(font);
  }
  return RET_OOM;
}

ret_t vgcanvas_asset_manager_remove_font(vgcanvas_asset_manager_t* vgcanvas_asset_manager, const char* font_name) {
  return_value_if_fail(vgcanvas_asset_manager != NULL, RET_BAD_PARAMS);

  if (font_name == NULL) {
    darray_clear(&(vgcanvas_asset_manager->font_list));
  } else {
    vgcanvas_asset_font_t* font = (vgcanvas_asset_font_t*)vgcanvas_asset_manager_darray_find_index(&(vgcanvas_asset_manager->font_list), vgcanvas_asset_manager_font_list_compare, (void*)font_name);
    if (font != NULL) {
      darray_remove(&(vgcanvas_asset_manager->font_list), font);
    }
  }
  return RET_OK;
}

ret_t vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, vgcanvas_asset_manager_bitmap_destroy_t bitmap_destroy, vgcanvas_asset_manager_font_destroy_t font_destroy) {
  vgcanvas_asset_vg_t* vg_data = NULL;
  return_value_if_fail(vgcanvas_asset_manager != NULL && vg != NULL, RET_BAD_PARAMS);
  vg_data = (vgcanvas_asset_vg_t*)vgcanvas_asset_manager_darray_find_index(&(vgcanvas_asset_manager->vg_list), vgcanvas_asset_manager_vg_list_compare, vg);
  if (vg_data == NULL) {
    vg_data = TKMEM_ZALLOC(vgcanvas_asset_vg_t);
    return_value_if_fail(vg_data != NULL, RET_OOM);
    vg_data->vg = vg;
    vg_data->vg_id = vgcanvas_asset_manager->vg_number++;
    darray_push(&(vgcanvas_asset_manager->vg_list), vg_data);
  }
  vg_data->font_destroy = font_destroy;
  vg_data->bitmap_destroy = bitmap_destroy;

  return RET_OK;
}

ret_t vgcanvas_asset_manager_remove_vg(vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg) {
  vgcanvas_asset_vg_t* vg_data = NULL;
  return_value_if_fail(vgcanvas_asset_manager != NULL && vg != NULL, RET_BAD_PARAMS);
  vg_data = (vgcanvas_asset_vg_t*)vgcanvas_asset_manager_darray_find_index(&(vgcanvas_asset_manager->vg_list), vgcanvas_asset_manager_vg_list_compare, vg);
  return darray_remove(&(vgcanvas_asset_manager->vg_list), vg_data);
}
#endif
