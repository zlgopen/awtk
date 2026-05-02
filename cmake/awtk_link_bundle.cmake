# Link internal STATIC CMake targets into one libawtk (SHARED or STATIC carrier).
#
# Mirrors SCons flow in src/SConscript (SharedLibrary awtk) and flags from:
#   awtk_config_common.toWholeArchive(libs)
#     -> Linux:  -Wl,--whole-archive -l<lib>... -Wl,--no-whole-archive  (SCons uses -l from LIBPATH)
#       CMake: pass --whole-archive and .a targets in one target_link_libraries() so Ninja keeps them
#       adjacent (splitting across target_link_options + target_link_libraries yields a ~16KB .so).
#   awtk_config_common.genDllLinkFlags(libs, def)
#     -> Darwin: -all_load  (SCons; here we use -force_load per .a — same effect)
#     -> MinGW:  toWholeArchive + optional .def
#     -> MSVC:   /WHOLEARCHIVE per lib + /DEF:...
#   awtk_config.AWTK_STATIC_LIBS + TKC_STATIC_LIBS + NANOVG_BACKEND_LIBS order is reflected in
#   src_libs.cmake as AWTK_WHOLE_ARCHIVE_LIBS (see comments there).

include_guard(GLOBAL)

function(awtk_link_whole_archive_static_libs _target)
  if(MSVC)
    # Same as SCons MSVC: /WHOLEARCHIVE (per lib; avoids ":" in /WHOLEARCHIVE:C:\... tokenization).
    foreach(_lib IN LISTS AWTK_WHOLE_ARCHIVE_LIBS)
      target_link_options(${_target} PRIVATE "LINKER:/WHOLEARCHIVE:$<TARGET_FILE:${_lib}>")
    endforeach()
  elseif(AWTK_WINDOWS_MINGW)
    target_link_libraries(${_target} PRIVATE
      "-Wl,--whole-archive"
      ${AWTK_WHOLE_ARCHIVE_LIBS}
      "-Wl,--no-whole-archive"
    )
  elseif(AWTK_OS_DARWIN)
    foreach(_lib IN LISTS AWTK_WHOLE_ARCHIVE_LIBS)
      target_link_options(${_target} PRIVATE "LINKER:-force_load,$<TARGET_FILE:${_lib}>")
    endforeach()
  else()
    # Linux / ELF: must keep --whole-archive immediately wrapping the .a list (see file header).
    target_link_libraries(${_target} PRIVATE
      "-Wl,--whole-archive"
      ${AWTK_WHOLE_ARCHIVE_LIBS}
      "-Wl,--no-whole-archive"
    )
  endif()
endfunction()
