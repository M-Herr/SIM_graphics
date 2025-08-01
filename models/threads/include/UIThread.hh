/**
@file

@verbatim
PURPOSE:
    (Test)
LANGUAGE:
    (C++)
ICG: (No) 
LIBRARY DEPENDENCY:
    ((UIThread.o)
     (UI/src/MainWindow.o)
     (UI/src/Window.o)
     (UI/src/Button.o)
     (UI/src/SimInfoUI.o)
     (TrickInterface/src/Variable.o)
    )
@endverbatim
*****************************************/
#pragma once
#ifndef SWIG

#include <cstdio>
#include <atomic>
#include <thread>
#include <mutex>


#include "UI/include/MainWindow.hh"
#include "UI/include/UICore.hh"
#include "UI/include/SimInfoUI.hh"

#include "TrickInterface/include/Variable.hh"

#include "trick/MemoryManager.hh"
#include "trick/Executive.hh"

#include "TrickInterface/include/ExecutiveInterface.hh"


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

class UIThread
{
    public:
    
    UIThread() = default;
    ~UIThread();

    void Initialize();

    void Update();

    void Run(const UI::WindowProperties& window_properties);
    void Shutdown();
    
    void UpdateSimTime(double sim_time);
    double ReadSimTime();

    MemoryManagerInterface* GetMMInterface() { return &trickMMInterface; }
    ExecutiveInterface* GetExecInterface() { return &executiveInterface; }    

    private:
    std::mutex simUpdateMutex;
    UI::MainWindow* mainWindow{nullptr};
    std::atomic<bool> runUI{true};
    bool initialized{false};

    UI::Node rootNode;
    UI::Node varListNode;

    UI::SimInfoUI simInfoUI;

    double simTime{0.0};
    UI::WindowProperties windowProperties;

    MemoryManagerInterface trickMMInterface;
    ExecutiveInterface executiveInterface;
};

#endif
