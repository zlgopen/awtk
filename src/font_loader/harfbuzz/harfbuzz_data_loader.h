/**
 * File:   harfbuzz_data_loader.h
 * Author: AWTK Develop Team
 * Brief:  harfbuzz loader
 *
 * Copyright (c) 2018 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-01-20 Zhao Yingjian <zhaoyingjian@zlg.cn> created
 *
 */

#ifndef HARFBUZZ_DATA_LOADER_H
#define HARFBUZZ_DATA_LOADER_H

#include "hb_info.h"
#include "base/types_def.h"

typedef struct _hb_str_info_t {
  const wchar_t* str;
  hb_info_t* hb_info;
  uint32_t hb_info_size;
  uint32_t str_len;
} hb_str_info_t;

typedef struct _hb_ttf_info_t {
  hb_str_info_t* str_info;
  char* font_name;
  uint32_t str_size;
} hb_ttf_info_t;

typedef struct _harfbuzz_data_loader_t {
  hb_ttf_info_t* ttf_info;
  uint32_t ttf_size;
  uint32_t refcount;
} harfbuzz_data_loader_t;

/**
 * @method harfbuzz_data_loader
 * 获取缺省的harfbuzz管理器。
 * @alias harfbuzz_data_loader_instance
 * @annotation ["constructor", "scriptable"]
 * @return {harfbuzz_data_loader_t*} 返回harfbuzz管理器。
 */
harfbuzz_data_loader_t* harfbuzz_data_loader(void);

/**
 * @method harfbuzz_data_loader_create
 * 创建harfbuzz管理器。
 * @annotation ["constructor"]
 * @param {const char*} name bin文件名称。
 *
 * @return {harfbuzz_data_loader_t*} 返回harfbuzz管理器对象。
 */
harfbuzz_data_loader_t* harfbuzz_data_loader_create(const char* name);

/**
 * @method harfbuzz_data_loader_set
 * 设置缺省的harfbuzz管理器。
 * @param {harfbuzz_data_loader_t*} loader harfbuzz管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t harfbuzz_data_loader_set(harfbuzz_data_loader_t* loader);

/**
 * @method harfbuzz_data_loader_find_info
 * 匹配字符串信息。
 * @param {harfbuzz_data_loader_t*} loader harfbuzz管理器对象。
 * @param {const char*} font_name 字体名称。
 * @param {const wchar_t*} str 需要匹配的完整字符串。
 * @param {uint32_t} str_len 字符串长度。
 *
 * @return {hb_str_info_t*} 成功返回该字符串对应的信息，失败返回NULL。
 */
hb_str_info_t* harfbuzz_data_loader_find_info(harfbuzz_data_loader_t* loader, const char* font_name, const wchar_t* str, uint32_t str_len);

/**
 * @method harfbuzz_data_loader_destroy
 * 销毁harfbuzz管理器。
 * @param {harfbuzz_data_loader_t*} loader harfbuzz管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t harfbuzz_data_loader_destroy(harfbuzz_data_loader_t* loader);

/**
 * @method harfbuzz_data_loader_ref
 * 引用harfbuzz管理器，若引用计数为0则创建。
 * @annotation ["constructor"]
 * @param {const char*} name bin文件名称。
 *
 * @return {harfbuzz_data_loader_t*} 返回harfbuzz管理器对象。
 */
harfbuzz_data_loader_t* harfbuzz_data_loader_ref(harfbuzz_data_loader_t* loader, const char* name);

/**
 * @method harfbuzz_data_loader_unref
 * 取消引用harfbuzz管理器，若引用计数为0则销毁。
 * @param {harfbuzz_data_loader_t*} loader harfbuzz管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t harfbuzz_data_loader_unref(harfbuzz_data_loader_t* loader);

#endif
