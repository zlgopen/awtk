# -*- coding: utf-8 -*-
import unittest

import compile_config


class ResolveVgcanvasTest(unittest.TestCase):
  def test_agge_overrides_nanovg_plus(self):
    self.assertEqual(compile_config.resolve_vgcanvas('NANOVG_PLUS', 'AGGE'), 'NANOVG')

  def test_agge_keeps_explicit_nanovg(self):
    self.assertEqual(compile_config.resolve_vgcanvas('NANOVG', 'AGGE'), 'NANOVG')

  def test_agge_keeps_cairo(self):
    self.assertEqual(compile_config.resolve_vgcanvas('CAIRO', 'AGGE'), 'CAIRO')

  def test_gpu_backend_keeps_nanovg_plus(self):
    self.assertEqual(compile_config.resolve_vgcanvas('NANOVG_PLUS', 'GL3'), 'NANOVG_PLUS')
    self.assertEqual(compile_config.resolve_vgcanvas('NANOVG_PLUS', 'GLES3'), 'NANOVG_PLUS')


if __name__ == '__main__':
  unittest.main()
