#!/bin/bash

genImages() {
  dot dots/*  -Tpng -O
  java -jar plantuml.jar umls/*.uml

  mv -v dots/*.png docs/images
  mv -v umls/*.png docs/images
}

if [ "$1" == "" ]
then
  node index.js
else
  node index.js "$1"
fi

genImages
