#include "TrickInterface/include/ExecutiveInterface.hh"


void ExecutiveInterface::SetInterfacePointer(Trick::Executive* executive)
{
    executivePtr = executive;
}

const uint64_t ExecutiveInterface::GetSoftwareFrameTics() const 
{ 
    return executivePtr->get_software_frame_tics(); 
}

const uint64_t ExecutiveInterface::GetFrameCount() const 
{ 
    return executivePtr->get_frame_count();
}

const SIM_MODE ExecutiveInterface::GetSimMode() const 
{ 
    return executivePtr->get_mode(); 
}

const uint64_t  ExecutiveInterface::GetTimeTicValue() const 
{
    return executivePtr->get_time_tic_value(); 
}

const uint64_t ExecutiveInterface::GetCurrentTimeTics() const 
{
    return executivePtr->get_time_tics();
}