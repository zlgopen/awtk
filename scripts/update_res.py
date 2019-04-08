import os
import sys 
import update_res_common as common

APP_ROOT=os.getcwd()
if APP_ROOT.endswith('scripts'):
  APP_ROOT=os.path.dirname(APP_ROOT)

os.chdir(APP_ROOT);
AWTK_ROOT=APP_ROOT
ASSETS_ROOT=common.joinPath(APP_ROOT, 'demos/assets')
ASSET_C=common.joinPath(APP_ROOT, 'demos/assets.c')

common.init(AWTK_ROOT, ASSETS_ROOT, ASSET_C);

def buildTools():
  os.system('scons '+common.toExe('bsvggen'))
  os.system('scons '+common.toExe('strgen'))
  os.system('scons '+common.toExe('resgen'))
  os.system('scons '+common.toExe('themegen'))
  os.system('scons '+common.toExe('imagegen'))
  os.system('scons '+common.toExe('fontgen'))
  os.system('scons '+common.toExe('xml_to_ui'))

def run():
  common.updateRes()

if sys.argv[1]=='all':
  buildTools()

run()

