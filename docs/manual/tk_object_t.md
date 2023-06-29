## tk\_object\_t
### 概述
![image](images/tk_object_t_0.png)

对象接口。
----------------------------------
### 函数
<p id="tk_object_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_object_t_tk_object_can_exec">tk\_object\_can\_exec</a> | 检查是否可以执行指定的命令。 |
| <a href="#tk_object_t_tk_object_can_exec_by_path">tk\_object\_can\_exec\_by\_path</a> | 检查是否可以执行指定的命令。 |
| <a href="#tk_object_t_tk_object_clone">tk\_object\_clone</a> | clone对象。 |
| <a href="#tk_object_t_tk_object_compare">tk\_object\_compare</a> | 比较两个对象。 |
| <a href="#tk_object_t_tk_object_copy_prop">tk\_object\_copy\_prop</a> | 拷贝指定的属性。 |
| <a href="#tk_object_t_tk_object_copy_props">tk\_object\_copy\_props</a> | 拷贝全部的属性。 |
| <a href="#tk_object_t_tk_object_create">tk\_object\_create</a> | 创建对象。 |
| <a href="#tk_object_t_tk_object_create_ex">tk\_object\_create\_ex</a> | 创建对象。 |
| <a href="#tk_object_t_tk_object_eval">tk\_object\_eval</a> | 计算一个表达式，表达式中引用的变量从prop中获取。 |
| <a href="#tk_object_t_tk_object_exec">tk\_object\_exec</a> | 执行指定的命令。 |
| <a href="#tk_object_t_tk_object_exec_by_path">tk\_object\_exec\_by\_path</a> | 执行指定的命令。 |
| <a href="#tk_object_t_tk_object_foreach_prop">tk\_object\_foreach\_prop</a> | 遍历所有属性。 |
| <a href="#tk_object_t_tk_object_get_child_object">tk\_object\_get\_child\_object</a> | 获取下一级子对象。 |
| <a href="#tk_object_t_tk_object_get_desc">tk\_object\_get\_desc</a> | 获取对象的描述信息。 |
| <a href="#tk_object_t_tk_object_get_prop">tk\_object\_get\_prop</a> | 获取指定属性的值。 |
| <a href="#tk_object_t_tk_object_get_prop_bool">tk\_object\_get\_prop\_bool</a> | 获取指定属性的bool类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_bool_by_path">tk\_object\_get\_prop\_bool\_by\_path</a> | 获取指定属性的bool类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_by_path">tk\_object\_get\_prop\_by\_path</a> | 获取指定path属性的值。 |
| <a href="#tk_object_t_tk_object_get_prop_double">tk\_object\_get\_prop\_double</a> | 获取指定属性的浮点数类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_float">tk\_object\_get\_prop\_float</a> | 获取指定属性的浮点数类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_float_by_path">tk\_object\_get\_prop\_float\_by\_path</a> | 获取指定属性的浮点数类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_int">tk\_object\_get\_prop\_int</a> | 获取指定属性的整数类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_int_by_path">tk\_object\_get\_prop\_int\_by\_path</a> | 获取指定属性的整数类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_int16">tk\_object\_get\_prop\_int16</a> | 获取指定属性的int16类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_int32">tk\_object\_get\_prop\_int32</a> | 获取指定属性的int32类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_int64">tk\_object\_get\_prop\_int64</a> | 获取指定属性的int64类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_int8">tk\_object\_get\_prop\_int8</a> | 获取指定属性的int8类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_object">tk\_object\_get\_prop\_object</a> | 获取指定属性的object类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_object_by_path">tk\_object\_get\_prop\_object\_by\_path</a> | 获取指定属性的object类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_pointer">tk\_object\_get\_prop\_pointer</a> | 获取指定属性的指针类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_pointer_by_path">tk\_object\_get\_prop\_pointer\_by\_path</a> | 获取指定属性的指针类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_str">tk\_object\_get\_prop\_str</a> | 获取指定属性的字符串类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_str_by_path">tk\_object\_get\_prop\_str\_by\_path</a> | 获取指定属性的字符串类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_uint16">tk\_object\_get\_prop\_uint16</a> | 获取指定属性的uint16类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_uint32">tk\_object\_get\_prop\_uint32</a> | 获取指定属性的uint32类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_uint64">tk\_object\_get\_prop\_uint64</a> | 获取指定属性的uint64类型的值。 |
| <a href="#tk_object_t_tk_object_get_prop_uint8">tk\_object\_get\_prop\_uint8</a> | 获取指定属性的uint8类型的值。 |
| <a href="#tk_object_t_tk_object_get_size">tk\_object\_get\_size</a> | 获取对象占用内存的大小。 |
| <a href="#tk_object_t_tk_object_get_type">tk\_object\_get\_type</a> | 获取对象的类型名称。 |
| <a href="#tk_object_t_tk_object_has_prop">tk\_object\_has\_prop</a> | 检查是否存在指定的属性。 |
| <a href="#tk_object_t_tk_object_has_prop_by_path">tk\_object\_has\_prop\_by\_path</a> | 检查是否存在指定的属性。 |
| <a href="#tk_object_t_tk_object_is_collection">tk\_object\_is\_collection</a> | 判断对象是否是集合。 |
| <a href="#tk_object_t_tk_object_notify_changed">tk\_object\_notify\_changed</a> | 触发EVT_PROPS_CHANGED事件。 |
| <a href="#tk_object_t_tk_object_ref">tk\_object\_ref</a> | 引用计数加1。 |
| <a href="#tk_object_t_tk_object_remove_prop">tk\_object\_remove\_prop</a> | 删除指定属性。 |
| <a href="#tk_object_t_tk_object_set_name">tk\_object\_set\_name</a> | 设置对象的名称。 |
| <a href="#tk_object_t_tk_object_set_prop">tk\_object\_set\_prop</a> | 设置指定属性的值。 |
| <a href="#tk_object_t_tk_object_set_prop_bool">tk\_object\_set\_prop\_bool</a> | 设置指定属性的bool类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_bool_by_path">tk\_object\_set\_prop\_bool\_by\_path</a> | 设置指定属性的bool类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_by_path">tk\_object\_set\_prop\_by\_path</a> | 设置指定属性的值。 |
| <a href="#tk_object_t_tk_object_set_prop_double">tk\_object\_set\_prop\_double</a> | 设置指定属性的浮点数类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_float">tk\_object\_set\_prop\_float</a> | 设置指定属性的浮点数类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_float_by_path">tk\_object\_set\_prop\_float\_by\_path</a> | 设置指定属性的浮点数类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_int">tk\_object\_set\_prop\_int</a> | 设置指定属性的整数类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_int_by_path">tk\_object\_set\_prop\_int\_by\_path</a> | 设置指定属性的整数类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_int16">tk\_object\_set\_prop\_int16</a> | 设置指定属性的int16类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_int32">tk\_object\_set\_prop\_int32</a> | 设置指定属性的int32类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_int64">tk\_object\_set\_prop\_int64</a> | 设置指定属性的int64类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_int8">tk\_object\_set\_prop\_int8</a> | 设置指定属性的int8类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_object">tk\_object\_set\_prop\_object</a> | 设置指定属性的object类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_object_by_path">tk\_object\_set\_prop\_object\_by\_path</a> | 设置指定属性的object类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_pointer">tk\_object\_set\_prop\_pointer</a> | 设置指定属性的指针类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_pointer_by_path">tk\_object\_set\_prop\_pointer\_by\_path</a> | 设置指定属性的指针类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_pointer_ex">tk\_object\_set\_prop\_pointer\_ex</a> | 设置指定属性的指针类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_str">tk\_object\_set\_prop\_str</a> | 设置指定属性的字符串类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_str_by_path">tk\_object\_set\_prop\_str\_by\_path</a> | 设置指定属性的字符串类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_uint16">tk\_object\_set\_prop\_uint16</a> | 设置指定属性的uint16类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_uint32">tk\_object\_set\_prop\_uint32</a> | 设置指定属性的uint32类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_uint64">tk\_object\_set\_prop\_uint64</a> | 设置指定属性的uint64类型的值。 |
| <a href="#tk_object_t_tk_object_set_prop_uint8">tk\_object\_set\_prop\_uint8</a> | 设置指定属性的uint8类型的值。 |
| <a href="#tk_object_t_tk_object_to_json">tk\_object\_to\_json</a> | 转换成JSON字符串。 |
| <a href="#tk_object_t_tk_object_unref">tk\_object\_unref</a> | 引用计数减1。引用计数为0时，销毁对象。 |
### 属性
<p id="tk_object_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#tk_object_t_name">name</a> | char* | 对象的名称。 |
| <a href="#tk_object_t_ref_count">ref\_count</a> | int32\_t | 引用计数。 |
#### tk\_object\_can\_exec 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_can_exec">检查是否可以执行指定的命令。

* 函数原型：

```
bool_t tk_object_can_exec (tk_object_t* obj, const char* name, const char* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示可以执行，否则表示不可以执行。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 命令的名称。 |
| args | const char* | 命令的参数。 |
#### tk\_object\_can\_exec\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_can_exec_by_path">检查是否可以执行指定的命令。

* 函数原型：

```
bool_t tk_object_can_exec_by_path (tk_object_t* obj, const char* path, const char* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示可以执行，否则表示不可以执行。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 命令的path。 |
| args | const char* | 命令的参数。 |
#### tk\_object\_clone 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_clone">clone对象。

* 函数原型：

```
tk_object_t* tk_object_clone (tk_object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回object对象。 |
| obj | tk\_object\_t* | object对象。 |
#### tk\_object\_compare 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_compare">比较两个对象。

* 函数原型：

```
int tk_object_compare (tk_object_t* obj, tk_object_t* other);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 返回比较结果。 |
| obj | tk\_object\_t* | object对象。 |
| other | tk\_object\_t* | 比较的object对象。 |
#### tk\_object\_copy\_prop 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_copy_prop">拷贝指定的属性。

* 函数原型：

```
ret_t tk_object_copy_prop (tk_object_t* obj, tk_object_t* src, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | 目标对象。 |
| src | tk\_object\_t* | 源对象。 |
| name | const char* | 属性的名称。 |
#### tk\_object\_copy\_props 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_copy_props">拷贝全部的属性。

* 函数原型：

```
ret_t tk_object_copy_props (tk_object_t* obj, tk_object_t* src, bool_t overwrite);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | 目标对象。 |
| src | tk\_object\_t* | 源对象。 |
| overwrite | bool\_t | 如果属性存在是否覆盖。 |
#### tk\_object\_create 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_create">创建对象。

> 仅供子类调用。

* 函数原型：

```
tk_object_t* tk_object_create (const object_vtable_t* vt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回object对象。 |
| vt | const object\_vtable\_t* | 虚函数表。 |
#### tk\_object\_create\_ex 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_create_ex">创建对象。

> 仅供子类调用。

* 函数原型：

```
tk_object_t* tk_object_create_ex (const object_vtable_t* vt, uint32_t extra_data_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回object对象。 |
| vt | const object\_vtable\_t* | 虚函数表。 |
| extra\_data\_size | uint32\_t | 额外的内存大小。 |
#### tk\_object\_eval 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_eval">计算一个表达式，表达式中引用的变量从prop中获取。

* 函数原型：

```
ret_t tk_object_eval (tk_object_t* obj, const char* expr, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| expr | const char* | 表达式。 |
| v | value\_t* | 返回计算结果。 |
#### tk\_object\_exec 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_exec">执行指定的命令。

* 函数原型：

```
ret_t tk_object_exec (tk_object_t* obj, const char* name, const char* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 命令的名称。 |
| args | const char* | 命令的参数。 |
#### tk\_object\_exec\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_exec_by_path">执行指定的命令。

* 函数原型：

```
ret_t tk_object_exec_by_path (tk_object_t* obj, const char* path, const char* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 命令的path。 |
| args | const char* | 命令的参数。 |
#### tk\_object\_foreach\_prop 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_foreach_prop">遍历所有属性。

* 函数原型：

```
ret_t tk_object_foreach_prop (tk_object_t* obj, tk_visit_t on_prop, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| on\_prop | tk\_visit\_t | 回调函数(data参数为named\_value\_t类型)。 |
| ctx | void* | 回调函数上下文。 |
#### tk\_object\_get\_child\_object 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_child_object">获取下一级子对象。
> 属性路径之间使用符号"."分割，例如：name.sub_name。

* 函数原型：

```
tk_object_t* tk_object_get_child_object (tk_object_t* obj, const char* path, const char** next_path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回下一级子对象，如果找不到则返回 NULL。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性路径。 |
| next\_path | const char** | 返回下一级属性路径。 |
#### tk\_object\_get\_desc 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_desc">获取对象的描述信息。

* 函数原型：

```
const char* tk_object_get_desc (tk_object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回对象的描述信息。 |
| obj | tk\_object\_t* | object对象。 |
#### tk\_object\_get\_prop 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop">获取指定属性的值。

* 函数原型：

```
ret_t tk_object_get_prop (tk_object_t* obj, const char* name, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| v | value\_t* | 返回属性的值。 |
#### tk\_object\_get\_prop\_bool 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_bool">获取指定属性的bool类型的值。

* 函数原型：

```
bool_t tk_object_get_prop_bool (tk_object_t* obj, const char* name, bool_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回指定属性的bool类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| defval | bool\_t | 缺省值。 |
#### tk\_object\_get\_prop\_bool\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_bool_by_path">获取指定属性的bool类型的值。

* 函数原型：

```
bool_t tk_object_get_prop_bool_by_path (tk_object_t* obj, const char* path, bool_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回指定属性的bool类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
| defval | bool\_t | 缺省值。 |
#### tk\_object\_get\_prop\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_by_path">获取指定path属性的值。

* 函数原型：

```
ret_t tk_object_get_prop_by_path (tk_object_t* obj, const char* path, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path，各级之间用.分隔。 |
| v | value\_t* | 返回属性的值。 |
#### tk\_object\_get\_prop\_double 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_double">获取指定属性的浮点数类型的值。

* 函数原型：

```
double tk_object_get_prop_double (tk_object_t* obj, const char* name, double defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | double | 返回指定属性的浮点数类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| defval | double | 缺省值。 |
#### tk\_object\_get\_prop\_float 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_float">获取指定属性的浮点数类型的值。

* 函数原型：

```
float_t tk_object_get_prop_float (tk_object_t* obj, const char* name, float_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float\_t | 返回指定属性的浮点数类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| defval | float\_t | 缺省值。 |
#### tk\_object\_get\_prop\_float\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_float_by_path">获取指定属性的浮点数类型的值。

e

* 函数原型：

```
float_t tk_object_get_prop_float_by_path (tk_object_t* obj, const char* path, float_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float\_t | 返回指定属性的浮点数类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
| defval | float\_t | 缺省值。 |
#### tk\_object\_get\_prop\_int 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_int">获取指定属性的整数类型的值。

* 函数原型：

```
int32_t tk_object_get_prop_int (tk_object_t* obj, const char* name, int32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回指定属性的整数类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| defval | int32\_t | 缺省值。 |
#### tk\_object\_get\_prop\_int\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_int_by_path">获取指定属性的整数类型的值。

* 函数原型：

```
int32_t tk_object_get_prop_int_by_path (tk_object_t* obj, const char* path, int32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回指定属性的整数类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
| defval | int32\_t | 缺省值。 |
#### tk\_object\_get\_prop\_int16 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_int16">获取指定属性的int16类型的值。

* 函数原型：

```
int16_t tk_object_get_prop_int16 (tk_object_t* obj, const char* name, int16_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int16\_t | 返回指定属性的int16类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| defval | int16\_t | 缺省值。 |
#### tk\_object\_get\_prop\_int32 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_int32">获取指定属性的int32类型的值。

* 函数原型：

```
int32_t tk_object_get_prop_int32 (tk_object_t* obj, const char* name, int32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回指定属性的int32类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| defval | int32\_t | 缺省值。 |
#### tk\_object\_get\_prop\_int64 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_int64">获取指定属性的int64类型的值。

* 函数原型：

```
int64_t tk_object_get_prop_int64 (tk_object_t* obj, const char* name, int64_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int64\_t | 返回指定属性的int64类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| defval | int64\_t | 缺省值。 |
#### tk\_object\_get\_prop\_int8 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_int8">获取指定属性的int8类型的值。

* 函数原型：

```
int8_t tk_object_get_prop_int8 (tk_object_t* obj, const char* name, int8_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int8\_t | 返回指定属性的int8类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| defval | int8\_t | 缺省值。 |
#### tk\_object\_get\_prop\_object 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_object">获取指定属性的object类型的值。

* 函数原型：

```
tk_object_t* tk_object_get_prop_object (tk_object_t* obj, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回指定属性的object类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
#### tk\_object\_get\_prop\_object\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_object_by_path">获取指定属性的object类型的值。

* 函数原型：

```
tk_object_t* tk_object_get_prop_object_by_path (tk_object_t* obj, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回指定属性的object类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
#### tk\_object\_get\_prop\_pointer 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_pointer">获取指定属性的指针类型的值。

* 函数原型：

```
void* tk_object_get_prop_pointer (tk_object_t* obj, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 返回指定属性的指针类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
#### tk\_object\_get\_prop\_pointer\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_pointer_by_path">获取指定属性的指针类型的值。

* 函数原型：

```
void* tk_object_get_prop_pointer_by_path (tk_object_t* obj, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 返回指定属性的指针类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
#### tk\_object\_get\_prop\_str 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_str">获取指定属性的字符串类型的值。

* 函数原型：

```
const char* tk_object_get_prop_str (tk_object_t* obj, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回指定属性的字符串类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
#### tk\_object\_get\_prop\_str\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_str_by_path">获取指定属性的字符串类型的值。

* 函数原型：

```
const char* tk_object_get_prop_str_by_path (tk_object_t* obj, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回指定属性的字符串类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
#### tk\_object\_get\_prop\_uint16 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_uint16">获取指定属性的uint16类型的值。

* 函数原型：

```
uint16_t tk_object_get_prop_uint16 (tk_object_t* obj, const char* name, uint16_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint16\_t | 返回指定属性的uint16类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| defval | uint16\_t | 缺省值。 |
#### tk\_object\_get\_prop\_uint32 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_uint32">获取指定属性的uint32类型的值。

* 函数原型：

```
uint32_t tk_object_get_prop_uint32 (tk_object_t* obj, const char* name, uint32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回指定属性的uint32类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| defval | uint32\_t | 缺省值。 |
#### tk\_object\_get\_prop\_uint64 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_uint64">获取指定属性的uint64类型的值。

* 函数原型：

```
uint64_t tk_object_get_prop_uint64 (tk_object_t* obj, const char* name, uint64_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint64\_t | 返回指定属性的uint64类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| defval | uint64\_t | 缺省值。 |
#### tk\_object\_get\_prop\_uint8 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_prop_uint8">获取指定属性的uint8类型的值。

* 函数原型：

```
uint8_t tk_object_get_prop_uint8 (tk_object_t* obj, const char* name, uint8_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint8\_t | 返回指定属性的uint8类型的值。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| defval | uint8\_t | 缺省值。 |
#### tk\_object\_get\_size 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_size">获取对象占用内存的大小。

* 函数原型：

```
uint32_t tk_object_get_size (tk_object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回对象占用内存的大小。 |
| obj | tk\_object\_t* | object对象。 |
#### tk\_object\_get\_type 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_get_type">获取对象的类型名称。

* 函数原型：

```
const char* tk_object_get_type (tk_object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回对象的类型名称。 |
| obj | tk\_object\_t* | object对象。 |
#### tk\_object\_has\_prop 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_has_prop">检查是否存在指定的属性。

* 函数原型：

```
bool_t tk_object_has_prop (tk_object_t* obj, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示存在，否则表示不存在。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
#### tk\_object\_has\_prop\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_has_prop_by_path">检查是否存在指定的属性。

* 函数原型：

```
bool_t tk_object_has_prop_by_path (tk_object_t* obj, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示存在，否则表示不存在。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path，各级之间用.分隔。 |
#### tk\_object\_is\_collection 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_is_collection">判断对象是否是集合。

* 函数原型：

```
bool_t tk_object_is_collection (tk_object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示是集合，否则不是。 |
| obj | tk\_object\_t* | object对象。 |
#### tk\_object\_notify\_changed 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_notify_changed">触发EVT_PROPS_CHANGED事件。

* 函数原型：

```
ret_t tk_object_notify_changed (tk_object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
#### tk\_object\_ref 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_ref">引用计数加1。

* 函数原型：

```
tk_object_t* tk_object_ref (tk_object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回object对象。 |
| obj | tk\_object\_t* | object对象。 |
#### tk\_object\_remove\_prop 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_remove_prop">删除指定属性。

* 函数原型：

```
ret_t tk_object_remove_prop (tk_object_t* obj, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
#### tk\_object\_set\_name 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_name">设置对象的名称。

* 函数原型：

```
ret_t tk_object_set_name (tk_object_t* obj, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 对象的名称。 |
#### tk\_object\_set\_prop 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop">设置指定属性的值。

* 函数原型：

```
ret_t tk_object_set_prop (tk_object_t* obj, const char* name, const value_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | const value\_t* | 属性的值。 |
#### tk\_object\_set\_prop\_bool 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_bool">设置指定属性的bool类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_bool (tk_object_t* obj, const char* name, bool_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | bool\_t | 属性的值。 |
#### tk\_object\_set\_prop\_bool\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_bool_by_path">设置指定属性的bool类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_bool_by_path (tk_object_t* obj, const char* path, bool_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
| value | bool\_t | 属性的值。 |
#### tk\_object\_set\_prop\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_by_path">设置指定属性的值。

* 函数原型：

```
ret_t tk_object_set_prop_by_path (tk_object_t* obj, const char* path, const value_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
| value | const value\_t* | 属性的值。 |
#### tk\_object\_set\_prop\_double 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_double">设置指定属性的浮点数类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_double (tk_object_t* obj, const char* name, double value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | double | 属性的值。 |
#### tk\_object\_set\_prop\_float 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_float">设置指定属性的浮点数类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_float (tk_object_t* obj, const char* name, float_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | float\_t | 属性的值。 |
#### tk\_object\_set\_prop\_float\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_float_by_path">设置指定属性的浮点数类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_float_by_path (tk_object_t* obj, const char* path, float_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
| value | float\_t | 属性的值。 |
#### tk\_object\_set\_prop\_int 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_int">设置指定属性的整数类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_int (tk_object_t* obj, const char* name, int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | int32\_t | 属性的值。 |
#### tk\_object\_set\_prop\_int\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_int_by_path">设置指定属性的整数类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_int_by_path (tk_object_t* obj, const char* path, int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
| value | int32\_t | 属性的值。 |
#### tk\_object\_set\_prop\_int16 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_int16">设置指定属性的int16类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_int16 (tk_object_t* obj, const char* name, int16_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | int16\_t | 属性的值。 |
#### tk\_object\_set\_prop\_int32 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_int32">设置指定属性的int32类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_int32 (tk_object_t* obj, const char* name, int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | int32\_t | 属性的值。 |
#### tk\_object\_set\_prop\_int64 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_int64">设置指定属性的int64类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_int64 (tk_object_t* obj, const char* name, int64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | int64\_t | 属性的值。 |
#### tk\_object\_set\_prop\_int8 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_int8">设置指定属性的int8类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_int8 (tk_object_t* obj, const char* name, int8_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | int8\_t | 属性的值。 |
#### tk\_object\_set\_prop\_object 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_object">设置指定属性的object类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_object (tk_object_t* obj, const char* name, tk_object_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | tk\_object\_t* | 属性的值。 |
#### tk\_object\_set\_prop\_object\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_object_by_path">设置指定属性的object类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_object_by_path (tk_object_t* obj, const char* path, tk_object_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
| value | tk\_object\_t* | 属性的值。 |
#### tk\_object\_set\_prop\_pointer 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_pointer">设置指定属性的指针类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_pointer (tk_object_t* obj, const char* name, void* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | void* | 属性的值。 |
#### tk\_object\_set\_prop\_pointer\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_pointer_by_path">设置指定属性的指针类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_pointer_by_path (tk_object_t* obj, const char* path, void* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
| value | void* | 属性的值。 |
#### tk\_object\_set\_prop\_pointer\_ex 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_pointer_ex">设置指定属性的指针类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_pointer_ex (tk_object_t* obj, const char* name, void* value, tk_destroy_t destroy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | void* | 属性的值。 |
| destroy | tk\_destroy\_t | 销毁函数。 |
#### tk\_object\_set\_prop\_str 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_str">设置指定属性的字符串类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_str (tk_object_t* obj, const char* name, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | const char* | 属性的值。 |
#### tk\_object\_set\_prop\_str\_by\_path 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_str_by_path">设置指定属性的字符串类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_str_by_path (tk_object_t* obj, const char* path, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| path | const char* | 属性的path。 |
| value | const char* | 属性的值。 |
#### tk\_object\_set\_prop\_uint16 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_uint16">设置指定属性的uint16类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_uint16 (tk_object_t* obj, const char* name, uint16_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | uint16\_t | 属性的值。 |
#### tk\_object\_set\_prop\_uint32 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_uint32">设置指定属性的uint32类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_uint32 (tk_object_t* obj, const char* name, uint32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | uint32\_t | 属性的值。 |
#### tk\_object\_set\_prop\_uint64 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_uint64">设置指定属性的uint64类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_uint64 (tk_object_t* obj, const char* name, uint64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | uint64\_t | 属性的值。 |
#### tk\_object\_set\_prop\_uint8 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_set_prop_uint8">设置指定属性的uint8类型的值。

* 函数原型：

```
ret_t tk_object_set_prop_uint8 (tk_object_t* obj, const char* name, uint8_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| name | const char* | 属性的名称。 |
| value | uint8\_t | 属性的值。 |
#### tk\_object\_to\_json 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_to_json">转换成JSON字符串。

* 函数原型：

```
ret_t tk_object_to_json (tk_object_t* obj, str_t* json, uint32_t indent, uint32_t level, bool_t oneline);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
| json | str\_t* | 返回JSON字符串。 |
| indent | uint32\_t | 缩进空格数。 |
| level | uint32\_t | 初始缩进层级。 |
| oneline | bool\_t | 是否不换行。 |
#### tk\_object\_unref 函数
-----------------------

* 函数功能：

> <p id="tk_object_t_tk_object_unref">引用计数减1。引用计数为0时，销毁对象。

* 函数原型：

```
ret_t tk_object_unref (tk_object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | object对象。 |
#### name 属性
-----------------------
> <p id="tk_object_t_name">对象的名称。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### ref\_count 属性
-----------------------
> <p id="tk_object_t_ref_count">引用计数。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
