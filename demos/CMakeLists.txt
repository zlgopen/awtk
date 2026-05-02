# demoui: mirrors demos/SConscript — demo_ui_app.c + static assets (res/assets.inc via assets.c)

add_library(awtk_demos_ui_assets STATIC "${CMAKE_SOURCE_DIR}/demos/assets.c")
target_include_directories(awtk_demos_ui_assets PRIVATE ${AWTK_SRC_INCLUDES})
awtk_apply_platform_compile_options(awtk_demos_ui_assets)
awtk_apply_common_compile_definitions(awtk_demos_ui_assets)
add_dependencies(awtk_demos_ui_assets awtk_idl_codegen)

add_executable(awtk_demo_ui "${CMAKE_SOURCE_DIR}/demos/demo_ui_app.c")
target_include_directories(awtk_demo_ui PRIVATE ${AWTK_SRC_INCLUDES})
awtk_apply_platform_compile_options(awtk_demo_ui)
awtk_apply_common_compile_definitions(awtk_demo_ui)

# SCons: LIBS=['assets'] + env['LIBS'] — assets before awtk
target_link_libraries(awtk_demo_ui PRIVATE awtk_demos_ui_assets awtk)
set_target_properties(awtk_demo_ui PROPERTIES OUTPUT_NAME demoui LINKER_LANGUAGE CXX)
add_dependencies(awtk_demo_ui awtk)

# awtk_main.inc uses wWinMain on MSVC (WIN32 && !MINGW); default /SUBSYSTEM:CONSOLE expects main → LNK2019.
if(AWTK_WINDOWS_MSVC)
  set_target_properties(awtk_demo_ui PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

if(AWTK_BUILD_SHARED AND (AWTK_OS_LINUX OR AWTK_OS_DARWIN))
  set_target_properties(awtk_demo_ui PROPERTIES BUILD_RPATH "$<TARGET_FILE_DIR:awtk>")
endif()

# Windows: demoui.exe lives under build/demos/<Config>/ but awtk.dll is under build/<Config>/ — copy deps next to the exe for running without PATH.
if(AWTK_BUILD_SHARED AND AWTK_OS_WINDOWS)
  add_custom_command(
    TARGET awtk_demo_ui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "$<TARGET_FILE:awtk>"
      "$<TARGET_FILE_DIR:awtk_demo_ui>"
    VERBATIM
  )
  if(SDL_SHARED AND TARGET SDL2)
    add_custom_command(
      TARGET awtk_demo_ui POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL2>"
        "$<TARGET_FILE_DIR:awtk_demo_ui>"
      VERBATIM
    )
  endif()
endif()
