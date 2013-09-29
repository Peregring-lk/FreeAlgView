// simple_debugger.hpp

#ifndef FREEALGVIEW_SIMPLE_DEBUGGER_HPP_INCLUDED
#define FREEALGVIEW_SIMPLE_DEBUGGER_HPP_INCLUDED

#include "../faudata.hpp"

namespace freealgview // SimpleDebugger
{
    class SimpleDebugger : public fauFunctor<SimpleDebugger>
    {
    public: // requests
        bool operator()() override;
    };

} // SimpleDebugger

#endif // SIMPLE_DEBUGGER_HPP_INCLUDED
