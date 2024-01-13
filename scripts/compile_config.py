import os
import io
import sys
import json
import shutil
import importlib
import collections

global COMPILE_CONFIG
COMPILE_CONFIG = None

global APP_ROOT
APP_ROOT = ''

global WIN32_RES
WIN32_RES = ''

def get_curr_config() :
  global COMPILE_CONFIG
  return COMPILE_CONFIG

def set_curr_config(complie_config) :
  global COMPILE_CONFIG
  COMPILE_CONFIG = complie_config

def set_app_win32_res(dir_path) :
  global WIN32_RES
  WIN32_RES = dir_path

def set_curr_app_root(app_root) :
  global APP_ROOT
  APP_ROOT = app_root

def get_curr_app_root() :
  global APP_ROOT
  return APP_ROOT

def get_curr_config_for_awtk() :
  global WIN32_RES
  global COMPILE_CONFIG
  if COMPILE_CONFIG != None :
    return COMPILE_CONFIG
  else :
    COMPILE_CONFIG = complie_helper()
    if not COMPILE_CONFIG.load_last_complie_argv() :
      print('========================= Error ================================')
      print('Please Recompile AWTK !!!!!')
      sys.exit('Not found last complie argv config file !!!!!')
    COMPILE_CONFIG.set_value('WIN32_RES', WIN32_RES)
    return COMPILE_CONFIG;

def json_obj_load_file(file_path) :
  obj = None;
  if os.path.exists(file_path) :
    try :
      with io.open(file_path, 'r', encoding='utf-8') as file :
        obj = json.load(file);
    except Exception as e :
        print(e)
  return obj;

def json_obj_save_file(obj, file_path) :
  dir = os.path.dirname(file_path);
  if os.path.exists(dir) :
    try :
      with io.open(file_path, 'w', encoding='utf-8') as file :
        if sys.version_info >= (3, 0):
          json.dump(obj, file, indent=4, ensure_ascii=False)
        else :
          file.write(json.dumps(obj, indent=4, ensure_ascii=False).decode('utf-8'))
    except Exception as e :
        print(e)
  else :
    print(dir + ' is not exists')

class complie_helper :
  DEFAULT_CONFIG_FILE = './awtk_config_define.py'
  LAST_COMLIP_ARGV_FILE = './bin/last_complie_argv.json'

  COMPILE_CMD_INFO = collections.OrderedDict({
    'help' : { 'name': 'HELP', 'help_info' : 'show all usage'},
    'userdefine' : { 'name' : 'DEFINE_FILE', 'help_info' : 'set user config define file, DEFINE_FILE=XXXXX'},
    'save_file' : { 'name' : 'EXPORT_DEFINE_FILE', 'help_info' : 'current config define export to file, EXPORT_DEFINE_FILE=./awtk_config_define.py'},
  })

  config = collections.OrderedDict({
    'OUTPUT_DIR' : { 'value' : None, 'type' : str.__name__, 'desc' : ['compiled export directory '], 'help_info' : 'set awtk compiled export directory, default value is \'\', \'\' is system\'s value'},
    'TOOLS_NAME' : { 'value' : None, 'type' : str.__name__, 'str_enum' : ['mingw'], 'desc' : ['value is \'mingw\' or None'], 'help_info' : 'set awtk compile\'s name, default value is None, None is system\'s value'},
    'INPUT_ENGINE' : { 'value' : None, 'type' : str.__name__, 'str_enum' : ['null', 'spinyin', 't9', 't9ext', 'pinyin'], 'desc' : ['value is null/spinyin/t9/t9ext/pinyin'], 'help_info' : 'set awtk use input engine, default value is None, None is system\'s value' },
    'VGCANVAS' : { 'value' : None, 'type' : str.__name__, 'str_enum' : ['NANOVG', 'NANOVG_PLUS', 'CAIRO'], 'desc' : ['value is NANOVG/NANOVG_PLUS/CAIRO'], 'help_info' : 'set awtk use render vgcanvas type, default value is None, None is system\'s value' },
    'NANOVG_BACKEND' : { 'value' : None, 'type' : str.__name__, 'str_enum' : ['GLES2', 'GLES3', 'GL3', 'AGG', 'AGGE'], 'desc' : ['if NANOVG_BACKEND is valid, VGCANVAS must be NANOVG or \'\'', 'if VGCANVAS is NANOVG_PLUS, NANOVG_BACKEND must be GLES2/GLES3/GL3 or None', 'NANOVG_BACKEND is GLES2/GLES3/GL3/AGG/AGGE'], 'help_info' : 'set awtk\'s nanovg use render model, default value is None, None is system\'s value'},
    'LCD_COLOR_FORMAT' : { 'value' : None, 'type' : str.__name__, 'str_enum' : ['bgr565', 'bgra8888', 'mono'], 'desc' : ['if NANOVG_BACKEND is GLES2/GLES3/GL3, LCD_COLOR_FORMAT must be bgra8888 or \'\'', 'if NANOVG_BACKEND is AGG/AGGE, LCD_COLOR_FORMAT must be bgr565/bgra8888/mono or None', 'NANOVG_BACKEND is bgr565/bgra8888/mono'], 'help_info' : 'set awtk\'s lcd color format, default value is None, None is system\'s value'},
    'DEBUG' : { 'value' : True, 'type' : bool.__name__, 'desc' : ['awtk\'s compile is debug'], 'help_info' : 'awtk\'s compile is debug, value is true or false, default value is true' },
    'PDB' : { 'value' : True, 'type' : bool.__name__, 'desc' : ['export pdb file'], 'help_info' : 'export pdb file, value is true or false' },
    'SDL_UBUNTU_USE_IME' : { 'value' : False, 'type' : bool.__name__, 'desc' : ['ubuntu use chinese input engine'], 'help_info' : 'ubuntu use ime, this sopt is ubuntu use chinese input engine, value is true or false, default value is false' },
    'NATIVE_WINDOW_BORDERLESS' : { 'value' : False, 'type' : bool.__name__, 'desc' : ['pc desktop program no borerless'], 'help_info' : 'pc desktop program no borerless, value is true or false, default value is false' },
    'NATIVE_WINDOW_NOT_RESIZABLE' : { 'value' : False, 'type' : bool.__name__, 'desc' : ['pc desktop program do not resize program'], 'help_info' : 'pc desktop program do not resize program\'s size, value is true or false, default value is false' },
    'BUILD_TESTS' : { 'value' : True, 'type' : bool.__name__, 'desc' : ['build awtk\'s gtest demo'], 'help_info' : 'build awtk\'s gtest demo, value is true or false, default value is true' },
    'BUILD_DEMOS' : { 'value' : True, 'type' : bool.__name__, 'desc' : ['build awtk\'s demo examples'], 'help_info' : 'build awtk\'s demo examples, value is true or false, default value is true' },
    'BUILD_TOOLS' : { 'value' : True, 'type' : bool.__name__, 'desc' : ['build awtk\'s tools'], 'help_info' : 'build awtk\'s tools, value is true or false, default value is true' },
    'WIN32_RES' : { 'value' : None, 'type' : str.__name__, 'save_file' : False, 'desc' : ['app\'s win32 res path'], 'help_info' : 'app\'s win32 res path, WIN32_RES=XXXXX, value\'s default=\'awtk/win32_res/awtk.res\' ' },
  })

  def try_load_default_config(self) :
    if os.path.exists(self.DEFAULT_CONFIG_FILE) :
      self.load_config(self.DEFAULT_CONFIG_FILE)

  def set_compile_config(self, config) :
    self.config = config

  def load_config_by_sopt(self, ARGUMENTS) :
    for key in ARGUMENTS :
      if key.upper() == self.COMPILE_CMD_INFO['userdefine']['name'] :
        file = ARGUMENTS[key]
        if os.path.exists(file) :
          self.load_config(file)
          break;
        else :
          sys.exit('userdefine sopt is not found :' + file)

  def check_config_legality(self) :
    for key in self.config :
      check = False
      if not 'str_enum' in self.config[key] :
        continue
      if self.config[key]['value'] != '' and self.config[key]['value'] != None:
        for str_enum in self.config[key]['str_enum'] :
          if str_enum == self.config[key]['value'] :
            check = True
            break
        if not check :
          if self.config[key]['value'] == None:
            sys.exit(key + ' \'s value is None, is not legality !')
          else :
            sys.exit(key + ' \'s value is ' + self.config[key]['value'] + ', is not legality !')

  def scons_user_sopt(self, ARGUMENTS) :
    EXPORT_USERDEFINE_FILE = None
    def find_compile_cmd_info_by_name(name) :
      for key in  self.COMPILE_CMD_INFO :
        if name ==  self.COMPILE_CMD_INFO[key]['name']:
          return True
      return False

    for arg in sys.argv :
      key = arg.upper()
      if key == self.COMPILE_CMD_INFO['help']['name'] :
        self.show_usage()
        sys.exit()

    self.load_config_by_sopt(ARGUMENTS)

    for key in ARGUMENTS :
      userdefine_key = key.upper()
      if self.has_key(userdefine_key) :
        value = ARGUMENTS[key]
        self.set_value(userdefine_key, value)
      elif not find_compile_cmd_info_by_name(userdefine_key):
        print('awtk\'s default compiler list does not support {0} , so skip {0} !!!!!!'.format(key))
      elif key.upper() == self.COMPILE_CMD_INFO['save_file']['name'] :
        EXPORT_USERDEFINE_FILE = ARGUMENTS[key]

    if EXPORT_USERDEFINE_FILE != None :
      self.save_config(EXPORT_USERDEFINE_FILE)
      sys.exit()

    self.check_config_legality()


  def show_usage(self) :
    print('=========================================================')
    for key in self.config :
      print((key + ' : ').ljust(32) + self.config[key]['help_info'])
      for desc in self.config[key]['desc'] :
        print(''.ljust(32) + desc)

    for key in self.COMPILE_CMD_INFO :
      print((self.COMPILE_CMD_INFO[key]['name'] + ' : ').ljust(32) + self.COMPILE_CMD_INFO[key]['help_info'])
    print('=========================================================')

  def write_file(self, filename, s):
    if sys.version_info >= (3, 0):
        with open(filename, 'w', encoding='utf8') as f:
            f.write(s)
    else:
        with open(filename, 'w') as f:
            s = s.encode('utf8')
            f.write(s)

  def save_config(self, file) :
    dir = os.path.dirname(file);
    if os.path.exists(dir) :
      save_data = ''
      save_data += '# user set default configuration item \n'
      save_data += '# if value is None, so value is default value \n'
      save_data += '\n'
      for key in self.config :
        if 'save_file' in self.config[key] and not self.config[key]['save_file'] :
          continue
        for desc in self.config[key]['desc'] :
          save_data += ('# ' + desc)
          save_data += '\n'
        if isinstance(self.config[key]['value'], str) :
          save_data += (key + ' = r\'' + self.config[key]['value'] + '\'')
        else :
          save_data += (key + ' = ' + str(self.config[key]['value']))
        save_data += '\n\n'
      
      try :
        self.write_file(file, save_data)
      except Exception as e :
          print(e)
    else :
      print(dir + ' is not exists')

  def load_config(self, file) :
    file = os.path.abspath(file)
    if os.path.exists(file) :
      script_dir = os.path.dirname(file)
      file_name = os.path.basename(file)
      module_name, ext = os.path.splitext(file_name)
      sys.path.insert(0, script_dir)
      config_script = importlib.import_module(module_name)
      sys.path.remove(script_dir)
      for key in self.config :
        if hasattr(config_script, key) :
          if not 'save_file' in self.config[key] or self.config[key]['save_file'] :
            self.config[key]['value'] = getattr(config_script, key)
    else :
      sys.exit(file + ' is not found')

  def has_key(self, name) :
    return name in self.config

  def get_value(self, name, default_value = None) :
    return self.get_unique_value(name, default_value)

  def get_unique_value(self, name, default_value) :
    if name in self.config and self.config[name]['value'] != None :
      return self.config[name]['value']
    return default_value

  def set_value(self, name, value) :
    if name in self.config :
      if isinstance(self.config[name]['value'], bool) and isinstance(value, bool):
        self.config[name]['value'] = value
      elif (isinstance(self.config[name]['value'], bool) and isinstance(value, str)) or ('type' in self.config[name] and self.config[name]['type'] == bool.__name__ and isinstance(value, str)):
        try :
          from utils import strtobool
          self.config[name]['value'] = strtobool(value) == 1
        except :
          self.config[name]['value'] = value.lower() == 'true'
      elif isinstance(self.config[name]['value'], list) and isinstance(value, str) :
        self.config[name]['value'] = value.split(',')
      elif self.config[name]['value'] == None and (not 'type' in self.config[name]):
        self.config[name]['value'] = value
      else:
        self.config[name]['value'] = value

  def save_last_complie_argv(self) :
    json_obj_save_file(self.config, self.LAST_COMLIP_ARGV_FILE);
  
  def load_last_complie_argv(self) :
    if os.path.exists(self.LAST_COMLIP_ARGV_FILE) :
      obj = json_obj_load_file(self.LAST_COMLIP_ARGV_FILE);
      if obj != None :
        self.config = collections.OrderedDict(obj);
        return True
    return False

  def output_compile_data(self, awtk_root) :
    output_dir = self.config['OUTPUT_DIR']['value']
    if output_dir != '' and output_dir != None :
      if os.path.exists(output_dir) :
        shutil.rmtree(output_dir)
      os.makedirs(output_dir)
      shutil.copytree(awtk_root + '/bin', output_dir + '/bin')
      shutil.copytree(awtk_root + '/lib', output_dir + '/lib')

