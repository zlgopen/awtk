## object\_wbuffer\_t
### 概述
![image](images/object_wbuffer_t_0.png)

将wbuffer包装成object。
----------------------------------
### 函数
<p id="object_wbuffer_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#object_wbuffer_t_object_wbuffer_create">object\_wbuffer\_create</a> | 创建wbuffer对象。 |
| <a href="#object_wbuffer_t_object_wbuffer_create_extendable">object\_wbuffer\_create\_extendable</a> | 创建可扩展的wbuffer对象。 |
### 属性
<p id="object_wbuffer_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#object_wbuffer_t_wbuffer">wbuffer</a> | wbuffer\_t* | wbuffer对象。 |
#### object\_wbuffer\_create 函数
-----------------------

* 函数功能：

> <p id="object_wbuffer_t_object_wbuffer_create">创建wbuffer对象。

* 函数原型：

```
object_t* object_wbuffer_create (uint8_t* data, uint16_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_t* | 返回object对象。 |
| data | uint8\_t* | 缓冲区。 |
| capacity | uint16\_t | 缓冲区的容量。 |
#### object\_wbuffer\_create\_extendable 函数
-----------------------

* 函数功能：

> <p id="object_wbuffer_t_object_wbuffer_create_extendable">创建可扩展的wbuffer对象。

* 函数原型：

```
object_t* object_wbuffer_create_extendable ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_t* | 返回object对象。 |
#### wbuffer 属性
-----------------------
> <p id="object_wbuffer_t_wbuffer">wbuffer对象。

* 类型：wbuffer\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
