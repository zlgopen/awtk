import os
import sys
import update_res_common as common

APP_ROOT = common.getcwd()
if APP_ROOT.endswith('scripts'):
    APP_ROOT = os.path.dirname(APP_ROOT)

os.chdir(APP_ROOT)

AWTK_ROOT = APP_ROOT
ASSET_C = common.join_path(APP_ROOT, 'demos/assets.c')
ASSETS_ROOT = common.join_path(APP_ROOT, 'demos/assets')
THEMES = ['default', 'dark']

def is_excluded_file(filename):
    excluded_files = [
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

    for f in excluded_files:
        if (filename.find(f) >= 0):
            return True;

    return False;


def buildTools():
    common.init(AWTK_ROOT, ASSETS_ROOT, 'default', ASSET_C)
    os.system('scons '+common.to_exe('bsvggen'))
    os.system('scons '+common.to_exe('strgen'))
    os.system('scons '+common.to_exe('resgen'))
    os.system('scons '+common.to_exe('themegen'))
    os.system('scons '+common.to_exe('imagegen'))
    os.system('scons '+common.to_exe('fontgen'))
    os.system('scons '+common.to_exe('xml_to_ui'))


def run():
    common.init(AWTK_ROOT, ASSETS_ROOT, THEMES, ASSET_C)
    common.set_is_excluded_file_handler(is_excluded_file)
    common.update_res()


if sys.argv[1] == 'all':
    buildTools()

run()
