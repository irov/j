#include "json_string.h"

//////////////////////////////////////////////////////////////////////////
const char * js_strpbrk( const char * _begin, const char * _end, const char * _str )
{
    const char * it = _begin;

    for( ; it != _end; ++it )
    {
        const char * it_str = _str;

        for( ; *it_str != '\0'; ++it_str )
        {
            if( *it == *it_str )
            {
                return it;
            }
        }
    }

    return JS_NULLPTR;
}
//////////////////////////////////////////////////////////////////////////
js_bool_t js_chrskip( char _ch, const char * _str )
{
    const char * it_str = _str;

    for( ; *it_str != '\0'; ++it_str )
    {
        if( _ch == *it_str )
        {
            return JS_TRUE;
        }
    }

    return JS_FALSE;
}
//////////////////////////////////////////////////////////////////////////
const char * js_strskip( const char * _begin, const char * _end, const char * _str )
{
    const char * it = _begin;

    for( ; it != _end; ++it )
    {
        if( js_chrskip( *it, _str ) == JS_FALSE )
        {
            return it;
        }
    }

    return JS_NULLPTR;
}
//////////////////////////////////////////////////////////////////////////
const char * js_strstr( const char * _begin, const char * _end, const char * _str )
{
    if( *_str == '\0' )
    {
        return _begin;
    }

    const char * b = _str;
    const char * it = _begin;

    for( ; it != _end; ++it )
    {
        if( *it != *b )
        {
            continue;
        }

        const char * a = it;

        for( ;; )
        {
            if( *b == 0 )
            {
                return it;
            }

            if( *a++ != *b++ )
            {
                break;
            }
        }

        b = _str;
    }

    return JS_NULLPTR;
}
//////////////////////////////////////////////////////////////////////////
const char * js_strchr( const char * _begin, const char * _end, char _ch )
{
    const char * it = _begin;

    for( ; it != _end; ++it )
    {
        if( *it == _ch )
        {
            return it;
        }
    }

    return JS_NULLPTR;
}
//////////////////////////////////////////////////////////////////////////
js_bool_t js_strncmp( const char * _s1, const char * _s2, js_size_t _n )
{
    if( _n == 0 )
    {
        return JS_TRUE;
    }

    do
    {
        if( *_s1 != *_s2++ )
        {
            return JS_FALSE;
        }

        if( *_s1++ == '\0' )
        {
            break;
        }
    } while( --_n );

    return JS_TRUE;
}
//////////////////////////////////////////////////////////////////////////
js_bool_t js_strzcmp( js_string_t _s1, js_string_t _s2 )
{
    if( _s1.size != _s2.size )
    {
        return JS_FALSE;
    }

    for( js_size_t index = 0; index != _s1.size; ++index )
    {
        if( _s1.value[index] != _s2.value[index] )
        {
            return JS_FALSE;
        }
    }

    return JS_TRUE;
}
//////////////////////////////////////////////////////////////////////////