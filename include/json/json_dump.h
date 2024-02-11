#ifndef JSON_DUMP_H_
#define JSON_DUMP_H_

#include "json/json_config.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef void * (*js_dump_buffer_fun_t)(js_size_t _size, void * _ud);

    typedef struct js_dump_ctx_t
    {
        js_dump_buffer_fun_t buffer;
        void * ud;
    } js_dump_ctx_t;

    void js_make_dump_ctx_buffer( js_buffer_t * _buffer, js_dump_ctx_t * const _ctx );
    void js_make_dump_ctx_default( js_dump_buffer_fun_t _fun, void * _ud, js_dump_ctx_t * const _ctx );

    js_result_t js_dump( const js_element_t * _element, js_dump_ctx_t * _ctx );

#ifdef __cplusplus
}
#endif

#endif