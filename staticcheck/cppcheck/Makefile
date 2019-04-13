
AWTK_ROOT=../../
include ../common/awtk_all.mk

CC      = cppcheck
EXTRA_FLAGS= --enable=warning  --enable=performance -DWITH_CPPCHECK
CPPFLAGS = -DHAS_STD_MALLOC -DNDEBUG $(C_INCLUDES)

ALL_FILES=$(TKC_FILES) \
					$(XML_FILES) \
					$(BASE_FILES) \
					$(LAYOUTERS_FILES) \
					$(WIDGETS_FILES) \
					$(UI_LOADER_FILES) \
					$(SVG_FILES) \
					$(CLIPBOARD_FILES) \
					$(WIDGET_ANIMATORS_FILES) \
					$(WINDOW_ANIMATORS_FILES) \
					$(EXT_WIDGETS_FILES) \
					$(AWTK_FILES) 


all:
	$(CC) $(CPPFLAGS) $(EXTRA_FLAGS) $(ALL_FILES) 

.PHONY:clean
clean:
	-rm -f *.o
