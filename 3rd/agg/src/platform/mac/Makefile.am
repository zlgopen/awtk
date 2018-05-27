if ENABLE_OSX
lib_LTLIBRARIES = libaggplatformmac.la
libaggplatformmac_la_LDFLAGS=-version-info @AGG_LIB_VERSION@
libaggplatformmac_la_SOURCES=agg_mac_pmap.cpp \
														 agg_platform_support.cpp 

libaggplatformmac_la_CXXFLAGS =  -I$(top_srcdir)/include @OSX_CFLAGS@
libaggplatformmac_la_LIBADD = @OSX_LIBS@ $(top_builddir)/src/libagg.la


endif
