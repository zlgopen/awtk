#include "tkc/fscript.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"

TEST(FScript, basic1) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "sum(1, 2)", &v);
	ASSERT_EQ(3, value_int(&v));
	value_reset(&v);
	OBJECT_UNREF(obj);
}

TEST(FScript, basic2) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "sum(1, 2);sum(1, 2, 3, 4)", &v);
	ASSERT_EQ(10, value_int(&v));
	value_reset(&v);
	OBJECT_UNREF(obj);
}

TEST(FScript, basic3) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "sum(1, 2)\nsum(1, 2, 3, 4)\nsum(123, 321)", &v);
	ASSERT_EQ(444, value_int(&v));
	value_reset(&v);
	OBJECT_UNREF(obj);
}

TEST(FScript, basic4) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "print(sum(1, 2))", &v);
	ASSERT_EQ(true, value_bool(&v));
	value_reset(&v);
	OBJECT_UNREF(obj);
}

TEST(FScript, if1) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "if(1, \"hello\", \"world\")", &v);
	ASSERT_STREQ(value_str(&v), "hello");
	value_reset(&v);
	
	fscript_eval(obj, "if(0, \"hello\", \"world\")", &v);
	ASSERT_STREQ(value_str(&v), "world");
	value_reset(&v);
	OBJECT_UNREF(obj);
}

TEST(FScript, if2) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "if(true, \"hello\", \"world\")", &v);
	ASSERT_STREQ(value_str(&v), "hello");
	value_reset(&v);
	
	fscript_eval(obj, "if(false, \"hello\", \"world\")", &v);
	ASSERT_STREQ(value_str(&v), "world");
	value_reset(&v);
	OBJECT_UNREF(obj);
}

TEST(FScript, if3) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "if(true, sum(1, 2), sum(1, 2, 3)", &v);
	ASSERT_EQ(value_int(&v), 3);
	value_reset(&v);
	
	fscript_eval(obj, "if(false, sum(1, 2), sum(1, 2, 3)", &v);
	ASSERT_EQ(value_int(&v), 6);
	value_reset(&v);
	OBJECT_UNREF(obj);
}

TEST(FScript, convert) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "int(\"3\")", &v);
	ASSERT_EQ(value_int(&v), 3);
	value_reset(&v);
	
	fscript_eval(obj, "float(\"3.5\")", &v);
	ASSERT_EQ(value_float(&v), 3.5);
	value_reset(&v);
	
	fscript_eval(obj, "str(123)", &v);
	ASSERT_STREQ(value_str(&v), "123.000000");
	value_reset(&v);
	
	fscript_eval(obj, "str(int(123))", &v);
	ASSERT_STREQ(value_str(&v), "123");
	value_reset(&v);
	OBJECT_UNREF(obj);
}

TEST(FScript, join) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "join(\",\",\"a\", \"b\", \"c\")", &v);
	ASSERT_STREQ(value_str(&v), "a,b,c");
	value_reset(&v);
	
	OBJECT_UNREF(obj);
}

TEST(FScript, set) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "set(a, 123)sum(get(a), 321)", &v);
	ASSERT_EQ(value_int(&v), 444);
	value_reset(&v);
	
	OBJECT_UNREF(obj);
}

TEST(FScript, iformat) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "iformat(\"hello:%d\", 123)", &v);
	ASSERT_STREQ(value_str(&v), "hello:123");
	value_reset(&v);
	
	OBJECT_UNREF(obj);
}

TEST(FScript, fformat) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "fformat(\"hello:%lf\", 123)", &v);
	ASSERT_STREQ(value_str(&v), "hello:123.000000");
	value_reset(&v);
	
	OBJECT_UNREF(obj);
}

TEST(FScript, mod) {
	value_t v;
	object_t* obj = object_default_create();
	fscript_eval(obj, "%(23, 7)", &v);
	ASSERT_EQ(value_int(&v), 2);
	value_reset(&v);
	
	OBJECT_UNREF(obj);
}

TEST(FScript, and) {
	value_t v;
	object_t* obj = object_default_create();
	
	fscript_eval(obj, "and(true, true)", &v);
	ASSERT_EQ(value_bool(&v), true);
	value_reset(&v);

	fscript_eval(obj, "and(true, false)", &v);
	ASSERT_EQ(value_bool(&v), false);
	value_reset(&v);
	
	fscript_eval(obj, "and(false, false)", &v);
	ASSERT_EQ(value_bool(&v), false);
	value_reset(&v);
	OBJECT_UNREF(obj);
}

TEST(FScript, or) {
	value_t v;
	object_t* obj = object_default_create();
	
	fscript_eval(obj, "or(true, true)", &v);
	ASSERT_EQ(value_bool(&v), true);
	value_reset(&v);

	fscript_eval(obj, "||(true, false)", &v);
	ASSERT_EQ(value_bool(&v), true);
	value_reset(&v);
	
	fscript_eval(obj, "||(false, false)", &v);
	ASSERT_EQ(value_bool(&v), false);
	value_reset(&v);
	OBJECT_UNREF(obj);
}

TEST(FScript, not) {
	value_t v;
	object_t* obj = object_default_create();
	
	fscript_eval(obj, "not(true)", &v);
	ASSERT_EQ(value_bool(&v), false);
	value_reset(&v);

	fscript_eval(obj, "!(false)", &v);
	ASSERT_EQ(value_bool(&v), true);
	value_reset(&v);
	
	OBJECT_UNREF(obj);
}

TEST(FScript, bit_and) {
	value_t v;
	object_t* obj = object_default_create();
	
	fscript_eval(obj, "&(1, 1)", &v);
	ASSERT_EQ(value_int(&v), 1);
	value_reset(&v);
	
	fscript_eval(obj, "&(1, 2)", &v);
	ASSERT_EQ(value_int(&v), 0);
	value_reset(&v);

	OBJECT_UNREF(obj);
}

TEST(FScript, bit_or) {
	value_t v;
	object_t* obj = object_default_create();
	
	fscript_eval(obj, "|(1, 1)", &v);
	ASSERT_EQ(value_int(&v), 1);
	value_reset(&v);
	
	fscript_eval(obj, "|(1, 2)", &v);
	ASSERT_EQ(value_int(&v), 3);
	value_reset(&v);

	OBJECT_UNREF(obj);
}

TEST(FScript, bit_not) {
	value_t v;
	object_t* obj = object_default_create();
	
	fscript_eval(obj, "~(1)", &v);
	ASSERT_EQ(value_int(&v), ~1);
	value_reset(&v);
	
	OBJECT_UNREF(obj);
}

TEST(FScript, less) {
	value_t v;
	object_t* obj = object_default_create();
	
	fscript_eval(obj, "<(1, 2)", &v);
	ASSERT_EQ(value_bool(&v), true);
	value_reset(&v);
	
	fscript_eval(obj, "<(2, 1)", &v);
	ASSERT_EQ(value_bool(&v), false);
	value_reset(&v);
	
	OBJECT_UNREF(obj);
}

TEST(FScript, le) {
	value_t v;
	object_t* obj = object_default_create();
	
	fscript_eval(obj, "<=(1, 2)", &v);
	ASSERT_EQ(value_bool(&v), true);
	value_reset(&v);
	
	fscript_eval(obj, "<=(2, 1)", &v);
	ASSERT_EQ(value_bool(&v), false);
	value_reset(&v);
	
	OBJECT_UNREF(obj);
}

TEST(FScript, great) {
	value_t v;
	object_t* obj = object_default_create();
	
	fscript_eval(obj, ">(1, 2)", &v);
	ASSERT_EQ(value_bool(&v), false);
	value_reset(&v);
	
	fscript_eval(obj, ">(2, 1)", &v);
	ASSERT_EQ(value_bool(&v), true);
	value_reset(&v);
	
	OBJECT_UNREF(obj);
}

TEST(FScript, ge) {
	value_t v;
	object_t* obj = object_default_create();
	
	fscript_eval(obj, "ge(1, 2)", &v);
	ASSERT_EQ(value_bool(&v), false);
	value_reset(&v);
	
	fscript_eval(obj, "ge(2, 1)", &v);
	ASSERT_EQ(value_bool(&v), true);
	value_reset(&v);
	
	OBJECT_UNREF(obj);
}

TEST(FScript, eq) {
	value_t v;
	object_t* obj = object_default_create();
	
	fscript_eval(obj, "eq(1, 2)", &v);
	ASSERT_EQ(value_bool(&v), false);
	value_reset(&v);
	
	fscript_eval(obj, "eq(2, 2)", &v);
	ASSERT_EQ(value_bool(&v), true);
	value_reset(&v);
	
	fscript_eval(obj, "eq(\"2\", \"2\")", &v);
	ASSERT_EQ(value_bool(&v), true);
	value_reset(&v);
	OBJECT_UNREF(obj);
}
