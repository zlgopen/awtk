#!/bin/bash

if [ "$1" == "" ]
then
  SRC=res/assets
else
  SRC="$1"
fi

echo "SRC=$SRC"

if [ -e "$SRC" ] 
then
  echo "preparing..."
else
  echo "res/assets not exists"
  exit 0
fi

rm -rf temp
rm -f assets.zip

mkdir temp
cp -rf "$SRC" temp 

cd temp/assets
rm -fv *.inc

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

echo "assets saved to assets.zip:"
ls -l assets.zip

