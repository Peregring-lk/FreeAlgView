// simple_debugger.cpp

#include "simple_debugger.hpp"

#include <iostream>

namespace freealgview // SimpleDebugger
{
    bool SimpleDebugger::operator()()
    {
        auto l_owner = owner();

        if (!l_owner)
            return false;

        std::cout << std::endl
                  << "ID: " << l_owner.id() << ", "
                  << " Degree: " << l_owner.degree() << " "
                  << std::endl;

        return true;
    }

} // SimpleDebugger
