# AWTK 应用程序中的资源

## 一、项目目录结构

使用 [AWTK Designer](https://awtk.zlg.cn/) （下文简称：Designer）可以创建 AWTK 项目，默认情况下，项目的目录结构如下表所示：

| 项目          | 说明                                                                          |
| ------------- | ----------------------------------------------------------------------------- |
| 3rd           | 存放 Designer 安装的插件（自定义控件）以及其他第三方库。                      |
| bin           | 存放可执行文件和动态库。                                                      |
| design        | 存放项目的原始资源，可在Designer上编辑。                                      |
| lib           | 存放静态库。                                                                  |
| res           | 存放项目运行时需要的资源，由 Designer 打包或者scripts/update_res.py脚本生成。 |
| scripts       | 存放编译项目和打包资源的脚本。                                                |
| src           | 存放项目的源码文件。                                                          |
| tests         | 存放项目的单元测试代码。                                                      |
| project.json  | 项目的描述文件，包含项目设置等信息，Designer 可通过该文件打开项目。           |
| manifest.json | 校验文件，保存项目文件的 md5 信息。                                           |
| SConstruct    | SCons编译脚本。                                                               |

## 二、资源目录结构

项目创建后，design 目录默认有一个名称为"default"的主题目录。主题的目录结构如下表所示：

| 项目    | 说明                                                                            |
| ------- | ------------------------------------------------------------------------------- |
| data    | 存放该主题的自定义资源，即除了字体、图片、语言、样式、UI、XML、脚本之外的文件。 |
| fonts   | 存放该主题使用的字体，内涵子目录详见下文。                                      |
| images  | 存放该主题使用的图片，内涵子目录详见下文。                                      |
| scripts | 存放该主题使用的脚本文件。                                                      |
| strings | 存放该主题使用的语言文件。                                                      |
| styles  | 存放该主题使用的样式文件。                                                      |
| ui      | 存放该主题使用的UI文件。                                                        |

(1) fonts 目录中包含以下子目录：

  - config：临时目录，存放记录字体的保留字符的文件；
  - origin：临时目录，存放被裁剪的TTF的原始文件。

(2) images 目录中包含以下子目录：
  - xx：存放与屏幕密度无关的图片；
  - x1：存放普通密度屏幕上使用的图片；
  - x2：存放2倍密度屏幕上使用的图片；
  - x3：存放3倍密度屏幕上使用的图片；
  - svg：存放svg图片。

> 备注：AWTK运行时会优先在 x1、x2、x3 目录中的其中一个查找图片（比如LCD的DPR=1，则在x1目录），如果找不到，则在xx目录中查找。

## 三、如何使用常规资源

这里说的常规资源指的是 字体、图片、UI、样式等资源，具体的使用方法可以参考文档：[AWTK 中的资源管理](./assets_manager.md)。

## 四、如何使用data目录下的资源（自定义资源）

主题中的 data 目录可以存放除了字体、图片、语言、样式、UI、XML、脚本之外的文件，比如项目的配置文件或者其他自定义类型的文件。这里假设项目存在一个配置文件 conf.json，内容如下：

```json
{
    "tom": {
        "name": "tom",
        "age": 100
    },
    "jim": {
        "name": "jim",
        "age": 200
    },
    "kite": {
        "name": {
            "first": "kite",
            "last": "gates"
        }
    }
}
```

可以将它存放在项目的 design/default/data 目录中，其中"default"是主题名称，使用 Designer 或执行 scripts/update_res.py 脚本打包资源时，会自动将该目录下的资源打包到 res 的对应目录中。

在代码中，可以通过 assets\_manager\_ref 接口查找并引用资源，代码如下：

```c
assets_manager_t* am = assets_manager();                                   /* 获取资源管理器对象 */
asset_info_t* info = assets_manager_ref(am, ASSET_TYPE_DATA, "conf.json"); /* 查找并引用资源对应资源 */

/* 获取自定义资源并做处理，这里仅打印信息，可根据实际情况编写处理代码 */
log_debug("%s\n", (char*)info->data);

assets_manager_unref(am, info);  /* 释放引用资源 */
```

## 五、相关文档

* [AWTK 中的资源管理](./assets_manager.md)
