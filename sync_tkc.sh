cp -rvf ../tkc/scripts/* scripts
cp -rfv ../tkc/tests/*test.cc tests
cp -rfv ../tkc/tests/fdb.cpp tests
cp -rfv ../tkc/tests/fscript_run.cpp tests
cp -fv ../tkc/tools/dll_def_gen/* tools/dll_def_gen/.
cp -fv ../tkc/tools/idl_gen/* tools/idl_gen/.
rm -f tools/dll_def_gen/package-lock.json
cp -rvf ../tkc/awtk_config_common.py .
cp -rf ../tkc/tests/fscripts/* tests/fscripts
cp -f ../tkc/3rd/gtest/googletest/include/gtest/gtest_helper.h 3rd/gtest/googletest/include/gtest/gtest_helper.h

for f in tkc.h compressors platforms conf_io hal streams ubjson charset csv	misc tkc xml debugger romfs
do
  cp -rvf ../tkc/src/$f src
done

cp -rvf ../tkc/src/fscript_ext/*.* src/fscript_ext


