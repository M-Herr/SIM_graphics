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
    )
@endverbatim
**/

#pragma once

#include <string>

#include "UI/include/UIComponent.hh"

namespace UI 
{
    class Button : public UIComponent
    {
        public:
        Button(std::string& name);
        ~Button();

        virtual void Begin() override;
        virtual void End() override;

        bool clicked;
        std::string button_text;
    };

}


