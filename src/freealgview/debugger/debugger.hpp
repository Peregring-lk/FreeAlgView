// debugger.hpp

#ifndef FREEALGVIEW_DEBUGGER_HPP_INCLUDED
#define FREEALGVIEW_DEBUGGER_HPP_INCLUDED

#include "../../fcloud/fspace.hpp"

namespace freealgview // Debugger
{
    class Debugger : public fspace::fowner
    {
    private:
        static unsigned s_counter;

    public: // requests
        bool operator()();

        static std::string next_filename();
        static std::string last_filename();

        static void show(bool pause = true);

    private: // methods
        void mf_generateImage(char const* mem) const;
        std::string mf_filter_quote(std::string const& str) const;
    };

} // Debugger

#endif // DEBUGGER_HPP_INCLUDED
