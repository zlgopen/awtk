#include "base/types_def.h"

#ifndef STR_H
#define STR_H

BEGIN_C_DECLS

typedef struct _str_t{
   uint16_t ref;
   uint32_t size;
   uint32_t capacity;
   char* str;
}str_t;

str_t* str_create(const char* str, uint32_t size, uint32_t capacity);
str_t* str_sub(str_t* s, uint32_t start, uint32_t size);
bool_t str_extend(str_t* s, uint32_t capacity);
bool_t str_copy(str_t* s, str_t* other);
str_t* str_dup(str_t* s);

bool_t str_end_with(str_t* s, const char* str);
bool_t str_start_with(str_t* s, const char* str);

int    str_cmp(str_t* s, const char* str);
int    str_ncmp(str_t* s, const char* str, uint32_t size);
bool_t str_equal(str_t* s, const char* str);

int32_t   str_index_of_c(str_t* s, char c);
int32_t   str_last_index_of_c(str_t* s, char c);
int32_t   str_index_of(str_t* s, const char* str);

bool_t str_replace_c(str_t* s, char c, char new_char);
bool_t str_replace(str_t* s, const char* str, const char* new_str);

bool_t str_set(str_t* s, const char* str, uint32_t size);
bool_t str_append_char(str_t* s, char c);
bool_t str_append(str_t* s, const char* str, uint32_t size);

bool_t str_trim_left(str_t* s, const char* str);
bool_t str_trim_right(str_t* s, const char* str);
bool_t str_trim(str_t* s, const char* str);

int32_t str_to_int(str_t* s);
float   str_to_float(str_t* s);
double  str_to_double(str_t* s);

bool_t str_from_int(str_t* s, int32_t value);
bool_t str_from_float(str_t* s, float value);
bool_t str_from_double(str_t* s, double value);

str_t* str_basename(const char* filename);
str_t* str_dirname(const char* filename);
str_t* str_normalize_path(const char* filename, const char* cwd);

bool_t str_to_lower(str_t* s);
bool_t str_to_upper(str_t* s);

typedef void (*str_on_token_t)(void* ctx, str_t* token);

bool_t str_tokenizer(str_t* s, str_on_token_t on_token, void* ctx, 
        const char* delim, const char* ctokens); 

str_t* str_ref(str_t* s);
str_t* str_unref(str_t* s);

str_t* str_init_static(str_t* s, const char* str);

END_C_DECLS

#endif/*STR_H*/

