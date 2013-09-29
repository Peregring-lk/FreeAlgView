// fnode.i.hpp

#ifndef FSPACE_FNODE_HPP_IMPL_INCLUDED
#define FSPACE_FNODE_HPP_IMPL_INCLUDED

namespace fspace // i_fnode
{
    struct i_fnode
    {
    public: // structors
        i_fnode() = default;

        explicit i_fnode(type_id const& type)
            : m_type(type)
        {}

        explicit i_fnode(fnode_id const& id, type_id const type = futil::nil)
            : m_id(id), m_type(type)
        {}

        i_fnode(i_fnode const& he)
            : m_father(he.m_father),
              m_ref(he.m_ref),
              m_type(he.m_type),
              m_id(he.m_id),
              m_children(he.m_children)
              // m_henfo excluded.
        {}

        i_fnode(i_fnode&& he)
            : m_henfo(std::move(he.m_henfo)),
              m_children(std::move(he.m_children)),
              u_ref_degree(he.u_ref_degree)
        {}

    private:
        friend fnode;

        unsigned u_ref_degree = 0;

        fnode m_father = futil::nil;
        fnode m_ref = futil::nil;
        type_id m_type = futil::nil;
        fnode_id m_id = futil::nil;

        henfo_map m_henfo;
        fnode_map m_children;
    };

} // i_fnode

namespace fspace // fnode::faddress
{
    struct fnode::faddress
    {
        faddress(fnode he) : m_node(he)
        {}

        fnode operator*() const
        {
            return m_node;
        }

        fnode m_node;
    };

} // fnode::faddress

namespace fspace // fanon
{
    inline fnode fanon()
    {
        return fnode(""_f);
    }
}

namespace fspace // fnode
{
    inline fnode::fnode() : fnode::fnode(futil::nil)
    {}

    template<typename... T, class>
    inline fnode::fnode(T&&... t)
        : base_tp(std::forward<T>(t)...)
    {}

    inline fnode::fnode(fnode const& he)
        : base_tp(static_cast<base_tp const&>(he))
    {}

    inline fnode::fnode(futil::nil_tp)
        : base_tp(futil::nil)
    {}

    // contents
    inline fset fnode::set() const
    {
        return fset(*this);
    }

    inline void fnode::view_all() const
    {
        std::cout << std::endl << "ID: " << id() << ", " << "TYPE: " << type()
                  << " Degree: " << degree() << " " << std::endl;

        for (auto const& i : mf_children())
            i.second.view_all();
    }

    inline fset fnode::travel() const
    {
        return set().reproduce();
    }

    inline type_id fnode::type() const
    {
        if (!*this)
            return futil::nil;
        else
            return mf_type();
    }

    inline fnode_id fnode::id() const
    {
        if (!*this)
            return futil::nil;
        else
            return mf_id();
    }

    inline unsigned fnode::nid() const
    {
        return mf_nid();
    }

    inline fnode fnode::father() const
    {
        if (!*this)
            return futil::nil;
        else
            return mf_father();
    }

    inline fset fnode::children() const
    {
        fset children_set;

        for (auto const& i : mf_children())
            children_set << i.second;

        return children_set;
    }

    inline fnode fnode::deref() const
    {
        if (!*this)
            return futil::nil;
        else
            return mf_ref();
    }

    inline unsigned fnode::degree() const
    {
        return mf_children().size();
    }

    inline unsigned fnode::idegree() const
    {
        return mf_ref_degree() + father() ? 1 : 0;
    }

    inline unsigned fnode::edegree() const
    {
        return degree() + (deref() ? 1 : 0);
    }

    inline unsigned fnode::order() const
    {
        return travel().size();
    }

    inline fnode fnode::child(fnode_id const& id) const
    {
        return get(id);
    }

    inline fnode fnode::get(fnode_id const& id) const
    {
        return mf_find(id);
    }

    // inline henfo& fnode::get(henfo_id const& id) const
    // {
    //     return mf_find(id);
    // }

    template<typename T>
    inline T& fnode::get() const
    {
        return mf_find(futil::stypeid<T>());
    }

    // iteration
    inline fset fnode::mf_travel(bool new_travel) const
    {
        static fset l_travel;

        if (new_travel)
            l_travel = travel();

        return l_travel;
    }

    inline auto fnode::begin() const -> fnode_set::iterator
    {
        return mf_travel(true).begin();
    }

    inline auto fnode::end() const -> fnode_set::iterator
    {
        return mf_travel(false).end();
    }

    inline bool fnode::exists(fnode_id const& id) const
    {
        return mf_find(id);
    }

    // inline bool fnode::exists(henfo_id const& id) const
    // {
    //     return mf_find(id);
    // }

    template<typename T>
    inline bool fnode::exists() const
    {
        return mf_find(futil::stypeid<T>());
    }

    inline bool fnode::root() const
    {
        return !father();
    }

    inline bool fnode::head() const
    {
        return father().root();
    }

    inline bool fnode::leaf() const
    {
        if (!*this)
            return true;
        else
            return mf_children().empty();
    }

    inline bool fnode::typed() const
    {
        return type();
    }

    inline bool fnode::untyped() const
    {
        return !typed();
    }

    inline bool fnode::identified() const
    {
        return !anonymous();
    }

    inline bool fnode::anonymous() const
    {
        return !id() or id().empty();
    }

    inline bool fnode::pointer() const
    {
        return deref();
    }

    // operators
    // inline henfo& fnode::operator[](henfo_id const& id) const
    // {
    //     return get(id);
    // }

    template<typename T, class>
    inline fnode::operator T&() const
    {
        return get<futil::pluck<T>>();
    }

    inline fnode fnode::operator[](fnode_id const& id) const
    {
        if (!*this)
            return futil::nil;
        else
            return get(id);
    }

    inline fnode::faddress fnode::operator~() const
    {
        if (mf_cw_mark_changed())
            mf_cw_mark_upload();

        return *this;
    }

    inline fnode fnode::operator*() const
    {
        return deref();
    }

    inline fnode fnode::operator->() const
    {
        return deref();
    }

    template<typename T, class>
    fnode& fnode::operator<<(T&& t)
    {
        add(std::forward<T>(t));

        return *this;
    }

    inline fnode& fnode::operator<<(fnode he)
    {
        add(he);

        return *this;
    }

    inline fnode& fnode::operator<<(faddress he)
    {
        ref(*he);

        return *this;
    }

    inline fnode& fnode::operator<<(fnode_id const& id)
    {
        fnode::id(id);

        return *this;
    }

    inline fnode& fnode::operator<<(type_id const& id)
    {
        type(id);

        return *this;
    }

    inline fnode& fnode::operator<<(bleach_tp const& bleacher)
    {
        *this = fnode("@@@fnode::operator<<"_f);

        return *this;
    }

    inline void fnode::operator<<=(fnode he)
    {
        he.supplant(*this);
    }

    inline void fnode::operator>>(fnode he)
    {
        supplant(he);
    }

    template<typename P>
    inline fset fnode::operator()(P const& p) const
    {
        return set()(p);
    }

    inline bool fnode::operator==(fnode_id const& id) const
    {
        if (!*this)
            return false;

        return this->id() == id;
    }

    inline bool fnode::operator==(type_id const& type) const
    {
        if (!*this)
            return false;

        return this->type() == type;
    }

    inline bool fnode::operator==(fnode const& he) const
    {
        return base_tp::operator==(he);
    }

    inline bool fnode::operator!=(fnode_id const& id) const
    {
        return !this->operator==(id);
    }

    inline bool fnode::operator!=(type_id const& type) const
    {
        return !this->operator==(type);
    }

    inline bool fnode::operator!=(fnode const& he) const
    {
        return !this->operator==(he);
    }

    // setters
    inline void fnode::type(type_id const& id)
    {
        mf_ensure_node("@@@fnode::type@@@"_f);

        mf_type() = id;
    }

    inline void fnode::father(fnode const& node)
    {
        mf_ensure_node("@@@fnode::father@@@"_f);

        auto& l_father = mf_father();

        if (l_father)
            l_father.mf_children().erase(id());

        l_father = node;

        if (l_father)
            // TODO: Check repeated identifiers.
            // If repaated, std::map::insert prevents the insertion.
            l_father.mf_children().insert(fnode_map::value_type(id(), *this));
    }

    inline void fnode::ref(fnode const& he)
    {
        if (!*this)
            return;

        if (he) {
            auto l_ref = mf_ref();

            if (l_ref)
                --l_ref.mf_ref_degree();

            ++const_cast<unsigned&>(he.mf_ref_degree());

            mf_ref() = he;
        }
    }

    inline void fnode::id(fnode_id const& id)
    {
        if (id.empty())
            return;

        if (!*this)
            mf_ensure_node(id, false);
        else {
            auto l_father = mf_father();

            father(futil::nil);
            mf_id() = id;
            father(l_father);
        }
    }

    inline void fnode::tydentify()
    {
        id(static_cast<fnode_id>(type()));
    }

    inline void fnode::add(fnode he)
    {
        if (!he)
            return;

        if (he.anonymous())
            he.tydentify();

        // If the node remains being anonymous (type empty), fnode::father calls
        // to "mf_ensure_node".
        he.father(*this);
    }

    template<typename T, class>
    inline void fnode::add(T&& t)
    {
        if (t.owner())
            t.owner().erase<T>();

        t.owner(*this);

        fspace::henfo henfo(t);

        mf_henfo().insert(henfo_map::value_type(futil::stypeid<T>(), henfo));
    }

    inline void fnode::erase(fnode_id const& id)
    {
        get(id).father(futil::nil);
    }

    // inline void fnode::erase(henfo_id const& id)
    // {
    //     mf_henfo().erase(id);
    // }

    template<typename T>
    inline void fnode::erase()
    {
        mf_henfo().erase(futil::stypeid<T>());
    }

    inline void fnode::supplant(fnode he)
    {
        father(futil::nil);

        if (!he)
            return;

        id(he.id());

        if (!he.root()) {
            he.father().mf_children()[id()] = *this;

            mf_father() = he.father();
            he.mf_father() = futil::nil;
        }
    }

    inline void fnode::free()
    {
        father(futil::nil);
    }

    inline void fnode::unref()
    {
        if (*this)
            mf_ref() = futil::nil;
    }

    inline void fnode::clean()
    {
        auto l_children = mf_children();

        for (auto& i : l_children)
            i.second.father(futil::nil);

        // REVIEW: Clean henfo?

        l_children.clear();
    }

    inline void fnode::clone()
    {
        mf_clone();
    }

    inline fnode fnode::mf_clone()
    {
        fspace::fnode l_clon(id(), type());

        l_clon << ~deref();

        for (auto& i : mf_children())
            l_clon << i.second.mf_clone();

        *this = l_clon;

        return l_clon;
    }

    inline fnode fnode::mf_find(fnode_id const& id) const
    {
        return mf_find<fnode>(id, mf_children());
    }

    // inline henfo& fnode::mf_find(henfo_id const& id) const
    // {
    //     return mf_find<henfo>(id, mf_henfo());
    // }

    inline henfo& fnode::mf_find(std::string const& id) const
    {
        return mf_find<henfo>(id, mf_henfo());
    }


    template<typename R, typename ID, typename C>
    R& fnode::mf_find(ID const& id, C const& c) const
    {
        static R l_ret;

        l_ret = futil::nil;

        auto const& l_it = c.find(id);

        if (l_it != c.end())
            l_ret = l_it->second;

        return l_ret;
    }

    inline void fnode::mf_ensure_node(fnode_id const& id, bool warn)
    {
        if (!*this)
            *this = fnode(id);

        /*
        if (warn)
            std::cerr << "fnode::mf_ensure_node: node automatically identify."
                      << std::endl;
        */
    }

    // accessor declarations
    M_accessor_impl(fnode, mf_father, m_father, fnode)
    M_accessor_impl(fnode, mf_ref, m_ref, fnode)
    M_accessor_impl(type_id, mf_type, m_type, fnode)
    M_accessor_impl(fnode_id, mf_id, m_id, fnode)
    M_accessor_impl(henfo_map, mf_henfo, m_henfo, fnode)
    M_accessor_impl(fnode_map, mf_children, m_children, fnode)
    M_accessor_impl(unsigned, mf_ref_degree, u_ref_degree, fnode)
} // fnode

#endif // FSPACE_FNODE_HPP_IMPL_INCLUDED
