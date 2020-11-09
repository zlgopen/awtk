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
	fscript_eval(obj, "sum(1, 2)\nsum(1, 2, 3, 4)", &v);
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

