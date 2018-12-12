
genImages() {
  rm -rf docs/images
  mkdir docs/images

  cd dots
  for f in *.dot;
  do
    dot $f -Tpng -o ../docs/images/"${f/\.dot/.png}"
  done
  cd -
}

node index.js
genImages
