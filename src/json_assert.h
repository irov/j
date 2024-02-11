#ifndef JSON_ASSERT_H_
#define JSON_ASSERT_H_

#ifndef JS_ASSERT
#   if defined(JS_DEBUG) && !defined(JS_NO_CRT_ASSERT)
#       include <assert.h>

#       define JS_ASSERT(X) assert(X)
#   else
#       define JS_ASSERT(X) ((void)0)
#   endif
#endif

#endif