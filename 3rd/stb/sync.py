#!/usr/bin/python

import os
import glob
import copy
import shutil
import platform

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

# XXX: make sure no no ascii chars in the path name.

SRC_ROOT_DIR=joinPath(os.getcwd(), '../../../3rd/stb/');
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
    "stb_image.h",
    "stb_image_write.h",
    "stb_truetype.h",
    "stb_textedit.h",
    "stb_image_resize.h",
    "stb_c_lexer.h"
]

for f in H_FILES:
    copyFile(f, f)

