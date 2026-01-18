/**
 * File:   data_reader_http.h
 * Author: AWTK Develop Team
 * Brief:  data_reader http
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License http for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2023-12-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DATA_READER_HTTP_H
#define TK_DATA_READER_HTTP_H

#include "tkc/data_reader.h"

BEGIN_C_DECLS

/* 最大HTTP响应大小限制：100MB */
#define HTTP_MAX_CONTENT_LENGTH (100 * 1024 * 1024)

/**
 * @class data_reader_http_t
 * @parent data_reader_t
 * @annotation ["fake"]
 * 基于HTTP/HTTPS实现的 data_reader。通过 data_reader_factory 创建 reader 时，URL的格式如下(请用函数data_reader_http_build_url生成)：
 * 
 * ```
 * http://t.weather.sojson.com/api/weather/city/101030100
 * https://restapi.amap.com/v3/weather/weatherInfo?city=110101&key=%3C%E7%94%A8%E6%88%B7key%3E
 * ```
 */

/**
 * @method data_reader_http_create
 * 创建基于文件的data reader。
 * > 不要直接调用，而是注册到 data\_reader\_factory后，通过data\_reader\_factory调用。
 * @annotation ["constructor"]
 * 
 * @param {const char*} url URL。
 *
 * @return {data_reader_t*} 返回data reader对象。
 */
data_reader_t* data_reader_http_create(const char* url);

END_C_DECLS

#endif /*TK_DATA_READER_HTTP_H*/
