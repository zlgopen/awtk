import os, re
import update_res_app as updater
import update_res_common as common

def have_file(filename, files):
    filename = filename.replace('\\', '/')
    for f in files:
        if (re.search(f, filename)):
            return True
    return False

def is_demouiold_excluded_file(filename):
    excluded_files = [
        '/fonts/ap.res',
        '/fonts/default_full.res',
        '/fonts/default_32.data',
        '/fonts/default_96.data',
        '/fonts/trado.res',
        '/images/bg_landscape_[1-3]',
        '/images/bg_portrait_[1-3]',
        '/images/bg[1-5]',
        '/images/bg[1-5]_s',
        '/images/repeat[1-4]',
        '/images/app[1-5]',
        '/images/logo_dynamic',
        '/images/debug_*_*',
        '/images/uiex_*',
        '/images/computer.bsvg',
        '/images/windmill.bsvg',
        '/styles/uiex*',
        '/ui/uiex/*'
    ]
    return have_file(filename, excluded_files)

def is_not_uiex_file(filename):
    uiex_file = [
        '/fonts/default.res',
        '/strings/en_US',
        '/images/bg[1-5]',
        '/images/bg[1-5]_s',
        '/images/repeat[1-4]',
        '/images/app[1-5]',
        '/images/logo_dynamic',
        '/images/debug_*_*',
        '/images/ani[1-9]',
        '/images/gauge_pointer',
        '/images/uiex_*',
        '/images/computer.bsvg',
        '/images/windmill.bsvg',
        '/styles/default',
        '/styles/dialog_toast',
        '/styles/uiex*',
        '/ui/uiex/*'
    ]
    return not have_file(filename, uiex_file)

def is_null_file(filename):
    return False

default_is_excluded_file_func = is_not_uiex_file

def gen_res(name = 'assets', is_excluded_file_func = default_is_excluded_file_func):
    action = common.get_action()
    assets_root = common.get_assets_root()
    output_root = common.get_output_root()

    common.set_is_excluded_file_handler(is_excluded_file_func)

    subname = common.get_assets_subname()
    common.set_assets_subname('__'+ name +'_')

    asset_c = common.get_asset_c()
    common.set_asset_c(common.join_path(output_root, '../'+ name +'.inc'))

    if action == 'all' and assets_root == output_root:
        common.clean_res()
        common.gen_res_c()
        if isinstance(updater.get_theme(0), dict):
            common.gen_res_c(False)
    elif action == 'clean':
        common.clean_res()
    elif action == 'web':
        common.gen_res_web_c()
    elif action == 'json':
        common.gen_res_json()
    elif action != 'pinyin' and action != 'res':
        common.gen_res_c()
        if isinstance(updater.get_theme(0), dict):
            common.gen_res_c(False)

    common.set_asset_c(asset_c)
    common.set_assets_subname(subname)
    common.set_is_excluded_file_handler(default_is_excluded_file_func)


AWTK_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
updater.run(AWTK_ROOT, default_is_excluded_file_func)

gen_res('assets_old', is_demouiold_excluded_file)
gen_res('assets_all', is_null_file)
