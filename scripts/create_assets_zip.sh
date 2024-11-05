#!/bin/bash

if [ "$1" == "" ]
then
  SRC=res/assets
else
  SRC="$1"
fi

if [ "$2" == "" ]
then
  DST="../assets.zip"
else
  DST="$2"
fi

echo "SRC=$SRC"
echo "DST=$DST"

if [ -e "$SRC" ] 
then
  echo "preparing..."
else
  echo "$SRC not exists"
  exit 0
fi

rm -rf temp
rm -f $DST

mkdir temp
cp -rf "$SRC" temp 

cd temp/assets
rm -f *.inc

for f in *;
do 
  if [ -d $f ]
  then
    echo "clean " $f;
    rm -rf $f/inc
    rm -rf $f/raw/ui/*.xml
    rm -rf $f/raw/styles/*.xml
    rm -rf $f/raw/strings/*.xml
  fi
done

cd ..

zip -r $DST assets
echo "assets saved to $DST:"
ls -l $DST

cd ..
rm -rf temp


