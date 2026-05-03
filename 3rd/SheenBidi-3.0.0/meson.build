project('SheenBidi', ['c', 'cpp'],
  version: run_command(
    'Tools/Version/extract.py',
    'Headers/SheenBidi/SBVersion.h',
    check: true
  ).stdout().strip(),
  meson_version: '>= 0.55.0',
  default_options: [
    'cpp_std=c++14'
  ]
)

build_text_api = get_option('text_api').enabled()
unity_mode = get_option('unity_mode').enabled()
build_generator = get_option('generator').enabled()

is_windows_host = host_machine.system() == 'windows'
static_mode = get_option('default_library') == 'static'
dll_mode = is_windows_host and not static_mode
build_tests = not dll_mode and not unity_mode

project_version = meson.project_version()
soversion = project_version.split('.')[0]

# ------------------------------------------------------------------------------
# SheenBidi Library
# ------------------------------------------------------------------------------

sheenbidi_headers = files(
  'Headers/SheenBidi/SBAlgorithm.h',
  'Headers/SheenBidi/SBAllocator.h',
  'Headers/SheenBidi/SBAttributeInfo.h',
  'Headers/SheenBidi/SBAttributeList.h',
  'Headers/SheenBidi/SBAttributeRegistry.h',
  'Headers/SheenBidi/SBBase.h',
  'Headers/SheenBidi/SBBidiType.h',
  'Headers/SheenBidi/SBCodepoint.h',
  'Headers/SheenBidi/SBCodepointSequence.h',
  'Headers/SheenBidi/SBGeneralCategory.h',
  'Headers/SheenBidi/SBLine.h',
  'Headers/SheenBidi/SBMirrorLocator.h',
  'Headers/SheenBidi/SBParagraph.h',
  'Headers/SheenBidi/SBRun.h',
  'Headers/SheenBidi/SBScript.h',
  'Headers/SheenBidi/SBScriptLocator.h',
  'Headers/SheenBidi/SBText.h',
  'Headers/SheenBidi/SBTextConfig.h',
  'Headers/SheenBidi/SBTextIterators.h',
  'Headers/SheenBidi/SBTextType.h',
  'Headers/SheenBidi/SBVersion.h',
  'Headers/SheenBidi/SheenBidi.h'
)
internal_headers = files(
  'Source/API/SBAlgorithm.h',
  'Source/API/SBAllocator.h',
  'Source/API/SBAssert.h',
  'Source/API/SBAttributeInfo.h',
  'Source/API/SBAttributeList.h',
  'Source/API/SBAttributeRegistry.h',
  'Source/API/SBBase.h',
  'Source/API/SBCodepoint.h',
  'Source/API/SBCodepointSequence.h',
  'Source/API/SBLine.h',
  'Source/API/SBLog.h',
  'Source/API/SBMirrorLocator.h',
  'Source/API/SBParagraph.h',
  'Source/API/SBScriptLocator.h',
  'Source/API/SBText.h',
  'Source/API/SBTextConfig.h',
  'Source/API/SBTextIterators.h',
  'Source/Core/AtomicFlag.h',
  'Source/Core/AtomicPointer.h',
  'Source/Core/AtomicUInt.h',
  'Source/Core/List.h',
  'Source/Core/Memory.h',
  'Source/Core/Object.h',
  'Source/Core/Once.h',
  'Source/Core/ThreadFence.h',
  'Source/Core/ThreadLocalStorage.h',
  'Source/Data/BidiTypeLookup.h',
  'Source/Data/GeneralCategoryLookup.h',
  'Source/Data/PairingLookup.h',
  'Source/Data/ScriptLookup.h',
  'Source/Script/ScriptStack.h',
  'Source/Text/AttributeDictionary.h',
  'Source/Text/AttributeManager.h',
  'Source/UBA/BidiChain.h',
  'Source/UBA/BracketQueue.h',
  'Source/UBA/BracketType.h',
  'Source/UBA/IsolatingRun.h',
  'Source/UBA/LevelRun.h',
  'Source/UBA/RunExtrema.h',
  'Source/UBA/RunKind.h',
  'Source/UBA/RunQueue.h',
  'Source/UBA/StatusStack.h'
)

if unity_mode
  add_project_arguments('-DSB_CONFIG_UNITY', language: ['c', 'cpp'])
  sheenbidi_sources = files('Source/SheenBidi.c')
else
  sheenbidi_sources = files(
    'Source/API/SBAlgorithm.c',
    'Source/API/SBAllocator.c',
    'Source/API/SBAttributeList.c',
    'Source/API/SBAttributeRegistry.c',
    'Source/API/SBBase.c',
    'Source/API/SBCodepoint.c',
    'Source/API/SBCodepointSequence.c',
    'Source/API/SBLine.c',
    'Source/API/SBLog.c',
    'Source/API/SBMirrorLocator.c',
    'Source/API/SBParagraph.c',
    'Source/API/SBScriptLocator.c',
    'Source/API/SBText.c',
    'Source/API/SBTextConfig.c',
    'Source/API/SBTextIterators.c',
    'Source/Core/List.c',
    'Source/Core/Memory.c',
    'Source/Core/Object.c',
    'Source/Core/Once.c',
    'Source/Data/BidiTypeLookup.c',
    'Source/Data/GeneralCategoryLookup.c',
    'Source/Data/PairingLookup.c',
    'Source/Data/ScriptLookup.c',
    'Source/Script/ScriptStack.c',
    'Source/Text/AttributeDictionary.c',
    'Source/Text/AttributeManager.c',
    'Source/UBA/BidiChain.c',
    'Source/UBA/BracketQueue.c',
    'Source/UBA/IsolatingRun.c',
    'Source/UBA/LevelRun.c',
    'Source/UBA/RunQueue.c',
    'Source/UBA/StatusStack.c'
  )
endif

sheenbidi_c_args = []
sheenbidi_dep_args = []

if dll_mode
  sheenbidi_c_args += '-DSB_CONFIG_DLL_EXPORT'
  sheenbidi_dep_args += '-DSB_CONFIG_DLL_IMPORT'
endif
if build_text_api
  sheenbidi_c_args += '-DSB_CONFIG_EXPERIMENTAL_TEXT_API'
  sheenbidi_dep_args += '-DSB_CONFIG_EXPERIMENTAL_TEXT_API'
endif

sheenbidi = library(
  'SheenBidi',
  sources: [sheenbidi_headers, internal_headers, sheenbidi_sources],
  c_args: sheenbidi_c_args,
  include_directories: ['Headers', 'Source'],
  version: project_version,
  soversion: soversion,
  install: true
)

sheenbidi_dep = declare_dependency(
  include_directories: 'Headers',
  link_with: sheenbidi,
  compile_args: sheenbidi_dep_args
)

# ------------------------------------------------------------------------------
# Parser Library (Required for Generator/Tests)
# ------------------------------------------------------------------------------

if build_generator or build_tests
  parser_files = files(
    'Tools/Parser/BidiBrackets.cpp',
    'Tools/Parser/BidiBrackets.h',
    'Tools/Parser/BidiCharacterTest.cpp',
    'Tools/Parser/BidiCharacterTest.h',
    'Tools/Parser/BidiMirroring.cpp',
    'Tools/Parser/BidiMirroring.h',
    'Tools/Parser/BidiTest.cpp',
    'Tools/Parser/BidiTest.h',
    'Tools/Parser/DataFile.cpp',
    'Tools/Parser/DataFile.h',
    'Tools/Parser/DerivedBidiClass.cpp',
    'Tools/Parser/DerivedBidiClass.h',
    'Tools/Parser/DerivedCoreProperties.cpp',
    'Tools/Parser/DerivedCoreProperties.h',
    'Tools/Parser/DerivedGeneralCategory.cpp',
    'Tools/Parser/DerivedGeneralCategory.h',
    'Tools/Parser/PropertyValueAliases.cpp',
    'Tools/Parser/PropertyValueAliases.h',
    'Tools/Parser/PropList.cpp',
    'Tools/Parser/PropList.h',
    'Tools/Parser/Scripts.cpp',
    'Tools/Parser/Scripts.h',
    'Tools/Parser/UnicodeData.cpp',
    'Tools/Parser/UnicodeData.h',
    'Tools/Parser/UnicodeVersion.cpp',
    'Tools/Parser/UnicodeVersion.h'
  )
  
  parser = static_library(
    'Parser',
    sources: parser_files,
    include_directories: ['Tools']
  )
  
  parser_dep = declare_dependency(
    include_directories: 'Tools',
    link_with: parser
  )
endif

# ------------------------------------------------------------------------------
# Generator Tool (Optional)
# ------------------------------------------------------------------------------

if build_generator
  generator_files = files(
    'Tools/Generator/Utilities/ArrayBuilder.cpp',
    'Tools/Generator/Utilities/ArrayBuilder.h',
    'Tools/Generator/Utilities/Converter.cpp',
    'Tools/Generator/Utilities/Converter.h',
    'Tools/Generator/Utilities/FileBuilder.cpp',
    'Tools/Generator/Utilities/FileBuilder.h',
    'Tools/Generator/Utilities/Math.cpp',
    'Tools/Generator/Utilities/Math.h',
    'Tools/Generator/Utilities/StreamBuilder.cpp',
    'Tools/Generator/Utilities/StreamBuilder.h',
    'Tools/Generator/Utilities/TextBuilder.cpp',
    'Tools/Generator/Utilities/TextBuilder.h',
    'Tools/Generator/BidiTypeLookupGenerator.cpp',
    'Tools/Generator/BidiTypeLookupGenerator.h',
    'Tools/Generator/GeneralCategoryLookupGenerator.cpp',
    'Tools/Generator/GeneralCategoryLookupGenerator.h',
    'Tools/Generator/main.cpp',
    'Tools/Generator/PairingLookupGenerator.cpp',
    'Tools/Generator/PairingLookupGenerator.h',
    'Tools/Generator/ScriptLookupGenerator.cpp',
    'Tools/Generator/ScriptLookupGenerator.h'
  )
  
  executable(
    'Generator',
    sources: generator_files,
    include_directories: ['Headers', 'Source', 'Tools'],
    dependencies: [parser_dep],
    install: false
  )
endif

# ------------------------------------------------------------------------------
# Testing (Only non-unity mode)
# ------------------------------------------------------------------------------

if build_tests
  test_common_files = files(
    'Tests/Utilities/Convert.cpp',
    'Tests/Utilities/Convert.h',
    'Tests/Utilities/Unicode.h'
  )

  test_targets = {
    'AlgorithmTests': [
      'Tests/AlgorithmTests.h',
      'Tests/AlgorithmTests.cpp'
    ],
    'AllocatorTests': [
      'Tests/AllocatorTests.h',
      'Tests/AllocatorTests.cpp'
    ],
    'AtomicTests': [
      'Tests/AtomicTests.h',
      'Tests/AtomicTests.cpp'
    ],
    'BidiTypeLookupTests': [
      'Tests/BidiTypeLookupTests.h',
      'Tests/BidiTypeLookupTests.cpp'
    ],
    'BracketLookupTests': [
      'Tests/BracketLookupTests.h',
      'Tests/BracketLookupTests.cpp'
    ],
    'BracketQueueTests': [
      'Tests/BracketQueueTests.h',
      'Tests/BracketQueueTests.cpp'
    ],
    'CodepointSequenceTests': [
      'Tests/CodepointSequenceTests.h',
      'Tests/CodepointSequenceTests.cpp'
    ],
    'CodepointTests': [
      'Tests/CodepointTests.h',
      'Tests/CodepointTests.cpp'
    ],
    'GeneralCategoryLookupTests': [
      'Tests/GeneralCategoryLookupTests.h',
      'Tests/GeneralCategoryLookupTests.cpp'
    ],
    'MirrorLookupTests': [
      'Tests/MirrorLookupTests.h',
      'Tests/MirrorLookupTests.cpp'
    ],
    'OnceTests': [
      'Tests/OnceTests.h',
      'Tests/OnceTests.cpp'
    ],
    'RunQueueTests': [
      'Tests/RunQueueTests.h',
      'Tests/RunQueueTests.cpp'
    ],
    'ScriptLocatorTests': [
      'Tests/ScriptLocatorTests.h',
      'Tests/ScriptLocatorTests.cpp'
    ],
    'ScriptLookupTests': [
      'Tests/ScriptLookupTests.h',
      'Tests/ScriptLookupTests.cpp'
    ],
    'ScriptTests': [
      'Tests/ScriptTests.h',
      'Tests/ScriptTests.cpp'
    ],
    'ThreadLocalStorageTests': [
      'Tests/ThreadLocalStorageTests.h',
      'Tests/ThreadLocalStorageTests.cpp'
    ]
  }

  if build_text_api
    test_targets += {
      'AttributeManagerTests': [
        'Tests/AttributeManagerTests.h',
        'Tests/AttributeManagerTests.cpp'
      ],
      'AttributeRunIteratorTests': [
        'Tests/AttributeRunIteratorTests.h',
        'Tests/AttributeRunIteratorTests.cpp'
      ],
      'LogicalRunIteratorTests': [
        'Tests/LogicalRunIteratorTests.h',
        'Tests/LogicalRunIteratorTests.cpp'
      ],
      'ParagraphIteratorTests': [
        'Tests/ParagraphIteratorTests.h',
        'Tests/ParagraphIteratorTests.cpp'
      ],
      'ScriptRunIteratorTests': [
        'Tests/ScriptRunIteratorTests.h',
        'Tests/ScriptRunIteratorTests.cpp'
      ],
      'TextTests': [
        'Tests/TextTests.h',
        'Tests/TextTests.cpp'
      ],
      'VisualRunIteratorTests': [
        'Tests/VisualRunIteratorTests.h',
        'Tests/VisualRunIteratorTests.cpp'
      ]
    }
  endif

  foreach test_name, test_files : test_targets
    test_exe = executable(
      test_name,
      sources: test_common_files + test_files,
      include_directories: ['Headers', 'Source', 'Tools'],
      dependencies: [parser_dep, sheenbidi_dep],
      cpp_args: ['-DSTANDALONE_TESTING'],
      install: false
    )
    test(
      test_name,
      test_exe,
      args: [meson.current_source_dir() / 'Tools/Unicode'],
      timeout: 60
    )
  endforeach
endif

# ------------------------------------------------------------------------------
# Installation
# ------------------------------------------------------------------------------

install_headers(
  sheenbidi_headers,
  subdir: 'SheenBidi'
)

pkg = import('pkgconfig')
pkg.generate(
  filebase: 'sheenbidi',
  name: 'SheenBidi',
  description: 'SheenBidi is a lightweight, fast and stable implementation of the Unicode Bidirectional Algorithm.',
  url: 'https://github.com/Tehreer/SheenBidi',
  version: project_version,
  libraries: sheenbidi,
  extra_cflags: sheenbidi_dep_args
)
