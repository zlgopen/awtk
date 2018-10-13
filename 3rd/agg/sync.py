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
]

for f in H_FILES:
    src=os.path.join('agg', f);
    dst=os.path.join('agg', f);
    copyFile(src, dst)

SRC_FILES=[]
for f in SRC_FILES:
    src=os.path.join('src/agg', f);
    dst=os.path.join('agg', f);
    copyFile(src, dst)

