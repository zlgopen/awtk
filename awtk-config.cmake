set(AWTK_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Path to AWTK root directory")

# 查找awtk库的头文件和库文件
find_library(AWTK_LIBRARY NAMES awtk PATHS ${AWTK_ROOT_DIR}/bin)

file(COPY  ${AWTK_LIBRARY} DESTINATION ${PROJECT_SOURCE_DIR}/bin)

add_compile_definitions(WITH_FS_RES)
add_compile_definitions(WITH_SDL)
add_compile_definitions(HAS_STDIO)
add_compile_definitions(HAS_STD_MALLOC)

set(AWTK_LIBRARIES ${AWTK_LIBRARY})
set(AWTK_INCLUDE_DIRS ${AWTK_ROOT_DIR}/src ${AWTK_ROOT_DIR}/3rd ${AWTK_ROOT_DIR}/src/ext_widgets ${AWTK_ROOT_DIR}/src ${AWTK_ROOT_DIR}/3rd/SDL/include)

link_directories(${AWTK_ROOT_DIR}/bin)
include_directories(${AWTK_INCLUDE_DIRS})

if(WIN32)
  file(COPY  ${AWTK_ROOT_DIR}/bin/awtk.dll DESTINATION ${CMAKE_BINARY_DIR}/Debug)
endif()

#示例请参考 awtk-hello/CMakeLists.txt
