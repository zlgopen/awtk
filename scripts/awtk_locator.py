import os
import sys


def getAwtkConfig(LINUX_FB):
    AWTK_ROOT = getAwtkOrAwtkLinuxFbRoot(LINUX_FB)
    sys.path.insert(0, AWTK_ROOT)
    import awtk_config as awtk

    return awtk

def getTkcOnly():
    env = os.environ
    if 'TKC_ONLY' in env:
        return env['TKC_ONLY'] == 'True'
    else:
        return False

def getAwtkOrAwtkLinuxFbRoot(is_linux_fb):
    if getTkcOnly():
        print('TKC_ONLY == True');
        return locateAWTK('tkc')
    elif is_linux_fb:
        return locateAWTK('awtk-linux-fb')
    else:
        return locateAWTK('awtk')


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
