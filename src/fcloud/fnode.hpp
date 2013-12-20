// fnode.hpp

#ifndef FSPACE_FNODE_HPP_INCLUDED
#define FSPACE_FNODE_HPP_INCLUDED

#include <set>

namespace fspace // fnode
{
    class fnode;
    class henfo;
    class fset;

    // using henfo_map = std::map<henfo_id, henfo>;
    using henfo_map = std::map<std::string, henfo>;

    using fnode_map = std::map<fnode_id, fnode>;
    using fnode_set = std::set<fnode>;

    class bleach_tp {};

    static bleach_tp bleacher()
    {
        static bleach_tp bleacher;

        return bleacher;
    }

    struct i_fnode;

    class fnode : public autoshared<i_fnode, true>
    {
    private:
        using base_tp = autoshared<i_fnode, true>;

        struct faddress;

        friend i_fnode;

    public: // structors
        fnode();

        template<typename... T, class = futil::ctor_purity<fnode, T...> >
        explicit fnode(T&&... t);

        fnode(fnode const& he);
        fnode(futil::nil_tp);

    public: // contents
        fset set() const;
        fset travel() const;

        //        type_id type() const;
        fnode_id id() const;
        unsigned nid() const;

        fnode father() const;
        fnode child(fnode_id const& id) const;
        fset children() const;

        fnode deref() const;

        // REMOVE: or HACK: I don't like that sort of primitive form of debug.
        void view_all() const;

        unsigned degree() const;
        unsigned idegree() const;
        unsigned edegree() const;
        unsigned order() const;

        fnode get(fnode_id const& id) const;

        template<typename T>
        T& get() const;

    private: // iteration
        fset mf_travel(bool new_travel) const;

    public: // iteration
        auto begin() const -> fnode_set::iterator;
        auto end() const -> fnode_set::iterator;

    public: // properties
        bool exists(fnode_id const& id) const;

        template<typename T>
        bool exists() const;

        bool root() const;
        bool head() const;
        bool leaf() const;

        bool identified() const;
        bool anonymous() const;

        bool pointer() const;

    public: // operators
        fnode operator[](fnode_id const& id) const;

        template<typename T, class =
                 futil::is_incongruent<T, fnode,
                                       bleach_tp, bool, int, float, char> >
        operator T&() const;

        faddress operator~() const;
        fnode operator*() const;
        fnode operator->() const;

        template<typename T, class =
                 futil::is_incongruent<T, fnode, fnode_id, bleach_tp> >
        fnode& operator<<(T&& t);

        fnode& operator<<(fnode he);
        fnode& operator<<(faddress he);
        fnode& operator<<(fnode_id const& id);
        fnode& operator<<(bleach_tp const& bleacher);

        void operator>>(fnode he);
        void operator<<=(fnode he);

        template<typename P>
        fset operator()(P const& p) const;

        bool operator==(fnode_id const& id) const;
        bool operator==(fnode const& he) const;

        bool operator!=(fnode_id const& id) const;
        bool operator!=(fnode const& he) const;

    public: // setters
        void id(fnode_id const& id);
        void father(fnode const& node);
        void ref(fnode const& he);

        void add(fnode he);

        template<typename T, class =
                 futil::is_incongruent<T, fnode, fnode_id,
                                       faddress, bleach_tp> >
        void add(T&& t);

        void erase(fnode_id const& id);
        void erase(fnode& he);

        template<typename T>
        void erase();

        void supplant(fnode he);
        void free();
        void unref();

        void clean();

        void clone();

    private:
        fnode mf_find(fnode_id const& id) const;
        henfo& mf_find(std::string const& id) const;

        template<typename R, typename ID, typename C>
        R& mf_find(ID const& id, C const& c) const;

        void mf_ensure_node(fnode_id const& id, bool warn = true);
        fnode mf_clone();

    private: // accessor declarations
        M_accessor_decl(fnode, mf_father)
        M_accessor_decl(fnode, mf_ref)
        M_accessor_decl(fnode_id, mf_id)
        M_accessor_decl(henfo_map, mf_henfo)
        M_accessor_decl(fnode_map, mf_children)
        M_accessor_decl(unsigned, mf_ref_degree)
    };

    fnode fanon();

    class fowner
    {
    protected:
        fspace::fnode m_owner;

    public:
        fspace::fnode owner() const
        {
            return m_owner;
        }

        void owner(fspace::fnode n)
        {
            m_owner = n;
        }
    };

} // fnode

inline fspace::fnode operator"" _n(char const * const str, size_t len)
{
    return fspace::fnode(fspace::fnode_id(str, len));
}

#endif // FSPACE_FNODE_HPP_INCLUDED
