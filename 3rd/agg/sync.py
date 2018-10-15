#!/usr/bin/python

import os
import glob
import copy
import shutil
import platform

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

# XXX: make sure no no ascii chars in the path name.

SRC_ROOT_DIR=joinPath(os.getcwd(), '../../../3rd/agg/');
DST_ROOT_DIR=os.getcwd();

def copyFile(src, dst):
    s = joinPath(SRC_ROOT_DIR, src)
    d = joinPath(DST_ROOT_DIR, dst)
    print(s + '->' + d)
    dir=os.path.dirname(d)
    if os.path.exists(dir):
        shutil.copyfile(s, d)
    else:
        os.makedirs(dir)
        shutil.copyfile(s, d)

H_FILES=[
  "agg_array.h",
  "agg_basics.h",
  "agg_clip_liang_barsky.h",
  "agg_color_gray.h",
  "agg_color_rgba.h",
  "agg_config.h",
  "agg_conv_adaptor_vcgen.h",
  "agg_conv_stroke.h",
  "agg_dda_line.h",
  "agg_gamma_functions.h",
  "agg_gamma_lut.h",
  "agg_image_accessors.h",
  "agg_image_filters.h",
  "agg_math.h",
  "agg_math_stroke.h",
  "agg_pixfmt_base.h",
  "agg_pixfmt_rgba.h",
  "agg_pixfmt_rgb.h",
  "agg_pixfmt_rgb_packed.h",
  "agg_rasterizer_cells_aa.h",
  "agg_rasterizer_scanline_aa.h",
  "agg_rasterizer_scanline_aa_nogamma.h",
  "agg_rasterizer_sl_clip.h",
  "agg_renderer_base.h",
  "agg_renderer_scanline.h",
  "agg_rendering_buffer.h",
  "agg_scanline_u.h",
  "agg_shorten_path.h",
  "agg_span_allocator.h",
  "agg_span_image_filter.h",
  "agg_span_image_filter_rgba.h",
  "agg_span_image_filter_rgb.h",
  "agg_span_interpolator_linear.h",
  "agg_trans_affine.h",
  "agg_trans_viewport.h",
  "agg_vcgen_stroke.h",
  "agg_vertex_sequence.h"
]

for f in H_FILES:
    src=os.path.join('include', f);
    dst=os.path.join('include', f);
    copyFile(src, dst)

SRC_FILES=[
"agg_color_rgba.cpp",
"agg_image_filters.cpp",
"agg_sqrt_tables.cpp",
"agg_trans_affine.cpp",
"agg_vcgen_stroke.cpp"
]
for f in SRC_FILES:
    src=os.path.join('src', f);
    dst=os.path.join('src', f);
    copyFile(src, dst)

