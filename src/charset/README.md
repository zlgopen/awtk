# 字符集转换函数

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

* API 描述

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
 * @param {const char*} to_str目标字符串。
 * @param {uint32_t} to_size 目标字符串的最大长度。
 *
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t encoding_convert(encoding_name_t from, const char* from_str, uint32_t from_size,
                       encoding_name_t to, char* to_str, uint32_t to_size);

/**
 * @method encoding_utf8_to_gbk
 *
 * 将utf8转换成gbk编码。
 *
 * @annotation ["static"]
 *
 * @param {const char*} from_str 源字符串(utf8)。
 * @param {uint32_t} from_size 源字符串的长度。
 * @param {const char*} to_str 目标字符串(gbk)。
 * @param {uint32_t} to_size 目标字符串的最大长度。
 *
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t encoding_utf8_to_gbk(const char* from_str, uint32_t from_size, char* to_str,
                           uint32_t to_size);

/**
 * @method encoding_gbk_to_utf8
 *
 * 将gbk转换成utf8编码。
 *
 * @annotation ["static"]
 *
 * @param {const char*} from_str 源字符串(gbk)。
 * @param {uint32_t} from_size 源字符串的长度。
 * @param {const char*} to_str目标字符串(utf8)。
 * @param {uint32_t} to_size 目标字符串的最大长度。
 *
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t encoding_gbk_to_utf8(const char* from_str, uint32_t from_size, char* to_str,
                           uint32_t to_size);
```

## 其它
* windows 下的 iconv 实现是从 win_iconv.inc [win-iconv](https://github.com/win-iconv/win-iconv) 改造来，版权属于原作者。
