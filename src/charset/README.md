# 字符集转换函数

## 一、介绍

* 本模块属于可选组件，使用时需要自己包含头文件。

```
#include "charset/encoding.h"
```

* 使用示例：

```c
  char gbk[32];
  const char* str = "中文";
  encoding_utf8_to_gbk(str, strlen(str), gbk, sizeof(gbk));
```

## 二、 API 描述

```c

/**
 * @method encoding_convert
 *
 * 字符集转换。
 *
 * @annotation ["static"]
 *
 * @param {encoding_name_t} from 源字符串的编码名称。
 * @param {const char*} from_str 源字符串。
 * @param {uint32_t} from_size 源字符串的长度。
 * @param {encoding_name_t} to 目标字符串的编码名称。
 * @param {const char*} to_str 目标字符串。
 * @param {uint32_t} to_size 目标字符串的最大长度。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t encoding_convert(encoding_name_t from, const char* from_str, uint32_t from_size,
                       encoding_name_t to, char* to_str, uint32_t to_size);

/**
 * @method encoding_utf8_to_gbk
 *
 * 将 utf8 转换成 gbk 编码。
 *
 * @annotation ["static"]
 *
 * @param {const char*} from_str 源字符串 (utf8)。
 * @param {uint32_t} from_size 源字符串的长度。
 * @param {const char*} to_str 目标字符串 (gbk)。
 * @param {uint32_t} to_size 目标字符串的最大长度。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t encoding_utf8_to_gbk(const char* from_str, uint32_t from_size, char* to_str,
                           uint32_t to_size);

/**
 * @method encoding_gbk_to_utf8
 *
 * 将 gbk 转换成 utf8 编码。
 *
 * @annotation ["static"]
 *
 * @param {const char*} from_str 源字符串 (gbk)。
 * @param {uint32_t} from_size 源字符串的长度。
 * @param {const char*} to_str 目标字符串 (utf8)。
 * @param {uint32_t} to_size 目标字符串的最大长度。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t encoding_gbk_to_utf8(const char* from_str, uint32_t from_size, char* to_str,
                           uint32_t to_size);
```

## 三、其它

* 1. Linux/MacOS 平台使用 iconv 实现。

* 2. windows 下的 iconv 实现是从 win_iconv.inc [win-iconv](https://github.com/win-iconv/win-iconv) 改造来，版权属于原作者。

* 3. 嵌入式平台移植了 [postgresql](https://www.postgresql.org/) 中的 mb 库 (postgresql-9.6.3/src/backend/utils/mb)，版权属于原作者。考虑到空间问题，目前只加入了 utf8 和 gbk 转换的支持，如果需要支持其它字符集，请修改 impl/encoding_conv.inc。
