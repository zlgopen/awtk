#!/usr/bin/python

import os
import sys

# try add AWTK_ROOT/scripts to system environment
AWTK_ROOT = '../awtk'
if os.path.exists(AWTK_ROOT):
    sys.path.append(AWTK_ROOT + '/scripts')
elif not os.path.isabs(AWTK_ROOT):
    AWTK_ROOT = '../' + AWTK_ROOT
    if os.path.exists(AWTK_ROOT):
        sys.path.append(AWTK_ROOT + '/scripts')

# AWTK_ROOT/scripts/update_res_common.py
import update_res_common as common


APP_ROOT = common.getcwd()
if APP_ROOT.endswith('scripts'):
    APP_ROOT = os.path.dirname(APP_ROOT)

os.chdir(APP_ROOT)

AWTK_ROOT = common.join_path(APP_ROOT, AWTK_ROOT)
ASSETS_ROOT = common.join_path(APP_ROOT, 'assets')
ASSET_C = common.join_path(APP_ROOT, 'src/assets.c')
THEMES = ['default']

common.init(AWTK_ROOT, ASSETS_ROOT, THEMES, ASSET_C)
common.update_res()
