/**
 * File:   image_manager.h
 * Author: AWTK Develop Team
 * Brief:  image manager
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IMAGE_MANAGER_H
#define TK_IMAGE_MANAGER_H

#include "tkc/darray.h"
#include "base/image_loader.h"
#include "base/assets_manager.h"

BEGIN_C_DECLS

/**
 * 但没有文件系统时，图片被转成位图，直接编译到程序中。bitmap_header_t用来描述该位图的信息。
 */
typedef struct _bitmap_header_t {
  uint16_t w;
  uint16_t h;
  uint16_t flags;  /*bitmap_flag_t*/
  uint16_t format; /*bitmap_format_t*/
  uint8_t data[4];
} bitmap_header_t;

/**
 * @class image_manager_t
 * @annotation ["scriptable"]
 * 图片管理器。负责加载，解码和缓存图片。
 */
struct _image_manager_t {
  /**
   * @property {darray_t} images
   * @annotation ["private"]
   * 缓存的图片。
   */
  darray_t images;

  /**
   * @property {assets_manager_t*} assets_manager
   * @annotation ["private"]
   * 资源管理器。
   */
  assets_manager_t* assets_manager;
};

/**
 * @method image_manager
 * 获取缺省的图片管理器。
 * @alias image_manager_instance
 * @annotation ["constructor", "scriptable"]
 * @return {image_manager_t*} 返回图片管理器对象。
 */
image_manager_t* image_manager(void);

/**
 * @method image_manager_set
 * 设置缺省的图片管理器。
 * @param {image_manager_t*} imm 图片管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_manager_set(image_manager_t* imm);

/**
 * @method image_manager_create
 * 创建图片管理器。
 * @annotation ["constructor"]
 *
 * @return {image_manager_t*} 返回图片管理器对象。
 */
image_manager_t* image_manager_create(void);

/**
 * @method image_manager_init
 * 初始化图片管理器。
 * @annotation ["constructor"]
 * @param {image_manager_t*} imm 图片管理器对象。
 *
 * @return {image_manager_t*} 返回图片管理器对象。
 */
image_manager_t* image_manager_init(image_manager_t* imm);

/**
 * @method image_manager_get_bitmap
 * 获取指定的图片。
 * 先从缓存查找，如果没找到，再加载并缓存。
 *
 * @annotation ["scriptable"]
 * @param {image_manager_t*} imm 图片管理器对象。
 * @param {char*} name 图片名称。
 * @param {bitmap_t*} image 用于返回图片。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_manager_get_bitmap(image_manager_t* imm, const char* name, bitmap_t* image);

/**
 * @method image_manager_preload
 * 预加载指定的图片。
 * @annotation ["scriptable"]
 * @param {image_manager_t*} imm 图片管理器对象。
 * @param {char*} name 图片名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_manager_preload(image_manager_t* imm, const char* name);

/**
 * @method image_manager_unload_unused
 * 从图片管理器中卸载指定时间内没有使用的图片。
 * @param {image_manager_t*} imm 图片管理器对象。
 * @param {uint32_t} time_delta_s 时间范围，单位为秒。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_manager_unload_unused(image_manager_t* imm, uint32_t time_delta_s);

/**
 * @method image_manager_unload_all
 * 从图片管理器中卸载全部图片。
 *
 * @param {image_manager_t*} imm 图片管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_manager_unload_all(image_manager_t* imm);

/**
 * @method image_manager_unload_bitmap
 * 从图片管理器中卸载指定的图片。
 * @param {image_manager_t*} imm 图片管理器对象。
 * @param {bitmap_t*} image 图片。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_manager_unload_bitmap(image_manager_t* imm, bitmap_t* image);

/**
 * @method image_manager_update_specific
 * 更新缓存中图片的specific信息。
 * @param {image_manager_t*} imm 图片管理器对象。
 * @param {bitmap_t*} image 返回图片信息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_manager_update_specific(image_manager_t* imm, bitmap_t* image);

/**
 * @method image_manager_set_assets_manager
 * 设置资源管理器对象。
 *
 * 之所以需要设置资源管理器对象，而不是使用缺省的资源管理器对象，是因为在designer中有两个图片管理器：
 *
 * * 一个用于designer本身加载图片。
 *
 * * 一个用于被设计的窗口加载图片。
 *
 *这两个图片管理器需要从不同的路径加载资源。
 *
 * @param {image_manager_t*} imm 图片管理器对象。
 * @param {assets_manager_t*} assets_manager 资源管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_manager_set_assets_manager(image_manager_t* imm, assets_manager_t* assets_manager);

/**
 * @method image_manager_deinit
 * 析构图片管理器。
 * @param {image_manager_t*} imm 图片管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_manager_deinit(image_manager_t* im);

/**
 * @method image_manager_destroy
 * 析构并释放图片管理器。
 * @param {image_manager_t*} imm 图片管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_manager_destroy(image_manager_t* im);

/*public for test*/
ret_t image_manager_add(image_manager_t* imm, const char* name, const bitmap_t* image);
ret_t image_manager_lookup(image_manager_t* imm, const char* name, bitmap_t* image);

END_C_DECLS

#endif /*TK_IMAGE_MANAGER_H*/
