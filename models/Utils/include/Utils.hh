/**
@file

@verbatim
PURPOSE:
    (Test)
LANGUAGE:
    (C++)
ICG: (No) 
LIBRARY DEPENDENCY:
    ()
@endverbatim
*****************************************/

#pragma once
#include <vector>
#include <execution>

namespace Utils
{

    template<class It, class Pred>
    std::vector<It> find_all(std::execution::parallel_policy exec_policy, It first, It last, Pred pred)
    {
        std::vector<It> result;
        while(first != last)
        {
            first = std::find_if(exec_policy, first, last, pred);
            if(first == last) break;
            result.push_back(first);
            ++first;
        }
        return result;
    }





}