#!/bin/bash

DATE=`date "+%Y-%m-%d"`

YEAR=`date "+%Y"`

#查找clang-format(没有安装则跳过格式化)，查找顺序见find_clang_format.sh
. "`dirname "$0"`/../../find_clang_format.sh"
CLANG_FORMAT=`find_clang_format`

if [ -z "$CLANG_FORMAT" ]; then
  echo "clang-format not found, skip format"
else
  echo "using $CLANG_FORMAT"
fi

function format_code() {
  if [ -n "$CLANG_FORMAT" ]; then
    "$CLANG_FORMAT" -i "$@"
  fi
}

function gen_blend() {
  dst=$1
  DST=`echo $dst | tr a-z A-Z`
  src=$2
  SRC=`echo $src | tr a-z A-Z`
  echo "generating $dst $src $DATE"

  #有手写的 blend_a_$dst_$src.inc 时包含它，并定义 blend_a 指向它，否则删除模板中的占位符
  if [ -f blend_a_"$dst"_"$src".inc ]; then
    BLEND_A_INCLUDE_SED='s|{blend_a_include}|#include "blend_a_{dst}_{src}.inc"|'
    BLEND_A_DEFINE_SED='s|{blend_a_define}|#define blend_a blend_a_{dst}_{src}|'
  else
    #删除占位符时，连同占位符后面的空行一起删除，保证不留空行
    BLEND_A_INCLUDE_SED='/{blend_a_include}/d'
    BLEND_A_DEFINE_SED='/{blend_a_define}/{N;d}'
  fi

  sed -e "$BLEND_A_INCLUDE_SED" -e "$BLEND_A_DEFINE_SED" -e "s/{dst}/$dst/" -e "s/{DST}/$DST/" -e "s/{src}/$src/" -e "s/{SRC}/$SRC/" -e "s/{date}/$DATE/" -e "s/{YEAR}/$YEAR/" template/blend_image_c.tmpl > blend_image_"$dst"_"$src".c
  sed -e "s/{dst}/$dst/" -e "s/{DST}/$DST/" -e "s/{src}/$src/" -e "s/{SRC}/$SRC/" -e "s/{date}/$DATE/" -e "s/{YEAR}/$YEAR/" template/blend_image_h.tmpl > blend_image_"$dst"_"$src".h

  format_code blend_image_"$dst"_"$src".c blend_image_"$dst"_"$src".h
}

function gen_rotate() {
  dst=$1
  DST=`echo $dst | tr a-z A-Z`
  echo "generating rotate/fill $dst $DATE"

  sed -e "s/{dst}/$dst/" -e "s/{DST}/$DST/"  -e "s/{date}/$DATE/" -e "s/{YEAR}/$YEAR/" template/rotate_image_c.tmpl > rotate_image_"$dst".c
  sed -e "s/{dst}/$dst/" -e "s/{DST}/$DST/"  -e "s/{date}/$DATE/" -e "s/{YEAR}/$YEAR/" template/rotate_image_h.tmpl > rotate_image_"$dst".h

  format_code rotate_image_"$dst".c rotate_image_"$dst".h
}

function gen_fill() {
  dst=$1
  DST=`echo $dst | tr a-z A-Z`
  echo "generating rotate/fill $dst $DATE"

  sed -e "s/{dst}/$dst/" -e "s/{DST}/$DST/"  -e "s/{date}/$DATE/" -e "s/{YEAR}/$YEAR/" template/fill_image_c.tmpl > fill_image_"$dst".c
  sed -e "s/{dst}/$dst/" -e "s/{DST}/$DST/"  -e "s/{date}/$DATE/" -e "s/{YEAR}/$YEAR/" template/fill_image_h.tmpl > fill_image_"$dst".h

  format_code fill_image_"$dst".c fill_image_"$dst".h
}

#supported formats: rgb565 bgr565 rgb888 bgr888 rgba8888 abgr8888 bgra8888 argb8888

if [ "$1" == "" ]; then
  FLAG="Auto"
  DST="rgb565 bgr565 bgr888 bgra8888 rgba8888"
else
  FLAG="NotAuto"
  DST="$1"
fi

for dst in ${DST}
do
  gen_fill $dst
  gen_rotate $dst
  for src in bgr565 rgba8888 bgra8888
  do
    gen_blend $dst $src
  done
done

if [ "$FLAG" == "Auto" ] ; then
  gen_fill "rgb888"
  gen_fill "abgr8888"
  gen_fill "argb8888"
  gen_rotate "rgb888"
  gen_blend "bgr565" "rgb565"
  gen_blend "rgb565" "rgb565"
  gen_blend "bgr888" "rgb565"
  gen_blend "bgr888" "bgr888"
  gen_blend "rgb888" "bgr565"
  gen_blend "rgb888" "rgb565"
  gen_blend "rgb888" "rgb888"
  gen_blend "rgb888" "rgba8888"
  gen_blend "rgb888" "bgra8888"
  gen_blend "bgra8888" "rgb565"
  gen_blend "rgba8888" "rgb565"
fi
