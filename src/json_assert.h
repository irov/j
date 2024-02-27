#ifndef JSON_ASSERT_H_
#define JSON_ASSERT_H_

#include "json/json_config.h"

#ifndef JS_ASSERT
#   if defined(JS_DEBUG) && !defined(JS_NO_CRT_ASSERT)
#       include <assert.h>

#       define JS_ASSERT(X, File, Line) assert(X)
#   else
#       define JS_ASSERT(X, File, Line) ((void)0)
#   endif
#endif

#endif