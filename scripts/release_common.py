import os
import sys
import copy
import glob
import shutil
import platform
import fnmatch

BIN_DIR = 'bin'
EXE_NAME = 'demoui'
ASSETS_DIR = 'assets'
OUTPUT_DIR = 'release'

def init(exe):
    global BIN_DIR
    global EXE_NAME
    global ASSETS_DIR
    global OUTPUT_DIR

    EXE_NAME = exe
    APP_ROOT = os.getcwd()
    BIN_DIR = joinPath(APP_ROOT, 'bin')
    ASSETS_DIR = joinPath(APP_ROOT, 'assets')
    OUTPUT_DIR = joinPath(APP_ROOT, 'release')
    if not os.path.exists(ASSETS_DIR):
      ASSETS_DIR = joinPath(APP_ROOT, 'demos/assets')
    if not os.path.exists(ASSETS_DIR):
      print(ASSETS_DIR + ' not exist.')
      sys.exit()

    if os.path.exists(OUTPUT_DIR):
      shutil.rmtree(OUTPUT_DIR)

    print('ASSETS_DIR:' + ASSETS_DIR)
    print('OUTPUT_DIR:' + OUTPUT_DIR)
    print('BIN_DIR:' + BIN_DIR)
    print('EXE_NAME:' + EXE_NAME)


def joinPath(root, subdir):
    return os.path.normpath(os.path.join(root, subdir))


def copyFile(src_root_dir, src, dst_root_dir, dst):
    s = joinPath(src_root_dir, src)
    d = joinPath(dst_root_dir, dst)
    print(s + '->' + d)
    if os.path.exists(s):
        dir = os.path.dirname(d)
        if os.path.exists(dir):
            shutil.copyfile(s, d)
        else:
            os.makedirs(dir)
            shutil.copyfile(s, d)
    else:
        print('!!! copyFile src NOT EXISTS: ' + s)


def ignore_patterns_list(patterns_list):
    def _ignore_patterns(path, names):
        ignored_names = []
        for pattern in patterns_list:
            ignored_names.extend(fnmatch.filter(names, pattern))
        return set(ignored_names)
    return _ignore_patterns


def copyFiles(src_root_dir, src, dst_root_dir, dst, ignore_files=[]):
    s = joinPath(src_root_dir, src)
    d = joinPath(dst_root_dir, dst)
    print(s + '->' + d)
    if os.path.exists(s):
        shutil.rmtree(d, True)
        ignore_files.append('*.o')
        ignore_files.append('*.obj')
        ignore_files.append('*.res')
        ignore_files.append('*.xml')
        ignore_files.append('*.inc')
        shutil.copytree(s, d, ignore=ignore_patterns_list(ignore_files))
    else:
        print('!!! copyFiles src NOT EXISTS: ' + s)


def copyExe():
    copyFile(BIN_DIR, EXE_NAME, joinPath(OUTPUT_DIR, 'bin'), EXE_NAME)


def copyAssets():
    copyFiles(ASSETS_DIR, 'default/raw', OUTPUT_DIR, 'assets/default/raw/')


def release():
    copyExe()
    copyAssets()
