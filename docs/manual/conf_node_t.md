## conf\_node\_t
### 概述
节点。
----------------------------------
### 函数
<p id="conf_node_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#conf_node_t_conf_doc_foreach">conf\_doc\_foreach</a> | 遍历节点类型为 CONF_NODE_SIMPLE 的节点。 |
| <a href="#conf_node_t_conf_node_count_children">conf\_node\_count\_children</a> | 获取节点的子集个数。 |
| <a href="#conf_node_t_conf_node_find_child">conf\_node\_find\_child</a> | 查找指定名称的子节点。 |
| <a href="#conf_node_t_conf_node_find_child_by_index">conf\_node\_find\_child\_by\_index</a> | 查找指定索引的子节点。 |
| <a href="#conf_node_t_conf_node_find_sibling">conf\_node\_find\_sibling</a> | 查找指定名称的兄弟节点。 |
| <a href="#conf_node_t_conf_node_get_child_value">conf\_node\_get\_child\_value</a> | 获取子节点的值。 |
| <a href="#conf_node_t_conf_node_get_child_value_bool">conf\_node\_get\_child\_value\_bool</a> | 获取子节点的值。 |
| <a href="#conf_node_t_conf_node_get_child_value_by_index">conf\_node\_get\_child\_value\_by\_index</a> | 获取子节点的值。 |
| <a href="#conf_node_t_conf_node_get_child_value_double">conf\_node\_get\_child\_value\_double</a> | 获取子节点的值。 |
| <a href="#conf_node_t_conf_node_get_child_value_float">conf\_node\_get\_child\_value\_float</a> | 获取子节点的值。 |
| <a href="#conf_node_t_conf_node_get_child_value_int32">conf\_node\_get\_child\_value\_int32</a> | 获取子节点的值。 |
| <a href="#conf_node_t_conf_node_get_child_value_str">conf\_node\_get\_child\_value\_str</a> | 获取子节点的值。 |
| <a href="#conf_node_t_conf_node_get_child_value_uint32">conf\_node\_get\_child\_value\_uint32</a> | 获取子节点的值。 |
| <a href="#conf_node_t_conf_node_get_first_child">conf\_node\_get\_first\_child</a> | 获取第一个子节点。 |
| <a href="#conf_node_t_conf_node_get_name">conf\_node\_get\_name</a> | 获得节点的名称。 |
| <a href="#conf_node_t_conf_node_get_value">conf\_node\_get\_value</a> | 获取节点的值。 |
| <a href="#conf_node_t_conf_node_get_value_bool">conf\_node\_get\_value\_bool</a> | 获取节点的值。 |
| <a href="#conf_node_t_conf_node_get_value_double">conf\_node\_get\_value\_double</a> | 获取节点的值。 |
| <a href="#conf_node_t_conf_node_get_value_float">conf\_node\_get\_value\_float</a> | 获取节点的值。 |
| <a href="#conf_node_t_conf_node_get_value_int32">conf\_node\_get\_value\_int32</a> | 获取节点的值。 |
| <a href="#conf_node_t_conf_node_get_value_str">conf\_node\_get\_value\_str</a> | 获取节点的值。 |
| <a href="#conf_node_t_conf_node_get_value_uint32">conf\_node\_get\_value\_uint32</a> | 获取节点的值。 |
| <a href="#conf_node_t_conf_node_set_first_child">conf\_node\_set\_first\_child</a> | 设置第一个子节点。 |
| <a href="#conf_node_t_conf_node_set_value">conf\_node\_set\_value</a> | 设置节点的值。 |
### 属性
<p id="conf_node_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#conf_node_t_next">next</a> | conf\_node\_t* | 下一个兄弟节点。 |
| <a href="#conf_node_t_node_type">node\_type</a> | conf\_node\_type\_t | 节点类型。 |
| <a href="#conf_node_t_parent">parent</a> | conf\_node\_t* | 父节点。 |
| <a href="#conf_node_t_value_type">value\_type</a> | conf\_node\_value\_t | 值的类型。 |
#### conf\_doc\_foreach 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_doc_foreach">遍历节点类型为 CONF_NODE_SIMPLE 的节点。

* 函数原型：

```
ret_t conf_doc_foreach (conf_doc_t* doc, conf_doc_on_visit_t on_visit, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回 ret\_t 值 |
| doc | conf\_doc\_t* | 文档对象。 |
| on\_visit | conf\_doc\_on\_visit\_t | 回调。 |
| ctx | void* | 回调参数。 |
#### conf\_node\_count\_children 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_count_children">获取节点的子集个数。

* 函数原型：

```
uint32_t conf_node_count_children (conf_node_t* node);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 成功返回节点个数。 |
| node | conf\_node\_t* | 节点对象。 |
#### conf\_node\_find\_child 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_find_child">查找指定名称的子节点。

* 函数原型：

```
conf_node_t* conf_node_find_child (conf_node_t* node, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | conf\_node\_t* | 返回节点对象。 |
| node | conf\_node\_t* | 节点对象。 |
| name | const char* | 节点名称。 |
#### conf\_node\_find\_child\_by\_index 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_find_child_by_index">查找指定索引的子节点。

* 函数原型：

```
conf_node_t* conf_node_find_child_by_index (conf_node_t* node, int32_t index);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | conf\_node\_t* | 返回节点对象。 |
| node | conf\_node\_t* | 节点对象。 |
| index | int32\_t | 索引。 |
#### conf\_node\_find\_sibling 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_find_sibling">查找指定名称的兄弟节点。

* 函数原型：

```
conf_node_t* conf_node_find_sibling (conf_node_t* node, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | conf\_node\_t* | 返回节点对象。 |
| node | conf\_node\_t* | 节点对象。 |
| name | const char* | 节点名称。 |
#### conf\_node\_get\_child\_value 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_child_value">获取子节点的值。

* 函数原型：

```
ret_t conf_node_get_child_value (conf_node_t* node, const char* name, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| node | conf\_node\_t* | 节点对象。 |
| name | const char* | 子节点名称。 |
| v | value\_t* | 值(返回)。 |
#### conf\_node\_get\_child\_value\_bool 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_child_value_bool">获取子节点的值。

* 函数原型：

```
bool_t conf_node_get_child_value_bool (conf_node_t* node, const char* name, bool_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回值。 |
| node | conf\_node\_t* | 节点对象。 |
| name | const char* | 子节点名称。 |
| defval | bool\_t | 默认值。 |
#### conf\_node\_get\_child\_value\_by\_index 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_child_value_by_index">获取子节点的值。

* 函数原型：

```
ret_t conf_node_get_child_value_by_index (conf_node_t* node, uint32_t index, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| node | conf\_node\_t* | 节点对象。 |
| index | uint32\_t | 子节点的序数。 |
| v | value\_t* | 值(返回)。 |
#### conf\_node\_get\_child\_value\_double 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_child_value_double">获取子节点的值。

* 函数原型：

```
double conf_node_get_child_value_double (conf_node_t* node, const char* name, double defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | double | 返回值。 |
| node | conf\_node\_t* | 节点对象。 |
| name | const char* | 子节点名称。 |
| defval | double | 默认值。 |
#### conf\_node\_get\_child\_value\_float 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_child_value_float">获取子节点的值。

* 函数原型：

```
float conf_node_get_child_value_float (conf_node_t* node, const char* name, float defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float | 返回值。 |
| node | conf\_node\_t* | 节点对象。 |
| name | const char* | 子节点名称。 |
| defval | float | 默认值。 |
#### conf\_node\_get\_child\_value\_int32 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_child_value_int32">获取子节点的值。

* 函数原型：

```
int32_t conf_node_get_child_value_int32 (conf_node_t* node, const char* name, int32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回值。 |
| node | conf\_node\_t* | 节点对象。 |
| name | const char* | 子节点名称。 |
| defval | int32\_t | 默认值。 |
#### conf\_node\_get\_child\_value\_str 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_child_value_str">获取子节点的值。

* 函数原型：

```
const char* conf_node_get_child_value_str (conf_node_t* node, const char* name, const char* defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回值。 |
| node | conf\_node\_t* | 节点对象。 |
| name | const char* | 子节点名称。 |
| defval | const char* | 默认值。 |
#### conf\_node\_get\_child\_value\_uint32 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_child_value_uint32">获取子节点的值。

* 函数原型：

```
uint32_t conf_node_get_child_value_uint32 (conf_node_t* node, const char* name, uint32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回值。 |
| node | conf\_node\_t* | 节点对象。 |
| name | const char* | 子节点名称。 |
| defval | uint32\_t | 默认值。 |
#### conf\_node\_get\_first\_child 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_first_child">获取第一个子节点。

* 函数原型：

```
conf_node_t* conf_node_get_first_child (conf_node_t* node);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | conf\_node\_t* | 返回第一个子节点。 |
| node | conf\_node\_t* | 节点对象。 |
#### conf\_node\_get\_name 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_name">获得节点的名称。

* 函数原型：

```
const char* conf_node_get_name (conf_node_t* node);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回节点名称。 |
| node | conf\_node\_t* | 节点对象。 |
#### conf\_node\_get\_value 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_value">获取节点的值。

* 函数原型：

```
ret_t conf_node_get_value (conf_node_t* node, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| node | conf\_node\_t* | 节点对象。 |
| v | value\_t* | 值(返回)。 |
#### conf\_node\_get\_value\_bool 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_value_bool">获取节点的值。

* 函数原型：

```
bool_t conf_node_get_value_bool (conf_node_t* node, bool_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回值。 |
| node | conf\_node\_t* | 节点对象。 |
| defval | bool\_t | 默认值。 |
#### conf\_node\_get\_value\_double 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_value_double">获取节点的值。

* 函数原型：

```
double conf_node_get_value_double (conf_node_t* node, double defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | double | 返回值。 |
| node | conf\_node\_t* | 节点对象。 |
| defval | double | 默认值。 |
#### conf\_node\_get\_value\_float 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_value_float">获取节点的值。

* 函数原型：

```
float conf_node_get_value_float (conf_node_t* node, float defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float | 返回值。 |
| node | conf\_node\_t* | 节点对象。 |
| defval | float | 默认值。 |
#### conf\_node\_get\_value\_int32 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_value_int32">获取节点的值。

* 函数原型：

```
int32_t conf_node_get_value_int32 (conf_node_t* node, int32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回值。 |
| node | conf\_node\_t* | 节点对象。 |
| defval | int32\_t | 默认值。 |
#### conf\_node\_get\_value\_str 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_value_str">获取节点的值。

* 函数原型：

```
const char* conf_node_get_value_str (conf_node_t* node, const char* defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回值。 |
| node | conf\_node\_t* | 节点对象。 |
| defval | const char* | 默认值。 |
#### conf\_node\_get\_value\_uint32 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_get_value_uint32">获取节点的值。

* 函数原型：

```
uint32_t conf_node_get_value_uint32 (conf_node_t* node, uint32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回值。 |
| node | conf\_node\_t* | 节点对象。 |
| defval | uint32\_t | 默认值。 |
#### conf\_node\_set\_first\_child 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_set_first_child">设置第一个子节点。

* 函数原型：

```
ret_t conf_node_set_first_child (conf_node_t* node, conf_node_t* child);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| node | conf\_node\_t* | 节点对象。 |
| child | conf\_node\_t* | 子节点对象。 |
#### conf\_node\_set\_value 函数
-----------------------

* 函数功能：

> <p id="conf_node_t_conf_node_set_value">设置节点的值。

* 函数原型：

```
ret_t conf_node_set_value (conf_node_t* node, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| node | conf\_node\_t* | 节点对象。 |
| v | const value\_t* | 值。 |
#### next 属性
-----------------------
> <p id="conf_node_t_next">下一个兄弟节点。

* 类型：conf\_node\_t*

#### node\_type 属性
-----------------------
> <p id="conf_node_t_node_type">节点类型。

* 类型：conf\_node\_type\_t

#### parent 属性
-----------------------
> <p id="conf_node_t_parent">父节点。

* 类型：conf\_node\_t*

#### value\_type 属性
-----------------------
> <p id="conf_node_t_value_type">值的类型。

* 类型：conf\_node\_value\_t

