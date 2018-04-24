# Picasso - a vector graphics library
# 
# Copyright (C) 2013 Zhang Ji Peng
# Contact: onecoolx@gmail.com

{
 'conditions': [
   ['OS=="win"', {
     'actions': [
       {
         'action_name': 'copy_img1',
         'inputs': [
           'pat.bmp', 
         ],
         'outputs': [
           '$(OutDir)/pat.bmp' 
         ],
         'action': [
           'python',
           'tools/cp.py',
           '<(_inputs)',
           '$(OutDir)/pat.bmp',
         ],
         'msvs_cygwin_shell': 0,
       },
       {
         'action_name': 'copy_img2',
         'inputs': [
           'selt2.bmp' 
         ],
         'outputs': [
           '$(OutDir)/selt2.bmp' 
         ],
         'action': [
           'python',
           'tools/cp.py',
           '<(_inputs)',
           '$(OutDir)/selt2.bmp',
         ],
         'msvs_cygwin_shell': 0,
       },
     ],
   }],
   ['OS=="linux"', {
     'actions': [
       {
         'action_name': 'copy_img1',
         'inputs': [
           'pat.png', 
         ],
         'outputs': [
           '$(builddir)/pat.png' 
         ],
         'action': [
           'python',
           'tools/cp.py',
           '<(_inputs)',
           '$(builddir)/pat.png',
         ],
       },
       {
         'action_name': 'copy_img2',
         'inputs': [
           'selt2.png' 
         ],
         'outputs': [
           '$(builddir)/selt2.png' 
         ],
         'action': [
           'python',
           'tools/cp.py',
           '<(_inputs)',
           '$(builddir)/selt2.png',
         ],
       },
     ],
   }],
 ],
}
