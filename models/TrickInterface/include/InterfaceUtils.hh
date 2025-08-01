/**
@file

@verbatim
PURPOSE:
    (Test)
LANGUAGE:
    (C++)
ICG: (No) 
LIBRARY DEPENDENCY:
    ((Variable.o)
   (InterfaceUtils.o)
   )
@endverbatim
*****************************************/

#pragma once
#include "trick/attributes.h"
#include "trick/MemoryManager.hh"
#include "TrickInterface/include/TrickTypeConversion.hh"

#include <vector>

struct VariableEntry
{
    std::string fullName;
    std::string typeName;
    const ATTRIBUTES* attributes{nullptr};
    void* address{nullptr};
    size_t bytes{0};
};

// Given a *type* descriptor, determine if it is a struct.
static inline bool is_struct_type(const ATTRIBUTES* type_attr) {
    return type_attr && type_attr->type == TRICK_STRUCTURED;
}

// Given a *type* descriptor for a struct, get its member list (sentinel-terminated).
static inline const ATTRIBUTES* struct_member_list(const ATTRIBUTES* type_attr) {
    return is_struct_type(type_attr)
         ? static_cast<const ATTRIBUTES*>(type_attr->attr)
         : nullptr;
}

// Count members (sentinel-terminated by name == nullptr)
static inline size_t member_count(const ATTRIBUTES* members) {
    size_t n = 0;
    if (!members) return 0;
    while (members[n].name != nullptr) ++n;
    return n;
}
