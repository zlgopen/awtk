# AWTK 项目描述文件

AWTK Designer（下面简称 Designer）创建的项目均有一个项目描述文件 project.json。该文件是一个JSON格式的文件，是每个项目必须的，主要描述了项目的名称、类型、启动页面、资源配置等。Designer 通过该文件来获取项目信息并识别项目是否可以打开。

如：

```json
{
  "name": "project_name",
  "version": "1.0.0",
  "date": "2020-07-04",
  "team": "AWTK Develop Team",
  "author": "XXXX <xxxx@zlg.cn>",
  "desc": "使用 XML(UI文件)描述用户界面 和 C/C++ 实现业务逻辑的应用程序",
  "copyright": "Guangzhou ZHIYUAN Electronics Co.,Ltd.",
  "type": "PROJECT_WIDGETS_APPLICATION",
  "subType": "",
  "entry": "main",
  "assets": {
    "activedTheme": "default",
    "outputDir": "res",
    "loadFrom": "any",
    "const": "resource_data",
    "screenDPR": "x1",
    "defaultLanguage": "zh",
    "defaultCountry": "CN",
    "themes": {
      "default": {
        "activedSystemBar": "system_bar",
        "activedBottomSystemBar": "system_bar_b",
        "packaged": true,
        "fonts": {
          "default": {
            "text": " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~，。？！确定取消",
            "18": " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~，。？！确定取消"
          }
        },
        "lcd": {
          "width": "800",
          "height": "480",
          "colorDepth": "16bit",
          "colorFormat": "BGR(A)"
        }
      }
    }
  },
  "creator": {
    "name": "AWTK Designer",
    "version": "1.0.0"
  },
  "awtkRoot": "${DesignerPath}/SDK/awtk",
  "usesSdk": {
    "awtk:minSdkVersion": "20070",
    "awtk:maxSdkVersion": "20090",
    "awtk:targetSdkVersion": "20090"
  }
}
```

## 1. name

name 是必须的，表示项目的名称。

> 注意：对于自定义控件库项目，该名称必须是全小写英文字母，单词之间用下划线连接。

## 2. version

version 是可选的，表示项目的版本号，缺省为“1.0.0”。

## 3. date

date 是可选的，表示项目创建的日期。

## 4. team

team 是可选的，表示项目开发团队的名称。

## 5. author

author 是可选的，表示作者、联系方式。

## 6. desc

desc 是可选的，表示项目简介或者功能描述。

## 7. copyright

copyright 是可选的，表示版权声明。

## 8. type

type 是可选的，表示项目类型，缺省为“PROJECT_WIDGETS_APPLICATION”。

目前有如下几种项目类型：

* PROJECT_WIDGETS_APPLICATION 表示使用 XML(UI文件)描述用户界面 和 C/C++ 实现业务逻辑的应用程序，在代码中可通过 AWTK API 直接访问 Widget；

* PROJECT_CUSTOM_WIDGET 表示自定义控件库，可导入 Designer，安装到项目后可以像内置控件一样编辑并实时预览显示效果；

* PROJECT_THEME 表示应用程序主题，可导入 Designer 并应用到不同的项目；

* PROJECT_UI_SNIPPET 表示界面片段，可导入 Designer 并应用到不同的项目。

## 9. subType

subType 是可选的。

对于PROJECT_UI_SNIPPET类型的项目，目前有system_bar（系统栏）、keyboard（软键盘）两种。

## 10. entry

entry 是必须的，表示应用程序或者示例程序的启动页面。

## 11. assets

assets 是必须的，是应用程序或者示例程序的资源配置。主要参数有：

#### 11.1 activedTheme

activedTheme 表示Designer当前激活的主题，也是应用程序或者示例程序的默认主题。

#### 11.2 outputDir

outputDir 表示资源打包时的输出目录。默认为项目的res目录。

#### 11.3 loadFrom

loadFrom 表示资源的打包方式，有 2 个选项：

* any 表示文件+常量，即同时生成用于文件系统的资源和可直接编译到代码中的常量资源（以常量数组的形式存在）；

* fs 表示为仅文件，即仅生成用于文件系统的资源。

#### 11.4 const
const 表示资源的常量格式，主要指字体、图片打包时生成的常量资源的格式，有 2 种格式：

* resource_data 表示原始数据，即常量数组中缓存的是原始的文件数据（比如，PNG 图片的原始数据为 PNG 数据）；

* bitmap_data 表示位图数据，即常量数组中缓存的是 Bitmap 数据。

* all_data 表示同时生成上述两种类型的数据。

#### 11.5 screenDPR

screenDPR 表示LCD的设备像素比（DPR），影响打包生成常量资源时应包含哪些图片资源，有 3 个选项：

* x1 表示包含 x1、xx、svg；

* x2 表示包含 x2、xx、svg；

* x3 表示包含 x3、xx、svg。

#### 11.6 defaultLanguage

defaultLanguage 表示应用程序默认使用的语言，默认为中文（zh）。

#### 11.7 defaultCountry

defaultCountry 表示应用程序默认使用的语言所属的国家或地区，默认为中国（CN）。

#### 11.8 themes

themes 表示应用程序使用的主题，主题可以有多个，缺省为default。

主题的参数有：

* activedSystemBar 表示正在使用的顶部系统栏

* activedBottomSystemBar 表示正在使用的底部系统栏

* packaged 表示资源打包时是否打包该主题

* fonts 表示该主题使用到的字体及其设置
  比如名称为 default 的字体，可以设置：
  text 表示裁剪 TTF 文件时需要保留的字符；
  18 表示生成 18 字号的 Bitmap 字体时需要保留的字符，这里数值表示字体的字号。

* lcd 表示 目标 LCD 的参数
  主要的参数有：
  width 表示 LCD 的宽度
  height 表示 LCD 的高度
  colorDepth 表示 LCD 的颜色深度
  colorFormat 表示 LCD 的颜色格式

其中，LCD 的颜色深度和颜色格式决定了 LCD Frame Buffer 的格式。

> 颜色深度为 1bit 时，颜色格式固定为 MONO，表示 Frame Buffer 为 MONO；
> 颜色深度为 16bit 且颜色格式为 RGB(A)，表示 Frame Buffer 为 RGB565；
> 颜色深度为 32bit 且颜色格式为 RGB(A)，表示 Frame Buffer 为 RGB8888；
> 颜色深度为 16bit 且颜色格式为 BGR(A)，表示 Frame Buffer 为 BGR565；
> 颜色深度为 32bit 且颜色格式为 BGR(A)，表示 Frame Buffer 为 BGR8888。

而 LCD Frame Buffer 的格式又决定打包生成的字体位图数据（也就是字模数据）和图片位图数据的格式。

> 对于字体：
> Frame Buffer 为 MONO 时，字模的 alpha 通道为 1bit；否则为 8bit。
> 对于图片：
> Frame Buffer 为 MONO 时，无论图片的颜色通道是什么格式，统一转为 1bit；原颜色灰度>10 时 为 1（表示白色），否则为 0（表示黑色）。
> Frame Buffer 为 565 格式时，没有 alpha 通道的图片会转为 565 格式的位图数据，但有 alpha 通道
的图片则仍会转为 8888 格式的位图数据（运行时经过半透混合会转成 565 格式）；
> Frame Buffer 为 8888 格式时，则全部转为 8888 格式的位图数据。
> 需要注意的是，对于控件样式（比如背景颜色、文本颜色等）：
> 由于运行时计算灰度会影响效率，因此，当 Frame Buffer 为 MONO 时，AWTK 认为颜色的 R 通道为 0 时表示黑色，否则为白色。建议 MONO 模式下，样式里的颜色均显式地设为黑色或白色。

## 12. creator

creator 是必须的，表示项目的创建者，主要的参数有：

* name 表示创建者的名称，固定为“AWTK Designer”；

* version 表示创建者的版本号，比如“0.1.6”。

## 13. usesSdk

usesSdk 是可选的，表示项目依赖的SDK的信息。

比如，项目依赖的AWTK有如下参数：

* 参数 awtk:minSdkVersion 是可选的，表示兼容的最低版本。

* 参数 awtk:maxSdkVersion 是可选的，表示兼容的最高版本。

* 参数 awtk:targetSdkVersion 是可选的，表示最佳版本。

> 上述版本号对应发布的 AWTK 中 component.json 文件中的 "release_id"
> 注意：如果没有显式设置，则认为兼容所有版本。

## 14. awtkRoot

对于1.0.0以上版本的Designer，awtkRoot 是必须的，用于指定项目编译时（即点击 Designer 工具栏上的编译按钮启动编译时）依赖的 AWTK 的路径。

默认为 “${DesignerPath}/SDK/awtk”。

其中， “${DesignerPath}” 表示 Designer 的安装目录。

> 使用 scons 进行编译时，可以通过 AWTK_ROOT 参数手动设置 AWTK 的路径，比如：

```cmd
scons AWTK_ROOT=c:/AWTK/SDK/awtk
```


