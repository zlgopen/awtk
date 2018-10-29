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

#INPUT_ENGINE='null'
INPUT_ENGINE='pinyin'

LCD='SDL'
LCD='GL'

VGCANVAS='NANOVG'
#NANOVG_BACKEND='AGGE'
#NANOVG_BACKEND='BGFX'
#NANOVG_BACKEND='GL3'
#NANOVG_BACKEND='GLES'
#NANOVG_BACKEND='AGG'
NANOVG_BACKEND='AGGE'

#FRAME_BUFFER_FORMAT='bgr888'
FRAME_BUFFER_FORMAT='bgr565'
FRAME_BUFFER_FORMAT='bgra8888'

COMMON_CCFLAGS=' -DTK_ROOT=\\\"'+TK_ROOT+'\\\" -DHAS_STD_MALLOC -DWITH_SDL -DWITH_FS_RES -DHAS_STDIO -DWITH_DESKTOP_STYLE '
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DSTBTT_STATIC -DSTB_IMAGE_STATIC -DWITH_STB_IMAGE -DWITH_STB_FONT -DWITH_VGCANVAS -DWITH_UNICODE_BREAK '

NANOVG_BACKEND_LIBS=[];
NANOVG_BACKEND_PROJS=[];

if LCD == 'GL':
  NANOVG_BACKEND='GL'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_GL -DWITH_VGCANVAS_LCD'
else:
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_BITMAP_BGRA -DWITH_NANOVG_SOFT '
  if FRAME_BUFFER_FORMAT=='bgra8888':
    COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FB_BGRA8888=1 ';
  elif FRAME_BUFFER_FORMAT=='bgr888':
    COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FB_BGR888=1 ';
  else:
    COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FB_BGR565=1 ';

if NANOVG_BACKEND == 'AGG':
  NANOVG_BACKEND_LIBS=['nanovg-agg', 'agg'];
  NANOVG_BACKEND_PROJS=['3rd/agg/SConscript'];
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_AGG '
elif NANOVG_BACKEND == 'AGGE':
  NANOVG_BACKEND_LIBS=['nanovg-agge', 'agge'];
  NANOVG_BACKEND_PROJS=['3rd/agge/SConscript'];
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_AGGE -DWITH_BITMAP_BGR565 '
elif NANOVG_BACKEND == 'BGFX':
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_BGFX '
elif NANOVG_BACKEND == 'GLES':
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_GLES '
else:
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_GL3 '

os.environ['LCD'] = LCD
os.environ['VGCANVAS'] = VGCANVAS 
os.environ['BIN_DIR'] = BIN_DIR;
os.environ['LIB_DIR'] = LIB_DIR;
os.environ['TK_ROOT'] = TK_ROOT;
os.environ['GTEST_ROOT'] = GTEST_ROOT;
os.environ['INPUT_ENGINE'] = INPUT_ENGINE;
os.environ['NANOVG_BACKEND'] = NANOVG_BACKEND;
os.environ['FRAME_BUFFER_FORMAT'] = FRAME_BUFFER_FORMAT;

OS_LIBS=[]
OS_LIBPATH=[]
OS_CPPPATH=[]
OS_LINKFLAGS=''
OS_FLAGS='-g -Wall'
OS_SUBSYSTEM_CONSOLE=''
OS_SUBSYSTEM_WINDOWS=''
OS_PROJECTS=[]

if OS_NAME == 'Darwin':
  OS_LINKFLAGS='-framework OpenGL'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -D__APPLE__ -DHAS_PTHREAD -DMACOS -DENABLE_MEM_LEAK_CHECK1 '
  COMMON_CCFLAGS=COMMON_CCFLAGS+' -DENABLE_CURSOR '
  OS_LIBS = OS_LIBS + ['stdc++', 'pthread', 'm', 'dl']

elif OS_NAME == 'Linux':
  OS_LIBS = ['GL'] + OS_LIBS + ['gtk-3','gdk-3','Xext', 'X11', 'sndio','stdc++', 'pthread', 'm', 'dl']
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DLINUX -DHAS_PTHREAD'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_REAL_API -DSDL_TIMER_UNIX -DSDL_VIDEO_DRIVER_X11 -DSDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_AUDIO_DRIVER_SNDIO -DSDL_VIDEO_OPENGL_GLX -DSDL_VIDEO_RENDER_OGL '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_LOADSO_DLOPEN -DSDL_VIDEO_OPENGL_EGL -DSDL_VIDEO_OPENGL_ES2 '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_REAL_API -DSDL_HAPTIC_DISABLED -DSDL_SENSOR_DISABLED -DSDL_JOYSTICK_DISABLED '
  OS_PROJECTS=['3rd/SDL/SConscript']

elif OS_NAME == 'Windows':
  OS_LIBS=['gdi32', 'user32','winmm.lib','imm32.lib','version.lib','shell32.lib','ole32.lib','Oleaut32.lib','Advapi32.lib']
  OS_FLAGS='-DWIN32 -D_WIN32 -DWINDOWS /EHsc -D_CONSOLE  /DEBUG /Od  /FS /Z7 '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_REAL_API -DSDL_HAPTIC_DISABLED -DSDL_SENSOR_DISABLED -DSDL_JOYSTICK_DISABLED '
  OS_LINKFLAGS='/MACHINE:X64 /DEBUG'
  OS_SUBSYSTEM_CONSOLE='/SUBSYSTEM:CONSOLE  '
  OS_SUBSYSTEM_WINDOWS='/SUBSYSTEM:WINDOWS  '
  OS_PROJECTS=['3rd/SDL/SConscript']

LINKFLAGS=OS_LINKFLAGS;
LIBPATH=[LIB_DIR] + OS_LIBPATH
CCFLAGS=OS_FLAGS + COMMON_CCFLAGS 
LIBS=['awtk', 'gpinyin', 'awtk', 'linebreak'] + NANOVG_BACKEND_LIBS + ['nanovg', 'SDL2', 'glad'] + OS_LIBS

CPPPATH=[TK_ROOT, 
  TK_SRC, 
  TK_3RD_ROOT, 
  joinPath(TK_SRC, 'ext_widgets'), 
  joinPath(TK_3RD_ROOT, 'agge'), 
  joinPath(TK_3RD_ROOT, 'agg/include'), 
  joinPath(TK_3RD_ROOT, 'nanovg'), 
  joinPath(TK_3RD_ROOT, 'nanovg/base'), 
  joinPath(TK_3RD_ROOT, 'nanovg/gl'), 
  joinPath(TK_3RD_ROOT, 'nanovg/agge'), 
  joinPath(TK_3RD_ROOT, 'SDL/src'), 
  joinPath(TK_3RD_ROOT, 'SDL/include'), 
  joinPath(TK_3RD_ROOT, 'agge/src'), 
  joinPath(TK_3RD_ROOT, 'agge/include'), 
  joinPath(TK_3RD_ROOT, 'gpinyin/include'), 
  joinPath(TK_3RD_ROOT, 'libunibreak'), 
  TK_TOOLS_ROOT] + OS_CPPPATH

DefaultEnvironment(CCFLAGS = CCFLAGS, 
  LIBS = LIBS,
  LIBPATH = LIBPATH,
  CPPPATH = CPPPATH,
  LINKFLAGS = LINKFLAGS,
  OS_SUBSYSTEM_CONSOLE=OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=OS_SUBSYSTEM_WINDOWS
)

SConscriptFiles=NANOVG_BACKEND_PROJS + [
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
  ] + OS_PROJECTS
  
SConscript(SConscriptFiles)

