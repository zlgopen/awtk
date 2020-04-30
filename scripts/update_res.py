import os
import sys
import update_res_common as common

APP_ROOT = os.getcwd()
if APP_ROOT.endswith('scripts'):
    APP_ROOT = os.path.dirname(APP_ROOT)

os.chdir(APP_ROOT)

AWTK_ROOT = APP_ROOT
themes = ['default', 'dark']
ASSET_C = common.joinPath(APP_ROOT, 'demos/assets.c')
ASSETS_ROOT = common.joinPath(APP_ROOT, 'demos/assets')

def isFileExcluded(filename):
    exclude_files = [
        '/images/bg_landscape_1',
        '/images/bg_landscape_2',
        '/images/bg_landscape_3',
        '/images/bg_landscape_21',
        '/images/bg_portrait_1',
        '/images/bg_portrait_2',
        '/images/bg_portrait_3',
        '/fonts/ap.res',
        '/fonts/default_full.res',
        '/fonts/default_32.data',
        '/fonts/default_96.data'
    ]
    filename = filename.replace('\\', '/')

    for f in exclude_files:
        if (filename.find(f) >= 0):
            return True;

    return False;


def buildTools():
    common.init(AWTK_ROOT, ASSETS_ROOT, 'default', ASSET_C)
    os.system('scons '+common.toExe('bsvggen'))
    os.system('scons '+common.toExe('strgen'))
    os.system('scons '+common.toExe('resgen'))
    os.system('scons '+common.toExe('themegen'))
    os.system('scons '+common.toExe('imagegen'))
    os.system('scons '+common.toExe('fontgen'))
    os.system('scons '+common.toExe('xml_to_ui'))

def run():
    for theme in themes:
        print('========================='+theme+' begin =========================')
        common.init(AWTK_ROOT, ASSETS_ROOT, theme, ASSET_C, isFileExcluded)
        common.updateRes()
        print('========================='+theme+' end =========================\n')
    common.genAssetC(themes, ASSET_C)


if sys.argv[1] == 'all':
    buildTools()

run()
