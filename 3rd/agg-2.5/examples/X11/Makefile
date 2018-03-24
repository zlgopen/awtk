include ../../Makefile.in.$(shell uname)

PLATFORM=X11

PLATFORMSOURCES=../../src/platform/$(PLATFORM)/agg_platform_support.o

CXXFLAGS= $(AGGCXXFLAGS) -I../../include \
-L../../src \
$(PIXFMT)

CXXFREETYPEFLAGS= $(AGGCXXFLAGS) -Wall \
-I../../include \
-I../../font_freetype \
-I/usr/local/include/freetype2 \
-L../../src \
$(PIXFMT)

LIBS = $(AGGLIBS) -lm -lX11

base:
	cd ../../src/; make
	make aa_demo
	make aa_test
	make alpha_gradient
	make alpha_mask
	make alpha_mask2
	make alpha_mask3
	make bezier_div
	make bspline
	make circles
	make component_rendering
	make conv_contour
	make conv_dash_marker
	make conv_stroke
	make flash_rasterizer
	make flash_rasterizer2
	make gamma_correction
	make gamma_ctrl
	make gamma_tuner
	make gouraud
	make gouraud_mesh
	make gradient_focal
	make gradients
	make graph_test
	make idea
	make lion
	make lion_lens
	make lion_outline
	make multi_clip
	make pattern_fill
	make perspective
	make polymorphic_renderer
	make raster_text
	make rasterizers
	make rasterizers2
	make rounded_rect
	make scanline_boolean
	make scanline_boolean2
	make simple_blur
	make trans_polar
	make image_alpha
	make image_filters
	make image_filters2
	make image_fltr_graph
	make image_perspective
	make image_resample
	make image_transforms
	make image1
	make distortions
	make pattern_perspective
	make compositing
	make compositing2
	make line_patterns
	make line_patterns_clip
	make mol_view
	make blur
	make rasterizer_compound
	make blend_color
	
freetype:
	make freetype_test
	make trans_curve1_ft
	make trans_curve2_ft

gpc:
	make gpc_test

all:
	make base
	make freetype
	make gpc
	make svg_test

aa_demo: ../aa_demo.o $(PLATFORMSOURCES) 
	$(CXX) $(CXXFLAGS) $^ -o aa_demo $(LIBS)

aa_test: ../aa_test.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o aa_test $(LIBS)

alpha_gradient: ../alpha_gradient.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o alpha_gradient $(LIBS)

alpha_mask: ../alpha_mask.o ../parse_lion.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o alpha_mask $(LIBS)

alpha_mask2: ../alpha_mask2.o ../parse_lion.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o alpha_mask2 $(LIBS)

alpha_mask3: ../alpha_mask3.o ../make_arrows.o ../make_gb_poly.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o alpha_mask3 $(LIBS)
	
bezier_div: ../bezier_div.o ../interactive_polygon.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o bezier_div $(LIBS)

blur: ../blur.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o blur $(LIBS)

bspline: ../bspline.o ../interactive_polygon.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o bspline $(LIBS)

circles: ../circles.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o circles $(LIBS)

component_rendering: ../component_rendering.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o component_rendering $(LIBS)

compositing: ../compositing.o $(PLATFORMSOURCES) compositing.ppm
	$(CXX) $(CXXFLAGS) ../compositing.o $(PLATFORMSOURCES) -o compositing $(LIBS)

compositing2: ../compositing2.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o compositing2 $(LIBS)

conv_contour: ../conv_contour.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o conv_contour $(LIBS)

conv_dash_marker: ../conv_dash_marker.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o conv_dash_marker $(LIBS)

conv_stroke: ../conv_stroke.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o conv_stroke $(LIBS)

distortions: ../distortions.o $(PLATFORMSOURCES) spheres.ppm
	$(CXX) $(CXXFLAGS) ../distortions.o $(PLATFORMSOURCES) -o distortions $(LIBS)

flash_rasterizer: ../flash_rasterizer.o $(PLATFORMSOURCES) shapes.txt
	$(CXX) $(CXXFLAGS) ../flash_rasterizer.o $(PLATFORMSOURCES) -o flash_rasterizer $(LIBS)

flash_rasterizer2: ../flash_rasterizer2.o $(PLATFORMSOURCES) shapes.txt
	$(CXX) $(CXXFLAGS) ../flash_rasterizer2.o $(PLATFORMSOURCES) -o flash_rasterizer2 $(LIBS)

gamma_correction: ../gamma_correction.o $(PLATFORMSOURCES) 
	$(CXX) $(CXXFLAGS) $^ -o gamma_correction $(LIBS)

gamma_ctrl: ../gamma_ctrl.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o gamma_ctrl $(LIBS)

gamma_tuner: ../gamma_tuner.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o gamma_tuner $(LIBS)

gouraud: ../gouraud.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o gouraud $(LIBS)

gouraud_mesh: ../gouraud_mesh.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o gouraud_mesh $(LIBS)

gradient_focal: ../gradient_focal.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o gradient_focal $(LIBS)

gradients: ../gradients.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o gradients $(LIBS)

graph_test: ../graph_test.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o graph_test $(LIBS)

idea: ../idea.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o idea $(LIBS)

image_alpha: ../image_alpha.o $(PLATFORMSOURCES) spheres.ppm
	$(CXX) $(CXXFLAGS) ../image_alpha.o $(PLATFORMSOURCES) -o image_alpha $(LIBS)

image_filters: ../image_filters.o $(PLATFORMSOURCES) spheres.ppm 
	$(CXX) $(CXXFLAGS) ../image_filters.o $(PLATFORMSOURCES) -o image_filters $(LIBS)

image_filters2: ../image_filters2.o $(PLATFORMSOURCES) spheres.ppm 
	$(CXX) $(CXXFLAGS) ../image_filters2.o $(PLATFORMSOURCES) -o image_filters2 $(LIBS)

image_fltr_graph: ../image_fltr_graph.o $(PLATFORMSOURCES) spheres.ppm 
	$(CXX) $(CXXFLAGS) ../image_fltr_graph.o $(PLATFORMSOURCES) -o image_fltr_graph $(LIBS)

image_perspective: ../image_perspective.o ../interactive_polygon.o $(PLATFORMSOURCES) spheres.ppm
	$(CXX) $(CXXFLAGS) ../image_perspective.o  ../interactive_polygon.o $(PLATFORMSOURCES) -o image_perspective $(LIBS)

image_resample: ../image_resample.o  ../interactive_polygon.o $(PLATFORMSOURCES) spheres.ppm
	$(CXX) $(CXXFLAGS) ../image_resample.o ../interactive_polygon.o $(PLATFORMSOURCES) -o image_resample $(LIBS)

image_transforms: ../image_transforms.o $(PLATFORMSOURCES) spheres.ppm
	$(CXX) $(CXXFLAGS) ../image_transforms.o $(PLATFORMSOURCES) -o image_transforms $(LIBS)

image1: ../image1.o $(PLATFORMSOURCES) spheres.ppm
	$(CXX) $(CXXFLAGS) ../image1.o $(PLATFORMSOURCES) -o image1 $(LIBS)

line_patterns: ../line_patterns.o $(PLATFORMSOURCES) 1.ppm 2.ppm 3.ppm 4.ppm 5.ppm 6.ppm 7.ppm 8.ppm 9.ppm
	$(CXX) $(CXXFLAGS) ../line_patterns.o $(PLATFORMSOURCES) -o line_patterns $(LIBS)

line_patterns_clip: ../line_patterns_clip.o $(PLATFORMSOURCES) 1.ppm
	$(CXX) $(CXXFLAGS) ../line_patterns_clip.o $(PLATFORMSOURCES) -o line_patterns_clip $(LIBS)

lion: ../lion.o ../parse_lion.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o lion $(LIBS)

lion_lens: ../lion_lens.o ../parse_lion.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o lion_lens $(LIBS)

lion_outline: ../lion_outline.o ../parse_lion.o $(PLATFORMSOURCES) 
	$(CXX) $(CXXFLAGS) $^ -o lion_outline $(LIBS)

mol_view: ../mol_view.o $(PLATFORMSOURCES) 1.sdf
	$(CXX) $(CXXFLAGS) ../mol_view.o $(PLATFORMSOURCES) -o mol_view $(LIBS)

multi_clip: ../multi_clip.o ../parse_lion.o $(PLATFORMSOURCES) 
	$(CXX) $(CXXFLAGS) $^ -o multi_clip $(LIBS)

pattern_fill: ../pattern_fill.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o pattern_fill $(LIBS)

pattern_perspective: ../pattern_perspective.o ../interactive_polygon.o $(PLATFORMSOURCES) agg.ppm
	$(CXX) $(CXXFLAGS) ../pattern_perspective.o ../interactive_polygon.o $(PLATFORMSOURCES) -o pattern_perspective $(LIBS)

perspective: ../perspective.o ../interactive_polygon.o ../parse_lion.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o perspective $(LIBS)

polymorphic_renderer: ../polymorphic_renderer.o $(PLATFORMSOURCES) 
	$(CXX) $(CXXFLAGS) $^ -o polymorphic_renderer $(LIBS)

raster_text: ../raster_text.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o raster_text $(LIBS)

rasterizers: ../rasterizers.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o rasterizers $(LIBS)

rasterizers2: ../rasterizers2.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o rasterizers2 $(LIBS)

rasterizer_compound: ../rasterizer_compound.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o rasterizer_compound $(LIBS)

blend_color: ../blend_color.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o blend_color $(LIBS)

rounded_rect: ../rounded_rect.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o rounded_rect $(LIBS)

scanline_boolean: ../scanline_boolean.o ../interactive_polygon.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o scanline_boolean $(LIBS)

scanline_boolean2: ../scanline_boolean2.o ../make_arrows.o ../make_gb_poly.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o scanline_boolean2 $(LIBS)

simple_blur: ../simple_blur.o ../parse_lion.o $(PLATFORMSOURCES) 
	$(CXX) $(CXXFLAGS) $^ -o simple_blur $(LIBS)

trans_polar: ../trans_polar.o $(PLATFORMSOURCES) 
	$(CXX) $(CXXFLAGS) $^ -o trans_polar $(LIBS)
	
freetype_test: ../freetype_test.o ../../font_freetype/agg_font_freetype.o $(PLATFORMSOURCES)  timesi.ttf
	$(CXX) $(CXXFREETYPEFLAGS) ../freetype_test.o ../../font_freetype/agg_font_freetype.o $(PLATFORMSOURCES) -o freetype_test $(LIBS) -lfreetype
	
trans_curve1_ft: ../trans_curve1_ft.o ../../font_freetype/agg_font_freetype.o  ../interactive_polygon.o $(PLATFORMSOURCES) timesi.ttf
	$(CXX) $(CXXFLAGS) ../trans_curve1_ft.o ../../font_freetype/agg_font_freetype.o  ../interactive_polygon.o $(PLATFORMSOURCES) -o trans_curve1_ft $(LIBS) -lfreetype
	
trans_curve2_ft: ../trans_curve2_ft.o ../../font_freetype/agg_font_freetype.o  ../interactive_polygon.o $(PLATFORMSOURCES) timesi.ttf
	$(CXX) $(CXXFLAGS) ../trans_curve2_ft.o ../../font_freetype/agg_font_freetype.o  ../interactive_polygon.o $(PLATFORMSOURCES) -o trans_curve2_ft $(LIBS) -lfreetype
	
gpc_test: ../gpc_test.o ../make_arrows.o ../make_gb_poly.o $(PLATFORMSOURCES)
	$(CXX) $(CXXFLAGS) $^ -o gpc_test $(LIBS)

svg_test: ../svg_viewer/agg_svg_parser.o ../svg_viewer/agg_svg_path_renderer.o ../svg_viewer/agg_svg_path_tokenizer.o ../svg_viewer/svg_test.o $(PLATFORMSOURCES) tiger.svg
	$(CXX) $(CXXFLAGS) ../svg_viewer/agg_svg_parser.o ../svg_viewer/agg_svg_path_renderer.o ../svg_viewer/agg_svg_path_tokenizer.o ../svg_viewer/svg_test.o $(PLATFORMSOURCES) -o svg_test $(LIBS) -lfreetype -lexpat

clean:
	rm -f ../*.o
	rm -f ../svg_viewer/*.o
	rm -f ../../src/platform/$(PLATFORM)/agg_platform_support.o

agg.ppm:
	@echo "Please download http://www.antigrain.com/agg.ppm"
	@echo "Attempting to retrieve via wget: (will fail if wget is not installed)"
	wget http://www.antigrain.com/agg.ppm

compositing.ppm:
	@echo "Please download http://www.antigrain.com/compositing.ppm"
	@echo "Attempting to retrieve via wget: (will fail if wget is not installed)"
	wget http://www.antigrain.com/compositing.ppm

spheres.ppm:
	@echo "Please download http://www.antigrain.com/spheres.ppm"
	@echo "Attempting to retrieve file: (will fail if wget is not installed)"
	wget http://www.antigrain.com/spheres.ppm

shapes.txt:
	@echo "Please download http://www.antigrain.com/shapes.txt"
	@echo "Attempting to retrieve file: (will fail if wget is not installed)"
	wget http://www.antigrain.com/shapes.txt

1.sdf:
	@echo "Please download http://www.antigrain.com/1.sdf"
	@echo "Attempting to retrieve file: (will fail if wget is not installed)"
	wget http://www.antigrain.com/1.sdf

1.ppm:
	@echo "Please download http://www.antigrain.com/line_patterns.tar.gz and decompress it"
	@echo "Attempting to retrieve file: (will fail if wget is not installed)"
	wget http://www.antigrain.com/line_patterns.tar.gz
	gunzip line_patterns.tar.gz
	tar -xvf line_patterns.tar
	
timesi.ttf:
	@echo "Please download http://www.antigrain.com/timesi.zip and decompress it"
	@echo "Attempting to retrieve file: (will fail if wget is not installed)"
	wget http://www.antigrain.com/timesi.zip
	unzip -o timesi.zip

tiger.svg:
	@echo "Please download http://www.antigrain.com/svg/tiger.svg"
	@echo "Attempting to retrieve file: (will fail if wget is not installed)"
	wget http://www.antigrain.com/svg/tiger.svg
	
../freetype_test.o:	../freetype_test.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFREETYPEFLAGS) $*.cpp -o $@
	
../trans_curve1_ft.o:	../trans_curve1_ft.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFREETYPEFLAGS) $*.cpp -o $@
	
../trans_curve2_ft.o:	../trans_curve2_ft.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFREETYPEFLAGS) $*.cpp -o $@
	
../../font_freetype/agg_font_freetype.o: ../../font_freetype/agg_font_freetype.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFREETYPEFLAGS) $*.cpp -o $@
	
../gpc_test.o:	../gpc_test.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFLAGS) -I../../gpc $*.cpp -o $@

%.o:	%.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@
	

.PHONY : clean


