import os
import sys
import json
import shutil
import platform
from SCons import Script

PLATFORM = platform.system()

def getTkcOnly():
    env = os.environ
    if 'TKC_ONLY' in env:
        return env['TKC_ONLY'] == 'True'
    else:
        return False

def join_path(root, sub):
    return os.path.abspath(os.path.join(root, sub))


def mkdir_if_not_exist(fullpath):
    if os.path.exists(fullpath):
        print(fullpath+' exist.')
    else:
        os.makedirs(fullpath)


def load_project_json(filename):
    try:
        if sys.version_info >= (3, 0):
            with open(filename, 'r', encoding='utf8') as f:
                info = json.load(f)
                return info
        else:
            with open(filename, 'r') as f:
                info = json.load(f)
                return info
    except:
        return None


def get_project_w(info, theme):
    return info['assets']['themes'][theme]['lcd']['width']


def get_project_h(info, theme):
    return info['assets']['themes'][theme]['lcd']['height']


def get_project_theme(info):
    return info['assets']['activedTheme']


def get_project_language(info):
    return info['assets']['defaultLanguage']


def get_project_country(info):
    return info['assets']['defaultCountry']

def get_project_res_root(info):
    res_root = info['assets']['outputDir']
    if os.path.isabs(res_root):
        return res_root
    else: 
        return '../' + res_root

class AppHelperBase:
    def set_deps(self, DEPENDS_LIBS):
        self.DEPENDS_LIBS = DEPENDS_LIBS
        return self

    def set_src_dir(self, SRC_DIR):
        self.SRC_DIR = SRC_DIR
        return self

    def set_tkc_only(self):
        self.AWTK_LIBS = ['tkc']
        self.TKC_ONLY = True

        return self

    def set_libs(self, APP_LIBS):
        self.APP_LIBS = APP_LIBS
        return self

    def set_dll_def(self, DEF_FILE):
        self.DEF_FILE = DEF_FILE
        return self

    def set_dll_def_processor(self, processor):
        self.DEF_FILE_PROCESSOR = processor
        return self

    def set_ccflags(self, APP_CCFLAGS):
        self.APP_CCFLAGS = APP_CCFLAGS
        return self

    def set_cxxflags(self, APP_CXXFLAGS):
        self.APP_CXXFLAGS = APP_CXXFLAGS
        return self

    def add_deps(self, DEPENDS_LIBS):
        self.DEPENDS_LIBS += DEPENDS_LIBS
        return self

    def add_libs(self, APP_LIBS):
        self.APP_LIBS += APP_LIBS
        return self

    def add_platform_libs(self, plat, PLATFORM_LIBS):
        if plat == PLATFORM:
            self.PLATFORM_LIBS += PLATFORM_LIBS
        return self

    def add_libpath(self, APP_LIBPATH):
        self.APP_LIBPATH += APP_LIBPATH
        return self

    def add_platform_libpath(self, plat, APP_LIBPATH):
        if plat == PLATFORM:
            self.APP_LIBPATH += APP_LIBPATH
        return self

    def add_cpppath(self, APP_CPPPATH):
        self.APP_CPPPATH += APP_CPPPATH
        return self

    def add_platform_cpppath(self, plat, APP_CPPPATH):
        if plat == PLATFORM:
            self.APP_CPPPATH += APP_CPPPATH
        return self

    def add_ccflags(self, APP_CCFLAGS):
        self.APP_CCFLAGS += APP_CCFLAGS
        return self

    def add_cflags(self, APP_CFLAGS):
        self.APP_CFLAGS += APP_CFLAGS
        return self

    def add_platform_ccflags(self, plat, APP_CCFLAGS):
        if plat == PLATFORM:
            self.APP_CCFLAGS += APP_CCFLAGS
        return self

    def use_std_cxx(self, VERSION):
      if platform.system() == 'Windows':
        self.APP_CXXFLAGS += ' /std:c++'+str(VERSION)+' '
      else:
        self.APP_CXXFLAGS += ' -std=c++'+str(VERSION)+' '

    def add_cxxflags(self, APP_CXXFLAGS):
        self.APP_CXXFLAGS += APP_CXXFLAGS
        return self

    def add_platform_cxxflags(self, plat, APP_CXXFLAGS):
        if plat == PLATFORM:
            self.APP_CXXFLAGS += APP_CXXFLAGS
        return self

    def add_linkflags(self, APP_LINKFLAGS):
        self.APP_LINKFLAGS += APP_LINKFLAGS
        return self

    def add_platform_linkflags(self, plat, APP_LINKFLAGS):
        if plat == PLATFORM:
            self.APP_LINKFLAGS += APP_LINKFLAGS
        return self
        
    def SConscript(self, SConscriptFiles):
        if not self.BUILD_DIR:
            Script.SConscript(SConscriptFiles)
        else:
            for sc in SConscriptFiles:
                dir = os.path.dirname(sc)
                build_dir = os.path.join(self.BUILD_DIR, dir)
                Script.SConscript(sc, variant_dir=build_dir, duplicate=False)

    def __init__(self, ARGUMENTS):
        APP_ROOT = os.path.normpath(os.getcwd())

        self.SRC_DIR = 'src'
        self.TKC_ONLY = getTkcOnly();
        self.ARGUMENTS = ARGUMENTS
        self.DEF_FILE = None
        self.DEF_FILE_PROCESSOR = None
        self.DEPENDS_LIBS = []
        self.GEN_IDL_DEF = True
        self.BUILD_SHARED = True
        self.LINUX_FB = ARGUMENTS.get('LINUX_FB', '') == 'true'
        self.AWTK_ROOT = self.getAwtkRoot()
        self.awtk = self.getAwtkConfig()
        self.AWTK_LIBS = self.awtk.LIBS
        self.AWTK_CFLAGS = self.awtk.CFLAGS
        self.AWTK_CCFLAGS = self.awtk.CCFLAGS
        self.APP_ROOT = APP_ROOT
        self.BUILD_DIR = ARGUMENTS.get('BUILD_DIR', '')
        self.BIN_DIR = os.path.join(self.BUILD_DIR, 'bin')
        self.LIB_DIR = os.path.join(self.BUILD_DIR, 'lib')
        self.APP_BIN_DIR = os.path.join(APP_ROOT, self.BIN_DIR)
        self.APP_LIB_DIR = os.path.join(APP_ROOT, self.LIB_DIR)
        self.APP_SRC = os.path.join(APP_ROOT, 'src')
        self.APP_RES = os.path.join(APP_ROOT, 'res')
        self.APP_LIBS = []
        self.APP_LIBPATH = [self.APP_LIB_DIR, self.APP_BIN_DIR]
        self.APP_LINKFLAGS = ''
        self.APP_CPPPATH = [self.APP_SRC, self.APP_RES]
        self.PLATFORM_LIBS = []
        self.APP_TOOLS = None
        self.WITH_JERRYSCRIPT = False
        self.MVVM_ROOT = None
        self.AWFLOW_ROOT = None

        mkdir_if_not_exist(self.APP_BIN_DIR)
        mkdir_if_not_exist(self.APP_LIB_DIR)

        os.environ['APP_SRC'] = self.APP_SRC
        os.environ['APP_ROOT'] = self.APP_ROOT
        os.environ['BIN_DIR'] = self.APP_BIN_DIR
        os.environ['LIB_DIR'] = self.APP_LIB_DIR
        os.environ['LINUX_FB'] = 'false'
        os.environ['BUILD_DIR'] = self.BUILD_DIR
        if self.LINUX_FB:
            os.environ['LINUX_FB'] = 'true'

        self.WITH_JERRYSCRIPT = ARGUMENTS.get('WITH_JERRYSCRIPT', '').lower().startswith('t')

        WITH_MVVM = ARGUMENTS.get('WITH_MVVM', '').lower().startswith('t')
        MVVM_ROOT = ARGUMENTS.get('MVVM_ROOT', '')
        if WITH_MVVM or os.path.exists(MVVM_ROOT):
            os.environ['WITH_MVVM'] = 'true'
            if not os.path.exists(MVVM_ROOT):
                MVVM_ROOT = self.getMvvmRoot()
            self.MVVM_ROOT = MVVM_ROOT
            print("MVVM_ROOT: " + self.MVVM_ROOT)

        WITH_AWFLOW = ARGUMENTS.get('WITH_AWFLOW', '').lower().startswith('t')
        AWFLOW_ROOT = ARGUMENTS.get('AWFLOW_ROOT', '')
        print(WITH_AWFLOW)
        if WITH_AWFLOW or os.path.exists(AWFLOW_ROOT):
            os.environ['WITH_AWFLOW'] = 'true'
            if not os.path.exists(AWFLOW_ROOT):
                AWFLOW_ROOT = self.getAwflowRoot()
            self.AWFLOW_ROOT = AWFLOW_ROOT
            print("AWFLOW_ROOT: " + self.AWFLOW_ROOT)

        self.parseArgs(self.awtk, ARGUMENTS)

        print("AWTK_ROOT: " + self.AWTK_ROOT)
        print("TKC_ONLY: " + str(self.TKC_ONLY))
        print(ARGUMENTS)

    def getAwtkConfig(self):
        sys.path.insert(0, self.AWTK_ROOT)
        import awtk_config as awtk
        return awtk

    def saveUsesSdkInfo(self):
        awtk = self.awtk
        release_id = ''
        filename = os.path.join(self.AWTK_ROOT, 'component.json')
        if os.path.exists(filename):
            with open(filename, 'r') as f:
                component_content = f.read()
                if len(component_content) > 0:
                    component_json = json.loads(component_content)
                    if 'release_id' in component_json:
                        release_id = component_json['release_id']
                        if sys.version_info < (3, 0):
                            release_id = release_id.encode('ascii')

        content = '{\n'
        content += '  "compileSDK": {\n'
        content += '    "awtk": {\n'
        content += '      "path": "' + \
            self.AWTK_ROOT.replace('\\', '/') + '",\n'
        content += '      "release_id": "' + release_id + '",\n'
        content += '      "nanovg_backend": "' + awtk.NANOVG_BACKEND + '"\n'
        content += '    }\n'
        content += '  }\n'
        content += '}'

        filename = os.path.join(self.APP_BIN_DIR, 'uses_sdk.json')
        if sys.version_info < (3, 0):
            with open(filename, 'w') as f:
                f.write(content)
        else:
            with open(filename, 'w', encoding='utf8') as f:
                f.write(content)

    def isBuildShared(self):
        return self.BUILD_SHARED

    def copyAwtkSharedLib(self):
        if self.TKC_ONLY:
            self.awtk.copySharedLib(self.AWTK_ROOT, self.APP_BIN_DIR, 'tkc')
        else:
            self.awtk.copySharedLib(self.AWTK_ROOT, self.APP_BIN_DIR, 'awtk')

        for iter in self.DEPENDS_LIBS:
            for so in iter['shared_libs']:
                src = os.path.join(iter['root'], self.BUILD_DIR)
                self.awtk.copySharedLib(src, self.APP_BIN_DIR, so)

    def genIdlAndDef(self):
        if self.DEF_FILE:
            print(self.DEF_FILE)
        else:
            return

        idl_gen_tools = os.path.join(self.AWTK_ROOT, 'tools/idl_gen/index.js')
        dll_def_gen_tools = os.path.join(
            self.AWTK_ROOT, 'tools/dll_def_gen/index.js')

        cmd = 'node ' + '"' + idl_gen_tools + '"' + ' idl/idl.json ' + self.SRC_DIR
        if os.system(cmd) != 0:
            print('exe cmd: ' + cmd + ' failed.')

        cmd = 'node ' + '"' + dll_def_gen_tools + '"' + \
            ' idl/idl.json ' + self.DEF_FILE
        if os.system(cmd) != 0:
            print('exe cmd: ' + cmd + ' failed.')
        else:
            if self.DEF_FILE_PROCESSOR != None:
                self.DEF_FILE_PROCESSOR()

    def showHelp(self):
        print('Options default values:')
        print('  LCD=320_480')
        print('  LANGUAGE=zh_CH')
        print('  FONT="default"')
        print('  THEME="default"')
        print('  SHARED=true')
        print('  LINUX_FB=false')
        print('  BUILD_DIR=arm')
        sys.exit(0)

    def parseArgs(self, awtk, ARGUMENTS):
        APP_RES_ROOT = '../res'
        APP_THEME = 'default'
        LCD_WIDTH = '320'
        LCD_HEIGHT = '480'
        APP_DEFAULT_FONT = 'default'
        APP_DEFAULT_LANGUAGE = 'zh'
        APP_DEFAULT_COUNTRY = 'CN'

        config = load_project_json('project.json')
        if config and 'assets' in config:
            APP_THEME = get_project_theme(config)
            LCD_WIDTH = get_project_w(config, APP_THEME)
            LCD_HEIGHT = get_project_h(config, APP_THEME)
            APP_DEFAULT_LANGUAGE = get_project_language(config)
            APP_DEFAULT_COUNTRY = get_project_country(config)
            APP_RES_ROOT = get_project_res_root(config)

        if ARGUMENTS.get('HELP', ''):
            self.showHelp()

        LCD = ARGUMENTS.get('LCD', '')
        if len(LCD) > 0:
            wh = LCD.split('_')
            if len(wh) >= 1:
                LCD_WIDTH = wh[0]
            if len(wh) >= 2:
                LCD_HEIGHT = wh[1]

        FONT = ARGUMENTS.get('FONT', '')
        if len(FONT) > 0:
            APP_DEFAULT_FONT = FONT

        THEME = ARGUMENTS.get('THEME', '')
        if len(THEME) > 0:
            APP_THEME = THEME

        LANGUAGE = ARGUMENTS.get('LANGUAGE', '')
        if len(LANGUAGE) > 0:
            lan = LANGUAGE.split('_')
            if len(lan) >= 1:
                APP_DEFAULT_LANGUAGE = lan[0]
            if len(lan) >= 2:
                APP_DEFAULT_COUNTRY = lan[1]

        RES_ROOT = ARGUMENTS.get('RES_ROOT', '')
        if len(RES_ROOT) > 0:
            APP_RES_ROOT = RES_ROOT
            self.APP_RES = os.path.abspath(
                os.path.join(self.APP_BIN_DIR, RES_ROOT))

        SHARED = ARGUMENTS.get('SHARED', '')
        if len(SHARED) > 0:
            self.BUILD_SHARED = not SHARED.lower().startswith('f')

        IDL_DEF = ARGUMENTS.get('IDL_DEF', '')
        if len(IDL_DEF) > 0:
            self.GEN_IDL_DEF = not IDL_DEF.lower().startswith('f')

        APP_CCFLAGS = ' -DLCD_WIDTH=' + LCD_WIDTH + ' -DLCD_HEIGHT=' + LCD_HEIGHT + ' '
        APP_CCFLAGS = APP_CCFLAGS + ' -DAPP_DEFAULT_FONT=\\\"' + APP_DEFAULT_FONT + '\\\" '
        APP_CCFLAGS = APP_CCFLAGS + ' -DAPP_THEME=\\\"' + APP_THEME + '\\\" '
        APP_CCFLAGS = APP_CCFLAGS + ' -DAPP_RES_ROOT=\"\\\"' + APP_RES_ROOT + '\\\"\" '
        APP_CCFLAGS = APP_CCFLAGS + ' -DAPP_DEFAULT_LANGUAGE=\\\"' + \
            APP_DEFAULT_LANGUAGE + '\\\" '
        APP_CCFLAGS = APP_CCFLAGS + ' -DAPP_DEFAULT_COUNTRY=\\\"' + \
            APP_DEFAULT_COUNTRY + '\\\" '
        APP_CCFLAGS = APP_CCFLAGS + ' -DAPP_ROOT=\"\\\"' + \
            self.APP_ROOT + '\\\"\" '
        self.APP_CFLAGS = ''
        self.APP_CCFLAGS = APP_CCFLAGS
        self.APP_CXXFLAGS = self.APP_CCFLAGS

        if PLATFORM == 'Linux':
            self.APP_LINKFLAGS += ' -Wl,-rpath=' + self.APP_BIN_DIR + ' '

        if not self.isBuildShared() and hasattr(awtk, 'AWTK_CCFLAGS'):
            self.AWTK_CCFLAGS = awtk.AWTK_CCFLAGS

        if self.isBuildShared():
            self.AWTK_LIBS = awtk.SHARED_LIBS
            self.APP_LIBPATH = [self.APP_BIN_DIR, self.APP_LIB_DIR]

        if hasattr(awtk, 'TOOLS_NAME') and awtk.TOOLS_NAME != '':
            self.APP_TOOLS = [awtk.TOOLS_NAME]

        os.environ['BUILD_SHARED'] = str(self.isBuildShared())
        print(LCD_WIDTH, LCD_HEIGHT)

    def prepare(self):
        if self.GEN_IDL_DEF and not self.LINUX_FB:
            self.genIdlAndDef()

        if self.isBuildShared():
            self.copyAwtkSharedLib()

        self.saveUsesSdkInfo()

    def getAwtkRoot(self):
        AWTK_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        if self.LINUX_FB:
            AWTK_ROOT = os.path.join(os.path.dirname(AWTK_ROOT), 'awtk-linux-fb')
        return AWTK_ROOT

    def getMvvmRoot(self):
        MVVM_ROOT = os.path.join(os.path.dirname(self.getAwtkRoot()), 'awtk-mvvm')
        return MVVM_ROOT

    def getAwflowRoot(self):
        AWFLOW_ROOT = os.path.join(os.path.dirname(self.getAwtkRoot()), 'aw-flow')
        return AWFLOW_ROOT

    def call(self, DefaultEnvironment):
        awtk = self.awtk
        CPPPATH = awtk.CPPPATH + self.APP_CPPPATH
        LINKFLAGS = awtk.LINKFLAGS + self.APP_LINKFLAGS
        LIBS = self.AWTK_LIBS + self.PLATFORM_LIBS
        LIBPATH = self.APP_LIBPATH + awtk.LIBPATH
        CFLAGS = self.APP_CFLAGS + self.AWTK_CFLAGS
        CCFLAGS = self.APP_CCFLAGS + self.AWTK_CCFLAGS
        TARGET_ARCH = awtk.TARGET_ARCH
        APP_TOOLS = self.APP_TOOLS
        CXXFLAGS = self.APP_CXXFLAGS
        DEPENDS_LIBS = []

        if self.MVVM_ROOT:
            if self.WITH_JERRYSCRIPT:
                MVVM_3RD_ROOT = join_path(self.MVVM_ROOT, '3rd')
                DEPENDS_LIBS += [{
                    'cxxflags': '-DWITH_MVVM -DWITH_JERRYSCRIPT',
                    'cflags': '-DWITH_MVVM -DWITH_JERRYSCRIPT',
                    'ccflags': '-DWITH_MVVM -DWITH_JERRYSCRIPT',
                    'root' : self.MVVM_ROOT,
                    'shared_libs': ['mvvm'],
                    'static_libs': ['']
                }]
                CPPPATH += [
                    join_path(MVVM_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/include'),
                    join_path(MVVM_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/include'),
                ]
            else:
                DEPENDS_LIBS += [{
                    'cxxflags': '-DWITH_MVVM',
                    'cflags': '-DWITH_MVVM',
                    'ccflags': '-DWITH_MVVM',
                    'root' : self.MVVM_ROOT,
                    'shared_libs': ['mvvm'],
                    'static_libs': []
                }]

        if self.AWFLOW_ROOT:
            DEPENDS_LIBS += [{
                'cxxflags': '-DWITH_AWFLOW',
                'cflags': '-DWITH_AWFLOW',
                'ccflags': '-DWITH_AWFLOW',
                'root' : self.AWFLOW_ROOT,
                'shared_libs': ['awflow'],
                'static_libs': []
            }]

            if self.isBuildShared():
                src = join_path(self.AWFLOW_ROOT, 'bin/nodes')
                dst = join_path(self.APP_ROOT, 'bin/nodes')
                if os.path.exists(dst):
                    shutil.rmtree(dst)
                print(src + '==>' + dst)
                shutil.copytree(src, dst)

        self.DEPENDS_LIBS = DEPENDS_LIBS + self.DEPENDS_LIBS

        if self.TKC_ONLY:
            CCFLAGS += ' -DTKC_ONLY=1 '
        else:
            CCFLAGS += ' -DWITH_AWTK=1 '

        for iter in self.DEPENDS_LIBS:
            if 'shared_libs' in iter:
                LIBS = iter['shared_libs'] + LIBS
            if 'static_libs' in iter:
                LIBS = iter['static_libs'] + LIBS
            if 'cxxflags' in iter:
                CXXFLAGS += " " + iter['cxxflags'] + " "
            if 'cflags' in iter:
                CFLAGS += " " + iter['cflags'] + " "
            if 'ccflags' in iter:
                CCFLAGS += " " + iter['ccflags'] + " "

            if 'cpppath' in iter:
                for f in iter['cpppath']:
                    CPPPATH = CPPPATH + [join_path(iter['root'], f)]
            else:
                CPPPATH += [join_path(iter['root'], 'src')]

            if 'libpath' in iter:
                for f in iter['libpath']:
                    LIBPATH = LIBPATH + [join_path(iter['root'], f)]
            else:
                if self.isBuildShared():
                    LIBPATH += [join_path(iter['root'], self.BIN_DIR)]
                LIBPATH += [join_path(iter['root'], self.LIB_DIR)]
        LIBS = self.APP_LIBS + LIBS

        if hasattr(awtk, 'CC'):
            env = DefaultEnvironment(
                CC=awtk.CC,
                CXX=awtk.CXX,
                LD=awtk.LD,
                AR=awtk.AR,
                STRIP=awtk.STRIP,
                RANLIB=awtk.RANLIB,
                TOOLS=APP_TOOLS,
                CPPPATH=CPPPATH,
                LINKFLAGS=LINKFLAGS,
                LIBS=LIBS,
                LIBPATH=LIBPATH,
                CCFLAGS=CCFLAGS,
                CFLAGS=CFLAGS,
                CXXFLAGS=CXXFLAGS,
                TARGET_ARCH=awtk.TARGET_ARCH,
                OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
                OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS)
        else:
            env = DefaultEnvironment(
                TOOLS=APP_TOOLS,
                CPPPATH=CPPPATH,
                LINKFLAGS=LINKFLAGS,
                LIBS=LIBS,
                LIBPATH=LIBPATH,
                CFLAGS=CFLAGS,
                CCFLAGS=CCFLAGS,
                CXXFLAGS=CXXFLAGS,
                TARGET_ARCH=awtk.TARGET_ARCH,
                OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
                OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS)
        
        def variant_SConscript(env, SConscriptFiles):
            self.SConscript(SConscriptFiles)
        env.AddMethod(variant_SConscript, "SConscript")
    
        self.prepare()
        return env
