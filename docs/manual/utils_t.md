## utils\_t
### 概述
工具类。
----------------------------------
### 函数
<p id="utils_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#utils_t_tk_atob">tk\_atob</a> | 将字符串转换为布尔类型。 |
| <a href="#utils_t_tk_atof">tk\_atof</a> | 将字符串转换为浮点类型。 |
| <a href="#utils_t_tk_atoi">tk\_atoi</a> | 将字符串转换为整形。 |
| <a href="#utils_t_tk_ftoa">tk\_ftoa</a> | 将浮点型转换为字符串。 |
| <a href="#utils_t_tk_itoa">tk\_itoa</a> | 将整形转换为字符串。 |
| <a href="#utils_t_tk_skip_to_num">tk\_skip\_to\_num</a> | 跳过字符串函数，如：字符串"hello123world"，返回的结果是"123world"。 |
| <a href="#utils_t_tk_snprintf">tk\_snprintf</a> | 将可变参数(...)按照format格式化字符串，并将字符串复制到str中。 |
| <a href="#utils_t_tk_sscanf">tk\_sscanf</a> | 从字符串读取格式化输入。 |
| <a href="#utils_t_tk_str_append">tk\_str\_append</a> | 字符串追加函数。 |
| <a href="#utils_t_tk_str_cmp">tk\_str\_cmp</a> | 字符串比较函数。 |
| <a href="#utils_t_tk_str_icmp">tk\_str\_icmp</a> | 字符串比较函数（不区分大小写）。 |
| <a href="#utils_t_tk_str_start_with">tk\_str\_start\_with</a> | 检查字符串是否以指定的字符串prefix开头。 |
| <a href="#utils_t_tk_str_tolower">tk\_str\_tolower</a> | 将大写字母转换为小写字母。 |
| <a href="#utils_t_tk_str_toupper">tk\_str\_toupper</a> | 将小写字母转换为大写字母。 |
| <a href="#utils_t_tk_strcpy">tk\_strcpy</a> | 将src所指向的字符串复制到dst。 |
| <a href="#utils_t_tk_strdup">tk\_strdup</a> | 字符串拷贝函数。 |
| <a href="#utils_t_tk_strncpy">tk\_strncpy</a> | 将src所指向的字符串复制到dst，最多复制len个字符串。 |
| <a href="#utils_t_tk_strndup">tk\_strndup</a> | 字符串拷贝函数，最多复制len个字符串。 |
| <a href="#utils_t_tk_strtol">tk\_strtol</a> | 将字符串转换为长整形。 |
| <a href="#utils_t_tk_watob">tk\_watob</a> | 将宽字符串转换为布尔类型。 |
| <a href="#utils_t_tk_watof">tk\_watof</a> | 将宽字符串转换为浮点类型。 |
| <a href="#utils_t_tk_watoi">tk\_watoi</a> | 将宽字符串转换为整形。 |
| <a href="#utils_t_tk_wstrdup">tk\_wstrdup</a> | 宽字符串拷贝函数。 |
#### tk\_atob 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_atob">将字符串转换为布尔类型。

* 函数原型：

```
bool_t tk_atob (const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回转换后的布尔类型。 |
| str | const char* | 要转换为布尔类型的字符串。 |
#### tk\_atof 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_atof">将字符串转换为浮点类型。

* 函数原型：

```
double tk_atof (const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | double | 返回转换后的浮点类型。 |
| str | const char* | 要转换为浮点类型的字符串。 |
#### tk\_atoi 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_atoi">将字符串转换为整形。

* 函数原型：

```
int tk_atoi (const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 返回转换后的整形。 |
| str | const char* | 要转换为整形的字符串。 |
#### tk\_ftoa 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_ftoa">将浮点型转换为字符串。

* 函数原型：

```
const char* tk_ftoa (char* str, int len, double f);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回字符串。 |
| str | char* | 保存字符串缓冲区。 |
| len | int | 缓冲区大小。 |
| f | double | 要转换的浮点型。 |
#### tk\_itoa 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_itoa">将整形转换为字符串。

* 函数原型：

```
const char* tk_itoa (char* str, int len, int n);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回字符串。 |
| str | char* | 保存字符串缓冲区。 |
| len | int | 缓冲区大小。 |
| n | int | 要转换的整形。 |
#### tk\_skip\_to\_num 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_skip_to_num">跳过字符串函数，如：字符串"hello123world"，返回的结果是"123world"。

* 函数原型：

```
const char* tk_skip_to_num (const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回转换后的字符串。 |
| str | const char* | 要输入的原始字符串。 |
#### tk\_snprintf 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_snprintf">将可变参数(...)按照format格式化字符串，并将字符串复制到str中。

* 函数原型：

```
int tk_snprintf (char* str, size_t size, const char* format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 返回格式化后的字符串长度+1。 |
| str | char* | 目标字符串。 |
| size | size\_t | 拷贝字节数。 |
| format | const char* | 格式化字符串。 |
#### tk\_sscanf 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_sscanf">从字符串读取格式化输入。

* 函数原型：

```
int tk_sscanf (char* str, const char* format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 返回成功匹配和赋值的个数。 |
| str | char* | 要输入的字符串。 |
| format | const char* | 格式化字符串。 |
#### tk\_str\_append 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_str_append">字符串追加函数。

* 函数原型：

```
ret_t tk_str_append (char* str, uint32_t max_len, const char* s);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | char* | 被追加字符串。 |
| max\_len | uint32\_t | 字符串的最大长度。 |
| s | const char* | 要追加的字符串。 |
#### tk\_str\_cmp 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_str_cmp">字符串比较函数。

* 函数原型：

```
int32_t tk_str_cmp (const char* a, const char* b);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 如果返回值=-1，则表示a为NULL；如果返回值=1，则表示b为NULL；如果返回值<0，则表示a小于b；如果返回值>0，则表示a大于b；如果返回值=0，则表示a等于b。 |
| a | const char* | 要进行比较的第一个字符串。 |
| b | const char* | 要进行比较的第二个字符串。 |
#### tk\_str\_icmp 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_str_icmp">字符串比较函数（不区分大小写）。

* 函数原型：

```
int32_t tk_str_icmp (const char* a, const char* b);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 如果返回值=-1，则表示a为NULL；如果返回值=1，则表示b为NULL；如果返回值<0，则表示a小于b；如果返回值>0，则表示a大于b；如果返回值=0，则表示a等于b。 |
| a | const char* | 要进行比较的第一个字符串。 |
| b | const char* | 要进行比较的第二个字符串。 |
#### tk\_str\_start\_with 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_str_start_with">检查字符串是否以指定的字符串prefix开头。

* 函数原型：

```
bool_t tk_str_start_with (const char* str, const char* prefix);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示是；否则表示不是。 |
| str | const char* | 要检查字符串。 |
| prefix | const char* | 被检查的字符串。 |
#### tk\_str\_tolower 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_str_tolower">将大写字母转换为小写字母。

* 函数原型：

```
char* tk_str_tolower (char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回转换后的小写字母字符串。 |
| str | char* | 要被转换成小写字母的字符串。 |
#### tk\_str\_toupper 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_str_toupper">将小写字母转换为大写字母。

* 函数原型：

```
char* tk_str_toupper (char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回转换后的大写字母字符串。 |
| str | char* | 要被转换成大写字母的字符串。 |
#### tk\_strcpy 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_strcpy">将src所指向的字符串复制到dst。

* 函数原型：

```
char* tk_strcpy (char* dst, const char* src);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回目标字符串。 |
| dst | char* | 目标字符串。 |
| src | const char* | 原字符串。 |
#### tk\_strdup 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_strdup">字符串拷贝函数。

* 函数原型：

```
char* tk_strdup (const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回指向的复制字符串指针，如果失败则返回NULL。 |
| str | const char* | 原字符串。 |
#### tk\_strncpy 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_strncpy">将src所指向的字符串复制到dst，最多复制len个字符串。

* 函数原型：

```
char* tk_strncpy (char* dst, const char* src, size_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回目标字符串。 |
| dst | char* | 目标字符串。 |
| src | const char* | 原字符串。 |
| len | size\_t | 要复制的字符串个数。 |
#### tk\_strndup 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_strndup">字符串拷贝函数，最多复制len个字符串。

* 函数原型：

```
char* tk_strndup (const char* str, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回指向的复制字符串指针，如果失败则返回NULL。 |
| str | const char* | 原字符串。 |
| len | uint32\_t | 要复制的字符串个数。 |
#### tk\_strtol 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_strtol">将字符串转换为长整形。

* 函数原型：

```
long tk_strtol (const char* str, const char** end, int base);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | long | 返回转换后的长整形。 |
| str | const char* | 要转换为长整形的字符串。 |
| end | const char** | 对类型char*的对象的引用。 |
| base | int | 基数。 |
#### tk\_watob 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_watob">将宽字符串转换为布尔类型。

* 函数原型：

```
bool_t tk_watob (const wchar_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回转换后的布尔类型。 |
| str | const wchar\_t* | 要转换为布尔类型的宽字符串。 |
#### tk\_watof 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_watof">将宽字符串转换为浮点类型。

* 函数原型：

```
double tk_watof (const wchar_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | double | 返回转换后的浮点类型。 |
| str | const wchar\_t* | 要转换为浮点类型的宽字符串。 |
#### tk\_watoi 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_watoi">将宽字符串转换为整形。

* 函数原型：

```
int tk_watoi (const wchar_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 返回转换后的整形。 |
| str | const wchar\_t* | 要转换为整形的宽字符串。 |
#### tk\_wstrdup 函数
-----------------------

* 函数功能：

> <p id="utils_t_tk_wstrdup">宽字符串拷贝函数。

* 函数原型：

```
wchar_t* tk_wstrdup (const wchar_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wchar\_t* | 返回指向的复制宽字符串指针，如果失败则返回NULL。 |
| str | const wchar\_t* | 原宽字符串。 |
