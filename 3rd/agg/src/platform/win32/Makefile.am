

if ENABLE_WIN32
lib_LTLIBRARIES = libaggplatformwin32.la

libaggplatformwin32_la_LDFLAGS = -version-info @AGG_LIB_VERSION@
libaggplatformwin32_la_SOURCES = agg_platform_support.cpp \
																 agg_win32_bmp.cpp

libaggplatformwin32_la_CXXFLAGS =  -I$(top_srcdir)/include @WINDOWS_CFLAGS@
libaggplatformwin32_la_LIBADD = @WINDOWS_LIBS@ $(top_builddir)/src/libagg.la
endif

