import os
import sys


def getAwtkConfig(LINUX_FB):
    AWTK_ROOT = getAwtkOrAwtkLinuxFbRoot(LINUX_FB)
    sys.path.insert(0, AWTK_ROOT)
    import awtk_config as awtk

    return awtk


def getAwtkOrAwtkLinuxFbRoot(LINUX_FB):
    if LINUX_FB:
        AWTK_ROOT = getAwtkLinuxFbRoot()
    else:
        AWTK_ROOT = getAwtkRoot()

    return AWTK_ROOT


def getAwtkScriptsRoot():
    return os.path.join(locateAWTK('awtk'), 'scripts')


def getAwtkRoot():
    return locateAWTK('awtk')


def getAwtkLinuxFbRoot():
    return locateAWTK('awtk-linux-fb')


def locateAWTK(awtk):
    awtk_root = '../' + awtk
    if not os.path.exists(awtk_root):
        dirnames = ['../'+awtk, '../../'+awtk, '../../../'+awtk]
        for dirname in dirnames:
            if os.path.exists(dirname):
                awtk_root = dirname
                break
    return os.path.abspath(awtk_root)


AWTK_ROOT = getAwtkRoot()
AWTK_SCRIPTS_ROOT = getAwtkScriptsRoot()
sys.path.insert(0, AWTK_SCRIPTS_ROOT)

print('AWTK_ROOT:' + AWTK_ROOT)
print('AWTK_SCRIPTS_ROOT:' + AWTK_SCRIPTS_ROOT)
