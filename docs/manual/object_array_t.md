## object\_array\_t
### 概述
![image](images/object_array_t_0.png)

简单的动态数组，内部存放value对象。

访问时属性名称为：

* "size"/"length" 用于获取数组的长度。
* index 用于访问属性，-1可以用来追加新元素。
----------------------------------
### 函数
<p id="object_array_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#object_array_t_object_array_avg">object\_array\_avg</a> | 求平均值。 |
| <a href="#object_array_t_object_array_clear_props">object\_array\_clear\_props</a> | 清除全部属性。 |
| <a href="#object_array_t_object_array_clone">object\_array\_clone</a> | 克隆对象。 |
| <a href="#object_array_t_object_array_create">object\_array\_create</a> | 创建对象。 |
| <a href="#object_array_t_object_array_create_with_str">object\_array\_create\_with\_str</a> | 通过字符串构建数组。 |
| <a href="#object_array_t_object_array_dup">object\_array\_dup</a> | 复制部分或全部元素。 |
| <a href="#object_array_t_object_array_get">object\_array\_get</a> | 获取指定序数的元素。 |
| <a href="#object_array_t_object_array_get_and_remove">object\_array\_get\_and\_remove</a> | 在指定位置删除一个元素，并返回它。 |
| <a href="#object_array_t_object_array_index_of">object\_array\_index\_of</a> | 查找元素出现的第一个位置。 |
| <a href="#object_array_t_object_array_insert">object\_array\_insert</a> | 在指定位置插入一个元素。 |
| <a href="#object_array_t_object_array_join">object\_array\_join</a> | 使用分隔符把各个元素拼接起来。 |
| <a href="#object_array_t_object_array_last_index_of">object\_array\_last\_index\_of</a> | 查找元素出现的最后一个位置。 |
| <a href="#object_array_t_object_array_max">object\_array\_max</a> | 查找最大值。 |
| <a href="#object_array_t_object_array_min">object\_array\_min</a> | 查找最小值。 |
| <a href="#object_array_t_object_array_pop">object\_array\_pop</a> | 弹出最后一个元素。 |
| <a href="#object_array_t_object_array_push">object\_array\_push</a> | 追加一个元素。 |
| <a href="#object_array_t_object_array_remove">object\_array\_remove</a> | 在指定位置删除一个元素。 |
| <a href="#object_array_t_object_array_set">object\_array\_set</a> | 设置指定序数的元素。 |
| <a href="#object_array_t_object_array_shift">object\_array\_shift</a> | 弹出第一个元素。 |
| <a href="#object_array_t_object_array_sort">object\_array\_sort</a> | 排序。 |
| <a href="#object_array_t_object_array_sort_as_double">object\_array\_sort\_as\_double</a> | 按浮点数排序。 |
| <a href="#object_array_t_object_array_sort_as_int">object\_array\_sort\_as\_int</a> | 按整数排序。 |
| <a href="#object_array_t_object_array_sort_as_str">object\_array\_sort\_as\_str</a> | 按字符串排序。 |
| <a href="#object_array_t_object_array_sum">object\_array\_sum</a> | 求和。 |
| <a href="#object_array_t_object_array_unref">object\_array\_unref</a> | for script gc |
### 属性
<p id="object_array_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#object_array_t_capacity">capacity</a> | uint32\_t | 属性数组的容量。 |
| <a href="#object_array_t_props">props</a> | value\_t | 属性数组。 |
| <a href="#object_array_t_size">size</a> | uint32\_t | 属性个数。 |
#### object\_array\_avg 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_avg">求平均值。

* 函数原型：

```
ret_t object_array_avg (object_t* obj, value_t* result);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| result | value\_t* | 结果。 |
#### object\_array\_clear\_props 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_clear_props">清除全部属性。

* 函数原型：

```
ret_t object_array_clear_props (object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
#### object\_array\_clone 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_clone">克隆对象。

* 函数原型：

```
object_t* object_array_clone (object_t* o);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_t* | 返回object对象。 |
| o | object\_t* | 被克隆的对象。 |
#### object\_array\_create 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_create">创建对象。

* 函数原型：

```
object_t* object_array_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_t* | 返回object对象。 |
#### object\_array\_create\_with\_str 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_create_with_str">通过字符串构建数组。

* 函数原型：

```
object_t* object_array_create_with_str (const char* str, const char* sep, value_type_t type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_t* | 返回object对象。 |
| str | const char* | 字符串 |
| sep | const char* | 分隔符。 |
| type | value\_type\_t | 类型。 |
#### object\_array\_dup 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_dup">复制部分或全部元素。

* 函数原型：

```
object_t* object_array_dup (object_t* obj, uint32_t start, uint32_t end);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_t* | 返回object对象。 |
| obj | object\_t* | 数组对象。 |
| start | uint32\_t | 开始的位置。 |
| end | uint32\_t | 结束的位置(不包含)。 |
#### object\_array\_get 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_get">获取指定序数的元素。

* 函数原型：

```
ret_t object_array_get (object_t* obj, uint32_t i, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| i | uint32\_t | 序数。 |
| v | value\_t* | 返回的元素。 |
#### object\_array\_get\_and\_remove 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_get_and_remove">在指定位置删除一个元素，并返回它。

* 函数原型：

```
ret_t object_array_get_and_remove (object_t* obj, uint32_t index, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
| index | uint32\_t | 位置。 |
| v | value\_t* | 用于返回值。 |
#### object\_array\_index\_of 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_index_of">查找元素出现的第一个位置。

* 函数原型：

```
int32_t object_array_index_of (object_t* obj, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 如果找到返回其位置，否则返回-1。 |
| obj | object\_t* | 对象。 |
| v | const value\_t* | 值。 |
#### object\_array\_insert 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_insert">在指定位置插入一个元素。

* 函数原型：

```
ret_t object_array_insert (object_t* obj, uint32_t index, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
| index | uint32\_t | 位置。 |
| v | const value\_t* | 值。 |
#### object\_array\_join 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_join">使用分隔符把各个元素拼接起来。

* 函数原型：

```
ret_t object_array_join (object_t* obj, const char* sep, str_t* result);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| sep | const char* | 分隔符。 |
| result | str\_t* | 生成的字符串。 |
#### object\_array\_last\_index\_of 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_last_index_of">查找元素出现的最后一个位置。

* 函数原型：

```
int32_t object_array_last_index_of (object_t* obj, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 如果找到返回其位置，否则返回-1。 |
| obj | object\_t* | 对象。 |
| v | const value\_t* | 值。 |
#### object\_array\_max 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_max">查找最大值。

* 函数原型：

```
ret_t object_array_max (object_t* obj, value_t* result);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| result | value\_t* | 结果。 |
#### object\_array\_min 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_min">查找最小值。

* 函数原型：

```
ret_t object_array_min (object_t* obj, value_t* result);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| result | value\_t* | 结果。 |
#### object\_array\_pop 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_pop">弹出最后一个元素。

* 函数原型：

```
ret_t object_array_pop (object_t* obj, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
| v | value\_t* | 返回值。 |
#### object\_array\_push 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_push">追加一个元素。

* 函数原型：

```
ret_t object_array_push (object_t* obj, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
| v | const value\_t* | 值。 |
#### object\_array\_remove 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_remove">在指定位置删除一个元素。

* 函数原型：

```
ret_t object_array_remove (object_t* obj, uint32_t index);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
| index | uint32\_t | 位置。 |
#### object\_array\_set 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_set">设置指定序数的元素。

* 函数原型：

```
ret_t object_array_set (object_t* obj, uint32_t i, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| i | uint32\_t | 序数。 |
| v | const value\_t* | 元素。 |
#### object\_array\_shift 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_shift">弹出第一个元素。

* 函数原型：

```
ret_t object_array_shift (object_t* obj, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
| v | value\_t* | 返回值。 |
#### object\_array\_sort 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_sort">排序。

* 函数原型：

```
ret_t object_array_sort (object_t* obj, tk_compare_t cmp);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| cmp | tk\_compare\_t | 比较函数。 |
#### object\_array\_sort\_as\_double 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_sort_as_double">按浮点数排序。

* 函数原型：

```
ret_t object_array_sort_as_double (object_t* obj, bool_t ascending);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| ascending | bool\_t | 升序或降序。 |
#### object\_array\_sort\_as\_int 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_sort_as_int">按整数排序。

* 函数原型：

```
ret_t object_array_sort_as_int (object_t* obj, bool_t ascending);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| ascending | bool\_t | 升序或降序。 |
#### object\_array\_sort\_as\_str 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_sort_as_str">按字符串排序。

* 函数原型：

```
ret_t object_array_sort_as_str (object_t* obj, bool_t ascending, bool_t ignore_case);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| ascending | bool\_t | 升序或降序。 |
| ignore\_case | bool\_t | 是否忽略大小写。 |
#### object\_array\_sum 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_sum">求和。

* 函数原型：

```
ret_t object_array_sum (object_t* obj, value_t* result);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| result | value\_t* | 结果。 |
#### object\_array\_unref 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_unref">for script gc

* 函数原型：

```
ret_t object_array_unref (object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
#### capacity 属性
-----------------------
> <p id="object_array_t_capacity">属性数组的容量。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### props 属性
-----------------------
> <p id="object_array_t_props">属性数组。

* 类型：value\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### size 属性
-----------------------
> <p id="object_array_t_size">属性个数。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
