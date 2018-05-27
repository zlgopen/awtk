================================================================================

 AGG Contribution Pack - Gradients 1 (AGG CP - Gradients 1)
 http://milan.marusinec.sk/aggcp

 For Anti-Grain Geometry - Version 2.4 
 http://www.antigrain.org

 Contribution Created By:
  Milan Marusinec alias Milano
  milan@marusinec.sk
  Copyright (c) 2007-2008

================================================================================
 How To Install
================================================================================
 
 1. Copy \examples and \include directories directly to the \agg-2.4 directory.

 2. MS Visual Studio: In Solution Explorer click "Add Existing Project ..." 
    from \examples\win32_api\gradients_contour subdirectory.

 3. Build, Run, Enjoy.

================================================================================
 About
================================================================================
 
 This AGG Contribution Pack (CP) extends existing set of gradient functions 
 found in AGG 2.4 and later and is not a part of original source code 
 distribution package, so you have to download and install this CP separately.
 
 Following additional gradient functions are included:
 
 gradient_contour
 ================
 
 located in "agg_span_gradient_contour.h"
 
 Creates color transitions from shape defined by an arbitrary (in fact any)
 path. It computes so called Distance Transform (DT) from image produced by 
 only stroking the path, which is then the source of color level in 
 the underlying gradient function. 
 
 Contour gradient can be used in two forms:
 
 One is to define shape for contour different from shape of object being drawn. 
 Second is to use the same shape for contour and for drawing (AutoContour).

 
 gradient_conic_angle
 ====================
 
 located in "gradients_contour.cpp"
 
 Assymetric conic gradient (also called angle) is the same as conic, but the ray 
 of light with color transitions going counter clockwise travels whole circle 
 instead of just half (as with conic).
 
 Note: Implementation of this one is so tiny, it doesn't have it's own header
       file. If you want to use it, just copy and paste it from the demo file.
       

 gradient_image
 ==============
 
 located in "agg_span_gradient_image.h"
 
 Bitmap gradient is very similar to pattern fill, but works in the framework 
 of gradient functions interfaces. Because of that all interpolator 
 transformations from gradient span generator can be applied to this kind 
 of fill (all affine transformations, perspective, bilinear & warp).
 
 Note: This gradient function doesn't generates indexes for colors used.
 
       When constructing the span_gradient object, the color function type is 
       eg. agg::one_color_function<agg::rgba8> and instance of that color 
       function is retrieved by calling special function of gradient_image 
       class -> gradient_func.color_function().
            
       See the "gradients_countour.cpp" demo for more details.

================================================================================
 End of file
================================================================================
