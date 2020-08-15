import os
import sys
import json
import shutil
import platform

def mkdir_if_not_exist(fullpath):
    if os.path.exists(fullpath):
        print(fullpath+' exist.')
    else:
        os.makedirs(fullpath)

class AppHelperBase:
    def set_deps(self, DEPENDS_LIBS):
        self.DEPENDS_LIBS = DEPENDS_LIBS
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
    
    def add_libpath(self, APP_LIBPATH):
        self.APP_LIBPATH += APP_LIBPATH
        return self

    def add_cpppath(self, APP_CPPPATH):
        self.APP_CPPPATH += APP_CPPPATH
        return self

    def add_ccflags(self, APP_CCFLAGS):
        self.APP_CCFLAGS += APP_CCFLAGS
        return self

    def add_cxxflags(self, APP_CXXFLAGS):
        self.APP_CXXFLAGS += APP_CXXFLAGS
        return self

    def add_linkflags(self, APP_LINKFLAGS):
        self.APP_LINKFLAGS += APP_LINKFLAGS
        return self

    def __init__(self, ARGUMENTS):
        APP_ROOT = os.path.normpath(os.getcwd())

        self.APP_LIBS = []
        self.DEF_FILE = None
        self.DEF_FILE_PROCESSOR = None
        self.DEPENDS_LIBS = []
        self.APP_ROOT = APP_ROOT
        self.GEN_IDL_DEF = True
        self.BUILD_SHARED = True
        self.ARGUMENTS = ARGUMENTS
        self.LINUX_FB = ARGUMENTS.get('LINUX_FB', '') != ''
        self.AWTK_ROOT = self.getAwtkRoot()
        self.APP_BIN_DIR = os.path.join(APP_ROOT, 'bin')
        self.APP_LIB_DIR = os.path.join(APP_ROOT, 'lib')
        self.APP_SRC = os.path.join(APP_ROOT, 'src')
        
        mkdir_if_not_exist(self.APP_BIN_DIR);
        mkdir_if_not_exist(self.APP_LIB_DIR);

        sys.path.insert(0, self.AWTK_ROOT)
        import awtk_config as awtk

        self.awtk = awtk
        self.AWTK_LIBS = awtk.LIBS
        self.AWTK_SHARED_LIBS = awtk.SHARED_LIBS

        os.environ['APP_SRC'] = self.APP_SRC
        os.environ['APP_ROOT'] = self.APP_ROOT
        os.environ['BIN_DIR'] = self.APP_BIN_DIR
        os.environ['LIB_DIR'] = self.APP_LIB_DIR

        self.parseArgs(awtk, ARGUMENTS)

        print("AWTK_ROOT:" + self.AWTK_ROOT)
        print(ARGUMENTS)

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
        return 'WITH_AWTK_SO' in os.environ and os.environ['WITH_AWTK_SO'] == 'true' and self.BUILD_SHARED 

    def copyAwtkSharedLib(self):
        self.awtk.copySharedLib(self.AWTK_ROOT, self.APP_BIN_DIR, 'awtk')
        for iter in self.DEPENDS_LIBS:
            for so in iter['shared_libs']:
                self.awtk.copySharedLib(iter['root'], self.APP_BIN_DIR, so)

    def genIdlAndDef(self):
        if self.DEF_FILE:
            print(self.DEF_FILE)
        else:
            return

        idl_gen_tools = os.path.join(self.AWTK_ROOT, 'tools/idl_gen/index.js')
        dll_def_gen_tools = os.path.join(
            self.AWTK_ROOT, 'tools/dll_def_gen/index.js')

        cmd = 'node ' + '"' + idl_gen_tools + '"' + ' idl/idl.json ' + 'src'
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
        sys.exit(0)

    def parseArgs(self, awtk, ARGUMENTS):
        BUILD_SHARED = True
        GEN_IDL_DEF = True
        LCD_WIDTH = '320'
        LCD_HEIGHT = '480'
        APP_DEFAULT_FONT = 'default'
        APP_THEME = 'default'
        APP_RES_ROOT = '../res'
        APP_DEFAULT_LANGUAGE = 'zh'
        APP_DEFAULT_COUNTRY = 'CN'

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

        SHARED = ARGUMENTS.get('SHARED', '')
        if len(SHARED) > 0 and SHARED.lower().startswith('f'):
            self.BUILD_SHARED = False

        IDL_DEF = ARGUMENTS.get('IDL_DEF', '')
        if len(IDL_DEF) > 0 and IDL_DEF.lower().startswith('f'):
            GEN_IDL_DEF = False

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
        os.environ['BUILD_SHARED'] = str(self.isBuildShared())

        APP_LINKFLAGS = ''
        AWTK_LIBS = self.AWTK_LIBS
        APP_CPPPATH = [self.APP_SRC]
        APP_LIBPATH = [self.APP_LIB_DIR]
        AWTK_CCFLAGS = awtk.CCFLAGS

        if platform.system() == 'Linux':
            APP_LINKFLAGS += ' -Wl,-rpath=' + self.APP_BIN_DIR + ' '

        if not self.isBuildShared() and hasattr(awtk, 'AWTK_CCFLAGS'):
            AWTK_CCFLAGS = awtk.AWTK_CCFLAGS

        if self.isBuildShared():
            APP_LIBPATH = [self.APP_BIN_DIR, self.APP_LIB_DIR]
            AWTK_LIBS = self.AWTK_SHARED_LIBS

        APP_TOOLS = None
        if hasattr(awtk, 'TOOLS_NAME') and awtk.TOOLS_NAME != '':
            APP_TOOLS = [awtk.TOOLS_NAME]

        self.AWTK_LIBS = AWTK_LIBS
        self.AWTK_CCFLAGS = AWTK_CCFLAGS
        self.APP_TOOLS = APP_TOOLS
        self.APP_CCFLAGS = APP_CCFLAGS
        self.APP_LIBPATH = APP_LIBPATH
        self.APP_CPPPATH = APP_CPPPATH
        self.APP_LINKFLAGS = APP_LINKFLAGS
        self.GEN_IDL_DEF = GEN_IDL_DEF
        self.APP_CXXFLAGS = self.APP_CCFLAGS

    def prepare(self):
        if self.GEN_IDL_DEF and not self.LINUX_FB:
            self.genIdlAndDef()

        if self.isBuildShared():
            self.copyAwtkSharedLib()

        self.saveUsesSdkInfo()

    def getAwtkRoot(self):
        return os.path.abspath('../../awtk')

    def call(self, DefaultEnvironment):
        awtk = self.awtk

        CPPPATH = awtk.CPPPATH + self.APP_CPPPATH
        LINKFLAGS = awtk.LINKFLAGS + self.APP_LINKFLAGS
        LIBS = self.APP_LIBS + self.AWTK_LIBS
        LIBPATH = self.APP_LIBPATH + awtk.LIBPATH
        CCFLAGS = self.APP_CCFLAGS + self.AWTK_CCFLAGS
        TARGET_ARCH = awtk.TARGET_ARCH
        APP_TOOLS = self.APP_TOOLS
        CXXFLAGS = self.APP_CXXFLAGS

        for iter in self.DEPENDS_LIBS:
            if 'shared_libs' in iter:
                LIBS += iter['shared_libs']
            if 'static_libs' in iter:
                LIBS += iter['static_libs']
            CPPPATH += [os.path.join(os.path.abspath(iter['root']), 'src')]
            LIBPATH += [os.path.join(os.path.abspath(iter['root']), 'lib')]
            LIBPATH += [os.path.join(os.path.abspath(iter['root']), 'bin')]

        self.prepare()
        if hasattr(awtk, 'CC'):
            DefaultEnvironment(
                CC=awtk.CC,
                CXX=awtk.CXX,
                LD=awtk.LD,
                AR=awtk.AR,
                STRIP=awtk.STRIP,
                TOOLS=APP_TOOLS,
                CPPPATH=CPPPATH,
                LINKFLAGS=LINKFLAGS,
                LIBS=LIBS,
                LIBPATH=LIBPATH,
                CCFLAGS=CCFLAGS,
                CXXFLAGS=CXXFLAGS,
                TARGET_ARCH=awtk.TARGET_ARCH,
                OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
                OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS)
        else:
            DefaultEnvironment(
                TOOLS=APP_TOOLS,
                CPPPATH=CPPPATH,
                LINKFLAGS=LINKFLAGS,
                LIBS=LIBS,
                LIBPATH=LIBPATH,
                CCFLAGS=CCFLAGS,
                CXXFLAGS=CXXFLAGS,
                TARGET_ARCH=awtk.TARGET_ARCH,
                OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
                OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS)
