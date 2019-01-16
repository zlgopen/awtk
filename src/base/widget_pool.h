/**
 * File:   widget_pool.h
 * Author: AWTK Develop Team
 * Brief:  widget pool
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_POOL_H
#define TK_WIDGET_POOL_H

#include "tkc/object_default.h"
#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @class widget_pool_t
 *
 * 有时需要频繁创建和销毁控件，widget pool把销毁的控件缓存起来，再次使用时直接拿回来用。
 *
 * * 优点：速度快，内存碎片少。
 * * 缺点：占用一点内存空间。
 *
 * 通过宏WITH\_WIDGET\_POOL来指定最大缓存控件的数量。
 *
 * 控件可以通过vt->enable\_pool参数决定该类控件是否启用缓存。
 *
 */
typedef struct _widget_pool_t {
  object_t* cache;
  uint32_t cache_nr;
  uint32_t max_cache_nr;
} widget_pool_t;

/**
 * @method widget_pool
 * 获取缺省widget pool对象。
 *
 * @annotation ["constructor"]
 * @return {widget_pool_t*} 返回widget pool对象。
 */
widget_pool_t* widget_pool(void);

/**
 * @method widget_pool_set
 * 设置缺省widget pool对象。
 *
 * @param {widget_pool_t*} pool widget pool对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_pool_set(widget_pool_t* pool);

/**
 * @method widget_pool_create
 * 创建widget pool对象。
 *
 * @annotation ["constructor"]
 * @param {uint32_t} max_cache_nr 最大缓存对象个数。
 *
 * @return {widget_pool_t*} 返回widget pool对象。
 */
widget_pool_t* widget_pool_create(uint32_t max_cache_nr);

/**
 * @method widget_pool_create_widget
 * 创建widget对象。
 *
 * @param {widget_pool_t*} pool widget pool对象。
 * @param {const widget_vtable_t*} vt 控件虚表。
 *
 * @return {widget_t} 返回控件对象。
 */
widget_t* widget_pool_create_widget(widget_pool_t* pool, const widget_vtable_t* vt);

/**
 * @method widget_pool_destroy_widget
 * 销毁或回收widget对象。
 *
 * @param {widget_pool_t*} pool widget pool对象。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_pool_destroy_widget(widget_pool_t* pool, widget_t* widget);

/**
 * @method widget_pool_clear_cache
 * 清除全部缓存控件。
 * @param {widget_pool_t*} pool widget pool对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_pool_clear_cache(widget_pool_t* pool);

/**
 * @method widget_pool_count
 * 计算缓存控件个数。
 * @param {widget_pool_t*} pool widget pool对象。
 *
 * @return {int32_t} 返回缓存控件个数。
 */
int32_t widget_pool_count(widget_pool_t* pool);

/**
 * @method widget_pool_destroy
 * 清除全部缓存控件并销毁widget pool对象。

 * @param {widget_pool_t*} pool widget pool对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_pool_destroy(widget_pool_t* pool);

END_C_DECLS

#endif /*TK_WIDGET_POOL_H*/
