#!/bin/bash

SRC=$1

if [ -z "$SRC" ] 
  then
    echo "Usage: $0 assets_dir"
    exit 0
  fi

rm -rf temp
rm -f assets.zip

mkdir temp
cp -rf "$SRC" temp 

cd temp/assets

for f in *;
do 
  if [ -d $f ]
  then
    echo "clean " $f;
    rm -rvf $f/inc
    rm -rvf $f/raw/ui/*.xml
    rm -rvf $f/raw/styles/*.xml
    rm -rvf $f/raw/strings/*.xml
  fi
done

cd ..

zip -r ../assets.zip assets

cd ..

rm -rf temp

ls -l assets.zip

