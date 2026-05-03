# Common compile definitions (mirrors awtk_config.py COMMON_CCFLAGS + branches).

function(awtk_apply_common_compile_definitions _target)
  if(AWTK_SDL_VERSION STREQUAL "3")
    target_compile_definitions(${_target} PRIVATE AWTK_SDL3=1)
  endif()

  # Match awtk_config.py: -DTK_ROOT as a C string literal for the repo root.
  cmake_path(NATIVE_PATH CMAKE_SOURCE_DIR NORMALIZE _tkr)
  string(REPLACE "\\" "\\\\" _tkr "${_tkr}")
  target_compile_options(${_target} PRIVATE "SHELL:-DTK_ROOT=\\\"${_tkr}\\\"")
  if(AWTK_BIDI_BACKEND STREQUAL "fribidi")
    set(_awtk_bidi_compile_def WITH_BIDI_FRIBIDI=1)
  else()
    set(_awtk_bidi_compile_def WITH_BIDI_SHEEN=1)
  endif()
  target_compile_definitions(${_target} PRIVATE
    WITH_MBEDTLS=1
    ENABLE_CURSOR=1
    WITH_TEXT_BIDI=1
    ${_awtk_bidi_compile_def}
    WITH_DATA_READER_WRITER=1
    WITH_EVENT_RECORDER_PLAYER=1
    WITH_ASSET_LOADER
    WITH_FS_RES
    WITH_ASSET_LOADER_ZIP
    STBTT_STATIC
    STB_IMAGE_STATIC
    WITH_STB_IMAGE
    WITH_VGCANVAS
    WITH_UNICODE_BREAK
    WITH_DESKTOP_STYLE
    WITH_SDL
    HAS_STDIO
    HAVE_STDIO_H
    HAS_GET_TIME_US64
    HAS_STD_MALLOC
    TK_MAX_MEM_BLOCK_NR=3
    WITH_RES_TOOLS
    WITH_MAIN_LOOP_CONSOLE=1
  )
  if(NOT AWTK_BIDI_BACKEND STREQUAL "fribidi")
    target_compile_definitions(${_target} PRIVATE SB_CONFIG_EXPERIMENTAL_TEXT_API=1)
  endif()

  if(AWTK_OPENGL_ANTIALIAS STREQUAL "HW")
    target_compile_definitions(${_target} PRIVATE WITH_ANTIALIAS WITH_OPENGL_HW_ANTIALIAS)
  elseif(AWTK_OPENGL_ANTIALIAS STREQUAL "SW")
    target_compile_definitions(${_target} PRIVATE WITH_ANTIALIAS)
  endif()

  if(AWTK_NATIVE_WINDOW_BORDERLESS)
    target_compile_definitions(${_target} PRIVATE NATIVE_WINDOW_BORDERLESS=1)
  endif()
  if(AWTK_NATIVE_WINDOW_NOT_RESIZABLE)
    target_compile_definitions(${_target} PRIVATE NATIVE_WINDOW_NOT_RESIZABLE=1)
  endif()

  if(AWTK_INPUT_ENGINE STREQUAL "t9")
    target_compile_definitions(${_target} PRIVATE WITH_IME_T9)
  elseif(AWTK_INPUT_ENGINE STREQUAL "t9ext")
    target_compile_definitions(${_target} PRIVATE WITH_IME_T9EXT)
  elseif(AWTK_INPUT_ENGINE STREQUAL "pinyin")
    target_compile_definitions(${_target} PRIVATE WITH_IME_PINYIN)
  elseif(AWTK_INPUT_ENGINE STREQUAL "spinyin")
    target_compile_definitions(${_target} PRIVATE WITH_IME_SPINYIN)
  elseif(AWTK_INPUT_ENGINE STREQUAL "null")
    target_compile_definitions(${_target} PRIVATE WITH_IME_NULL)
  endif()

  if(AWTK_VGCANVAS STREQUAL "CAIRO")
    message(FATAL_ERROR "AWTK_VGCANVAS=CAIRO is not wired in CMake yet")
  elseif(AWTK_VGCANVAS STREQUAL "NANOVG_PLUS")
    target_compile_definitions(${_target} PRIVATE
      WITH_NANOVG_PLUS_GPU
      WITH_NANOVG_GPU
      WITH_GPU_GL
    )
    if(AWTK_NANOVG_BACKEND STREQUAL "GLES2")
      target_compile_definitions(${_target} PRIVATE WITH_GPU_GLES2 NVGP_GLES2)
    elseif(AWTK_NANOVG_BACKEND STREQUAL "GLES3")
      target_compile_definitions(${_target} PRIVATE WITH_GPU_GLES3 NVGP_GLES3)
    else()
      target_compile_definitions(${_target} PRIVATE WITH_GPU_GL3 NVGP_GL3)
    endif()
  else()
    message(FATAL_ERROR "AWTK_VGCANVAS=${AWTK_VGCANVAS} not wired in CMake yet (use NANOVG_PLUS)")
  endif()

  # LCD path defaults for NANOVG_PLUS + desktop GL3 (matches awtk_config.py)
  set(_lcd "SDL_GPU")
  if(AWTK_VGCANVAS STREQUAL "NANOVG_PLUS" AND AWTK_NANOVG_BACKEND STREQUAL "AGGE")
    set(_lcd "SDL_FB")
  endif()
  if(_lcd STREQUAL "SDL_GPU")
    target_compile_definitions(${_target} PRIVATE WITH_GPU WITH_VGCANVAS_LCD WITH_STB_FONT)
  else()
    target_compile_definitions(${_target} PRIVATE WITH_STB_FONT WITH_BITMAP_BGRA WITH_NANOVG_SOFT WITH_FB_BGR565=1)
  endif()
endfunction()
