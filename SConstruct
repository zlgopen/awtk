import os
import platform

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

OS_NAME=platform.system()
TK_ROOT = os.path.normpath(os.getcwd())
BIN_DIR=joinPath(TK_ROOT, 'bin')
LIB_DIR=joinPath(TK_ROOT, 'lib')
TK_SRC = joinPath(TK_ROOT, 'src')
TK_3RD_ROOT = joinPath(TK_ROOT, '3rd')
TK_TOOLS_ROOT = joinPath(TK_ROOT, 'tools')
GTEST_ROOT = joinPath(TK_ROOT, '3rd/gtest/googletest')

if OS_NAME == 'Windows':
  TK_ROOT=TK_ROOT.replace('\\', '\\\\');

#INPUT_ENGINE='null'
INPUT_ENGINE='pinyin'

VGCANVAS='NANOVG'
NANOVG_BACKEND='GL3'
#NANOVG_BACKEND='GLES2'
#NANOVG_BACKEND='GLES3'
#NANOVG_BACKEND='AGG'
#NANOVG_BACKEND='AGGE'
#NANOVG_BACKEND='BGFX'

FRAME_BUFFER_FORMAT=''
if NANOVG_BACKEND == 'AGGE' or NANOVG_BACKEND == 'AGG':
  LCD='SDL_FB'
  FRAME_BUFFER_FORMAT='bgr565'
  #FRAME_BUFFER_FORMAT='bgra8888'
else:  
  LCD='SDL_GPU'

NANOVG_BACKEND_LIBS=[];
NANOVG_BACKEND_PROJS=[];

COMMON_CCFLAGS=' -DTK_ROOT=\\\"'+TK_ROOT+'\\\" ' 

COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_STB_FONT '
#COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FT_FONT '

COMMON_CCFLAGS=COMMON_CCFLAGS+' -DSTBTT_STATIC -DSTB_IMAGE_STATIC -DWITH_STB_IMAGE '
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_VGCANVAS -DWITH_UNICODE_BREAK -DWITH_DESKTOP_STYLE '
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DSDL2 -DHAS_STD_MALLOC -DWITH_SDL -DWITH_FS_RES -DHAS_STDIO '

if LCD == 'SDL_GPU':
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_GPU -DWITH_VGCANVAS_LCD'
else:
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_BITMAP_BGRA -DWITH_NANOVG_SOFT '
  if FRAME_BUFFER_FORMAT=='bgra8888':
    COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FB_BGRA8888=1 ';
  else:
    COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FB_BGR565=1';

if NANOVG_BACKEND == 'AGG':
  NANOVG_BACKEND_LIBS=['nanovg-agg', 'agg'];
  NANOVG_BACKEND_PROJS=['3rd/agg/SConscript'];
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_AGG '
elif NANOVG_BACKEND == 'AGGE':
  NANOVG_BACKEND_LIBS=['nanovg-agge', 'agge'];
  NANOVG_BACKEND_PROJS=['3rd/agge/SConscript'];
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_AGGE '
elif NANOVG_BACKEND == 'BGFX':
  NANOVG_BACKEND_LIBS=['nanovg-bgfx','bgfx'];
  NANOVG_BACKEND_PROJS=['3rd/bgfx/SConscript'];
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_BGFX '
elif NANOVG_BACKEND == 'GLES2':
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_GLES2 -DWITH_NANOVG_GL '
elif NANOVG_BACKEND == 'GLES3':
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_GLES3 -DWITH_NANOVG_GL '
else:
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_GL3 -DWITH_NANOVG_GL '

OS_LIBS=[]
OS_LIBPATH=[]
OS_CPPPATH=[]
OS_LINKFLAGS=''
OS_FLAGS='-g -Wall'
OS_SUBSYSTEM_CONSOLE=''
OS_SUBSYSTEM_WINDOWS=''
OS_PROJECTS=[]

if OS_NAME == 'Darwin':
  OS_LIBS = ['stdc++', 'pthread', 'm', 'dl']
  OS_LINKFLAGS='-framework Cocoa -framework QuartzCore -framework OpenGL -weak_framework Metal -weak_framework MetalKit'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_WIDGET_POOL=1000 '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -D__APPLE__ -DHAS_PTHREAD -DMACOS -DENABLE_MEM_LEAK_CHECK '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS  -DBGFX_CONFIG_RENDERER_METAL=1 '

elif OS_NAME == 'Linux':
  OS_LIBS = ['GL', 'gtk-3','gdk-3','Xext', 'X11', 'sndio','stdc++', 'pthread', 'm', 'dl']
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DLINUX -DHAS_PTHREAD'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_REAL_API -DSDL_TIMER_UNIX -DSDL_VIDEO_DRIVER_X11 -DSDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_AUDIO_DRIVER_SNDIO -DSDL_VIDEO_OPENGL_GLX -DSDL_VIDEO_RENDER_OGL '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_LOADSO_DLOPEN -DSDL_VIDEO_OPENGL_EGL -DSDL_VIDEO_OPENGL_ES2 '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_REAL_API -DSDL_HAPTIC_DISABLED -DSDL_SENSOR_DISABLED -DSDL_JOYSTICK_DISABLED '
  OS_PROJECTS=['3rd/SDL/SConscript']

elif OS_NAME == 'Windows':
  OS_LIBS=['gdi32', 'user32','winmm.lib','imm32.lib','version.lib','shell32.lib','ole32.lib','Oleaut32.lib','Advapi32.lib','DelayImp.lib','psapi.lib']
  OS_FLAGS='-DWIN32 -D_WIN32 -DWINDOWS /EHsc -D_CONSOLE  /DEBUG /Od  /FS /Z7 '
  #OS_FLAGS='-DWIN32 -D_WIN32 -DWINDOWS /EHsc -D_CONSOLE  /DEBUG /Od  /FS /Z7 -D_DEBUG /MDd '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_REAL_API -DSDL_HAPTIC_DISABLED -DSDL_SENSOR_DISABLED -DSDL_JOYSTICK_DISABLED '
  COMMON_CCFLAGS = COMMON_CCFLAGS + '-D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_HAS_EXCEPTIONS=0 -D_HAS_ITERATOR_DEBUGGING=0 -D_ITERATOR_DEBUG_LEVEL=0 -D_SCL_SECURE=0'
  COMMON_CCFLAGS = COMMON_CCFLAGS + '-D_SECURE_SCL=0 -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_WIN64'
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
  joinPath(TK_3RD_ROOT, 'bgfx/bgfx/include'), 
  joinPath(TK_3RD_ROOT, 'bgfx/bx/include'), 
  joinPath(TK_3RD_ROOT, 'bgfx/bimg/include'), 
  joinPath(TK_3RD_ROOT, 'agge'), 
  joinPath(TK_3RD_ROOT, 'agg/include'), 
  joinPath(TK_3RD_ROOT, 'nanovg'), 
  joinPath(TK_3RD_ROOT, 'nanovg/gl'), 
  joinPath(TK_3RD_ROOT, 'nanovg/base'), 
  joinPath(TK_3RD_ROOT, 'nanovg/agge'), 
  joinPath(TK_3RD_ROOT, 'nanovg/bgfx'), 
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

os.environ['LCD'] = LCD
os.environ['BIN_DIR'] = BIN_DIR;
os.environ['LIB_DIR'] = LIB_DIR;
os.environ['TK_ROOT'] = TK_ROOT;
os.environ['CCFLAGS'] = CCFLAGS;
os.environ['VGCANVAS'] = VGCANVAS 
os.environ['GTEST_ROOT'] = GTEST_ROOT;
os.environ['TK_3RD_ROOT'] = TK_3RD_ROOT;
os.environ['INPUT_ENGINE'] = INPUT_ENGINE;
os.environ['NANOVG_BACKEND'] = NANOVG_BACKEND;
os.environ['FRAME_BUFFER_FORMAT'] = FRAME_BUFFER_FORMAT;

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
  'tools/svg_gen/SConscript',
  'demos/SConscript', 
  'tests/SConscript'
  ] + OS_PROJECTS
  
SConscript(SConscriptFiles)

