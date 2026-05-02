cmake_minimum_required(VERSION 3.20)
project(awtk LANGUAGES C CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

include(awtk_options)

if(NOT CMAKE_BUILD_TYPE)
  if(AWTK_DEBUG)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
  else()
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
  endif()
endif()

include(awtk_platform)

if(AWTK_OS_LINUX OR AWTK_OS_DARWIN)
  find_package(PkgConfig QUIET)
endif()

include(awtk_codegen)
include(third_party_libs)
include(src_libs)
include(awtk_link_bundle)

if(AWTK_BUILD_SHARED)
  add_library(awtk SHARED "${CMAKE_CURRENT_SOURCE_DIR}/cmake/empty.c")
else()
  add_library(awtk STATIC "${CMAKE_CURRENT_SOURCE_DIR}/cmake/empty.c")
endif()

set_target_properties(awtk PROPERTIES OUTPUT_NAME "awtk" LINKER_LANGUAGE CXX)
target_include_directories(awtk PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/awtk>
)

add_dependencies(awtk awtk_idl_codegen)
add_dependencies(awtk ${AWTK_WHOLE_ARCHIVE_LIBS})
if(TARGET SDL2)
  add_dependencies(awtk SDL2)
endif()

# Pack internal static libs into libawtk (see cmake/awtk_link_bundle.cmake + awtk_config_common.py).
awtk_link_whole_archive_static_libs(awtk)
if(MSVC)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/dllexports/awtk.def")
    target_link_options(awtk PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/dllexports/awtk.def")
  endif()
  if(AWTK_DEBUG AND AWTK_PDB)
    target_link_options(awtk PRIVATE /DEBUG)
  endif()
elseif(AWTK_WINDOWS_MINGW)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/dllexports/awtk.def")
    target_link_options(awtk PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/dllexports/awtk.def")
  endif()
endif()

if(NOT AWTK_OS_DARWIN)
  target_link_libraries(awtk PRIVATE awtk_glad)
endif()

# LINK_ONLY: SDL is linked into libawtk but must not appear in export() / install(EXPORT) dependency graph.
target_link_libraries(awtk PRIVATE "$<LINK_ONLY:${AWTK_SDL_LINK_TARGET}>")

find_package(Threads REQUIRED)

if(AWTK_OS_LINUX)
  find_package(OpenGL REQUIRED)
  find_library(AWTK_X11_LIB X11 REQUIRED)
  find_library(AWTK_XEXT_LIB Xext REQUIRED)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(AWTK_GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0)
  target_link_libraries(awtk PRIVATE
    OpenGL::GL
    ${AWTK_X11_LIB}
    ${AWTK_XEXT_LIB}
    PkgConfig::AWTK_GTK3
    Threads::Threads
    m
    stdc++
    ${CMAKE_DL_LIBS}
  )
  find_library(AWTK_ASOUND asound)
  if(AWTK_ASOUND)
    target_link_libraries(awtk PRIVATE ${AWTK_ASOUND})
  endif()
  find_library(AWTK_SNDIO sndio)
  if(AWTK_SNDIO)
    target_link_libraries(awtk PRIVATE ${AWTK_SNDIO})
  endif()
elseif(AWTK_OS_DARWIN)
  target_link_libraries(awtk PRIVATE
    Threads::Threads
    iconv
    "-framework IOKit"
    "-framework Cocoa"
    "-framework QuartzCore"
    "-framework OpenGL"
    "-weak_framework Metal"
    "-weak_framework MetalKit"
  )
elseif(AWTK_OS_WINDOWS AND AWTK_WINDOWS_MSVC)
  target_link_libraries(awtk PRIVATE
    gdi32 user32 winmm imm32 version shell32 Setupapi ole32 oleaut32 advapi32 delayimp psapi ws2_32
  )
elseif(AWTK_WINDOWS_MINGW)
  target_link_libraries(awtk PRIVATE
    kernel32 gdi32 user32 winmm imm32 version shell32 Setupapi ole32 oleaut32 advapi32 uuid stdc++ ws2_32 iphlpapi
  )
endif()

awtk_apply_platform_link_options(awtk PRIVATE)

if(AWTK_DEBUG AND (AWTK_OS_LINUX OR AWTK_OS_DARWIN))
  target_compile_options(awtk PRIVATE -g -O0)
elseif(NOT AWTK_DEBUG AND (AWTK_OS_LINUX OR AWTK_OS_DARWIN))
  target_compile_options(awtk PRIVATE -Os)
endif()

# --- Install + CMake package ---
set(AWTK_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/awtk")

if(AWTK_OS_DARWIN)
  set_target_properties(awtk PROPERTIES INSTALL_RPATH "@loader_path")
elseif(AWTK_OS_LINUX)
  set_target_properties(awtk PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()

set(_awtk_export_install_targets awtk)
if(AWTK_SDL_VERSION STREQUAL "2")
  if(TARGET SDL2)
    list(APPEND _awtk_export_install_targets SDL2)
  endif()
  if(TARGET SDL2main)
    list(APPEND _awtk_export_install_targets SDL2main)
  endif()
endif()
install(TARGETS ${_awtk_export_install_targets} EXPORT awtkTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/awtk"
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rd/fribidi/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/awtk/3rd/fribidi"
  FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rd/mbedtls/include/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/awtk/3rd/mbedtls/include"
  FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rd/gpinyin/include/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/awtk/3rd/gpinyin/include"
  FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rd/libunibreak/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/awtk/3rd/libunibreak"
  FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rd/svgtiny/include/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/awtk/3rd/svgtiny/include"
  FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rd/nanovg_plus/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/awtk/3rd/nanovg_plus"
  FILES_MATCHING PATTERN "*.h"
)

install(EXPORT awtkTargets
  FILE awtkTargets.cmake
  NAMESPACE awtk::
  DESTINATION "${AWTK_INSTALL_CMAKEDIR}"
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/AwtkConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/export/awtkConfig.cmake"
  INSTALL_DESTINATION "${AWTK_INSTALL_CMAKEDIR}"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/awtkConfigVersion.cmake"
  VERSION 1.7.0
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/export/awtkConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/awtkConfigVersion.cmake"
  DESTINATION "${AWTK_INSTALL_CMAKEDIR}"
)

# Build-tree package: IMPORTED awtk::awtk (cannot use export(TARGETS) while linking bundled SDL2).
# Use configure_file (not file(GENERATE)) so $<TARGET_FILE:awtk> stays literal; multi-config
# generators otherwise evaluate it per-config and clash on a single output path.
if(AWTK_BUILD_SHARED)
  set(AWTK_IMPORT_KIND "SHARED")
else()
  set(AWTK_IMPORT_KIND "STATIC")
endif()
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/awtk_build_tree_exports.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/awtk_build_tree_exports.cmake"
  @ONLY
)
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/AwtkConfigBuildTree.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/awtkConfig.cmake"
  @ONLY
)

option(AWTK_BUILD_CMAKE_FIND_DEMO "Build examples/cmake_find_awtk against the build tree" ON)
if(AWTK_BUILD_CMAKE_FIND_DEMO)
  add_subdirectory(examples/cmake_find_awtk)
endif()

if(AWTK_BUILD_DEMOS)
  add_subdirectory(demos)
endif()
