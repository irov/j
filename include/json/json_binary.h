#ifndef JSON_BINARY_H_
#define JSON_BINARY_H_

#include "json.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef void (*js_binary_write_fun_t)(const void * _buffer, js_size_t _size, void * _ud);

    typedef struct js_binary_ctx_t
    {
        js_binary_write_fun_t write;
        void * ud;
    } js_binary_ctx_t;

    void js_binary( const js_element_t * _element, js_binary_ctx_t * _ctx );

#ifdef __cplusplus
}
#endif

#endif