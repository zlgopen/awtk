# 利用 app_helper 编写 SConstruct

编写 SConstruct 是一件繁琐的事情。我们把一些公共的功能，提取到 app_helper 中，让 SConstruct 稍微简化一点：

## 一、示例

```python
import os
import scripts.app_helper as app

helper = app.Helper(ARGUMENTS);
helper.add_libs(['sqlite3']).add_cpppath([os.path.join(helper.APP_ROOT, '3rd')]).call(DefaultEnvironment)

SConscript(['src/SConscript', '3rd/sqlite3/SConscript'])
```

## 二、helper API 介绍

* add_deps 增加依赖的第三方库

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

* add_libs 增加依赖的库。

```
helper.add_libs(['sqlite3'])
```

* set_dll_def 设置动态库的 def 文件名。

```
helper.set_dll_def('src/table_view.def')
```

* add_cpppath 增加头文件搜索路径。

```
helper.add_cpppath([os.path.join(helper.APP_ROOT, '3rd')])
```

* add_libpath 增加库的搜索路径。 

* add_cxxflags 增加 C++预处理参数。

* add_ccflags 增加 C 预处理参数。

* add_linkflags 增加链接参数。

* add_platform_libs 增加特定平台的需要的库。

```
helper.add_platform_libs('Windows', ['ws2_32'])
```

* add_platform_cpppath 增加特定平台的需要的头文件搜索路径。

* add_platform_libpath 增加特定平台的需要的库搜索路径。 

* add_platform_ccflags 增加特定平台的需要的 C 预处理参数。
    
* add_platform_cxxflags  增加特定平台的需要的 C++预处理参数。

* add_platform_linkflags 增加特定平台的需要的链接参数。

## 三、注意事项

* 平台相关的函数，plat 的取值：

  * Windows 表示 Windows
  * Linux 表示 Linux
  * Darwin 表示 MacOS

* helper 支持链式调用，call 函数需要放到最后。

```
helper.add_libs(['sqlite3']).add_cpppath([os.path.join(helper.APP_ROOT, '3rd')]).call(DefaultEnvironment)
```

## 四、参考

* https://github.com/zlgopen/awtk-hello/blob/master/SConstruct

* https://github.com/zlgopen/awtk-mvvm-c-hello/blob/master/SConstruct

* https://github.com/zlgopen/awtk-widget-table-view/blob/master/SConstruct
