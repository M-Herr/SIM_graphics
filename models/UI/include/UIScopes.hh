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
#include "imgui.h"

struct WindowScope
{
    bool open{false};
    WindowScope(const char* title, bool* p_open = nullptr, ImGuiWindowFlags flags = 0)
    {
        open = ImGui::Begin(title, p_open, flags);
    }

    ~WindowScope() {ImGui::End();}

    explicit operator bool() const noexcept { return open; }
};

struct ChildScope 
{
    ChildScope(const char* str_id, ImVec2 size = ImVec2(0,0), bool border = false, ImGuiWindowFlags flags =0)
    {
        ImGui::BeginChild(str_id, size, border, flags);   
    }

    ~ChildScope() { ImGui::EndChild(); }

};

struct TableScope 
{
    bool open{false};
    TableScope(const char* id, int columns, ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchProp)
    {
        open = ImGui::BeginTable(id, columns, flags);
    }
    ~TableScope() { ImGui::EndTable(); }
    
    explicit operator bool() const noexcept { return open; }
};





