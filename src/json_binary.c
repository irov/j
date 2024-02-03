#include "json_binary.h"

//////////////////////////////////////////////////////////////////////////
static void __binary_write_element( js_binary_ctx_t * _ctx, const js_element_t * _element );
//////////////////////////////////////////////////////////////////////////
typedef enum js_binary_type_e
{
    js_binary_type_null,
    js_binary_type_false,
    js_binary_type_true,
    js_binary_type_integer_z,
    js_binary_type_integer_1,
    js_binary_type_integer,
    js_binary_type_real_z,
    js_binary_type_real_1,
    js_binary_type_real,
    js_binary_type_string_z,
    js_binary_type_string,
    js_binary_type_array_z,
    js_binary_type_array,
    js_binary_type_dict_z,
    js_binary_type_dict,
} js_binary_type_e;
//////////////////////////////////////////////////////////////////////////
static void __binary_write_type( js_binary_ctx_t * _ctx, js_binary_type_e _type )
{
    uint8_t type8 = (uint8_t)_type;
    _ctx->write( &type8, sizeof( uint8_t ), _ctx->ud );
}
//////////////////////////////////////////////////////////////////////////
static void __binary_write_size( js_binary_ctx_t * _ctx, js_size_t _size )
{
    if( _size < 255 )
    {
        uint8_t size8 = (uint8_t)_size;
        _ctx->write( &size8, sizeof( uint8_t ), _ctx->ud );
    }
    else
    {
        const uint8_t size255 = 255;
        _ctx->write( &size255, sizeof( uint8_t ), _ctx->ud );

        uint32_t size32 = (uint32_t)_size;
        _ctx->write( &size32, sizeof( uint32_t ), _ctx->ud );
    }
}
//////////////////////////////////////////////////////////////////////////
static void __binary_write_integer( js_binary_ctx_t * _ctx, const js_element_t * _element )
{
    js_integer_t value = js_get_integer( _element );

    if( value == 0 )
    {
        __binary_write_type( _ctx, js_binary_type_integer_z );
    }
    else if( value == 1 )
    {
        __binary_write_type( _ctx, js_binary_type_integer_1 );
    }
    else
    {
        __binary_write_type( _ctx, js_binary_type_integer );
        _ctx->write( &value, sizeof( js_integer_t ), _ctx->ud );
    }
}
//////////////////////////////////////////////////////////////////////////
static void __binary_write_real( js_binary_ctx_t * _ctx, const js_element_t * _element )
{
    js_real_t value = js_get_real( _element );

    if( value == (js_real_t)0.0 )
    {
        __binary_write_type( _ctx, js_binary_type_real_z );
    }
    else if( value == (js_real_t)1.0 )
    {
        __binary_write_type( _ctx, js_binary_type_real_1 );
    }
    else
    {
        __binary_write_type( _ctx, js_binary_type_real );
        _ctx->write( &value, sizeof( js_real_t ), _ctx->ud );
    }
}
//////////////////////////////////////////////////////////////////////////
static void __binary_write_string( js_binary_ctx_t * _ctx, const js_element_t * _element )
{
    js_string_t str;
    js_get_string( _element, &str );

    if( str.size == 0 )
    {
        __binary_write_type( _ctx, js_binary_type_string_z );

        return;
    }

    __binary_write_type( _ctx, js_binary_type_string );

    __binary_write_size( _ctx, str.size );
    _ctx->write( str.value, str.size, _ctx->ud );
}
//////////////////////////////////////////////////////////////////////////
static void __binary_write_array_foreach( js_size_t _index, const js_element_t * _value, void * _ud )
{
    JS_UNUSED( _index );

    js_binary_ctx_t * ctx = (js_binary_ctx_t *)_ud;

    __binary_write_element( ctx, _value );
}
//////////////////////////////////////////////////////////////////////////
static void __binary_write_array( js_binary_ctx_t * _ctx, const js_element_t * _element )
{
    js_size_t size = js_array_size( _element );

    if( size == 0 )
    {
        __binary_write_type( _ctx, js_binary_type_array_z );

        return;
    }

    __binary_write_type( _ctx, js_binary_type_array );

    __binary_write_size( _ctx, size );

    js_array_foreach( _element, &__binary_write_array_foreach, _ctx );
}
//////////////////////////////////////////////////////////////////////////
static void __binary_write_object_foreach( js_size_t _index, const js_element_t * _key, const js_element_t * _value, void * _ud )
{
    JS_UNUSED( _index );

    js_binary_ctx_t * ctx = (js_binary_ctx_t *)_ud;

    __binary_write_string( ctx, _key );

    __binary_write_element( ctx, _value );
}
//////////////////////////////////////////////////////////////////////////
static void __binary_write_object( js_binary_ctx_t * _ctx, const js_element_t * _element )
{
    js_size_t root_size = js_object_size( _element );

    if( root_size == 0 )
    {
        __binary_write_type( _ctx, js_binary_type_dict_z );

        return;
    }

    __binary_write_type( _ctx, js_binary_type_dict );

    __binary_write_size( _ctx, root_size );

    js_object_foreach( _element, &__binary_write_object_foreach, _ctx );
}
//////////////////////////////////////////////////////////////////////////
static void __binary_write_element( js_binary_ctx_t * _ctx, const js_element_t * _element )
{
    js_type_t type = js_type( _element );

    switch( type )
    {
    case js_type_null:
        {
            __binary_write_type( _ctx, js_binary_type_null );
        }break;
    case js_type_false:
        {
            __binary_write_type( _ctx, js_binary_type_false );
        }break;
    case js_type_true:
        {
            __binary_write_type( _ctx, js_binary_type_true );
        }break;
    case js_type_integer:
        {
            __binary_write_integer( _ctx, _element );            
        }break;
    case js_type_real:
        {
            __binary_write_real( _ctx, _element );
        }break;
    case js_type_string:
        {
            __binary_write_string( _ctx, _element );
        }break;
    case js_type_array:
        {
            __binary_write_array( _ctx, _element );
        }break;
    case js_type_object:
        {
            __binary_write_object( _ctx, _element );
        }break;
    }
}
//////////////////////////////////////////////////////////////////////////
void js_binary( const js_element_t * _element, js_binary_ctx_t * _ctx )
{
    js_size_t root_size = js_object_size( _element );

    if( root_size == 0 )
    {
        return;
    }

    __binary_write_type( _ctx, js_binary_type_dict );

    __binary_write_size( _ctx, root_size );

    js_object_foreach( _element, &__binary_write_object_foreach, _ctx );
}
//////////////////////////////////////////////////////////////////////////