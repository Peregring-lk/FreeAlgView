// ftravel.hpp

#ifndef FSPACE_FSET_HPP_INCLUDED
#define FSPACE_FSET_HPP_INCLUDED

#include <set>
#include <algorithm>

namespace fspace // fset
{
    struct i_fset
    {
    private:
        friend class fset;

        fnode_set m_set;
    };

    class fset : public pseudoshared<i_fset>
    {
    private:
        using base_tp = pseudoshared<i_fset>;

    public:
        fset() = default;

        fset(fnode const& node)
        {
            insert(node);
        }

        fset(fset const& he) : base_tp(static_cast<base_tp const&>(he))
        {}

    private: // accessors
        M_accessor(fnode_set, mf_set, m_set)

    public: // properties
        bool empty() const
        {
            return mf_set().empty();
        }

        std::size_t size() const
        {
            return mf_set().size();
        }

    public: // contents
        fset children() const
        {
            fset l_ret;

            for (auto const& i : *this)
                l_ret << i.children();

            return l_ret;
        }

        fset annexes() const
        {
            fset l_ret;

            for (auto const& i : *this)
                l_ret << *i;

            return l_ret;
        }

        fset parents() const
        {
            fset l_ret;

            for (auto const& i : *this)
                l_ret << i.father();

            return l_ret;
        }

        fset leafs() const
        {
            fset l_ret;

            for (auto const& i : *this)
                if (i.leaf())
                    l_ret << i;

            return l_ret;
        }

    public: // iteration

        auto begin() const -> fnode_set::iterator
        {
            return mf_set().begin();
        }

        auto end() const -> fnode_set::iterator
        {
            return mf_set().end();
        }

    public: // requests
        fset& insert(fnode const& node)
        {
            if (node)
                mf_set().insert(node);

            return *this;
        }

        fset& insert(fset const& he)
        {
            if (empty())
                *this = he;
            else
                mf_set().insert(he.begin(), he.end());

            return *this;
        }

        fset split_leafs()
        {
            fset l_leafs, l_rest;

            for (auto const& i : *this)
                if (i.leaf())
                    l_leafs << i;
                else
                    l_rest << i;

            *this << l_rest;

            return l_leafs;
        }

        fset& clear()
        {
            mf_set().clear();

            return *this;
        }

        fset& reproduce()
        {
            if (empty())
                return *this;

            *this << children().reproduce()
                  << annexes().reproduce();

            return *this;
        }

        fset& descend(unsigned steps = 1)
        {
            while (!empty() and steps--)
                *this = children();

            return *this;
        }

        fset& ascend(unsigned steps = 1)
        {
            if (!empty() and steps--)
                *this = parents();

            return *this;
        }

        fset& expand(unsigned steps = 1)
        {
            if (!empty() and steps > 0)
                *this << children().expand(steps - 1);

            return *this;
        }

        fset& close()
        {
            if (empty())
                return *this;

            fset l_leafs(split_leafs());

            *this = children().close();
            *this << l_leafs;

            return *this;
        }

        template<typename P>
        fset& filter(P const& p)
        {
            fset l_ret;

            for (auto const& i : *this)
                if (!p(i))
                    l_ret << i;

            *this = l_ret;

            return *this;
        }

    public: // operators
        fset& operator<<(fnode const& node)
        {
            return insert(node);
        }

        fset& operator<<(fset const& he)
        {
            return insert(he);
        }

        template<typename P>
        fset& operator()(P const& p)
        {
            *this = p(*this);

            return *this;
        }
    };
};

#endif // FSPACE_FSET_HPP_INCLUDED
