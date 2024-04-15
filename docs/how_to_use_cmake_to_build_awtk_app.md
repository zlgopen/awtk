# 如何使用 CMake 构建 AWTK 应用

## 1. 安装 CMake

CMake 是一个开源的跨平台的构建工具，可以用来构建 AWTK 应用。

在 Windows 下，可以从 [CMake 官网](https://cmake.org/download/) 下载安装包进行安装。

在 Linux 下，可以使用包管理工具安装：

```bash
sudo apt-get install cmake
```

## 2. 创建 CMakeLists.txt

* 指定 awtk 的路径

```cmake
set(awtk_DIR "../awtk")
```

* 导入 awtk 的配置

```cmake
find_package(awtk REQUIRED)
```

* 添加头文件路径(请根据自己的情况调整)

```cmake
include_directories(
    ${CMAKE_SOURCE_DIR}/res
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/3rd
    )   
```

* 添加源文件(请根据自己的情况调整)
    
```cmake
file(GLOB SOURCES src/*.c 3rd/sqlite3/sqlite3.c)
add_executable(hello ${SOURCES})
```

* 添加链接库(请根据自己的情况调整)
    
```cmake
target_link_libraries(hello awtk)
```

* 设置 WIN32_EXECUTABLE 属性

```cmake
set_target_properties(hello PROPERTIES WIN32_EXECUTABLE TRUE)
```

完整示例：

```cmake
cmake_minimum_required (VERSION 3.10)

project(hello)

set(awtk_DIR "../awtk")
find_package(awtk REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/res
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/3rd
    )   

file(GLOB SOURCES src/*.c 3rd/sqlite3/sqlite3.c)
add_executable(hello ${SOURCES})
target_link_libraries(hello awtk)
set_target_properties(hello PROPERTIES WIN32_EXECUTABLE TRUE)
```

## 3. 配置

```bash
cmake -S . cmake -B build
```

## 4. 编译

> 在 Windows 下可以用 visual studio 打开 build 目录下的工程，然后进行编译。

```bash
cmake --build build
```

## 5. 运行

```bash
./build/hello
```

## 6. 调试

> 在 Windows 下可以用 visual studio 打开 build 目录下的工程，然后进行调试。


参考：[awtk-hello](https://gitee.com/zlgopen/awtk-hello)