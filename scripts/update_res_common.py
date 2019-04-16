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
ASSETS_ROOT = ''
AWTK_ROOT = ''
INPUT_DIR = ''
OUTPUT_DIR = ''
IMAGEGEN_OPTIONS = ''
###########################


def to_var_name(s):
    out = ''
    for c in s:
        if(c.isalpha() or c.isdigit()):
            out += c
        else:
            out += '_'
    return out


def fix_output_file_name(name):
    filename, extname = os.path.splitext(name)
    basename = os.path.basename(filename)
    dirname = os.path.dirname(filename)
    newname = os.path.normpath(os.path.join(
        dirname, to_var_name(basename) + extname))
    return newname


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
    if os.path.exists(fullpath):
        print(fullpath+" exist.")
    else:
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
    execCmd(toExe('themegen') + ' ' + joinPath(INPUT_DIR, raw) +
            ' ' + joinPath(OUTPUT_DIR, inc))


def themegen_bin(raw, bin):
    execCmd(toExe('themegen') + ' ' + joinPath(INPUT_DIR, raw) +
            ' ' + joinPath(INPUT_DIR, bin) + ' bin')


def strgen(raw, inc):
    execCmd(toExe('strgen') + ' ' + joinPath(INPUT_DIR, raw) +
            ' ' + joinPath(OUTPUT_DIR, inc))


def strgen_bin(raw, bin):
    execCmd(toExe('strgen') + ' ' + joinPath(INPUT_DIR, raw) +
            ' ' + joinPath(INPUT_DIR, bin) + ' bin')


def resgen(raw, inc):
    execCmd(toExe('resgen') + ' ' + joinPath(INPUT_DIR, raw) +
            ' ' + joinPath(OUTPUT_DIR, inc))


def fontgen(raw, text, inc, size):
    execCmd(toExe('fontgen') + ' ' + joinPath(INPUT_DIR, raw) + ' ' +
            joinPath(INPUT_DIR, text) + ' ' + joinPath(OUTPUT_DIR, inc) + ' ' + str(size))


def imagegen(raw, inc):
    execCmd(toExe('imagegen') + ' ' + raw + ' ' + inc + ' ' + IMAGEGEN_OPTIONS)
    inc = inc.replace('.data', '.res')
    resgen(raw, inc)


def svggen(raw, inc, bin):
    execCmd(toExe('bsvggen') + ' ' + raw + ' ' + inc)
    execCmd(toExe('bsvggen') + ' ' + raw + ' ' + bin + ' bin')


def xml_to_ui(raw, inc):
    execCmd(toExe('xml_to_ui') + ' ' + raw + ' ' + inc)


def xml_to_ui_bin(raw, bin):
    execCmd(toExe('xml_to_ui') + ' ' + raw + ' ' + bin + ' bin')


def gen_res_all_style():
    for f in glob.glob(joinPath(INPUT_DIR, 'styles/*.xml')):
        inc = copy.copy(f)
        raw = copy.copy(f)
        bin = copy.copy(f)
        inc = inc.replace('.xml', '.data')
        inc = inc.replace(INPUT_DIR, OUTPUT_DIR)
        inc = fix_output_file_name(inc)
        themegen(raw, inc)
        bin = bin.replace('.xml', '.bin')
        themegen_bin(raw, bin)


def gen_res_svg():
    for f in glob.glob(joinPath(INPUT_DIR, 'images/svg/*.svg')):
        inc = copy.copy(f)
        bin = copy.copy(f)
        raw = copy.copy(f)
        basename = os.path.basename(inc)
        inc = joinPath(OUTPUT_DIR, 'images/'+basename)
        inc = inc.replace('.svg', '.bsvg')
        inc = fix_output_file_name(inc)
        bin = bin.replace('.svg', '.bsvg')
        svggen(raw, inc, bin)


def gen_res_png_jpg():
    for f in glob.glob(joinPath(INPUT_DIR, 'images/'+DPI+'/*.*')):
        inc = copy.copy(f)
        raw = copy.copy(f)
        basename = os.path.basename(inc)
        inc = joinPath(OUTPUT_DIR, 'images/'+basename)
        inc = inc.replace('.png', '.data')
        inc = inc.replace('.jpg', '.data')
        inc = inc.replace('.gif', '.data')
        inc = fix_output_file_name(inc)
        imagegen(raw, inc)


def gen_res_all_image():
    gen_res_png_jpg()
    gen_res_svg()


def gen_res_all_ui():
    for f in glob.glob(joinPath(INPUT_DIR, 'ui/*.xml')):
        inc = copy.copy(f)
        raw = copy.copy(f)
        bin = copy.copy(f)
        inc = inc.replace('.xml', '.data')
        inc = inc.replace(INPUT_DIR, OUTPUT_DIR)
        inc = fix_output_file_name(inc)
        xml_to_ui(raw, inc)
        bin = bin.replace('.xml', '.bin')
        xml_to_ui_bin(raw, bin)


def gen_res_all_data():
    for f in glob.glob(joinPath(INPUT_DIR, 'data/*.*')):
        inc = copy.copy(f)
        raw = copy.copy(f)
        _, extname = os.path.splitext(inc)
        uextname = extname.replace('.', '_')
        inc = inc.replace(extname, uextname+'.data')
        inc = inc.replace(INPUT_DIR, OUTPUT_DIR)
        inc = fix_output_file_name(inc)
        resgen(raw, inc)


def gen_res_all_xml():
    for f in glob.glob(joinPath(INPUT_DIR, 'xml/*.xml')):
        inc = copy.copy(f)
        raw = copy.copy(f)
        inc = inc.replace('.xml', '.data')
        inc = inc.replace(INPUT_DIR, OUTPUT_DIR)
        inc = fix_output_file_name(inc)
        resgen(raw, inc)


def gen_res_all_font():
    for f in glob.glob(joinPath(INPUT_DIR, 'fonts/*.ttf')):
        res = copy.copy(f)
        raw = copy.copy(f)
        res = res.replace(INPUT_DIR, '.')
        res = res.replace('.ttf', '.res')
        raw = raw.replace(INPUT_DIR, '.')
        resgen(raw, res)
    fontgen('fonts/default.ttf', 'fonts/text.txt', 'fonts/default.data', 18)


def gen_res_all_script():
    for f in glob.glob(joinPath(INPUT_DIR, 'scripts/*.js')):
        res = copy.copy(f)
        raw = copy.copy(f)
        res = res.replace(INPUT_DIR, '.')
        res = res.replace('.js', '.res')
        raw = raw.replace(INPUT_DIR, '.')
        raw = fix_output_file_name(raw)
        resgen(raw, res)


def gen_res_all_string():
    print('gen_res_all_string');
    strgen('strings/strings.xml', 'strings')
    strgen_bin('strings/strings.xml', 'strings')


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


def writeResult(str):
    fd = os.open(ASSET_C, os.O_RDWR | os.O_CREAT | os.O_TRUNC)
    os.write(fd, str)
    os.close(fd)

def writeResultJSON(str):
    fd = os.open('assets.js', os.O_RDWR | os.O_CREAT | os.O_TRUNC)
    os.write(fd, str)
    os.close(fd)

def genIncludes(files):
    str1 = ""
    for f in files:
        incf = copy.copy(f)
        incf = incf.replace(os.path.dirname(ASSETS_ROOT), ".")
        incf = incf.replace('\\', '/')
        incf = incf.replace('./', '')
        str1 += '#include "'+incf+'"\n'

    return str1


def gen_add_assets(files):
    result = ""
    for f in files:
        incf = copy.copy(f)
        basename = incf.replace(OUTPUT_DIR, '.')
        basename = basename.replace('\\', '/')
        basename = basename.replace('/fonts/', '/font/')
        basename = basename.replace('/images/', '/image/')
        basename = basename.replace('/styles/', '/style/')
        basename = basename.replace('./', '')
        basename = basename.replace('/', '_')
        basename = basename.replace('.data', '')
        basename = basename.replace('.bsvg', '')
        if basename == 'font_default':
            result += "#if defined(WITH_MINI_FONT) && (defined(WITH_STB_FONT) || defined(WITH_FT_FONT))\n"
            result += '  assets_manager_add(rm, font_default_mini);\n'
            result += "#else/*WITH_MINI_FONT*/\n"
            result += '  assets_manager_add(rm, font_default);\n'
            result += '#endif/*WITH_MINI_FONT*/\n'
        else:
            result += '  assets_manager_add(rm, '+basename+');\n'
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

    result += "#ifdef WITH_STB_IMAGE\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'images/*.res'))
    result += genIncludes(files)
    result += "#else\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'images/*.data'))
    result += genIncludes(files)
    result += '#endif/*WITH_STB_IMAGE*/\n'

    result += "#ifdef WITH_VGCANVAS\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'images/*.bsvg'))
    result += genIncludes(files)
    result += '#endif/*WITH_VGCANVAS*/\n'

    result += "#if defined(WITH_STB_FONT) || defined(WITH_FT_FONT)\n"
    result += "#if defined(WITH_MINI_FONT)\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'fonts/default_mini.res'))
    result += genIncludes(files)
    result += "#else/*WITH_MINI_FONT*/\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'fonts/default.res'))
    result += genIncludes(files)
    result += '#endif/*WITH_MINI_FONT*/\n'
    result += "#else/*WITH_STB_FONT or WITH_FT_FONT*/\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'fonts/*.data'))
    result += genIncludes(files)
    result += '#endif/*WITH_STB_FONT or WITH_FT_FONT*/\n'

    result += '#endif/*WITH_FS_RES*/\n'

    result += '\n'
    result += 'ret_t assets_init(void) {\n'
    result += '  assets_manager_t* rm = assets_manager();\n\n'
    result += ''

    result += '#ifdef WITH_FS_RES\n'
    result += "#if defined(WITH_MINI_FONT)\n"
    result += '  assets_manager_preload(rm, ASSET_TYPE_FONT, "default_mini");\n'
    result += "#else/*WITH_MINI_FONT*/\n"
    result += '  assets_manager_preload(rm, ASSET_TYPE_FONT, "default");\n'
    result += '#endif/*WITH_MINI_FONT*/\n'
    result += '  assets_manager_preload(rm, ASSET_TYPE_STYLE, "default");\n'
    result += '#else\n'

    files = glob.glob(joinPath(OUTPUT_DIR, '**/*.data'))
    result += gen_add_assets(files)

    result += "#ifdef WITH_VGCANVAS\n"
    files = glob.glob(joinPath(OUTPUT_DIR, 'images/*.bsvg'))
    result += gen_add_assets(files)
    result += '#endif/*WITH_VGCANVAS*/\n'

    result += '#endif\n'

    result += '\n'
    result += '  tk_init_assets();\n'
    result += '  return RET_OK;\n'
    result += '}\n'
    writeResult(result)


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
    result += '  assets_manager_t* rm = assets_manager();\n\n'
    result += ''

    result += gen_add_assets(files)

    result += '\n'
    result += '  tk_init_assets();\n'
    result += '  return RET_OK;\n'
    result += '}\n'

    global ASSET_C
    ASSET_C = ASSET_C.replace('.c', '_web.c');
    writeResult(result)

def gen_res_json_one(res_type, files):
    result= "\n  " + res_type + ': [\n'
    for f in files:
        uri = f.replace(os.getcwd(), "")[1:]
        filename, extname = os.path.splitext(uri)
        basename = os.path.basename(filename)
        result = result + '    {name:"' + basename + '\", uri:"' + uri;
        if res_type == 'image' and extname != '.svg' and extname != '.bsvg':
            img = Image.open(f)
            w, h = img.size
            result = result + '", w:' + str(w) + ', h:' + str(h)+ '},\n';
        else:
            result = result + '"},\n';
    result = result + '  ],'

    return result;

def gen_res_json():
    result = 'const g_awtk_assets = {';

    result = result + gen_res_json_one("image", glob.glob(joinPath(INPUT_DIR, 'images/*/*.*')));
    result = result + gen_res_json_one("ui", glob.glob(joinPath(INPUT_DIR, 'ui/*.bin')));
    result = result + gen_res_json_one("style", glob.glob(joinPath(INPUT_DIR, 'styles/*.bin')));
    result = result + gen_res_json_one("string", glob.glob(joinPath(INPUT_DIR, 'strings/*.bin')));
    result = result + gen_res_json_one("xml", glob.glob(joinPath(INPUT_DIR, 'xml/*.xml')));
    result = result + gen_res_json_one("data", glob.glob(joinPath(INPUT_DIR, 'data/*.*')));
    result = result + gen_res_json_one("script", glob.glob(joinPath(INPUT_DIR, 'scripts/*.*')));
    result = result + gen_res_json_one("font", glob.glob(joinPath(INPUT_DIR, 'fonts/*.ttf')));
    result = result + '\n};';

    global ASSET_C
    ASSET_C = ASSET_C.replace('.c', '_web.js');
    writeResult(result);

def gen_res():
    prepare()
    gen_res_all()
    gen_res_c()


def init(awtk_root, assets_root, asset_c):
    global DPI
    global ASSET_C
    global BIN_DIR
    global ASSETS_ROOT
    global AWTK_ROOT
    global INPUT_DIR
    global OUTPUT_DIR
    global IMAGEGEN_OPTIONS

    ASSET_C = asset_c
    AWTK_ROOT = awtk_root
    ASSETS_ROOT = assets_root

    BIN_DIR = joinPath(AWTK_ROOT, 'bin')
    INPUT_DIR = joinPath(ASSETS_ROOT, 'raw')
    OUTPUT_DIR = joinPath(ASSETS_ROOT, 'inc')


def dumpArgs():
    print('ASSETS_ROOT='+ASSETS_ROOT)
    print('AWTK_ROOT='+AWTK_ROOT)
    print('INPUT_DIR='+INPUT_DIR)
    print('OUTPUT_DIR='+OUTPUT_DIR)
    print('ASSET_C='+ASSET_C)
    print('DPI='+DPI)
    print('IMAGEGEN_OPTIONS='+IMAGEGEN_OPTIONS)
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
    args = ' action[clean|web|json|all|font|image|ui|style|string|script|data|xml] dpi[x1|x2] image_options[rgba|bgra+bgr565]'
    if len(sys.argv) == 1:
        print('=========================================================')
        print('Usage: '+sys.argv[0] + args)
        print('Example:')
        print(sys.argv[0] + ' all')
        print(sys.argv[0] + ' clean')
        print(sys.argv[0] + ' style')
        print(sys.argv[0] + ' all x1 bgra+bgr565')
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


showUsage()
