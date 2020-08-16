# 静态链接与动态链接

目前 PC 版本的 awtk 同时生成动态库和静态库，你可以根据具体情况进行选择。使用方法如下：

## 1. 链接动态库

> LIBS 参数使用 awtk.SHARED_LIBS 

示例：

```python
DefaultEnvironment(TOOLS = APP_TOOLS,
  CCFLAGS = awtk.CCFLAGS,
  LIBS = awtk.SHARED_LIBS,
  LIBPATH = awtk.LIBPATH,
  CPPPATH = awtk.CPPPATH,
  LINKFLAGS = awtk.LINKFLAGS,
  TARGET_ARCH=awtk.TARGET_ARCH,
  OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS
)
```

## 2. 链接静态库

> LIBS 参数使用 awtk.STATIC_LIBS 

示例：

```python
DefaultEnvironment(TOOLS = APP_TOOLS,
  CCFLAGS = awtk.CCFLAGS,
  LIBS = awtk.STATIC_LIBS,
  LIBPATH = awtk.LIBPATH,
  CPPPATH = awtk.CPPPATH,
  LINKFLAGS = awtk.LINKFLAGS,
  TARGET_ARCH=awtk.TARGET_ARCH,
  OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS
)
```

> 原来的 awtk.LIBS 仍然可以使用，为链接静态库方式。
