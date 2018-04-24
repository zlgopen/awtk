# Picasso - a vector graphics library
# 
# Copyright (C) 2013 Zhang Ji Peng
# Contact: onecoolx@gmail.com

{
  'configurations': {
    'Debug': {
      'conditions': [
        ['OS=="win"', {
          'defines': [
            '_DEBUG'
            'ENABLE_FAST_COPY=1'
          ],
          'msvs_configuration_attributes': {
            'ConfigurationType': '2',
            'CharacterSet': '1',
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
			  'Optimization': '0',
			  'MinimalRebuild': 'true',
			  'ExceptionHandling': '0',
			  'BasicRuntimeChecks': '3',
			  'RuntimeLibrary': '1',
			  'TreatWChar_tAsBuiltInType': 'false',
			  'RuntimeTypeInfo': 'false',
			  'WarningLevel': '3',
			  'WarnAsError': 'true',
			  'SuppressStartupBanner': 'true',
			  'DebugInformationFormat': '3'
            },
            'VCLinkerTool': {
				'LinkIncremental': '1',
				'SuppressStartupBanner': 'true',
				'GenerateDebugInformation': 'true',
				'RandomizedBaseAddress': '1',
				'DataExecutionPrevention': '0',
				'TargetMachine': '1'
            },
          },
        }],
        ['OS=="linux"', {
          'defines': [
            '_DEBUG',
            'ENABLE_FREE_TYPE2=1'
            'ENABLE_FONT_CONFIG=1'
            'ENABLE_FAST_COPY=1'
          ],
          'cflags_cc': [
            '-O0',
            '-Wall',
            '-g',
            '-fPIC',
            '-fno-rtti',
            '-fno-exceptions',
            '-Wno-unused-result',
          ],
        }],
      ],
    },
    'Release': {
      'conditions': [
        ['OS=="win"', {
          'defines': [
            'NDEBUG'
            'ENABLE_FAST_COPY=1'
          ],
          'msvs_configuration_attributes': {
            'ConfigurationType': '2',
            'CharacterSet': '1',
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
			  'Optimization': '3',
			  'InlineFunctionExpansion': '2',
			  'FavorSizeOrSpeed': '1',
			  'WholeProgramOptimization': 'true',
			  'StringPooling': 'true',
			  'ExceptionHandling': '0',
			  'RuntimeLibrary': '0',
			  'EnableFunctionLevelLinking': 'true',
			  'EnableEnhancedInstructionSet': '2',
			  'FloatingPointModel': '2',
			  'TreatWChar_tAsBuiltInType': 'false',
			  'RuntimeTypeInfo': 'false',
			  'WarningLevel': '3',
			  'WarnAsError': 'true',
			  'SuppressStartupBanner': 'true',
			  'DebugInformationFormat': '3'
            },
            'VCLinkerTool': {
				'LinkIncremental': '1',
				'SuppressStartupBanner': 'true',
				'GenerateDebugInformation': 'false',
				'GenerateMapFile': 'true',
				'LinkTimeCodeGeneration': '1',
				'RandomizedBaseAddress': '1',
				'DataExecutionPrevention': '0',
				'TargetMachine': '1'
            },
          },
        }],
        ['OS=="linux"', {
          'defines': [
            'NDEBUG',
            'ENABLE_FREE_TYPE2=1'
            'ENABLE_FONT_CONFIG=1'
            'ENABLE_FAST_COPY=1'
          ],
          'cflags_cc': [
            '-O3',
            '-Wall',
            '-fPIC',
            '-fno-rtti',
            '-fno-exceptions',
            '-Wno-unused-result',
          ],
        }],
      ],
    },
  },
}
