find src -name \*.h -exec node tools/to_utf8_bom/index.js {} \;
find src -name \*.c -exec node tools/to_utf8_bom/index.js {} \;
find demos -name \*.c -exec node tools/to_utf8_bom/index.js {} \;

