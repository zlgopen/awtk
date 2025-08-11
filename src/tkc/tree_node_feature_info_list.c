/**
 * File:   tree_node_feature_info_list.c
 * Author: AWTK Develop Team
 * Brief:  树结点特征信息列表
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/tree_node_feature_info_list.h"
#include "tkc/mem.h"

inline static int tree_node_feature_info_list_item_compare_by_info(
    const tree_node_feature_info_list_item_t* iter, const tree_node_feature_info_t* ctx) {
  return_value_if_fail(iter != NULL, 1);
  return_value_if_fail(ctx != NULL, -1);
  return iter->info - ctx;
}

inline static int tree_node_feature_info_list_item_compare(
    const tree_node_feature_info_list_item_t* iter, const tree_node_feature_info_list_item_t* ctx) {
  return_value_if_fail(ctx != NULL, -1);
  return tree_node_feature_info_list_item_compare_by_info(iter, ctx->info);
}

inline static ret_t tree_node_feature_info_list_item_destroy(
    tree_node_feature_info_list_item_t* item) {
  return_value_if_fail(item != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(item);
  return RET_OK;
}

inline static tree_node_feature_info_list_item_t* tree_node_feature_info_list_item_create(
    const tree_node_feature_info_t* info) {
  tree_node_feature_info_list_item_t* ret = TKMEM_ZALLOC(tree_node_feature_info_list_item_t);
  return_value_if_fail(ret != NULL, NULL);

  ret->info = info;

  return ret;
}

tree_node_feature_info_list_t* tree_node_feature_info_list_create(void) {
  tree_node_feature_info_list_t* ret = TKMEM_ZALLOC(tree_node_feature_info_list_t);
  return_value_if_fail(ret != NULL, NULL);

  goto_error_if_fail(NULL != slist_init(&(ret->base),
                                        (tk_destroy_t)tree_node_feature_info_list_item_destroy,
                                        (tk_compare_t)tree_node_feature_info_list_item_compare));

  return ret;
error:
  tree_node_feature_info_list_destroy(ret);
  return NULL;
}

inline static ret_t tree_node_feature_info_list_update_on_visit(void* ctx, const void* data) {
  tree_node_feature_info_list_item_t* iter = (tree_node_feature_info_list_item_t*)(data);
  tree_node_feature_info_list_t* list = (tree_node_feature_info_list_t*)(ctx);
  return_value_if_fail(list != NULL && iter != NULL, RET_BAD_PARAMS);

  iter->offset = list->features_size;
  list->features_size += iter->info->size;

  return RET_OK;
}

inline static ret_t tree_node_feature_info_list_update(tree_node_feature_info_list_t* list) {
  return_value_if_fail(list != NULL, RET_BAD_PARAMS);

  list->features_size = 0;
  return slist_foreach(&(list->base), tree_node_feature_info_list_update_on_visit, list);
}

tree_node_feature_info_list_item_t* tree_node_feature_info_list_find(
    tree_node_feature_info_list_t* list, const tree_node_feature_info_t* info) {
  return_value_if_fail(list != NULL && info != NULL, NULL);

  return (tree_node_feature_info_list_item_t*)slist_find_ex(
      &(list->base), (tk_compare_t)tree_node_feature_info_list_item_compare_by_info, (void*)info);
}

ret_t tree_node_feature_info_list_append(tree_node_feature_info_list_t* list,
                                         const tree_node_feature_info_t* info) {
  ret_t ret = RET_OK;
  tree_node_feature_info_list_item_t* item = NULL;
  return_value_if_fail(list != NULL && info != NULL, RET_BAD_PARAMS);
  return_value_if_fail(NULL == list->owner, RET_FAIL);

  return_value_if_fail(NULL == tree_node_feature_info_list_find(list, info), RET_FOUND);

  item = tree_node_feature_info_list_item_create(info);
  return_value_if_fail(item != NULL, RET_OOM);

  ret = slist_append(&(list->base), item);
  goto_error_if_fail(RET_OK == ret);

  return ret;
error:
  tree_node_feature_info_list_item_destroy(item);
  return ret;
}

ret_t tree_node_feature_info_list_remove(tree_node_feature_info_list_t* list,
                                         const tree_node_feature_info_t* info) {
  return_value_if_fail(list != NULL && info != NULL, RET_BAD_PARAMS);
  return_value_if_fail(NULL == list->owner, RET_FAIL);

  return slist_remove_ex(&(list->base),
                         (tk_compare_t)tree_node_feature_info_list_item_compare_by_info,
                         (void*)info, 1);
}

ret_t tree_node_feature_info_list_clear(tree_node_feature_info_list_t* list) {
  return_value_if_fail(list != NULL, RET_BAD_PARAMS);
  return_value_if_fail(NULL == list->owner, RET_FAIL);

  return slist_remove_all(&(list->base));
}

ret_t tree_node_feature_info_list_destroy(tree_node_feature_info_list_t* list) {
  return_value_if_fail(list != NULL, RET_BAD_PARAMS);
  return_value_if_fail(NULL == list->owner, RET_FAIL);

  slist_deinit(&(list->base));
  TKMEM_FREE(list);

  return RET_OK;
}

ret_t tree_node_feature_info_list_attach(tree_node_feature_info_list_t* list, void* owner) {
  return_value_if_fail(list != NULL && owner != NULL, RET_BAD_PARAMS);
  return_value_if_fail(NULL == list->owner, RET_FAIL);

  list->owner = owner;

  tree_node_feature_info_list_update(list);

  return RET_OK;
}

ret_t tree_node_feature_info_list_detach(tree_node_feature_info_list_t* list) {
  return_value_if_fail(list != NULL, RET_BAD_PARAMS);
  ENSURE(list->owner != NULL);

  list->owner = NULL;

  return RET_OK;
}
