include(awtk_platform)
include(awtk_compile_defs)

if(NOT AWTK_SDL_VERSION STREQUAL "2" AND NOT AWTK_SDL_VERSION STREQUAL "3")
  message(FATAL_ERROR "AWTK_SDL_VERSION must be \"2\" or \"3\" (got \"${AWTK_SDL_VERSION}\")")
endif()

if(AWTK_SDL_VERSION STREQUAL "3")
  find_package(SDL3 3.2.0 REQUIRED CONFIG)
  set(AWTK_SDL_LINK_TARGET SDL3::SDL3)
  set(AWTK_USING_BUNDLED_SDL2 FALSE)
  find_path(AWTK_SDL3_INCLUDE_DIR NAMES SDL3/SDL.h REQUIRED)
  set(AWTK_SDL3_EXTRA_INCLUDES "${AWTK_SDL3_INCLUDE_DIR}")
else()
  # Bundled SDL2 from 3rd/SDL (all platforms; matches SCons using in-tree SDL).
  set(_awtk_sdl_build "${CMAKE_BINARY_DIR}/third_party/SDL2")
  file(MAKE_DIRECTORY "${_awtk_sdl_build}")

  set(SDL_TEST OFF CACHE BOOL "SDL2 test programs" FORCE)
  set(SDL_SHARED ON CACHE BOOL "SDL2 shared library" FORCE)
  set(SDL_STATIC OFF CACHE BOOL "SDL2 static library" FORCE)

  # Linux: GTK+3 is linked into SDL2 for X11 DPI. PkgConfig::gtk+-3.0 also exposes Wayland-related
  # includes; bundled SDL 2.0.x Wayland driver then clashes with newer wayland-client (duplicate
  # WAYLAND_* symbols, missing wl_proxy_marshal_flags). AWTK uses X11 here; turn off SDL Wayland video.
  if(AWTK_OS_LINUX)
    set(VIDEO_WAYLAND OFF CACHE BOOL "SDL2 Wayland video driver" FORCE)
  endif()

  # Skip SDL's own install() rules (broken for Debug+macOS OUTPUT_NAME); AWTK installs libSDL2 in CMakeLists.txt.
  set(AWTK_EMBEDDED_SDL2 ON)
  # SDL defaults LIBC=OFF on MSVC (/NODEFAULTLIB). Linking then requires a consistent no-CRT build; force CRT use.
  if(AWTK_WINDOWS_MSVC)
    set(LIBC ON CACHE BOOL "Use the system C library" FORCE)
  endif()
  add_subdirectory("${CMAKE_SOURCE_DIR}/3rd/SDL" "${_awtk_sdl_build}")
  unset(AWTK_EMBEDDED_SDL2)

  # SDL 2.0.x enables -Werror=declaration-after-statement; some .m files are built as C and need C99 / relaxed errors.
  # MSVC does not accept these flags (invalid numeric argument /D8021).
  if(TARGET SDL2 AND NOT AWTK_WINDOWS_MSVC)
    target_compile_options(SDL2 PRIVATE
      -std=gnu99
      -Wno-error=declaration-after-statement
    )
  endif()

  # AWTK fork: SDL_x11window.c includes <gtk/gtk.h> for X11 DPI; SDL's CMake does not add gtk+-3.0 include/link.
  if(AWTK_OS_LINUX AND TARGET SDL2)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(_awtk_sdl_gtk REQUIRED IMPORTED_TARGET gtk+-3.0)
    # SDL2's CMake uses plain target_link_libraries(); mixing PRIVATE would error (CMP0023).
    target_link_libraries(SDL2 PkgConfig::_awtk_sdl_gtk)
  endif()

  if(TARGET SDL2::SDL2)
    set(AWTK_SDL2_TARGET SDL2::SDL2)
  elseif(TARGET SDL2)
    add_library(SDL2::SDL2 ALIAS SDL2)
    set(AWTK_SDL2_TARGET SDL2::SDL2)
  elseif(TARGET SDL2-static)
    add_library(SDL2::SDL2 ALIAS SDL2-static)
    set(AWTK_SDL2_TARGET SDL2::SDL2)
  else()
    message(FATAL_ERROR "Bundled 3rd/SDL did not define target SDL2 or SDL2-static (check SDL_SHARED/SDL_STATIC).")
  endif()

  set(AWTK_SDL_LINK_TARGET "${AWTK_SDL2_TARGET}")
  set(AWTK_USING_BUNDLED_SDL2 TRUE)
  set(AWTK_SDL3_EXTRA_INCLUDES "")
endif()

# One-line summary for logs / CI (matches AWTK_SDL_VERSION cache string "2" or "3").
if(AWTK_SDL_VERSION STREQUAL "3")
  set(_awtk_sdl3_report_ver "")
  if(DEFINED SDL3_VERSION AND NOT "${SDL3_VERSION}" STREQUAL "")
    set(_awtk_sdl3_report_ver "${SDL3_VERSION}")
  else()
    find_program(_awtk_sdl3_config NAMES sdl3-config)
    if(_awtk_sdl3_config)
      execute_process(
        COMMAND "${_awtk_sdl3_config}" --version
        OUTPUT_VARIABLE _awtk_sdl3_report_ver
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
      )
    endif()
  endif()
  if("${_awtk_sdl3_report_ver}" STREQUAL "")
    set(_awtk_sdl3_report_ver "unknown (link: ${AWTK_SDL_LINK_TARGET})")
  endif()
  message(STATUS "AWTK: SDL = 3 (external), version ${_awtk_sdl3_report_ver}, includes ${AWTK_SDL3_INCLUDE_DIR}")
else()
  # SDL_VERSION is set only in 3rd/SDL's directory scope, not visible here — read shipped headers.
  set(_awtk_sdl2_report_ver "unknown")
  set(_awtk_sdl_ver_h "${CMAKE_SOURCE_DIR}/3rd/SDL/include/SDL_version.h")
  if(EXISTS "${_awtk_sdl_ver_h}")
    file(READ "${_awtk_sdl_ver_h}" _awtk_sdl_ver_h_content)
    set(_awtk_sdl2_maj "")
    set(_awtk_sdl2_min "")
    set(_awtk_sdl2_pat "")
    if(_awtk_sdl_ver_h_content MATCHES "#define SDL_MAJOR_VERSION[ \t]+([0-9]+)")
      set(_awtk_sdl2_maj "${CMAKE_MATCH_1}")
    endif()
    if(_awtk_sdl_ver_h_content MATCHES "#define SDL_MINOR_VERSION[ \t]+([0-9]+)")
      set(_awtk_sdl2_min "${CMAKE_MATCH_1}")
    endif()
    if(_awtk_sdl_ver_h_content MATCHES "#define SDL_PATCHLEVEL[ \t]+([0-9]+)")
      set(_awtk_sdl2_pat "${CMAKE_MATCH_1}")
    endif()
    if(NOT _awtk_sdl2_maj STREQUAL "" AND NOT _awtk_sdl2_min STREQUAL "" AND NOT _awtk_sdl2_pat STREQUAL "")
      set(_awtk_sdl2_report_ver "${_awtk_sdl2_maj}.${_awtk_sdl2_min}.${_awtk_sdl2_pat}")
    endif()
  endif()
  message(STATUS "AWTK: SDL = 2 (bundled 3rd/SDL ${_awtk_sdl2_report_ver}, link: ${AWTK_SDL_LINK_TARGET})")
endif()

file(GLOB AWTK_GLAD_SRC CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/3rd/glad/*.c")
add_library(awtk_glad STATIC ${AWTK_GLAD_SRC})
target_include_directories(awtk_glad PUBLIC "${CMAKE_SOURCE_DIR}/3rd/glad")
awtk_apply_platform_compile_options(awtk_glad)
awtk_apply_common_compile_definitions(awtk_glad)

file(GLOB AWTK_MBEDTLS_SRC CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/3rd/mbedtls/library/*.c")
add_library(awtk_mbedtls STATIC ${AWTK_MBEDTLS_SRC})
target_include_directories(awtk_mbedtls PUBLIC
  "${CMAKE_SOURCE_DIR}/3rd/mbedtls/include"
  "${CMAKE_SOURCE_DIR}/3rd/mbedtls/3rdparty/everest/include"
)
awtk_apply_platform_compile_options(awtk_mbedtls)
awtk_apply_common_compile_definitions(awtk_mbedtls)

set(AWTK_LZ4_SRC
  "${CMAKE_SOURCE_DIR}/3rd/lz4/lz4.c"
  "${CMAKE_SOURCE_DIR}/3rd/lz4/lz4frame.c"
  "${CMAKE_SOURCE_DIR}/3rd/lz4/lz4hc.c"
  "${CMAKE_SOURCE_DIR}/3rd/lz4/xxhash.c"
)
add_library(awtk_lz4 STATIC ${AWTK_LZ4_SRC})
target_include_directories(awtk_lz4 PUBLIC "${CMAKE_SOURCE_DIR}/3rd/lz4")
awtk_apply_platform_compile_options(awtk_lz4)
awtk_apply_common_compile_definitions(awtk_lz4)

set(AWTK_MINIZ_SRC
  "${CMAKE_SOURCE_DIR}/3rd/miniz/miniz.c"
  "${CMAKE_SOURCE_DIR}/3rd/miniz/miniz_tdef.c"
  "${CMAKE_SOURCE_DIR}/3rd/miniz/miniz_tinfl.c"
  "${CMAKE_SOURCE_DIR}/3rd/miniz/miniz_zip.c"
)
add_library(awtk_miniz STATIC ${AWTK_MINIZ_SRC})
target_include_directories(awtk_miniz PUBLIC "${CMAKE_SOURCE_DIR}/3rd/miniz")
awtk_apply_platform_compile_options(awtk_miniz)
awtk_apply_common_compile_definitions(awtk_miniz)

if(NOT AWTK_BIDI_BACKEND STREQUAL "sheenbidi" AND NOT AWTK_BIDI_BACKEND STREQUAL "fribidi")
  message(FATAL_ERROR "AWTK_BIDI_BACKEND must be \"sheenbidi\" or \"fribidi\" (got \"${AWTK_BIDI_BACKEND}\")")
endif()

if(AWTK_BIDI_BACKEND STREQUAL "fribidi")
  set(_awtk_fribidi_main "${CMAKE_SOURCE_DIR}/3rd/fribidi/fribidi.c")
  if(NOT EXISTS "${_awtk_fribidi_main}")
    message(FATAL_ERROR "AWTK_BIDI_BACKEND=fribidi but bundled sources are missing: ${_awtk_fribidi_main}")
  endif()
  file(GLOB AWTK_FRIBIDI_SRC CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/3rd/fribidi/*.c")
  add_library(awtk_fribidi STATIC ${AWTK_FRIBIDI_SRC})
  target_include_directories(awtk_fribidi PUBLIC "${CMAKE_SOURCE_DIR}/3rd/fribidi")
  awtk_apply_platform_compile_options(awtk_fribidi)
  awtk_apply_common_compile_definitions(awtk_fribidi)
  set(AWTK_BIDI_THIRD_PARTY_TARGET awtk_fribidi)
else()
  set(_awtk_sheen_main "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBAlgorithm.c")
  if(NOT EXISTS "${_awtk_sheen_main}")
    message(FATAL_ERROR "AWTK_BIDI_BACKEND=sheenbidi but SheenBidi sources are missing: ${_awtk_sheen_main}")
  endif()
  set(AWTK_SHEENBIDI_SRC
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBAlgorithm.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBAllocator.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBAttributeList.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBAttributeRegistry.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBBase.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBCodepoint.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBCodepointSequence.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBLine.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBLog.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBMirrorLocator.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBParagraph.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBScriptLocator.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBText.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBTextConfig.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/API/SBTextIterators.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/Core/List.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/Core/Memory.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/Core/Object.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/Core/Once.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/Data/BidiTypeLookup.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/Data/GeneralCategoryLookup.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/Data/PairingLookup.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/Data/ScriptLookup.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/Script/ScriptStack.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/Text/AttributeDictionary.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/Text/AttributeManager.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/UBA/BidiChain.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/UBA/BracketQueue.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/UBA/IsolatingRun.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/UBA/LevelRun.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/UBA/RunQueue.c"
    "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source/UBA/StatusStack.c"
  )
  add_library(awtk_sheenbidi STATIC ${AWTK_SHEENBIDI_SRC})
  target_include_directories(awtk_sheenbidi PUBLIC "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Headers")
  target_include_directories(awtk_sheenbidi PRIVATE "${CMAKE_SOURCE_DIR}/3rd/SheenBidi-3.0.0/Source")
  target_compile_definitions(awtk_sheenbidi PRIVATE SB_CONFIG_EXPERIMENTAL_TEXT_API=1)
  awtk_apply_platform_compile_options(awtk_sheenbidi)
  awtk_apply_common_compile_definitions(awtk_sheenbidi)
  set(AWTK_BIDI_THIRD_PARTY_TARGET awtk_sheenbidi)
endif()

set(AWTK_LINEBREAK_SRC
  graphemebreak.c linebreak.c linebreakdef.c unibreakbase.c unibreakdef.c
  wordbreak.c emojidef.c linebreakdata.c
)
list(TRANSFORM AWTK_LINEBREAK_SRC PREPEND "${CMAKE_SOURCE_DIR}/3rd/libunibreak/")
add_library(awtk_linebreak STATIC ${AWTK_LINEBREAK_SRC})
target_include_directories(awtk_linebreak PUBLIC "${CMAKE_SOURCE_DIR}/3rd/libunibreak")
awtk_apply_platform_compile_options(awtk_linebreak)
awtk_apply_common_compile_definitions(awtk_linebreak)

file(GLOB AWTK_SVGTINY_SRC CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/3rd/svgtiny/src/*.c")
add_library(awtk_svgtiny STATIC ${AWTK_SVGTINY_SRC})
target_include_directories(awtk_svgtiny PUBLIC
  "${CMAKE_SOURCE_DIR}/3rd/svgtiny/include"
  "${CMAKE_SOURCE_DIR}/3rd/svgtiny"
  "${CMAKE_SOURCE_DIR}/src"
)
awtk_apply_platform_compile_options(awtk_svgtiny)
awtk_apply_common_compile_definitions(awtk_svgtiny)

file(GLOB AWTK_GPINYIN_SRC CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/3rd/gpinyin/src/*.cpp")
add_library(awtk_gpinyin STATIC ${AWTK_GPINYIN_SRC})
target_include_directories(awtk_gpinyin PUBLIC
  "${CMAKE_SOURCE_DIR}/3rd/gpinyin/include"
  "${CMAKE_SOURCE_DIR}/src"
)
set_target_properties(awtk_gpinyin PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
awtk_apply_platform_compile_options(awtk_gpinyin)
awtk_apply_common_compile_definitions(awtk_gpinyin)

file(GLOB AWTK_NANOVG_PLUS_SRC CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/3rd/nanovg_plus/base/*.c"
  "${CMAKE_SOURCE_DIR}/3rd/nanovg_plus/gl/*.c"
)
add_library(awtk_nanovg_plus STATIC ${AWTK_NANOVG_PLUS_SRC})
target_include_directories(awtk_nanovg_plus PUBLIC
  "${CMAKE_SOURCE_DIR}/3rd/nanovg_plus"
  "${CMAKE_SOURCE_DIR}/3rd/nanovg_plus/base"
  "${CMAKE_SOURCE_DIR}/3rd/nanovg_plus/gl"
  "${CMAKE_SOURCE_DIR}/3rd"
  "${CMAKE_SOURCE_DIR}/src"
)
awtk_apply_platform_compile_options(awtk_nanovg_plus)
awtk_apply_common_compile_definitions(awtk_nanovg_plus)
target_link_libraries(awtk_nanovg_plus PRIVATE "$<LINK_ONLY:${AWTK_SDL_LINK_TARGET}>")

set(AWTK_NFD_BUILT FALSE)
if(AWTK_WITH_NFD)
  if(DEFINED ENV{SDL_VIDEODRIVER})
    message(STATUS "AWTK: SDL_VIDEODRIVER is set; skipping nfd (matches awtk_config.py)")
  elseif(AWTK_OS_LINUX)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
      pkg_check_modules(AWTK_GTK IMPORTED_TARGET gtk+-3.0)
    endif()
    if(AWTK_GTK_FOUND)
      add_library(awtk_nfd STATIC
        "${CMAKE_SOURCE_DIR}/3rd/nativefiledialog/src/nfd_common.c"
        "${CMAKE_SOURCE_DIR}/3rd/nativefiledialog/src/nfd_gtk.c"
      )
      target_include_directories(awtk_nfd PUBLIC
        "${CMAKE_SOURCE_DIR}/3rd/nativefiledialog/src"
        "${CMAKE_SOURCE_DIR}/3rd/nativefiledialog/src/include"
      )
      if(AWTK_USING_BUNDLED_SDL2)
        target_compile_definitions(awtk_nfd PRIVATE SDL_STATIC_LIB)
      endif()
      target_compile_definitions(awtk_nfd PRIVATE __FLTUSED__=1)
      target_link_libraries(awtk_nfd PRIVATE PkgConfig::AWTK_GTK)
      awtk_apply_platform_compile_options(awtk_nfd)
      awtk_apply_common_compile_definitions(awtk_nfd)
      set(AWTK_NFD_BUILT TRUE)
    else()
      message(WARNING "AWTK: gtk+-3.0 not found via pkg-config; nfd disabled")
    endif()
  elseif(AWTK_OS_DARWIN)
    enable_language(OBJC)
    add_library(awtk_nfd STATIC
      "${CMAKE_SOURCE_DIR}/3rd/nativefiledialog/src/nfd_common.c"
      "${CMAKE_SOURCE_DIR}/3rd/nativefiledialog/src/nfd_cocoa.m"
    )
    target_include_directories(awtk_nfd PUBLIC
      "${CMAKE_SOURCE_DIR}/3rd/nativefiledialog/src"
      "${CMAKE_SOURCE_DIR}/3rd/nativefiledialog/src/include"
    )
    if(AWTK_USING_BUNDLED_SDL2)
      target_compile_definitions(awtk_nfd PRIVATE SDL_STATIC_LIB)
    endif()
    target_compile_definitions(awtk_nfd PRIVATE __FLTUSED__=1)
    awtk_apply_platform_compile_options(awtk_nfd)
    awtk_apply_common_compile_definitions(awtk_nfd)
    set(AWTK_NFD_BUILT TRUE)
  elseif(AWTK_OS_WINDOWS)
    add_library(awtk_nfd STATIC
      "${CMAKE_SOURCE_DIR}/3rd/nativefiledialog/src/nfd_common.c"
      "${CMAKE_SOURCE_DIR}/3rd/nativefiledialog/src/nfd_win.cpp"
    )
    target_include_directories(awtk_nfd PUBLIC
      "${CMAKE_SOURCE_DIR}/3rd/nativefiledialog/src"
      "${CMAKE_SOURCE_DIR}/3rd/nativefiledialog/src/include"
    )
    if(AWTK_USING_BUNDLED_SDL2)
      target_compile_definitions(awtk_nfd PRIVATE SDL_STATIC_LIB)
    endif()
    target_compile_definitions(awtk_nfd PRIVATE __FLTUSED__=1)
    awtk_apply_platform_compile_options(awtk_nfd)
    awtk_apply_common_compile_definitions(awtk_nfd)
    set(AWTK_NFD_BUILT TRUE)
  endif()
endif()

set(AWTK_THIRD_PARTY_TARGETS
  awtk_glad
  awtk_mbedtls
  awtk_lz4
  awtk_miniz
  ${AWTK_BIDI_THIRD_PARTY_TARGET}
  awtk_linebreak
  awtk_svgtiny
  awtk_gpinyin
  awtk_nanovg_plus
)
if(AWTK_NFD_BUILT)
  list(APPEND AWTK_THIRD_PARTY_TARGETS awtk_nfd)
endif()

foreach(_t IN LISTS AWTK_THIRD_PARTY_TARGETS)
  add_dependencies(${_t} awtk_idl_codegen)
endforeach()
