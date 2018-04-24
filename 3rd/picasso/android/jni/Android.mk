

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

SOURCE_PATH := ../..


LOCAL_MODULE := picasso

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/$(SOURCE_PATH)/include/ \
	$(LOCAL_PATH)/$(SOURCE_PATH)/src/ \
	$(LOCAL_PATH)/$(SOURCE_PATH)/src/include/ \
	$(LOCAL_PATH)/$(SOURCE_PATH)/src/gfx/ \
	$(LOCAL_PATH)/$(SOURCE_PATH)/src/gfx/include/

LOCAL_SRC_FILES := \
	$(SOURCE_PATH)/src/core/curve.cpp \
	$(SOURCE_PATH)/src/core/device.cpp \
	$(SOURCE_PATH)/src/core/graphic_path.cpp \
	$(SOURCE_PATH)/src/gfx/gfx_device.cpp \
	$(SOURCE_PATH)/src/gfx/gfx_font_adapter_freetype2.cpp \
	$(SOURCE_PATH)/src/gfx/gfx_font_load_freetype2.cpp \
	$(SOURCE_PATH)/src/gfx/gfx_image_filters.cpp \
	$(SOURCE_PATH)/src/gfx/gfx_blur.cpp \
	$(SOURCE_PATH)/src/gfx/gfx_gradient_adapter.cpp \
	$(SOURCE_PATH)/src/gfx/gfx_raster_adapter.cpp \
	$(SOURCE_PATH)/src/gfx/gfx_rendering_buffer.cpp \
	$(SOURCE_PATH)/src/gfx/gfx_sqrt_tables.cpp \
	$(SOURCE_PATH)/src/picasso_api.cpp \
	$(SOURCE_PATH)/src/picasso_canvas.cpp \
	$(SOURCE_PATH)/src/picasso_font_api.cpp \
	$(SOURCE_PATH)/src/picasso_font.cpp \
	$(SOURCE_PATH)/src/picasso_gpc.cpp \
	$(SOURCE_PATH)/src/picasso_gradient_api.cpp \
	$(SOURCE_PATH)/src/picasso_gradient.cpp \
	$(SOURCE_PATH)/src/picasso_image.cpp \
	$(SOURCE_PATH)/src/picasso_mask_api.cpp \
	$(SOURCE_PATH)/src/picasso_mask.cpp \
	$(SOURCE_PATH)/src/picasso_matrix_api.cpp \
	$(SOURCE_PATH)/src/picasso_matrix.cpp \
	$(SOURCE_PATH)/src/picasso_painter.cpp \
	$(SOURCE_PATH)/src/picasso_path.cpp \
	$(SOURCE_PATH)/src/picasso_pattern.cpp \
	$(SOURCE_PATH)/src/picasso_raster_adapter.cpp \
	$(SOURCE_PATH)/src/picasso_rendering_buffer.cpp

LOCAL_CPPFLAGS := -DEXPORT=1 -DNDEBUG=1 -D__ANDROID__=1 \
				  -O3 -Wall -fPIC -march=armv7-a -mfpu=neon -ftree-vectorize -mfloat-abi=softfp \
				  -fno-rtti \
				  -fno-exceptions \
				  -Wno-unused-result

LOCAL_CFLAGS := $(LOCAL_CPPFLAGS)

LOCAL_LDLIBS := \
	$(LOCAL_PATH)/$(SOURCE_PATH)/android/freetype/obj/local/armeabi/libft2.a \
	$(LOCAL_PATH)/$(SOURCE_PATH)/android/expat/obj/local/armeabi/libexpat.a


include $(BUILD_STATIC_LIBRARY)



# test application
 
include $(CLEAR_VARS)

LOCAL_MODULE := picasso-test

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/$(SOURCE_PATH)/include/ \
	$(LOCAL_PATH)/$(SOURCE_PATH)/test/

LOCAL_CPPFLAGS := -O3 -Wall

LOCAL_CFLAGS := $(LOCAL_CPPFLAGS)

LOCAL_SRC_FILES := test_android.cpp \
        $(SOURCE_PATH)/test/alpha_func.c

LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := android_native_app_glue picasso

LOCAL_LDFLAGS := \
	$(LOCAL_PATH)/$(SOURCE_PATH)/android/obj/local/armeabi/libpicasso.a \
	$(LOCAL_PATH)/$(SOURCE_PATH)/android/freetype/obj/local/armeabi/libft2.a \
	$(LOCAL_PATH)/$(SOURCE_PATH)/android/expat/obj/local/armeabi/libexpat.a

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
