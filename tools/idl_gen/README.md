# AWTK IDL 生成工具

在 AWTK 中，对外的 API 都按 [规定格式](https://github.com/zlgopen/awtk/blob/master/docs/api_doc.md) 编写注释，通过 IDL gen 工具把这些注释提取成 JSON 文件，这个 JSON 文件可以用来生成文档、各种语言的绑定和 web 版本对外的 API。

## 一、准备工作

（如果已经安装请跳过）

* 安装 [nodejs](https://nodejs.org/zh-cn/)

## 二、生成 IDL 文件

* 1. 基本用法

```
node index.js  [输出的 IDL 文件名] [源代码路径]
```

* 2. 生成 AWTK 的 IDL 文件。

```
node index.js
```

* 3. 生成外部项目的 IDL 文件。

生成外部项目的 IDL 文件时，需要指定**输出的 IDL 文件名**和**源代码的路径**。如：

```
node index.js  slider_circle.idl ../../../awtk_slider_circle/src
```