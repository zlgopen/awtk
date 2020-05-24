## enum\_helper
### 概述

----------------------------------
### 函数
<p id="enum_helper_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#enum_helper_asset_type_find_by_value">asset\_type\_find\_by\_value</a> | 获取资源类型枚值和名称的对应关系。 |
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
