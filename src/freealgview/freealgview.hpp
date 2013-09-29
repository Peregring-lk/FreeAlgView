#ifndef FREEALGVIEW_HPP_INCLUDED
#define FREEALGVIEW_HPP_INCLUDED

#include "faudata.hpp"

namespace freealgview // FreeAlgView
{
    class FreeAlgView : public fauFunctor<FreeAlgView>
    {
    public: //!! requests
        bool operator()() override;
    };

} // FreeAlgView

#endif // FREEALGVIEW_HPP_INCLUDED
