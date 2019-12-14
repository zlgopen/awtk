## int\_str\_t
### 概述
数字-字符串类型。

负责把一个数字映射成一个字符串。

示例：

```c
static const int_str_t color_values[] = {{1, "red"}, {2, "black"}, {3, "blue"}, {4, "white"}};
const char* value = int_str_value(color_values, 3);
int32_t name = int_str_name(color_values, "blue", 0);
printf("value = %s \n", value);
printf("name = %d \n", name);
```
----------------------------------
### 函数
<p id="int_str_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#int_str_t_int_str_name">int\_str\_name</a> | 根据value获取对应的name。 |
| <a href="#int_str_t_int_str_value">int\_str\_value</a> | 根据name获取对应的value。 |
#### int\_str\_name 函数
-----------------------

* 函数功能：

> <p id="int_str_t_int_str_name">根据value获取对应的name。

* 函数原型：

```
int32_t int_str_name (const int_str_t* items, const char* value, int32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回name。 |
| items | const int\_str\_t* | int\_str\_t数组。 |
| value | const char* | value。 |
| defval | int32\_t | 如果没有找到对应的name，则返回的defval默认值。 |
#### int\_str\_value 函数
-----------------------

* 函数功能：

> <p id="int_str_t_int_str_value">根据name获取对应的value。

* 函数原型：

```
const char* int_str_value (const int_str_t* items, int32_t name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回value。 |
| items | const int\_str\_t* | int\_str\_t数组。 |
| name | int32\_t | name。 |
