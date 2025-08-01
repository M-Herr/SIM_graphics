#include "UI/include/Window.hh"

namespace UI
{      

    Window::Window(std::string& name)
    : name(name)
    {

    }

    void Window::Begin() 
    {
        ImGui::Begin(name.c_str());
    }
    void Window::End()
    {
        ImGui::End();   
    }

}
