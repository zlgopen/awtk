# 读写 XML/JSON/INI 和 UBJSON 等格式的数据文件

> 将抽象放进代码，细节放进元数据。

开发应用程序，会经常使用各种数据文件（如配置数据和元数据），常见的数据文件格式有 INI、XML、JSON 和 UBJSON，对一个复杂的应用程序，其中可能会同时使用多种不同格式的数据文件。

通常，操作这些数据文件的函数各不相同，对于程序员来说即是学习负担，也是记忆负担。AWTK 提供了一套统一的接口函数，同一套接口函数，可以操作不同的格式的数据文件。

## 1. 基本用法

加载数据文件成功后得到一个 object 的实例，可以用 object 的函数对其进行增加、修改、删除和查询。如果熟悉 object 的用法，那就非常简单了。

### 1.1 读取数据

> 下面的函数可以读取任意基本类型的数据。

```c
/**
 * @method tk_object_get_prop
 * 获取指定属性的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object 对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} v 返回属性的值。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t tk_object_get_prop(tk_object_t* obj, const char* name, value_t* v);
```

> 为了方便使用，也提供了常见数据类型的包装函数。如：

```c
/**
 * @method tk_object_get_prop_str
 * 获取指定属性的字符串类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object 对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {const char*} 返回指定属性的字符串类型的值。
 */
const char* tk_object_get_prop_str(tk_object_t* obj, const char* name);
```

### 1.2 增加/修改数据

> 下面的函数可以增加/修改任意基本类型的数据。

```c
/**
 * @method tk_object_set_prop
 * 设置指定属性的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object 对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} value 属性的值。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t tk_object_set_prop(tk_object_t* obj, const char* name, const value_t* value);

```

> 为了方便使用，也提供了常见数据类型的包装函数。如：

```c
/**
 * @method tk_object_set_prop_str
 * 设置指定属性的字符串类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object 对象。
 * @param {const char*} name 属性的名称。
 * @param {const char*} value 属性的值。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_str(tk_object_t* obj, const char* name, const char* value);
```

### 1.3 删除数据

```c
/**
 * @method tk_object_remove_prop
 * 删除指定属性。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object 对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t tk_object_remove_prop(tk_object_t* obj, const char* name);
```

### 1.4 path 的基本格式

* 配置信息通常都是层次结构的，为了能访问所有数据，path 也必须是多级的，各级的名称之间用“.”分隔。

> “.”不能作为名称的一部分，如果出现，需要用其它字符代替。

```json
{
    "network" : { 
        "eth0" : { 
            "ip" : "192.169.0.100",
            "gateway" : "192.169.0.1",
            "mask" : "255.255.255.0"
        }   
    }   
}
```

在这个例子中，要访问 ip 的这个配置，可以用"network.eth0.ip":

* 按索引访问

在访问时，每一级的名称，可以用索引代替，格式为：[+索引+]，比如 [0] 表示第一项，[2] 表示第三项。这对于不清楚名称的情况，或遍历
全部配置，很有帮助。

* 用 #size 获取子节点个数

示例：

```ini
[wifi]
  on = 1
  name = awtk
[server]
  port = 8080
  timeout = 1.000000

```

在上面的例子中，"#size"获取分组的个数为 2，"wifi.#size"获取 wifi 下的子项数为 2。

* 用 #name 获取项的名称。

示例：

```ini
[wifi]
  on = 1
  name = awtk
[server]
  port = 8080
  timeout = 1.000000

```

在上面的例子中，"[0].#name" 获取第一个分组的名称为"wifi"，"wifi.[0].#name" 获取 wifi 下第一项的名称为 "on"。

* 用 #index 获取配置项在兄弟节点中的序数。

```ini
[wifi]
  on = 1
  name = awtk
[server]
  port = 8080
  timeout = 1.000000

```
在上面的例子中，"wifi.#index" 获取 wifi 的序数为 0，"server.#index"  获取 server 的序数为 1。

## 2. 高级用法

上面的函数使用简单，但是在数据量大的时候，不如直接去操作 node 效率更高，如果有此需求可以查看 src/conf_io/conf_node.h 中函数，以及单元测试中的用法，这里不再赘述。

## 3. 打开和保存

>此时需要使用具体格式的函数

### 3.1 JSON 格式

* 打开

```c
/**
 * @method conf_json_load 
 * 从指定 URL 加载 JSON 对象。 
 * @annotation ["constructor"]
 * 
 * @param {const char*} url 路径（通常是文件路径）。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_json_load(const char* url, bool_t create_if_not_exist);
```

* 保存

```c
/**
 * @method conf_json_save_as
 * 将 doc 对象保存到指定 URL。
 * @annotation ["static"]
 * 
 * @param {tk_object_t*} obj doc 对象。
 * @param {const char*} url 保存的位置。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败
 */
ret_t conf_json_save_as(tk_object_t* obj, const char* url);
```

### 3.2 XML 格式

* 打开

```c
/**
 * @method conf_xml_load 
 * 从指定 URL 加载 XML 对象。 
 * 
 * @annotation ["constructor"]
 * 
 * @param {const char*} url 路径（通常是文件路径）。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_xml_load(const char* url, bool_t create_if_not_exist);
```

* 保存

```c
/**
 * @method conf_xml_save_as
 * 将 doc 对象保存到指定 URL。
 * @annotation ["static"]
 * 
 * @param {tk_object_t*} obj doc 对象。
 * @param {const char*} url 保存的位置。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败
 */
ret_t conf_xml_save_as(tk_object_t* obj, const char* url);
```

### 3.3 INI 格式

* 打开

```c
/**
 * @method conf_ini_load 
 * 从指定 URL 加载 INI 对象。 
 * 
 * @annotation ["constructor"]
 * 
 * @param {const char*} url 路径（通常是文件路径）。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_ini_load(const char* url, bool_t create_if_not_exist);
```

* 保存

```c
/**
 * @method conf_ini_save_as
 * 将 doc 对象保存到指定 URL。
 * @annotation ["static"]
 * 
 * @param {tk_object_t*} obj doc 对象。
 * @param {const char*} url 保存的位置。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败
 */
ret_t conf_ini_save_as(tk_object_t* obj, const char* url);

```

### 3.4 UBJSON 格式

* 打开

```c
/**
 * @method conf_ubjson_load 
 * 从指定 URL 加载 UBJSON 对象。 
 * 
 * @annotation ["constructor"]
 * 
 * @param {const char*} url 路径（通常是文件路径）。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_ubjson_load(const char* url, bool_t create_if_not_exist);

* 保存

```c
/**
 * @method conf_ubjson_save_as
 * 将 doc 对象保存到指定 URL。
 * @annotation ["static"]
 * 
 * @param {tk_object_t*} obj doc 对象。
 * @param {const char*} url 保存的位置。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败
 */
ret_t conf_ubjson_save_as(tk_object_t* obj, const char* url);
```

## 完整示例

* 文件读写

```c
#include "conf_io/conf_json.h"

void demo_conf_json(c) {
  tk_object_t* doc = conf_json_load("./test.json", TRUE);
  tk_object_set_prop_str(doc, "name", "awtk");
  tk_object_set_prop_int(doc, "age", 100);
  conf_json_save_as(doc, "./test.json");
  TK_OBJECT_UNREF(doc);
  
  doc = conf_json_load("./test.json", TRUE);

  log_debug("name:%s\n", tk_object_get_prop_str(doc, "name"));
  log_debug("age:%d\n", tk_object_get_prop_int(doc, "age", 0));
  TK_OBJECT_UNREF(doc);
}
```

对应的数据文件为：

```json
{
    "name" : "awtk",
    "age" : 100 
}
```

* 内存数据读写

```c
  wbuffer_t wb;
  tk_object_t* conf = conf_xml_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_set_prop_int(conf, "awtk.value", 123), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(conf, "awtk.value", 0), 123);

  ASSERT_EQ(conf_xml_save_to_buff(conf, &wb), RET_OK);
  TK_OBJECT_UNREF(conf);

  conf = conf_xml_load_from_buff(wb.data, wb.cursor, FALSE);
  ASSERT_EQ(tk_object_get_prop_int(conf, "awtk.value", 0), 123);
  TK_OBJECT_UNREF(conf);

  wbuffer_deinit(&wb);
```  
