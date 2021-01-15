# 编写跨平台的代码

## 1. 尽量使用 tkc 中的函数

tkc 中提供了编码、时间、网络、串口、线程、互斥和文件系统等各种需要跨平台接口的封装，这些接口在各个平台都有实现。

## 2. 针对接口编程

如果需要的某个功能，在各个平台都有不同的实现，先研究各个平台提供的功能，再抽象一个统一的接口，针对各个平台实现一个适配器。应用程序使用统一的接口去访问这些功能，应用程序不但能够跨平台运行，代码也更具可读性。

## 3. 指定特定平台的库、头文件、预处理参数和链接参数

helper 提供了一组函数来处理不同平台的参数。平台名称的取值有：

* "Windows" Windows 平台
* "Linux"  Linux  平台
* "Darwin" MacOS 平台

> Android 和  IOS 不使用 scons 编译，故不使用这种方式。


 * 添加平台特有的库

示例：

```python
helper.add_platform_libs("Windows", ["ws2_32"])
helper.add_platform_libs("Linux", ["pthread", "dl", "readline", "m"])
helper.add_platform_libs("Darwin", ["pthread", "dl", "readline", "m"])
```

 * 添加平台特有的库的搜索路径
 
```
 add_platform_libpath(self, platform_name, APP_LIBPATH)
```

示例：

```
helper.add_platform_libpath("Linux", ["/usr/local/lib"])
```

> 注意这里的 APP_LIBPATH 是数组类型。

 * 添加平台特有的头文件的搜索路径

```
  add_platform_cpppath(self, platform_name, APP_CPPPATH)
```

示例：

```
helper.add_platform_libpath("Linux", ["/usr/local/include"])
```
> 注意这里的 APP_CPPPATH 是数组类型


* 添加平台特有的宏定义预处理参数

```
 add_platform_ccflags(self, platform_name, APP_CCFLAGS)
```

示例:

```
helper.add_platform_ccflags("Linux", " -DSOME_MACRO=1 ")
```

> ccflags 是字符串格式不是数组

* 添加平台特有的C++文件的预处理参数

```
 add_platform_cxxflags(self, platform_name, APP_CXXFLAGS)
```

示例:

```
helper.add_platform_cxxflags("Linux", " -DSOME_MACRO=1 ")
```

> cxxflags 是字符串格式不是数组

* 添加平台特有的链接参数

```
 add_platform_linkflags(self, platform_name, APP_LINKFLAGS)
```
> linkflags 是字符串格式不是数组

## 4. 区分各个平台的宏

* WIN32 或 WINDOWS Windows 平台
* MINGW Windows 平台的 MinGW 编译。 
* LINUX Linux 平台
* MACOS 或__APPLE__ Macos 平台
* ANDROID Android 平台
* IOS iOS 平台

> 这些宏虽然可以区分不同的平台，但是如果将这些宏分散在应用程序中，代码将会变得丑陋难读。建议先定义接口，在适配器中使用这些宏。

示例：

```
#ifdef ANDROID
#include "SDL.h"
#endif /*ANDROID*/

#if defined(__APPLE__) || defined(LINUX)
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#elif defined(WIN32)
#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <Shlobj.h>
#include <fileapi.h>
#define unlink _unlink
#define rename MoveFileA
#define ftruncate _chsize
#include "./dirent.inc"
#include "tkc/utf8.h"
#include "tkc/mem.h"
#endif

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif /*__APPLE__*/
```

## 5. 添加平台特有的文件

有些文件是特定平台才需要的，可以根据平台的名称，来决定添加哪些文件。

示例：

```python
import platform;

OS_NAME=platform.system()
if OS_NAME == 'Windows':
  sources=Glob('src/windows/*.c')
elif OS_NAME == 'Linux':
  sources=Glob('src/linux/*.c')
```
