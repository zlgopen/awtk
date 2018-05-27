if ENABLE_X11
lib_LTLIBRARIES = libaggplatformX11.la

libaggplatformX11_la_LDFLAGS = -version-info @AGG_LIB_VERSION@
libaggplatformX11_la_SOURCES = agg_platform_support.cpp
libaggplatformX11_la_CXXFLAGS =  -I$(top_srcdir)/include @x_includes@
libaggplatformX11_la_LIBADD = -lX11
endif
