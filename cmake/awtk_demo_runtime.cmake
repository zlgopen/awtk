# Copy awtk/SDL DLLs and res/assets once into the demo output directory.
# Per-demo POST_BUILD copies of those files raced under parallel builds (LNK1105).
# include() creates awtk_demos_runtime. awtk_attach_demo_runtime() only adds a dependency.
# Demos must keep the default output directory of the CMakeLists that includes this file.

include_guard(GLOBAL)

if(AWTK_BUILD_SHARED AND AWTK_OS_WINDOWS)
  if(CMAKE_CONFIGURATION_TYPES)
    set(_dest "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")
  else()
    set(_dest "${CMAKE_CURRENT_BINARY_DIR}")
  endif()
  set(_stamp "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/awtk_demos_runtime_$<CONFIG>.stamp")

  set(_deps awtk)
  set(_sdl_copy "")
  if(SDL_SHARED AND TARGET SDL2)
    set(_sdl_copy COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:SDL2>" "${_dest}")
    list(APPEND _deps SDL2)
  endif()

  add_custom_command(
    OUTPUT "${_stamp}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${_dest}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:awtk>" "${_dest}"
    ${_sdl_copy}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_SOURCE_DIR}/res/assets" "${_dest}/res/assets"
    COMMAND ${CMAKE_COMMAND} -E touch "${_stamp}"
    DEPENDS ${_deps}
    COMMENT "AWTK: stage shared demo runtime"
    VERBATIM
  )
  add_custom_target(awtk_demos_runtime DEPENDS "${_stamp}")

  unset(_dest)
  unset(_stamp)
  unset(_deps)
  unset(_sdl_copy)
else()
  add_custom_target(awtk_demos_runtime)
endif()

function(awtk_attach_demo_runtime _target)
  add_dependencies(${_target} awtk_demos_runtime)
endfunction()
