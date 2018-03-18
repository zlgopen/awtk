#!/bin/bash

BIN_DIR=./bin
RAW_DIR=demos/res/raw
SRC_DIR=demos/res/src

rm -rf $SRC_DIR
mkdir -p $SRC_DIR/theme
mkdir -p $SRC_DIR/images
mkdir -p $SRC_DIR/fonts
mkdir -p $SRC_DIR/ui

$BIN_DIR/fontgen $RAW_DIR/fonts/font.ttf  $RAW_DIR/fonts/text.txt $SRC_DIR/fonts/font20.data 20
$BIN_DIR/themegen $RAW_DIR/theme/theme.xml $SRC_DIR/theme/default.data

for f in $RAW_DIR/images/*.png; 
do 
  F1=${f/raw/src}; 
  F2=${F1/\.png/.data};
  F3=${F2/\.jpg/.data};

  $BIN_DIR/imagegen $f $F3
done

for f in $RAW_DIR/ui/*.xml
do 
  F1=${f/raw/src}; 
  F2=${F1/\.xml/.data};

  $BIN_DIR/xml_to_ui $f $F2
done

function gen_res_c() {
  echo '#include "lftk.h"'
  echo '#include "base/resource_manager.h"'
  find demos/res/src -name \*.data|sed -e 's/demos\//#include "/g' -e 's/$/\"/g'

  echo ''
  echo 'ret_t resource_init() {'
  echo '  resource_manager_init(15);'
  echo ''
  find demos/res/src -name \*.data|sed -e 's/demos\/res\/src\//  resource_manager_add(/g' -e 's/s\//_/g' -e 's/\//_/g' -e 's/\.data/);/g'
  echo ''
  echo '  lftk_init_resources();'
  echo '  return RET_OK;'
  echo '}'
}

gen_res_c > demos/resource.c


