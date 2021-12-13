# AWTK 中的字符串编码

## 1. 背景

不少同学在使用 AWTK 的过程中，遇到过字符串编码的问题。这里介绍一下 AWTK 中的字符串编码，希望能让新手少走弯路。

AWTK 中的字符串都是 Unicode 的，但是会使用两种不同的编码：

* UTF-8 其字符串类型为 char*。
* UTF-32 其字符串类型为 wchar_t*。

使用场景如下：

* 不需要显示的字符串使用 UTF-8 编码。如控件名、属性名、资源名和文件名等等。

* 需要显示的字符串（也就是控件的文本）使用 UTF-32 编码。如按钮上的文本和编辑器中的文本等等。

> UTF-8 的好处是能够兼容 ASCII，一般字符串操作比较方便。但是在编辑和显示文本时，需要把字符解码出来，使用不方便而且影响性能，所以 AWTK 中需要显示的字符串使用了 UTF-32 编码。

## 2. 源文件的编码

AWTK 的源文件，不管是 AWTK 本身还是应用程序，源文件一定要使用 UTF-8 BOM 格式。如果文件头有 UTF-8 BOM(Byte Order Mark) ，明确告诉编译器源文件的编码格式。否则不同的编译器可能有不同的解释，导致显示出现乱码。

> 最好是不要在源文件中使用非 ASCII 字符串，把界面上要显示的文本全部放到字符串资源文件中。可以避免不少问题，后期支持国际化也非常方便。

> 好像也有非主流编译器不支持 UTF-8 BOM 格式的源文件。AWTK 也提供了一个工具，用来去掉 UTF-8 BOM，在这个目录下：tools/remove\_utf8\_bom。

## 3. 用 UTF-8 的方式设置控件文本

用 widget\_set\_text\_utf8 设置控件文本，其函数原型如下：

```c
/**
 * @method widget_set_text_utf8
 * 设置控件的文本。
 * 只是对 widget\_set\_prop 的包装，文本的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @alias set_text
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  text 文本。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */

ret_t widget_set_text_utf8(widget_t* widget, const char* text);
```

要注意的是：这里的 text 一定要是有效的 UTF-8 编码的字符串。常见的错误有两种：

* 把 GBK 等编码直接当作 UTF-8 编码使用。
* 使用了 UTF-8 编码，但是最后一个字符非正常截断（使用 strncpy/snprintf 等函数时，如果目标内存不够大，可能会出现）。

这两种情况都可能导致下面的 assert:

```c
libunibreak/linebreak.c:799: set_linebreaks: Assertion `posLast == posCur - 1 && posCur <= len' failed.
```

解决方法如下：

* 对于 GBK 编码，需要转换后，才能设置到控件中。AWTK 提供了转函数：

> 请包含下列头文件（嵌入式系统需要自己加入相关源码和数据）。

```
charset/encoding.h
```

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

* 对于非正常截断的字符串。最好是分配更大的内存，防止字符串被意外截断。另外，AWTK 提供了一个函数，用来移除最后一个无效字符串。

```c
/**
 * @method tk_utf8_trim_invalid_char
 *
 * 如果字符串最后一个字符串是无效的，删掉该无效字符。
 *
 * > 对于用 strncpy/snprintf 等生成字符串时，如果目标字符串内存大小不够，
 * > 可能最后一个字符被从中间截断，导致该字符是无效的，这可能会触发 assert，
 * > 本函数可以用来删除最后一个无效字符。
 *
 * @param {const char*} str 输入字符串。
 *
 * @return {char*} 返回 UTF8 字符串。
 */
char* tk_utf8_trim_invalid_char(char* str);
```

## 4. 用 UTF-8 的方式获取控件文本

用 widget\_get\_text\_utf8 可获取控件的文本，其格式为 UTF-8 格式的字符串。

```c
/**
 * @method widget_get_text_utf8
 * 获取控件的文本。
 * 只是对 widget\_get\_prop 的包装，文本的意义由子类控件决定。
 * @param {widget_t*} widget 控件对象。
 * @param {char*}  text 用于返回文本。
 * @param {uint32_t} size text 内存长度。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_get_text_utf8(widget_t* widget, char* text, uint32_t size);
```

这个要注意的是 text 的内存要够大，否则可能触发 assert。

## 5. 用 UTF-32 的方式设置控件文本

用 widget\_set\_text 设置控件的文本，其原型如下：

```c
/**
 * @method widget_set_text
 * 设置控件的文本。
 * 只是对 widget\_set\_prop 的包装，文本的意义由子类控件决定。
 * @param {widget_t*} widget 控件对象。
 * @param {const wchar_t*}  text 文本。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_set_text(widget_t* widget, const wchar_t* text);
```

如果直接设置常量字符串，字符串一定要加 L 前缀。L 是告诉编译器使用 UTF-32 格式的字符串，否则可能编译出错，或者显示乱码（如果忽略警告）。

```c
widget_set_text(button, L"文本");
```

## 6. 用 UTF-32 的方式获取控件文本

用 widget\_get\_text 获取控件文本，其原型如下：

```c
/**
 * @method widget_get_text
 * 获取控件的文本。
 * 只是对 widget\_get\_prop 的包装，文本的意义由子类控件决定。
 *
 * 如果希望获取 UTF8 格式的文本，可以参考下面的代码：
 *
 * ```c
 *  str_t str;
 *  str_init(&str, 0);
 *  str_from_wstr(&str, widget_get_text(target));
 *  log_debug("%s: %s\n", target->name, str.str);
 *  str_reset(&str);
 * ```
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {const wchar_t*} 返回文本。
 */
const wchar_t* widget_get_text(widget_t* widget);
```
