
ADD_EXECUTABLE( aa_demo ${WIN32GUI}
    aa_demo.cpp
)

ADD_EXECUTABLE( aa_test ${WIN32GUI}
    aa_test.cpp
)

ADD_EXECUTABLE( alpha_gradient ${WIN32GUI}
    alpha_gradient.cpp
)

ADD_EXECUTABLE( alpha_mask ${WIN32GUI}
    alpha_mask.cpp
    parse_lion.cpp
)

ADD_EXECUTABLE( alpha_mask2 ${WIN32GUI}
    alpha_mask.cpp
    parse_lion.cpp
)

ADD_EXECUTABLE( alpha_mask3 ${WIN32GUI}
    alpha_mask.cpp
    make_arrows.cpp
    make_gb_poly.cpp
    parse_lion.cpp
)

ADD_EXECUTABLE( bezier_div ${WIN32GUI}
    bezier_div.cpp
    interactive_polygon.cpp
)

ADD_EXECUTABLE( blend_color ${WIN32GUI}
    blend_color.cpp
)

ADD_EXECUTABLE( blur ${WIN32GUI}
    blur.cpp
)

ADD_EXECUTABLE( bspline ${WIN32GUI}
    bspline.cpp
    interactive_polygon.cpp
)

ADD_EXECUTABLE( circles ${WIN32GUI}
    circles.cpp
)

ADD_EXECUTABLE( component_rendering ${WIN32GUI}
    component_rendering.cpp
)

ADD_EXECUTABLE( compositing ${WIN32GUI}
    compositing.cpp
)

ADD_EXECUTABLE( compositing2 ${WIN32GUI}
    compositing2.cpp
)

ADD_EXECUTABLE( conv_contour ${WIN32GUI}
    conv_contour.cpp
)

ADD_EXECUTABLE( conv_dash_marker ${WIN32GUI}
    conv_dash_marker.cpp
)

ADD_EXECUTABLE( conv_stroke ${WIN32GUI}
    conv_stroke.cpp
)

ADD_EXECUTABLE( distortions ${WIN32GUI}
    distortions.cpp
)

ADD_EXECUTABLE( flash_rasterizer ${WIN32GUI}
    flash_rasterizer.cpp
)

ADD_EXECUTABLE( flash_rasterizer2 ${WIN32GUI}
    flash_rasterizer2.cpp
)

IF ( agg_USE_FREETYPE )
    ADD_EXECUTABLE( freetype_test ${WIN32GUI}
        freetype_test.cpp
        make_arrows.cpp
        make_gb_poly.cpp
    )
ENDIF ( agg_USE_FREETYPE )

ADD_EXECUTABLE( gamma_correction ${WIN32GUI}
    gamma_correction.cpp
)

ADD_EXECUTABLE( gamma_ctrl ${WIN32GUI}
    gamma_ctrl.cpp
)

ADD_EXECUTABLE( gamma_tuner ${WIN32GUI}
    gamma_tuner.cpp
)

ADD_EXECUTABLE( gouraud ${WIN32GUI}
    gouraud.cpp
)

ADD_EXECUTABLE( gouraud_mesh ${WIN32GUI}
    gouraud_mesh.cpp
)

IF ( agg_USE_GPC )
    ADD_EXECUTABLE( gpc_test ${WIN32GUI}
        gpc_test.cpp
        make_arrows.cpp
        make_gb_poly.cpp
        )
ENDIF ( agg_USE_GPC )

ADD_EXECUTABLE( gradients ${WIN32GUI}
    gradients.cpp
)

ADD_EXECUTABLE( gradient_focal ${WIN32GUI}
    gradient_focal.cpp
)

ADD_EXECUTABLE( gradients_contour ${WIN32GUI}
    gradients_contour.cpp
    make_arrows.cpp
    make_gb_poly.cpp
    parse_lion.cpp
)

ADD_EXECUTABLE( graph_test ${WIN32GUI}
    graph_test.cpp
)

ADD_EXECUTABLE( idea ${WIN32GUI}
    idea.cpp
)

ADD_EXECUTABLE( image1 ${WIN32GUI}
    image1.cpp
)

ADD_EXECUTABLE( image_alpha ${WIN32GUI}
    image_alpha.cpp
)

ADD_EXECUTABLE( image_filters ${WIN32GUI}
    image_filters.cpp
)

ADD_EXECUTABLE( image_filters2 ${WIN32GUI}
    image_filters2.cpp
)

ADD_EXECUTABLE( image_fltr_graph ${WIN32GUI}
    image_fltr_graph.cpp
)
ADD_EXECUTABLE( image_perspective ${WIN32GUI}
    image_perspective.cpp
    interactive_polygon.cpp
)

ADD_EXECUTABLE( image_resample ${WIN32GUI}
    image_resample.cpp
    interactive_polygon.cpp
)

ADD_EXECUTABLE( image_transforms ${WIN32GUI}
    image_transforms.cpp
)

ADD_EXECUTABLE( line_patterns ${WIN32GUI}
    line_patterns.cpp
)

ADD_EXECUTABLE( line_patterns_clip ${WIN32GUI}
    line_patterns_clip.cpp
)

ADD_EXECUTABLE( lion ${WIN32GUI}
    lion.cpp
    parse_lion.cpp
)

ADD_EXECUTABLE( lion_lens ${WIN32GUI}
    lion_lens.cpp
    parse_lion.cpp
)

ADD_EXECUTABLE( lion_outline ${WIN32GUI}
    lion_outline.cpp
    parse_lion.cpp
)

ADD_EXECUTABLE( mol_view ${WIN32GUI}
    mol_view.cpp
)

ADD_EXECUTABLE( multi_clip ${WIN32GUI}
    multi_clip.cpp
    parse_lion.cpp
)

ADD_EXECUTABLE( pattern_fill ${WIN32GUI}
    pattern_fill.cpp
)

ADD_EXECUTABLE( pattern_perspective ${WIN32GUI}
    pattern_perspective.cpp
    interactive_polygon.cpp
)

ADD_EXECUTABLE( pattern_resample ${WIN32GUI}
    pattern_resample.cpp
    interactive_polygon.cpp
)

ADD_EXECUTABLE( perspective ${WIN32GUI}
    perspective.cpp
    interactive_polygon.cpp
    parse_lion.cpp
)

ADD_EXECUTABLE( polymorphic_renderer ${WIN32GUI}
    polymorphic_renderer.cpp
)

ADD_EXECUTABLE( rasterizers ${WIN32GUI}
    rasterizers.cpp
)

ADD_EXECUTABLE( rasterizers2 ${WIN32GUI}
    rasterizers2.cpp
)

ADD_EXECUTABLE( rasterizer_compound ${WIN32GUI}
    rasterizer_compound.cpp
)

ADD_EXECUTABLE( raster_text ${WIN32GUI}
    raster_text.cpp
)

ADD_EXECUTABLE( rounded_rect ${WIN32GUI}
    rounded_rect.cpp
)

ADD_EXECUTABLE( scanline_boolean ${WIN32GUI}
    scanline_boolean.cpp
    interactive_polygon.cpp
)

ADD_EXECUTABLE( scanline_boolean2 ${WIN32GUI}
    scanline_boolean2.cpp
    make_arrows.cpp
    make_gb_poly.cpp
)

ADD_EXECUTABLE( simple_blur ${WIN32GUI}
    simple_blur.cpp
    parse_lion.cpp
)

IF(WIN32)

    ADD_EXECUTABLE( trans_curve1 ${WIN32GUI}
        trans_curve1.cpp
        interactive_polygon.cpp
    )

    ADD_EXECUTABLE( trans_curve2 ${WIN32GUI}
        trans_curve2.cpp
        interactive_polygon.cpp
    )

    ADD_EXECUTABLE( truetype_test ${WIN32GUI}
        truetype_test.cpp
    )

ENDIF(WIN32)

ADD_EXECUTABLE( trans_polar ${WIN32GUI}
    trans_polar.cpp
)

IF ( agg_USE_EXPAT )
    ADD_EXECUTABLE( svg_test ${WIN32GUI}
        ./svg_viewer/svg_test.cpp
        ./svg_viewer/agg_svg_exception.h
        ./svg_viewer/agg_svg_parser.cpp
        ./svg_viewer/agg_svg_parser.h
        ./svg_viewer/agg_svg_path_renderer.cpp
        ./svg_viewer/agg_svg_path_renderer.h
        ./svg_viewer/agg_svg_path_tokenizer.cpp
        ./svg_viewer/agg_svg_path_tokenizer.h
        )
ENDIF ( agg_USE_EXPAT )

IF(WIN32)
    ADD_EXECUTABLE( pure_api ${WIN32GUI}
        ./win32_api/pure_api/pure_api.h
        ./win32_api/pure_api/pure_api.cpp
        ./win32_api/pure_api/resource.h
        ./win32_api/pure_api/StdAfx.h
        ./win32_api/pure_api/StdAfx.cpp
        ./win32_api/pure_api/pure_api.rc
        parse_lion.cpp
            
    )
ENDIF(WIN32)

IF( agg_USE_AGG2D )
    ADD_EXECUTABLE( agg2_demo ${WIN32GUI}
        agg2d_demo.cpp
    )
ENDIF( agg_USE_AGG2D )





    
