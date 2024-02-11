#ifndef JSON_STRING_H_
#define JSON_STRING_H_

#include "json/json_config.h"

const char * js_strpbrk( const char * _begin, const char * _end, const char * _str );
js_bool_t js_chrskip( char _ch, const char * _str );
const char * js_strskip( const char * _begin, const char * _end, const char * _str );
const char * js_strstr( const char * _begin, const char * _end, const char * _str );
const char * js_strchr( const char * _begin, const char * _end, char _ch );
js_bool_t js_strncmp( const char * _s1, const char * _s2, js_size_t _n );
js_bool_t js_strzcmp( js_string_t _s1, js_string_t _s2 );

#endif