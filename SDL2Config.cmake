set(SDL2_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Path to SDL2 root directory")

# 查找awtk库的头文件和库文件
find_library(SDL2_LIBRARY NAMES SDL2 PATHS ${SDL2_ROOT_DIR}/lib)

set(SDL2_LIBRARIES)
set(SDL2_INCLUDE_DIRS ${SDL2_ROOT_DIR}/3rd/SDL/include)

link_directories(${SDL2_ROOT_DIR}/lib)
include_directories(${SDL2_INCLUDE_DIRS})

