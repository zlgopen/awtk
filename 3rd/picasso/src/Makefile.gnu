#DLL=-DDLL_EXPORT
FREETYPE_INC=-I/usr/include/freetype2
#FREETYPE_INC=-I/extra/freetype2/include -I/extra/freetype2/include/freetype2
CXX=g++ -Wall -O3 -g -fno-rtti -fno-exceptions -msse2
#CXX=g++ -Wall -O3 -g -fprofile-arcs -ftest-coverage -fno-rtti -fno-exceptions
#CXX=g++ -Wall -O3 -g -pg -fno-rtti -fno-exceptions
INC=-I. -I./gfx -I./gles2 -I./include -I./simd -I./../ -I./../include -I./../build ${FREETYPE_INC}
CXXFLAGS=-DEXPORT ${DLL} 

VPATH=./ ./include ./core ./simd ./gfx ./gles2

objects= \
        device.o \
        curve.o \
        graphic_path.o \
        clipper.o \
        \
        gfx_device.o \
        gfx_raster_adapter.o \
        gfx_gradient_adapter.o \
        gfx_image_filters.o \
        gfx_rendering_buffer.o \
        gfx_sqrt_tables.o \
        gfx_blur.o \
        gfx_font_adapter_win32.o \
        gfx_font_adapter_freetype2.o \
        gfx_font_load_freetype2.o \
        \
        picasso_matrix.o \
        picasso_matrix_api.o \
        picasso_painter.o \
        picasso_rendering_buffer.o \
        picasso_raster_adapter.o \
        picasso_canvas.o \
        picasso_image.o \
        picasso_pattern.o \
        picasso_path.o \
        picasso_gradient.o \
        picasso_gradient_api.o \
        picasso_font.o \
        picasso_font_api.o \
        picasso_mask.o \
        picasso_mask_api.o \
        picasso_api.o

all: libpicasso.a

libpicasso.a : ${objects}
	ar rcu libpicasso.a ${objects}

%.o : %.cpp 
	${CXX} ${CXXFLAGS} -c $<  -o $@ ${INC}

clean:
	rm *.o *.a
