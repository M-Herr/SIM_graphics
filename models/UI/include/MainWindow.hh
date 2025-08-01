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
*****************************************/
#pragma once

#include "threads/include/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"

namespace UI {

    typedef struct
    {
        size_t width{1920};
        size_t height{1080};
        ImVec4 clear_color{0.f, 1.f, 0.f, 1.f};
        std::string name{"DefaultName"};
    } WindowProperties;

    class MainWindow
    {
        public:
        MainWindow(const WindowProperties& in_properties);
        ~MainWindow();

        bool InitializeGlad();

        bool InitializeGLFW();

        bool SetupImgui();

        void BeginFrame();

        void EndFrame();

        void CleanUp();
        
        bool running{true};
        private:

        GLFWwindow* window;
        WindowProperties properties;
        std::string glslVersion;
        
    };

}
