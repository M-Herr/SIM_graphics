
#include "UI/include/Button.hh"
#include "imgui_internal.h"

namespace UI
{
    Button::Button(std::string& name)
    : clicked(false), button_text(name)
    {

    }

    void Button::Begin()
    {   
        ImGui::Button(button_text.c_str());
    }

    void Button::End()
    {
        ImGui::End();
    }
}
