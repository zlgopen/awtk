
for f in \
  "tests/fscripts/demo_app_conf.fs" \
  "tests/fscripts/demo_function1.fs" \
  "tests/fscripts/demo_typed_array1.fs" \
  "tests/fscripts/demo_array.fs" \
  "tests/fscripts/demo_function2.fs" \
  "tests/fscripts/demo_typed_array2.fs" \
  "tests/fscripts/demo_array1.fs"	\
  "tests/fscripts/demo_function3.fs" \
  "tests/fscripts/demo_ubjson.fs" \
  "tests/fscripts/demo_array2.fs"	\
  "tests/fscripts/demo_if1.fs"	\
  "tests/fscripts/demo_array3.fs" \
  "tests/fscripts/demo_json.fs"	\
  "tests/fscripts/demo_wbuffer.fs" \
  "tests/fscripts/demo_array4.fs"	\
  "tests/fscripts/demo_json1.fs"	\
  "tests/fscripts/demo_wbuffer1.fs" \
  "tests/fscripts/demo_array5.fs"	\
  "tests/fscripts/demo_local.fs"	\
  "tests/fscripts/demo_wbuffer2.fs" \
  "tests/fscripts/demo_array6.fs"	\
  "tests/fscripts/demo_object.fs" \
  "tests/fscripts/demo_wbuffer3.fs" \
  "tests/fscripts/demo_bits.fs"	\
  "tests/fscripts/demo_return.fs"	\
  "tests/fscripts/demo_wbuffer4.fs" \
  "tests/fscripts/demo_comments.fs"	\
  "tests/fscripts/demo_while1.fs" \
  "tests/fscripts/demo_consts.fs"	\
  "tests/fscripts/demo_str.fs"	\
  "tests/fscripts/demo_while2.fs" \
  "tests/fscripts/demo_while3.fs" \
  "tests/fscripts/demo_str3.fs" \
  "tests/fscripts/demo_while4.fs" \
  "tests/fscripts/demo_else_if.fs" \
  "tests/fscripts/demo_stream_file.fs" \
  "tests/fscripts/demo_zh_var_func.fs" \
  "tests/fscripts/demo_endian.fs"	\
  "tests/fscripts/demo_stream_file1.fs"	\
  "tests/fscripts/foo.fs" \
  "tests/fscripts/demo_expr.fs"	\
  "tests/fscripts/demo_fs.fs"	\
  "tests/fscripts/demo_typed_array.fs"
do
echo $f
./bin/runFScript "@"$f|grep assert
done

