set(AWTK_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/awtk)
set(AWTK_APP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/app)
set(AWTK_3RD_PATH ${AWTK_ROOT_PATH}/3rd)
set(AWTK_SRC_PATH ${AWTK_ROOT_PATH}/src)
set(SDL2_SOURCE_DIR ${AWTK_ROOT_PATH}/3rd/SDL)

set(AWTK_COMMON_INCLUDES 
  ${AWTK_SRC_PATH}
  ${AWTK_3RD_PATH}
  ${AWTK_3RD_PATH}/glad
  ${AWTK_3RD_PATH}/nanovg
  ${AWTK_3RD_PATH}/nanovg/gl
  ${AWTK_3RD_PATH}/nanovg/base
  ${AWTK_3RD_PATH}/gpinyin/include
  ${AWTK_3RD_PATH}/libunibreak
  ${AWTK_SRC_PATH}/ext_widgets
  ${AWTK_SRC_PATH}/custom_widgets)

set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DMOBILE_APP -DWITH_ASSET_LOADER ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DWITH_STB_FONT -DWITHOUT_GLAD ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DSTBTT_STATIC -DSTB_IMAGE_STATIC -DWITH_STB_IMAGE ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DWITH_VGCANVAS -DWITH_UNICODE_BREAK -DWITH_DESKTOP_STYLE ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DHAS_STD_MALLOC -DWITH_FS_RES -DHAS_STDIO ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DWITH_NANOVG_GPU -DWITH_VGCANVAS_LCD ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DWITH_GPU_GL ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DHAS_PTHREAD -DHAS_GET_TIME_US64=1 ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DWITH_DATA_READER_WRITER=1 ")

file(GLOB AWTK_BASE_SOURCE_FILES 
  ${AWTK_3RD_PATH}/libunibreak/*.c
  ${AWTK_3RD_PATH}/miniz/*.c
  ${AWTK_3RD_PATH}/nanovg/base/*.c
  ${AWTK_SRC_PATH}/tkc/*.c
  ${AWTK_SRC_PATH}/base/*.c
  ${AWTK_SRC_PATH}/layouters/*.c
  ${AWTK_SRC_PATH}/widgets/*.c
  ${AWTK_SRC_PATH}/ui_loader/*.c
  ${AWTK_SRC_PATH}/xml/*.c
  ${AWTK_SRC_PATH}/svg/*.c
  ${AWTK_SRC_PATH}/clip_board/*.c
  ${AWTK_SRC_PATH}/font_loader/*.c
  ${AWTK_SRC_PATH}/graphic_buffer/*.c
  ${AWTK_SRC_PATH}/ext_widgets/*.c
  ${AWTK_SRC_PATH}/ext_widgets/*/*.c
  ${AWTK_SRC_PATH}/custom_widgets/*.c
  ${AWTK_SRC_PATH}/custom_widgets/*/*.c
  ${AWTK_SRC_PATH}/custom_widgets/*.cc
  ${AWTK_SRC_PATH}/custom_widgets/*/*.cc
  ${AWTK_SRC_PATH}/custom_widgets/*.cxx
  ${AWTK_SRC_PATH}/custom_widgets/*/*.cxx
  ${AWTK_SRC_PATH}/custom_widgets/*.cpp
  ${AWTK_SRC_PATH}/custom_widgets/*/*.cpp
  ${AWTK_SRC_PATH}/widget_animators/*.c
  ${AWTK_SRC_PATH}/window_animators/*.c
  ${AWTK_SRC_PATH}/window_manager/window_manager_default.c
  ${AWTK_SRC_PATH}/dialog_highlighters/*.c
  ${AWTK_SRC_PATH}/*.c
  ${AWTK_SRC_PATH}/ubjson/*.c
  ${AWTK_SRC_PATH}/streams/buffered/*.c
  ${AWTK_SRC_PATH}/streams/mem/*.c
  ${AWTK_SRC_PATH}/streams/file/*.c
  ${AWTK_SRC_PATH}/streams/*.c
  ${AWTK_SRC_PATH}/csv/*.c
  ${AWTK_SRC_PATH}/conf_io/*.c
  ${AWTK_SRC_PATH}/fscript_ext/*.c
  ${AWTK_SRC_PATH}/charset/*.c
)
  
file(GLOB AWTK_COMMON_SOURCE_FILES
  ${AWTK_BASE_SOURCE_FILES}
  ${AWTK_SRC_PATH}/streams/inet/*.c
  ${AWTK_SRC_PATH}/streams/serial/*.c
  ${AWTK_SRC_PATH}/lcd/lcd_nanovg.c
  ${AWTK_SRC_PATH}/image_loader/*.c
  ${AWTK_SRC_PATH}/main_loop/main_loop_simple.c
  ${AWTK_SRC_PATH}/main_loop/main_loop_console.c
  ${AWTK_SRC_PATH}/input_engines/input_engine_pinyin.cpp
  ${AWTK_SRC_PATH}/platforms/pc/*.c)

set(AWTK_SDL_COMMON_SOURCE_FILES
	${SDL2_SOURCE_DIR}/src/*.c
	${SDL2_SOURCE_DIR}/src/audio/*.c
	${SDL2_SOURCE_DIR}/src/audio/dummy/*.c
	${SDL2_SOURCE_DIR}/src/atomic/*.c
	${SDL2_SOURCE_DIR}/src/cpuinfo/*.c
	${SDL2_SOURCE_DIR}/src/dynapi/*.c
	${SDL2_SOURCE_DIR}/src/events/*.c
	${SDL2_SOURCE_DIR}/src/file/*.c
	${SDL2_SOURCE_DIR}/src/haptic/*.c
	${SDL2_SOURCE_DIR}/src/joystick/*.c
	${SDL2_SOURCE_DIR}/src/joystick/hidapi/*.c
	${SDL2_SOURCE_DIR}/src/loadso/dlopen/*.c
	${SDL2_SOURCE_DIR}/src/power/*.c
	${SDL2_SOURCE_DIR}/src/sensor/*.c
	${SDL2_SOURCE_DIR}/src/render/*.c
	${SDL2_SOURCE_DIR}/src/render/*/*.c
	${SDL2_SOURCE_DIR}/src/stdlib/*.c
	${SDL2_SOURCE_DIR}/src/thread/*.c
	${SDL2_SOURCE_DIR}/src/thread/pthread/*.c
	${SDL2_SOURCE_DIR}/src/timer/*.c
	${SDL2_SOURCE_DIR}/src/timer/unix/*.c
	${SDL2_SOURCE_DIR}/src/video/*.c
	${SDL2_SOURCE_DIR}/src/video/yuv2rgb/*.c
  ${SDL2_SOURCE_DIR}/src/video/dummy/*.c
  ${SDL2_SOURCE_DIR}/src/haptic/dummy/*.c	
	${AWTK_SRC_PATH}/input_methods/input_method_creator.c
	${AWTK_SRC_PATH}/main_loop/main_loop_sdl.c
	${AWTK_SRC_PATH}/native_window/native_window_sdl.c
)

file(GLOB AWTK_APP_SOURCE_FILES
  ${AWTK_APP_PATH}/*.c
  ${AWTK_APP_PATH}/*.cpp
  ${AWTK_APP_PATH}/**/*.c
  ${AWTK_APP_PATH}/**/*.cpp
  ${AWTK_APP_PATH}/**/**/*.c
  ${AWTK_APP_PATH}/**/**/*.cpp
)

