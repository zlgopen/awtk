#!/usr/bin/python

import os
import glob
import copy
import shutil
import platform

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

# XXX: make sure no no ascii chars in the path name.

SRC_ROOT_DIR=joinPath(os.getcwd(), '../../../3rd/nanovg/');
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

BASE_FILES=['fontstash.h', 'nanovg.c', 'nanovg.h', 'stb_image.h', 'stb_truetype.h']
for f in BASE_FILES:
    src=os.path.join('src', f);
    dst=os.path.join('base', f);
    copyFile(src, dst)

GL_FILES=['nanovg_gl.h', 'nanovg_gl_utils.h']
for f in GL_FILES:
    src=os.path.join('src', f);
    dst=os.path.join('gl', f);
    copyFile(src, dst)


SRC_ROOT_DIR=joinPath(os.getcwd(), '../../../3rd/bgfx-awtk/nanovg/');
BGFX_FILES=['fs_nanovg_fill.sc', 'nanovg_bgfxEx.h', 'vs_nanovg_fill.sc',
        'fontstash.h','nanovg_bgfx.cpp','varying.def.sc',
        'fs_nanovg_fill.bin.h','nanovg_bgfx.h','vs_nanovg_fill.bin.h'];

for f in BGFX_FILES:
    src=os.path.join('bgfx', f);
    dst=os.path.join('bgfx', f);
    copyFile(src, dst)
