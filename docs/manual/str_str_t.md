## str\_str\_t
### 概述
字符串-字符串类型。

负责把一个字符串映射成另外一个字符串。

示例：

```c
static const str_str_t color_values[] = {{"1", "red"}, {"2", "black"}, {"3", "blue"}, {"4", "white"}};
const char* value = str_str_value(color_values, "3");
const char* name = str_str_name(color_values, "blue", "0");
printf("value = %s \n", value);
printf("name = %s \n", name);
```
----------------------------------
### 函数
<p id="str_str_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#str_str_t_str_str_name">str\_str\_name</a> | 根据value获取对应的name。 |
| <a href="#str_str_t_str_str_value">str\_str\_value</a> | 根据name获取对应的value。 |
#### str\_str\_name 函数
-----------------------

* 函数功能：

> <p id="str_str_t_str_str_name">根据value获取对应的name。

* 函数原型：

```
const char* str_str_name (const str_str_t* items, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回name。 |
| items | const str\_str\_t* | str\_str\_t数组。 |
| value | const char* | value |
#### str\_str\_value 函数
-----------------------

* 函数功能：

> <p id="str_str_t_str_str_value">根据name获取对应的value。

* 函数原型：

```
const char* str_str_value (const str_str_t* items, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回value。 |
| items | const str\_str\_t* | str\_str\_t数组。 |
| name | const char* | name。 |
