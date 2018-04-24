#!/bin/sh

export NDK_ROOT=/d/android-ndk-r8b-windows/android-ndk-r8b

export PROJECT_ROOT=`pwd`

export NDK_PROJECT_PATH=${PROJECT_ROOT}/android/freetype/
${NDK_ROOT}/ndk-build

export NDK_PROJECT_PATH=${PROJECT_ROOT}/android/expat/
${NDK_ROOT}/ndk-build

export NDK_PROJECT_PATH=${PROJECT_ROOT}/android/
${NDK_ROOT}/ndk-build
