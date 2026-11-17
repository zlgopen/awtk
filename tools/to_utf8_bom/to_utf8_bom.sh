for f in src demos tests
do
  find $f -name \*.h -exec node ../awtk/tools/to_utf8_bom/index.js {} \;
  find $f -name \*.c -exec node ../awtk/tools/to_utf8_bom/index.js {} \;
  find $f -name \*.inc -exec node ../awtk/tools/to_utf8_bom/index.js {} \;
  find $f -name \*.cpp -exec node ../awtk/tools/to_utf8_bom/index.js {} \;
  find $f -name \*.cc -exec node ../awtk/tools/to_utf8_bom/index.js {} \;
done

