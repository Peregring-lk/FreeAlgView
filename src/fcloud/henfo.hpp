// henfo.hpp

#ifndef FSPACE_HENFO_HPP_INCLUDED
#define FSPACE_HENFO_HPP_INCLUDED

namespace fspace // henfo
{
    class henfo : public autoshared<futil::erasure>
    {
    private:
        using base_tp = autoshared<futil::erasure>;

    public:
        template<typename T, class = futil::ctor_purity<henfo, T> >
        henfo(T&& t)
            : base_tp(std::forward<T>(t)) // , m_id(t.hid())
        {}

        henfo(henfo const& he)
            : base_tp(static_cast<base_tp const&>(he)) //, m_id(he.m_id)
        {}

        henfo(futil::nil_tp = futil::nil)
           : base_tp(futil::nil)
        {}

        template<typename T, class = futil::assig_purity<henfo, T> >
        henfo& operator=(T&& t)
        {
            operator=(henfo(t));

            return *this;
        }

        henfo& operator=(henfo const& he)
        {
            base_tp::operator=(he);

            //  m_id = he.m_id;

            return *this;
        }

        henfo& operator=(futil::nil_tp)
        {
            base_tp::operator=(futil::nil);

            // m_id = futil::nil;

            return *this;
        }

    public:
        template<typename U, class = futil::is_incongruent<U, bool> >
        inline operator U&()
        {
            U& u = mf_unwrapp<U>(false);

            return u;
        }

        template<typename U, class = futil::is_incongruent<U, bool> >
        inline operator U const&() const
        {
            U& u = mf_unwrapp<U>(false);

            return u;
        }

    public:
        // henfo_id id() const
        // {
        //     return m_id;
        // }

    private:
        //  henfo_id m_id = futil::nil;
    };

}

#endif // FSPACE_HENFO_HPP_INCLUDED
