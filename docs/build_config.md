# 特殊平台编译配置

有些平台编译比较特殊，不能使用 scons 编译，也不能使用 keil 编译。我们用一个配置文件描述项目，然后用脚本来编译或生成项目模板。目前有 web、ios 和 android 版本使用这种方式。

配置文件采用 JSON 格式，位置放在项目的根目录下，推荐命名为 build.json。

## 一、通用选项

* name 应用程序名称（不支持中文和特殊符号）。
* version 版本号。
* assets 资源所在的目录（相对于配置文件所在的目录，android/ios 一般使用"res/assets", web 一般使用 design）。
* sources 源文件列表（相对于配置文件所在的目录）。文件名支持通配符如*.c。只要添加应用程序本身和使用的第三方库的源码即可，AWTK 本身用到的代码会自动添加。
* includes 头文件搜索路径列表（相对于配置文件所在的目录）。

> sources 虽然是通用选项，但是不同平台，包含的源文件可能并不相同，此时应该放到具体平台之下。

## 二、web 平台选项

web 平台选项放在 web 子键下面，目前支持以下选项：

* app_type 目前可选择的值有。"c"表示用 C 语言开发的应用程序，"js"表示用 Javascript 开发的应用程序。
* config 用于指定一些运行参数。
  * width 如果应用开发时没有做自适应性布局，可以用 width 参数指定画布的宽度。
  * height 如果应用开发时没有做自适应性布局，可以用 height 参数指定画布的高度。
  * defaultFont 缺省字体。缺省为"sans"。
  * fontScale 字体的缩放比例。缺省为 1。

> config 中的参数也可以直接在 URL 中指定。如：

```
http://192.168.1.117:8080/demoui/index.html?width=480&height=800&fontScale=0.8&defaultFont=serif
```

如：

用 C 语言写的 demoui 的配置文件：

```
  "web": {
    "app_type": "c",
    "assets": "design",
    "sources": [
      "demos/assets.c",
      "demos/demo_ui_app.c"
    ],
    "config": {
      "fontScale": "0.8",
      "defaultFont": "sans"
    }
  },
```

用 Javascript 写的 demoui 的配置文件：

```
{
  "name":"demouijs",
  "version":"1.0",
  "app_type":"js",
  "assets": "design",  
  "sources":["./design/default/scripts/basic.js"]
}
```

## 三、android 平台选项

android 平台选项放在 android 子键下面，目前支持以下选项：

* app_name 应用程序的完整名称。如：org.zlgopen.demoui
* cflags C 代码额外的编译参数。
* cppflags C++代码额外的编译参数。
* features 平台专用特性。目前支持：
  * fullscreen 是否全屏。true 表示全屏，false 表示不全屏。

示例：

```
  "android": {
    "features": {
      "fullscreen":true
    },    
    "app_name": "org.zlgopen.demoui",
    "sources": [
      "demos/assets.c",
      "demos/demo_ui_app.c",
      "demos/vg_common.inc",
      "res/assets.inc",
      "res/assets/__assets_default.inc"
    ]
  },
```

## 四、 ios 平台选项

* app_name 应用程序的完整名称。如：org.zlgopen.demoui
* defines 宏定义。ios 和 android 定义方式有些不同，宏定义要放到 defines 中，而不能放到 cflags 和 cppflags。格式也有些不同，如：

```
"defines":" HAVE_CONFIG_H=1 BUILDING_LIBCURL=1 WITH_CURL=1 ",
```

```
  "ios": {
    "app_name": "org.zlgopen.demoui",
    "sources": [
      "demos/assets.c",
      "demos/demo_ui_app.c",
      "demos/vg_common.inc",
      "res/assets.inc",
      "res/assets/__assets_default.inc"
    ]
  }
```

## 五、完整示例

下面是 demoui 的完整示例

```
{
  "name": "demoui",
  "version": "1.0",
  "assets": "res/assets",
  "author": "xianjimli@hotmail.com",
  "copyright": "Guangzhou ZHIYUAN Electronics Co.,Ltd.",
  "web": {
    "app_type": "c",
    "assets": "design",
    "sources": [
      "demos/assets.c",
      "demos/demo_ui_app.c"
    ],
    "config": {
      "fontScale": "0.8",
      "defaultFont": "sans"
    }
  },
  "android": {
    "app_name": "org.zlgopen.demoui",
    "sources": [
      "demos/assets.c",
      "demos/demo_ui_app.c",
      "demos/vg_common.inc",
      "res/assets.inc",
      "res/assets/__assets_default.inc"
    ]
  },
  "ios": {
    "app_name": "org.zlgopen.demoui",
    "sources": [
      "demos/assets.c",
      "demos/demo_ui_app.c",
      "demos/vg_common.inc",
      "res/assets.inc",
      "res/assets/__assets_default.inc"
    ]
  }
}

```

## 六、示例与参考

* [awtk-http-client 的配置文件](https://github.com/zlgopen/awtk-http-client/blob/master/build.json)

* [awtk-hello 的配置文件](https://github.com/zlgopen/awtk-hello/blob/master/build.json)

* [Android/IOS 插件的配置文件](https://github.com/zlgopen/awtk-mobile-plugins/blob/master/build.json)

* [awtk-web-js 的配置文件](https://github.com/zlgopen/awtk-jerryscript/blob/master/build_web.json)