/**
 * File:   vgcanvas_asset_manager.h
 * Author: AWTK Develop Team
 * Brief:  vgcanvas asset manger.
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_VG_CANVAS_ASSET_MANAGER_H
#define TK_VG_CANVAS_ASSET_MANAGER_H

#include "base/vgcanvas.h"
#include "base/system_info.h"
#include "tkc/color_parser.h"
#include "tkc/mem.h"
#include "tkc/darray.h"
#include "tkc/utils.h"

BEGIN_C_DECLS

typedef struct _vgcanvas_asset_manager_t {
  /* private */
  uint32_t vg_number;
  darray_t vg_list;
  darray_t font_list;
  darray_t image_list;
} vgcanvas_asset_manager_t;

typedef ret_t (*vgcanvas_asset_manager_bitmap_destroy_t)(void* vg, void* specific);
typedef ret_t (*vgcanvas_asset_manager_font_destroy_t)(void* vg, const char* font_name,
                                                       void* specific);

/**
 * @method vgcanvas_asset_manager
 * 获取缺省矢量画布资源管理器。
 * @alias vgcanvas_asset_manager_instance
 * @annotation ["constructor", "scriptable"]
 *
 * @return {vgcanvas_asset_manager_t*} 返回矢量画布资源管理器对象。
 */
vgcanvas_asset_manager_t* vgcanvas_asset_manager(void);

/**
 * @method vgcanvas_asset_manager_set
 * 设置缺省矢量画布资源管理器。
 * @param {vgcanvas_asset_manager_t*} vgcanvas_asset_manager 矢量画布资源管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_asset_manager_set(vgcanvas_asset_manager_t* vgcanvas_asset_manager);

/**
 * @method vgcanvas_asset_manager_create
 * 创建矢量画布资源管理器。
 * @annotation ["constructor"]
 *
 * @return {vgcanvas_asset_manager_t*} 返回矢量画布资源管理器对象。
 */
vgcanvas_asset_manager_t* vgcanvas_asset_manager_create(void);

/**
 * @method vgcanvas_asset_manager_destroy
 * 释放建矢量画布资源管理器对象
 *
 * @param {vgcanvas_asset_manager_t*} vgcanvas_asset_manager 矢量画布资源管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_asset_manager_destroy(vgcanvas_asset_manager_t* vgcanvas_asset_manager);

/**
 * @method vgcanvas_asset_manager_get_image_specific
 * 获取贴图的特殊相关上下文
 *
 * @param {vgcanvas_asset_manager_t*} vgcanvas_asset_manager 矢量画布资源管理器。
 * @param {void*} vg 使用该字库的矢量画布对象
 * @param {bitmap_t*} img 贴图对象。
 * @param {ret_t*} reuslt 返回获取结果是否成功，成功返回 RET_OK，否则表示失败。
 *
 * @return {const void*} 返回特殊上下文。
 */
const void* vgcanvas_asset_manager_get_image_specific(
    vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, bitmap_t* img, ret_t* reuslt);

/**
 * @method vgcanvas_asset_manager_add_image
 * 添加一个贴图资源
 *
 * @param {vgcanvas_asset_manager_t*} vgcanvas_asset_manager 矢量画布资源管理器。
 * @param {void*} vg 使用该字库的矢量画布对象
 * @param {bitmap_t*} img 贴图对象。
 * @param {void*} specific 贴图特殊上下文参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_asset_manager_add_image(vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg,
                                       bitmap_t* img, void* specific);

/**
 * @method vgcanvas_asset_manager_remove_image
 * 移除贴图资源
 * 备注：如果 bitmap 对象为 NULL，则移除该 vg 对象下的所有贴图。
 * @param {vgcanvas_asset_manager_t*} vgcanvas_asset_manager 矢量画布资源管理器。
 * @param {void*} vg 使用该字库的矢量画布对象
 * @param {bitmap_t*} img 贴图对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_asset_manager_remove_image(vgcanvas_asset_manager_t* vgcanvas_asset_manager,
                                          void* vg, bitmap_t* img);

/**
 * @method vgcanvas_asset_manager_get_font_ctx
 * 获取字库的相关上下文
 *
 * @param {vgcanvas_asset_manager_t*} vgcanvas_asset_manager 矢量画布资源管理器。
 * @param {void*} vg 使用该字库的矢量画布对象
 * @param {const char*} font_name 字库名称。
 * @param {ret_t*} reuslt 返回获取结果是否成功，成功返回 RET_OK，否则表示失败。
 *
 * @return {const void*} 返回上下文。
 */
const void* vgcanvas_asset_manager_get_font_ctx(vgcanvas_asset_manager_t* vgcanvas_asset_manager,
                                                void* vg, const char* font_name, ret_t* reuslt);

/**
 * @method vgcanvas_asset_manager_add_font
 * 添加一个字库资源
 *
 * @param {vgcanvas_asset_manager_t*} vgcanvas_asset_manager 矢量画布资源管理器。
 * @param {void*} vg 使用该字库的矢量画布对象
 * @param {const char*} font_name 字库名称。
 * @param {void*} ctx 上下文参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_asset_manager_add_font(vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg,
                                      const char* font_name, void* ctx);

/**
 * @method vgcanvas_asset_manager_remove_font
 * 移除字库资源
 * 备注：如果字库名字为 NULL，则移除该 vg 对象下的所有字库。
 * @param {vgcanvas_asset_manager_t*} vgcanvas_asset_manager 矢量画布资源管理器。
 * @param {void*} vg 使用该字库的矢量画布对象
 * @param {const char*} font_name 字库名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_asset_manager_remove_font(vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg,
                                         const char* font_name);

/**
 * @method vgcanvas_asset_manager_add_vg
 * 添加一个矢量画布的对象
 *
 * @param {vgcanvas_asset_manager_t*} vgcanvas_asset_manager 矢量画布资源管理器。
 * @param {void*} vg 矢量画布对象。
 * @param {vgcanvas_asset_manager_bitmap_destroy_t} bitmap_destroy 矢量画布对象释放贴图资源的回调函数。
 * @param {vgcanvas_asset_manager_font_destroy_t} font_destroy 矢量画布对象释放字库资源的回调函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg,
                                    vgcanvas_asset_manager_bitmap_destroy_t bitmap_destroy,
                                    vgcanvas_asset_manager_font_destroy_t font_destroy);

/**
 * @method vgcanvas_asset_manager_remove_vg
 * 移除一个矢量画布的对象
 *
 * @param {vgcanvas_asset_manager_t*} vgcanvas_asset_manager 矢量画布资源管理器。
 * @param {void*} vg 矢量画布对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vgcanvas_asset_manager_remove_vg(vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg);

END_C_DECLS

#endif /*TK_VG_CANVAS_ASSET_MANAGER_H*/
