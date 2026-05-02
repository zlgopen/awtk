cmake_minimum_required(VERSION 3.20)
project(awtk_find_demo LANGUAGES C)

add_executable(awtk_find_demo_minimal main.c)

if(TARGET awtk)
  target_link_libraries(awtk_find_demo_minimal PRIVATE awtk)
else()
  find_package(awtk CONFIG REQUIRED)
  target_link_libraries(awtk_find_demo_minimal PRIVATE awtk::awtk)
endif()
