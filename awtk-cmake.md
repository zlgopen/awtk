# AWTK CMake 编译与安装说明

本文介绍使用 **CMake** 在 AWTK 源码树中配置、编译与安装 `libawtk` 的方法。CMake 与现有 **SCons** 构建并存，可按需任选其一。

**要求**：CMake 3.20 及以上；C/C++ 编译器。默认（**`AWTK_SDL_VERSION=2`**）下 SDL2 由源码树内 **`3rd/SDL`** 经 `add_subdirectory` 一并构建并链接（与 SCons 使用内置 SDL 一致），**无需**单独安装系统 SDL2 开发包即可完成 AWTK 的 CMake 配置与编译。若设置 **`AWTK_SDL_VERSION=3`**，则通过 **`find_package(SDL3)`** 使用系统已安装的 SDL3（需 CMake 包与头文件，例如 Homebrew **`brew install sdl3`**），且安装导出中**不会**再附带内置的 `SDL2` 目标。

---

## SDL2 与 SDL3（`AWTK_SDL_VERSION`）

| 取值 | 行为 |
|------|------|
| **`2`（默认）** | 使用源码树 **`3rd/SDL`**，与 SCons 内置 SDL2 一致；无需系统 SDL2 开发包。 |
| **`3`** | 使用 **`find_package(SDL3 3.2.0 REQUIRED CONFIG)`** 链接系统/前缀中的 SDL3；**必须**已安装带 **CMake config** 的 SDL3（且能被 `find_path` 找到 **`SDL3/SDL.h`**）。 |

**使用 SDL3 的步骤概要**：

1. 安装 SDL3 开发文件（须含 **`SDL3Config.cmake`** 及头文件）。示例：**macOS** `brew install sdl3`；**Linux** 使用发行版 **`libsdl3-dev`** 或从 [SDL 发布页](https://github.com/libsdl-org/SDL/releases) 自行安装到统一前缀。
2. 若 CMake 找不到包，在配置时把 SDL3 的安装前缀加入 **`CMAKE_PREFIX_PATH`**（与使用其它 CMake config 包相同），例如：  
   `cmake -S . -B build_cmake -DCMAKE_PREFIX_PATH=/opt/homebrew -DAWTK_SDL_VERSION=3`
3. 执行 **`cmake` 配置** 后，在输出中查找 **`AWTK: SDL = ...`** 一行，可确认当前选用的是 SDL2 还是 SDL3，以及解析到的版本与头文件路径（SDL3 在部分环境下版本来自 **`sdl3-config --version`** 回退）。

应用或示例代码若直接 **`#include <SDL3/SDL.h>`**（或 SDL2 的 **`#include <SDL.h>`**），除通过 **`awtk::awtk`** 传递的编译接口外，可能仍需在自己的 **`target_include_directories`** 中补充对应 SDL 的 include（与是否使用内置 SDL2 相同，取决于你的包含方式）。

---

## 依赖准备

### 通用

- **SDL2**：由 **`cmake/third_party_libs.cmake`** 引入 **`3rd/SDL`** 子工程；安装前缀下会随 **`install(EXPORT awtkTargets)`** 一并导出 **`awtk::SDL2`** / **`awtk::SDL2main`**（若存在对应目标），消费方一般只需 **`target_link_libraries(... awtk::awtk)`** 即可带上 SDL 依赖。
- **C++ 标准库**：最终链接 `libawtk` 时使用 C++ 链接器（工程内含 C++ 源码，如 gpinyin）。
- 若应用代码直接 **`#include <SDL.h>`**，需自行将 **`3rd/SDL/include`** 加入包含路径，或从安装树中引用（当前安装规则主要镜像 AWTK 与部分第三方头，**未**默认安装整棵 SDL 公共头目录）。

### Linux

- 构建脚本会查找 **OpenGL**、**X11**、**gtk+-3.0**（pkg-config）等，用于图形与（可选）原生文件对话框；请安装对应 `-dev` 包。
- 若环境变量 **`SDL_VIDEODRIVER=wayland`**，与 SCons 行为一致，将跳过 nfd；否则在启用 `AWTK_WITH_NFD` 时需能解析 gtk。

### macOS

- 需 **Xcode / Command Line Tools**；无需为 AWTK 单独安装 Homebrew **SDL2**。
- 默认定义 **`WITHOUT_GLAD=1`**，与 `awtk_config_common.py` 中 Darwin 行为一致，不将 glad 链入 `libawtk`。

### Windows

- 使用 **Visual Studio** 或 **Ninja + MSVC** 时，一般无需设置 `AWTK_TOOLS_NAME`。
- 使用 **MinGW** 时，请配置 CMake 为 MinGW 工具链，并设置 **`-DAWTK_TOOLS_NAME=mingw`**，以匹配 SCons 中 `TOOLS_NAME` 语义。
- 若存在 **`dllexports/awtk.def`**，MSVC / MinGW 链接阶段会尽量使用该导出定义（与 SCons 对齐）。

### 可选：IDL / DEF 代码生成

- 打开 **`-DAWTK_RUN_IDL_CODEGEN=ON`** 时，需在 PATH 中找到 **`node`**，以执行与 `awtk_config_common.genIdlAndDefEx` 相同的 Node 脚本（更新 `dllexports/*.def` 等）。
- 默认 **OFF**，便于在无 Node 环境下先完成库编译。

---

## 配置与编译

在源码根目录（含 `CMakeLists.txt`）外使用独立构建目录：

```bash
cmake -S /path/to/awtk -B /path/to/awtk/build_cmake
cmake --build /path/to/awtk/build_cmake -j$(nproc 2>/dev/null || sysctl -n hw.ncpu)
```

常用 CMake 变量示例：

```bash
cmake -S . -B build_cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DAWTK_DEBUG=OFF \
  -DAWTK_RUN_IDL_CODEGEN=OFF \
  -DAWTK_BUILD_CMAKE_FIND_DEMO=ON
```

- **`CMAKE_BUILD_TYPE`**：未指定时，若 `AWTK_DEBUG=ON` 则默认为 **Debug**，否则 **Release**（与 `awtk_options.cmake` 逻辑一致）。
- **`AWTK_BUILD_CMAKE_FIND_DEMO`**：是否在主工程中顺带编译 **`examples/cmake_find_awtk`** 中的最小链接示例（默认 ON）。

构建产物中，共享库名在 Unix 上为 **`libawtk.so` / `libawtk.dylib`**，在 Windows 上为 **`awtk.dll`**（具体以生成器输出为准）。

---

## 安装

指定安装前缀后执行 install：

```bash
cmake --install /path/to/awtk/build_cmake --prefix /path/to/install
```

安装内容主要包括：

- **库**：`lib`（或平台等价目录）下的 **`awtk`** 及（共享库构建时）与之配套的依赖库；**`AWTK_SDL_VERSION=2`** 时安装导出中常含 **`awtk::SDL2`** / **`awtk::SDL2main`**；**`AWTK_SDL_VERSION=3`** 时由 **`SDL3::SDL3`** 在构建期链入 **`libawtk`**，安装导出**不**再附带内置 SDL2 目标。CMake 导出仍为 **`awtkTargets.cmake`**（命名空间 **`awtk::`**）。
- **头文件**：`include/awtk` 下镜像的部分 `src` 与第三方头文件，供 `#include` 使用。
- **包配置**：`lib/cmake/awtk/` 下的 **`awtkConfig.cmake`**、**`awtkConfigVersion.cmake`**。

已设置安装目标的 **`INSTALL_RPATH`**（Linux：`$ORIGIN`；macOS：`@loader_path`），便于与安装目录内的其它动态库同目录部署时加载。

---

## 在自己的 CMake 工程中使用

安装或直接使用 **构建目录** 作为包前缀均可。构建目录会生成 **`awtkConfig.cmake`**；因内置 SDL2 目标无法与旧式 **`export(TARGETS awtk ...)`** 兼容，构建树包通过 **`configure_file(cmake/awtk_build_tree_exports.cmake.in → awtk_build_tree_exports.cmake)`** 提供 **`awtk::awtk`** 的 **`IMPORTED`** 定义（详见根 **`CMakeLists.txt`**）。

```bash
cmake -S your_app -B your_app/build \
  -DCMAKE_PREFIX_PATH=/path/to/install   # 或 /path/to/awtk/build_cmake
```

在 `CMakeLists.txt` 中：

```cmake
find_package(awtk CONFIG REQUIRED)
add_executable(myapp main.c)
target_link_libraries(myapp PRIVATE awtk::awtk)
```

**`awtkConfig.cmake`** 仅 **`include(awtkTargets.cmake)`**，**不再**调用 **`find_dependency(SDL2 ...)`**。在 **`AWTK_SDL_VERSION=2`** 的构建下，SDL2 由安装导出中的 **`awtk::SDL2`** 等目标传递；若 AWTK 是以 **`AWTK_SDL_VERSION=3`** 构建的 **`libawtk`**，则 SDL3 已在库中链接完毕，消费方 **`find_package(awtk)`** 后一般**无需**再单独 **`find_package(SDL3)`**，除非你的源码直接包含 SDL3 头文件并需要其 include 路径。

仓库内参考示例：**[examples/cmake_find_awtk](examples/cmake_find_awtk/)**（可作为子工程或直接 `find_package`）。

---

## 与 `compile_config` / `awtk_config.py` 相关的 CMake 选项

下列缓存变量与 **`scripts/compile_config.py`** 及 **`awtk_config.py`** 中的概念对应，可在 `cmake -D...` 或 `ccmake` / CMake GUI 中修改：

| CMake 变量 | 说明 |
|------------|------|
| `AWTK_DEBUG` | 调试构建（影响默认 `CMAKE_BUILD_TYPE` 及编译优化选项） |
| `AWTK_PDB` | MSVC：是否生成 PDB |
| `AWTK_BUILD_TOOLS` / `AWTK_BUILD_DEMOS` / `AWTK_BUILD_TESTS` | 与 SCons 同名开关对应（当前主目标为 `libawtk`，其它目标可随后续扩展使用） |
| `AWTK_BUILD_SHARED` | 是否构建共享库形式的 `awtk` |
| `AWTK_OUTPUT_DIR` | 对应 `OUTPUT_DIR` |
| `AWTK_TOOLS_NAME` | Windows：`mingw` 或空（MSVC 风格） |
| `AWTK_INPUT_ENGINE` | `null` / `spinyin` / `t9` / `t9ext` / `pinyin` |
| `AWTK_VGCANVAS` | `NANOVG` / `NANOVG_PLUS` / `CAIRO`（CMake 侧若组合未实现会报错提示） |
| `AWTK_NANOVG_BACKEND` | `GLES2` / `GLES3` / `GL3` / `AGG` / `AGGE` / `BGFX` 等 |
| `AWTK_OPENGL_ANTIALIAS` | `HW` / `SW` / `NONE` |
| `AWTK_LCD_COLOR_FORMAT` | `bgr565` / `bgra8888` / `mono` 或留空 |
| `AWTK_WIN32_RES` | 可选 Windows 资源文件路径 |
| `AWTK_SDL_VERSION` | **`2`**（默认，内置 **`3rd/SDL`**）或 **`3`**（系统 **`SDL3::SDL3`**，需已安装 SDL3 CMake 包与头文件） |
| `AWTK_SDL_UBUNTU_USE_IME` | Ubuntu SDL 中文输入法相关 |
| `AWTK_NATIVE_WINDOW_BORDERLESS` / `AWTK_NATIVE_WINDOW_NOT_RESIZABLE` | 窗口行为 |
| `AWTK_WITH_NFD` | 是否构建 nativefiledialog |
| `AWTK_SDL_VIDEODRIVER` | Linux：记录用途；Wayland 可与环境变量 `SDL_VIDEODRIVER` 配合 |
| `AWTK_RUN_IDL_CODEGEN` | 是否在构建时运行 Node idl/def 生成 |

更细的 SCons → CMake 模块划分见 **`cmake/awtk_inventory.cmake`** 中的说明。

---

## 与 SCons 的关系

- **SCons**：仍通过根目录 **`SConstruct`**、`awtk_config.py` 等按原流程构建。
- **CMake**：根目录 **`CMakeLists.txt`**，不修改 SCons 默认工作流。
- 两套构建**不应**在同一源码树中混用同一套中间产物目录时互相覆盖；建议使用独立 **`build_cmake`** 目录专供 CMake。

---

## 已知限制与后续方向

- **Linux 上 SDL 子系统**：内置 **`3rd/SDL`** 在 Linux 上的 X11 / Wayland 等选项取决于该子目录内 CMake 的探测结果；若与目标桌面环境不符，需在 SDL 侧选项或补丁中调整（与是否使用系统包无关）。
- **`AWTK_VGCANVAS=CAIRO`** 等部分组合：若 CMake 逻辑未完整镜像 `awtk_config.py`，配置阶段可能报错，避免与 SCons 行为不一致。
- **Windows / Linux**：请在目标平台上实机验证工具链与依赖；文档随 CMake 脚本演进可能需同步更新。

如有问题，可对照 **`cmake/`** 下各模块与 **`CMakeLists.txt`** 中的实际逻辑为准。
