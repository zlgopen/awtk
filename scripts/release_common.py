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
CWD = os.getcwd()
OS_NAME = platform.system();
if OS_NAME == 'Darwin':
  AWTK_DLL='libawtk.dylib'
elif OS_NAME == 'Linux':
  AWTK_DLL='libawtk.so'
elif OS_NAME == 'Windows':
  AWTK_DLL='awtk.dll'


def init(exe, assets_root, bin_root):
    global BIN_DIR
    global EXE_NAME
    global ASSETS_DIR
    global OUTPUT_DIR

    EXE_NAME = exe
    BIN_DIR = joinPath(bin_root, 'bin')
    OUTPUT_DIR = joinPath(CWD, 'release')
    ASSETS_DIR = joinPath(assets_root, 'assets')

    if not os.path.exists(BIN_DIR):
        BIN_DIR = joinPath(bin_root, 'build/bin')

    if not os.path.exists(ASSETS_DIR):
        ASSETS_DIR = joinPath(assets_root, 'demos/assets')
    if not os.path.exists(ASSETS_DIR):
        ASSETS_DIR = joinPath(assets_root, '../awtk/demos/assets')
    if not os.path.exists(ASSETS_DIR):
        print(ASSETS_DIR + ' not exist.')
        sys.exit()

    if os.path.exists(OUTPUT_DIR):
        shutil.rmtree(OUTPUT_DIR)

    print('==================================================')
    print('EXE_NAME:' + EXE_NAME)
    print('ASSETS_DIR:' + ASSETS_DIR)
    print('OUTPUT_DIR:' + OUTPUT_DIR)
    print('BIN_DIR:' + BIN_DIR)
    print('==================================================')


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
    output_bin_dir = joinPath(OUTPUT_DIR, 'bin')
    copyFile(BIN_DIR, EXE_NAME, output_bin_dir, EXE_NAME)
    copyFile(BIN_DIR, AWTK_DLL, output_bin_dir, AWTK_DLL)
  
    os.chmod(joinPath(output_bin_dir, EXE_NAME), 0o755)

def copyAssets():
    copyFiles(ASSETS_DIR, '', OUTPUT_DIR, 'assets/')	


def release():
    copyExe()
    copyAssets()
