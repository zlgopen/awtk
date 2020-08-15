## conf\_doc\_t
### 概述
代表整个配置文件。
----------------------------------
### 函数
<p id="conf_doc_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#conf_doc_t_conf_doc_add_child">conf\_doc\_add\_child</a> | 追加一个子节点。 |
| <a href="#conf_doc_t_conf_doc_append_child">conf\_doc\_append\_child</a> | 追加一个子节点。 |
| <a href="#conf_doc_t_conf_doc_append_sibling">conf\_doc\_append\_sibling</a> | 追加一个兄弟节点。 |
| <a href="#conf_doc_t_conf_doc_clear">conf\_doc\_clear</a> | 删除指定路径的节点的全部子节点。 |
| <a href="#conf_doc_t_conf_doc_create">conf\_doc\_create</a> | 构造函数。 |
| <a href="#conf_doc_t_conf_doc_create_node">conf\_doc\_create\_node</a> | 创建一个空节点。 |
| <a href="#conf_doc_t_conf_doc_destroy">conf\_doc\_destroy</a> | 析构函数。 |
| <a href="#conf_doc_t_conf_doc_destroy_node">conf\_doc\_destroy\_node</a> | 销毁节点对象。 |
| <a href="#conf_doc_t_conf_doc_exists">conf\_doc\_exists</a> | 判断指定路径的节点是否存在。 |
| <a href="#conf_doc_t_conf_doc_get">conf\_doc\_get</a> | 获取指定路径节点的值。 |
| <a href="#conf_doc_t_conf_doc_get_bool">conf\_doc\_get\_bool</a> | 获取指定路径节点的值。 |
| <a href="#conf_doc_t_conf_doc_get_float">conf\_doc\_get\_float</a> | 获取指定路径节点的值。 |
| <a href="#conf_doc_t_conf_doc_get_int">conf\_doc\_get\_int</a> | 获取指定路径节点的值。 |
| <a href="#conf_doc_t_conf_doc_get_str">conf\_doc\_get\_str</a> | 获取指定路径节点的值。 |
| <a href="#conf_doc_t_conf_doc_is_first">conf\_doc\_is\_first</a> | 检查指定节点是否在兄弟节点中排第一。 |
| <a href="#conf_doc_t_conf_doc_is_last">conf\_doc\_is\_last</a> | 检查指定节点是否在兄弟节点中排最后。 |
| <a href="#conf_doc_t_conf_doc_move_down">conf\_doc\_move\_down</a> | 和后一个兄弟节点交换位置。 |
| <a href="#conf_doc_t_conf_doc_move_up">conf\_doc\_move\_up</a> | 和前一个兄弟节点交换位置。 |
| <a href="#conf_doc_t_conf_doc_remove">conf\_doc\_remove</a> | 删除指定路径的节点。 |
| <a href="#conf_doc_t_conf_doc_remove_child">conf\_doc\_remove\_child</a> | 删除指定的子节点。 |
| <a href="#conf_doc_t_conf_doc_remove_child_by_name">conf\_doc\_remove\_child\_by\_name</a> | 删除指定的子节点。 |
| <a href="#conf_doc_t_conf_doc_remove_children">conf\_doc\_remove\_children</a> | 删除全部子节点。 |
| <a href="#conf_doc_t_conf_doc_remove_sibling">conf\_doc\_remove\_sibling</a> | 删除指定的兄弟节点。 |
| <a href="#conf_doc_t_conf_doc_set">conf\_doc\_set</a> | 设置指定路径节点的值。 |
| <a href="#conf_doc_t_conf_doc_set_bool">conf\_doc\_set\_bool</a> | 设置指定路径节点的值。 |
| <a href="#conf_doc_t_conf_doc_set_float">conf\_doc\_set\_float</a> | 设置指定路径节点的值。 |
| <a href="#conf_doc_t_conf_doc_set_int">conf\_doc\_set\_int</a> | 设置指定路径节点的值。 |
| <a href="#conf_doc_t_conf_doc_set_str">conf\_doc\_set\_str</a> | 设置指定路径节点的值。 |
### 属性
<p id="conf_doc_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#conf_doc_t_root">root</a> | conf\_node\_t* | 根节点。 |
#### conf\_doc\_add\_child 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_add_child">追加一个子节点。

* 函数原型：

```
ret_t conf_doc_add_child (conf_doc_t* doc, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
#### conf\_doc\_append\_child 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_append_child">追加一个子节点。

* 函数原型：

```
ret_t conf_doc_append_child (conf_doc_t* doc, conf_node_t* node, conf_node_t* child);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| node | conf\_node\_t* | 节点对象。 |
| child | conf\_node\_t* | 待追加节点对象。 |
#### conf\_doc\_append\_sibling 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_append_sibling">追加一个兄弟节点。

* 函数原型：

```
ret_t conf_doc_append_sibling (conf_doc_t* doc, conf_node_t* node, conf_node_t* sibling);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| node | conf\_node\_t* | 节点对象。 |
| sibling | conf\_node\_t* | 待追加节点对象。 |
#### conf\_doc\_clear 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_clear">删除指定路径的节点的全部子节点。

* 函数原型：

```
ret_t conf_doc_clear (conf_doc_t* doc, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
#### conf\_doc\_create 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_create">构造函数。

* 函数原型：

```
conf_doc_t* conf_doc_create (uint32_t prealloc_nodes_nr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | conf\_doc\_t* | 返回doc对象。 |
| prealloc\_nodes\_nr | uint32\_t | 预先分配的节点数。 |
#### conf\_doc\_create\_node 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_create_node">创建一个空节点。

* 函数原型：

```
conf_node_t* conf_doc_create_node (conf_doc_t* doc, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | conf\_node\_t* | 返回节点对象。 |
| doc | conf\_doc\_t* | 文档对象。 |
| name | const char* | 节点名称。 |
#### conf\_doc\_destroy 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_destroy">析构函数。

* 函数原型：

```
ret_t conf_doc_destroy (conf_doc_t* doc);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
#### conf\_doc\_destroy\_node 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_destroy_node">销毁节点对象。

* 函数原型：

```
ret_t conf_doc_destroy_node (conf_doc_t* doc, conf_node_t* node);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| node | conf\_node\_t* | 节点对象。 |
#### conf\_doc\_exists 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_exists">判断指定路径的节点是否存在。

* 函数原型：

```
bool_t conf_doc_exists (conf_doc_t* doc, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示成功，FALSE表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
#### conf\_doc\_get 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_get">获取指定路径节点的值。

* 函数原型：

```
ret_t conf_doc_get (conf_doc_t* doc, const char* path, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
| v | value\_t* | 用于返回值。 |
#### conf\_doc\_get\_bool 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_get_bool">获取指定路径节点的值。

* 函数原型：

```
bool_t conf_doc_get_bool (conf_doc_t* doc, const char* path, bool_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回值。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
| defval | bool\_t | 缺省值。 |
#### conf\_doc\_get\_float 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_get_float">获取指定路径节点的值。

* 函数原型：

```
float conf_doc_get_float (conf_doc_t* doc, const char* path, float defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float | 返回值。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
| defval | float | 缺省值。 |
#### conf\_doc\_get\_int 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_get_int">获取指定路径节点的值。

* 函数原型：

```
int32_t conf_doc_get_int (conf_doc_t* doc, const char* path, int32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回值。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
| defval | int32\_t | 缺省值。 |
#### conf\_doc\_get\_str 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_get_str">获取指定路径节点的值。

* 函数原型：

```
const char* conf_doc_get_str (conf_doc_t* doc, const char* path, const char* defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回值。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
| defval | const char* | 缺省值。 |
#### conf\_doc\_is\_first 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_is_first">检查指定节点是否在兄弟节点中排第一。

* 函数原型：

```
bool_t conf_doc_is_first (conf_doc_t* doc, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示是，否则表示不是。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
#### conf\_doc\_is\_last 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_is_last">检查指定节点是否在兄弟节点中排最后。

* 函数原型：

```
bool_t conf_doc_is_last (conf_doc_t* doc, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示是，否则表示不是。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
#### conf\_doc\_move\_down 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_move_down">和后一个兄弟节点交换位置。

* 函数原型：

```
ret_t conf_doc_move_down (conf_doc_t* doc, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
#### conf\_doc\_move\_up 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_move_up">和前一个兄弟节点交换位置。

* 函数原型：

```
ret_t conf_doc_move_up (conf_doc_t* doc, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
#### conf\_doc\_remove 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_remove">删除指定路径的节点。

* 函数原型：

```
ret_t conf_doc_remove (conf_doc_t* doc, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
#### conf\_doc\_remove\_child 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_remove_child">删除指定的子节点。

* 函数原型：

```
ret_t conf_doc_remove_child (conf_doc_t* doc, conf_node_t* node, conf_node_t* child);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| node | conf\_node\_t* | 节点对象。 |
| child | conf\_node\_t* | 待删除节点对象。 |
#### conf\_doc\_remove\_child\_by\_name 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_remove_child_by_name">删除指定的子节点。

* 函数原型：

```
ret_t conf_doc_remove_child_by_name (conf_doc_t* doc, conf_node_t* node, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| node | conf\_node\_t* | 节点对象。 |
| name | const char* | 待删除节点的名称。 |
#### conf\_doc\_remove\_children 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_remove_children">删除全部子节点。

* 函数原型：

```
ret_t conf_doc_remove_children (conf_doc_t* doc, conf_node_t* node);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| node | conf\_node\_t* | 节点对象。 |
#### conf\_doc\_remove\_sibling 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_remove_sibling">删除指定的兄弟节点。

* 函数原型：

```
ret_t conf_doc_remove_sibling (conf_doc_t* doc, conf_node_t* node, conf_node_t* sibling);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| node | conf\_node\_t* | 节点对象。 |
| sibling | conf\_node\_t* | 待删除节点对象。 |
#### conf\_doc\_set 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_set">设置指定路径节点的值。

* 函数原型：

```
ret_t conf_doc_set (conf_doc_t* doc, const char* path, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
| v | const value\_t* | 值。 |
#### conf\_doc\_set\_bool 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_set_bool">设置指定路径节点的值。

* 函数原型：

```
ret_t conf_doc_set_bool (conf_doc_t* doc, const char* path, bool_t v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
| v | bool\_t | 值。 |
#### conf\_doc\_set\_float 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_set_float">设置指定路径节点的值。

* 函数原型：

```
ret_t conf_doc_set_float (conf_doc_t* doc, const char* path, float v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
| v | float | 值。 |
#### conf\_doc\_set\_int 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_set_int">设置指定路径节点的值。

* 函数原型：

```
ret_t conf_doc_set_int (conf_doc_t* doc, const char* path, int32_t v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
| v | int32\_t | 值。 |
#### conf\_doc\_set\_str 函数
-----------------------

* 函数功能：

> <p id="conf_doc_t_conf_doc_set_str">设置指定路径节点的值。

* 函数原型：

```
ret_t conf_doc_set_str (conf_doc_t* doc, const char* path, const char* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| doc | conf\_doc\_t* | 文档对象。 |
| path | const char* | 节点的路径。 |
| v | const char* | 值。 |
#### root 属性
-----------------------
> <p id="conf_doc_t_root">根节点。

* 类型：conf\_node\_t*

