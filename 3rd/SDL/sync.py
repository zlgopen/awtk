#!/usr/bin/python

import os
import glob
import copy
import shutil
import platform

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

# XXX: make sure no no ascii chars in the path name.

SRC_ROOT_DIR=joinPath(os.getcwd(), '../../../3rd/SDL/');
DST_ROOT_DIR=os.getcwd();

def copyFiles(src):
    s = joinPath(SRC_ROOT_DIR, src)
    d = joinPath(DST_ROOT_DIR, src)
    print(s + '->' + d)
    shutil.rmtree(d, True)
    shutil.copytree(s, d)

copyFiles('src');
