# this is now the default FreeType build for Android
#
ifndef USE_FREETYPE
USE_FREETYPE := 2.3.6
endif

ifeq ($(USE_FREETYPE),2.3.6)
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# compile in ARM mode, since the glyph loader/renderer is a hotspot
# when loading complex pages in the browser
#
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES:= \
	src/base/ftbbox.c \
	src/base/ftbitmap.c \
	src/base/ftglyph.c \
	src/base/ftstroke.c \
	src/base/ftxf86.c \
	src/base/ftbase.c \
	src/base/ftsystem.c \
	src/base/ftinit.c \
	src/base/ftgasp.c \
	src/base/ftadvanc.c \
	src/raster/raster.c \
	src/sfnt/sfnt.c \
	src/smooth/smooth.c \
	src/autofit/autofit.c \
	src/truetype/truetype.c \
	src/cff/cff.c \
	src/psnames/psnames.c \
	src/pshinter/pshinter.c

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/builds \
	$(LOCAL_PATH)/include

LOCAL_CFLAGS += -W -Wall
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
ifeq ($(TARGET_SIMULATOR),true)
LOCAL_CFLAGS += "-DFT_DEBUG_MEMORY"
endif

# the following activates the new experimental auto-hinter
# one that should create less problems with a wide variety
# of glyphs, including our double-decker "g"
#LOCAL_CFLAGS += -DFT_OPTION_AUTOFIT2

# the following is for testing only, and should not be used in final builds
# of the product
#LOCAL_CFLAGS += "-DTT_CONFIG_OPTION_BYTECODE_INTERPRETER"

ifeq ($(TARGET_BUILD_TYPE),release)
	LOCAL_CFLAGS += -O2
endif

LOCAL_MODULE:= libft2

include $(BUILD_STATIC_LIBRARY)
endif

