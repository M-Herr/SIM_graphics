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
   (ExecutiveInterface.o)
   )
@endverbatim
*****************************************/

#pragma once
#include "trick/Executive.hh"
#include <cstdint>


class ExecutiveInterface
{
    public:
    
    ExecutiveInterface() = default;
    ~ExecutiveInterface() = default;

    void SetInterfacePointer(Trick::Executive* executive);

    inline const uint64_t GetSoftwareFrameTics() const;
    inline const uint64_t GetFrameCount() const; 
    inline const SIM_MODE GetSimMode() const; 
    inline const uint64_t GetTimeTicValue() const; 
    inline const uint64_t GetCurrentTimeTics() const;

    private:
    Trick::Executive* executivePtr{nullptr};

};