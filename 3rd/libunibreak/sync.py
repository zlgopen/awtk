#!/usr/bin/python

import os
import glob
import copy
import shutil
import platform

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

# XXX: make sure no no ascii chars in the path name.

SRC_ROOT_DIR=joinPath(os.getcwd(), '../../../3rd/libunibreak/src');
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
    "graphemebreak.h",
    "graphemebreakdef.h",
    "linebreak.h",
    "linebreakdef.h",
    "unibreakbase.h",
    "unibreakdef.h",
    "wordbreak.h",
    "emojidef.h",
    "wordbreakdef.h"
]

for f in H_FILES:
    copyFile(f, f)

SRC_FILES=['graphemebreak.c', 'graphemebreakdata.c', 'linebreak.c', 'wordbreakdata.c', 'linebreakdata.c', 'emojidata.c',
    'emojidef.c', 'linebreakdef.c', 'unibreakbase.c', 'unibreakdef.c', 'wordbreak.c', 'linebreakdata.c']
for f in SRC_FILES:
    copyFile(f, f)

