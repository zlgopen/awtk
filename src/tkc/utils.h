/**
 * File:   utils.h
 * Author: AWTK Develop Team
 * Brief:  utils struct and utils functions.
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UTILS_H
#define TK_UTILS_H

#include "tkc/str.h"
#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class utils_t
 * @annotation ["fake"]
 * 工具类。
 *
 */

/**
 * @method tk_atoi
 *
 * 将字符串转换为整形。
 *
 * @param {const char*} str 要转换为整形的字符串。
 *
 * @return {int} 返回转换后的整形。
 */
int tk_atoi(const char* str);

/**
 * @method tk_atob
 *
 * 将字符串转换为布尔类型。
 *
 * @param {const char*} str 要转换为布尔类型的字符串。
 *
 * @return {bool_t} 返回转换后的布尔类型。
 */
bool_t tk_atob(const char* str);

/**
 * @method tk_atof
 *
 * 将字符串转换为浮点类型。
 *
 * @param {const char*} str 要转换为浮点类型的字符串。
 *
 * @return {double} 返回转换后的浮点类型。
 */
double tk_atof(const char* str);

/**
 * @method tk_str_cmp
 *
 * 字符串比较函数。
 *
 * @param {const char*} a 要进行比较的第一个字符串。
 * @param {const char*} b 要进行比较的第二个字符串。
 *
 * @return {int32_t} 如果返回值=-1，则表示a为NULL；如果返回值=1，则表示b为NULL；如果返回值<0，则表示a小于b；如果返回值>0，则表示a大于b；如果返回值=0，则表示a等于b。
 */
int32_t tk_str_cmp(const char* a, const char* b);

/**
 * @method tk_str_icmp
 *
 * 字符串比较函数（不区分大小写）。
 *
 * @param {const char*} a 要进行比较的第一个字符串。
 * @param {const char*} b 要进行比较的第二个字符串。
 *
 * @return {int32_t} 如果返回值=-1，则表示a为NULL；如果返回值=1，则表示b为NULL；如果返回值<0，则表示a小于b；如果返回值>0，则表示a大于b；如果返回值=0，则表示a等于b。
 */
int32_t tk_str_icmp(const char* a, const char* b);

/**
 * @method tk_watoi
 *
 * 将宽字符串转换为整形。
 *
 * @param {const wchar_t*} str 要转换为整形的宽字符串。
 *
 * @return {int} 返回转换后的整形。
 */
int tk_watoi(const wchar_t* str);

/**
 * @method tk_watoi_n
 *
 * 将宽字符串转换为整形。
 *
 * @param {const wchar_t*} str 要转换为整形的宽字符串。
 * @param {uint32_t} len 字符串长度。
 *
 * @return {int} 返回转换后的整形。
 */
int tk_watoi_n(const wchar_t* str, uint32_t len);

/**
 * @method tk_watob
 *
 * 将宽字符串转换为布尔类型。
 *
 * @param {const wchar_t*} str 要转换为布尔类型的宽字符串。
 *
 * @return {bool_t} 返回转换后的布尔类型。
 */
bool_t tk_watob(const wchar_t* str);

/**
 * @method tk_watof
 *
 * 将宽字符串转换为浮点类型。
 *
 * @param {const wchar_t*} str 要转换为浮点类型的宽字符串。
 *
 * @return {double} 返回转换后的浮点类型。
 */
double tk_watof(const wchar_t* str);

/**
 * @method tk_skip_to_num
 *
 * 跳过字符串函数，如：字符串"hello123world"，返回的结果是"123world"。
 *
 * @param {const char*} str 要输入的原始字符串。
 *
 * @return {const char*} 返回转换后的字符串。
 */
const char* tk_skip_to_num(const char* str);

/**
 * @method tk_itoa
 *
 * 将整形转换为字符串。
 *
 * @param {char*} str 保存字符串缓冲区。
 * @param {int} len 缓冲区大小。
 * @param {int} n 要转换的整形。
 *
 * @return {const char*} 返回字符串。
 */
const char* tk_itoa(char* str, int len, int n);

/**
 * @method tk_ftoa
 *
 * 将浮点型转换为字符串。
 *
 * @param {char*} str 保存字符串缓冲区。
 * @param {int} len 缓冲区大小。
 * @param {double} f 要转换的浮点型。
 *
 * @return {const char*} 返回字符串。
 */
const char* tk_ftoa(char* str, int len, double f);

/**
 * @method tk_strtol
 *
 * 将字符串转换为长整形。
 *
 * @param {const char*} str 要转换为长整形的字符串。
 * @param {const char**} end 对类型char*的对象的引用。
 * @param {int} base 基数。
 *
 * @return {long} 返回转换后的长整形。
 */
long tk_strtol(const char* str, const char** end, int base);

/**
 * @method tk_strcpy
 *
 * 将src所指向的字符串复制到dst。
 *
 * @param {char*} dst 目标字符串。
 * @param {const char*} src 原字符串。
 *
 * @return {char*} 返回目标字符串。
 */
char* tk_strcpy(char* dst, const char* src);

/**
 * @method tk_strncpy
 *
 * 将src所指向的字符串复制到dst，最多复制len个字符串。
 *
 * @param {char*} dst 目标字符串。
 * @param {const char*} src 原字符串。
 * @param {size_t} len 要复制的字符串个数。
 *
 * @return {char*} 返回目标字符串。
 */
char* tk_strncpy(char* dst, const char* src, size_t len);

/**
 * @method tk_strdup
 *
 * 字符串拷贝函数。
 *
 * @param {const char*} str 原字符串。
 *
 * @return {char*} 返回指向的复制字符串指针，如果失败则返回NULL。
 */
char* tk_strdup(const char* str);

/**
 * @method tk_wstrdup
 *
 * 宽字符串拷贝函数。
 *
 * @param {const wchar_t*} str 原宽字符串。
 *
 * @return {wchar_t*} 返回指向的复制宽字符串指针，如果失败则返回NULL。
 */
wchar_t* tk_wstrdup(const wchar_t* str);

/**
 * @method tk_strndup
 *
 * 字符串拷贝函数，最多复制len个字符串。
 *
 * @param {const char*} str 原字符串。
 * @param {uint32_t} len 要复制的字符串个数。
 *
 * @return {char*} 返回指向的复制字符串指针，如果失败则返回NULL。
 */
char* tk_strndup(const char* str, uint32_t len);

/**
 * @method tk_strlen
 *
 * 获取字符串的长度。str为空时返回0。
 *
 * @param {const char*} str 字符串。
 *
 * @return {uint32_t} 返回字符串的长度。
 */
uint32_t tk_strlen(const char* str);

/**
 * @method tk_str_append
 *
 * 字符串追加函数。
 *
 * @param {char*} str 被追加字符串。
 * @param {uint32_t} max_len 字符串的最大长度。
 * @param {const char*} s 要追加的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_str_append(char* str, uint32_t max_len, const char* s);

/**
 * @method tk_memset16
 *
 * 设置数据2字节。
 *
 * @param {uint16_t*} buff buff
 * @param {uint16_t} val 值。
 * @param {uint32_t} size 个数。
 *
 * @return {uint16_t*} 返回设置好的buff。
 */
uint16_t* tk_memset16(uint16_t* buff, uint16_t val, uint32_t size);

/**
 * @method tk_memset24
 *
 * 设置数据3字节。
 *
 * @param {uint32_t*} buff buff。
 * @param {void*} val 值。
 * @param {uint32_t} size 个数。
 *
 * @return {uint32_t*} 返回设置好的buff。
 */
uint32_t* tk_memset24(uint32_t* buff, void* val, uint32_t size);

/**
 * @method tk_memset32
 *
 * 设置数据4字节。
 *
 * @param {uint32_t*} buff buff。
 * @param {uint32_t} val 值。
 * @param {uint32_t} size 个数。
 *
 * @return {uint32_t*} 返回设置好的buff。
 */
uint32_t* tk_memset32(uint32_t* buff, uint32_t val, uint32_t size);

/**
 * @method tk_memcpy16
 *
 * 拷贝数据2字节。
 *
 * @param {uint16_t*} dst 目标
 * @param {uint16_t*} src 源。
 * @param {uint32_t} size 个数。
 *
 * @return {uint16_t*} 返回设置好的buff。
 */
uint16_t* tk_memcpy16(uint16_t* dst, uint16_t* src, uint32_t size);

/**
 * @method tk_memcpy32
 *
 * 拷贝数据4字节。
 *
 * @param {uint32_t*} dst 目标
 * @param {uint32_t*} src 源。
 * @param {uint32_t} size 个数。
 *
 * @return {uint32_t*} 返回设置好的buff。
 */
uint32_t* tk_memcpy32(uint32_t* dst, uint32_t* src, uint32_t size);

/**
 * @method tk_pixel_copy
 *
 * 已bpp字节为标准拷贝数据。
 *
 * @param {void*} dst 目标
 * @param {void*} src 源。
 * @param {uint32_t} size 个数。
 * @param {uint8_t} bpp 单个数据的字节数。
 *
 * @return {void*} 返回设置好的buff。
 */
void* tk_pixel_copy(void* dst, const void* src, uint32_t size, uint8_t bpp);

/**
 * @method tk_snprintf
 *
 * 将可变参数(...)按照format格式化字符串，并将字符串复制到str中。
 *
 * @param {char*} str 目标字符串。
 * @param {size_t} size 拷贝字节数。
 * @param {const char*} format 格式化字符串。
 *
 * @return {int} 返回格式化后的字符串长度+1。
 */
int tk_snprintf(char* str, size_t size, const char* format, ...);

/**
 * @method tk_sscanf
 *
 * 从字符串读取格式化输入。
 *
 * @param {char*} str 要输入的字符串。
 * @param {const char*} format 格式化字符串。
 *
 * @return {int} 返回成功匹配和赋值的个数。
 */
int tk_sscanf(const char* str, const char* format, ...);

/**
 * @method filename_to_name
 *
 * 从完整文件名中获取文件名。
 *
 * @param {const char*} filename 完整的文件名。
 * @param {char*} str 用于返回文件名。
 * @param {uint32_t} size 文件名(str参数)的最大长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t filename_to_name(const char* filename, char* str, uint32_t size);

/**
 * @method filename_to_name_ex
 *
 * 从完整文件名中获取文件名。
 *
 * @param {const char*} filename 完整的文件名。
 * @param {char*} str 用于返回文件名。
 * @param {uint32_t} size 文件名(str参数)的最大长度。
 * @param {bool_t} remove_extname 是否移除扩展名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t filename_to_name_ex(const char* filename, char* str, uint32_t size, bool_t remove_extname);

/**
 * @method xml_file_expand_read
 *
 * expand include process instruction to file content: <?include filename="view_me.inc" ?>
 *
 * @param {const char*} filename 文件名。
 * @param {str_t*} s 用于返回内容。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t xml_file_expand_read(const char* filename, str_t* s);

/**
 * @method tk_str_copy
 *
 * 字符串拷贝函数。
 * > XXX: 要求dst为NULL或内存块的首地址，本函数调用之后，dst可能无效，请保留返回的地址
 *
 * @param {const char*} dst 目标字符串。
 * @param {const char*} src 源字符串。
 *
 * @return {char*} 返回指向的复制字符串指针，如果失败则返回NULL。
 */
char* tk_str_copy(char* dst, const char* src);

/**
 * @method dummy_destroy
 * 
 * 空的destroy函数。
 * @param {void*} data 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dummy_destroy(void* data);

/**
 * @method default_destroy
 * 
 * 缺省的destroy函数。释放data指向的内存。
 * @param {void*} data 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t default_destroy(void* data);

/**
 * @method pointer_compare
 * 
 * 指针比较。
 *
 * @param {const void*} a 数据a。
 * @param {const void*} b 数据b。
 *
 * @return {int} 返回0表示相等，返回负数表示小于，返回整数表示大于。
 */
int pointer_compare(const void* a, const void* b);

/**
 * @method compare_always_equal
 * 
 * 始终返回相等。
 *
 * @param {const void*} a 数据a。
 * @param {const void*} b 数据b。
 *
 * @return {int} 始终返回0。
 */
int compare_always_equal(const void* a, const void* b);

/**
 * @method tk_is_valid_name
 *
 * 判断是否是有效的属性名。
 *
 * @param {const char*} name 名字字符串。
 *
 * @return {bool_t} 如果名称有效返回TRUE，否则返回FALSE。
 */
bool_t tk_is_valid_name(const char* name);

ret_t tk_replace_locale(const char* name, char out[TK_NAME_LEN + 1], const char* locale);

/**
 * @method tk_str_start_with
 *
 * 检查字符串是否以指定的字符串prefix开头。
 *
 * @param {const char*} str 要检查字符串。
 * @param {const char*} prefix 被检查的字符串。
 *
 * @return {bool_t} 返回TRUE表示是；否则表示不是。
 */
bool_t tk_str_start_with(const char* str, const char* prefix);

/**
 * @method tk_under_score_to_camel
 *
 * 将下划线名字转成驼峰名字。
 *
 * @param {const char*} name 下划线名字。
 * @param {char*} out 驼峰名字(保存结果)。
 * @param {uint32_t} max_out_size 结果最大长度。 
 *
 * @return {const char* } 返回T驼峰名字。
 */
const char* tk_under_score_to_camel(const char* name, char* out, uint32_t max_out_size);

/**
 * @method tk_pointer_to_int
 *
 * 将指针转换成int。
 * 
 * > 常用于将int类型的数据作为回调函数的ctx。
 * 
 * @param {const void*} p 指针。
 * 
 * @return {int32_t} 返回对应的int数据。
 */
int32_t tk_pointer_to_int(const void* p);

/**
 * @method tk_pointer_from_int
 *
 * 将int转换成指针。
 * 
 * > 常用于将int类型的数据作为回调函数的ctx。
 * 
 * @param {int32_t} v 整数。
 * 
 * @return {void*} 返回对应的指针。
 */
void* tk_pointer_from_int(int32_t v);

/**
 * @method tk_str_toupper
 *
 * 将小写字母转换为大写字母。
 *
 * @param {char*} str 要被转换成大写字母的字符串。
 *
 * @return {char*} 返回转换后的大写字母字符串。
 */
char* tk_str_toupper(char* str);

/**
 * @method tk_str_tolower
 *
 * 将大写字母转换为小写字母。
 *
 * @param {char*} str 要被转换成小写字母的字符串。
 *
 * @return {char*} 返回转换后的小写字母字符串。
 */
char* tk_str_tolower(char* str);

/**
 * @method tk_wstr_dup_utf8
 *
 * 将utf8字符串拷贝为UCS字符串。
 *
 * @param {char*} str utf8编码的字符串。
 *
 * @return {wchar_t*} 返回UCS字符串(需要调用TKMEM_FREE释放)。
 */
wchar_t* tk_wstr_dup_utf8(const char* str);

/**
 * @method tk_wstr_count_c
 *
 * 统计UCS字符串中某个字符出现的次数。
 *
 * @param {const wchar_t*} str 字符串。
 * @param {wchar_t} c 字符。
 *
 * @return {uint32_t} 返回字符出现的次数。
 */
uint32_t tk_wstr_count_c(const wchar_t* str, wchar_t c);

const char* tk_normalize_key_name(const char* name, char fixed_name[TK_NAME_LEN + 1]);

END_C_DECLS

#endif /*TK_UTILS_H*/
