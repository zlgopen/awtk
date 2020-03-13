/**
 * File:   dl.h
 * Author: AWTK Develop Team
 * Brief:  dynamic libaray api
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-03-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DL_H
#define TK_DL_H

#include "tkc/types_def.h"

/**
 * @class tk_dl_t
 * 动态链接库对象。
 */
struct _tk_dl_t;
typedef struct _tk_dl_t tk_dl_t;

BEGIN_C_DECLS

/**
 * @method tk_dl_open
 * 打开动态链接库。
 *
 * @param {const char*}  filename 文件名。
 *
 * @return {tk_dl_t*} 动态链接库对象。
 */
tk_dl_t* tk_dl_open(const char* filename);

/**
 * @method tk_dl_sym
 * 获取指定名称函数的地址。
 * @param {tk_dl_t*}    dl dl对象。
 * @param {const char*}  name 函数名。
 *
 * @return {void*} 返回函数的地址。
 */
void* tk_dl_sym(tk_dl_t* dl, const char* name);

/**
 * @method tk_dl_close
 * 关闭dl对象。
 * @param {tk_dl_t*}    dl dl对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_dl_close(tk_dl_t* dl);

END_C_DECLS

#endif /*TK_DL_H*/
