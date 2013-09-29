// faudata.hpp

#ifndef FREEALGVIEW_FAUDATA_HPP_INCLUDED
#define FREEALGVIEW_FAUDATA_HPP_INCLUDED

#include "../fcloud/fspace.hpp"

namespace freealgview // fauData, fauFunctor, fauString
{
    template<class Derived>
    class fauData
    {
    protected:
        fspace::fnode m_owner;

    private:
        static fspace::fidentifier s_class_id()
        {
            static auto l_id = fspace::fidentifier(futil::stypeid<Derived>());

            return l_id;
        }

    public:
        static fspace::fnode_id class_id()
        {
            return static_cast<fspace::fnode_id>(s_class_id());
        }

        static fspace::henfo_id class_hid()
        {
            return static_cast<fspace::henfo_id>(s_class_id());
        }

    public:
        fspace::fnode owner() const
        {
            return m_owner;
        }

        fspace::henfo_id hid() const
        {
            return class_hid();
        }

        void owner(fspace::fnode n)
        {
            m_owner = n;
        }

        fauData& operator>>(fspace::fnode n)
        {
            n << fauData(*this);

            return *this;
        }
    };

    template<class Derived>
    class fauFunctor : public fauData<Derived>
    {
    public:
        //!! Virtual
        virtual bool operator()() = 0;
    };

    class fauString : public fspace::fidentifier, public fauData<fauString>
    {
    private:
        using base_tp = fspace::fidentifier;

    public:
        template<typename... T, class = futil::ctor_purity<fauString, T...> >
        explicit fauString(T&&... t) : base_tp(std::forward<T>(t)...)
        {}

        explicit fauString(base_tp const& he) : base_tp(he)
        {}

        fauString(fauString const& he)
            : base_tp(static_cast<base_tp const&>(he))
        {}

        fauString(futil::nil_tp)
           : base_tp(futil::nil)
        {}

        fauString& operator=(base_tp const& he)
        {
            base_tp::operator=(he);

            return *this;
        }

        fauString& operator=(fauString const& he)
        {
            base_tp::operator=(he);

            return *this;
        }

        fauString& operator=(futil::nil_tp)
        {
            base_tp::operator=(futil::nil);

            return *this;
        }
    };

} // freealgview

/*
inline freealgview::fauString operator"" _s(char const * const str, size_t len)
{
    return freealgview::fauString(str, len);
}
*/

#include "debugger/debugger.hpp"
#include "simple_debugger/simple_debugger.hpp"

#endif // FREEALGVIEW_FAUDATA_HPP_INCLUDED
