// fquery.hpp

#ifndef FSPACE_FQUERY_HPP_INCLUDED
#define FSPACE_FQUERY_HPP_INCLUDED

#include <list>

namespace fspace // fquery
{
    class i_qunit
    {
    public:
        fset operator()(fset& set) const
        {
            mf_apply(set);

            return set;
        }

    protected:
        virtual void mf_apply(fset& set) const {};
    };

    class qmax : public i_qunit
    {
    public:
        qmax(unsigned max) : m_max(max)
        {}

    private:
        void mf_apply(fset& set) const override
        {
            set.expand(m_max);
        }

        unsigned m_max;
    };

    class qmin : public i_qunit
    {
    public:
        qmin(unsigned min) : m_min(min)
        {}

    private:
        void mf_apply(fset& set) const override
        {
            set.descend(m_min);
            set.reproduce();
        }

        unsigned m_min;
    };

    class qdescend : public i_qunit
    {
    public:
        qdescend(unsigned steps = 1) : m_steps(steps)
        {}

    private:
        void mf_apply(fset& set) const override
        {
            set.descend(m_steps);
        }

        unsigned m_steps;
    };

    class qascend : public i_qunit
    {
    public:
        qascend(unsigned steps = 1) : m_steps(steps)
        {}

    private:
        void mf_apply(fset& set) const override
        {
            set.descend(m_steps);
        }

        unsigned m_steps;
    };

    class qchildren : public qdescend
    {}; // Use the qdescend's default constructor (descend only 1 step).

    class qparents : public qascend
    {}; // Use the qascend's default constructor (ascend only 1 step).

    class qinterval : public i_qunit
    {
    public:
        qinterval(unsigned min, unsigned max) : m_min(min), m_max(max)
        {}

    private:
        void mf_apply(fset& set) const override
        {
            set.descend(m_min);
            set.expand(m_max - m_min);
        }

        unsigned m_min;
        unsigned m_max;
    };

    class qid : public i_qunit
    {
    public:
        qid(fnode_id const& id) : m_id(id)
        {}

    private:
        void mf_apply(fset& set) const override
        {
            set.filter([this](fnode node) -> bool
                       { return node.id() == m_id; } );
        }

        fnode_id m_id;
    };

    class qany : public i_qunit
    {
    private:
        void mf_apply(fset& set) const override
        {
            set.reproduce();
        }
    };

    class qunit : public pseudoshared<i_qunit>
    {
    public:
        fset operator()(fset& set) const
        {
            return mf_get()(set);
        }
    };

    struct i_fquery
    {
    private:
        friend class fquery;

        std::list<qunit> m_query;
    };

    class fquery : public pseudoshared<i_fquery>
    {
    public:
        fset operator()(fset& set)
        {
            for (auto& i : mf_query())
                i(set);

            return set;
        }

        void push_back(qunit const& q)
        {
            mf_query().push_back(q);
        }

        fquery& operator<<(qunit const& q)
        {
            push_back(q);

            return *this;
        }

    private:
        M_accessor(std::list<qunit>, mf_query, m_query)
    };
}

#endif // FSPACE_FQUERY_HPP_INCLUDED
