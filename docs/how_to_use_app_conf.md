
# 如何存取应用程序的配置信息

## 1. 介绍

AWTK 提供了 [app_conf](https://github.com/zlgopen/awtk/blob/master/docs/manual/app_conf_t.md) 接口，用于保存或读取应用程序的配置信息。其特点有：

* 线程安全，可以多线程访问。
* 观察者模式，配置变化时通知关注者。
* 统一接口，不用关注存储格式和存储介质。
* 接口支持多种数据类型，开发者无需在字符串和其它类型之间转换。

配置信息可以保存为任意格式，目前支持下列文件格式（其它格式开发者可以自行增加）:

* [INI](https://zh.wikipedia.org/wiki/INI)

* [JSON](https://zh.wikipedia.org/wiki/JSON)

* [UBJSON](https://en.wikipedia.org/wiki/UBJSON)

配置信息可以存放在任何介质上，目前支持以下介质：

* 文件系统。

* Flash(TODO)

## 2. 初始化（任意选一种格式即可）

[app_conf](https://github.com/zlgopen/awtk/blob/master/docs/manual/app_conf_t.md) 作为可选组件，需要开发者自己初始化。

> 初始化函数一般在 application_init 函数中调用。所有读写等函数，必须在成功初始化之后才能调用。

配置文件通常保存在用户目录下，比如应用程序的名称为demo，存储格式为json，那么在不同的平台下，存放的位置为：

* linux

```
～/demo/app_conf.json
```

* macos

```
～/demo/app_conf.json
```

* windows

```
～/AppData/Local/demo/app_conf.json
```

* 嵌入式

```
/appdata/demo/app_conf.json
```

> 应用程序在启动时，会打印配置文件的位置，以实际位置为准。

* 2.1 使用 ini 格式时的初始方法

包含头文件：

```c
#include "conf_io/app_conf_init_ini.h"
```

调用初始化函数

```c
/**
 * @method app_conf_init_ini
 *
 * 初始化应用程序的配置信息。
 *
 * @param {const char*} app_name 应用程序名称。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_init_ini(const char* app_name)
```

示例：

```c
  ENSURE(app_conf_init_ini("demo") == RET_OK);
```

* 2.2 使用 json 格式时的初始方法

包含头文件：

```c
#include "conf_io/app_conf_init_json.h"
```

调用初始化函数

```c
/**
 * @method app_conf_init_json
 *
 * 初始化应用程序的配置信息。
 *
 * @param {const char*} app_name 应用程序名称。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_init_json(const char* app_name)
```

示例：

```c
  ENSURE(app_conf_init_json("demo") == RET_OK);
```

* 2.3 使用 ubjson 格式时的初始方法

包含头文件：

```c
#include "conf_io/app_conf_init_ubjson.h"
```

调用初始化函数

```c
/**
 * @method app_conf_init_ubjson
 *
 * 初始化应用程序的配置信息。
 *
 * @param {const char*} app_name 应用程序名称。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_init_ubjson(const char* app_name)
```

示例：

```c
  ENSURE(app_conf_init_ubjson("demo") == RET_OK);
```

## 3. 存取配置信息

### 3.1 包含头文件：

app\_conf 作为可选组件，需要开发者自己包含头文件。

```c
#include "conf_io/app_conf.h"
```

### 3.2 修改配置信息

```c
/**
 * @method app_conf_set_int
 * 设置整数类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {int32_t} v 配置项的值。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_set_int(const char* key, int32_t v);

/**
 * @method app_conf_set_int64
 * 设置 64 位整数类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {int64_t} v 配置项的值。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_set_int64(const char* key, int64_t v);

/**
 * @method app_conf_set_bool
 * 设置 bool 类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {bool_t} v 配置项的值。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_set_bool(const char* key, bool_t v);

/**
 * @method app_conf_set_double
 * 设置双精度类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {double} v 配置项的值。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_set_double(const char* key, double v);

/**
 * @method app_conf_set_str
 * 设置字符串类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {const char*} v 配置项的值。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_set_str(const char* key, const char* v);
```

示例：

```c
  app_conf_set_bool("wifi.on", TRUE);
  app_conf_set_str("wifi.name", "awtk");
  app_conf_set_int("server.port", 8080);
  app_conf_set_double("server.timeout", 1.0);
```

使用 ini 格式时，实际存为：

```ini
[wifi]
  on = 1
  name = awtk
[server]
  port = 8080
  timeout = 1.000000
```

使用 json 格式时，实际存为：

```ini
{
    "wifi" : {
        "on" : true,
        "name" : "awtk"
    },
    "server" : {
        "port" : 8080,
        "timeout" : 1
    }
}
```

使用 ubjson 格式时，实际存为二进制格式，这里不再举例。

### 3.3 读取配置信息

```c
/**
 * @method app_conf_get_int
 * 获取整数类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {int32_t} defval 缺省值。
 * 
 * @return {int32_t} 返回配置项的值（如果不存在返回缺省值）。
 */
int32_t app_conf_get_int(const char* key, int32_t defval);

/**
 * @method app_conf_get_int64
 * 获取 64 位整数类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {int64_t} defval 缺省值。
 * 
 * @return {int64_t} 返回配置项的值（如果不存在返回缺省值）。
 */
int64_t app_conf_get_int64(const char* key, int64_t defval);

/**
 * @method app_conf_get_bool
 * 获取 bool 类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {bool_t} defval 缺省值。
 * 
 * @return {bool_t} 返回配置项的值（如果不存在返回缺省值）。
 */
bool_t app_conf_get_bool(const char* key, bool_t defval);

/**
 * @method app_conf_get_double
 * 获取单精度浮点数类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {double} defval 缺省值。
 * 
 * @return {double} 返回配置项的值（如果不存在返回缺省值）。
 */
double app_conf_get_double(const char* key, double defval);

/**
 * @method app_conf_get_str
 * 获取字符串类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {const char*} defval 缺省值。
 * 
 * @return {const char*} 返回配置项的值（如果不存在返回缺省值）。
 */
const char* app_conf_get_str(const char* key, const char* defval);
```

示例：

```c
  on = app_conf_get_bool("wifi.on", FALSE);
  port = app_conf_get_int("server.port", 0);
  timeout = app_conf_get_double("server.timeout", 0);
  name = app_conf_get_str("wifi.name", NULL);
```

3.4 删除配置信息

```c
/**
 * @method app_conf_remove
 * 删除配置项。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_remove(const char* key);
```

示例：

```c
  app_conf_set_str("wifi.password", "123123");
  assert(app_conf_exist("wifi.password"));

  app_conf_remove("wifi.password");
  assert(!app_conf_exist("wifi.password"));
```

3.5 检查配置信息是否存在

```c
/**
 * @method app_conf_exist
 * 检查配置项是否存在。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * 
 * @return {bool_t} 返回 TRUE 表示存在，FALSE 表示不存在。
 */
bool_t app_conf_exist(const char* key);
```

示例：

```c
  app_conf_set_str("wifi.password", "123123");
  assert(app_conf_exist("wifi.password"));

  app_conf_remove("wifi.password");
  assert(!app_conf_exist("wifi.password"));
```

3.6 持久保存配置信息

```c
/**
 * @method app_conf_save
 * 持久保存配置。
 *
 * @annotation ["static", "scriptable"]
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_save(void);
```

示例：

```c
app_conf_save();
```

3.7 关注配置信息的变化

```c
/**
 * @method app_conf_on_changed
 * 注册配置变化事件。
 *
 * @annotation ["static", "scriptable"]
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 * @return {uint32_t} 返回 id，用于 app_conf_off_changed。
 */
uint32_t app_conf_on_changed(event_func_t on_event, void* ctx);

/**
 * @method app_conf_off_changed
 * 注销配置变化事件。
 *
 * @annotation ["static", "scriptable"]
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_off_changed(uint32_t id);

/**
 * @method app_conf_off_changed_by_ctx
 * 注销配置变化事件。
 *
 * @annotation ["static"]
 * @param {void*} ctx 事件处理函数上下文。
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_off_changed_by_ctx(void* ctx);
```

示例：

```c
static ret_t app_conf_changed(void* ctx, event_t* e) {
  prop_change_event_t* evt = prop_change_event_cast(e);

  log_debug("%s changed\n", evt->name);

  return RET_OK;
}

...

app_conf_on_changed(app_conf_changed, NULL);
```

> 回调函数在修改配置的线程调用。如果是后台线程修改配置，在配置变化时，需要更新界面，此时需要用 idle_queue 进行串行化。

## 6. 释放配置相关资源。

```c
/**
 * @method app_conf_deinit
 * 释放 conf 对象。
 *
 * @annotation ["static", "scriptable"]
 * 
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t app_conf_deinit(void);
```

示例：

```c
ret_t application_exit() {
  app_conf_deinit();
  log_debug("application_exit\n");
  return RET_OK;
}
```

## 7. key 的规则

* 配置信息通常都是层次结构的，为了能访问所有数据，key 也必须是多级的，各级的名称之间用“.”分隔。

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

```c
  app_conf_set_str("network.eth0.ip", "192.169.0.100");
```

* 按索引访问

在访问时，每一级的名称，可以用索引代替，格式为：[+索引+]，比如 [0] 表示第一项，[2] 表示第三项。这对于不清楚名称的情况，或遍历全部配置，很有帮助。

如：

```c
  const char* ip0 = app_conf_get_str("network.[0].ip", NULL);
  const char* ip1 = app_conf_get_str("network.[1].ip", NULL);
  const char* ip2 = app_conf_get_str("network.[2].ip", NULL);
```

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

```c
  assert(app_conf_get_int("#size", 0) == 2); 
  assert(app_conf_get_int("wifi.#size", 0) == 2); 
```

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

```c
  assert(strcmp(app_conf_get_str("[0].#name", NULL), "wifi") == 0);
  assert(strcmp(app_conf_get_str("wifi.[0].#name", NULL), "on") == 0);
```

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

```c
  assert(app_conf_get_int("wifi.#index", -1) == 0);
  assert(app_conf_get_int("server.#index", -1) == 1);
```

## 8. 缺省配置
  
  * 位置。缺省配置放到资源中，位于：design/default/data
  
  * 命名：应用程序名 + '.' + 文件扩展名。

  * 文件扩展名与初始化时指定的格式一致。 

比如，下面的例子中，应用程序名 demo，扩展名为 json。所以对应的缺省配置文件为：design/default/demo.json

```c
  ENSURE(app_conf_init_json("demo") == RET_OK);
```

> 在初始化时，如果配置文件不存在，则将缺省配置拷贝到配置文件。

## 9. 注意事项

* . 作为 key 的分隔符。

* ini 格式不支持注释和换行。

## 10. 参考示例

* [conf_ini](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/conf_ini.c)

* [conf_json](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/conf_json.c)

* [conf_ubjson](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/conf_ubjson.c)
