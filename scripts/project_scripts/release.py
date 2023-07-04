import os
import sys
import platform
import shutil
import json
import collections


def join_path(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))


OS_NAME = platform.system()
PRJ_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUTPUT_DIR = join_path(PRJ_DIR, 'release')
BIN_DIR = join_path(PRJ_DIR, 'bin')


def read_file(filename):
  content = ''
  if sys.version_info >= (3, 0):
    with open(filename, 'r', encoding='utf8') as f:
      content = f.read()
  else:
    with open(filename, 'r') as f:
      content = f.read()
  return content


def to_file_system_coding(s):
    if sys.version_info >= (3, 0): return s
    coding = sys.getfilesystemencoding()
    return s.encode(coding)


def init_project_config():
  global CONFIG
  json_path = join_path(PRJ_DIR, 'project.json')
  if not os.path.exists(json_path):
    return

  content = read_file(json_path)
  CONFIG = json.loads(content, object_pairs_hook=collections.OrderedDict)


def get_args(args, longsopts = []) :
    list_opts = []
    for arg in args:
        if arg.startswith('--') :
            tmp_opt = ''
            for opt in longsopts:
                if arg.find(opt) > 0 :
                    tmp_opt = opt
                    break
            if tmp_opt != '' :
                list_opts.append(arg.split(tmp_opt)[1])
                continue
            else :
                print(arg + " not find command, command :")
                print(longsopts)
                sys.exit()
    return list_opts


def release():
  if not os.path.exists(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

  init_project_config()

  assets = CONFIG['assets']
  if 'outputDir' in assets:
    res_root = to_file_system_coding(assets['outputDir'])
    assets_root = join_path(PRJ_DIR, res_root + '/assets')

  exeName = CONFIG['appExeName']
  if exeName == '':
    exeName = 'demo'
  exeName = exeName + '.exe' if OS_NAME == 'Windows' else exeName

  copyExe(exeName)
  copyAssets(assets_root)
  cleanFiles()


def copyExe(exeName):
  output_bin_dir = join_path(OUTPUT_DIR, 'bin')
  common.copyFile(BIN_DIR, exeName, output_bin_dir, exeName)
  common.copySharedLib(BIN_DIR, output_bin_dir)

  os.chmod(join_path(output_bin_dir, exeName), 0o755)


def copyAssets(assets_root):
  common.copyFiles(assets_root, '', OUTPUT_DIR, 'assets/')


def cleanFiles():
  assets = CONFIG['assets']
  if 'themes' not in assets:
    return

  themes = assets['themes']

  for theme in themes:
    d = join_path(OUTPUT_DIR, 'assets/' + theme + '/inc')
    shutil.rmtree(d, True)


import release_common as common
release()
