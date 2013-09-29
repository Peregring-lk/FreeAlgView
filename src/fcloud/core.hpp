// core.hpp
// TODO: Add guards. #ifndef ...

#include <iostream>
#include <string>
#include <map>
#include <tuple>
#include <stack>
#include <stdexcept>
#include <cstdint>

#ifdef __GNUC__
#include <cxxabi.h>
#endif

namespace futil
{
    template<bool b, typename T = void>
    using assert = typename std::enable_if<b, T>::type;

    template<typename... T>
    constexpr bool is_empty_pack()
    {
        return sizeof...(T) == 0;
    }

    template<typename... T>
    using proper_pack = assert<!is_empty_pack<T...>()>;

    template<typename T>
    inline std::string stypeid()
    {
        char const* l_cname = typeid(T).name();
        std::string l_sname(l_cname);

        // REVIEW: Is it safe to delete l_cname or not?
        // delete l_cname;

        return l_sname;
    }

    template<typename T>
    struct f_add_specs
    {
        static inline std::string& name(std::string& s)
        {
            return s;
        }
    };

    template<typename T>
    struct f_add_specs<T const>
    {
    public:
        static inline std::string& name(std::string& s)
        {
            s += " const";

            return s;
        }
    };

    template<typename T>
    struct f_add_specs<T volatile>
    {
        static inline std::string& name(std::string& s)
        {
            s += " volatile";

            return s;
        }
    };

    template<typename T>
    std::string demangled()
    {
        std::string l_mangledtype = stypeid<T>();

#ifdef __GNUC__
        int l_status;
        const char* id_error = "Weak error: demangled(): ";

        char* l_cdemangledtype =
            abi::__cxa_demangle(l_mangledtype.c_str(),
                                nullptr, nullptr, &l_status);

        std::string l_demangledtype(l_cdemangledtype);
        delete l_cdemangledtype;

        switch(l_status) {
        case 0: // succeeded.
            // Adding constant/volatile specifiers
            return f_add_specs<T>::name(l_demangledtype);

        default:
            std::cerr << id_error << l_mangledtype
                      << " (status abi::__cxa_demangle: " << l_status << ")"
                      << std::endl;

            return l_mangledtype;
        }

#else
        return l_mangledtype;
#endif
    }

    template<typename T>
    inline std::string demangled(T&)
    {
        return demangled<T>();
    }

    template<typename T>
    inline std::string demangled(T*)
    {
        return demangled<T*>();
    }

    inline std::string demangled_args_helper()
    {
        return "";
    }

    template<typename T>
    inline std::string demangled_args_helper(T&& t)
    {
        return demangled(std::forward<T>(t));
    }

    template<typename T, typename... Args>
    std::string demangled_args_helper(T&& t, Args&&... args)
    {
        return demangled_args_helper(std::forward<T>(t)) + ", "
            + demangled_args_helper(std::forward<Args>(args)...);
    }

    template<typename... Args>
    std::string demangled_args(Args&&... args)
    {
        return "(" + demangled_args_helper(std::forward<Args>(args)...) + ")";
    }

    template<typename T>
    inline std::string demangled_pack_helper()
    {
        return demangled<T>();
    }

    template<typename T, typename... Args,
             class = assert<sizeof...(Args) != 0>
             >
    std::string demangled_pack_helper()
    {
        return demangled_pack_helper<T>() + ", "
            + demangled_pack_helper<Args...>();
    }

    template<typename... Args>
    inline std::string demangled_pack()
    {
        return "<" + demangled_pack_helper<Args...>() + ">";
    }

    template<typename R, typename... P, typename C, typename... Args>
    inline std::string demangled_fun(C* c, char const* name, Args&&... args)
    {
        return futil::demangled<R>() + " "
            + futil::demangled(*c) + "::" + name + futil::demangled_pack<P...>()
            + futil::demangled_args(std::forward<Args>(args)...);
    }

    template<typename R, typename...P, typename... Args>
    inline std::string demangled_fun(char const* name, Args&&... args)
    {
        return futil::demangled<R>() + " " + name
            + futil::demangled_pack<P...>()
            + futil::demangled_args(std::forward<Args>(args)...);
    }

    template<typename T>
    using pluck = typename std::remove_cv<
        typename std::remove_reference<T>::type>::type;

    template<typename T>
    struct filter_lref_helper
    {
        using type = T;
    };

    template<typename T>
    struct filter_lref_helper<T&>
    {
        using type = typename filter_lref_helper<T>::type;
    };

    template<typename T>
    struct filter_lref_helper<T const>
    {
        using type = typename filter_lref_helper<T>::type const;
    };

    template<typename T>
    struct filter_lref_helper<T volatile>
    {
        using type = typename filter_lref_helper<T>::type volatile;
    };

    template<typename T>
    using filter_lref = typename filter_lref_helper<T>::type;

    template<typename T, typename U>
    constexpr bool is_same()
    {
        return std::is_same<T, U>::value;
    }

    template<typename T, typename U>
    constexpr bool is_congruent()
    {
        return is_same<pluck<T>, pluck<U> >();
    }

    template<typename...>
    struct is_pure_ctor_helper
    {
        static constexpr bool value = true;
    };

    template<class C, class D>
    struct is_pure_ctor_helper<C, D>
    {
        static constexpr bool value = !is_congruent<C, D>();
    };

    template<typename... T>
    constexpr bool is_pure_ctor()
    {
        return is_pure_ctor_helper<T...>::value;
    }

    template<typename T, typename U>
    constexpr bool is_incongruent_helper()
    {
        return !is_congruent<T, U>();
    }

    template<typename T, typename U, typename... More,
             class = proper_pack<More...> >
    constexpr bool is_incongruent_helper()
    {
        return is_incongruent_helper<T, U>()
            and is_incongruent_helper<T, More...>();
    }

    template<typename... T>
    using is_incongruent = assert<is_incongruent_helper<T...>()>;

    /*
    template<typename T, typename U>
    using equality = assert<is_same<T, U>()>;

    template<typename T, typename U>
    using congruence = assert<is_congruent<T, U>()>;
    */

    template<typename... T>
    using ctor_purity = assert<is_pure_ctor<T...>()>;

    template<typename... T>
    using assig_purity = ctor_purity<T...>;

    template<typename>
    inline constexpr bool delayed_false() noexcept
    {
        return false;
    }

    class erasure;

    template<class P, typename... Args>
    struct f_not
    {
    public: // types
        using plucked_tp = pluck<P>;

    public: // structors
        f_not(const plucked_tp& p) : m_p(p)
        {}

        f_not(plucked_tp&& p) : m_p(std::move(p))
        {}

    public: // requests
        bool operator()(Args const&... arg) const
        {
            return !m_p(arg...);
        }

    public: // data
        plucked_tp m_p;

    }; // _f_not

    using nil_tp = std::nullptr_t;
    constexpr nil_tp nil = nil_tp();

    template<typename T>
    class static_print;

    constexpr char t = '\t';
    constexpr char s = ' ';

    template<typename T>
    pluck<T>& singleton()
    {
        static pluck<T> s;

        return s;
    }

} // futil

// using namespace futil;

using refcounter_tp = uint_fast32_t;

inline constexpr refcounter_tp s_nullcounter() noexcept
{
    return 1;
}

using id_tp = uint_fast32_t;

inline constexpr id_tp s_nullid() noexcept
{
    return 1;
}

inline id_tp& s_last_id() noexcept
{
    static id_tp counter = s_nullid();

    return counter;
}

// REVIEW: Casting methods with wpackets (get accessors and so on). Traits?
// boost::polymorphic_cast?
template<typename T, bool id_tof = false>
struct wpacket;

// HACK: Careful!
static bool using_pool = true;

template<>
struct wpacket<futil::erasure>
{
    inline wpacket() : m_refs(s_nullcounter() + 1)
    {}

    /*
    inline wpacket(const wpacket&) = delete;
    inline wpacket(wpacket&) = delete;
    inline wpacket(wpacket&&) = delete;
    inline wpacket& operator=(const wpacket&) = delete;
    */

    inline const refcounter_tp& nrefs() const noexcept
    {
        return m_refs;
    }

    inline refcounter_tp& nrefs() noexcept
    {
        return m_refs;
    }

    inline bool cleaned() const noexcept
    {
        return nrefs() < s_nullcounter();
    }

    inline void clean() noexcept
    {
        m_refs = s_nullcounter() - 1;
    }

    inline void recharge() noexcept
    {
        m_refs = s_nullcounter() + 1;
    }

    virtual wpacket* clone() const = 0;
    virtual void liberate() = 0;

    template<typename T>
    static inline wpacket* s_pack(T&& t)
    {
        return new wpacket<T, false>(std::forward<T>(t));
    }

    template<typename T>
    inline operator T&()
    {
        return mf_object<T>();
    }

    template<typename T>
    inline operator T const&() const
    {
        return mf_object<T>();
    }

    template<typename T>
    inline T& object()
    {
        return mf_object<T>();
    }

    template<typename T>
    inline T const& object() const
    {
        return mf_object<T>();
    }

    refcounter_tp m_refs;

protected:
    virtual bool mf_type_checking(const std::type_info& target) const
        noexcept = 0;

    virtual void* mf_intermediate_cast() const noexcept = 0;

private:
    template<typename T>
    T& mf_object() const
    {
        if (!mf_type_checking(typeid(T)))
            throw std::logic_error
                ("Invalid cast. Trace: \n\t"
                 + futil::demangled_fun<T&, T>(this, "mf_object"));

        return *static_cast<T*>(mf_intermediate_cast());
    }
};

template<>
struct wpacket<futil::erasure, true> : public wpacket<futil::erasure, false>
{
    using base_tp = wpacket<futil::erasure, false>;

    inline wpacket() : m_id(++s_last_id())
    {}

    inline bool cleaned() const noexcept
    {
        return m_id == s_nullid();
    }

    inline void clean() noexcept
    {
        m_id = s_nullid();
    }

    inline void recharge() noexcept
    {
        // base_tp::recharge();

        ++m_refs;
        m_id = ++s_last_id();
    }

    inline const id_tp& id() const noexcept
    {
        return m_id;
    }

    inline id_tp& id() noexcept
    {
        return m_id;
    }

    template<typename T>
    static inline wpacket* s_pack(T&& t)
    {
        return new wpacket<T, true>(std::forward<T>(t));
    }

    id_tp m_id;
};

template<typename T, bool id_tof>
struct wpacket : public wpacket<futil::erasure, id_tof>
{
    using base_tp = wpacket<futil::erasure, id_tof>;

    mutable T m_object;

    template<typename... Args>
    inline wpacket(Args&&... args)
        : m_object(std::forward<Args>(args)...)
    {}

    template<typename U = T>
    inline U const& object() const noexcept
    {
        return mf_object<U>();
    }

    template<typename U = T>
    inline U& object() noexcept
    {
        return mf_object<U>();
    }

    template<typename U>
    inline operator U&() noexcept
    {
        return mf_object<U>();
    }

    template<typename U>
    inline operator U const&() const noexcept
    {
        return mf_object<U>();
    }

    inline void clean() noexcept
    {
        try {
            m_object.~T();
        } catch (...) {

            std::cerr << "Warning: Worrying discovery: type "
                      << futil::demangled<T>()
                      << " has an unsafe destructor!!"
                      << std::endl;

            std::cerr << "Likely memory leak." << std::endl;

        } // try-catch block

        base_tp::clean();
    }

    template<typename... Args>
    inline void recharge(Args&&... args)
    {
        // Dangerous? m_object has been manual destroyed.
        new (&m_object) T(std::forward<Args>(args)...);
        //        m_object = T(std::forward<Args>(args)...);

        base_tp::recharge();
    }

    wpacket* clone() const override;
    void liberate() override;

    template<typename... Args>
    inline static wpacket* s_pack(Args&&... args)
    {
        return new wpacket(std::forward<Args>(args)...);
    }

private:
    inline bool mf_type_checking(const std::type_info& target) const noexcept
    {
        return target == typeid(T);
    }

    inline void* mf_intermediate_cast() const noexcept
    {
        return static_cast<void*>(&mf_object());
    }

    template<typename U = T>
    inline U& mf_object() const noexcept
    {
        static_assert(std::is_same<U, T>(),
                      "wpacket::object(): Invalid type");

        return m_object;
    }
};

using address_id = std::size_t;

struct packet_pool
{
    template<typename T, bool id_tof>
    using packet_tp = wpacket<T, id_tof>;

private:
    template<typename T, typename... Args>
    struct building_type
    {
        using type = T;
    };

    template<typename T>
    struct building_type<futil::erasure, T>
    {
        using type = T;
    };

    template<typename T, typename... Args>
    using building_tp_helper = typename building_type<T, Args...>::type;

    template<typename T, typename... Args>
    //  using building_tp = futil::filter_lref<building_tp_helper<T, Args...> >;
    using building_tp = futil::pluck<building_tp_helper<T, Args...> >;

    template<typename T, bool id_tof>
    struct packet_stack_tp : public std::stack<packet_tp<T, id_tof>*>
    {
        inline ~packet_stack_tp() noexcept
        {
            while (!this->empty()) {
                operator delete(this->top());
                this->pop();
            }
        }
    };

public:
    // IDEA: When I've a number enough of free packets, I could erase some
    // packets in order to reduce the ocupied memory. That's a sort of garbage
    // recollection. That limit could be different for each type, and should be
    // calculated in execution time (or not).
    template<typename T, bool id_tof>
    inline static packet_stack_tp<T, id_tof>& stack_lpackets() noexcept
    {
        static packet_stack_tp<T, id_tof> lpackets;

        // TODO: SFINAE idiom to detect what?

        return lpackets;
    }

    // HACK: I don't like such kind of debug information.
    template<typename T, bool id_tof>
    inline static unsigned& s_created_objs() noexcept
    {
        static unsigned counter = 0;

        return counter;
    }

    template<typename T, bool id_tof>
    inline static unsigned& s_reused_objs() noexcept
    {
        static unsigned counter = 0;

        return counter;
    }

    template<typename T, bool id_tof>
    inline static unsigned& s_liberated_objs() noexcept
    {
        static unsigned counter = 0;

        return counter;
    }

    template<typename T, bool id_tof>
    inline static unsigned s_instack_objs() noexcept
    {
        return stack_lpackets<T, id_tof>().size();
    }

    template<typename T, bool id_tof>
    inline static void s_debug() noexcept
    {
        std::cout << std::endl;

        std::cout << "Information about memory pool for "
                  << futil::demangled<T>() << std::endl;

        std::cout << "------------------------" << std::endl;

        std::cout << "Created objects: " << s_created_objs<T, id_tof>()
                  << std::endl;
        std::cout << "Reused objects: " << s_reused_objs<T, id_tof>()
                  << std::endl;
        std::cout << "Liberated objects: " << s_liberated_objs<T, id_tof>()
                  << std::endl;
        std::cout << "In stack objects: " << s_instack_objs<T, id_tof>()
                  << std::endl;

        std::cout << std::endl;
    }

    template<typename T, bool id_tof, typename... Args>
    inline static packet_tp<T, id_tof>* s_pack(Args&&... args)
    {
        using T = building_tp<T, Args...>;

        packet_tp<T, id_tof>* l_packet;

        // HACK: Careful! using_pool.
        if (!using_pool or stack_lpackets<T, id_tof>().empty()) {
            l_packet = packet_tp<T, id_tof>::
                s_pack(std::forward<Args>(args)...);

            ++s_created_objs<T, id_tof>();
        }
        else {
            l_packet = stack_lpackets<T, id_tof>().top();
            l_packet->recharge(std::forward<Args>(args)...);

            stack_lpackets<T, id_tof>().pop();
            ++s_reused_objs<T, id_tof>();
        }

        return l_packet;
    }

    template<typename T, bool id_tof>
    inline static bool s_is_nil(packet_tp<T, id_tof>* wp) noexcept
    {
        return wp == nullptr;
    }

    template<typename T, bool id_tof>
    inline static bool s_clonable(packet_tp<T, id_tof>*& wp)
    {
        return wp->nrefs() > s_nullcounter() + 1;
    }

    template<typename T, bool id_tof>
    inline static void s_clone(packet_tp<T, id_tof>*& wp)
    {
        if (s_is_nil(wp))
            return;

        // An object is clone, only if there's more than one copy active.
        if (s_clonable(wp)) {
            s_leave(wp);

            // wp->clone() calls to pool_tp::s_pack(...)
            wp = wp->clone();
        }
    }

    template<typename T, bool id_tof>
    inline static void s_share(packet_tp<T, id_tof>* wp) noexcept
    {
        if (s_is_nil(wp))
            return;

        ++wp->nrefs();
    }

    template<typename T, bool id_tof>
    inline static void s_leave(packet_tp<T, id_tof>* wp) noexcept
    {
        if (s_is_nil(wp))
            return;

        --wp->nrefs();

        if (wp->nrefs() == s_nullcounter())
            s_liberate(wp);
    }

    template<typename T, bool id_tof>
    inline static void s_liberate(packet_tp<T, id_tof>* wp) noexcept
    {
        if (s_is_nil(wp))
            return;

        wp->liberate();
    }

};

template<typename T, bool id_tof>
inline wpacket<T, id_tof>* wpacket<T, id_tof>::clone() const
{
    return packet_pool::s_pack<T, id_tof>(object());
}

template<typename T, bool id_tof>
inline void wpacket<T, id_tof>::liberate()
{
    if (!this->cleaned()) {
        this->clean();

     ++packet_pool::s_liberated_objs<T, id_tof>();

     // HACK: Careful! using_pool
     if (using_pool)
         packet_pool::stack_lpackets<T, id_tof>().push(this);
     else
         delete this;
    }
}

template<typename T, bool id_tof = false>
class shared_obj_base
{
protected: // packet
    using pool_tp = packet_pool;

    // template<typename T, bool id_tof>
    using packet_tp = pool_tp::packet_tp<T, id_tof>;

protected: // protected queries
    inline packet_tp*& packet() const noexcept
    {
        return ptr_packet;
    }

    inline bool mf_is_nil() const noexcept
    {
        //        return packet() == nullptr;
        return pool_tp::s_is_nil(packet());
    }

protected: // structors
    template<typename... Args,
             class = futil::ctor_purity<shared_obj_base, Args...> >
    inline shared_obj_base(Args&&... args)
        : ptr_packet(pool_tp::s_pack<T, id_tof>(std::forward<Args>(args)...)),
          b_manual(true)
    {}

    shared_obj_base(shared_obj_base const& he) noexcept
    : ptr_packet(he.packet()), b_manual(true)
    {
        pool_tp::s_share<T, id_tof>(he.packet());
    }

    template<typename U>
    inline shared_obj_base(shared_obj_base<U, id_tof> const& he) noexcept
        : ptr_packet(he.packet()), b_manual(true)
    {
        pool_tp::s_share<U, id_tof>(he.packet());
    }

    inline shared_obj_base(futil::nil_tp) noexcept
        : ptr_packet(nullptr)
    {}

    inline ~shared_obj_base() noexcept
    {
        if (b_manual)
            pool_tp::s_leave<T, id_tof>(packet());
    }

protected: // protected requests
    virtual inline void mf_check_use_invariant(bool const_instance) const
    {}

    inline unsigned mf_nid() const
    {
        return std::uintptr_t(packet());
    }

public: // operators
    inline bool operator<(const shared_obj_base& he) const noexcept
    {
        return packet() < he.packet();
    }

    inline bool operator==(const shared_obj_base& he) const noexcept
    {
        return packet() == he.packet();
    }

    inline bool operator==(futil::nil_tp) const noexcept
    {
        return mf_is_nil();
    }

    inline shared_obj_base& operator=(const shared_obj_base& he) noexcept
    {
        pool_tp::s_leave<T, id_tof>(packet());

        // REVIEW: Assign pointer or assign contents?
        packet() = he.packet();

        pool_tp::s_share<T, id_tof>(packet());

        return *this;
    }

    inline shared_obj_base& operator=(futil::nil_tp) noexcept
    {
        pool_tp::s_leave<T, id_tof>(packet());

        packet() = nullptr;

        return *this;
    }

    operator bool() const noexcept
    {
        return !mf_is_nil();
    }

private:
    inline void throw_if_is_nil() const
    {
        if (mf_is_nil())
            throw std::logic_error
                ("shared_obj_base::mf_get(): invalid use of nil");
    }

protected:
    template<typename U>
    inline U& mf_unwrapp(bool const_instance, bool ignore_check = false) const
    {
        throw_if_is_nil();

        if (!b_external_checker && !ignore_check)
            mf_check_use_invariant(const_instance);

        U& i = packet()->template object<U>();

        return i;
    }

private:
    class scope_checker
    {
    public:
        scope_checker(shared_obj_base* owner, bool const_instance)
            : ptr_owner(owner)
        {
            if (ptr_owner && ptr_owner->b_external_checker)
                ptr_owner = nullptr;

            if (ptr_owner) {
                ptr_owner->b_external_checker = true;
                ptr_owner->mf_check_use_invariant(const_instance);
            }
        }

        scope_checker(const scope_checker& he) noexcept
            : ptr_owner(he.ptr_owner)
        {
            he.ptr_owner = nullptr;
        }

        ~scope_checker() noexcept
        {
            if (ptr_owner)
                ptr_owner->b_external_checker = false;
        }

    private:
        shared_obj_base* ptr_owner;
    };

protected:
    scope_checker mf_scope_check()
    {
        return scope_checker(this, false);
    }

    scope_checker mf_scope_check() const
    {
        return scope_checker(this, true);
    }

    template<typename U = T>
    inline U const& mf_get() const
    {
        return mf_unwrapp<U>(true);
    }

    template<typename U = T>
    inline U& mf_get()
    {
        return mf_unwrapp<U>(false);
    }

    template<typename U = T>
    inline U& cmf_get() const
    {
        return mf_unwrapp<U>(false);
    }

    void mf_liberate() noexcept
    {
        pool_tp::s_liberate(packet());
    }

    bool mf_clonable() const
    {
        return pool_tp::s_clonable(packet());
    }

    void mf_clone() const
    {
        if (mf_clonable())
            pool_tp::s_clone(packet());
    }

private:
    mutable packet_tp* ptr_packet;
    mutable bool b_external_checker = false;
    mutable bool b_manual = false;
};

#define M_var(var) mf_get().var
#define M_cvar(var) cmf_get().var

#define M_accessor_body(var) { return M_var(var); }
#define M_accessor_cbody(var) { return M_cvar(var); }

#define M_accessor_decl(type, fun) \
    type& fun(); \
    type const& fun() const; \
    type& c##fun() const;

#define M_accessor_impl(type, fun, var, ...) \
    inline type& __VA_ARGS__::fun() M_accessor_body(var) \
    inline type const& __VA_ARGS__::fun() const M_accessor_body(var) \
    inline type& __VA_ARGS__::c##fun() const M_accessor_cbody(var)

#define M_accessor(type, fun, var) \
    inline type& fun() M_accessor_body(var) \
    inline type const& fun() const M_accessor_body(var) \
    inline type& c##fun() const M_accessor_cbody(var)


template<class T, bool shared = true, bool softly = false,
         bool copy_on_write = false>
struct shared_obj : public shared_obj_base<T>
{
private:
    using base_tp = shared_obj_base<T>;

public:
    template<typename... Args, class = futil::ctor_purity<shared_obj, Args...> >
    shared_obj(Args&&... args) : base_tp(std::forward<Args>(args)...)
    {}

    shared_obj(const shared_obj& he) : base_tp(static_cast<const base_tp&>(he))
    {}

    shared_obj(futil::nil_tp) : base_tp(futil::nil)
    {}
};

template<class T>
struct shared_obj<T, true, true> : public shared_obj_base<T, true>
{
private:
    using base_tp = shared_obj_base<T, true>;

public:
    inline operator bool() const noexcept
    {
        return mf_active();
    }

    inline bool operator==(const shared_obj& he)
    {
        return base_tp::operator==(static_cast<const base_tp&>(he))
            and m_id == he.m_id;
    }

    inline bool operator<(const shared_obj& he)
    {
        return mf_inactive() ?
            true : base_tp::operator<(static_cast<const base_tp&>(he));
    }

    inline shared_obj& operator=(const shared_obj& he)
    {
        base_tp::operator=(he);

        if (!this->mf_nil())
            m_id = this->packet()->id();

        return *this;
    }

    inline shared_obj& operator=(futil::nil_tp)
    {
        base_tp::operator=(futil::nil);

        return *this;
    }

protected:
    inline bool mf_active() const
    {
        return !this->mf_is_nil() && m_id == this->packet()->id();
    }

    inline bool mf_inactive() const
    {
        return !mf_active();
    }

    inline void mf_suspend() const
    {
        if (mf_active())
            this->mf_liberate();
    }

    inline id_tp mf_getid() const
    {
        return m_id;
    }

public:
    template<typename... Args, class = futil::ctor_purity<shared_obj, Args...> >
    inline shared_obj(Args&&... args)
        : base_tp(std::forward<Args>(args)...),
          m_id(this->packet()->id())
    {}

    inline shared_obj(const shared_obj& he)
         : base_tp(static_cast<const base_tp&>(he)),
           m_id(he.m_id)
    {}

    inline shared_obj(futil::nil_tp)
         : base_tp(futil::nil)
    {}

private:
    id_tp m_id;

private:
    inline void mf_check_use_invariant(bool) const override
    {
        if (!mf_inactive())
            throw std::logic_error
                ("softlyshared::get: Attempt to use an invalid object. "
                 "It could have been a segmentation fault!");
    }
};

template<class T>
struct shared_obj<T, false> : public shared_obj_base<T>
{
private:
    using base_tp = shared_obj_base<T>;

public:
    template<typename... Args, class = futil::ctor_purity<shared_obj, Args...> >
    inline shared_obj(Args&&... args)
        : base_tp(std::forward<Args>(args)...), b_owner(true)
    {}

    inline shared_obj(const shared_obj& he)
        : base_tp(static_cast<const base_tp&>(he)),
          b_owner(false)
    {
        he.b_owner = false;
    }

    inline shared_obj(futil::nil_tp)
        : base_tp(futil::nil)
    {}

public:
    inline shared_obj& operator=(const shared_obj& he)
    {
        base_tp::operator=(he);

        b_owner = false;
        he.b_owner = false;

        return *this;
    }

    inline shared_obj& operator=(futil::nil_tp)
    {
        base_tp::operator=(futil::nil);

        return *this;
    }

private:
    mutable bool b_owner;

    inline void mf_check_use_invariant(bool const_instance) const override
    {
        if (!const_instance && !b_owner) {
            this->mf_clone();
            b_owner = true;
        }
    }
};

template<class T, bool softly>
struct shared_obj<T, true, softly, true>
    : public shared_obj<T, true, softly, false>
{
private:
    using base_tp = shared_obj<T, true, softly, false>;
    using ptr_packet_tp = typename base_tp::packet_tp*;
    using pool_tp = typename base_tp::pool_tp;

private:
    struct cw_mark_tp : public shared_obj<ptr_packet_tp>
    {
        using base_tp = shared_obj<ptr_packet_tp>;

        cw_mark_tp() : base_tp(futil::nil)
        {}

        cw_mark_tp(cw_mark_tp const& he)
            : base_tp(static_cast<base_tp const&>(he))
        {}

        cw_mark_tp(ptr_packet_tp packet) : base_tp(packet)
        {}

        cw_mark_tp(futil::nil_tp) : base_tp(futil::nil)
        {}

        cw_mark_tp& operator=(cw_mark_tp const& he)
        {
            base_tp::operator=(static_cast<base_tp const&>(he));

            return *this;
        }

        cw_mark_tp& operator=(ptr_packet_tp packet)
        {
            if (this->mf_is_nil())
                return operator=(cw_mark_tp(packet));
            else
                this->mf_get() = packet;

            return *this;
        }

        cw_mark_tp& operator=(futil::nil_tp)
        {
            base_tp::operator=(futil::nil);

            return *this;
        }

        ptr_packet_tp& get()
        {
            return this->mf_get();
        }

        bool operator!() const
        {
            return base_tp::operator!();
        }
    };

public:
    template<typename... Args, class = futil::ctor_purity<shared_obj, Args...> >
    shared_obj(Args&&... args) : base_tp(std::forward<Args>(args)...)
    {}

    shared_obj(shared_obj const& he)
        : base_tp(static_cast<base_tp const&>(he)), cw_mark(he.cw_mark)
    {}

    shared_obj(futil::nil_tp const&) : base_tp(futil::nil)
    {}

private:
    mutable cw_mark_tp cw_mark;

    void mf_set_cw_mark(ptr_packet_tp packet) const
    {
        cw_mark = packet;
    }

    void mf_unmark_cw() const
    {
        cw_mark = futil::nil;
    }

    void mf_cw_cancel() const
    {
        mf_set_cw_mark(futil::nil);
        mf_unmark_cw();
    }

    bool mf_cw_active() const
    {
        if (cw_mark)
            if (!cw_mark.get()) {
                mf_unmark_cw();
                return false;
            }
            else
                return true;
        else
            return false;
    }

    bool mf_cw_mode() const
    {
        return mf_cw_active() and cw_mark.get() == this->packet();
    }

protected:
    bool mf_cw_mark_changed() const
    {
        return mf_cw_active() and cw_mark.get() != this->packet();
    }

    void mf_mark_cw_mode() const
    {
        if (!*this)
            return;

        if (mf_cw_mode()) // To be independent from other objects.
            mf_unmark_cw();

        if (this->mf_clonable())
            mf_set_cw_mark(this->packet());
    }

    void mf_cw_mark_upload() const
    {
        this->packet() = cw_mark.get();

        pool_tp::s_share(this->packet());

        mf_unmark_cw();
    }

    inline void mf_check_use_invariant(bool const_instance) const override
    {
        if (mf_cw_mark_changed())
            mf_cw_mark_upload();
        else if (mf_cw_mode() and !const_instance) {
            if (this->mf_clonable()) {
                this->mf_clone();
                mf_set_cw_mark(this->packet());
                mf_unmark_cw();
            }
            else
                mf_cw_cancel();
        }
    }
};

template<class T, bool copy_on_write = false>
using autoshared = shared_obj<T, true, false, copy_on_write>;

template<class T, bool copy_on_write = false>
using softlyshared = shared_obj<T, true, true, copy_on_write>;

template<class T>
using pseudoshared = shared_obj<T, false>;

/*
template<class T>
class implicit_nil_impl : public T
{
public:
    implicit_nil() : T(futil::nil)
    {}

    template<typename... Args,
             class = futil::ctor_purity<implicit_nil, Args...> >
    implicit_nil(Args&& args) : T(std::forward<Args>(args)...)
    {}

    implicit_nil(implicit_nil const& he) : T(static_cast<T const&>(he))
    {}
};

template<class T>
using implicit_nil = implicit_nil_impl<T>;
*/
