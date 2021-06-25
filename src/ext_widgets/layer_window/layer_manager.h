/**
 * File:   layer_manager.h
 * Author: AWTK Develop Team
 * Brief:  LCD layer manager。
 *
 * Copyright (c) Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#ifndef TK_LAYER_MANAGER_H
#define TK_LAYER_MANAGER_H

#include "tkc/object.h"
#include "layer_window/layer.h"

BEGIN_C_DECLS

#ifndef TK_MAX_LAYERS_NR
#define TK_MAX_LAYERS_NR 5
#endif/*TK_MAX_LAYERS_NR*/

/**
 * @class layer_manager_t 
 * 
 * LCD layer manager。
 */
typedef struct _layer_manager_t {
  /**
   * @property {uint32_t} nr
   * @annotation ["readable"]
   * 层的个数
   */
  uint32_t nr;

  /*private*/
  layer_t* layers[TK_MAX_LAYERS_NR];
} layer_manager_t;

/**
 * @method layer_manager_create
 * 创建对象。
 * @return {layer_manager_t*} 返回非NULL表示成功，否则表示失败。
 */
layer_manager_t* layer_manager_create(void);

/**
 * @method layer_manager_add
 * 增加层。
 * @param {layer_manager_t*} layer_manager layer_manager对象。
 * @param {layer_t*} layer layer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t layer_manager_add(layer_manager_t* layer_manager, layer_t* layer);

/**
 * @method layer_manager_find
 * 查找层。
 * @param {layer_manager_t*} layer_manager layer_manager对象。
 * @param {const char*} name 名称。
 *
 * @return {layer_manager_t*} 返回非NULL表示成功，否则表示失败。
 */
layer_t* layer_manager_find(layer_manager_t* layer_manager, const char* name);

/**
 * @method layer_manager_destroy
 * 销毁layer manager对象。
 * @param {layer_manager_t*} layer_manager layer_manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t layer_manager_destroy(layer_manager_t* layer_manager);

/*单例*/
/**
 * @method layer_manager
 * 获取全局的layer manager对象。
 * @return {layer_manager_t*} 返回非NULL表示成功，否则表示失败。
 */
layer_manager_t* layer_manager(void);

/**
 * @method layer_manager_set
 * 设置全局的layer manager对象。
 * @param {layer_manager_t*} layer_manager layer_manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t layer_manager_set(layer_manager_t* layer_manager);

#define LAYER_MANAGER(obj) layer_manager_cast(OBJECT(obj))

END_C_DECLS

#endif/*TK_LAYER_MANAGER_H*/
