// simple_debugger.hpp

#ifndef FREEALGVIEW_SIMPLE_DEBUGGER_HPP_INCLUDED
#define FREEALGVIEW_SIMPLE_DEBUGGER_HPP_INCLUDED

#include "../../fcloud/fspace.hpp"

namespace freealgview // SimpleDebugger
{
    class SimpleDebugger : public fspace::fowner
    {
    public: // requests
        bool operator()();
    };

} // SimpleDebugger

#endif // SIMPLE_DEBUGGER_HPP_INCLUDED
