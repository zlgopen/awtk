import os
import update_res_app as updater

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
        '/fonts/default_96.data',
        '/fonts/trado.res'
    ]

    filename = filename.replace('\\', '/')
    for f in excluded_files:
        if (filename.find(f) >= 0):
            return True
    return False

AWTK_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
updater.run(AWTK_ROOT, is_excluded_file)
