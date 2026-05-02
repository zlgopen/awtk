# Static libraries under src/ (mirrors src/SConscript + src/*/SConscript).

file(GLOB _cw_inc
  "${CMAKE_SOURCE_DIR}/src/custom_widgets/*/src"
  "${CMAKE_SOURCE_DIR}/src/custom_widgets/*/src/*/src"
  "${CMAKE_SOURCE_DIR}/src/custom_widgets/*/src/*/include"
)

set(AWTK_SRC_INCLUDES
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/3rd"
  "${CMAKE_SOURCE_DIR}/src/ext_widgets"
  "${CMAKE_SOURCE_DIR}/src/custom_widgets"
  "${CMAKE_SOURCE_DIR}/3rd/fribidi"
  "${CMAKE_SOURCE_DIR}/3rd/mbedtls/include"
  "${CMAKE_SOURCE_DIR}/3rd/mbedtls/3rdparty/everest/include"
  "${CMAKE_SOURCE_DIR}/3rd/gpinyin/include"
  "${CMAKE_SOURCE_DIR}/3rd/libunibreak"
  "${CMAKE_SOURCE_DIR}/3rd/gtest/googletest"
  "${CMAKE_SOURCE_DIR}/3rd/gtest/googletest/include"
  "${CMAKE_SOURCE_DIR}/tools"
  "${CMAKE_SOURCE_DIR}/3rd/nanovg_plus/gl"
  "${CMAKE_SOURCE_DIR}/3rd/nanovg_plus/base"
  "${CMAKE_SOURCE_DIR}/3rd/svgtiny/include"
  "${CMAKE_SOURCE_DIR}/res"
  ${_cw_inc}
)

if(AWTK_SDL_VERSION STREQUAL "2")
  list(APPEND AWTK_SRC_INCLUDES
    "${CMAKE_SOURCE_DIR}/3rd/SDL/src"
    "${CMAKE_SOURCE_DIR}/3rd/SDL/include"
  )
elseif(AWTK_SDL3_EXTRA_INCLUDES)
  list(APPEND AWTK_SRC_INCLUDES ${AWTK_SDL3_EXTRA_INCLUDES})
endif()

macro(awtk_src_lib _name)
  add_library(${_name} STATIC ${ARGN})
  target_include_directories(${_name} PUBLIC ${AWTK_SRC_INCLUDES})
  awtk_apply_platform_compile_options(${_name})
  awtk_apply_common_compile_definitions(${_name})
  add_dependencies(${_name} awtk_idl_codegen)
endmacro()

# --- base (src/SConscript) ---
set(_base_src
  "${CMAKE_SOURCE_DIR}/src/lcd/lcd_mono.c"
  "${CMAKE_SOURCE_DIR}/src/main_loop/main_loop_simple.c"
  "${CMAKE_SOURCE_DIR}/src/main_loop/main_loop_console.c"
  "${CMAKE_SOURCE_DIR}/src/lcd/lcd_nanovg.c"
  "${CMAKE_SOURCE_DIR}/src/main_loop/main_loop_sdl.c"
  "${CMAKE_SOURCE_DIR}/src/input_methods/input_method_creator.c"
  "${CMAKE_SOURCE_DIR}/src/vgcanvas/vgcanvas_nanovg_plus.c"
  "${CMAKE_SOURCE_DIR}/src/vgcanvas/vgcanvas_nanovg_gl2.c"
  "${CMAKE_SOURCE_DIR}/src/vgcanvas/vgcanvas_nanovg_gl3.c"
)
file(GLOB _base_glob CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/layouters/*.c"
  "${CMAKE_SOURCE_DIR}/src/base/*.c"
  "${CMAKE_SOURCE_DIR}/src/ui_loader/*.c"
  "${CMAKE_SOURCE_DIR}/src/svg/*.c"
  "${CMAKE_SOURCE_DIR}/src/clip_board/*.c"
  "${CMAKE_SOURCE_DIR}/src/font_loader/*.c"
  "${CMAKE_SOURCE_DIR}/src/blend/*.c"
  "${CMAKE_SOURCE_DIR}/src/image_loader/*.c"
  "${CMAKE_SOURCE_DIR}/src/designer_support/*.c"
  "${CMAKE_SOURCE_DIR}/src/widget_animators/*.c"
  "${CMAKE_SOURCE_DIR}/src/window_animators/*.c"
  "${CMAKE_SOURCE_DIR}/src/dialog_highlighters/*.c"
  "${CMAKE_SOURCE_DIR}/src/service/*.c"
  "${CMAKE_SOURCE_DIR}/src/remote_ui/service/*.c"
  "${CMAKE_SOURCE_DIR}/src/remote_ui/client/*.c"
  "${CMAKE_SOURCE_DIR}/src/remote_ui/shared/*.c"
  "${CMAKE_SOURCE_DIR}/src/window_manager/window_manager_default.c"
  "${CMAKE_SOURCE_DIR}/src/graphic_buffer/graphic_buffer_default.c"
  "${CMAKE_SOURCE_DIR}/src/native_window/native_window_sdl.c"
  "${CMAKE_SOURCE_DIR}/src/lcd/lcd_mem_*.c"
  "${CMAKE_SOURCE_DIR}/src/input_engines/input_engine_pinyin.cpp"
)
list(APPEND _base_src ${_base_glob})
awtk_src_lib(awtk_base ${_base_src})
set_source_files_properties("${CMAKE_SOURCE_DIR}/src/input_engines/input_engine_pinyin.cpp" PROPERTIES CXX_STANDARD 11)

file(GLOB _widgets CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/widgets/*.c")
awtk_src_lib(awtk_widgets ${_widgets})

file(GLOB _extw CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/ext_widgets/*.c"
  "${CMAKE_SOURCE_DIR}/src/ext_widgets/*/*.c"
  "${CMAKE_SOURCE_DIR}/src/custom_widgets/*.c"
  "${CMAKE_SOURCE_DIR}/src/custom_widgets/*/*.c"
  "${CMAKE_SOURCE_DIR}/src/custom_widgets/*/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/src/custom_widgets/*/src/*/*.cpp"
  "${CMAKE_SOURCE_DIR}/src/custom_widgets/*/src/*.c"
  "${CMAKE_SOURCE_DIR}/src/custom_widgets/*/src/*/*.c"
  "${CMAKE_SOURCE_DIR}/src/custom_widgets/*/src/*/src/*.cpp"
)
awtk_src_lib(awtk_extwidgets ${_extw})

awtk_src_lib(awtk_awtk_global "${CMAKE_SOURCE_DIR}/src/awtk_global.c")

file(GLOB _tkc CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/tkc/*.c"
  "${CMAKE_SOURCE_DIR}/src/platforms/pc/*.c"
)
awtk_src_lib(awtk_tkc_core ${_tkc})

if(AWTK_SDL_VERSION STREQUAL "3")
  target_link_libraries(awtk_base PRIVATE SDL3::SDL3)
  target_link_libraries(awtk_tkc_core PRIVATE SDL3::SDL3)
endif()

# hal
if(AWTK_OS_LINUX)
  file(GLOB _hal CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/hal/*.c"
    "${CMAKE_SOURCE_DIR}/src/hal/linux/*.c"
    "${CMAKE_SOURCE_DIR}/src/hal/linux/*.cpp"
  )
elseif(AWTK_OS_DARWIN)
  file(GLOB _hal CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/hal/*.c"
    "${CMAKE_SOURCE_DIR}/src/hal/macos/*.c"
    "${CMAKE_SOURCE_DIR}/src/hal/macos/*.cpp"
  )
elseif(AWTK_OS_WINDOWS)
  file(GLOB _hal CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/hal/*.c"
    "${CMAKE_SOURCE_DIR}/src/hal/windows/*.c"
    "${CMAKE_SOURCE_DIR}/src/hal/windows/*.cpp"
  )
else()
  set(_hal "")
endif()
awtk_src_lib(awtk_hal ${_hal})

file(GLOB _dbg CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/debugger/*.c")
awtk_src_lib(awtk_debugger ${_dbg})

set(_fscript_src
  fscript_app_conf.c fscript_array.c fscript_bits.c fscript_crc.c fscript_date_time.c
  fscript_endian.c fscript_ext.c fscript_fs.c fscript_iostream.c fscript_iostream_file.c
  fscript_iostream_inet.c fscript_iostream_serial.c fscript_istream.c fscript_json.c
  fscript_math.c fscript_module.c fscript_object.c fscript_ostream.c fscript_rbuffer.c
  fscript_typed_array.c fscript_utils.c fscript_wbuffer.c
)
list(TRANSFORM _fscript_src PREPEND "${CMAKE_SOURCE_DIR}/src/fscript_ext/")
awtk_src_lib(awtk_fscript_ext ${_fscript_src})
awtk_src_lib(awtk_fscript_ext_widgets "${CMAKE_SOURCE_DIR}/src/fscript_ext/fscript_widget.c")

awtk_src_lib(awtk_romfs "${CMAKE_SOURCE_DIR}/src/romfs/romfs.c")

file(GLOB _conf CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/conf_io/*.c")
awtk_src_lib(awtk_conf_io ${_conf})

file(GLOB _xml CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/xml/*.c")
awtk_src_lib(awtk_xml ${_xml})

file(GLOB _charset CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/charset/*.c")
awtk_src_lib(awtk_charset ${_charset})

file(GLOB _csv CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/csv/*.c")
awtk_src_lib(awtk_csv ${_csv})

file(GLOB _streams CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/streams/*.c"
  "${CMAKE_SOURCE_DIR}/src/streams/buffered/*.c"
  "${CMAKE_SOURCE_DIR}/src/streams/file/*.c"
  "${CMAKE_SOURCE_DIR}/src/streams/inet/*.c"
  "${CMAKE_SOURCE_DIR}/src/streams/mem/*.c"
  "${CMAKE_SOURCE_DIR}/src/streams/serial/*.c"
  "${CMAKE_SOURCE_DIR}/src/streams/process/*.c"
  "${CMAKE_SOURCE_DIR}/src/streams/shdlc/*.c"
  "${CMAKE_SOURCE_DIR}/src/streams/noisy/*.c"
  "${CMAKE_SOURCE_DIR}/src/streams/misc/*.c"
  "${CMAKE_SOURCE_DIR}/src/streams/statistics/*.c"
)
awtk_src_lib(awtk_streams ${_streams})

file(GLOB _ubj CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/ubjson/*.c")
awtk_src_lib(awtk_ubjson ${_ubj})

file(GLOB _comp CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/compressors/*.c")
awtk_src_lib(awtk_compressors ${_comp})

# Ordered STATIC targets merged into libawtk (whole-archive / WHOLEARCHIVE / force_load).
# Aligns with awtk_config.py:
#   AWTK_STATIC_LIBS: awtk_global, fscript_ext_widgets, extwidgets, widgets, base, gpinyin,
#                     fribidi, linebreak, svgtiny [, nfd if SDL_VIDEODRIVER unset]
#   + awtk_config_common.TKC_STATIC_LIBS: debugger, fscript_ext, romfs, conf_io, hal, xml, charset,
#     csv, streams, ubjson, compressors, miniz, tkc_core, mbedtls
#   + NANOVG_BACKEND_LIBS (e.g. nanovg_plus)
# CMake adds awtk_lz4 (not named in TKC_STATIC_LIBS in Python but required by tree).
set(AWTK_CORE_STATIC_LIBS
  awtk_awtk_global
  awtk_fscript_ext_widgets
  awtk_extwidgets
  awtk_widgets
  awtk_base
  awtk_gpinyin
  awtk_fribidi
  awtk_linebreak
  awtk_svgtiny
)
if(AWTK_NFD_BUILT)
  list(APPEND AWTK_CORE_STATIC_LIBS awtk_nfd)
endif()
list(APPEND AWTK_CORE_STATIC_LIBS
  awtk_debugger
  awtk_fscript_ext
  awtk_romfs
  awtk_conf_io
  awtk_hal
  awtk_xml
  awtk_charset
  awtk_csv
  awtk_streams
  awtk_ubjson
  awtk_compressors
  awtk_lz4
  awtk_miniz
  awtk_tkc_core
  awtk_mbedtls
  awtk_nanovg_plus
)

# Passed to cmake/awtk_link_bundle.cmake:awtk_link_whole_archive_static_libs (see awtk_config_common.genDllLinkFlags).
set(AWTK_WHOLE_ARCHIVE_LIBS ${AWTK_CORE_STATIC_LIBS})
