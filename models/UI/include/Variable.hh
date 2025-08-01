/**
@file

@verbatim
PURPOSE:
    (Test)
LANGUAGE:
    (C++)
ICG: (No) 
LIBRARY DEPENDENCY:
    ((UI/src/Variable.o)
   (TrickInterface/src/InterfaceUtils.o)
   )
@endverbatim
*****************************************/

#pragma once
#include "TrickInterface/include/InterfaceUtils.hh"


class VariableUI
{
    public:
    VariableUI() = default;
    VariableUI(VariableEntry& entry);

    ~VariableUI();

    void DrawUI();

    private:
    VariableEntry* variableEntry;

};