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
#include <mutex>
#include <string_view>
#include <optional>
#include <functional>
#include <execution>

#include "trick/MemoryManager.hh"
#include "TrickInterface/include/InterfaceUtils.hh"
#include "TrickInterface/include/TrickTypeConversion.hh"


enum class NameSearchOptions
{
    FindNameExactReturnFirst,   //Will match a variable name.  Returns first found
    FindNameExactReturnAll,   //Will match a variable name.  Returns fall matches
    FindNameContains
};

class MemoryManagerInterface
{

    public:
    MemoryManagerInterface(Trick::MemoryManager* memory_manager);
    MemoryManagerInterface();
    ~MemoryManagerInterface() = default;

    void SetInterfacePointer(Trick::MemoryManager* memory_manager);
    void GetAllVariablesFromTrick();
  
    //std::vector<VariableEntry> FindVariable

    template<typename T>
    std::vector<std::reference_wrapper<VariableEntry>> FindAll(T name);

    template<typename T>
    std::optional<std::reference_wrapper<VariableEntry>> FindFirst(T address);


    const std::vector<VariableEntry>&  GetCatalog() const { return varCatalog; }

    private:
    std::mutex memoryManagerMutex;
    Trick::MemoryManager* memoryManagerPtr{nullptr};
    std::vector<VariableEntry> varCatalog;

    private:
    std::vector<VariableEntry> ParseAttributes(ALLOC_INFO* alloc_info, const std::string& prefix);
    std::vector<VariableEntry> ParseAttribute(ATTRIBUTES* attribute_ptr,  void* start_address, const std::string& prefix);
};

template<typename T>
inline std::optional<std::reference_wrapper<VariableEntry>> MemoryManagerInterface::FindFirst(T find_by_val)
{
    return std::nullopt;
}

//Compiler will probably undo this inline, but it stops annoying warnings so...
template<>
inline std::optional<std::reference_wrapper<VariableEntry>> MemoryManagerInterface::FindFirst<std::string_view>(std::string_view find_by_val)
{
    
    auto search_predicate = [find_by_val](const VariableEntry& var)
    {
        if(var.fullName.find(find_by_val))
        {
            return true;
        }
        return false;
    };
    
    auto res = std::find_if(std::execution::par, varCatalog.begin(), varCatalog.end(), search_predicate);

    if( res != varCatalog.end())
    {
        return std::ref(*res);
    }

    return std::nullopt;
}


template<>
inline std::optional<std::reference_wrapper<VariableEntry>> MemoryManagerInterface::FindFirst<void*>(void* find_by_val)
{
    
    auto search_predicate = [find_by_val](const VariableEntry& var)
    {
        if(var.address == find_by_val)
        {
            return true;
        }
        return false;
    };
    
    auto res = std::find_if(std::execution::par, varCatalog.begin(), varCatalog.end(), search_predicate);

    if( res != varCatalog.end())
    {
        return std::ref(*res);
    }

    return std::nullopt;
}


template<typename T>
std::vector<std::reference_wrapper<VariableEntry>> MemoryManagerInterface::FindAll(T key)
{
    std::vector<std::reference_wrapper<VariableEntry>> res;

    res.reserve(varCatalog.size());

    if constexpr(std::is_convertible_v<T, std::string_view>)
    {
        std::string_view sv = key;
        for(auto& v : varCatalog)
        {
            if (v.fullName.find(sv) != std::string::npos)
            {
                res.emplace_back(v);
            }
        }
    } 
    else if constexpr (std::is_same_v<std::decay_t<T>, void*>)
    {
        for(auto& v : varCatalog)
        {
            if(v.address == key)
            {
                res.emplace_back(v);
            }
        }
    }
    else 
    {
        static_assert(!sizeof(T), "Unsupported FindAll key type");
    }

    return res;
}