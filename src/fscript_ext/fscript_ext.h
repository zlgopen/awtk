/**
 * File:   fscript_ext.h
 * Author: AWTK Develop Team
 * Brief:  ext functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-03 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_FSCRIPT_EXT_H
#define TK_FSCRIPT_EXT_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_ext_init
 * 注册扩展函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_ext_init(void);

#if defined(LINUX) || defined(WINDOWS) || defined(MACOS) || defined(ANDROID) || defined(IOS)
#define WITH_FSCRIPT_EXT 1

#define FSCRIPT_WITH_CRC 1
#define FSCRIPT_WITH_MATH 1
#define FSCRIPT_WITH_ENDIAN 1
#define FSCRIPT_WITH_BUFFER 1
#define FSCRIPT_WITH_ARRAY 1
#define FSCRIPT_WITH_TYPED_ARRAY 1
#define FSCRIPT_WITH_STREAM 1
#define FSCRIPT_WITH_BITS 1
#define FSCRIPT_WITH_FS 1
#define FSCRIPT_WITH_JSON 1
#define FSCRIPT_WITH_APP_CONF 1
#define FSCRIPT_WITH_STREAM_FILE 1
#define FSCRIPT_WITH_STREAM_INET 1
#define FSCRIPT_WITH_STREAM_SERIAL 1
#define FSCRIPT_WITH_DATE_TIME 1

#if !defined(TKC_ONLY) || defined(WITH_AWTK)
#define FSCRIPT_WITH_WIDGET 1
#endif /*TKC_ONLY*/

#endif /*PC*/

END_C_DECLS

#endif /*TK_FSCRIPT_EXT_H*/
