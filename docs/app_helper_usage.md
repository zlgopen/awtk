# 利用 app_helper 编写 SConstruct

AWTK 项目中有 SConstruct 文件，该文件是 Scons 的编译脚本。当开发者需要在程序中使用依赖库或者进行自定义预处理时，需要遵循 Python 语法编写 SConstruct 文件，这是一件繁琐的事情。

为简化 SConstruct 的编写，AWTK 把一些公共功能提取到 awtk/scripts/app_helper_base.py 中，并且在项目目录下的 scripts/app_helper.py 中导入 app_helper_base.py，让开发者可直接在项目的 SConstruct 文件中调用这些函数。

## 1 相关函数

app_helper.py 提供了下表中的函数，具体用法请参考 awtk/docs/app_helper_usage.md。

| 函数名称               | 说明                               |
| ---------------------- | ---------------------------------- |
| add_deps               | 增加依赖的第三方库                 |
| add_libs               | 增加依赖的库                       |
| set_dll_def            | 设置动态库的 def 文件名            |
| add_cpppath            | 增加头文件搜索路径                 |
| add_libpath            | 增加库的搜索路径                   |
| add_cxxflags           | 增加C++预处理参数                  |
| add_ccflags            | 增加C 预处理参数                   |
| add_linkflags          | 增加链接参数                       |
| add_platform_libs      | 增加特定平台的需要的库             |
| add_platform_cpppath   | 增加特定平台的需要的头文件搜索路径 |
| add_platform_libpath   | 增加特定平台的需要的库搜索路径     |
| add_platform_ccflags   | 增加特定平台的需要的C 预处理参数   |
| add_platform_cxxflags  | 增加特定平台的需要的C++预处理参数  |
| add_platform_linkflags | 增加特定平台的需要的链接参数       |

> 上表中的函数实现在 awtk/scripts/app_helper_base.py 中。

## 2 示例

使用 app_helper.py 首先需要在SConstruct中导入它，然后调用 Helper 函数得到helper对象，最后使用 helper 对象调用上一节的函数。

例如，在项目中使用 SQLite 软件库（SQL数据库引擎），静态链接库以及头文件的目录结构如下：

```bash
项目目录/
    |-- 3rd/sqlite3/sqlite3.h
    |-- lib/sqlite3.lib
```

在项目目录下的 SConstruct 文件中调用 add_libs() 函数增加 sqlite3.lib 依赖库，并且调用 add_cpppath() 函数增加该依赖库头文件的搜索路径，代码如下：

```python
import os
import scripts.app_helper as app

helper = app.Helper(ARGUMENTS);
helper.add_libs(['sqlite3']).add_cpppath([os.path.join(helper.APP_ROOT, '3rd')]).call(DefaultEnvironment)

SConscript(['src/SConscript', '3rd/sqlite3/SConscript'])
```

需要注意的是：

1. 本示例中将静态链接库 sqlite3.lib 放在项目目录的 lib 文件夹下，AWTK 默认将该路径添加到库的搜索路径中。如果开发者将静态链接库放在其他路径，则还需要调用 add_libpath() 函数将该路径添加到库的搜索路径中。
2. helper 支持链式调用，call 函数需要放到最后。

## 3 特殊函数

调用 app_helper.py 提供的函数，通常只需要传入文件或目录对应的路径，但有个别函数的还需要传入其他参数，包括 add_deps 和 add_platform_ 开头的所有函数，下面举例说明。

（1）add_deps() 函数原型如下：

```python
def add_deps(self, DEPENDS_LIBS):
```

调用 add_deps() 函数增加依赖的第三方库，代码如下：

```python
DEPENDS_LIBS = [
{
  "root" : '../awtk-restful-httpd',
  'shared_libs': ['httpd'],
  'static_libs': []
}
]
helper.add_deps(DEPENDS_LIBS)
```

"root"为第三方库的路径，支持相对路径和绝对路径；"shared_libs"为动态链接库；"static_libs"为静态链接库。

（2）add_platform_libs() 函数原型如下：

```python
def add_platform_libs(self, plat, PLATFORM_LIBS)
```

调用 add_platform_libs() 函数增加 Windows 平台下的 ws2_32.lib 库，代码如下：

```python
helper.add_platform_libs('Windows', ['ws2_32'])
```

plat参数为特定平台，取值如下：Windows、Linux 和 Darwin，分别表示 Windows 平台、Linux 平台和 macOS 平台。PLATFORM_LIBS 参数为库的名称。其他 add_platform_ 开头的函数用法也类似。

> 1. [awtk-hello](https://github.com/zlgopen/awtk-hello/blob/master/SConstruct)
> 2. [awtk-mvvm-c-hello](https://github.com/zlgopen/awtk-mvvm-c-hello/blob/master/SConstruct)
> 3. [awtk-widget-table-view](https://github.com/zlgopen/awtk-widget-table-view/blob/master/SConstruct)
