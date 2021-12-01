cp -rvf ../tkc/scripts/* scripts
cp -rfv ../tkc/tests/*test.cc tests
cp -rfv ../tkc/tools/* tools

for f in compressors fscript_ext platforms conf_io hal streams ubjson charset csv	misc tkc xml
do
  cp -rvf ../tkc/src/$f src
done


