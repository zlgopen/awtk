/**
 * File:   feature_info_list.h
 * Author: AWTK Develop Team
 * Brief:  特征信息列表
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

#ifndef TK_FEATURE_INFO_LIST_H
#define TK_FEATURE_INFO_LIST_H

#include "tkc/types_def.h"
#include "tkc/slist.h"

BEGIN_C_DECLS

typedef ret_t (*feature_info_list_feature_init_t)(void* feature);
typedef ret_t (*feature_info_list_feature_deinit_t)(void* feature);

/**
 * @struct feature_info_list_feature_info_t
 * 特征信息
 */
typedef struct _feature_info_list_feature_info_t {
  uint32_t size;
  feature_info_list_feature_init_t init;
  feature_info_list_feature_deinit_t deinit;
} feature_info_list_feature_info_t;

/**
 * @struct feature_info_list_item_t
 * 特征信息列表项
 */
typedef struct _feature_info_list_item_t {
  const feature_info_list_feature_info_t* info;
  uint32_t offset;
} feature_info_list_item_t;

/**
 * @class feature_info_list_t
 * @parent slist_t
 * 特征信息列表
 */
typedef struct _feature_info_list_t {
  slist_t base;
  uint32_t features_size;
  void* owner;
} feature_info_list_t;

/**
 * @method feature_info_list_create
 * @annotation ["constructor"]
 * 创建特征信息列表
 * @return {feature_info_list_t*} 返回特征信息列表对象
 */
feature_info_list_t* feature_info_list_create(void);

/**
 * @method feature_info_list_find
 * 查找特征信息
 * @param {tree_node_feature_info_list_t*} list 特征信息列表对象
 * @param {const feature_info_list_feature_info_t*} info 特征信息
 *
 * @return {tree_node_feature_info_list_item_t*} 返回特征信息列表项对象
 */
feature_info_list_item_t* feature_info_list_find(feature_info_list_t* list,
                                                 const feature_info_list_feature_info_t* info);

/**
 * @method feature_info_list_append
 * 追加特征信息
 * @param {feature_info_list_t*} list 特征信息列表对象
 * @param {const feature_info_list_feature_info_t*} info 特征信息
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t feature_info_list_append(feature_info_list_t* list,
                               const feature_info_list_feature_info_t* info);
/**
 * @method feature_info_list_remove
 * 移除特征信息
 * @param {feature_info_list_t*} list 特征信息列表对象
 * @param {const feature_info_list_feature_info_t*} info 特征信息
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t feature_info_list_remove(feature_info_list_t* list,
                               const feature_info_list_feature_info_t* info);

/**
 * @method feature_info_list_clear
 * 清空特征信息列表
 * @param {feature_info_list_t*} list 特征信息列表对象
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t feature_info_list_clear(feature_info_list_t* list);

/**
 * @method feature_info_list_destroy
 * @annotation ["deconstructor"]
 * 销毁特征信息列表
 * @param {feature_info_list_t*} list 特征信息列表对象
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t feature_info_list_destroy(feature_info_list_t* list);

/**
 * @method feature_info_list_attach
 * 绑定特征信息列表
 * > 需要增加特征的类内部使用，外部不应该调用此接口。
 * @param {feature_info_list_t*} list 特征信息列表对象
 * @param {void*} owner 拥有者
 * @param {uint32_t} attach_obj_size 附加对象的大小
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t feature_info_list_attach(feature_info_list_t* list, void* owner, uint32_t attach_obj_size);

/**
 * @method feature_info_list_detach
 * 解绑特征信息列表
 * > 需要增加特征的类内部使用，外部不应该调用此接口。
 * @param {feature_info_list_t*} list 特征信息列表对象
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t feature_info_list_detach(feature_info_list_t* list);

/**
 * @method feature_info_list_init_features
 * 初始化对象的特征
 * > 需要增加特征的类内部使用，外部不应该调用此接口。
 * @param {feature_info_list_t*} list 特征信息列表对象
 * @param {void*} p 对象指针
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t feature_info_list_init_features(feature_info_list_t* list, void* p);

/**
 * @method feature_info_list_deinit_features
 * 反初始化对象的特征
 * > 需要增加特征的类内部使用，外部不应该调用此接口。
 * @param {feature_info_list_t*} list 特征信息列表对象
 * @param {void*} p 对象指针
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t feature_info_list_deinit_features(feature_info_list_t* list, void* p);

#define FEATURE_INFO_LIST_ITEM_GET_FEATURE(item, p) ((uint8_t*)(p) + (item)->offset)

END_C_DECLS

#endif /*TK_FEATURE_INFO_LIST_H*/
