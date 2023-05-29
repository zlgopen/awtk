/**
 * File:   file_dialog.h
 * Author: AWTK Develop Team
 * Brief: 文件对话框。
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
 * 2022-05-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#ifndef TK_FILE_DIALOG_H
#define TK_FILE_DIALOG_H

#include "tkc.h"

BEGIN_C_DECLS

/**
 * @class file_dialog_t
 * @annotation ["fake"]
 * 文件对话框。
 */

/**
 * @method tk_choose_files
 * 多选文件，用完后请调用darray_destroy销毁返回的数组。
 * @param {const char*} filters 过滤规则(文件扩展名列表,如:".c.cpp.cxx")。
 * @param {const char*} init_dir 初始目录。
 *
 * @return {darray_t*} 返回NULL表示失败，返回非NULL为文件名列表。
 */
darray_t* tk_choose_files(const char* filters, const char* init_dir);

/**
 * @method tk_choose_file
 * 选择单个文件，用完后请调用TKMEM_FREE销毁返回的字符串。
 * @param {const char*} filters 过滤规则(文件扩展名列表,如:".c.cpp.cxx")。
 * @param {const char*} init_dir 初始目录。
 *
 * @return {char*} 返回NULL表示失败，返回非NULL为文件名。
 */
char* tk_choose_file(const char* filters, const char* init_dir);

/**
 * @method tk_choose_folder
 * 选择单个目录，用完后请调用TKMEM_FREE销毁返回的字符串。
 * @param {const char*} init_dir 初始目录。
 *
 * @return {char*} 返回NULL表示失败，返回非NULL为文件名。
 */
char* tk_choose_folder(const char* init_dir);

/**
 * @method tk_choose_file_for_save
 * 选择单个文件用于保存，用完后请调用TKMEM_FREE销毁返回的字符串。
 * @param {const char*} filters 过滤规则(文件扩展名列表,如:".c.cpp.cxx")。
 * @param {const char*} init_dir 初始目录。
 *
 * @return {char*} 返回NULL表示失败，返回非NULL为文件名。
 */
char* tk_choose_file_for_save(const char* filters, const char* init_dir);

END_C_DECLS

#endif /*TK_FILE_DIALOG_H*/
