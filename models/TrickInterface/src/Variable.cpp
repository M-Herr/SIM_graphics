#include "TrickInterface/include/Variable.hh"
#include "Utils/include/Utils.hh"

#include <cstddef>
#include <execution>
#include <algorithm>

MemoryManagerInterface::MemoryManagerInterface(Trick::MemoryManager* memory_manager)
: memoryManagerPtr(memory_manager)
{

}

MemoryManagerInterface::MemoryManagerInterface()
{

}

std::vector<VariableEntry> MemoryManagerInterface::ParseAttribute(ATTRIBUTES* attribute_ptr, void* start_address, const std::string& prefix)
{
    std::vector<VariableEntry> vars;

    size_t index = 0;
    //ATTRIBUTES* attribute = &attribute_ptr[index];

    for(ATTRIBUTES* attribute = attribute_ptr; attribute && attribute->name[0] != '\0'; ++attribute)
    {
        auto member = static_cast<char*>(start_address) + static_cast<std::ptrdiff_t>(attribute->offset);

        VariableEntry entry;
        entry.address = static_cast<void*>(member);
        entry.attributes = attribute;
        entry.fullName = prefix + attribute->name;
        entry.typeName = attribute->type_name;

        vars.emplace_back(entry);
        
        
        if(attribute->type == TRICK_STRUCTURED)
        {
            if(auto* sub_attr = static_cast<ATTRIBUTES*>(attribute->attr))
            {
                auto sub_attr_vec = ParseAttribute(sub_attr, entry.address, entry.fullName + ".");
                vars.insert(vars.end(), sub_attr_vec.begin(), sub_attr_vec.end());
            }
        }
      
    }

    return vars;
}

std::vector<VariableEntry> MemoryManagerInterface::ParseAttributes(ALLOC_INFO* alloc_info, const std::string& prefix)
{
    
    std::vector<VariableEntry> var_entries;

    ATTRIBUTES* attr = alloc_info->attr;
    
    size_t index = 0;
    ATTRIBUTES* attribute = &attr[index];

    auto attribute_vec = ParseAttribute(attribute, alloc_info->start, prefix);
    var_entries.insert(var_entries.end(), attribute_vec.begin(), attribute_vec.end());

    return var_entries;
} 

void MemoryManagerInterface::GetAllVariablesFromTrick()
{
    std::vector<VariableEntry> vec;
    for(auto it = memoryManagerPtr->variable_map_begin(); it != memoryManagerPtr->variable_map_end(); ++it)
    {
       
        auto new_vec = ParseAttributes(it->second, it->first + std::string("."));
        vec.insert(vec.end(), new_vec.begin(), new_vec.end());
        
       
    }

    varCatalog = vec;
}

void MemoryManagerInterface::SetInterfacePointer(Trick::MemoryManager* memory_manager)
{
    std::lock_guard<std::mutex> guard(memoryManagerMutex);

    memoryManagerPtr = memory_manager;
}
