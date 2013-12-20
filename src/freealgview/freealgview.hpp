#ifndef FREEALGVIEW_HPP_INCLUDED
#define FREEALGVIEW_HPP_INCLUDED

#include "../fcloud/fspace.hpp"

namespace freealgview // FreeAlgView
{
    class FreeAlgView : public fspace::fowner
    {
    public: //!! requests
        bool operator()();
    };

} // FreeAlgView

#endif // FREEALGVIEW_HPP_INCLUDED
