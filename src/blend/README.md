# 图片合成函数

下列文件均由gen.sh根据template中的模板自动生成：

* blend\_image\*.c/.h 
* rotate\_image\*.c/.h 
* fill\_image\*.c/.h

> 支持新的格式可以修改gen.sh，并运行gen.sh。

> gen.sh是bash脚本，Windows下可在git bash下运行。

> 生成后会调用clang-format格式化代码。查找顺序：环境变量CLANG_FORMAT、PATH，然后是按平台确定的常见安装目录（VS Code的C/C++插件、LLVM/Homebrew等，Windows下找clang-format.exe），都找不到时自动跳过。

## 手工优化的blend_a实现

blend\_image\_{dst}\_{src}.c默认使用blend\_image.inc中的通用blend\_a实现。

如果要对某种格式组合做手工优化，按下列步骤操作：

1. 新建blend\_a\_{dst}\_{src}.inc，在里面实现blend\_a\_{dst}\_{src}函数。
2. 运行gen.sh。生成blend\_image\_{dst}\_{src}.c时，如果检测到该inc文件存在，就自动包含它，并生成define语句，用blend\_a\_{dst}\_{src}替换blend\_image.inc中的通用实现。

生成的blend\_image\_{dst}\_{src}.c中的相关代码如下（不存在inc文件时，这两行都不会生成）：

```c
#include "blend_a_bgr565_bgr565.inc"
#define blend_a blend_a_bgr565_bgr565

#include "pixel_ops.inc"
#include "blend_image.inc"
```

> blend\_a\*.inc是手工维护的，gen.sh不会覆盖它。

> blend\_image\*.c仍然是自动生成的，请不要手工修改。

> 没有inc文件的格式组合，不需要创建inc文件。

目前手工优化的格式组合有：

* blend\_a\_bgr565\_bgr565.inc
* blend\_a\_bgr565\_bgra8888.inc
* blend\_a\_bgr565\_rgba8888.inc
