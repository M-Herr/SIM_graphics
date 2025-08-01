/**
@file

@verbatim
PURPOSE:
    (Test)
LANGUAGE:
    (C++)
ICG: (No) 
LIBRARY DEPENDENCY:
    (
    (TrickInterface/src/Variable.o)
    (SimInfoUI.o)
    )
@endverbatim
**/

#pragma once
#include "TrickInterface/include/Variable.hh"
#include "UI/include/UIScopes.hh"
#include "UI/include/UICore.hh"

namespace UI
{

    enum class SimMode
    {
        Run,
        Pause
    };

    struct SimInfo
    {
        uint64_t* frameCount{nullptr};
        uint64_t* simTics{nullptr};

    };

    class SimInfoUI
    {  
        public:
        SimInfoUI() {}

        void Initialize(MemoryManagerInterface* mm_if);

        Child JobQueue();
        Child Update();

        SimInfo simInfo;
        private:
        MemoryManagerInterface* memoryManagerIf;
        std::vector<std::reference_wrapper<VariableEntry>> vars;
        
    };

}