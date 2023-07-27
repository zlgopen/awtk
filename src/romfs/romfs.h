/**
 * File:   romfs.c
 * Author: AWTK Develop Team
 * Brief:  romfs
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-07-27 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#ifndef TK_ROMFS_H
#define TK_ROMFS_H

#include "tkc/value.h"
#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class romfs_t
 * 一个简单的ROMFS(目前用于WEB加载配置文件)
 */

/**
 * @method romfs_init
 * 初始化。
 * @param {const uint8_t*} header_data 文件信息数据。
 * @param {uint32_t} header_size 文件信息数据长度。
 * @param {const uint8_t*} body_data 文件数据。
 * @param {uint32_t} body_size 文件数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t romfs_init(const uint8_t* header_data, uint32_t header_size, const uint8_t* body_data, uint32_t body_size);

/**
 * @method romfs_deinit
 * ~初始化。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t romfs_deinit(void);

/**
 * @method romfs_get
 * 获取fs对象。
 *
 * @return {fs_t*} 返回fs对象。
 */
fs_t* romfs_get(void);

END_C_DECLS

#endif /*TK_ROMFS_H*/

