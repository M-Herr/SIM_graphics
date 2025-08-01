#include "UI/include/SimInfoUI.hh"

namespace UI
{
    void SimInfoUI::Initialize(MemoryManagerInterface* mm_if) 
    {
        memoryManagerIf = mm_if;

        vars = memoryManagerIf->FindAll("trick_sys");

        auto it = std::find_if(vars.begin(), vars.end(), [](const VariableEntry& var){
            if(var.fullName.find("frame_count") != std::string::npos)
            {
                return true;
            }
            return false;
        });

        simInfo.frameCount = static_cast<uint64_t*>(it->get().address);

        it = std::find_if(vars.begin(), vars.end(), [](const VariableEntry& var){
            if(var.fullName.find("time_tics")  != std::string::npos)
            {
                return true;
            }
            return false;
        });
        
        simInfo.simTics = static_cast<uint64_t*>(it->get().address);
    }

    Child SimInfoUI::JobQueue()
    { 
        return Child 
        {
        };
    }

    Child SimInfoUI::Update()
    { 
        return Child 
        {
            .id = "tree_view",
            .border = true,
            .children {
                TreeNode
                {
                    .id{"simulation info"},
                    .label{Text("Simulation Info")},
                    .children{
                        TreeNode {
                        .id{"frame_count"},
                        .label{Text("Frame Count: ")}, 
                        .value{Text(simInfo.frameCount)}
                        },
                        TreeNode {
                            .id{"sim_tics"},
                            .label{Text("Sim Tics")}, 
                            .value{Text(simInfo.simTics)}
                        }
                    }
                }
            }
        };
    }
}