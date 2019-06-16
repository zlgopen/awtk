#!/bin/bash

DATE=`date "+%Y-%m-%d"`

function gen() {
  fmt=$1
  FMT=`echo $fmt | tr a-z A-Z`
  echo "generating $fmt $FMT $DATE"

  sed -e "s/{fmt}/$fmt/" -e "s/{FMT}/$FMT/" -e "s/{date}/$DATE/" template/lcd_mem_h.tmpl > lcd_mem_$fmt.h
  sed -e "s/{fmt}/$fmt/" -e "s/{FMT}/$FMT/" -e "s/{date}/$DATE/" template/lcd_mem_c.tmpl > lcd_mem_$fmt.c 
}

if [ "$1" == "" ]; then
  FMT="rgb565 bgr565 bgra5551 bgr888 bgra8888 rgba8888"
else
  FMT="$1"
fi
#supproted formats: rgb565 bgr565 rgb888 bgr888 rgba8888 abgr8888 bgra8888 argb8888
for fmt in ${FMT}
do
  gen $fmt
done
