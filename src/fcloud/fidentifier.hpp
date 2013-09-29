// fidentifier.hpp

#ifndef FSPACE_FIDENTIFIER_HPP_INCLUDED
#define FSPACE_FIDENTIFIER_HPP_INCLUDED

#include <string>
#include <regex>

// REVIEW: Complete review of use of fidentifiers.
namespace fspace // fidentifier, type_id and fnode_id
{
    class fidentifier : public pseudoshared<std::string>
    {
    private:
        using base_tp = pseudoshared<std::string>;

    public: // structors
        template<typename... T, class = futil::ctor_purity<fidentifier, T...> >
        explicit fidentifier(T&&... t)
            : base_tp(std::forward<T>(t)...)
        {}

        fidentifier(fidentifier const& he)
            : base_tp(static_cast<base_tp const&>(he))
        {}

        fidentifier(futil::nil_tp)
           : base_tp(futil::nil)
        {}

        operator std::string() const
        {
            return mf_get();
        }

    public: // operators
        friend std::ostream& operator<<(std::ostream& os, fidentifier const& id)
        {
            if (id)
                os << id.mf_get();

            return os;
        }

        bool operator<(fidentifier const& he) const
        {
            if (!*this || !he)
                return true;

            return mf_get() < he.mf_get();
        }

        bool operator==(fidentifier const& he) const
        {
            if (!this and !he)
                return true;

            if (static_cast<bool>(this) != static_cast<bool>(he))
                return false;

            return mf_get() == he.mf_get();
        }

        bool operator!=(fidentifier const& he) const
        {
            return !operator==(he);
        }

        fidentifier& operator=(fidentifier const& he)
        {
            base_tp::operator=(he);

            return *this;
        }

        fidentifier& operator=(futil::nil_tp)
        {
            base_tp::operator=(futil::nil);

            return *this;
        }

        template<typename T>
        fidentifier operator+(T&& t) const
        {
            return fidentifier(mf_get() + std::forward<T>(t));
        }

        template<typename T>
        fidentifier& operator<<(T&& t)
        {
            mf_get() += std::forward<T>(t);

            return *this;
        }

        template<typename T>
        bool operator>>(T&& t)
        {
            std::regex l_regex(std::string(std::forward<T>(t)));

            auto l_flags =
                std::regex_constants::match_default |
                std::regex_constants::match_not_bol |
                std::regex_constants::match_not_eol |
                std::regex_constants::match_not_bow |
                std::regex_constants::match_not_eow |
                std::regex_constants::match_any;

            return std::regex_match(std::string(*this), l_regex, l_flags);
        }

    public: // queries
        std::size_t size() const
        {
            return mf_get().size();
        }

        char const* c_str() const
        {
            return mf_get().c_str();
        }

        bool empty() const
        {
            return size() == 0;
        }
    };

    class type_id : public fidentifier
    {
    private:
        using base_tp = fidentifier;

    public:
        template<typename... T, class = futil::ctor_purity<type_id, T...> >
        explicit type_id(T&&... t) : base_tp(std::forward<T>(t)...)
        {}

        explicit type_id(base_tp const& he) : base_tp(he)
        {}

        type_id(type_id const& he)
            : base_tp(static_cast<base_tp const&>(he))
        {}

        type_id(futil::nil_tp)
           : base_tp(futil::nil)
        {}

        type_id& operator=(base_tp const& he)
        {
            base_tp::operator=(he);

            return *this;
        }

        type_id& operator=(type_id const& he)
        {
            base_tp::operator=(he);

            return *this;
        }

        type_id& operator=(futil::nil_tp)
        {
            base_tp::operator=(futil::nil);

            return *this;
        }
    };

    class fnode_id : public fidentifier
    {
    private:
        using base_tp = fidentifier;

    public:
        template<typename... T, class = futil::ctor_purity<fnode_id, T...> >
        explicit fnode_id(T&&... t) : base_tp(std::forward<T>(t)...)
        {}

        explicit fnode_id(base_tp const& he) : base_tp(he)
        {}

        fnode_id(fnode_id const& he)
            : base_tp(static_cast<base_tp const&>(he))
        {}

        fnode_id(futil::nil_tp)
           : base_tp(futil::nil)
        {}

        fnode_id& operator=(base_tp const& he)
        {
            base_tp::operator=(he);

            return *this;
        }

        fnode_id& operator=(fnode_id const& he)
        {
            base_tp::operator=(he);

            return *this;
        }

        fnode_id& operator=(futil::nil_tp)
        {
            base_tp::operator=(futil::nil);

            return *this;
        }

        fnode_id operator+(fnode_id id) const
        {
            return fnode_id(mf_get() + static_cast<std::string>(id));
        }

    };

    class henfo_id : public fidentifier
    {
    private:
        using base_tp = fidentifier;

    public:
        template<typename... T, class = futil::ctor_purity<henfo_id, T...> >
        explicit henfo_id(T&&... t) : base_tp(std::forward<T>(t)...)
        {}

        explicit henfo_id(base_tp const& he) : base_tp(he)
        {}

        henfo_id(henfo_id const& he)
            : base_tp(static_cast<base_tp const&>(he))
        {}

        henfo_id(futil::nil_tp)
           : base_tp(futil::nil)
        {}

        henfo_id& operator=(base_tp const& he)
        {
            base_tp::operator=(he);

            return *this;
        }

        henfo_id& operator=(henfo_id const& he)
        {
            base_tp::operator=(he);

            return *this;
        }

        henfo_id& operator=(futil::nil_tp)
        {
            base_tp::operator=(futil::nil);

            return *this;
        }
    };

} // fspace

inline fspace::henfo_id operator"" _h(char const * const str, size_t len)
{
    return fspace::henfo_id(str, len);
}

inline fspace::type_id operator"" _t(char const * const str, size_t len)
{
    return fspace::type_id(str, len);
}

inline fspace::fnode_id operator"" _fn(char const * const str, size_t len)
{
    return fspace::fnode_id(str, len);
}

inline fspace::fnode_id operator"" _f(char const * const str, size_t len)
{
    return fspace::fnode_id(str, len);
}

inline std::string operator"" _s(char const* const str, size_t len)
{
    return std::string(str, len);
}

#endif // FSPACE_FIDENTIFIER_HPP_INCLUDED
