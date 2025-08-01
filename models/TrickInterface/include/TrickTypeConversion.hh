/**
@file

@verbatim
PURPOSE:
    (Test)
LANGUAGE:
    (C++)
ICG: (No) 
LIBRARY DEPENDENCY:
    ()
@endverbatim
*****************************************/
#pragma once

#include "trick/parameter_types.h"
#include <cstdint>
#include <exception>
#include <string>


template<TRICK_TYPE type>
struct trick_to_builtin {};


template<>
struct trick_to_builtin<TRICK_VOID>
{
    using type = void;
};

template<>
struct trick_to_builtin<TRICK_CHARACTER>
{
    using type = char;
};

template<>
struct trick_to_builtin<TRICK_UNSIGNED_CHARACTER>
{
    using type = unsigned char;
};

template<>
struct trick_to_builtin<TRICK_STRING>
{
    using type = char*;
};

template<>
struct trick_to_builtin<TRICK_SHORT>
{
    using type = short;
};

template<>
struct trick_to_builtin<TRICK_UNSIGNED_SHORT>
{
    using type = unsigned short;
};

template<>
struct trick_to_builtin<TRICK_INTEGER>
{
    using type = int;
};

template<>
struct trick_to_builtin<TRICK_UNSIGNED_INTEGER>
{
    using type = unsigned int;
};

template<>
struct trick_to_builtin<TRICK_LONG>
{
    using type = long;
};

template<>
struct trick_to_builtin<TRICK_UNSIGNED_LONG>
{
    using type = unsigned long;
};

template<>
struct trick_to_builtin<TRICK_FLOAT>
{
    using type = float;
};

template<>
struct trick_to_builtin<TRICK_DOUBLE>
{
    using type = double;
};

template<>
struct trick_to_builtin<TRICK_LONG_LONG>
{
    using type = long long;
};

template<>
struct trick_to_builtin<TRICK_UNSIGNED_LONG_LONG>
{
    using type = unsigned long long;
};

template<>
struct trick_to_builtin<TRICK_BOOLEAN>
{
    using type = bool;
};

template<>
struct trick_to_builtin<TRICK_VOID_PTR>
{
     using type = void*;
};

template<TRICK_TYPE type>
using trick_type_t = typename trick_to_builtin<type>::type;


template<TRICK_TYPE type>
struct trick_type_to_string {};


template<>
struct trick_type_to_string<TRICK_VOID>
{
    const std::string name{"TRICK_VOID"};
};

template<>
struct trick_type_to_string<TRICK_CHARACTER>
{
    const std::string name{"TRICK_CHARACTER"};
};

template<>
struct trick_type_to_string<TRICK_UNSIGNED_CHARACTER>
{
    const std::string name{"TRICK_UNSIGNED_CHARACTER"};
};

template<>
struct trick_type_to_string<TRICK_STRING>
{
    const std::string name{"TRICK_STRING"};
};

template<>
struct trick_type_to_string<TRICK_SHORT>
{
    const std::string name{"TRICK_SHORT"};
};

template<>
struct trick_type_to_string<TRICK_UNSIGNED_SHORT>
{
    const std::string name{"TRICK_UNSIGNED_SHORT"};
};

template<>
struct trick_type_to_string<TRICK_INTEGER>
{
    const std::string name{"TRICK_INTEGER"};
};

template<>
struct trick_type_to_string<TRICK_UNSIGNED_INTEGER>
{
    const std::string name{"TRICK_UNSIGNED_INTEGER"};
};

template<>
struct trick_type_to_string<TRICK_LONG>
{
    const std::string name{"TRICK_LONG"};
};

template<>
struct trick_type_to_string<TRICK_UNSIGNED_LONG>
{
    const std::string name{"TRICK_UNSIGNED_LONG"};
};

template<>
struct trick_type_to_string<TRICK_FLOAT>
{
    const std::string name{"TRICK_FLOAT"};
};

template<>
struct trick_type_to_string<TRICK_DOUBLE>
{
    const std::string name{"TRICK_DOUBLE"};
};

template<>
struct trick_type_to_string<TRICK_LONG_LONG>
{
    const std::string name{"TRICK_LONG_LONG"};
};

template<>
struct trick_type_to_string<TRICK_UNSIGNED_LONG_LONG>
{
    const std::string name{"TRICK_UNSIGNED_LONG_LONG"};
};

template<>
struct trick_type_to_string<TRICK_BOOLEAN>
{
    const std::string name{"TRICK_BOOLEAN"};
};


template<>
struct trick_type_to_string<TRICK_VOID_PTR>
{
    const std::string name{"TRICK_VOID_PTR"};
};

template<TRICK_TYPE type>
using trick_type_name_v = typename trick_type_to_string<type>::name;