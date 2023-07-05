#!/usr/bin/python

import os
import sys
import json
import collections

# AWTK_ROOT/scripts/update_res_common.py
import update_res_common as common
import res_config

RES_CONFIG = None
RES_OUTPUT_DIR='./res'

def get_theme(i):
    return THEMES[i]

def use_theme_config_from_res_config(res_config_path, config = None):
    global DPI
    global THEMES
    global OUTPUT_ROOT
    global IS_GENERATE_INC_RES
    global IS_GENERATE_INC_BITMAP
    global APP_THEME
    global APP_ROOT
    global LCD_ORIENTATION
    global LCD_FAST_ROTATION_MODE
    content = None

    if config == None :
        config_json = common.join_path(APP_ROOT, res_config_path)
        if not os.path.exists(config_json) or res_config_path == '':
            config_json = common.join_path(APP_ROOT, 'project.json')
            if not os.path.exists(config_json) :
                print(config_json + ' is not exists.')
                return
        
        content = res_config.res_config()
        content.load_file(config_json)
    else :
        content = config;

    OUTPUT_ROOT = common.join_path(APP_ROOT, common.join_path(content.get_res_res_root(), 'assets'))
    APP_THEME = content.get_res_actived_theme()
    LCD_FAST_ROTATION_MODE = content.get_res_lcd_fast_rotation_mode()
    LCD_ORIENTATION = content.get_res_lcd_orientation()

    DPI = content.get_res_dpi()
    IS_GENERATE_INC_RES = content.get_inc_res()
    IS_GENERATE_INC_BITMAP = content.get_inc_bitmap()

    if not content.has_themes():
        return

    for theme_name in content.get_res_themes_key() :
        color_format = content.get_res_color_format(theme_name)
        color_depth = content.get_res_color_depth(theme_name)
        if color_format == 'MONO':
          imagegen_options = 'mono'
        elif color_format == 'BGR(A)':
            if color_depth == '16bit':
                imagegen_options = 'bgr565'
            else:
                imagegen_options = 'bgra'
        else:
            if color_depth == '16bit':
                imagegen_options = 'rgb565'
            else:
                imagegen_options = 'rgba'

        if IS_GENERATE_INC_BITMAP:
            config_dir = common.join_path(ASSETS_ROOT, common.join_path(theme_name, 'fonts/config'))
            common.remove_dir(config_dir)
            common.make_dirs(config_dir)
            for font_name in content.get_res_fonts_key(theme_name):
                for font_size in content.get_res_font_size_key(theme_name, font_name):
                    font_name = common.to_file_system_coding(font_name)
                    font_size = common.to_file_system_coding(font_size)
                    filename = common.join_path(config_dir, font_name+'_'+font_size+'.txt')
                    common.write_file(filename, content.get_res_font_value(theme_name, font_name, font_size))

        theme = {'name': theme_name, 'imagegen_options': imagegen_options, 'packaged': content.get_res_packaged(theme_name)}
        if theme_name == 'default':
            THEMES.insert(0, theme)
        else:
            THEMES.append(theme)


def use_default_theme_config(res_config_path, res_config = None):
    global THEMES

    use_theme_config_from_res_config(res_config_path, res_config)

    if len(THEMES) == 0:
        if os.path.isdir(ASSETS_ROOT):
            for file in os.listdir(ASSETS_ROOT):
                if os.path.isdir(common.join_path(ASSETS_ROOT, file)):
                    if file == 'default':
                        THEMES.insert(0, file)
                    else:
                        THEMES.append(file)


def is_dependencies_ok():
    dependencies = ['bsvggen', 'fontgen', 'imagegen', 'resgen', 'strgen', 'themegen', 'xml_to_ui']
    for d in dependencies:
      if not os.path.exists(common.join_path(TOOLS_ROOT, common.to_exe(d))):
          print('Can\'t find ' + common.to_exe(d) + ', please build AWTK.')
          return False

    if IS_GENERATE_INC_BITMAP:
        for t in THEMES:
            if isinstance(t, dict):
                print(t['imagegen_options'])

    return True


def on_generate_res_event():
    if os.path.exists(common.join_path(APP_ROOT, 'scripts/update_res_generate_res_handler.py')):
        import update_res_generate_res_handler as generate_res
        if hasattr(generate_res, 'on_generate_res_before'):
           common.on_generate_res_before(generate_res.on_generate_res_before)
        if hasattr(generate_res, 'on_generate_res_after'):
           common.on_generate_res_after(generate_res.on_generate_res_after)

def getopt(args):
    return common.get_args(args);

def set_res_config(config) :
    global RES_CONFIG
    RES_CONFIG = config

def set_res_config_by_script(script_path, res_config_script_argv):
    return set_res_config(res_config.set_res_config_by_script(script_path, res_config_script_argv))

def get_dict_value(dict, key, default_value) :
    if key in dict :
        return dict[key]
    return default_value

def run(awtk_root, is_excluded_file_handler = None, is_new_usage = False) :
    global DPI
    global AWTK_ROOT
    global TOOLS_ROOT
    global THEMES
    global APP_THEME
    global APP_ROOT
    global ASSETS_ROOT
    global OUTPUT_ROOT
    global IS_GENERATE_INC_RES
    global IS_GENERATE_INC_BITMAP
    global LCD_ORIENTATION
    global LCD_FAST_ROTATION_MODE
    global RES_CONFIG

    common.show_usage(is_new_usage);

    GDPI=''
    TMP_APP_ROOT = ''
    IMAGEGEN_OPTIONS=''
    TMP_LCD_ORIENTATION=''
    RES_CONFIG_JSON_PATH=''
    RES_OUTPUT_DIR='./res'
    TMP_LCD_FAST_ROTATION_MODE=None
    args = sys.argv[1:];
    if common.is_all_sopts_args(args) :
        longsots_dict = common.get_longsopts_args(args)
        common.set_action(longsots_dict['action'])

        awtk_root = get_dict_value(longsots_dict, 'awtk_root', awtk_root)
        GDPI = get_dict_value(longsots_dict, 'dpi', GDPI)
        IMAGEGEN_OPTIONS = get_dict_value(longsots_dict, 'image_options', IMAGEGEN_OPTIONS)
        TMP_LCD_ORIENTATION = get_dict_value(longsots_dict, 'lcd_orientation', TMP_LCD_ORIENTATION)
        TMP_LCD_FAST_ROTATION_MODE = get_dict_value(longsots_dict, 'lcd_enable_fast_rotation', TMP_LCD_FAST_ROTATION_MODE)
        RES_CONFIG_JSON_PATH = get_dict_value(longsots_dict, 'res_config_file', RES_CONFIG_JSON_PATH)
        RES_OUTPUT_DIR = get_dict_value(longsots_dict, 'output_dir', RES_OUTPUT_DIR)
        TMP_APP_ROOT = get_dict_value(longsots_dict, 'app_root', TMP_APP_ROOT)

        if 'res_config_script' in longsots_dict :
            res_config_script_argv = ''
            res_config_script = longsots_dict['res_config_script']
            if 'res_config_script_argv' in longsots_dict :
                res_config_script_argv = longsots_dict['res_config_script_argv']
            set_res_config_by_script(res_config_script, res_config_script_argv)

    else :
        sys_args = common.get_args(args)
        if len(sys_args) > 0 :
            common.set_action(sys_args[0])
            if len(sys_args) > 1:
                GDPI = sys_args[1]
            if len(sys_args) > 2:
                IMAGEGEN_OPTIONS = sys_args[2]
            if len(sys_args) > 3:
                TMP_LCD_ORIENTATION = sys_args[3]
                TMP_LCD_FAST_ROTATION_MODE = False

    AWTK_ROOT = awtk_root
    action = common.get_action()
    if TMP_APP_ROOT == '' :
        APP_ROOT = common.getcwd()
        if APP_ROOT.endswith('scripts'):
            APP_ROOT = os.path.dirname(APP_ROOT)
    else :
        APP_ROOT = TMP_APP_ROOT

    os.chdir(APP_ROOT)

    DPI = 'x1'
    THEMES = []
    APP_THEME = 'default'
    LCD_ORIENTATION = ''
    OUTPUT_ROOT = './res'
    IS_GENERATE_INC_RES = True
    IS_GENERATE_INC_BITMAP = True
    LCD_FAST_ROTATION_MODE = False
    TOOLS_ROOT = common.join_path(AWTK_ROOT, 'bin')
    AWTK_ROOT = common.join_path(APP_ROOT, AWTK_ROOT)
    ASSETS_ROOT = common.join_path(APP_ROOT, 'design')

    use_default_theme_config(RES_CONFIG_JSON_PATH, RES_CONFIG)

    OUTPUT_ROOT = common.join_path(RES_OUTPUT_DIR, 'assets')
    if TMP_LCD_ORIENTATION != '' :
        LCD_ORIENTATION = TMP_LCD_ORIENTATION
    if TMP_LCD_FAST_ROTATION_MODE != None :
        LCD_FAST_ROTATION_MODE = TMP_LCD_FAST_ROTATION_MODE

    ASSET_C = common.join_path(OUTPUT_ROOT, '../assets.inc')
    if action == 'json':
        ASSET_C = common.join_path(APP_ROOT, 'assets_web.js')

    if not is_dependencies_ok():
        print('For details, please read scripts/README.md.')
    elif len(THEMES) == 0:
        print('Not found theme.')
        print('For details, please read scripts/README.md.')
    else:
        if GDPI != '':
           DPI = GDPI
        if IMAGEGEN_OPTIONS != '':
            for theme in THEMES :
                theme["imagegen_options"] = IMAGEGEN_OPTIONS
        if LCD_ORIENTATION != '':
            for theme in THEMES :
                theme["lcd_orientation"] = LCD_ORIENTATION

        common.init(AWTK_ROOT, ASSETS_ROOT, THEMES, ASSET_C, OUTPUT_ROOT)
        common.set_tools_dir(TOOLS_ROOT)
        common.set_lcd_rortrail_info(LCD_ORIENTATION, LCD_FAST_ROTATION_MODE)
        common.set_dpi(DPI)
        common.set_app_theme(APP_THEME)
        common.set_enable_generate_inc_res(IS_GENERATE_INC_RES)
        common.set_enable_generate_inc_bitmap(IS_GENERATE_INC_BITMAP)
        common.set_is_excluded_file_handler(is_excluded_file_handler)
        on_generate_res_event()
        common.update_res()

        if isinstance(THEMES[0], dict):
            if action != 'clean' and action != 'web' and action != 'json' and action != 'pinyin' and action != 'res':
                common.gen_res_c(False)
