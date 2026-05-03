# Cache options aligned with scripts/compile_config.py (awtk_config_define / SCons sopt).

option(AWTK_DEBUG "Debug build (-g / no optimizations on GCC-like)" ON)
option(AWTK_PDB "MSVC: emit PDB" ON)
option(AWTK_BUILD_TOOLS "Build resource / developer tools (SCons BUILD_TOOLS)" ON)
option(AWTK_BUILD_DEMOS "Build demos" ON)
option(AWTK_BUILD_TESTS "Build tests" ON)
option(AWTK_BUILD_SHARED "Build libawtk as SHARED" ON)

# Demos always link libawtk as a shared library on all platforms (static libawtk does not propagate
# whole-archive deps to executables reliably).
if(AWTK_BUILD_DEMOS AND NOT AWTK_BUILD_SHARED)
  message(STATUS "AWTK_BUILD_DEMOS is ON: forcing AWTK_BUILD_SHARED=ON (demos require shared libawtk).")
  set(AWTK_BUILD_SHARED ON CACHE BOOL "Build libawtk as SHARED" FORCE)
endif()

set(AWTK_OUTPUT_DIR "" CACHE PATH "Install / output prefix override (maps OUTPUT_DIR)")
set(AWTK_TOOLS_NAME "" CACHE STRING "Windows: empty for MSVC, 'mingw' for MinGW (maps TOOLS_NAME)")
set(AWTK_INPUT_ENGINE "pinyin" CACHE STRING "null|spinyin|t9|t9ext|pinyin")
set(AWTK_VGCANVAS "NANOVG_PLUS" CACHE STRING "NANOVG|NANOVG_PLUS|CAIRO")
set(AWTK_NANOVG_BACKEND "GL3" CACHE STRING "GLES2|GLES3|GL3|AGG|AGGE|BGFX (valid combos depend on VGCANVAS)")
set(AWTK_OPENGL_ANTIALIAS "HW" CACHE STRING "HW|SW|NONE")
set(AWTK_LCD_COLOR_FORMAT "" CACHE STRING "bgr565|bgra8888|mono or empty for defaults")
set(AWTK_WIN32_RES "" CACHE FILEPATH "Optional Windows .res path (WIN32_RES)")

option(AWTK_SDL_UBUNTU_USE_IME "SDL Ubuntu IME" OFF)
option(AWTK_NATIVE_WINDOW_BORDERLESS "Borderless native window" OFF)
option(AWTK_NATIVE_WINDOW_NOT_RESIZABLE "Non-resizable native window" OFF)
option(AWTK_WITH_NFD "Build nativefiledialog (off if SDL_VIDEODRIVER set in env)" ON)

set(AWTK_BIDI_BACKEND "sheenbidi" CACHE STRING "Unicode bidi backend: sheenbidi (default) or fribidi")
set_property(CACHE AWTK_BIDI_BACKEND PROPERTY STRINGS sheenbidi fribidi)

set(AWTK_SDL_VIDEODRIVER "" CACHE STRING "Linux: x11|wayland (empty uses x11; matches SDL_VIDEODRIVER env in SCons)")

set(AWTK_SDL_VERSION "2" CACHE STRING "SDL backend: 2 (bundled 3rd/SDL) or 3 (find_package SDL3)")
set_property(CACHE AWTK_SDL_VERSION PROPERTY STRINGS 2 3)

option(AWTK_RUN_IDL_CODEGEN "Run Node idl/def generators at build time (requires node; OFF skips)" OFF)
