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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_stdlib.h"


namespace UI 
{

    class UIComponent 
    {
        public:

        virtual void Begin() = 0;
        
        virtual void End() = 0;

    };

}


