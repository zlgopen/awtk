import os
import io
import sys
import json

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

def get_dict_value(dict, key, default_value) :
  if key in dict:
    return dict[key]
  else :
    return default_value

def set_res_config_by_script(script_path, res_config_script_argv):
    script_path = os.path.abspath(script_path)
    if os.path.exists(script_path) :
        import importlib
        script_dir = os.path.dirname(script_path)
        file_name = os.path.basename(script_path)
        module_name, ext = os.path.splitext(file_name)
        sys.path.insert(0, script_dir)
        res_config_script = importlib.import_module(module_name)
        sys.path.remove(script_dir)
        if hasattr(res_config_script, "get_res_config") :
            return res_config_script.get_res_config(res_config_script_argv)
        else :
          sys.exit(script_path + ' script not found get_res_config function')
    else :
      sys.exit('res_config_file sopt not found :' + script_path)
    return None

class res_multidict(dict) :
  def __getitem__(self, item) :
    try :
      return dict.__getitem__(self, item)
    except KeyError :
      value = self[item] = type(self)()
      return value

  def deep_copy_by_dict(self, src, dst) :
    for key in dst :
      if isinstance(dst[key], dict) :
        self.deep_copy_by_dict(src[key], dst[key])
      elif isinstance(dst[key], list) :
        if not key in src :
          src[key] = list();
        src[key] += list(dst[key]);
      else :
        src[key] = dst[key];

  def deep_copy(self, dst) :
    self.deep_copy_by_dict(self, dst);

class res_config:
  assets = None
  inc_res = True
  inc_bitmap = True
  inc_bitmap_font = True

  def __init__(self) :
    self.assets = res_multidict();

  def get_res_type_info_by_const_and_load_from(self, assets, inc_res, inc_bitmap, inc_bitmap_font) :
    const = get_dict_value(assets, 'const', None)
    loadFrom = get_dict_value(assets, 'loadFrom', None)
    if loadFrom != None and loadFrom == 'fs':
      inc_res = False
      inc_bitmap = False
      inc_bitmap_font = False
    elif const != None and const != 'all_data':
      if const == 'resource_data':
        inc_bitmap = False
        inc_bitmap_font = False
      else:
        inc_res = False
    
    return inc_res, inc_bitmap, inc_bitmap_font

  def has_themes(self) :
    if 'themes' in self.assets :
      return len(self.assets['themes']) > 0;
    return False

  def has_theme(self, theme_name) :
    if 'themes' in self.assets :
      return theme_name in self.assets['themes'];
    return False

  def has_lcd(self, theme_name) :
    if self.has_theme(theme_name) :
      return 'lcd' in self.assets['themes'][theme_name];
    return False
  
  def has_font(self, theme_name, font_name) :
    if self.has_theme(theme_name) :
      if 'fonts' in self.assets['themes'][theme_name] :
        return font_name in self.assets['themes'][theme_name]['fonts']
    return False
  



  def load_file(self, file_path) :
    obj = json_obj_load_file(file_path);
    if obj != None and 'assets' in obj :
      assets = obj['assets']
      self.assets.deep_copy(assets)
      self.inc_res, self.inc_bitmap, self.inc_bitmap_font = self.get_res_type_info_by_const_and_load_from(assets, self.inc_res, self.inc_bitmap, self.inc_bitmap_font)
    else :
      print('not found or not parsered ' + file_path + ', so use default config')
  def save_file(self, file_path) :
    res_config_dict = {
      'assets' : self.assets
    }
    json_obj_save_file(res_config_dict, file_path);

  def reset_default(self) :
    self.set_res_actived_theme()
    self.set_res_res_root()
    self.set_res_load_from()
    self.set_res_const()
    self.set_res_dpi()
    self.set_res_language()
    self.set_res_country()
    self.set_res_lcd_orientation()
    self.set_res_lcd_fast_rotation_mode()

    self.set_res_actived_system_bar()
    self.set_res_actived_bottom_system_bar()
    self.set_res_packaged()
    self.set_res_font_value()
    self.set_res_font_value(key='text')
    self.set_res_font_bpp()

    self.set_res_w()
    self.set_res_h()
    self.set_res_color_depth()
    self.set_res_color_format()


  def get_res_themes_key(self) :
    if self.has_themes() :
      return self.assets['themes'].keys()
    return list()
  
  def get_res_fonts_key(self, theme_name) :
    if self.has_themes() :
      return self.assets[theme_name]['fonts'].keys()
    return list()

  def get_res_font_size_key(self, theme_name, font_name) :
    key = []
    if self.has_themes() :
      for name in self.assets[theme_name]['fonts'][font_name].keys() : 
        if name != 'text' or name != 'bpp' :
          key.append(name)
    return key

  def set_res_lcd_value(self, theme_name, key, value) :
    self.assets['themes'][theme_name]['lcd'][key] = value

  def get_res_lcd_value(self, theme_name, key, default_value) :
    if self.has_lcd(theme_name) :
      return get_dict_value(self.assets['themes'][theme_name]['lcd'], key, default_value)
    return default_value;

  def set_res_theme_value(self, theme_name, key, value) :
    self.assets['themes'][theme_name][key] = value

  def get_res_theme_value(self, theme_name, key, default_value) :
    if self.has_theme(theme_name) :
      return get_dict_value(self.assets['themes'][theme_name], key, default_value)
    return default_value;

  def set_res_font_bpp(self, theme_name =  'default', font_name = 'default', value = '8bits') :
    self.assets['themes'][theme_name]['fonts'][font_name]['bpp'] = value

  def get_res_font_bpp(self, theme_name, font_name, default_value) :
    if self.has_font(theme_name, font_name) :
      return get_dict_value(self.assets['themes'][theme_name]['fonts'][font_name], 'bpp', default_value)
    return default_value;

  def set_res_font_value(self, theme_name =  'default', font_name = 'default', key = '18', value = '') :
    self.assets['themes'][theme_name]['fonts'][font_name][key] = value

  def get_res_font_value(self, theme_name, font_name, key, default_value) :
    if self.has_font(theme_name, font_name) :
      return get_dict_value(self.assets['themes'][theme_name]['fonts'][font_name], key, default_value)
    return default_value;

  def set_res_packaged(self, packaged = True, theme_name = 'default') :
    return self.set_res_theme_value(theme_name, 'packaged', packaged);

  def get_res_packaged(self, theme_name = 'default') :
    return self.get_res_theme_value(theme_name, 'packaged', True);

  def set_res_actived_bottom_system_bar(self, bottom_system_bar='', theme_name = 'default') :
    return self.set_res_theme_value(theme_name, 'activedBottomSystemBar', bottom_system_bar);

  def get_res_actived_bottom_system_bar(self, theme_name = 'default') :
    return self.get_res_theme_value(theme_name, 'activedBottomSystemBar', '');

  def set_res_actived_system_bar(self, system_bar = '', theme_name = 'default') :
    return self.set_res_theme_value(theme_name, 'activedSystemBar', system_bar);

  def get_res_actived_system_bar(self, theme_name = 'default') :
    return self.get_res_theme_value(theme_name, 'activedSystemBar', '');

  def set_res_color_depth(self, color_depth ='rgba', theme_name = 'default') :
    return self.set_res_lcd_value(theme_name, 'colorDepth', color_depth);

  def get_res_color_depth(self, theme_name = 'default') :
    return self.get_res_lcd_value(theme_name, 'colorDepth', 'rgba');

  def set_res_color_format(self, color_format ='rgba', theme_name = 'default') :
    return self.set_res_lcd_value(theme_name, 'colorFormat', color_format);

  def get_res_color_format(self, theme_name = 'default') :
    return self.get_res_lcd_value(theme_name, 'colorFormat', 'rgba');

  def set_res_w(self, w = '800', theme_name = 'default'):
    self.set_res_lcd_value(theme_name, 'width', w)

  def get_res_w(self, theme_name):
    return self.get_res_lcd_value(theme_name, 'width', '800');

  def set_res_h(self, h = '480', theme_name = 'default'):
    self.set_res_lcd_value(theme_name, 'height', h)

  def get_res_h(self, theme_name):
    return self.get_res_lcd_value(theme_name, 'height', '480');

  def set_res_lcd_fast_rotation_mode(self, lcd_fast_rotation_mode = False):
    self.assets['lcdFastRotationMode'] = lcd_fast_rotation_mode

  def get_res_lcd_fast_rotation_mode(self):
    return get_dict_value(self.assets, 'lcdFastRotationMode', False);

  def set_res_lcd_orientation(self, orientation = '0', theme_name = None):
    if theme_name == None :
      self.assets['lcdOrientation'] = orientation
    else :
      self.assets['themes'][theme_name]['lcd']['orientation'] = orientation

  def get_res_lcd_orientation(self, theme_name = ''):
    orientation = '0'
    if theme_name == '' and 'lcdOrientation' in self.assets :
        orientation = self.assets['lcdOrientation']
    elif theme_name != '' and self.has_lcd(theme_name) and 'orientation' in self.assets['themes'][theme_name]['lcd'] :
        orientation = self.assets['themes'][theme_name]['lcd']['orientation']
    return orientation

  def set_res_actived_theme(self, actived_theme = 'default'):
    self.assets['activedTheme'] = actived_theme

  def get_res_actived_theme(self):
    return get_dict_value(self.assets, 'activedTheme', 'default')

  def set_res_dpi(self, dpi = 'x1'):
    self.assets['screenDPR'] = dpi

  def get_res_dpi(self):
    return get_dict_value(self.assets, 'screenDPR', 'x1')

  def set_res_language(self, language = 'zh'):
    self.assets['defaultLanguage'] = language

  def get_res_language(self):
    return get_dict_value(self.assets, 'defaultLanguage', 'zh')

  def set_res_country(self, country = 'CN'):
    self.assets['defaultCountry'] = country

  def get_res_country(self):
    return get_dict_value(self.assets, 'defaultCountry', 'CN')

  def set_res_res_root(self, res_root = 'res'):
    self.assets['outputDir'] = res_root

  def get_res_res_root(self):
    res_root = get_dict_value(self.assets, 'outputDir', 'res')
    if os.path.isabs(res_root):
        return res_root
    else: 
        return './' + res_root

  def set_res_load_from(self, load_from = 'any'):
    self.assets['loadFrom'] = load_from
    self.inc_res, self.inc_bitmap, self.inc_bitmap_font = self.get_res_type_info_by_const_and_load_from(self.assets, self.inc_res, self.inc_bitmap, self.inc_bitmap_font)
  
  def get_res_load_from(self):
    return get_dict_value(self.assets, 'loadFrom', 'any')

  def set_res_const(self, const = 'all_data'):
    self.assets['const'] = const
    self.inc_res, self.inc_bitmap, self.inc_bitmap_font = self.get_res_type_info_by_const_and_load_from(self.assets, self.inc_res, self.inc_bitmap, self.inc_bitmap_font)
  
  def get_res_load_from(self):
    return get_dict_value(self.assets, 'const', 'all_data')

  def get_inc_res(self) :
    return self.inc_res
  
  def get_inc_bitmap(self) :
    return self.inc_bitmap
  
  def get_inc_bitmap_font(self) :
    return self.inc_bitmap_font
