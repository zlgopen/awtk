# AWTK 文档生成工具

生成文档的流程：

* 1. 在头文件中按 [规定格式](https://github.com/zlgopen/awtk/blob/master/docs/api_doc.md) 编写注释。

* 2. 利用工具 [IDL gen](https://github.com/zlgopen/awtk/tree/master/tools/IDL_gen) 提取注释生成 IDL JSON 文件。

* 3. 利用本工具把 IDL JSON 文件转换成 Markdown 文档。 

## 一、准备工作

（如果已经安装请跳过）

* 安装 [bash](https://gitforwindows.org/)
* 安装 [nodejs](https://nodejs.org/zh-cn/)
* 安装 [java se](https://www.oracle.com/technetwork/java/javase/downloads/jdk12-downloads-5295953.html)
* 安装 [graphviz](http://www.graphviz.org/download/)

## 二、生成文档

* 1. 生成 AWTK 的文档。

```
./gen.sh 
```

* 2. 生成外部项目的文档

```
./gen.sh some_idl.json
```
  
> 这里的 some_idl.json 文件是用 [IDL gen](https://github.com/zlgopen/awtk/tree/master/tools/IDL_gen) 提取注释生成的 IDL JSON 文件。

## 三、同步到docs/manual

生成的文档放在 docs 目录里，AWTK 的文档可以用 sync.sh 拷贝到手册目录。

```
./sync.sh
```

> 外部项目的文档请自行拷贝到自己的目录中。