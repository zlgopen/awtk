# 如何引用第三方库

使用第三方库时，在应用程序的 SConstruct 中描述依赖关系即可。具体方式为：先定义依赖描述，再调用 set_deps 将依赖描述设置到 helper 里。

示例：

```python
DEPENDS_LIBS = [
  {
    "root" : '../awtk-mvvm',
    'shared_libs': ['mvvm'],
    'static_libs': []
  },  
  {
    "root" : '../awtk-restful-httpd',
    'shared_libs': ['httpd'],
    'static_libs': []
  }
]

helper.set_deps(DEPENDS_LIBS)
```

> 注意：helper.set\_deps 一定要在 helper.call 之前调用。

## 依赖描述内容

* root 第三方库的根目录。建议使用相对于应用程序的相对路径。

如：
```
  "root" : '../awtk-mvvm',
```

* shared_libs 第三方库提供的共享库列表（数组），不带扩展名和 lib 前缀。

如：
```
"shared_libs" : ["a","b","c"]
```

* static_libs 第三方库提供的静态库库列表（数组），不带扩展名和 lib 前缀。

如：
```
"static_libs" : ["a","b","c"]
```

* libpath 库所在的路径，相对于前面指定的 root 路径。缺省为：["lib", "bin"]

如：

```
"libpath" : ["libs", "bin"]
```

* cpppath 头文件所在的路径，相对于前面指定的 root 路径。缺省为：["src"]

如：
```
"cpppath" : ["includes", "src"]
```

* cflags C 语言的 flags 定义（可选）。

如：
```
"cflags" : " -DNDEBUG=1 "
```

* cxxflags C++语言的 flags 定义（可选）。

 
* ccflags C/C++语言的 flags 定义（可选）。

## 完整示例

* [awtk-mvvm-c-hello](https://github.com/zlgopen/awtk-mvvm-c-hello/blob/master/SConstruct)
