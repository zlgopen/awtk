import os
import platform

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

OS_NAME=platform.system()
TK_ROOT = os.path.normpath(os.getcwd())
TK_SRC = joinPath(TK_ROOT, 'src')
TK_3RD_ROOT = joinPath(TK_ROOT, '3rd')
TK_TOOLS_ROOT = joinPath(TK_ROOT, 'tools')
GTEST_ROOT = joinPath(TK_ROOT, '3rd/gtest/googletest')
BIN_DIR=joinPath(TK_ROOT, 'bin')
LIB_DIR=joinPath(TK_ROOT, 'lib')

if OS_NAME == 'Windows':
  TK_ROOT=TK_ROOT.replace('\\', '\\\\');

INPUT_ENGINE='null'
INPUT_ENGINE='pinyin'

LCD='SDL'
LCD='GL'

VGCANVAS='NANOVG'

FRAME_BUFFER_FORMAT='bgra8888'
FRAME_BUFFER_FORMAT='bgr565'
FRAME_BUFFER_FORMAT='bgr888'

COMMON_CCFLAGS=' -DTK_ROOT=\\\"'+TK_ROOT+'\\\" -DHAS_STD_MALLOC -DWITH_SDL -DWITH_FS_RES -DHAS_STDIO -DWITH_DESKTOP_STYLE '
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DSTBTT_STATIC -DSTB_IMAGE_STATIC -DWITH_STB_IMAGE -DWITH_STB_FONT -DWITH_VGCANVAS -DWITH_UNICODE_BREAK '

if FRAME_BUFFER_FORMAT=='bgra8888':
  COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FB_BGRA8888=1';
elif FRAME_BUFFER_FORMAT=='bgr888':
  COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FB_BGR888=1';
else:
  COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FB_BGR565=1 -DWITHOUT_BITMAP_RGB565';

if LCD == 'GL':
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG -DWITH_NANOVG_GL3 -DWITH_NANOVG_GL -DWITH_VGCANVAS_LCD'
else:
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_BITMAP_BGRA1 -DWITH_NANOVG_AGGE -DWITH_NANOVG_SOFT'

os.environ['LCD'] = LCD
os.environ['VGCANVAS'] = VGCANVAS 
os.environ['BIN_DIR'] = BIN_DIR;
os.environ['LIB_DIR'] = LIB_DIR;
os.environ['TK_ROOT'] = TK_ROOT;
os.environ['GTEST_ROOT'] = GTEST_ROOT;
os.environ['INPUT_ENGINE'] = INPUT_ENGINE;
os.environ['FRAME_BUFFER_FORMAT'] = FRAME_BUFFER_FORMAT;

OS_LIBS=[]
OS_LIBPATH=[]
OS_CPPPATH=[]
OS_LINKFLAGS=''
OS_FLAGS='-g -Wall'
OS_SUBSYSTEM_CONSOLE=''
OS_SUBSYSTEM_WINDOWS=''

if OS_NAME == 'Darwin':
  OS_LINKFLAGS='-framework OpenGL'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -D__APPLE__ -DHAS_PTHREAD -DMACOS -DENABLE_MEM_LEAK_CHECK '
  OS_LIBS = OS_LIBS + ['stdc++', 'pthread', 'm', 'dl']

elif OS_NAME == 'Linux':
  OS_LIBS = ['GL'] + OS_LIBS + ['stdc++', 'pthread', 'm', 'dl']
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DLINUX -DHAS_PTHREAD'

elif OS_NAME == 'Windows':
  OS_LIBS=['gdi32', 'user32']
  OS_FLAGS='-DWIN32 -D_WIN32 -DWINDOWS /EHsc -D_CONSOLE  /DEBUG /Od /ZI'
  OS_LINKFLAGS='/MACHINE:X64 /DEBUG'
  OS_LIBPATH=[TK_3RD_ROOT+'/SDL2-2.0.7/lib/x64']
  OS_CPPPATH=[TK_3RD_ROOT+'/SDL2-2.0.7/']
  OS_SUBSYSTEM_CONSOLE='/SUBSYSTEM:CONSOLE  '
  OS_SUBSYSTEM_WINDOWS='/SUBSYSTEM:WINDOWS  '
  
LINKFLAGS=OS_LINKFLAGS;
LIBPATH=[LIB_DIR] + OS_LIBPATH
CCFLAGS=OS_FLAGS + COMMON_CCFLAGS 
LIBS=['awtk', 'gpinyin', 'awtk', 'agge', 'linebreak', 'nanovg', 'SDL2', 'glad'] + OS_LIBS

CPPPATH=[TK_ROOT, 
  TK_SRC, 
  TK_3RD_ROOT, 
  joinPath(TK_SRC, 'ext_widgets'), 
  joinPath(TK_3RD_ROOT, 'nanovg/src'), 
  joinPath(TK_3RD_ROOT, 'agge/src'), 
  joinPath(TK_3RD_ROOT, 'agge/include'), 
  joinPath(TK_3RD_ROOT, 'gpinyin/include'), 
  joinPath(TK_3RD_ROOT, 'libunibreak/src'), 
  TK_TOOLS_ROOT] + OS_CPPPATH

DefaultEnvironment(CCFLAGS = CCFLAGS, 
  LIBS = LIBS,
  LIBPATH = LIBPATH,
  CPPPATH = CPPPATH,
  LINKFLAGS = LINKFLAGS,
  OS_SUBSYSTEM_CONSOLE=OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=OS_SUBSYSTEM_WINDOWS
)

SConscriptFiles=[
  '3rd/agge/SConscript',
  '3rd/nanovg/SConscript',
  '3rd/glad/SConscript',
  '3rd/gpinyin/SConscript', 
  '3rd/libunibreak/SConscript',
  'src/SConscript',
  'tools/common/SConscript', 
  'tools/theme_gen/SConscript', 
  'tools/font_gen/SConscript', 
  'tools/image_gen/SConscript', 
  'tools/image_resize/SConscript', 
  'tools/res_gen/SConscript', 
  'tools/str_gen/SConscript', 
  'tools/ui_gen/qt_to_xml/SConscript',
  'tools/ui_gen/xml_to_ui/SConscript',
  'demos/SConscript', 
  'tests/SConscript'
  ]
  
SConscript(SConscriptFiles)

