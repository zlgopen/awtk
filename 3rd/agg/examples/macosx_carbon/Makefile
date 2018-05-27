AGGLIBS= -lagg -framework Carbon -framework QuickTime
AGGCXXFLAGS = -bind_at_load -O3 -I/Developer/Headers/FlatCarbon
CXX = g++
C = gcc
#CXX = icc
LIB = ar cr -s

PLATFORM=mac

PLATFORMSOURCES=../../src/platform/$(PLATFORM)/agg_platform_support.o ../../src/platform/$(PLATFORM)/agg_mac_pmap.o

CXXFLAGS= $(AGGCXXFLAGS) -I../../include \
-L../../src \
$(PIXFMT)

LIBS = $(AGGLIBS) -lm

all:
	cd ../../src/; make
	make aa_demo
	make aa_test
	make alpha_gradient
	make alpha_mask
	make alpha_mask2
	make alpha_mask3
	make bezier_div
	make blur
	make blend_color
	make bspline
	make circles
	make component_rendering
	make conv_contour
	make conv_dash_marker
	make conv_stroke
	make gamma_correction
	make gamma_ctrl
	make gouraud
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
	make rasterizer_compound
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
	make line_patterns
	make mol_view

aa_demo: ../aa_demo.o $(PLATFORMSOURCES) aa_demo.app
	$(CXX) $(CXXFLAGS) ../aa_demo.o $(PLATFORMSOURCES) -o aa_demo.app/Contents/MacOS/aa_demo $(LIBS)

aa_test: ../aa_test.o $(PLATFORMSOURCES) aa_test.app
	$(CXX) $(CXXFLAGS) ../aa_test.o $(PLATFORMSOURCES) -o aa_test.app/Contents/MacOS/aa_test $(LIBS)

alpha_gradient: ../alpha_gradient.o $(PLATFORMSOURCES) alpha_gradient.app
	$(CXX) $(CXXFLAGS) ../alpha_gradient.o $(PLATFORMSOURCES) -o alpha_gradient.app/Contents/MacOS/alpha_gradient $(LIBS)

alpha_mask: ../alpha_mask.o ../parse_lion.o $(PLATFORMSOURCES) alpha_mask.app
	$(CXX) $(CXXFLAGS) ../alpha_mask.o ../parse_lion.o $(PLATFORMSOURCES) -o alpha_mask.app/Contents/MacOS/alpha_mask $(LIBS)

alpha_mask2: ../alpha_mask2.o ../parse_lion.o $(PLATFORMSOURCES) alpha_mask2.app
	$(CXX) $(CXXFLAGS) ../alpha_mask2.o ../parse_lion.o $(PLATFORMSOURCES) -o alpha_mask2.app/Contents/MacOS/alpha_mask2 $(LIBS)

alpha_mask3: ../alpha_mask3.o ../make_arrows.o ../make_gb_poly.o $(PLATFORMSOURCES) alpha_mask3.app
	$(CXX) $(CXXFLAGS) ../alpha_mask3.o ../make_arrows.o ../make_gb_poly.o $(PLATFORMSOURCES) -o alpha_mask3.app/Contents/MacOS/alpha_mask3 $(LIBS)

bezier_div: ../bezier_div.o ../interactive_polygon.o $(PLATFORMSOURCES)  bezier_div.app
	$(CXX) $(CXXFLAGS) ../bezier_div.o ../interactive_polygon.o $(PLATFORMSOURCES) -o bezier_div.app/Contents/MacOS/bezier_div $(LIBS)

blur: ../blur.o $(PLATFORMSOURCES) blur.app
	$(CXX) $(CXXFLAGS) ../blur.o $(PLATFORMSOURCES) -o blur.app/Contents/MacOS/blur $(LIBS)

blend_color: ../blend_color.o $(PLATFORMSOURCES) blend_color.app
	$(CXX) $(CXXFLAGS) ../blend_color.o $(PLATFORMSOURCES) -o blend_color.app/Contents/MacOS/blend_color $(LIBS)

bspline: ../bspline.o ../interactive_polygon.o $(PLATFORMSOURCES) bspline.app
	$(CXX) $(CXXFLAGS) ../bspline.o ../interactive_polygon.o $(PLATFORMSOURCES) -o bspline.app/Contents/MacOS/bspline $(LIBS)

circles: ../circles.o $(PLATFORMSOURCES) circles.app
	$(CXX) $(CXXFLAGS) ../circles.o  $(PLATFORMSOURCES) -o circles.app/Contents/MacOS/circles $(LIBS)

component_rendering: ../component_rendering.o $(PLATFORMSOURCES) component_rendering.app
	$(CXX) $(CXXFLAGS) ../component_rendering.o $(PLATFORMSOURCES) -o component_rendering.app/Contents/MacOS/component_rendering $(LIBS)

compositing: ../compositing.o $(PLATFORMSOURCES) compositing.bmp compositing.app
	cp compositing.bmp compositing.app/Contents/MacOS/compositing.bmp
	$(CXX) $(CXXFLAGS) ../compositing.o $(PLATFORMSOURCES) -o compositing.app/Contents/MacOS/compositing $(LIBS)

conv_contour: ../conv_contour.o $(PLATFORMSOURCES) conv_contour.app
	$(CXX) $(CXXFLAGS) ../conv_contour.o $(PLATFORMSOURCES) -o conv_contour.app/Contents/MacOS/conv_contour $(LIBS)

conv_dash_marker: ../conv_dash_marker.o $(PLATFORMSOURCES) conv_dash_marker.app
	$(CXX) $(CXXFLAGS) ../conv_dash_marker.o $(PLATFORMSOURCES) -o conv_dash_marker.app/Contents/MacOS/conv_dash_marker $(LIBS)

conv_stroke: ../conv_stroke.o $(PLATFORMSOURCES) conv_stroke.app
	$(CXX) $(CXXFLAGS) ../conv_stroke.o $(PLATFORMSOURCES) -o conv_stroke.app/Contents/MacOS/conv_stroke $(LIBS)

distortions: ../distortions.o $(PLATFORMSOURCES) spheres.bmp distortions.app
	cp spheres.bmp distortions.app/Contents/MacOS/spheres.bmp
	$(CXX) $(CXXFLAGS) ../distortions.o $(PLATFORMSOURCES) -o distortions.app/Contents/MacOS/distortions $(LIBS)

gamma_correction: ../gamma_correction.o $(PLATFORMSOURCES) gamma_correction.app
	$(CXX) $(CXXFLAGS) ../gamma_correction.o $(PLATFORMSOURCES) -o gamma_correction.app/Contents/MacOS/gamma_correction $(LIBS)

gamma_ctrl: ../gamma_ctrl.o $(PLATFORMSOURCES) gamma_ctrl.app
	$(CXX) $(CXXFLAGS) ../gamma_ctrl.o $(PLATFORMSOURCES) -o gamma_ctrl.app/Contents/MacOS/gamma_ctrl $(LIBS)

gouraud: ../gouraud.o $(PLATFORMSOURCES) gouraud.app
	$(CXX) $(CXXFLAGS) ../gouraud.o $(PLATFORMSOURCES) -o gouraud.app/Contents/MacOS/gouraud $(LIBS)

gradient_focal: ../gradient_focal.o $(PLATFORMSOURCES) gradient_focal.app
	$(CXX) $(CXXFLAGS) ../gradient_focal.o $(PLATFORMSOURCES) -o gradient_focal.app/Contents/MacOS/gradient_focal $(LIBS)

gradients: ../gradients.o $(PLATFORMSOURCES) gradients.app
	$(CXX) $(CXXFLAGS) ../gradients.o $(PLATFORMSOURCES) -o gradients.app/Contents/MacOS/gradients $(LIBS)

graph_test: ../graph_test.o $(PLATFORMSOURCES) graph_test.app
	$(CXX) $(CXXFLAGS) ../graph_test.o $(PLATFORMSOURCES) -o graph_test.app/Contents/MacOS/graph_test $(LIBS)

idea: ../idea.o $(PLATFORMSOURCES) idea.app
	$(CXX) $(CXXFLAGS) ../idea.o $(PLATFORMSOURCES) -o idea.app/Contents/MacOS/idea $(LIBS)

image_alpha: ../image_alpha.o $(PLATFORMSOURCES) spheres.bmp image_alpha.app
	cp spheres.bmp image_alpha.app/Contents/MacOS/spheres.bmp
	$(CXX) $(CXXFLAGS) ../image_alpha.o $(PLATFORMSOURCES) -o image_alpha.app/Contents/MacOS/image_alpha $(LIBS)

image_filters: ../image_filters.o $(PLATFORMSOURCES) spheres.bmp image_filters.app
	cp spheres.bmp image_filters.app/Contents/MacOS/spheres.bmp
	$(CXX) $(CXXFLAGS) ../image_filters.o $(PLATFORMSOURCES) -o image_filters.app/Contents/MacOS/image_filters $(LIBS)

image_filters2: ../image_filters2.o $(PLATFORMSOURCES) spheres.bmp image_filters2.app
	cp spheres.bmp image_filters2.app/Contents/MacOS/spheres.bmp
	$(CXX) $(CXXFLAGS) ../image_filters2.o $(PLATFORMSOURCES) -o image_filters2.app/Contents/MacOS/image_filters2 $(LIBS)

image_fltr_graph: ../image_fltr_graph.o $(PLATFORMSOURCES) spheres.bmp image_fltr_graph.app
	cp spheres.bmp image_fltr_graph.app/Contents/MacOS/spheres.bmp
	$(CXX) $(CXXFLAGS) ../image_fltr_graph.o $(PLATFORMSOURCES) -o image_fltr_graph.app/Contents/MacOS/image_fltr_graph $(LIBS)

image_perspective: ../image_perspective.o ../interactive_polygon.o $(PLATFORMSOURCES) spheres.bmp image_perspective.app
	cp spheres.bmp image_perspective.app/Contents/MacOS/spheres.bmp
	$(CXX) $(CXXFLAGS) ../image_perspective.o  ../interactive_polygon.o $(PLATFORMSOURCES) -o image_perspective.app/Contents/MacOS/image_perspective $(LIBS)

image_resample: ../image_resample.o  ../interactive_polygon.o $(PLATFORMSOURCES) spheres.bmp image_resample.app
	cp spheres.bmp image_resample.app/Contents/MacOS/spheres.bmp
	$(CXX) $(CXXFLAGS) ../image_resample.o ../interactive_polygon.o $(PLATFORMSOURCES) -o image_resample.app/Contents/MacOS/image_resample $(LIBS)

image_transforms: ../image_transforms.o $(PLATFORMSOURCES) spheres.bmp image_transforms.app
	cp spheres.bmp image_transforms.app/Contents/MacOS/spheres.bmp
	$(CXX) $(CXXFLAGS) ../image_transforms.o $(PLATFORMSOURCES) -o image_transforms.app/Contents/MacOS/image_transforms $(LIBS)

image1: ../image1.o $(PLATFORMSOURCES) spheres.bmp image1.app
	cp spheres.bmp image1.app/Contents/MacOS/spheres.bmp
	$(CXX) $(CXXFLAGS) ../image1.o $(PLATFORMSOURCES) -o image1.app/Contents/MacOS/image1 $(LIBS)

line_patterns: ../line_patterns.o $(PLATFORMSOURCES) 1.bmp 2.bmp 3.bmp 4.bmp 5.bmp 6.bmp 7.bmp 8.bmp 9.bmp line_patterns.app
	cp 1.bmp line_patterns.app/Contents/MacOS/1.bmp
	cp 2.bmp line_patterns.app/Contents/MacOS/2.bmp
	cp 3.bmp line_patterns.app/Contents/MacOS/3.bmp
	cp 4.bmp line_patterns.app/Contents/MacOS/4.bmp
	cp 5.bmp line_patterns.app/Contents/MacOS/5.bmp
	cp 6.bmp line_patterns.app/Contents/MacOS/6.bmp
	cp 7.bmp line_patterns.app/Contents/MacOS/7.bmp
	cp 8.bmp line_patterns.app/Contents/MacOS/8.bmp
	cp 9.bmp line_patterns.app/Contents/MacOS/9.bmp
	$(CXX) $(CXXFLAGS) ../line_patterns.o $(PLATFORMSOURCES) -o line_patterns.app/Contents/MacOS/line_patterns $(LIBS)

lion: ../lion.o ../parse_lion.o $(PLATFORMSOURCES) lion.app
	$(CXX) $(CXXFLAGS) ../lion.o ../parse_lion.o $(PLATFORMSOURCES) -o lion.app/Contents/MacOS/lion $(LIBS)

lion_lens: ../lion_lens.o ../parse_lion.o $(PLATFORMSOURCES) lion_lens.app
	$(CXX) $(CXXFLAGS) ../lion_lens.o ../parse_lion.o $(PLATFORMSOURCES) -o lion_lens.app/Contents/MacOS/lion_lens $(LIBS)

lion_outline: ../lion_outline.o ../parse_lion.o $(PLATFORMSOURCES) lion_outline.app
	$(CXX) $(CXXFLAGS) ../lion_outline.o ../parse_lion.o $(PLATFORMSOURCES) -o lion_outline.app/Contents/MacOS/lion_outline $(LIBS)

mol_view: ../mol_view.o $(PLATFORMSOURCES) 1.sdf mol_view.app
	cp 1.sdf mol_view.app/Contents/MacOS/1.sdf
	$(CXX) $(CXXFLAGS) ../mol_view.o $(PLATFORMSOURCES) -o mol_view.app/Contents/MacOS/mol_view $(LIBS)

multi_clip: ../multi_clip.o ../parse_lion.o $(PLATFORMSOURCES) multi_clip.app
	$(CXX) $(CXXFLAGS) ../multi_clip.o ../parse_lion.o $(PLATFORMSOURCES) -o multi_clip.app/Contents/MacOS/multi_clip $(LIBS)

pattern_fill: ../pattern_fill.o $(PLATFORMSOURCES) pattern_fill.app
	$(CXX) $(CXXFLAGS) ../pattern_fill.o $(PLATFORMSOURCES) -o pattern_fill.app/Contents/MacOS/pattern_fill $(LIBS)

pattern_perspective: ../pattern_perspective.o ../interactive_polygon.o $(PLATFORMSOURCES) agg.bmp pattern_perspective.app
	cp agg.bmp pattern_perspective.app/Contents/MacOS/agg.bmp
	$(CXX) $(CXXFLAGS) ../pattern_perspective.o ../interactive_polygon.o $(PLATFORMSOURCES) -o pattern_perspective.app/Contents/MacOS/pattern_perspective $(LIBS)

perspective: ../perspective.o ../interactive_polygon.o ../parse_lion.o $(PLATFORMSOURCES) perspective.app
	$(CXX) $(CXXFLAGS) ../perspective.o ../interactive_polygon.o ../parse_lion.o $(PLATFORMSOURCES) -o perspective.app/Contents/MacOS/perspective $(LIBS)

polymorphic_renderer: ../polymorphic_renderer.o $(PLATFORMSOURCES) polymorphic_renderer.app
	$(CXX) $(CXXFLAGS) ../polymorphic_renderer.o $(PLATFORMSOURCES) -o polymorphic_renderer.app/Contents/MacOS/polymorphic_renderer $(LIBS)

raster_text: ../raster_text.o $(PLATFORMSOURCES) raster_text.app
	$(CXX) $(CXXFLAGS) ../raster_text.o $(PLATFORMSOURCES) -o raster_text.app/Contents/MacOS/raster_text $(LIBS)

rasterizers: ../rasterizers.o $(PLATFORMSOURCES) rasterizers.app
	$(CXX) $(CXXFLAGS) ../rasterizers.o $(PLATFORMSOURCES) -o rasterizers.app/Contents/MacOS/rasterizers $(LIBS)

rasterizers2: ../rasterizers2.o $(PLATFORMSOURCES) rasterizers2.app
	$(CXX) $(CXXFLAGS) ../rasterizers2.o $(PLATFORMSOURCES) -o rasterizers2.app/Contents/MacOS/rasterizers2 $(LIBS)

rasterizer_compound: ../rasterizer_compound.o $(PLATFORMSOURCES) rasterizer_compound.app
	$(CXX) $(CXXFLAGS) ../rasterizer_compound.o $(PLATFORMSOURCES) -o rasterizer_compound.app/Contents/MacOS/rasterizer_compound $(LIBS)

rounded_rect: ../rounded_rect.o $(PLATFORMSOURCES) rounded_rect.app
	$(CXX) $(CXXFLAGS) ../rounded_rect.o $(PLATFORMSOURCES) -o rounded_rect.app/Contents/MacOS/rounded_rect $(LIBS)

scanline_boolean: ../scanline_boolean.o ../interactive_polygon.o $(PLATFORMSOURCES) scanline_boolean.app
	$(CXX) $(CXXFLAGS) ../scanline_boolean.o ../interactive_polygon.o $(PLATFORMSOURCES) -o scanline_boolean.app/Contents/MacOS/scanline_boolean $(LIBS)

scanline_boolean2: ../scanline_boolean2.o ../make_arrows.o ../make_gb_poly.o $(PLATFORMSOURCES) scanline_boolean2.app
	$(CXX) $(CXXFLAGS) ../scanline_boolean2.o ../make_arrows.o ../make_gb_poly.o $(PLATFORMSOURCES) -o scanline_boolean2.app/Contents/MacOS/scanline_boolean2 $(LIBS)

simple_blur: ../simple_blur.o ../parse_lion.o $(PLATFORMSOURCES) simple_blur.app
	$(CXX) $(CXXFLAGS) ../simple_blur.o ../parse_lion.o $(PLATFORMSOURCES) -o simple_blur.app/Contents/MacOS/simple_blur $(LIBS)

trans_polar: ../trans_polar.o $(PLATFORMSOURCES) trans_polar.app
	$(CXX) $(CXXFLAGS) ../trans_polar.o $(PLATFORMSOURCES) -o trans_polar.app/Contents/MacOS/trans_polar $(LIBS)


clean:
	rm -rf *.app
	rm -f ../*.o
	rm -f ../../src/platform/$(PLATFORM)/agg_platform_support.o
	rm -f ../../src/platform/$(PLATFORM)/agg_mac_pmap.o

agg.bmp:
	@echo "Required file missing!!"
	@echo "Please download http://www.antigrain.com/agg.bmp to /examples/macosx_carbon/"
	@echo "Attempting to retrieve via curl: (will fail if curl is not installed)"
	curl -O http://www.antigrain.com/agg.bmp

compositing.bmp:
	@echo "Required file missing!!"
	@echo "Please download http://www.antigrain.com/compositing.bmp to /examples/macosx_carbon/"
	@echo "Attempting to retrieve via curl: (will fail if curl is not installed)"
	curl -O http://www.antigrain.com/compositing.bmp

spheres.bmp:
	@echo "Required file missing!!"
	@echo "Please download http://www.antigrain.com/spheres.bmp to /examples/macosx_carbon/"
	@echo "Attempting to retrieve file: (will fail if curl is not installed)"
	curl -O http://www.antigrain.com/spheres.bmp

1.sdf:
	@echo "Required file missing!!"
	@echo "Please download http://www.antigrain.com/1.sdf to /examples/macosx_carbon/"
	@echo "Attempting to retrieve file: (will fail if curl is not installed)"
	curl -O http://www.antigrain.com/1.sdf

1.bmp:
	@echo "Required file missing!!"
	@echo "Please download http://www.antigrain.com/line_patterns.bmp.zip to /examples/macosx_carbon/ and decoompress it"
	@echo "Attempting to retrieve file: (will fail if curl is not installed)"
	curl -O http://www.antigrain.com/line_patterns.bmp.zip
	unzip -o line_patterns.bmp.zip

%.app:
	mkdir $*.app
	mkdir $*.app/Contents
	mkdir $*.app/Contents/MacOS

%.o:	%.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@

.PHONY : clean


