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
     (UI/include/MainWindow.o)
     (threads/src/glad.o)
     (imgui.o)
     (imgui_stdlib.o)
     (imgui_demo.o)
     (imgui_tables.o)
     (imgui_widgets.o)
     (imgui_impl_opengl3.o)
     (imgui_impl_glfw.o)
     (imgui_draw.o)
    )
@endverbatim
**/
#pragma once

#include "UI/include/UIComponent.hh"

namespace UI 
{

class Window : public UIComponent
{
    public:
    Window(std::string& name);

    virtual void Begin() override;
    virtual void End() override;

    std::string name;
};
 
    

}


