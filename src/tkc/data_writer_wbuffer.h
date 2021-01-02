/**
 * File:   data_writer_wbuffer.h
 * Author: AWTK Develop Team
 * Brief:  data_writer wbuffer
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * @class data_writer_wbuffer_t
 * @parent data_writer_t
 * @annotation ["fake"]
 * 基于内存实现的 data_writer。通过 data_writer_factory 创建 writer 时，URL的格式如下(请用函数data_writer_wbuffer_build_url生成)：
 * 
 * ```
 * wbuffer://addr
 * ```
 */

/**
 * @method data_writer_wbuffer_create
 * 创建基于wbuffer的data writer。
 * 
 * > 不要直接调用，而是注册到 data\_writer\_factory后，通过data\_writer\_factory调用
 * @annotation ["constructor"]
 * @param {const char*} wbuffer_name wbuffer地址的字符串格式表示 。
 *
 * @return {data_writer_t*} 返回data writer对象。
 */
data_writer_t* data_writer_wbuffer_create(const char* wbuffer_name);

/**
 * @method data_writer_wbuffer_build_url
 * 构造wbuffer的URL。
 * @annotation ["static"]
 * 
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {char*} url 生成的URL。
 *
 * @return {const char*} 返回URL。
 */
const char* data_writer_wbuffer_build_url(wbuffer_t* buffer, char url[MAX_PATH + 1]);

#define DATA_WRITER_WBUFFER(writer) ((data_writer_t*)(writer))

END_C_DECLS

#endif /*TK_DATA_WRITER_WBUFFER_H*/
