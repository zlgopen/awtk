
AWTK_SRC=$(AWTK_ROOT)/src
AWTK_SVG=$(AWTK_SRC)/svg
AWTK_TKC=$(AWTK_SRC)/tkc
AWTK_XML=$(AWTK_SRC)/xml
AWTK_BASE=$(AWTK_SRC)/base
AWTK_WIDGETS=$(AWTK_SRC)/widgets
AWTK_LAYOUTERS=$(AWTK_SRC)/layouters
AWTK_UI_LOADER=$(AWTK_SRC)/ui_loader
AWTK_CLIPBOARD=$(AWTK_SRC)/clip_board
AWTK_WIDGET_ANIMATORS=$(AWTK_SRC)/widget_animators
AWTK_WINDOW_ANIMATORS=$(AWTK_SRC)/window_animators
AWTK_VGCANVAS=$(AWTK_SRC)/vgcanvas
AWTK_EXT_WIDGETS=$(AWTK_SRC)/ext_widgets
C_INCLUDES=-I$(AWTK_ROOT) -I$(AWTK_SRC) -I$(AWTK_SRC)/ext_widgets -I$(CWDDIR)/

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
CWDDIR:= $(dir $(mkfile_path))

include $(CWDDIR)/awtk_files.mk
include $(CWDDIR)/base_files.mk
include $(CWDDIR)/clipboard_files.mk
include $(CWDDIR)/ext_widgets_files.mk
include $(CWDDIR)/layouters_files.mk
include $(CWDDIR)/svg_files.mk
include $(CWDDIR)/tkc_files.mk
include $(CWDDIR)/ui_loader_files.mk
include $(CWDDIR)/vgcanvas_files.mk
include $(CWDDIR)/widget_animators_files.mk
include $(CWDDIR)/widgets_files.mk
include $(CWDDIR)/window_animators_files.mk
include $(CWDDIR)/xml_files.mk

