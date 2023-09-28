## endian\_t
### 概述
字节顺序转换相关函数。

示例

```c
uint16_t a = 0x1234;
ENSURE(is_little_endian());
ENSURE(int16_endian_invert(a) == 0x3412);
ENSURE(int16_from_big_endian(a) == 0x3412);
ENSURE(int16_to_big_endian(a) == 0x3412);

int32_t b = 0x12345678;
ENSURE(int32_endian_invert(b) == 0x78563412);
ENSURE(int32_from_big_endian(b) == 0x78563412);
ENSURE(int32_to_big_endian(b) == 0x78563412);

int64_t c = 0x1234567890ABCDEF;
ENSURE(int64_endian_invert(c) == 0xEFCDAB9078563412);
ENSURE(int64_from_big_endian(c) == 0xEFCDAB9078563412);
ENSURE(int64_to_big_endian(c) == 0xEFCDAB9078563412);

float f1 = 1.23;
float f2 = float_endian_invert(f1);
ENSURE(float_from_big_endian(f2) == f1);
float f3 = float_from_big_endian(f2);
ENSURE(float_to_big_endian(f3) == f2);

double d1 = 1.2345678;
double d2 = double_endian_invert(d1);
ENSURE(double_from_big_endian(d2) == d1);
double d3 = double_from_big_endian(d2);
ENSURE(double_to_big_endian(d3) == d2);
```
----------------------------------
### 函数
<p id="endian_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#endian_t_double_endian_invert">double\_endian\_invert</a> | double浮点数大小端模式转换。 |
| <a href="#endian_t_double_from_big_endian">double\_from\_big\_endian</a> | double浮点数转换为小端模式。 |
| <a href="#endian_t_double_to_big_endian">double\_to\_big\_endian</a> | double浮点数转换为大端模式。 |
| <a href="#endian_t_float_endian_invert">float\_endian\_invert</a> | float浮点数大小端模式转换。 |
| <a href="#endian_t_float_from_big_endian">float\_from\_big\_endian</a> | float浮点数转换为小端模式。 |
| <a href="#endian_t_float_to_big_endian">float\_to\_big\_endian</a> | float浮点数转换为大端模式。 |
| <a href="#endian_t_int16_endian_invert">int16\_endian\_invert</a> | 16 整数大小端模式转换。 |
| <a href="#endian_t_int16_from_big_endian">int16\_from\_big\_endian</a> | 16 整数转换为小端模式。 |
| <a href="#endian_t_int16_to_big_endian">int16\_to\_big\_endian</a> | 16 整数转换为大端模式。 |
| <a href="#endian_t_int32_endian_invert">int32\_endian\_invert</a> | 32 整数大小端模式转换。 |
| <a href="#endian_t_int32_from_big_endian">int32\_from\_big\_endian</a> | 32 整数转换为小端模式。 |
| <a href="#endian_t_int32_to_big_endian">int32\_to\_big\_endian</a> | 32 整数转换为大端模式。 |
| <a href="#endian_t_int64_endian_invert">int64\_endian\_invert</a> | 64 整数大小端模式转换。 |
| <a href="#endian_t_int64_from_big_endian">int64\_from\_big\_endian</a> | 64 整数转换为小端模式。 |
| <a href="#endian_t_int64_to_big_endian">int64\_to\_big\_endian</a> | 64 整数转换为大端模式。 |
| <a href="#endian_t_is_little_endian">is\_little\_endian</a> | 检查当前系统是否是小端模式。 |
#### double\_endian\_invert 函数
-----------------------

* 函数功能：

> <p id="endian_t_double_endian_invert">double浮点数大小端模式转换。

* 函数原型：

```
double double_endian_invert (double value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | double | 返回转换后的数据。 |
| value | double | 输入数据。 |
#### double\_from\_big\_endian 函数
-----------------------

* 函数功能：

> <p id="endian_t_double_from_big_endian">double浮点数转换为小端模式。

* 函数原型：

```
double double_from_big_endian (double value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | double | 返回转换后的数据。 |
| value | double | 输入数据。 |
#### double\_to\_big\_endian 函数
-----------------------

* 函数功能：

> <p id="endian_t_double_to_big_endian">double浮点数转换为大端模式。

* 函数原型：

```
double double_to_big_endian (double value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | double | 返回转换后的数据。 |
| value | double | 输入数据。 |
#### float\_endian\_invert 函数
-----------------------

* 函数功能：

> <p id="endian_t_float_endian_invert">float浮点数大小端模式转换。

* 函数原型：

```
float float_endian_invert (float value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float | 返回转换后的数据。 |
| value | float | 输入数据。 |
#### float\_from\_big\_endian 函数
-----------------------

* 函数功能：

> <p id="endian_t_float_from_big_endian">float浮点数转换为小端模式。

* 函数原型：

```
float float_from_big_endian (float value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float | 返回转换后的数据。 |
| value | float | 输入数据。 |
#### float\_to\_big\_endian 函数
-----------------------

* 函数功能：

> <p id="endian_t_float_to_big_endian">float浮点数转换为大端模式。

* 函数原型：

```
float float_to_big_endian (float value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float | 返回转换后的数据。 |
| value | float | 输入数据。 |
#### int16\_endian\_invert 函数
-----------------------

* 函数功能：

> <p id="endian_t_int16_endian_invert">16 整数大小端模式转换。

* 函数原型：

```
int16_t int16_endian_invert (int16_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int16\_t | 返回转换后的数据。 |
| value | int16\_t | 输入数据。 |
#### int16\_from\_big\_endian 函数
-----------------------

* 函数功能：

> <p id="endian_t_int16_from_big_endian">16 整数转换为小端模式。

* 函数原型：

```
int16_t int16_from_big_endian (int16_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int16\_t | 返回转换后的数据。 |
| value | int16\_t | 输入数据。 |
#### int16\_to\_big\_endian 函数
-----------------------

* 函数功能：

> <p id="endian_t_int16_to_big_endian">16 整数转换为大端模式。

* 函数原型：

```
int16_t int16_to_big_endian (int16_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int16\_t | 返回转换后的数据。 |
| value | int16\_t | 输入数据。 |
#### int32\_endian\_invert 函数
-----------------------

* 函数功能：

> <p id="endian_t_int32_endian_invert">32 整数大小端模式转换。

* 函数原型：

```
int32_t int32_endian_invert (int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回转换后的数据。 |
| value | int32\_t | 输入数据。 |
#### int32\_from\_big\_endian 函数
-----------------------

* 函数功能：

> <p id="endian_t_int32_from_big_endian">32 整数转换为小端模式。

* 函数原型：

```
int32_t int32_from_big_endian (int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回转换后的数据。 |
| value | int32\_t | 输入数据。 |
#### int32\_to\_big\_endian 函数
-----------------------

* 函数功能：

> <p id="endian_t_int32_to_big_endian">32 整数转换为大端模式。

* 函数原型：

```
int32_t int32_to_big_endian (int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回转换后的数据。 |
| value | int32\_t | 输入数据。 |
#### int64\_endian\_invert 函数
-----------------------

* 函数功能：

> <p id="endian_t_int64_endian_invert">64 整数大小端模式转换。

* 函数原型：

```
int64_t int64_endian_invert (int64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int64\_t | 返回转换后的数据。 |
| value | int64\_t | 输入数据。 |
#### int64\_from\_big\_endian 函数
-----------------------

* 函数功能：

> <p id="endian_t_int64_from_big_endian">64 整数转换为小端模式。

* 函数原型：

```
int64_t int64_from_big_endian (int64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int64\_t | 返回转换后的数据。 |
| value | int64\_t | 输入数据。 |
#### int64\_to\_big\_endian 函数
-----------------------

* 函数功能：

> <p id="endian_t_int64_to_big_endian">64 整数转换为大端模式。

* 函数原型：

```
int64_t int64_to_big_endian (int64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int64\_t | 返回转换后的数据。 |
| value | int64\_t | 输入数据。 |
#### is\_little\_endian 函数
-----------------------

* 函数功能：

> <p id="endian_t_is_little_endian">检查当前系统是否是小端模式。

* 函数原型：

```
bool_t is_little_endian ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示是，否则表示不是。 |
