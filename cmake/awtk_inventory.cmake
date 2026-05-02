#[[
  AWTK SCons -> CMake mapping (inventory). Keep in sync with SConstruct / awtk_config.py.

  Top-level entry (legacy): SConstruct loads compile_config + awtk_config, then SConscript().

  Subdirectory boundaries (CMake add_subdirectory or include() modules):
  - cmake/third_party_libs.cmake : 3rd/cjson, glad, mbedtls, lz4, miniz, fribidi,
    libunibreak (linebreak), svgtiny, gpinyin, nanovg_plus, nativefiledialog (optional),
    SDL2 (bundled add_subdirectory(3rd/SDL), default) or SDL3 (find_package, AWTK_SDL_VERSION=3).
  - cmake/src_libs.cmake        : src/SConscript + src/*/SConscript static libs,
    then final libawtk SHARED (mirrors env.SharedLibrary for awtk).

  Environment / options mirrored from awtk_config.py + scripts/compile_config.py:
  - INPUT_ENGINE, VGCANVAS, NANOVG_BACKEND, NATIVE_WINDOW, LCD (derived),
    GRAPHIC_BUFFER, OPENGL_ANTIALIAS, DEBUG, TOOLS_NAME (mingw vs MSVC),
    BUILD_TOOLS, BUILD_DEMOS, BUILD_TESTS, SDL_VIDEODRIVER / wayland flags, etc.

  Codegen (SConstruct awtk.genIdlAndDef):
  - Node: tools/idl_gen/tkc.js, tools/idl_gen/index.js, tools/dll_def_gen/index.js
  - Outputs: dllexports/*.def and generated bindings (see awtk_config_common.genIdlAndDefEx).

  Install / consumer surface:
  - Targets exported as awtk::awtk; CMake package awtkConfig.cmake from AwtkConfig.cmake.in.
]]
