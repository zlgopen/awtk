LOCAL_PATH:= $(call my-dir)

SOURCE_PATH := ..

common_SRC_FILES := \
	$(SOURCE_PATH)/lib/xmlparse.c \
	$(SOURCE_PATH)/lib/xmlrole.c \
	$(SOURCE_PATH)/lib/xmltok.c

common_CFLAGS := \
    -Wall \
    -Wmissing-prototypes -Wstrict-prototypes \
    -Wno-unused-parameter -Wno-missing-field-initializers \
    -fexceptions \
    -DHAVE_EXPAT_CONFIG_H

common_C_INCLUDES += \
	$(LOCAL_PATH)/$(SOURCE_PATH) \
	$(LOCAL_PATH)/$(SOURCE_PATH)/lib

# For the device
# =====================================================

# Device static library
include $(CLEAR_VARS)

ifeq ($(TARGET_ARCH),arm)
LOCAL_SDK_VERSION := 8
endif

LOCAL_SRC_FILES := $(common_SRC_FILES)
LOCAL_CFLAGS += $(common_CFLAGS)
LOCAL_C_INCLUDES += $(common_C_INCLUDES)

LOCAL_MODULE:= libexpat

include $(BUILD_STATIC_LIBRARY)
