# Picasso - a vector graphics library
# 
# Copyright (C) 2013 Zhang Ji Peng
# Contact: onecoolx@gmail.com

{
  'targets': [
    {
      # alpha
      'target_name': 'alpha',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'alpha_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
        'copy.gypi',
      ],
    },
    {
      # bitblt
      'target_name': 'bitblt',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'bitblt_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
        'copy.gypi',
      ],
    },
    {
      # blur
      'target_name': 'blur',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'blur_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
        'copy.gypi',
      ],
    },
    {
      # clip
      'target_name': 'clip',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'clip_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
        'copy.gypi',
      ],
    },
    {
      # composite
      'target_name': 'composite',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'composite_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
      ],
    },
    {
      # gamma
      'target_name': 'gamma',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'gamma_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
      ],
    },
    {
      # gcstate
      'target_name': 'gcstate',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'gcstate_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
        'copy.gypi',
      ],
    },
    {
      # gradient
      'target_name': 'gradient',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'gradient_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
      ],
    },
    {
      # mask
      'target_name': 'mask',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'mask_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
        'copy.gypi',
      ],
    },
    {
      # part
      'target_name': 'part',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'part_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
        'copy.gypi',
      ],
    },
    {
      # path
      'target_name': 'path',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'path_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
      ],
    },
    {
      # pattern
      'target_name': 'pattern',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'pattern_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
        'copy.gypi',
      ],
    },
    {
      # shadow
      'target_name': 'shadow',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'shadow_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
        'copy.gypi',
      ],
    },
    {
      # text
      'target_name': 'text',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'text_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
        'copy.gypi',
      ],
    },
    {
      # threads
      'target_name': 'threads',
      'type': 'executable',
      'dependencies': [
        'picasso2_sw',
      ],
      'include_dirs': [
        '../include',
	'../build',
        './'
      ],
      'sources': [
        'thread_func.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'thr_win32.c',
            'testWin.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'thr_posix.c',
            'testGtk2.c',
          ],
          'cflags': [
            '`pkg-config --cflags gtk+-2.0`',
          ],
          'libraries': [
            '-lfreetype',
            '-lz -lpthread `pkg-config --libs gtk+-2.0`',
          ],
        }],
      ],
      'includes':[
        '../build/configs.gypi',
        '../build/defines.gypi',
        'copy.gypi',
      ],
    },
  ],
}

