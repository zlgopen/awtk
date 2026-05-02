# Platform flags derived from awtk_config_common.py (subset for CMake).

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(AWTK_OS_DARWIN TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(AWTK_OS_LINUX TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(AWTK_OS_WINDOWS TRUE)
endif()

# Match awtk_config_common TOOLS_NAME: empty => MSVC-style link, "mingw" => MinGW.
if(AWTK_OS_WINDOWS)
  if(AWTK_TOOLS_NAME STREQUAL "mingw" OR MINGW)
    set(AWTK_WINDOWS_MINGW TRUE)
  else()
    set(AWTK_WINDOWS_MSVC TRUE)
  endif()
endif()

function(awtk_apply_platform_compile_options _target)
  if(AWTK_OS_DARWIN)
    target_compile_options(${_target} PRIVATE
      -Wall -Wno-unused-function -fPIC
      -DWITHOUT_GLAD=1
      -DHAS_SEM_OPEN
      -D__APPLE__ -DHAS_PTHREAD -DMACOS
      -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS
      -DBGFX_CONFIG_RENDERER_METAL=1
    )
  elseif(AWTK_OS_LINUX)
    target_compile_options(${_target} PRIVATE
      -Wall -Wno-unused-function -fPIC
      -std=gnu99
      -DLINUX -DHAS_PTHREAD
    )
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      target_compile_options(${_target} PRIVATE -DWITH_64BIT_CPU)
    else()
      target_compile_options(${_target} PRIVATE -U__FLT_EVAL_METHOD__ -D__FLT_EVAL_METHOD__=0)
    endif()
    if(DEFINED ENV{SDL_VIDEODRIVER} AND "$ENV{SDL_VIDEODRIVER}" STREQUAL "wayland")
      target_compile_definitions(${_target} PRIVATE WITHOUT_NATIVE_FILE_DIALOG SDL_VIDEO_DRIVER_WAYLAND=1)
    endif()
  elseif(AWTK_WINDOWS_MSVC)
    target_compile_options(${_target} PRIVATE
      /EHsc /FS /Z7 /utf-8
      -DWIN32 -DWINDOWS -D_CONSOLE -DHAVE_LIBC
      -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS
      -D_HAS_EXCEPTIONS=0 -D_HAS_ITERATOR_DEBUGGING=0 -D_ITERATOR_DEBUG_LEVEL=0 -D_SCL_SECURE=0
      -D_SECURE_SCL=0 -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE
    )
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      target_compile_options(${_target} PRIVATE -DWITH_64BIT_CPU)
    endif()
  elseif(AWTK_WINDOWS_MINGW)
    target_compile_options(${_target} PRIVATE
      -DWIN32 -DMINGW -DWINDOWS -D_CONSOLE -Wall -std=gnu99
      -U__FLT_EVAL_METHOD__ -D__FLT_EVAL_METHOD__=0 -DDECLSPEC=
      -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS
      -D_HAS_EXCEPTIONS=0 -D_HAS_ITERATOR_DEBUGGING=0 -D_ITERATOR_DEBUG_LEVEL=0 -D_SCL_SECURE=0
    )
  endif()
endfunction()

function(awtk_apply_platform_link_options _target _scope)
  if(AWTK_OS_DARWIN)
    target_link_options(${_target} ${_scope}
      LINKER:-framework,IOKit
      LINKER:-framework,Cocoa
      LINKER:-framework,QuartzCore
      LINKER:-framework,OpenGL
      LINKER:-weak_framework,Metal
      LINKER:-weak_framework,MetalKit
    )
  elseif(AWTK_OS_LINUX)
    # Do not use LINKER:-Wl,-rpath,... — CMake splits LINKER: args on commas, so ld sees bare "-Wl".
    target_link_options(${_target} ${_scope} "-Wl,-rpath,\$ORIGIN/../lib")
  elseif(AWTK_WINDOWS_MINGW)
    target_link_options(${_target} ${_scope} "-Wl,-rpath,./bin" "-Wl,-rpath,./")
  endif()
endfunction()
