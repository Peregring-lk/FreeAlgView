// fidentifier.hpp

#ifndef FSPACE_FSTRING_HPP_INCLUDED
#define FSPACE_FSTRING_HPP_INCLUDED

#include <string>
#include <regex>

// REVIEW: Complete review of use of fidentifiers.
namespace fspace // fidentifier, type_id and fnode_id
{
    class fstring : public std::string
    {
    private:
        using base_tp = std::string;

    public: // structors
        template<typename... T, class = futil::ctor_purity<fstring, T...> >
        explicit fstring(T&&... t)
            : base_tp(std::forward<T>(t)...)
        {}

        fstring(std::string const& he)
            : base_tp(he)
        {}

        fstring(std::string&& he)
            : base_tp(std::move(he))
        {}

        fstring(fstring const& he)
            : base_tp(static_cast<base_tp const&>(he))
        {}

        fstring(futil::nil_tp)
           : base_tp()
        {}

    public: // operators
        bool operator!() const
        {
            return empty();
        }

        operator bool() const
        {
            return !empty();
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

        bool empty() const
        {
            return size() == 0;
        }
    };

    class fnode_id : public fstring
    {
    public:
        using fstring::fstring;
    };

} // fspace

inline fspace::fstring operator"" _fs(char const * const str, size_t len)
{
    return fspace::fstring(str, len);
}

inline fspace::fnode_id operator"" _f(char const * const str, size_t len)
{
    return fspace::fnode_id(str, len);
}

#endif // FSPACE_FIDENTIFIER_HPP_INCLUDED
