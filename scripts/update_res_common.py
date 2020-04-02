import os
import sys
import copy
import glob
import shutil
import platform
from PIL import Image

###########################
DPI = ''
ACTION = 'all'
ASSET_C = ''
BIN_DIR = ''
THEME = 'default'
ASSETS_ROOT = ''
DEFAULT_THEME_ASSETS_ROOT = ''
AWTK_ROOT = ''
INPUT_DIR = ''
OUTPUT_DIR = ''
DEFAULT_THEME_OUTPUT_DIR = ''
FONT_OPTIONS = ''
IMAGEGEN_OPTIONS = ''
IS_DEFAULT_THEME = False
###########################


def to_var_name(s):
    out = ''
    for c in s:
        if(c.isalpha() or c.isdigit()):
            out += c
        else:
            out += '_'
    return out


def fix_output_file_name(filename):
    basename = os.path.basename(filename)
    dirname = os.path.dirname(filename)
    varname = to_var_name(basename)
    newname = os.path.normpath(os.path.join(dirname, varname))
    return newname


def to_assets_basename(filename, output_dir):
    basename = os.path.splitext(filename)[0]
    basename = basename.replace(output_dir, '.')
    basename = basename.replace('\\', '/')
    basename = basename.replace('/fonts/', '/font/')
    basename = basename.replace('/images/', '/image/')
    basename = basename.replace('/styles/', '/style/')
    basename = basename.replace('/scripts/', '/script/')
    basename = basename.replace('./', '')
    basename = basename.replace('/', '_')
    return basename


def fix_inc_file(filename):
    with open(filename, "r") as fr:
        content = fr.read()

    end = content.find('\n')
    if end <= 0:
        end = len(content)

    basename = to_assets_basename(filename, OUTPUT_DIR)
    index = content.find(basename + '[])', 0, end)
    if index > 0:
        with open(filename, 'w') as fw:
            content = content.replace(basename + '[])', basename + '_' + THEME + '[])', 1)
            fw.write(content)


def joinPath(root, subdir):
    return os.path.normpath(os.path.join(root, subdir))


OS_NAME = platform.system()


def toExe(name):
    if OS_NAME == 'Windows':
        return joinPath(BIN_DIR, name+'.exe')
    else:
        return joinPath(BIN_DIR, name)


def buildAll():
    os.system('scons')


def removeDir(path):
    if os.path.isdir(path):
        print('rmdir:' + path)
        shutil.rmtree(path)


def prepareOutputDir(name):
    fullpath = joinPath(OUTPUT_DIR, name)
    if(not os.path.exists(fullpath)):
        os.makedirs(fullpath)


def prepare():
    prepareOutputDir('styles')
    prepareOutputDir('images')
    prepareOutputDir('fonts')
    prepareOutputDir('strings')
    prepareOutputDir('ui')
    prepareOutputDir('scripts')
    prepareOutputDir('data')
    prepareOutputDir('xml')


def execCmd(cmd):
    print(cmd)
    os.system(cmd)


def themegen(raw, inc):
    input=joinPath(INPUT_DIR, raw)
    output=joinPath(OUTPUT_DIR, inc)
    if(os.path.isfile(input)):
        execCmd(toExe('themegen') + ' ' + input + ' ' + output)


def themegen_bin(raw, bin):
    input=joinPath(INPUT_DIR, raw)
    output=joinPath(INPUT_DIR, bin)
    if(os.path.isfile(input)):
        execCmd(toExe('themegen') + ' ' + input + ' ' + output + ' bin')


def strgen(raw, inc):
    input=joinPath(INPUT_DIR, raw)
    output=joinPath(OUTPUT_DIR, inc)
    if(os.path.isfile(input)):
        execCmd(toExe('strgen') + ' ' + input + ' ' + output)


def strgen_bin(raw, bin):
    input=joinPath(INPUT_DIR, raw)
    output=joinPath(INPUT_DIR, bin)
    if(os.path.isfile(input)):
        execCmd(toExe('strgen') + ' ' + input + ' ' + output + ' bin')

def resgen(raw, inc):
    input=joinPath(INPUT_DIR, raw)
    output=joinPath(OUTPUT_DIR, inc)
    if(os.path.isfile(input)):
        execCmd(toExe('resgen') + ' ' + input + ' ' + output)


def fontgen(raw, text, inc, size):
    inputFont=joinPath(INPUT_DIR, raw);
    inputText=joinPath(INPUT_DIR, text);
    output=joinPath(OUTPUT_DIR, inc);
    if(os.path.isfile(inputFont)):
        execCmd(toExe('fontgen') + ' ' + inputFont + ' ' +
            inputText + ' ' + output + ' ' +
            str(size) + ' ' + FONT_OPTIONS)


def imagegen(raw, inc):
    if(os.path.isfile(raw)):
        execCmd(toExe('imagegen') + ' ' + raw + ' ' + inc + ' ' + IMAGEGEN_OPTIONS)
        inc = inc.replace('.data', '.res')
        resgen(raw, inc)


def svggen(raw, inc, bin):
    if(os.path.isfile(raw)):
        execCmd(toExe('bsvggen') + ' ' + raw + ' ' + inc)
        execCmd(toExe('bsvggen') + ' ' + raw + ' ' + bin + ' bin')


def xml_to_ui(raw, inc):
    if(os.path.isfile(raw)):
        execCmd(toExe('xml_to_ui') + ' ' + raw + ' ' + inc)


def xml_to_ui_bin(raw, bin):
    if(os.path.isfile(raw)):
        execCmd(toExe('xml_to_ui') + ' ' + raw + ' ' + bin + ' bin')


def gen_res_all_style():
    for f in glob.glob(joinPath(INPUT_DIR, 'styles/*.xml')):
        filename, extname = os.path.splitext(f)
        raw = f
        bin = filename + '.bin'
        themegen_bin(raw, bin)
        filename = filename.replace(INPUT_DIR, OUTPUT_DIR)
        inc = fix_output_file_name(filename) + '.data'
        themegen(raw, inc)
        if not IS_DEFAULT_THEME: 
            fix_inc_file(inc)


def gen_res_svg():
    for f in glob.glob(joinPath(INPUT_DIR, 'images/svg/*.svg')):
        filename, extname = os.path.splitext(f)
        raw = f
        bin = filename + '.bsvg'
        basename = os.path.basename(filename)
        filename = joinPath(OUTPUT_DIR, 'images/'+basename)
        inc = fix_output_file_name(filename) + '.bsvg'
        svggen(raw, inc, bin)
        if not IS_DEFAULT_THEME: 
            fix_inc_file(inc)


def gen_res_png_jpg():
    formats = ['.png', '.jpg', '.jpeg', '.bmp', '.gif']
    files = glob.glob(joinPath(INPUT_DIR, 'images/'+DPI+'/*.*')) \
        + glob.glob(joinPath(INPUT_DIR, 'images/xx/*.*'))
    for f in files:
        filename, extname = os.path.splitext(f)
        for fm in formats:
            if fm == extname:
                raw = f
                basename = os.path.basename(filename)
                filename = joinPath(OUTPUT_DIR, 'images/'+basename)
                filename = fix_output_file_name(filename)
                inc = filename + '.data'
                imagegen(raw, inc)
                if not IS_DEFAULT_THEME: 
                    fix_inc_file(inc)
                    fix_inc_file(filename + '.res')
                break


def gen_res_all_image():
    gen_res_png_jpg()
    gen_res_svg()


def gen_res_all_ui():
    for f in glob.glob(joinPath(INPUT_DIR, 'ui/*.xml')):
        filename, extname = os.path.splitext(f)
        raw = f
        bin = filename + '.bin'
        xml_to_ui_bin(raw, bin)
        filename = filename.replace(INPUT_DIR, OUTPUT_DIR)
        inc = fix_output_file_name(filename) + '.data'
        xml_to_ui(raw, inc)
        if not IS_DEFAULT_THEME: 
            fix_inc_file(inc)


def gen_res_all_data():
    for f in glob.glob(joinPath(INPUT_DIR, 'data/*.*')):
        filename, extname = os.path.splitext(f)
        raw = f
        filename = filename.replace(INPUT_DIR, OUTPUT_DIR) + extname.replace('.', '_')
        inc = fix_output_file_name(filename) + '.data'
        resgen(raw, inc)
        if not IS_DEFAULT_THEME: 
            fix_inc_file(inc)


def gen_res_all_xml():
    for f in glob.glob(joinPath(INPUT_DIR, 'xml/*.xml')):
        filename, extname = os.path.splitext(f)
        raw = f
        filename = filename.replace(INPUT_DIR, OUTPUT_DIR)
        inc = fix_output_file_name(filename) + '.data'
        resgen(raw, inc)
        if not IS_DEFAULT_THEME: 
            fix_inc_file(inc)


def gen_res_all_font():
    for f in glob.glob(joinPath(INPUT_DIR, 'fonts/*.ttf')):
        filename, extname = os.path.splitext(f)
        raw = f
        filename = filename.replace(INPUT_DIR, OUTPUT_DIR)
        inc = fix_output_file_name(filename) + '.res'
        resgen(raw, inc)
        if not IS_DEFAULT_THEME: 
            fix_inc_file(inc)

    if os.path.exists(joinPath(INPUT_DIR, 'fonts/default_full.ttf')):
        if os.path.exists(joinPath(INPUT_DIR, 'fonts/text.txt')):
            font_sizes = [16, 18, 20, 24, 32, 96]
            for sz in font_sizes:
                inc = 'fonts/default_%d.data' % sz
                fontgen('fonts/default_full.ttf', 'fonts/text.txt', inc, sz)
                if not IS_DEFAULT_THEME: 
                    fix_inc_file(joinPath(OUTPUT_DIR, inc))



def gen_res_all_script():
    for f in glob.glob(joinPath(INPUT_DIR, 'scripts/*.js')):
        filename, extname = os.path.splitext(f)
        raw = f
        filename = filename.replace(INPUT_DIR, OUTPUT_DIR)
        inc = fix_output_file_name(filename) + '.res'
        resgen(raw, inc)
        if not IS_DEFAULT_THEME: 
            fix_inc_file(inc)


def gen_res_all_string():
    print('gen_res_all_string')
    strgen('strings/strings.xml', 'strings')
    strgen_bin('strings/strings.xml', 'strings')
    if not IS_DEFAULT_THEME: 
        files = glob.glob(joinPath(OUTPUT_DIR, 'strings/*.data'))
        for f in files:
            fix_inc_file(f)


def gen_gpinyin():
    execCmd(toExe('resgen') + ' ' + joinPath('3rd', 'gpinyin/data/gpinyin.dat') +
            ' ' + joinPath('3rd', 'gpinyin/src/gpinyin.inc'))
    execCmd(toExe('resgen') + ' ' + joinPath('tools', 'word_gen/words.bin') +
            ' ' + joinPath('src', 'input_methods/suggest_words.inc'))
    execCmd(toExe('resgen') + ' ' + joinPath('tools',
                                             'word_gen/words.bin') + ' ' + joinPath('tests', 'suggest_test.inc'))


def gen_res_all():
    gen_res_all_string()
    gen_res_all_font()
    gen_res_all_script()
    gen_res_all_image()
    gen_res_all_ui()
    gen_res_all_style()
    gen_res_all_data()
    gen_res_all_xml()


def writeResult(filename, str):
    with open(filename, "w") as text_file:
        text_file.write(str)


def writeResultJSON(str):
    with open('assets.js', "w") as text_file:
        text_file.write(str)


def genIncludes(files):
    result = ""
    assets_root = os.path.dirname(os.path.dirname(ASSETS_ROOT))
    for f in files:
        incf = copy.copy(f)
        incf = incf.replace(assets_root, ".")
        incf = incf.replace('\\', '/')
        incf = incf.replace('./', '')
        result += '#include "'+incf+'"\n'

    return result

def genExternOfDefaultTheme(files):
    result = ""
    for f in files:
        if(not os.path.exists(f.replace(DEFAULT_THEME_ASSETS_ROOT, ASSETS_ROOT))):
            basename = to_assets_basename(f, DEFAULT_THEME_OUTPUT_DIR)
            result += 'extern TK_CONST_DATA_ALIGN(const unsigned char '+basename+'[]);\n'

    return result

def gen_add_assets(files):
    result = ""
    for f in files:
        basename = to_assets_basename(f, OUTPUT_DIR)
        if IS_DEFAULT_THEME:
            result += '  assets_manager_add(am, '+basename+');\n'
        else:
            result += '  assets_manager_add(am, '+basename+'_'+THEME+');\n'
    return result

def gen_add_assets_of_default_theme(files):
    result = ""
    for f in files:
        if(not os.path.exists(f.replace(DEFAULT_THEME_ASSETS_ROOT, ASSETS_ROOT))):
            basename = to_assets_basename(f, DEFAULT_THEME_OUTPUT_DIR)
            result += '  assets_manager_add(am, '+basename+');\n'

    return result

def gen_res_c():
    result = '#include "awtk.h"\n'
    result += '#include "base/assets_manager.h"\n'
    result += '#ifndef WITH_FS_RES\n'

    files = glob.glob(joinPath(OUTPUT_DIR, 'strings/*.data')) \
        + glob.glob(joinPath(OUTPUT_DIR, 'styles/*.data'))  \
        + glob.glob(joinPath(OUTPUT_DIR, 'ui/*.data')) \
        + glob.glob(joinPath(OUTPUT_DIR, 'xml/*.data')) \
        + glob.glob(joinPath(OUTPUT_DIR, 'data/*.data'))
    result += genIncludes(files)
    if not IS_DEFAULT_THEME:
        files = glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'strings/*.data')) \
        + glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'styles/*.data'))  \
        + glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'ui/*.data')) \
        + glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'xml/*.data')) \
        + glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'data/*.data'))
        result += genExternOfDefaultTheme(files)

    result += "#ifdef WITH_STB_IMAGE\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'images/*.res'))
    result += genIncludes(files)
    if not IS_DEFAULT_THEME:
        files = glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'images/*.res'))
        result += genExternOfDefaultTheme(files)
    result += "#else\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'images/*.data'))
    result += genIncludes(files)
    if not IS_DEFAULT_THEME:
        files = glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'images/*.data'))
        result += genExternOfDefaultTheme(files)
    result += '#endif /*WITH_STB_IMAGE*/\n'

    result += "#ifdef WITH_VGCANVAS\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'images/*.bsvg'))
    result += genIncludes(files)
    if not IS_DEFAULT_THEME:
        files = glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'images/*.bsvg'))
        result += genExternOfDefaultTheme(files)
    result += '#endif /*WITH_VGCANVAS*/\n'

    result += "#if defined(WITH_STB_FONT) || defined(WITH_FT_FONT)\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'fonts/*.res'))
    result += genIncludes(files)
    if not IS_DEFAULT_THEME:
        files = glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'fonts/*.res'))
        result += genExternOfDefaultTheme(files)
    result += "#else/*WITH_STB_FONT or WITH_FT_FONT*/\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'fonts/*.data'))
    result += genIncludes(files)
    if not IS_DEFAULT_THEME:
        files = glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'fonts/*.data'))
        result += genExternOfDefaultTheme(files)
    result += '#endif /*WITH_STB_FONT or WITH_FT_FONT*/\n'

    files = glob.glob(joinPath(OUTPUT_DIR, 'scripts/*.res'))
    result += genIncludes(files)
    if not IS_DEFAULT_THEME:
        files = glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'scripts/*.res'))
        result += genExternOfDefaultTheme(files)

    result += '#endif /*WITH_FS_RES*/\n'

    result += '\n'
    result += 'ret_t assets_init_' + THEME + '(void) {\n'
    result += '  assets_manager_t* am = assets_manager();\n\n'
    result += ''

    result += '#ifdef WITH_FS_RES\n'
    result += '  assets_manager_preload(am, ASSET_TYPE_FONT, "default");\n'
    result += '  assets_manager_preload(am, ASSET_TYPE_STYLE, "default");\n'
    result += '#else\n'

    files = glob.glob(joinPath(OUTPUT_DIR, 'strings/*.data')) \
        + glob.glob(joinPath(OUTPUT_DIR, 'styles/*.data'))  \
        + glob.glob(joinPath(OUTPUT_DIR, 'ui/*.data')) \
        + glob.glob(joinPath(OUTPUT_DIR, 'xml/*.data')) \
        + glob.glob(joinPath(OUTPUT_DIR, 'data/*.data')) \
        + glob.glob(joinPath(OUTPUT_DIR, 'images/*.data'))
    result += gen_add_assets(files)
    if not IS_DEFAULT_THEME:
        files = glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'strings/*.data')) \
            + glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'styles/*.data'))  \
            + glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'ui/*.data')) \
            + glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'xml/*.data')) \
            + glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'data/*.data')) \
            + glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'images/*.data'))
        result += gen_add_assets_of_default_theme(files)

    result += "#if defined(WITH_STB_FONT) || defined(WITH_FT_FONT)\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'fonts/*.res'))
    result += gen_add_assets(files)
    if not IS_DEFAULT_THEME:
        files = glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'fonts/*.res'))
        result += gen_add_assets_of_default_theme(files)
    result += "#else /*WITH_STB_FONT or WITH_FT_FONT*/\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'fonts/*.data'))
    result += gen_add_assets(files)
    if not IS_DEFAULT_THEME:
        files = glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'fonts/*.data'))
        result += gen_add_assets_of_default_theme(files)
    result += '#endif /*WITH_STB_FONT or WITH_FT_FONT*/\n'

    result += "#ifdef WITH_VGCANVAS\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'images/*.bsvg'))
    result += gen_add_assets(files)
    if not IS_DEFAULT_THEME:
        files = glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'images/*.bsvg'))
        result += gen_add_assets_of_default_theme(files)
    result += '#endif /*WITH_VGCANVAS*/\n'

    files = glob.glob(joinPath(OUTPUT_DIR, 'scripts/*.res'))
    result += gen_add_assets(files)
    if not IS_DEFAULT_THEME:
        files = glob.glob(joinPath(DEFAULT_THEME_OUTPUT_DIR, 'scripts/*.res'))
        result += gen_add_assets_of_default_theme(files)

    result += '#endif\n'

    result += '\n'
    result += '  tk_init_assets();\n'
    result += '  return RET_OK;\n'
    result += '}\n'

    writeResult(ASSET_C.replace('.c', '_' + THEME + '.inc'), result)


def gen_res_web_c():
    result = '#include "awtk.h"\n'
    result += '#include "base/assets_manager.h"\n'

    files = glob.glob(joinPath(OUTPUT_DIR, 'images/*.bsvg')) \
        + glob.glob(joinPath(OUTPUT_DIR, 'strings/*.data')) \
        + glob.glob(joinPath(OUTPUT_DIR, 'styles/*.data'))  \
        + glob.glob(joinPath(OUTPUT_DIR, 'ui/*.data')) \
        + glob.glob(joinPath(OUTPUT_DIR, 'xml/*.data')) \
        + glob.glob(joinPath(OUTPUT_DIR, 'data/*.data'))

    result += genIncludes(files)

    result += '\n'
    result += 'ret_t assets_init(void) {\n'
    result += '  assets_manager_t* am = assets_manager();\n\n'
    result += ''

    result += gen_add_assets(files)

    result += '\n'
    result += '  tk_init_assets();\n'
    result += '  return RET_OK;\n'
    result += '}\n'

    writeResult(ASSET_C.replace('.c', '_web.c'), result)


def gen_res_json_one(res_type, files):
    result = "\n  " + res_type + ': [\n'
    for f in files:
        uri = f.replace(os.getcwd(), "")[1:]
        uri = uri.replace('\\', '/')
        filename, extname = os.path.splitext(uri)
        basename = os.path.basename(filename)
        result = result + '    {name:"' + basename + '\", uri:"' + uri
        if res_type == 'image' and extname != '.svg' and extname != '.bsvg':
            img = Image.open(f)
            w, h = img.size
            result = result + '", w:' + str(w) + ', h:' + str(h) + '},\n'
        else:
            result = result + '"},\n'
    result = result + '  ],'

    return result


def gen_res_json():
    result = 'const g_awtk_assets = {'

    result = result + \
        gen_res_json_one("image", glob.glob(
            joinPath(INPUT_DIR, 'images/*/*.*')))
    result = result + \
        gen_res_json_one("ui", glob.glob(joinPath(INPUT_DIR, 'ui/*.bin')))
    result = result + \
        gen_res_json_one("style", glob.glob(
            joinPath(INPUT_DIR, 'styles/*.bin')))
    result = result + \
        gen_res_json_one("string", glob.glob(
            joinPath(INPUT_DIR, 'strings/*.bin')))
    result = result + \
        gen_res_json_one("xml", glob.glob(joinPath(INPUT_DIR, 'xml/*.xml')))
    result = result + \
        gen_res_json_one("data", glob.glob(joinPath(INPUT_DIR, 'data/*.*')))
    result = result + \
        gen_res_json_one("script", glob.glob(
            joinPath(INPUT_DIR, 'scripts/*.*')))
    result = result + \
        gen_res_json_one("font", glob.glob(joinPath(INPUT_DIR, 'fonts/*.ttf')))
    result = result + '\n};'

    writeResult(ASSET_C.replace('.c', '_web.js'), result)


def gen_res():
    prepare()
    gen_res_all()
    gen_res_c()


def genAssetC(themes, asset_c):
    result = '#include "awtk.h"\n'
    result += '#include "base/assets_manager.h"\n\n'

    for theme in themes:
        result += '#include "assets_'+theme+'.inc"\n'

    result += '\n'
    result += '#ifndef APP_THEME\n'
    result += '#define APP_THEME "default"\n'
    result += '#endif /*APP_THEME*/\n\n'

    result += 'bool_t assets_has_theme(const char* name) {\n'
    result += '  return_value_if_fail(name != NULL, FALSE);\n\n'
    result += '  '
    for theme in themes:
        result +=   'if (tk_str_eq(name, "'+theme+'")) {\n'
        result += '    return TRUE;\n'
        result += '  } else '
    result += '{\n'
    result += '    return FALSE;\n  }\n}\n\n'

    result += 'static ret_t assets_init_internal(const char* theme) {\n'
    result += '  assets_manager_t* am = assets_manager();\n'
    result += '  return_value_if_fail(theme != NULL && am != NULL, RET_BAD_PARAMS);\n\n'
    result += '  assets_manager_set_theme(am, theme);\n\n'
    result += '  '
    for theme in themes:
        result +=   'if (tk_str_eq(theme, "'+theme+'")) {\n'
        result += '    return assets_init_'+theme+'();\n'
        result += '  } else '
    result += '{\n'
    result += '    log_debug(\"%s not support.\\n\", theme);\n'
    result += '    return RET_NOT_IMPL;\n  }\n}\n\n'

    result += 'ret_t assets_init(void) {\n'
    result += '  return assets_init_internal(APP_THEME);\n}\n\n'

    result += 'static ret_t widget_set_theme_without_file_system(widget_t* widget, const char* name) {\n'
    result += '#ifndef WITH_FS_RES\n'
    result += '  const asset_info_t* info = NULL;\n'
    result += '  event_t e = event_init(EVT_THEME_CHANGED, NULL);\n'
    result += '  widget_t* wm = widget_get_window_manager(widget);\n'
    result += '  font_manager_t* fm = widget_get_font_manager(widget);\n'
    result += '  image_manager_t* imm = widget_get_image_manager(widget);\n'
    result += '  assets_manager_t* am = widget_get_assets_manager(widget);\n'
    result += '  locale_info_t* locale_info = widget_get_locale_info(widget);\n\n'
    result += '  return_value_if_fail(am != NULL && name != NULL, RET_BAD_PARAMS);\n'
    result += '  return_value_if_fail(assets_has_theme(name), RET_BAD_PARAMS);\n\n'
    result += '  font_manager_unload_all(fm);\n'
    result += '  image_manager_unload_all(imm);\n'
    result += '  assets_manager_clear_all(am);\n'
    result += '  widget_reset_canvas(widget);\n\n'
    result += '  assets_init_internal(name);\n'
    result += '  locale_info_reload(locale_info);\n\n'
    result += '  info = assets_manager_ref(am, ASSET_TYPE_STYLE, "default");\n'
    result += '  theme_init(theme(), info->data);\n'
    result += '  assets_manager_unref(assets_manager(), info);\n\n'
    result += '  widget_dispatch(wm, &e);\n'
    result += '  widget_invalidate_force(wm, NULL);\n\n'
    result += '  log_debug("theme changed: %s\\n", name);\n\n'
    result += '  return RET_OK;\n'
    result += '#else /*WITH_FS_RES*/\n'
    result += '  return RET_NOT_IMPL;\n'
    result += '#endif /*WITH_FS_RES*/\n'
    result += '}\n\n'

    result += 'ret_t assets_set_global_theme(const char* name) {\n'
    result += '#ifdef WITH_FS_RES\n'
    result += '  return widget_set_theme(window_manager(), name);\n'
    result += '#else  /*WITH_FS_RES*/\n'
    result += '  return widget_set_theme_without_file_system(window_manager(), name);\n'
    result += '#endif /*WITH_FS_RES*/\n'
    result += '}\n'

    writeResult(asset_c, result)


def init(awtk_root, assets_root, theme, asset_c):
    global DPI
    global THEME
    global ASSET_C
    global BIN_DIR
    global ASSETS_ROOT
    global DEFAULT_THEME_ASSETS_ROOT
    global AWTK_ROOT
    global INPUT_DIR
    global OUTPUT_DIR
    global DEFAULT_THEME_OUTPUT_DIR
    global IMAGEGEN_OPTIONS
    global IS_DEFAULT_THEME

    IS_DEFAULT_THEME = theme == 'default'
    THEME = theme
    ASSET_C = asset_c
    AWTK_ROOT = awtk_root
    ASSETS_ROOT = joinPath(assets_root, theme)
    DEFAULT_THEME_ASSETS_ROOT = joinPath(assets_root, 'default')

    BIN_DIR = joinPath(AWTK_ROOT, 'bin')
    INPUT_DIR = joinPath(ASSETS_ROOT, 'raw')
    OUTPUT_DIR = joinPath(ASSETS_ROOT, 'inc')
    DEFAULT_THEME_OUTPUT_DIR = joinPath(DEFAULT_THEME_ASSETS_ROOT, 'inc')
    print(INPUT_DIR)
    print(OUTPUT_DIR)


def dumpArgs():
    print('-------------------------------------------------------')
    print('DPI='+DPI)
    print('THEME='+THEME)
    print('IMAGEGEN_OPTIONS='+IMAGEGEN_OPTIONS)
    print('AWTK_ROOT='+AWTK_ROOT)
    print('ASSETS_ROOT='+ASSETS_ROOT)
    print('INPUT_DIR='+INPUT_DIR)
    print('OUTPUT_DIR='+OUTPUT_DIR)
    print('ASSET_C='+ASSET_C)
    print('BIN_DIR='+BIN_DIR)


def updateRes():
    global ACTION
    if ACTION == 'all':
        removeDir(OUTPUT_DIR)
        gen_res()
    elif ACTION == 'clean':
        cleanRes()
    elif ACTION == 'web':
        gen_res_web_c()
    elif ACTION == 'json':
        gen_res_json()
    elif ACTION == 'string':
        prepare()
        gen_res_all_string()
        gen_res_c()
    elif ACTION == "font":
        prepare()
        gen_res_all_font()
        gen_res_c()
    elif ACTION == "script":
        prepare()
        gen_res_all_script()
        gen_res_c()
    elif ACTION == 'image':
        prepare()
        gen_res_all_image()
        gen_res_c()
    elif ACTION == 'ui':
        prepare()
        gen_res_all_ui()
        gen_res_c()
    elif ACTION == 'style':
        prepare()
        gen_res_all_style()
        gen_res_c()
    elif ACTION == 'data':
        prepare()
        gen_res_all_data()
        gen_res_c()
    elif ACTION == 'xml':
        prepare()
        gen_res_all_xml()
        gen_res_c()
    elif ACTION == 'pinyin':
        prepare()
        gen_gpinyin()
        gen_res_c()
    dumpArgs()


def cleanRes():
    print("==================================================================")
    resFiles = glob.glob(joinPath(INPUT_DIR, '*/*.bin')) + \
        glob.glob(joinPath(INPUT_DIR, '*/*/*.bin'))
    for f in resFiles:
        print("remove: " + f)
        os.remove(f)
    resFiles = glob.glob(joinPath(INPUT_DIR, '*/*.bin')) + \
        glob.glob(joinPath(INPUT_DIR, '*/*/*.bsvg'))
    for f in resFiles:
        print("remove: " + f)
        os.remove(f)
    removeDir(OUTPUT_DIR)
    print("==================================================================")


def showUsage():
    global DPI
    global ACTION
    global IMAGEGEN_OPTIONS
    global FONT_OPTIONS
    args = ' action[clean|web|json|all|font|image|ui|style|string|script|data|xml] dpi[x1|x2] image_options[rgba|bgra+bgr565|mono]'
    if len(sys.argv) == 1:
        print('=========================================================')
        print('Usage: python '+sys.argv[0] + args)
        print('Example:')
        print('python ' + sys.argv[0] + ' all')
        print('python ' + sys.argv[0] + ' clean')
        print('python ' + sys.argv[0] + ' style')
        print('python ' + sys.argv[0] + ' all x1 bgra+bgr565')
        print('=========================================================')
        sys.exit(0)
    else:
        ACTION = sys.argv[1]
        if len(sys.argv) > 2:
            DPI = sys.argv[2]
        else:
            DPI = 'x1'

        if len(sys.argv) > 3:
            IMAGEGEN_OPTIONS = sys.argv[3]
        else:
            IMAGEGEN_OPTIONS = 'bgra+bgr565'

        if IMAGEGEN_OPTIONS == 'mono':
            FONT_OPTIONS = 'mono'


showUsage()
