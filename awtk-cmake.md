# AWTK CMake 构建说明

AWTK 支持 CMake 与 SCons 两套构建，二者并存。CMake 需 3.20 及以上，请使用独立构建目录，不要与 SCons 的中间产物混用。

## 编译与安装

```bash
cd awtk
cmake -S . -B build_cmake
cmake --build build_cmake --parallel
cmake --install build_cmake --prefix ../awtk-install
```

Visual Studio、Xcode 这类多配置生成器，需在 build 和 install 时补 `--config Release` / `--config Debug` 或者其他的配置。

## 平台注意事项

**Linux**：需 OpenGL、X11、gtk+-3.0（pkg-config）的开发包。

**macOS**：需 Xcode / Command Line Tools；默认 `WITHOUT_GLAD=1`，不将 glad 链入 libawtk。

**Windows**：MSVC 无需额外设置；使用 MinGW 时，请配置 CMake 为 MinGW 工具链，并设置 `-DAWTK_TOOLS_NAME=mingw`。

```
MSVC:
cmake -S . -B build_cmake
cmake --build build_cmake --config Release --parallel
cmake --install build_cmake --config Release --prefix ../awtk-install
--------
MinGW + MinGW Makefiles:
cmake -S . -B build_cmake -G "MinGW Makefiles" -DAWTK_TOOLS_NAME=mingw -DCMAKE_COMPILER_IS_MINGW=1
cmake --build build_cmake --parallel
cmake --install build_cmake --prefix ../awtk-install
--------
MinGW + Ninja:
cmake -S . -B build_cmake -G Ninja -DAWTK_TOOLS_NAME=mingw -DCMAKE_COMPILER_IS_MINGW=1
cmake --build build_cmake --parallel
cmake --install build_cmake --prefix ../awtk-install
```

> 较新版本的 CMake 使用 MinGW + SDL2 时 configure 报 `DIRECTX requires the $DXSDK_DIR environment variable to be set` 时，加 `-DCMAKE_COMPILER_IS_MINGW=1`

## 在自己的工程中使用

在 `CMakeLists.txt` 中：

```cmake
find_package(awtk CONFIG REQUIRED)
add_executable(myapp main.c)
target_link_libraries(myapp PRIVATE awtk::awtk)
```

> 仓库内参考示例：[examples/cmake_find_awtk](examples/cmake_find_awtk/)。

执行cmake时，`CMAKE_PREFIX_PATH` 指向安装目录：

```bash
cmake -S your_app -B your_app/build -DCMAKE_PREFIX_PATH=/path/to/awtk-install
```

> 在 awtk 安装目录 `lib/cmake/awtk/` 下有 `awtkConfig.cmake`、`awtkTargets.cmake`；头文件在 `include/awtk/` 下。
>

## SDL 后端

| `AWTK_SDL_VERSION` | 行为 |
|---|---|
| `2`（默认） | 使用源码树内 `3rd/SDL`，无需系统安装 SDL2 |
| `3` | `find_package(SDL3 3.2.0 REQUIRED CONFIG)`，需预先装好 SDL3 |

## 常用选项

均为 `-DAWTK_XXX=...` 形式的缓存变量，完整列表见 `cmake/awtk_options.cmake`。

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
| `AWTK_NANOVG_BACKEND` | `GLES2` / `GLES3` / `GL3` / `AGGE` 等 |
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
