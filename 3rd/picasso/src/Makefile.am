TOP_DIR=..

INCLUDES = -I./include \
           -I./gfx \
           -I./core \
           -I./simd \
           -I../include \
           -I../ 

VPATH=./ ./inclue ./core ./simd ./gfx

lib_LTLIBRARIES = libpicasso.la

AM_CFLAGS = -DEXPORT=1
AM_CXXFLAGS = -DEXPORT=1

libpicasso_la_SOURCES= \
            include/color_type.h \
            include/common.h \
            include/convert.h \
            include/curve.h \
            include/clipper.h \
            include/data_vector.h \
            include/device.h \
            include/fastcopy.h \
            include/fixedopt.h \
            include/fastfill.h \
            include/geometry.h \
            include/graphic_base.h \
            include/graphic_helper.h \
            include/graphic_path.h \
            include/interfaces.h \
            include/math_type.h \
            include/memory_manager.h \
            include/platform.h \
            include/vertex.h \
            include/vertex_dist.h \
            \
            core/device.cpp \
            core/curve.cpp \
            core/clipper.cpp \
            core/fixedopt.cpp \
            core/graphic_path.cpp \
            \
            simd/fastcopy_sse.h \
            \
            gfx/gfx_blur.h \
            gfx/gfx_device.h \
            gfx/gfx_font_adapter.h \
            gfx/gfx_gamma_function.h \
            gfx/gfx_gradient_adapter.h \
            gfx/gfx_image_accessors.h \
            gfx/gfx_image_filters.h \
            gfx/gfx_line_generator.h \
            gfx/gfx_mask_layer.h \
            gfx/gfx_math.h \
            gfx/gfx_painter.h \
            gfx/gfx_painter_helper.h \
            gfx/gfx_pixfmt_rgb.h \
            gfx/gfx_pixfmt_rgb16.h \
            gfx/gfx_pixfmt_rgba.h \
            gfx/gfx_pixfmt_wrapper.h \
            gfx/gfx_raster_adapter.h \
            gfx/gfx_rasterizer_cell.h \
            gfx/gfx_rasterizer_scanline.h \
            gfx/gfx_renderer.h \
            gfx/gfx_rendering_buffer.h \
            gfx/gfx_scanline.h \
            gfx/gfx_scanline_renderer.h \
            gfx/gfx_scanline_storage.h \
            gfx/gfx_span_generator.h \
            gfx/gfx_span_image_filters.h \
            gfx/gfx_trans_affine.h \
            gfx/gfx_device.cpp \
            gfx/gfx_raster_adapter.cpp \
            gfx/gfx_gradient_adapter.cpp \
            gfx/gfx_image_filters.cpp \
            gfx/gfx_rendering_buffer.cpp \
            gfx/gfx_sqrt_tables.cpp \
            gfx/gfx_blur.cpp \
            gfx/gfx_font_adapter_win32.cpp \
            gfx/gfx_font_adapter_freetype2.cpp \
            gfx/gfx_font_load_freetype2.cpp \
            \
            picasso_font.h \
            picasso_font_cache.h \
            picasso_global.h \
            picasso_gpc.h \
            picasso_gradient.h \
            picasso_mask.h \
            picasso_matrix.h \
            picasso_objects.h \
            picasso_painter.h \
            picasso_private.h \
            picasso_raster_adapter.h \
            picasso_rendering_buffer.h \
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
            picasso_mask.cpp \
            picasso_mask_api.cpp \
            picasso_api.cpp


libpicasso_la_LDFLAGS= -no-undefined -release $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_MICRO)

EXTRA_DIST = \
			simd/fastfill16_neon.S \
			simd/fastfill32_neon.S
