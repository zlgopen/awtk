/**
 * File:   feature_info_list.c
 * Author: AWTK Develop Team
 * Brief:  特征信息列表
 *
 * Copyright (c) 2025 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-08-08 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "tkc/feature_info_list.h"

#include "tkc/mem.h"

inline static int feature_info_list_item_compare_by_info(
    const feature_info_list_item_t* iter, const feature_info_list_feature_info_t* ctx) {
  return_value_if_fail(iter != NULL, 1);
  return_value_if_fail(ctx != NULL, -1);
  return iter->info - ctx;
}

inline static int feature_info_list_item_compare(const feature_info_list_item_t* iter,
                                                 const feature_info_list_item_t* ctx) {
  return_value_if_fail(ctx != NULL, -1);
  return feature_info_list_item_compare_by_info(iter, ctx->info);
}

inline static ret_t feature_info_list_item_destroy(feature_info_list_item_t* item) {
  return_value_if_fail(item != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(item);
  return RET_OK;
}

inline static feature_info_list_item_t* feature_info_list_item_create(
    const feature_info_list_feature_info_t* info) {
  feature_info_list_item_t* ret = TKMEM_ZALLOC(feature_info_list_item_t);
  return_value_if_fail(ret != NULL, NULL);

  ret->info = info;

  return ret;
}

feature_info_list_t* feature_info_list_create(void) {
  feature_info_list_t* ret = TKMEM_ZALLOC(feature_info_list_t);
  return_value_if_fail(ret != NULL, NULL);

  goto_error_if_fail(NULL != slist_init(&(ret->base), (tk_destroy_t)feature_info_list_item_destroy,
                                        (tk_compare_t)feature_info_list_item_compare));

  return ret;
error:
  feature_info_list_destroy(ret);
  return NULL;
}

typedef struct _feature_info_list_update_on_visit_ctx_t {
  feature_info_list_t* list;
  uint32_t base_offset;
} feature_info_list_update_on_visit_ctx_t;

inline static ret_t feature_info_list_update_on_visit(void* ctx, const void* data) {
  feature_info_list_item_t* iter = (feature_info_list_item_t*)(data);
  feature_info_list_update_on_visit_ctx_t* actx = (feature_info_list_update_on_visit_ctx_t*)(ctx);
  return_value_if_fail(actx != NULL && iter != NULL, RET_BAD_PARAMS);
  return_value_if_fail(actx->list != NULL, RET_BAD_PARAMS);

  iter->offset = actx->base_offset + actx->list->features_size;
  actx->list->features_size += iter->info->size;

  return RET_OK;
}

inline static ret_t feature_info_list_update(feature_info_list_t* list, uint32_t base_offset) {
  feature_info_list_update_on_visit_ctx_t ctx = {.list = list, .base_offset = base_offset};
  return_value_if_fail(list != NULL, RET_BAD_PARAMS);

  list->features_size = 0;
  return slist_foreach(&(list->base), feature_info_list_update_on_visit, &ctx);
}

feature_info_list_item_t* feature_info_list_find(feature_info_list_t* list,
                                                 const feature_info_list_feature_info_t* info) {
  return_value_if_fail(list != NULL && info != NULL, NULL);

  return (feature_info_list_item_t*)slist_find_ex(
      &(list->base), (tk_compare_t)feature_info_list_item_compare_by_info, (void*)info);
}

ret_t feature_info_list_append(feature_info_list_t* list,
                               const feature_info_list_feature_info_t* info) {
  ret_t ret = RET_OK;
  feature_info_list_item_t* item = NULL;
  return_value_if_fail(list != NULL && info != NULL, RET_BAD_PARAMS);
  return_value_if_fail(NULL == list->owner, RET_FAIL);

  return_value_if_fail(NULL == feature_info_list_find(list, info), RET_FOUND);

  item = feature_info_list_item_create(info);
  return_value_if_fail(item != NULL, RET_OOM);

  ret = slist_append(&(list->base), item);
  goto_error_if_fail(RET_OK == ret);

  return ret;
error:
  feature_info_list_item_destroy(item);
  return ret;
}

ret_t feature_info_list_remove(feature_info_list_t* list,
                               const feature_info_list_feature_info_t* info) {
  return_value_if_fail(list != NULL && info != NULL, RET_BAD_PARAMS);
  return_value_if_fail(NULL == list->owner, RET_FAIL);

  return slist_remove_ex(&(list->base), (tk_compare_t)feature_info_list_item_compare_by_info,
                         (void*)info, 1);
}

ret_t feature_info_list_clear(feature_info_list_t* list) {
  return_value_if_fail(list != NULL, RET_BAD_PARAMS);
  return_value_if_fail(NULL == list->owner, RET_FAIL);

  return slist_remove_all(&(list->base));
}

ret_t feature_info_list_destroy(feature_info_list_t* list) {
  return_value_if_fail(list != NULL, RET_BAD_PARAMS);
  return_value_if_fail(NULL == list->owner, RET_FAIL);

  slist_deinit(&(list->base));
  TKMEM_FREE(list);

  return RET_OK;
}

ret_t feature_info_list_attach(feature_info_list_t* list, void* owner, uint32_t attach_obj_size) {
  return_value_if_fail(list != NULL && owner != NULL, RET_BAD_PARAMS);
  return_value_if_fail(NULL == list->owner, RET_FAIL);

  list->owner = owner;

  feature_info_list_update(list, attach_obj_size);

  return RET_OK;
}

ret_t feature_info_list_detach(feature_info_list_t* list) {
  return_value_if_fail(list != NULL, RET_BAD_PARAMS);
  ENSURE(list->owner != NULL);

  list->owner = NULL;

  return RET_OK;
}

static ret_t feature_info_list_init_features_on_visit(void* ctx, const void* data) {
  const feature_info_list_item_t* item = (const feature_info_list_item_t*)(data);
  return_value_if_fail(ctx != NULL && item != NULL, RET_BAD_PARAMS);

  if (item->info->init != NULL) {
    item->info->init(FEATURE_INFO_LIST_ITEM_GET_FEATURE(item, ctx));
  }

  return RET_OK;
}

ret_t feature_info_list_init_features(feature_info_list_t* list, void* p) {
  return_value_if_fail(list != NULL && p != NULL, RET_BAD_PARAMS);

  return slist_foreach(&(list->base), feature_info_list_init_features_on_visit, p);
}

static ret_t feature_info_list_deinit_features_on_visit(void* ctx, const void* data) {
  const feature_info_list_item_t* item = (const feature_info_list_item_t*)(data);
  return_value_if_fail(ctx != NULL && item != NULL, RET_BAD_PARAMS);

  if (item->info->deinit != NULL) {
    item->info->deinit(FEATURE_INFO_LIST_ITEM_GET_FEATURE(item, ctx));
  }

  return RET_OK;
}

ret_t feature_info_list_deinit_features(feature_info_list_t* list, void* p) {
  return_value_if_fail(list != NULL && p != NULL, RET_BAD_PARAMS);

  return slist_foreach(&(list->base), feature_info_list_deinit_features_on_visit, p);
}
