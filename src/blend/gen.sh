#!/bin/bash

DATE=`date "+%Y-%m-%d"`

function gen_blend() {
  dst=$1
  DST=`echo $dst | tr a-z A-Z`
  src=$2
  SRC=`echo $src | tr a-z A-Z`
  echo "generating $dst $src $DATE"

  sed -e "s/{dst}/$dst/" -e "s/{DST}/$DST/" -e "s/{src}/$src/" -e "s/{SRC}/$SRC/" -e "s/{date}/$DATE/" template/blend_image_c.tmpl > blend_image_"$dst"_"$src".c
  sed -e "s/{dst}/$dst/" -e "s/{DST}/$DST/" -e "s/{src}/$src/" -e "s/{SRC}/$SRC/" -e "s/{date}/$DATE/" template/blend_image_h.tmpl > blend_image_"$dst"_"$src".h
}

function gen_rotate_fill() {
  dst=$1
  DST=`echo $dst | tr a-z A-Z`
  echo "generating rotate/fill $dst $DATE"

  sed -e "s/{dst}/$dst/" -e "s/{DST}/$DST/"  -e "s/{date}/$DATE/" template/rotate_image_c.tmpl > rotate_image_"$dst".c
  sed -e "s/{dst}/$dst/" -e "s/{DST}/$DST/"  -e "s/{date}/$DATE/" template/rotate_image_h.tmpl > rotate_image_"$dst".h
  sed -e "s/{dst}/$dst/" -e "s/{DST}/$DST/"  -e "s/{date}/$DATE/" template/fill_image_c.tmpl > fill_image_"$dst".c
  sed -e "s/{dst}/$dst/" -e "s/{DST}/$DST/"  -e "s/{date}/$DATE/" template/fill_image_h.tmpl > fill_image_"$dst".h
}

#supported formats: rgb565 bgr565 rgb888 bgr888 rgba8888 abgr8888 bgra8888 argb8888

if [ "$1" == "" ]; then
  DST="rgb565 bgr565 bgr888 bgra8888 rgba8888"
else
  DST="$1"
fi

for dst in ${DST}
do
  gen_rotate_fill $dst
  for src in bgr565 rgba8888 bgra8888
  do
    gen_blend $dst $src
  done
done
