# this is now the default FreeType build for Android
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# compile in ARM mode, since the glyph loader/renderer is a hotspot
# when loading complex pages in the browser

LOCAL_MODULE := libft2

SOURCE_PATH := ..

LOCAL_SRC_FILES := \
	$(SOURCE_PATH)/src/base/ftbbox.c \
	$(SOURCE_PATH)/src/base/ftbitmap.c \
	$(SOURCE_PATH)/src/base/ftglyph.c \
	$(SOURCE_PATH)/src/base/ftstroke.c \
	$(SOURCE_PATH)/src/base/ftxf86.c \
	$(SOURCE_PATH)/src/base/ftbase.c \
	$(SOURCE_PATH)/src/base/ftsystem.c \
	$(SOURCE_PATH)/src/base/ftinit.c \
	$(SOURCE_PATH)/src/base/ftgasp.c \
	$(SOURCE_PATH)/src/base/ftadvanc.c \
	$(SOURCE_PATH)/src/raster/raster.c \
	$(SOURCE_PATH)/src/sfnt/sfnt.c \
	$(SOURCE_PATH)/src/smooth/smooth.c \
	$(SOURCE_PATH)/src/autofit/autofit.c \
	$(SOURCE_PATH)/src/truetype/truetype.c \
	$(SOURCE_PATH)/src/cff/cff.c \
	$(SOURCE_PATH)/src/psnames/psnames.c \
	$(SOURCE_PATH)/src/pshinter/pshinter.c


LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/$(SOURCE_PATH)/builds \
	$(LOCAL_PATH)/$(SOURCE_PATH)/include

LOCAL_CFLAGS += -W -Wall -Wno-empty-body
LOCAL_CFLAGS += -fPIC -DPIC
LOCAL_CFLAGS += "-DDARWIN_NO_CARBON"
LOCAL_CFLAGS += "-DFT2_BUILD_LIBRARY"

# only enable this if you want to enable our ugly font hack
# to be able to switch the system's font hinting at runtime
# through our modified Development Settings. the change will
# only happen when you reboot or restart an application
#
LOCAL_CFLAGS += -DANDROID_FONT_HACK=1

# enable the FreeType internal memory debugger in the simulator
# you need to define the FT2_DEBUG_MEMORY environment variable
# when running the program to activate it. It will dump memory
# statistics when FT_Done_FreeType is called
#

# the following activates the new experimental auto-hinter
# one that should create less problems with a wide variety
# of glyphs, including our double-decker "g"
#LOCAL_CFLAGS += -DFT_OPTION_AUTOFIT2

# the following is for testing only, and should not be used in final builds
# of the product
#LOCAL_CFLAGS += "-DTT_CONFIG_OPTION_BYTECODE_INTERPRETER"

LOCAL_CFLAGS += -O2

include $(BUILD_STATIC_LIBRARY)

