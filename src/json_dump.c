#include "json_dump.h"

//////////////////////////////////////////////////////////////////////////
static void __js_memcpy( char * _dst, const char * _src, js_size_t _size )
{
    while( _size-- )
    {
        *_dst++ = *_src++;
    }
}
//////////////////////////////////////////////////////////////////////////
static void __js_dump_char( js_dump_ctx_t * _ctx, char _value )
{
    char * dst = (*_ctx->buffer)(1, _ctx->ud);

    if( dst == JS_NULLPTR )
    {
        return;
    }

    *(dst) = _value;
}
//////////////////////////////////////////////////////////////////////////
static void __js_dump_string_internal( js_dump_ctx_t * _ctx, const char * _value, size_t _size )
{
    char * dst = (*_ctx->buffer)(_size, _ctx->ud);

    if( dst == JS_NULLPTR )
    {
        return;
    }

    __js_memcpy( dst, _value, _size );
}
//////////////////////////////////////////////////////////////////////////
static void __js_dump_string( js_dump_ctx_t * _ctx, js_string_t _value )
{
    __js_dump_string_internal( _ctx, _value.value, _value.size );
}
//////////////////////////////////////////////////////////////////////////
#define JS_DUMP_INTERNAL(data, value) __js_dump_string_internal(data, value, sizeof(value) - 1)
//////////////////////////////////////////////////////////////////////////
#define JS_MAX_INTEGER_SYMBOLS 20
//////////////////////////////////////////////////////////////////////////
static void __js_dump_integer( js_dump_ctx_t * _ctx, js_integer_t _value )
{
    char symbols[JS_MAX_INTEGER_SYMBOLS];

    char * it = symbols + JS_MAX_INTEGER_SYMBOLS;
    
    if( _value == 0 )
    {
        *--it = '0';
    } 
    else if( _value < 0 )
    {
        _value = -_value;

        while( _value )
        {
            js_integer_t symbol = _value % 10;
            _value /= 10;

            *--it = '0' + (char)symbol;
        }

        *--it = '-';
    }
    else
    {
        while( _value )
        {
            js_integer_t symbol = _value % 10;
            _value /= 10;

            *--it = '0' + (char)symbol;
        }
    }

    js_size_t symbols_size = JS_MAX_INTEGER_SYMBOLS - (it - symbols);

    char * dst = (*_ctx->buffer)(symbols_size, _ctx->ud);

    if( dst == JS_NULLPTR )
    {
        return;
    }

    __js_memcpy( dst, it, symbols_size );
}
//////////////////////////////////////////////////////////////////////////
static void __js_dump_double( js_dump_ctx_t * _ctx, double _value, int32_t _precision )
{
    if( _value == 0.0 )
    {
        JS_DUMP_INTERNAL( _ctx, "0.0" );

        return;
    }

    if( _value < 0.0 )
    {
        _value = -_value;

        __js_dump_char( _ctx, '-' );
    }

    double r = 0.0000000000000005;
    double fr = _value + r;

    int64_t i = (int64_t)fr;
    fr -= (double)i;

    if( i == 0 )
    {
        __js_dump_char( _ctx, '-' );
    }
    else
    {
        js_size_t n = 0;
        
        int64_t in = i;
        while( in != 0 )
        {
            in /= 10;
            ++n;
        }

        char * dst = (*_ctx->buffer)(n, _ctx->ud);

        if( dst == JS_NULLPTR )
        {
            return;
        }

        char * p = dst + n;

        while( n-- )
        {
            int64_t symbol = i % 10;
            i /= 10;

            *--p = '0' + (char)symbol;
        }
    }

    __js_dump_char( _ctx, '.' );

    if( fr == 0.0 )
    {
        __js_dump_char( _ctx, '0' );        
    }
    else
    {
        js_size_t n = 0;
        js_size_t nz = 0;

        double frn = fr;

        while( _precision-- )
        {
            frn *= 10.0;

            char c = (char)frn;
            frn -= c;

            ++n;

            if( c != 0 )
            {
                nz = n;
            }
        }

        if( nz == 0 )
        {
            __js_dump_char( _ctx, '0' );
        }
        else
        {
            char * dst = (*_ctx->buffer)(nz, _ctx->ud);

            if( dst == JS_NULLPTR )
            {
                return;
            }

            while( nz-- )
            {
                fr *= 10.0;

                char c = (char)fr;
                fr -= c;

                *dst++ = '0' + c;
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////
static void __js_dump_element( js_dump_ctx_t * _ctx, const js_element_t * _element );
//////////////////////////////////////////////////////////////////////////
static void __js_dump_array_element( js_size_t _index, const js_element_t * _value, void * _ud )
{
    js_dump_ctx_t * ctx = (js_dump_ctx_t *)_ud;

    if( _index != 0 )
    {
        __js_dump_char( ctx, ',' );
    }

    __js_dump_element( ctx, _value );
}
//////////////////////////////////////////////////////////////////////////
static void __js_dump_array( js_dump_ctx_t * _ctx, const js_element_t * _element )
{
    __js_dump_char( _ctx, '[' );
    js_array_foreach( _element, &__js_dump_array_element, _ctx );
    __js_dump_char( _ctx, ']' );
}
//////////////////////////////////////////////////////////////////////////
static void __js_dump_object_element( js_size_t _index, const js_element_t * _key, const js_element_t * _value, void * _ud )
{
    js_dump_ctx_t * ctx = (js_dump_ctx_t *)_ud;

    if( _index != 0 )
    {
        __js_dump_char( ctx, ',' );
    }

    __js_dump_char( ctx, '"' );

    js_string_t key;
    js_get_string( _key, &key );

    __js_dump_string( ctx, key );
    __js_dump_char( ctx, '"' );

    __js_dump_char( ctx, ':' );

    __js_dump_element( ctx, _value );
}
//////////////////////////////////////////////////////////////////////////
static void __js_dump_object( js_dump_ctx_t * _ctx, const js_element_t * _element )
{
    __js_dump_char( _ctx, '{' );
    js_object_foreach( _element, &__js_dump_object_element, _ctx );
    __js_dump_char( _ctx, '}' );
}
//////////////////////////////////////////////////////////////////////////
static void __js_dump_element( js_dump_ctx_t * _ctx, const js_element_t * _element )
{
    js_type_t type = js_type( _element );

    switch( type )
    {
    case js_type_null:
        {
            JS_DUMP_INTERNAL( _ctx, "null" );
        }break;
    case js_type_false:
        {
            JS_DUMP_INTERNAL( _ctx, "false" );
        }break;
    case js_type_true:
        {
            JS_DUMP_INTERNAL( _ctx, "true" );
        }break;
    case js_type_integer:
        {
            js_integer_t value = js_get_integer( _element );

            __js_dump_integer( _ctx, value );
        }break;
    case js_type_real:
        {
            js_real_t value = js_get_real( _element );

            __js_dump_double( _ctx, value, 6 );
        }break;
    case js_type_string:
        {
            js_string_t str;
            js_get_string( _element, &str );

            __js_dump_char( _ctx, '"' );
            __js_dump_string( _ctx, str );
            __js_dump_char( _ctx, '"' );
        }break;
    case js_type_array:
        {
            __js_dump_array( _ctx, _element );
        }break;
    case js_type_object:
        {
            __js_dump_object( _ctx, _element );
        }break;
    }
}
//////////////////////////////////////////////////////////////////////////
static char * __js_dump_buffer( js_size_t _size, void * _ud )
{
    js_buffer_t * buffer = (js_buffer_t *)_ud;

    if( buffer->memory + _size > buffer->end )
    {
        buffer->memory = buffer->end;

        return JS_NULLPTR;
    }

    uint8_t * new_buffer = buffer->memory;

    buffer->memory += _size;

    return (char *)new_buffer;
}
//////////////////////////////////////////////////////////////////////////
void js_make_dump_ctx_buffer( js_buffer_t * _buffer, js_dump_ctx_t * const _ctx )
{
    _ctx->buffer = &__js_dump_buffer;
    _ctx->ud = _buffer;
}
//////////////////////////////////////////////////////////////////////////
js_result_t js_dump( const js_element_t * _element, js_dump_ctx_t * _ctx )
{
    __js_dump_object( _ctx, _element );

    char * dst = _ctx->buffer( 1, _ctx->ud );

    if( dst == JS_NULLPTR )
    {
        return JS_FAILURE;
    }

    *dst = '\0';

    return JS_SUCCESSFUL;
}
//////////////////////////////////////////////////////////////////////////