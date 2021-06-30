# object 代码生成器

用来生成 object 的框架代码的小工具。

## 1.编写描述文件

```json
{
  "name":"foobar",
  "date":"2021-06-15",
  "team":"AWTK Develop Team",
  "author":"Li XianJing <xianjimli@hotmail.com>",
  "desc":"foobar",
  "copyright":"Guangzhou ZHIYUAN Electronics Co.,Ltd.",
  "parent": "object",
  "includes":[],
  "props": [
    {
      "name":"type",
      "desc":"a int value",
      "init_value":0,
      "type":"uint32_t"
    },
    {
      "name":"name",
      "desc":"a string value",
      "init_value":"NULL",
      "type":"char*"
    }
  ]
}
```

属性的类型目前支持：

  * int8\_t
  * int16\_t
  * int32\_t
  * int64\_t
  * uint8\_t
  * uint16\_t
  * uint32\_t
  * uint64\_t
  * float
  * double
  * char*

## 2.生成代码

```
node gen_object.js samples/foobar.json
```
