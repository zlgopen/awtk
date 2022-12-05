## enum\_helper
### 概述

----------------------------------
### 函数
<p id="enum_helper_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#enum_helper_asset_type_find_by_value">asset\_type\_find\_by\_value</a> | 获取资源类型枚值和名称的对应关系。 |
| <a href="#enum_helper_find_item">find\_item</a> | 获取特定枚值和名称的对应关系。 |
| <a href="#enum_helper_find_item_by_value">find\_item\_by\_value</a> | 获取特定枚值和名称的对应关系。 |
| <a href="#enum_helper_input_type_find">input\_type\_find</a> | 获取输入类型值和名称的对应关系。 |
| <a href="#enum_helper_keys_type_find">keys\_type\_find</a> | 获取按键值和名称的对应关系。 |
| <a href="#enum_helper_keys_type_find_by_value">keys\_type\_find\_by\_value</a> | 获取按键值和名称的对应关系。 |
| <a href="#enum_helper_keys_type_set_custom_keys">keys\_type\_set\_custom\_keys</a> | 设置自定义键值表。 |
#### asset\_type\_find\_by\_value 函数
-----------------------

* 函数功能：

> <p id="enum_helper_asset_type_find_by_value">获取资源类型枚值和名称的对应关系。

* 函数原型：

```
const key_type_value_t* asset_type_find_by_value (uint32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const key\_type\_value\_t* | 返回资源类型枚值和名称的对应关系。 |
| value | uint32\_t | 资源的类型。 |
#### find\_item 函数
-----------------------

* 函数功能：

> <p id="enum_helper_find_item">获取特定枚值和名称的对应关系。

* 函数原型：

```
const key_type_value_t* find_item (const key_type_value_t* items, uint32_t nr, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const key\_type\_value\_t* | 返回资源类型枚值和名称的对应关系。 |
| items | const key\_type\_value\_t* | 特定枚举值对应列表。 |
| nr | uint32\_t | 列表长度。 |
| name | const char* | 名称。 |
#### find\_item\_by\_value 函数
-----------------------

* 函数功能：

> <p id="enum_helper_find_item_by_value">获取特定枚值和名称的对应关系。

* 函数原型：

```
const key_type_value_t* find_item_by_value (const key_type_value_t* items, uint32_t nr, uint32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const key\_type\_value\_t* | 返回资源类型枚值和名称的对应关系。 |
| items | const key\_type\_value\_t* | 特定枚举值对应列表。 |
| nr | uint32\_t | 列表长度。 |
| value | uint32\_t | 资源的类型。 |
#### input\_type\_find 函数
-----------------------

* 函数功能：

> <p id="enum_helper_input_type_find">获取输入类型值和名称的对应关系。

* 函数原型：

```
const key_type_value_t* input_type_find (const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const key\_type\_value\_t* | 返回输入类型值和名称的对应关系。 |
| name | const char* | 输入类型名。 |
#### keys\_type\_find 函数
-----------------------

* 函数功能：

> <p id="enum_helper_keys_type_find">获取按键值和名称的对应关系。

* 函数原型：

```
const key_type_value_t* keys_type_find (const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const key\_type\_value\_t* | 返回按键值和名称的对应关系。 |
| name | const char* | 按键名。 |
#### keys\_type\_find\_by\_value 函数
-----------------------

* 函数功能：

> <p id="enum_helper_keys_type_find_by_value">获取按键值和名称的对应关系。

* 函数原型：

```
const key_type_value_t* keys_type_find_by_value (uint32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const key\_type\_value\_t* | 返回按键值和名称的对应关系。 |
| value | uint32\_t | 按键值。 |
#### keys\_type\_set\_custom\_keys 函数
-----------------------

* 函数功能：

> <p id="enum_helper_keys_type_set_custom_keys">设置自定义键值表。
> 请确保键值表的生命周期在程序运行期一直存在，不要传入局部变量。

* 函数原型：

```
ret_t keys_type_set_custom_keys (const key_type_value_t* table, uint32_t nr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| table | const key\_type\_value\_t* | 键值表(NULL清除之前的设置)。 |
| nr | uint32\_t | 键值表的长度。 |
