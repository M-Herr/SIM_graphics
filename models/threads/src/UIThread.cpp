#include "threads/include/UIThread.hh"
#include "UI/include/UICore.hh"

#include <stdio.h>
#include "trick/exec_proto.h"
#include "UI/include/SimInfoUI.hh"
#include "TrickInterface/include/TrickTypeConversion.hh"

UIThread::~UIThread()
{
   delete mainWindow;
}

void UIThread::Initialize()
{
    if(!mainWindow)
    {
        mainWindow = new UI::MainWindow(windowProperties);
    }

    if(!mainWindow->InitializeGLFW())
    {
        exec_terminate(__FILE__, "Failed to initialize GLFW.");
    }

    if(!mainWindow->InitializeGlad())
    {
        exec_terminate(__FILE__, "Failed to initialize GLAD");
    }
    
   
    if(!mainWindow->SetupImgui())
    {
        exec_terminate(__FILE__, "Failed to initialize ImGui.");
    }



    initialized = true;
}


void UIThread::Update()
{   

    mainWindow->BeginFrame();

    draw(rootNode);
    draw(varListNode);


    mainWindow->EndFrame();

}

void UIThread::UpdateSimTime(double sim_time)
{
    std::lock_guard<std::mutex> guard(simUpdateMutex);
    simTime = sim_time;
}


void UIThread::Run(const UI::WindowProperties& window_properties)
{
    windowProperties = window_properties;
 
    using namespace UI;    
    trickMMInterface.GetAllVariablesFromTrick();
    
    simInfoUI.Initialize(&trickMMInterface);

    rootNode = Window 
    {
        "Main", true, 0,
        {
            LabelValue{"test", 
                Text{"Sim Time: "},
                Text{simTime}
            },
            simInfoUI.Update()
        }
    };

    
    auto build_var_window = [&]()-> std::vector<Node> {
                
                std::vector<Node> out;
                out.reserve(trickMMInterface.GetCatalog().size());
                size_t counter = 0;
                
                for(const auto& v: trickMMInterface.GetCatalog())
                {

                    TreeNode tree_node_parent;
                    tree_node_parent.id = "var_" + std::to_string(counter);
                    tree_node_parent.default_open = false;
                    tree_node_parent.label = Text(v.fullName);

                    TreeNode node_value;
                    node_value.id = "var_" + std::to_string(counter++);
                    node_value.default_open = false;
                    node_value.label = Text("Value");
                    node_value.value = Text(make_text_from_trick_ptr(v.address, v.attributes->type));
                    

                    TreeNode node_trick_type;
                    node_trick_type.id = "var_" + std::to_string(counter++);
                    node_trick_type.default_open = false;
                    node_trick_type.label = Text("Type");
                    node_trick_type.value = Text(v.typeName);
                    
                    tree_node_parent.children.emplace_back(std::move(node_value));
                    tree_node_parent.children.emplace_back(std::move(node_trick_type));

                    out.emplace_back(tree_node_parent);
                    counter++;
                }
                return out;
            };

    varListNode = Window {
        "Variable List", true, 0,
         build_var_window()
        

    };
    

    while(runUI.load(std::memory_order_relaxed))
    {
        if(!initialized)
        {
            Initialize();
        }

        Update();
    }

    mainWindow->CleanUp();
}

void UIThread::Shutdown()
{
    runUI.store(false);
    
}