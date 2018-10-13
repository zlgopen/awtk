#!/usr/bin/python

import os
import glob
import copy
import shutil
import platform

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

# XXX: make sure no no ascii chars in the path name.

SRC_ROOT_DIR=joinPath(os.getcwd(), '../../../3rd/agge/');
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
    "clipper.h",
    "config.h",
    "filling_rules.h",
    "math.h",
    "memory.h",
    "path.h",
    "pod_vector.h",
    "precise_delta.h",
    "rasterizer.h",
    "renderer.h",
    "scanline.h",
    "stroke.h",
    "stroke_features.h",
    "tools.h",
    "types.h",
    "vector_rasterizer.h",
    "vertex_sequence.h"
]

for f in H_FILES:
    src=os.path.join('agge', f);
    dst=os.path.join('agge', f);
    copyFile(src, dst)

SRC_FILES=['math.cpp', 'stroke.cpp', 'stroke_features.cpp', 'vector_rasterizer.cpp']
for f in SRC_FILES:
    src=os.path.join('src/agge', f);
    dst=os.path.join('agge', f);
    copyFile(src, dst)

