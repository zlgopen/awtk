/**
 * File:   zip_file.h
 * Author: AWTK Develop Team
 * Brief:  zip file
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-12-19 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef ZIP_FILE_H
#define ZIP_FILE_H

#include "tkc/fs.h"
#include "tkc/buffer.h"

BEGIN_C_DECLS

/**
 * @class zip_file_t
 * zip文件。
*/

/**
 * @method zip_file_extract
 * 解压zip文件。
 *
 * @param {const char*} zipfile zip文件名。
 * @param {const char*} dst_dir 目录。
 * @param {const char*} password 密码(可选)
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t zip_file_extract(const char* zipfile, const char* dst_dir, const char* password);

END_C_DECLS

#endif /*ZIP_FILE_H*/
