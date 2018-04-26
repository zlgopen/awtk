# Picasso - a vector graphics library
# 
# Copyright (C) 2013 Zhang Ji Peng
# Contact: onecoolx@gmail.com

{
  'targets': [
    {
      # clock
      'target_name': 'clock',
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
        'clock.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'platform_win32.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'platform_gtk2.c',
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
      # flowers
      'target_name': 'flowers',
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
        'flowers.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'platform_win32.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'platform_gtk2.c',
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
      # subwaymap
      'target_name': 'subwaymap',
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
        'subwaymap.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'platform_win32.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'platform_gtk2.c',
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
      # tiger
      'target_name': 'tiger',
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
        'tiger.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'platform_win32.c',
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'platform_gtk2.c',
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
  ]
}

