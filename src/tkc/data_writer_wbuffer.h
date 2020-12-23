/**
 * File:   data_writer_wbuffer.h
 * Author: AWTK Develop Team
 * Brief:  data_writer wbuffer
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License wbuffer for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-12-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DATA_WRITER_WBUFFER_H
#define TK_DATA_WRITER_WBUFFER_H

#include "tkc/utils.h"
#include "tkc/buffer.h"
#include "tkc/data_writer.h"

BEGIN_C_DECLS

/**
 * @method data_writer_wbuffer_create
 * 创建基于wbuffer的data writer。
 * @param {const char*} wbuffer_name pointer to wbuffer 。
 *
 * @return {data_writer_t*} 返回data writer对象。
 */
data_writer_t* data_writer_wbuffer_create(const char* wbuffer_name);

/**
 * @method data_writer_wbuffer_build_url
 *
 * @param {wbuffer_t*} wbuffer pointer to wbuffer 。
 * @param {char*} url url.
 *
 * @return {const char*} 返回URL。
 */
const char* data_writer_wbuffer_build_url(wbuffer_t* buffer, char url[TK_NAME_LEN+1]);

#define DATA_WRITER_WBUFFER(writer) ((data_writer_t*)(writer))

END_C_DECLS

#endif /*TK_DATA_WRITER_WBUFFER_H*/
