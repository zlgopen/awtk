/**
 * File:   mmap.h
 * Author: AWTK Develop Team
 * Brief:  mmap
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-12-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MMAP_H
#define TK_MMAP_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class mmap_t
 * 把文件内容映射到内存。
 *
 * writable 为 TRUE 且 shared 为 TRUE：写入对其它共享映射可见，mmap_destroy 时刷回文件。
 * writable 为 TRUE 且 shared 为 FALSE：写时复制，只影响当前映射，不回写文件。
 * writable 为 FALSE：只读。shared 只决定是否与其它映射共享物理页。
 *
 * 空文件，以及大小超过 UINT32_MAX 的文件，mmap_create 返回 NULL。
 * Windows 上 filename 必须是合法 UTF-8，非法编码返回 NULL。
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
  bool_t writable;
  bool_t shared;
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
 * @param {mmap_t*} map mmap对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功。
 * 刷盘、解除映射或关闭文件失败时返回 RET_FAIL，对象仍会被释放。
 */
ret_t mmap_destroy(mmap_t* map);

END_C_DECLS

#endif /*TK_MMAP_H*/
