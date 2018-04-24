TOP_DIR=..

INCLUDES = -I./include \
		   -I./gfx \
		   -I./gfx/include \
           -I./core \
           -I./simd \
		   -I../include \
		   -I../ 

VPATH=./ ./inclue ./core ./simd ./gfx ./gfx/include ./gfx/lib

lib_LTLIBRARIES = libpicasso.la

AM_CFLAGS = -DEXPORT=1
AM_CXXFLAGS = -DEXPORT=1

libpicasso_la_SOURCES= \
			device.cpp \
			curve.cpp \
			graphic_path.cpp \
			\
			gfx_device.cpp \
			gfx_raster_adapter.cpp \
			gfx_gradient_adapter.cpp \
			gfx_image_filters.cpp \
			gfx_rendering_buffer.cpp \
			gfx_sqrt_tables.cpp \
			gfx_blur.cpp \
			gfx_font_adapter_win32.cpp \
			gfx_font_adapter_freetype2.cpp \
			gfx_font_load_freetype2.cpp \
			\
			picasso_matrix.cpp \
			picasso_matrix_api.cpp \
			picasso_painter.cpp \
			picasso_rendering_buffer.cpp \
			picasso_raster_adapter.cpp \
			picasso_canvas.cpp \
			picasso_image.cpp \
			picasso_pattern.cpp \
			picasso_path.cpp \
			picasso_gradient.cpp \
			picasso_gradient_api.cpp \
			picasso_font.cpp \
			picasso_font_api.cpp \
			picasso_gpc.cpp \
			picasso_mask.cpp \
			picasso_mask_api.cpp \
			picasso_api.cpp


libpicasso_la_LDFLAGS= -no-undefined -release $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_MICRO)

