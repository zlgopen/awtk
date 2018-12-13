
genImages() {
  dot dots/*  -Tpng -O
  java -jar plantuml.jar umls/*.uml

  mv -v dots/*.png docs/images
  mv -v umls/*.png docs/images
}

for d in dots umls docs/images;
do
  rm -rf $d 
  mkdir -p $d
done

node index.js
genImages
