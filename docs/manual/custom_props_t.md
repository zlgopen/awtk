## custom\_props\_t
### 概述
 widget自定义属性集合。

----------------------------------
### 函数
<p id="custom_props_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#custom_props_t_custom_props_create">custom\_props\_create</a> | 创建custom_props对象 |
| <a href="#custom_props_t_custom_props_destroy">custom\_props\_destroy</a> | 销毁属性集合。 |
| <a href="#custom_props_t_custom_props_get">custom\_props\_get</a> | 获取属性的值。 |
| <a href="#custom_props_t_custom_props_set">custom\_props\_set</a> | 设置属性的值。 |
#### custom\_props\_create 函数
-----------------------

* 函数功能：

> <p id="custom_props_t_custom_props_create"> 创建custom_props对象



* 函数原型：

```
custom_props_t* custom_props_create (uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | custom\_props\_t* | custom\_props对象。 |
| capacity | uint32\_t | 初始容量。 |
#### custom\_props\_destroy 函数
-----------------------

* 函数功能：

> <p id="custom_props_t_custom_props_destroy"> 销毁属性集合。



* 函数原型：

```
ret_t custom_props_destroy (custom_props_t props);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| props | custom\_props\_t | 属性集合对象。 |
#### custom\_props\_get 函数
-----------------------

* 函数功能：

> <p id="custom_props_t_custom_props_get"> 获取属性的值。



* 函数原型：

```
ret_t custom_props_get (custom_props_t props, char* name, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| props | custom\_props\_t | 属性集合对象。 |
| name | char* | 属性的名称。 |
| v | value\_t* | 用于返回属性的值。 |
#### custom\_props\_set 函数
-----------------------

* 函数功能：

> <p id="custom_props_t_custom_props_set"> 设置属性的值。



* 函数原型：

```
ret_t custom_props_set (custom_props_t props, char* name, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| props | custom\_props\_t | 属性集合对象。 |
| name | char* | 属性的名称。 |
| v | value\_t* | 属性的值。 |
