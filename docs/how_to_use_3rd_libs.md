# 如何引用第三方库

本文基于 AWTK 模板工程介绍如何在 SCons 编译脚本中引用第三方库。

> 注：这里所说的 AWTK 模板工程指 AWTK Designer 创建的常规工程，工程目录下有一个 SConstruct 文件，它是 SCons 编译脚本，遵顼 Python 语法，下文中的操作基本都通过编辑该文件实现。

## 1 引用第三方库的源码文件

如果第三方库提供的是源码文件，那么通常我们需要做以下两件事：

1. 将第三方库源码的头文件路径添加到工程中，方便编译时能找到库中的函数声明。
2. 把源码文件加入工程编译，方便链接时能找到库中的函数定义。

假设此处有一个名为 xxx 的第三方库，它提供了 xxx.h 和 xxx.c 两个文件，按照约定的规则，我们将它放在 AWTK 工程的 3rd/xxx 目录中，结构如下：

```bash
3rd
  xxx
    xxx.h
    xxx.c
...
SConstruct
```

### 1.1 添加头文件路径

在工程的 SConstruct 脚本文件中调用 helper.add_cpppath 函数添加库的头文件路径，传入参数为数组，此处的路径为：工程目录/3rd/xxx，如果有其他路径也可以在数组中添加新的元素，代码如下：

```python
# SConstruct
import os
import scripts.app_helper as app

...

# 构建 xxx 库的头文件路径：工程目录/3rd/xxx
XXX_PATH = [os.path.join(helper.APP_ROOT, '3rd', 'xxx')]
# 添加头文件路径，并链式调用 call 函数同步数据
helper.add_cpppath(XXX_PATH).call(DefaultEnvironment)

SConscriptFiles = ['src/SConscript', 'tests/SConscript']
helper.SConscript(SConscriptFiles)
```

> 注：在调用 helper.add_cpppath 函数后，需要调用 call 函数同步这些数据，call 函数支持链式调用，需要放到最后，且只需调用一次，更多 helper 对象中的相关函数请参考：[利用 app_helper 编写 SConstruct](https://github.com/zlgopen/awtk/blob/master/docs/app_helper_usage.md)。

### 1.2 把源码文件加入工程编译

在工程的 src/SConscript 脚本文件中，将 3rd/xxx 目录下的源码文件添加到 sources 数组中即可把它们编译到可执行程序中，代码如下：

```python
# src/SConstruct
import os
import sys
import platform

...

# 添加 src 目录下的所有 .c 文件，符号 * 为通配符
sources = Glob('**/*.c') + Glob('*.c')
# 添加 3rd/xxx 目录下的所有 .c 文件
sources += Glob('../3rd/xxx/*.c')

# 将 sources 中的所有文件编译为 bin/demo 程序
env.Program(os.path.join(BIN_DIR, 'demo'), sources, LIBS = env['LIBS'])
```

### 1.3 扩展用法（添加预处理参数、链接参数）

在工程的 SConstruct 脚本文件中调用 helper 对象中的以下函数可以添加预处理参数、链接参数：

- add_ccflags：增加 C 预处理参数。
- add_cxxflags：增加 C++ 预处理参数。
- add_linkflags：增加链接参数。

例如此处添加 C 预处理参数（定义宏NDEBUG=1），SConstruct 脚本代码如下，其他两个函数用法也是类似的：

```python
# SConstruct
...
XXX_PATH = [os.path.join(helper.APP_ROOT, '3rd', 'xxx')]
helper.add_cpppath(XXX_PATH).add_ccflags(' -DNDEBUG=1 ').call(DefaultEnvironment)
...
```

## 2 引用第三方库编译好的库文件

引用第三方库时，更常见的应该是直接引用编译好的库文件（动态库或静态库），此时在工程的 SConstruct 中描述依赖关系即可。具体方式为：先定义依赖描述，再调用 set_deps 将依赖描述设置到 helper 对象中。

### 2.1 常规用法

需要注意的是，使用本节方法引用第三方库时，默认按照 AWTK 约定的规则存放第三方库的代码文件和库文件，规则如下：

- 第三方库的代码文件需要放在库路径 **src** 目录中。
- 第三方库的动态库文件需要放在库路径的 **bin** 目录中。
- 第三方库的静态库文件需要放在库路径的 **lib** 目录中。

> 注：如果想自定义上述文件的路径，则需额外指定 cpppath 和 libpath 参数，详见下文。

假设此处有一个名为 xxx 的第三方库，它提供了头文件 xxx.h 和相关的库文件，按照约定的规则，我们将它放在 AWTK 工程的 3rd/xxx 目录中，结构如下：

```bash
3rd
  xxx
    bin
      xxx.dll  # 动态库文件
      xxx.lib  # 动态库的导出列表
    lib
      xxx.lib  # 静态库文件
    src
      xxx.h
...
SConstruct
```

在工程的 SConstruct 脚本文件中，引用动态库的方式如下：

```python
# SConstruct
...
DEPENDS_LIBS = [
  {
    "root" : '3rd/xxx',     # 第三方库路径，支持绝对路径和相对路径。
    'shared_libs': ['xxx'], # 引用的动态库名称，动态库文件必须放在 root/bin 目录下。
    'static_libs': []       # 引用的静态库名称，静态库文件必须放在 root/lib 目录下。
  }
]

# 设置依赖库，默认会添加以下路径：
# 1.头文件路径（root/src）
# 2.库文件路径（root/bin、root/lib）
helper.set_deps(DEPENDS_LIBS)
...
helper.call(DefaultEnvironment)
```

> 注：helper.set_deps 一定要在 helper.call 之前调用，call 函数支持链式调用，需要放到最后，且只需调用一次。

### 2.2 依赖描述参数

DEPENDS_LIBS 是一个数组，可以添加多个第三方依赖库，每个元素是一个依赖描述对象，可以指定以下参数：

- root 第三方库的根目录。建议使用相对于应用程序的相对路径。如：

```python
"root" : '../awtk-mvvm',
```

- shared_libs 第三方库提供的动态库列表（数组），不带扩展名和 lib 前缀。如：

```python
"shared_libs" : ["a","b","c"]
```

- static_libs 第三方库提供的静态库库列表（数组），不带扩展名和 lib 前缀。如：

```python
"static_libs" : ["a","b","c"]
```

- libpath 库所在的路径，相对于前面指定的 root 路径。缺省为：["lib", "bin"]。如：

```python
"libpath" : ["lib", "bin"]
```

- cpppath 头文件所在的路径，相对于前面指定的 root 路径。缺省为：["src"]。如：

```python
"cpppath" : ["includes", "src"]
```

- cflags C 语言的预处理参数（可选）。如：

```python
"cflags" : " -DNDEBUG=1 "
```

- cxxflags C++语言的预处理参数（可选）。

 
- ccflags C/C++语言的预处理参数（可选）。


### 2.3 完整示例

- [awtk-mvvm-c-hello](https://github.com/zlgopen/awtk-mvvm-c-hello/blob/master/SConstruct)
