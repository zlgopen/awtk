#==============================================
# Digital Mars Makefile 
#
# Modify this variable if necessary
#
DM   = \dm\bin
#

CXX  = $(DM)\dmc.exe -c -I..\..\include -I..\..\gpc -I..\..\font_win32_tt 
C    = $(DM)\dmc.exe -c -o+speed -I..\..\gpc 
LINK = $(DM)\dmc.exe -L$(DM)\link.exe -L/SU:WINDOWS gdi32.lib
LIB  = $(DM)\lib.exe

all: aa_demo.exe \
     aa_test.exe \
     alpha_gradient.exe \
     alpha_mask.exe \
     alpha_mask2.exe \
     alpha_mask3.exe \
     bezier_div.exe \
     blur.exe \
     blend_color.exe \
     bspline.exe \
     circles.exe \
     component_rendering.exe \
     compositing.exe \
     compositing2.exe \
     conv_contour.exe \
     conv_dash_marker.exe \
     conv_stroke.exe \
     distortions.exe \
     flash_rasterizer.exe \
     flash_rasterizer2.exe \
     gamma_correction.exe \
     gamma_ctrl.exe \
     gamma_tuner.exe \
     gouraud.exe \
     gouraud_mesh.exe \
     gpc_test.exe \
     gradient_focal.exe \
     gradients.exe \
     graph_test.exe \
     idea.exe \
     image1.exe \
     image_alpha.exe \
     image_filters.exe \
     image_filters2.exe \
     image_fltr_graph.exe \
     image_perspective.exe \
     image_resample.exe \
     image_transforms.exe \
     line_patterns.exe \
     line_patterns_clip.exe \
     lion.exe \
     lion_lens.exe \
     lion_outline.exe \
     mol_view.exe \
     multi_clip.exe \
     pattern_fill.exe \
     pattern_perspective.exe \
     pattern_resample.exe \
     perspective.exe \
     polymorphic_renderer.exe \
     rasterizers.exe \
     rasterizers2.exe \
     rasterizer_compound.exe \
     raster_text.exe \
     rounded_rect.exe \
     scanline_boolean.exe \
     scanline_boolean2.exe \
     simple_blur.exe \
     trans_curve1.exe \
     trans_curve2.exe \
     trans_polar.exe \
     truetype_test.exe

aa_demo.exe: aa_demo.obj agg.lib
	$(LINK) $**

aa_demo.obj: ..\aa_demo.cpp
	$(CXX) $** -o$@

aa_test.exe: aa_test.obj agg.lib
	$(LINK) $**

aa_test.obj: ..\aa_test.cpp
	$(CXX) $** -o$@

alpha_gradient.exe: alpha_gradient.obj agg.lib
	$(LINK) $**

alpha_gradient.obj: ..\alpha_gradient.cpp
	$(CXX) $** -o$@

alpha_mask.exe: alpha_mask.obj parse_lion.obj agg.lib
	$(LINK) $**

alpha_mask.obj: ..\alpha_mask.cpp
	$(CXX) $** -o$@

alpha_mask2.exe: alpha_mask2.obj parse_lion.obj agg.lib
	$(LINK) $**

alpha_mask2.obj: ..\alpha_mask2.cpp
	$(CXX) $** -o$@

alpha_mask3.exe: alpha_mask3.obj make_arrows.obj make_gb_poly.obj agg.lib
	$(LINK) $**

alpha_mask3.obj: ..\alpha_mask3.cpp
	$(CXX) $** -o$@

bezier_div.exe: bezier_div.obj agg.lib
	$(LINK) $**

bezier_div.obj: ..\bezier_div.cpp
	$(CXX) $** -o$@

blur.exe: blur.obj agg.lib
	$(LINK) $**

blur.obj: ..\blur.cpp
	$(CXX) $** -o$@

blend_color.exe: blend_color.obj agg.lib
	$(LINK) $**

blend_color.obj: ..\blend_color.cpp
	$(CXX) $** -o$@

bspline.exe: bspline.obj interactive_polygon.obj agg.lib
	$(LINK) $**

bspline.obj: ..\bspline.cpp
	$(CXX) $** -o$@

circles.exe: circles.obj agg.lib
	$(LINK) $**

circles.obj: ..\circles.cpp
	$(CXX) $** -o$@

component_rendering.exe: component_rendering.obj agg.lib
	$(LINK) $**

component_rendering.obj: ..\component_rendering.cpp
	$(CXX) $** -o$@

compositing.exe: compositing.obj agg.lib
	$(LINK) $**

compositing.obj: ..\compositing.cpp
	$(CXX) $** -o$@

compositing2.exe: compositing2.obj agg.lib
	$(LINK) $**

compositing2.obj: ..\compositing2.cpp
	$(CXX) $** -o$@

conv_contour.exe: conv_contour.obj agg.lib
	$(LINK) $**

conv_contour.obj: ..\conv_contour.cpp
	$(CXX) $** -o$@

conv_dash_marker.exe: conv_dash_marker.obj agg.lib
	$(LINK) $**

conv_dash_marker.obj: ..\conv_dash_marker.cpp
	$(CXX) $** -o$@

conv_stroke.exe: conv_stroke.obj agg.lib
	$(LINK) $**

conv_stroke.obj: ..\conv_stroke.cpp
	$(CXX) $** -o$@

distortions.exe: distortions.obj agg.lib
	$(LINK) $**

distortions.obj: ..\distortions.cpp
	$(CXX) $** -o$@

flash_rasterizer.exe: flash_rasterizer.obj agg.lib
	$(LINK) $**

flash_rasterizer.obj: ..\flash_rasterizer.cpp
	$(CXX) $** -o$@

flash_rasterizer2.exe: flash_rasterizer2.obj agg.lib
	$(LINK) $**

flash_rasterizer2.obj: ..\flash_rasterizer2.cpp
	$(CXX) $** -o$@

gamma_correction.exe: gamma_correction.obj agg.lib
	$(LINK) $**

gamma_correction.obj: ..\gamma_correction.cpp
	$(CXX) $** -o$@

gamma_ctrl.exe: gamma_ctrl.obj agg.lib
	$(LINK) $**

gamma_ctrl.obj: ..\gamma_ctrl.cpp
	$(CXX) $** -o$@

gamma_tuner.exe: gamma_tuner.obj agg.lib
	$(LINK) $**

gamma_tuner.obj: ..\gamma_tuner.cpp
	$(CXX) $** -o$@

gouraud.exe: gouraud.obj agg.lib
	$(LINK) $**

gouraud.obj: ..\gouraud.cpp
	$(CXX) $** -o$@

gouraud_mesh.exe: gouraud_mesh.obj agg.lib
	$(LINK) $**

gouraud_mesh.obj: ..\gouraud_mesh.cpp
	$(CXX) $** -o$@

gpc_test.exe: gpc_test.obj  make_arrows.obj make_gb_poly.obj agg.lib
	$(LINK) $**

gpc_test.obj: ..\gpc_test.cpp
	$(CXX) $** -o$@

gradient_focal.exe: gradient_focal.obj agg.lib
	$(LINK) $**

gradient_focal.obj: ..\gradient_focal.cpp
	$(CXX) $** -o$@

gradients.exe: gradients.obj agg.lib
	$(LINK) $**

gradients.obj: ..\gradients.cpp
	$(CXX) $** -o$@

graph_test.exe: graph_test.obj agg.lib
	$(LINK) $**

graph_test.obj: ..\graph_test.cpp
	$(CXX) $** -o$@

idea.exe: idea.obj agg.lib
	$(LINK) $**

idea.obj: ..\idea.cpp
	$(CXX) $** -o$@

image1.exe: image1.obj agg.lib
	$(LINK) $**

image1.obj: ..\image1.cpp
	$(CXX) $** -o$@

image_alpha.exe: image_alpha.obj agg.lib
	$(LINK) $**

image_alpha.obj: ..\image_alpha.cpp
	$(CXX) $** -o$@

image_filters.exe: image_filters.obj agg.lib
	$(LINK) $**

image_filters.obj: ..\image_filters.cpp
	$(CXX) $** -o$@

image_filters2.exe: image_filters2.obj agg.lib
	$(LINK) $**

image_filters2.obj: ..\image_filters2.cpp
	$(CXX) $** -o$@

image_fltr_graph.exe: image_fltr_graph.obj agg.lib
	$(LINK) $**

image_fltr_graph.obj: ..\image_fltr_graph.cpp
	$(CXX) $** -o$@

image_perspective.exe: image_perspective.obj interactive_polygon.obj agg.lib
	$(LINK) $**

image_perspective.obj: ..\image_perspective.cpp
	$(CXX) $** -o$@

image_resample.exe: image_resample.obj interactive_polygon.obj agg.lib
	$(LINK) $**

image_resample.obj: ..\image_resample.cpp
	$(CXX) $** -o$@

image_transforms.exe: image_transforms.obj agg.lib
	$(LINK) $**

image_transforms.obj: ..\image_transforms.cpp
	$(CXX) $** -o$@

line_patterns.exe: line_patterns.obj agg.lib
	$(LINK) $**

line_patterns.obj: ..\line_patterns.cpp
	$(CXX) $** -o$@

line_patterns_clip.exe: line_patterns_clip.obj agg.lib
	$(LINK) $**

line_patterns_clip.obj: ..\line_patterns_clip.cpp
	$(CXX) $** -o$@

lion.exe: lion.obj parse_lion.obj agg.lib
	$(LINK) $**

lion.obj: ..\lion.cpp
	$(CXX) $** -o$@

lion_lens.exe: lion_lens.obj parse_lion.obj agg.lib
	$(LINK) $**

lion_lens.obj: ..\lion_lens.cpp
	$(CXX) $** -o$@

lion_outline.exe: lion_outline.obj parse_lion.obj agg.lib
	$(LINK) $**

lion_outline.obj: ..\lion_outline.cpp
	$(CXX) $** -o$@

mol_view.exe: mol_view.obj agg.lib
	$(LINK) $**

mol_view.obj: ..\mol_view.cpp
	$(CXX) $** -o$@

multi_clip.exe: multi_clip.obj parse_lion.obj agg.lib
	$(LINK) $**

multi_clip.obj: ..\multi_clip.cpp
	$(CXX) $** -o$@

pattern_fill.exe: pattern_fill.obj agg.lib
	$(LINK) $**

pattern_fill.obj: ..\pattern_fill.cpp
	$(CXX) $** -o$@

pattern_perspective.exe: pattern_perspective.obj interactive_polygon.obj agg.lib
	$(LINK) $**

pattern_perspective.obj: ..\pattern_perspective.cpp
	$(CXX) $** -o$@

pattern_resample.exe: pattern_resample.obj interactive_polygon.obj agg.lib
	$(LINK) $**

pattern_resample.obj: ..\pattern_resample.cpp
	$(CXX) $** -o$@

perspective.exe: perspective.obj parse_lion.obj interactive_polygon.obj agg.lib
	$(LINK) $**

perspective.obj: ..\perspective.cpp
	$(CXX) $** -o$@

polymorphic_renderer.exe: polymorphic_renderer.obj agg.lib
	$(LINK) $**

polymorphic_renderer.obj: ..\polymorphic_renderer.cpp
	$(CXX) $** -o$@

rasterizers.exe: rasterizers.obj agg.lib
	$(LINK) $**

rasterizers.obj: ..\rasterizers.cpp
	$(CXX) $** -o$@

rasterizers2.exe: rasterizers2.obj agg.lib
	$(LINK) $**

rasterizers2.obj: ..\rasterizers2.cpp
	$(CXX) $** -o$@

rasterizer_compound.exe: rasterizer_compound.obj agg.lib
	$(LINK) $**

rasterizer_compound.obj: ..\rasterizer_compound.cpp
	$(CXX) $** -o$@

raster_text.exe: raster_text.obj agg.lib
	$(LINK) $**

raster_text.obj: ..\raster_text.cpp
	$(CXX) $** -o$@

rounded_rect.exe: rounded_rect.obj agg.lib
	$(LINK) $**

rounded_rect.obj: ..\rounded_rect.cpp
	$(CXX) $** -o$@

scanline_boolean.exe: scanline_boolean.obj interactive_polygon.obj agg.lib
	$(LINK) $**

scanline_boolean.obj: ..\scanline_boolean.cpp
	$(CXX) $** -o$@

scanline_boolean2.exe: scanline_boolean2.obj  make_arrows.obj make_gb_poly.obj agg.lib
	$(LINK) $**

scanline_boolean2.obj: ..\scanline_boolean2.cpp
	$(CXX) $** -o$@

simple_blur.exe: simple_blur.obj parse_lion.obj agg.lib
	$(LINK) $**

simple_blur.obj: ..\simple_blur.cpp
	$(CXX) $** -o$@

trans_curve1.exe: trans_curve1.obj interactive_polygon.obj agg.lib
	$(LINK) $**

trans_curve1.obj: ..\trans_curve1.cpp
	$(CXX) $** -o$@

trans_curve2.exe: trans_curve2.obj interactive_polygon.obj agg.lib
	$(LINK) $**

trans_curve2.obj: ..\trans_curve2.cpp
	$(CXX) $** -o$@

trans_polar.exe: trans_polar.obj agg.lib
	$(LINK) $**

trans_polar.obj: ..\trans_polar.cpp
	$(CXX) $** -o$@

truetype_test.exe: truetype_test.obj agg.lib
	$(LINK) $**

truetype_test.obj: ..\truetype_test.cpp
	$(CXX) $** -o$@

parse_lion.obj: ..\parse_lion.cpp
	$(CXX) $** -o$@

make_arrows.obj: ..\make_arrows.cpp
	$(CXX) $** -o$@

make_gb_poly.obj: ..\make_gb_poly.cpp
	$(CXX) $** -o$@

interactive_polygon.obj: ..\interactive_polygon.cpp
	$(CXX) $** -o$@

agg.lib: agg_arc.obj agg_arrowhead.obj agg_bezier_arc.obj agg_bspline.obj agg_curves.obj \
         agg_embedded_raster_fonts.obj agg_gsv_text.obj agg_image_filters.obj \
         agg_line_aa_basics.obj agg_line_profile_aa.obj agg_rounded_rect.obj \
         agg_sqrt_tables.obj agg_trans_affine.obj agg_trans_double_path.obj \
         agg_trans_single_path.obj agg_trans_warp_magnifier.obj agg_vcgen_bspline.obj \
         agg_vcgen_contour.obj agg_vcgen_dash.obj agg_vcgen_markers_term.obj \
         agg_vcgen_smooth_poly1.obj agg_vcgen_stroke.obj agg_vpgen_clip_polygon.obj \
         agg_vpgen_clip_polyline.obj agg_vpgen_segmentator.obj agg_font_win32_tt.obj \
         agg_bezier_ctrl.obj agg_cbox_ctrl.obj agg_gamma_ctrl.obj agg_gamma_spline.obj \
         agg_polygon_ctrl.obj agg_rbox_ctrl.obj agg_scale_ctrl.obj agg_slider_ctrl.obj \
         agg_spline_ctrl.obj agg_platform_support.obj agg_win32_bmp.obj gpc.obj
	$(LIB) -c $@ $**

agg_arc.obj:                        ..\..\src\agg_arc.cpp
	$(CXX) $** -o$@

agg_arrowhead.obj:                  ..\..\src\agg_arrowhead.cpp
	$(CXX) $** -o$@

agg_bezier_arc.obj:                 ..\..\src\agg_bezier_arc.cpp
	$(CXX) $** -o$@

agg_bspline.obj:                    ..\..\src\agg_bspline.cpp
	$(CXX) $** -o$@

agg_curves.obj:                     ..\..\src\agg_curves.cpp
	$(CXX) $** -o$@

agg_embedded_raster_fonts.obj:      ..\..\src\agg_embedded_raster_fonts.cpp
	$(CXX) $** -o$@

agg_gsv_text.obj:                   ..\..\src\agg_gsv_text.cpp
	$(CXX) $** -o$@

agg_image_filters.obj:              ..\..\src\agg_image_filters.cpp
	$(CXX) $** -o$@

agg_line_aa_basics.obj:             ..\..\src\agg_line_aa_basics.cpp
	$(CXX) $** -o$@

agg_line_profile_aa.obj:            ..\..\src\agg_line_profile_aa.cpp
	$(CXX) $** -o$@

agg_rounded_rect.obj:               ..\..\src\agg_rounded_rect.cpp
	$(CXX) $** -o$@

agg_sqrt_tables.obj:                ..\..\src\agg_sqrt_tables.cpp
	$(CXX) $** -o$@

agg_trans_affine.obj:               ..\..\src\agg_trans_affine.cpp
	$(CXX) $** -o$@

agg_trans_double_path.obj:          ..\..\src\agg_trans_double_path.cpp
	$(CXX) $** -o$@

agg_trans_single_path.obj:          ..\..\src\agg_trans_single_path.cpp
	$(CXX) $** -o$@

agg_trans_warp_magnifier.obj:       ..\..\src\agg_trans_warp_magnifier.cpp
	$(CXX) $** -o$@

agg_vcgen_bspline.obj:              ..\..\src\agg_vcgen_bspline.cpp
	$(CXX) $** -o$@

agg_vcgen_contour.obj:              ..\..\src\agg_vcgen_contour.cpp
	$(CXX) $** -o$@

agg_vcgen_dash.obj:                 ..\..\src\agg_vcgen_dash.cpp
	$(CXX) $** -o$@

agg_vcgen_markers_term.obj:         ..\..\src\agg_vcgen_markers_term.cpp
	$(CXX) $** -o$@

agg_vcgen_smooth_poly1.obj:         ..\..\src\agg_vcgen_smooth_poly1.cpp
	$(CXX) $** -o$@

agg_vcgen_stroke.obj:               ..\..\src\agg_vcgen_stroke.cpp
	$(CXX) $** -o$@

agg_vpgen_clip_polygon.obj:         ..\..\src\agg_vpgen_clip_polygon.cpp
	$(CXX) $** -o$@

agg_vpgen_clip_polyline.obj:        ..\..\src\agg_vpgen_clip_polyline.cpp
	$(CXX) $** -o$@

agg_vpgen_segmentator.obj:          ..\..\src\agg_vpgen_segmentator.cpp
	$(CXX) $** -o$@

agg_font_win32_tt.obj:              ..\..\font_win32_tt\agg_font_win32_tt.cpp
	$(CXX) $** -o$@

agg_bezier_ctrl.obj:                ..\..\src\ctrl\agg_bezier_ctrl.cpp
	$(CXX) $** -o$@

agg_cbox_ctrl.obj:                  ..\..\src\ctrl\agg_cbox_ctrl.cpp
	$(CXX) $** -o$@

agg_gamma_ctrl.obj:                 ..\..\src\ctrl\agg_gamma_ctrl.cpp
	$(CXX) $** -o$@

agg_gamma_spline.obj:               ..\..\src\ctrl\agg_gamma_spline.cpp
	$(CXX) $** -o$@

agg_polygon_ctrl.obj:               ..\..\src\ctrl\agg_polygon_ctrl.cpp
	$(CXX) $** -o$@

agg_rbox_ctrl.obj:                  ..\..\src\ctrl\agg_rbox_ctrl.cpp
	$(CXX) $** -o$@

agg_scale_ctrl.obj:                 ..\..\src\ctrl\agg_scale_ctrl.cpp
	$(CXX) $** -o$@

agg_slider_ctrl.obj:                ..\..\src\ctrl\agg_slider_ctrl.cpp
	$(CXX) $** -o$@

agg_spline_ctrl.obj:                ..\..\src\ctrl\agg_spline_ctrl.cpp
	$(CXX) $** -o$@

agg_platform_support.obj:           ..\..\src\platform\win32\agg_platform_support.cpp
	$(CXX) $** -o$@

agg_win32_bmp.obj:                  ..\..\src\platform\win32\agg_win32_bmp.cpp
	$(CXX) $** -o$@

gpc.obj:                            ..\..\gpc\gpc.c
	$(C) $(CFLAGS) $** -o$@

clean:
	del *.obj
	del agg.lib
	del *.exe
	del *.map

