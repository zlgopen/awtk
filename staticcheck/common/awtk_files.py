import os
import copy
import glob
import subprocess

def joinPath(root, subdir):
  return os.path.abspath(os.path.normpath(os.path.join(root, subdir)))

if 'AWTK_ROOT_DIR' in os.environ:
    AWTK_ROOT_DIR=os.path.abspath(os.environ['AWTK_ROOT_DIR']);
else:
    AWTK_ROOT_DIR=os.path.abspath('../../');

AWTK_SRC_DIR=joinPath(AWTK_ROOT_DIR, 'src');

AWKT_TKC_FILES=glob.glob(AWTK_SRC_DIR+'/tkc/*.c')
AWKT_BASE_FILES=glob.glob(AWTK_SRC_DIR+'/base/*.c')
AWKT_WIDGETS_FILES=glob.glob(AWTK_SRC_DIR+'/widgets/*.c')
AWKT_WIDGETS_FILES=glob.glob(AWTK_SRC_DIR+'/widgets/*.c')
AWKT_EXT_WIDGETS_FILES=glob.glob(AWTK_SRC_DIR+'/ext_widgets/*.c') + glob.glob(AWTK_SRC_DIR+'/ext_widgets/*/*.c')
AWKT_UILOADER_FILES=glob.glob(AWTK_SRC_DIR+'/ui_loader/*.c')
AWKT_LAYOUTERS_FILES=glob.glob(AWTK_SRC_DIR+'/layouters/*.c')
AWKT_XML_FILES=glob.glob(AWTK_SRC_DIR+'/xml/*.c')
AWKT_SVG_FILES=glob.glob(AWTK_SRC_DIR+'/svg/*.c')
AWKT_UBJSON_FILES=glob.glob(AWTK_SRC_DIR+'/ubjson/*.c')
AWKT_CLIPBOARD_FILES=glob.glob(AWTK_SRC_DIR+'/clip_board/clip_board_default.c')
AWKT_WIDGET_ANIMATORS_FILES=glob.glob(AWTK_SRC_DIR+'/widget_animators/*.c')
AWKT_WINDOW_ANIMATORS_FILES=glob.glob(AWTK_SRC_DIR+'/window_animators/*.c')
AWKT_DIALOG_HIGHLIGHTERS_FILES=glob.glob(AWTK_SRC_DIR+'/dialog_highlighters/*.c')
AWKT_FONT_LOADER_FILES=glob.glob(AWTK_SRC_DIR+'/font_loader/*.c')
AWKT_IMAGE_LOADER_FILES=glob.glob(AWTK_SRC_DIR+'/image_loader/*.c')
AWKT_DESIGNER_SUPPORT_FILES=glob.glob(AWTK_SRC_DIR+'/designer_support/*.c')
AWKT_INPUT_METHOD_FILES=glob.glob(AWTK_SRC_DIR+'/input_methods/input_method_creator.c')
AWKT_FONT_GLOBAL_FILES=glob.glob(AWTK_SRC_DIR+'/*.c')
NATIVE_WINDOW_FILES=glob.glob(AWTK_SRC_DIR+'/native_window/native_window_raw.c')
WINDOW_MANAGER_FILES=glob.glob(AWTK_SRC_DIR+'/window_manager/window_manager_default.c')
GRAPHIC_BUFFER_FILES=glob.glob(AWTK_SRC_DIR+'/graphic_buffer/graphic_buffer_default.c')

COMMON_FILES=AWKT_TKC_FILES + AWKT_BASE_FILES + AWKT_WIDGETS_FILES + AWKT_EXT_WIDGETS_FILES + AWKT_UILOADER_FILES + AWKT_LAYOUTERS_FILES + AWKT_SVG_FILES + AWKT_WIDGET_ANIMATORS_FILES + AWKT_WINDOW_ANIMATORS_FILES + AWKT_DIALOG_HIGHLIGHTERS_FILES + AWKT_CLIPBOARD_FILES + AWKT_FONT_GLOBAL_FILES + AWKT_INPUT_METHOD_FILES + NATIVE_WINDOW_FILES + WINDOW_MANAGER_FILES + GRAPHIC_BUFFER_FILES;

INFER_FILES=COMMON_FILES
WEB_FILES=COMMON_FILES + AWKT_XML_FILES + AWKT_DESIGNER_SUPPORT_FILES
CPPCHECK_FILES=COMMON_FILES + AWKT_XML_FILES + AWKT_FONT_LOADER_FILES + AWKT_IMAGE_LOADER_FILES

def getCppCheckFiles():
    return CPPCHECK_FILES;

def getInferFiles():
    return INFER_FILES;

def getWebFiles():
    return WEB_FILES;

def getIncludes():
    return '-I' + AWTK_ROOT_DIR + '/3rd ' + '-I' + AWTK_ROOT_DIR + ' -I' + AWTK_SRC_DIR +' -I' + AWTK_SRC_DIR +'/ext_widgets';

def toExe(name):
    if OS_NAME == 'Windows':
        return name + '.exe'
    else:
        return name

def writeArgs(filename, str):
    with open(filename, "w") as text_file:
        text_file.write(str);
	
def runArgsInFile(cmd, flags, files):
    cmd_args = flags + ' ' + getIncludes() + ' ' + ' '.join(files) 
    cmd_args = cmd_args.replace('\\', '\\\\');
    writeArgs("args.txt", cmd_args);
    print(cmd_args)
    os.system(cmd + ' @args.txt');

def run(cmd, flags, files):
    cmd_args = cmd + ' ' + flags + ' ' + getIncludes() + ' ' + ' '.join(files) 
    cmd_args = cmd_args.replace('\\', '\\\\');
    print(cmd_args)
    os.system(cmd_args);

