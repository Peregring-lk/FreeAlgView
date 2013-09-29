// debugger.cpp

#include "debugger.hpp"

#include <iostream>
#include <sstream>
#include <graphviz/gvc.h>
#include <cstdio>
#include <cstring>

namespace freealgview // Debugger
{
    unsigned Debugger::s_counter = 0;

    bool Debugger::operator()()
    {
        auto l_owner = owner();

        if (!l_owner)
            return false;

        std::ostringstream os;

        os << "digraph" << futil::s << l_owner.id() << std::endl
           << "{" << std::endl;

        for (auto i : l_owner) {
            os << futil::t << i.nid() << futil::s
               << "[label=\"";

            if (!i.anonymous())
                os << mf_filter_quote(i.id());

            // << futil::s << "[" << mf_filter_quote(i.type()) << "]"

            os << "\"";

            if (i.leaf())
                os << " color=blue";

            os << "];" << std::endl;

            if (i.father())
                os << futil::t << i.father().nid() << " -> " << i.nid()
                   << ";" << std::endl;

            auto ref = *i;

            if (ref)
                os << futil::t << i.nid() << " -> " << ref.nid()
                   << " [color=firebrick];"
                   << std::endl;
        }

        os << "}" << std::endl;

        mf_generateImage(os.str().c_str());

        return true;
    }

    std::string Debugger::last_filename()
    {
        std::string filename("Debugger-out-");

        filename += std::to_string(s_counter) + ".png";

        return filename;
    }

    std::string Debugger::next_filename()
    {
        ++s_counter;

        return last_filename();
    }

    void Debugger::show(bool pause)
    {
        static bool first = true;

        if (first) {
            std::string cmd("gpicview " + last_filename() + " &");
            std::system(cmd.c_str());

            first = false;
        }

        if (pause) {
            char c;
            std::cin >> c;

            if (c == 'q')
                exit(0);
        }
    }

    void Debugger::mf_generateImage(char const* mem) const
    {
        // HACK: Grrrr!! Ugly Ugly!
        FILE* tmpf = std::tmpfile();
        std::fputs(mem, tmpf);
        std::rewind(tmpf);

        GVC_t* gvc = gvContext();
        Agraph_t* G = agread(tmpf);

        std::fclose(tmpf);

        gvLayout(gvc, G, "dot");
        gvRenderFilename(gvc, G, "png", next_filename().c_str());

        gvFreeLayout(gvc, G);
        agclose(G);
        gvFreeContext(gvc);
    }

    std::string Debugger::mf_filter_quote(std::string const& str) const
    {
        std::string out;

        for (auto& i : str)
            if (i == '"')
                out += "\\\"";
            else
                out += i;

        return out;
    }

} // Debugger
