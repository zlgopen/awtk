if ENABLE_SDL

lib_LTLIBRARIES = libaggplatformsdl.la

libaggplatformsdl_la_LDFLAGS = -version-info @AGG_LIB_VERSION@
libaggplatformsdl_la_SOURCES = agg_platform_support.cpp
libaggplatformsdl_la_CXXFLAGS = -I$(top_srcdir)/include @SDL_CFLAGS@
libaggplatformsdl_la_LIBADD = @SDL_LIBS@
endif

