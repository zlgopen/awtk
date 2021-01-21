# conf-io

## 介绍
为应用程序访问配置文件提供统一的 API，不管使用哪种格式，除了初始化代码外，其它访问配置信息的方式完全一样。

> 目前支持 INI/JSON/UBJSON。

conf-io 是可选组件，需要自己包含头文件，否则 64 位数据类型会被截断成 32 位的数据。

```c
#include "conf_io/app_conf.h"
```

初始化需要包含相应的头文件：
```c
#include "conf_io/app_conf_init_json.h"
```

具体用法请参考 [如何存取应用程序的配置信息](https://github.com/zlgopen/awtk/blob/master/docs/how_to_use_app_conf.md)

## 直接使用 INI/JSON/UBJSON 的方法

开发者也可以直接使用 INI/JSON/UBJSON 提供的 API 去操作自己的配置文件，可以从文件加载，从内存加载或其它地方加载（实现对于的 data\_reader/data\_writer 即可）。

> key 的规则请参考 [如何存取应用程序的配置信息](https://github.com/zlgopen/awtk/blob/master/docs/how_to_use_app_conf.md) 的第7节。

### 1. 直接使用 JSON 的示例
test.json 文件内容

```json
{
    "group" : {
        "key" : "value",
        "tom" : {
            "name" : "tom",
            "age" : 100,
            "weight" : 60.5100
        }
    },
    "array" : [
        {
            "name" : "none"
        },
        2,
        3
    ],
    "tom" : {
        "name" : "tom",
        "age" : 100,
        "weight" : 60.5000
    }
}c
```

示例代码

```c
  object_t* conf = conf_json_load("file://./tests/testdata/test.json", TRUE);

  ASSERT_EQ(conf_obj_save(conf), RET_OK);
  ASSERT_EQ(object_set_prop_str(conf, "tom.name", "tom"), RET_OK);
  ASSERT_EQ(object_set_prop_int(conf, "tom.age", 100), RET_OK);
  ASSERT_EQ(object_set_prop_float(conf, "tom.weight", 60.5), RET_OK);

  ASSERT_STREQ(object_get_prop_str(conf, "tom.name"), "tom");
  ASSERT_EQ(object_get_prop_int(conf, "tom.age", 0), 100);
  ASSERT_EQ(object_get_prop_float(conf, "tom.weight", 0), 60.5);

  ASSERT_EQ(conf_obj_save(conf), RET_OK);

  ASSERT_STREQ(object_get_prop_str(conf, "group.key"), "value");
  ASSERT_EQ(object_remove_prop(conf, "group.key"), RET_OK);
  ASSERT_EQ(object_get_prop_str(conf, "group.key"), (char*)NULL);

  OBJECT_UNREF(conf);
```

从内存加载：

```c
#include "tkc/data_reader_mem.h"
...
  object_t* conf = NULL;
  char url[MAX_PATH + 1]; 
  data_reader_mem_build_url(data, size, url);
  conf = conf_json_load(url, FALSE);
  ASSERT_NE(conf, (object_t*)NULL);
```

> 更多用法请参考：tests/conf_json_test.cc