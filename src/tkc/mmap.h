/**
 * File:   mmap.h
 * Author: AWTK Develop Team
 * Brief:  mmap
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is dimmapibuted in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-12-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MMAP_H
#define TK_MMAP_H

#include "tkc/str.h"

BEGIN_C_DECLS

/**
 * @class mmap_t
 * 把文件内容映射到内存。
 * 
 */
typedef struct _mmap_t {
  /**
   * @property {void*} data
   * @annotation ["readable"]
   * 内存地址。
   */
  void* data;

  /**
   * @property {uint32_t} size
   * @annotation ["readable"]
   * 数据长度。
   */
  uint32_t size;
  /*private*/
  void* handle;
  void* fd;
} mmap_t;

/**
 * @method mmap_create
 * 初始化mmap对象。
 * @annotation ["constructor"]
 * @param {const char*} filename 文件名。
 * @param {bool_t} writable 是否可写。
 * @param {bool_t} shared 是否共享。
 *
 * @return {mmap_t*} mmap对象本身。
 */
mmap_t* mmap_create(const char* filename, bool_t writable, bool_t shared);

/**
 * @method mmap_destroy
 * 销毁mmap。
 * @param {mmap_t*} mmap mmap对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mmap_destroy(mmap_t* mmap);

END_C_DECLS

#endif /*TK_MMAP_H*/
