
AWTK_ROOT=../../
include ../common/awtk_all.mk

CC      = infer run -- clang -c
CPPFLAGS = -DHAS_STD_MALLOC -DNDEBUG $(C_INCLUDES)

ALL_FILES=$(TKC_FILES) \
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
	rm -f *.o 

.PHONY:clean
clean:
	rm -fr *.o infer-out
