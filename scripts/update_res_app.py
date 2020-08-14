#!/usr/bin/python

import os
import sys
import json
import collections

# AWTK_ROOT/scripts/update_res_common.py
import update_res_common as common

def use_theme_config_from_project_json():
    global DPI
    global THEMES
    global OUTPUT_ROOT
    global IS_GENERATE_INC_RES
    global IS_GENERATE_INC_BITMAP
    global APP_THEME
    global APP_ROOT

    project_json = common.join_path(APP_ROOT, 'project.json')
    if not os.path.exists(project_json):
        print('project.json is not exists.')
        return

    content = common.read_file(project_json)
    content = json.loads(content, object_pairs_hook=collections.OrderedDict)

    if not isinstance(content, dict):
        return

    if 'assets' not in content:
        return
    assets = content['assets']

    if 'outputDir' in assets:
        OUTPUT_ROOT = common.to_file_system_coding(assets['outputDir'])
        OUTPUT_ROOT = common.join_path(APP_ROOT, OUTPUT_ROOT+'/assets')

    if 'activedTheme' in assets:
        APP_THEME = assets['activedTheme']

    if 'themes' not in assets:
        return
    
    if len(assets['themes']) == 0:
        return

    if assets['const'] == 'resource_data':
        IS_GENERATE_INC_BITMAP = False
    else:
        IS_GENERATE_INC_RES = False

    if 'screenDPR' in assets:
        DPI = assets['screenDPR']

    for theme_name, theme_setting in assets['themes'].items():
        theme_name = common.to_file_system_coding(theme_name)
        color_format = theme_setting['lcd']['colorFormat']
        color_depth = theme_setting['lcd']['colorDepth']

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
            font = theme_setting['fonts']
            config_dir = common.join_path(ASSETS_ROOT, theme_name+'/fonts/config')
            common.remove_dir(config_dir)
            common.make_dirs(config_dir)
            for font_name, font_setting in theme_setting['fonts'].items():
                for font_size, text in font_setting.items():
                    if font_size.isdigit():
                        font_name = common.to_file_system_coding(font_name)
                        font_size = common.to_file_system_coding(font_size)
                        filename = common.join_path(config_dir, font_name+'_'+font_size+'.txt')
                        common.write_file(filename, text)

        theme = {'name': theme_name, 'imagegen_options': imagegen_options, 'packaged': theme_setting['packaged']}
        if theme_name == 'default':
            THEMES.insert(0, theme)
        else:
            THEMES.append(theme)


def use_default_theme_config():
    global THEMES

    use_theme_config_from_project_json()

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
                if 'imagegen_options' in t and t['imagegen_options'] == 'mono':
                    if not os.path.exists(common.join_path(TOOLS_ROOT, common.to_exe('fontgen_ft'))):
                        print('If you need to generate fonts those display on MONO LCD, please generate fontgen_ft.')
                        return False

    return True


def on_generate_res_event():
    if os.path.exists(common.join_path(APP_ROOT, 'scripts/update_res_generate_res_handler.py')):
        import update_res_generate_res_handler as generate_res
        if hasattr(generate_res, 'on_generate_res_before'):
           common.on_generate_res_before(generate_res.on_generate_res_before)
        if hasattr(generate_res, 'on_generate_res_after'):
           common.on_generate_res_after(generate_res.on_generate_res_after)


def run(awtk_root):
    global AWTK_ROOT
    global TOOLS_ROOT
    global THEMES
    global APP_ROOT
    global ASSETS_ROOT
    global IS_GENERATE_INC_BITMAP
    global IS_GENERATE_INC_BITMAP 

    AWTK_ROOT = awtk_root
    APP_ROOT = common.getcwd()
    if APP_ROOT.endswith('scripts'):
        APP_ROOT = os.path.dirname(APP_ROOT)

    os.chdir(APP_ROOT)

    DPI = 'x1'
    THEMES = []
    APP_THEME = 'default'
    IS_GENERATE_INC_RES = True
    IS_GENERATE_INC_BITMAP = True
    TOOLS_ROOT = common.join_path(AWTK_ROOT, 'bin')
    AWTK_ROOT = common.join_path(APP_ROOT, AWTK_ROOT)
    ASSETS_ROOT = common.join_path(APP_ROOT, 'design')
    ASSET_C = common.join_path(APP_ROOT, 'res/assets.inc')
    OUTPUT_ROOT = common.join_path(APP_ROOT, 'res/assets')

    use_default_theme_config()
    if not is_dependencies_ok():
        print('For details, please read scripts/README.md.')
    elif len(THEMES) == 0:
        print('Not found theme.')
        print('For details, please read scripts/README.md.')
    else:
        common.init(AWTK_ROOT, ASSETS_ROOT, THEMES, ASSET_C, OUTPUT_ROOT)
        common.set_tools_dir(TOOLS_ROOT)
        common.set_dpi(DPI)
        common.set_app_theme(APP_THEME)
        common.set_enable_generate_inc_res(IS_GENERATE_INC_RES)
        common.set_enable_generate_inc_bitmap(IS_GENERATE_INC_BITMAP)
        on_generate_res_event()
        common.update_res()

        if isinstance(THEMES[0], dict):
            action = common.get_action()
            if action != 'clean' and action != 'web' and action != 'json' and action != 'pinyin':
                common.gen_res_c(False)
