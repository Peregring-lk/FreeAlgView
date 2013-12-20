// freealgview.cpp

#include "freealgview.hpp"

#include "faupp/faupp.hpp"

// TODO: Generalizating debuggers.
#include "debugger/debugger.hpp"
#include "simple_debugger/simple_debugger.hpp"
#include "graphic_debugger/graphic_debugger.hpp"

namespace freealgview // FreeAlgView
{
    class fauCall
    {
    protected:
        template<typename T, typename... Args>
        static auto call(Args&&... args)
            -> decltype(T()(std::forward<Args>(args)...))
        {
            static T t;

            return t(std::forward<Args>(args)...);
        }
    };

    class fauInit : fauCall, public fspace::fowner
    {
    public:
        bool operator()();
    };

    class StackTop : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode stack)
        {
            auto l_car = stack["car"_f];

            if (!l_car)
                return futil::nil;

            return *l_car;
        }
    };

    class StackPop : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode stack)
        {
            auto l_car = stack["car"_f];

            if (!l_car)
                return futil::nil;

            auto l_top = *l_car;
            auto l_cdr = stack["cdr"_f];

            if (!l_cdr)
                l_car >> futil::nil;
            else {
                auto l_cdar = l_cdr["car"_f];
                auto l_cddr = l_cdr["cdr"_f];

                if (!l_cdar) {
                    l_cdr >> l_car;
                }
                else {
                    l_cdar >> l_car;
                    l_cddr >> l_cdr;
                }
            }

            /*
            l_cdr["car"_f] >> stack["car"_f];
            l_cdr["cdr"_f] >> l_cdr;
            */

            return l_top;
        }
    };

    class StackPush : fauCall
    {
    public:
        void operator()(fspace::fnode stack, fspace::fnode node)
        {
            if (!node)
                return;

            if (stack != "stack"_f)
                return;

            fspace::fnode l_car = stack["car"_f];

            if (!l_car) {
                stack << ("car"_n << ~node);

                return;
            }

            fspace::fnode l_cdr = stack["cdr"_f];

            if (!l_cdr) {
                stack << ("cdr"_n << ~*l_car);

                l_car << ~node;

                return;
            }

            auto l_newcdr("cdr"_n);

            l_newcdr << l_car;
            l_newcdr << l_cdr;

            stack << l_newcdr;
            stack << ("car"_n << ~node);
        }
    };

    class StackEmpty : fauCall
    {
    public:
        bool operator()(fspace::fnode stack)
        {
            return !call<StackTop>(stack);
        }
    };

    class StackSize : fauCall
    {
    public:
        int operator()(fspace::fnode stack)
        {
            if (stack != "stack"_f)
                return 1;
            else if (call<StackEmpty>(stack))
                return 0;
            else
                return 1 + call<StackSize>(*stack["cdr"_f]);
        }
    };

    class ListSize : fauCall
    {
    public:
        int operator()(fspace::fnode list)
        {
            if (!list)
                return 0;
            else if (list != "#!list"_f)
                return 1;
            else if (call<StackEmpty>(list))
                return 0;
            else
                return 1 + call<ListSize>(*list["cdr"_f]);
        }
    };

    class One : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return "1"_n;
        }
    };

    class IsZero : fauCall
    {
    public:
        bool operator()(fspace::fnode node)
        {
            return node == "0"_f;
        }
    };

    class Increment : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode node)
        {
            auto l_next = std::to_string(std::stoi(node.id()) + 1);

            return fspace::fnode(fspace::fnode_id(l_next));
        }
    };

    class MakeNumber : fauCall
    {
    public:
        fspace::fnode operator()(unsigned i)
        {
            return fspace::fnode(fspace::fnode_id(std::to_string(i)));
        }
    };

    class Decrement : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode node)
        {
            auto l_next = std::to_string(std::stoi(node.id()) - 1);

            return fspace::fnode(fspace::fnode_id(l_next));
        }
    };

    class ArrackIdx : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode arrack)
        {
            if (arrack != "arrack"_f)
                return futil::nil;
            else
                return arrack["idx"_f];
        }
    };

    class ArrackEmpty : fauCall
    {
    public:
        bool operator()(fspace::fnode arrack)
        {
            return !call<ArrackIdx>(arrack);
        }
    };

    class ArrackGet : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode arrack, fspace::fnode idx)
        {
            if (arrack != "arrack"_f or !idx)
                return futil::nil;

            return arrack[idx.id()];
        }

        fspace::fnode operator()(fspace::fnode arrack, unsigned i)
        {
            return call<ArrackGet>(arrack, call<MakeNumber>(i));
        }
    };

    class ArrackGetFirst : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode arrack)
        {
            return *call<ArrackIdx>(arrack);
        }
    };

    class ArrackFirst : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode arrack)
        {
            return *call<ArrackGetFirst>(arrack);
        }
    };

    class ArrackPush : fauCall
    {
    public:
        void operator()(fspace::fnode arrack, fspace::fnode node)
        {
            if (!node)
                return;

            auto l_idx = call<ArrackIdx>(arrack);

            if (call<ArrackEmpty>(arrack)) {
                auto l_one = call<One>() << ~node;

                arrack << l_one;
                arrack << ("idx"_n << ~l_one);
            }
            else {
                auto l_next = call<Increment>(call<ArrackGetFirst>(arrack))
                    << ~node;

                arrack << l_next;
                l_idx << ~l_next;
            }
        }
    };

    class ArrackPop : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode arrack)
        {
            if (call<ArrackEmpty>(arrack))
                return futil::nil;

            auto l_first = call<ArrackGetFirst>(arrack);
            auto l_out = *l_first;

            auto l_prev = call<ArrackGet>(arrack, call<Decrement>(l_first));

            l_first >> futil::nil;

            if (call<IsZero>(l_prev))
                call<ArrackIdx>(arrack) >> futil::nil;
            else
                call<ArrackIdx>(arrack) << ~l_prev;

            return l_out;
        }
    };

    class ArrackSize : fauCall
    {
    public:
        unsigned operator()(fspace::fnode arrack)
        {
            if (call<ArrackEmpty>(arrack))
                return 0;
            else
                return std::stoi((*call<ArrackIdx>(arrack)).id());
        }
    };

    // Implements the 'next' function for the machine type.
    class Machine : fauCall
    {
    private:
        static fspace::fnode& s_get()
        {
            static fspace::fnode l_machine = futil::nil;

            return l_machine;
        }

    public:
        static fspace::fnode get()
        {
            return s_get();
        }

        static void set(fspace::fnode machine)
        {
            if (machine == "machine"_f)
                s_get() = machine;
        }

    public:
        bool operator()();
    };

    class GetPc : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return *Machine::get()["pc"_f];
        }
    };

    class GetXtack : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return *Machine::get()["xtack"_f];
        }
    };

    class GetCtxTable : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return *Machine::get()["ctx_table"_f];
        }
    };

    class XtackTop : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return call<StackTop>(call<GetXtack>());
        }
    };

    class XtackPop : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return call<StackPop>(call<GetXtack>());
        }
    };

    class XtackPush : fauCall
    {
    public:
        void operator()(fspace::fnode node)
        {
            call<StackPush>(call<GetXtack>(), node);
        }
    };

    class PcTop : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return call<StackTop>(call<GetPc>());
        }
    };

    class PcPop : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return call<StackPop>(call<GetPc>());
        }
    };

    class PcPush : fauCall
    {
    public:
        void operator()(fspace::fnode code)
        {
            call<StackPush>(call<GetPc>(), code << ~"pre"_n);
        }
    };

    class PushContext : fauCall
    {
    public:
        void operator()()
        {
            call<PushContext>(fspace::fanon());
        }

        void operator()(fspace::fnode context)
        {
            call<ArrackPush>(call<GetCtxTable>(), context);
        }
    };

    class PopContext : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return call<ArrackPop>(call<GetCtxTable>());
        }
    };

    class FirstContext : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return call<ArrackFirst>(call<GetCtxTable>());
        }
    };

    class GetContext : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode idx)
        {
            return *call<ArrackGet>(call<GetCtxTable>(), idx);
        }

        fspace::fnode operator()(unsigned idx)
        {
            return *call<ArrackGet>(call<GetCtxTable>(), idx);
        }
    };

    class SizeCtxTable : fauCall
    {
    public:
        unsigned operator()()
        {
            return call<ArrackSize>(call<GetCtxTable>());
        }
    };

    class GetSymbol : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode symbol)
        {
            unsigned ctx_size = call<SizeCtxTable>();

            while (ctx_size > 0) {
                auto l_ctx = call<GetContext>(ctx_size--);
                auto l_found = l_ctx[symbol.id()];

                if (l_found)
                    return l_found;
            }

            return futil::nil;
        }
    };

    class AssignSymbol : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode symbol,
                                 fspace::fnode value = futil::nil)
        {
            auto l_symbol_found = call<GetSymbol>(symbol);

            if (!l_symbol_found)
                call<FirstContext>() << symbol;

            // XXX: Be careful! Clone?
            if (value)
                value >> symbol;

            return symbol;
        }
    };

    class IsLvalue : fauCall
    {
    public:
        bool operator()(fspace::fnode node)
        {
            return node["properties"_f]["lvalue"_f];
        }
    };

    class IsForced : fauCall
    {
    public:
        bool operator()(fspace::fnode node)
        {
            return node["properties"_f]["forced"_f];
        }
    };

    class EnsureProperties : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode node)
        {
            auto l_properties = node["properties"_f];

            if (!l_properties) {
                l_properties = "properties"_n;

                node << l_properties;
            }

            return l_properties;
        }
    };

    class AddLvalue : fauCall
    {
    public:
        void operator()(fspace::fnode node)
        {
            auto l_properties = call<EnsureProperties>(node);

            if (!call<IsLvalue>(node))
                l_properties << "lvalue"_n;
        }
    };

    class AddForced : fauCall
    {
    public:
        void operator()(fspace::fnode node)
        {
            auto l_properties = call<EnsureProperties>(node);

            if (!call<IsForced>(node))
                l_properties << "forced"_n;
        }
    };

    class SemanticRestrictions : fauCall
    {
    public:
        void operator()(fspace::fnode code)
        {
            if (!code)
                return;

            // NOTE: Now, we have restrictions only for assignments.
            // Propagating forced and lvalue marks.
            if (code == "#!list"_f) {
                call<SemanticRestrictions>(*code["car"_f]);
                call<SemanticRestrictions>(*code["cdr"_f]);
            }
            else if (code == "#!assign"_f)
                call<SemanticRestrictions>(*code["lvalue"_f]);
            else if (code == "#!access"_f) {
                auto l_obj = *code["obj"_f];
                auto l_field = *code["field"_f];

                call<AddLvalue>(l_obj);

                bool b_islvalue = call<IsLvalue>(code);
                bool b_isforced = call<IsForced>(code);

                if (b_islvalue)
                    call<AddLvalue>(l_field);

                if (b_isforced) {
                    call<AddForced>(l_obj);
                    call<AddForced>(l_field);
                }

                call<SemanticRestrictions>(l_obj);

                if (b_islvalue or b_isforced)
                    call<SemanticRestrictions>(l_field);
            }
            else if (code =="#!object"_f) {
                call<SemanticRestrictions>(*code["context"_f]);
                call<SemanticRestrictions>(*code["overloads"_f]);
            }
            else if (code == "#!closure"_f)
                call<SemanticRestrictions>(*code["body"_f]);
            else if (code == "#!expr_stmnt"_f)
                call<SemanticRestrictions>(*code["value"_f]);
            else if (code == "#!if"_f or code == "#!while"_f or
                     code == "#!dowhile"_f)
                call<SemanticRestrictions>(*code["body"_f]);
        }
    };

    class Rvalueying : fauCall
    {
    public:
        bool operator()(fspace::fnode code)
        {
            if (**code == "pushed"_f)
                call<PopContext>();

            auto l_top = call<XtackTop>();
            auto l_trivial_overload = *l_top["0"_f];

            if (!l_trivial_overload) {
                code.unref();
                return true;
            }
            else {
                call<XtackPop>();
                call<PushContext>(*l_top);
                call<PcPush>(*(*l_top["0"_f])["body"_f]);

                *code << ~"pushed"_n;

                return false;
            }
        };
    };

    class AssignPre : fauCall
    {
    public:
        void operator()(fspace::fnode assign)
        {
            auto l_value = *assign["lvalue"_f];

            if (l_value == "#!symbol"_f)
                call<AddForced>(l_value);

            call<PcPush>(l_value);
            call<PcPush>(*assign["rvalue"_f]);
        }
    };

    class AssignIn : fauCall
    {
    public:
        void operator()(fspace::fnode assign)
        {
            if (assign["rvalue"_f]) {
                if (!call<Rvalueying>(assign))
                    return;

                // The element stack.top() is the lvalue.
                // The stack.pop().top() is the rvalue.
                auto l_lvalue = call<XtackPop>();
                auto l_rvalue = call<XtackPop>();

                l_rvalue.clone();
                l_rvalue >> l_lvalue;
            }
            else {
                call<XtackPop>();
                assign.unref();
            }
        }
    };

    class AccessPre : fauCall
    {
    public:
        void operator()(fspace::fnode access)
        {
            call<PcPush>(*access["obj"_f]);

            *access << ~"obj"_n;
        }
    };

    class AccessIn : fauCall
    {
    public:
        void operator()(fspace::fnode access)
        {
            if (**access == "obj"_f) {
                auto l_obj = call<XtackPop>();

                // TODO: Marks?
                if (!*l_obj) // If 'a' hasn't a context, we create one.
                    l_obj << ~fspace::fanon();

                call<PushContext>(*l_obj);
                call<PcPush>(*access["field"_f]);

                *access << ~"field"_n;
            }
            else if (**access == "field"_f) {
                call<PopContext>();
                access.unref();
            }
        }
    };

    class MakeError : fauCall
    {
    public:
        void operator()(fspace::fnode_id msg)
        {
            Machine::get() <<
                ("#!error"_n << ~(fspace::fnode(msg)
                                  << ("Last node:"_n << ~call<PcTop>())));
        }
    };

    class MakeEnd : fauCall
    {
    public:
        void operator()()
        {
            Machine::get() << "The End :)"_n;
        }
    };

    class GetError : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return Machine::get()["#!error"_f];
        }
    };

    class SymbolPre : fauCall
    {
    public:
        void operator()(fspace::fnode node)
        {
            auto l_symbol = *node["value"_f];
            auto l_found = call<GetSymbol>(l_symbol);

            if (!l_found and call<IsForced>(node)) {
                l_symbol.clone();
                l_found = l_symbol;
                call<AssignSymbol>(l_symbol);
            }

            if (l_found)
                // if (call<IsLvalue>(node))
                // TODO: Lvalue irrelevant for Symbols?
                    call<XtackPush>(l_found);
            else
                call<MakeError>("The symbol: '"_f + l_symbol.id() +
                                "' doesn't exist"_f);
        }
    };

    class ObjectPre : fauCall
    {
    public:
        void operator()(fspace::fnode object)
        {
            // TODO: Closure mark (context's name).
            call<PushContext>();
            call<PcPush>(*(*object["context"_f])["body"_f]);
        }
    };

    class GetMark : fauCall
    {
    public:
        fspace::fnode_id operator()(fspace::fnode node)
        {
            return (*(*node["mark"_f])["value"_f]).id();
        }
    };

    class SearchOverloadLoc : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode idx, fspace::fnode params)
        {
            if (!params)
                return futil::nil;

            if (params == "#!param"_f) {
                auto l_mark = call<GetMark>(params);
                auto l_found = idx[l_mark];

                if (l_found) {
                    call<MakeError>("Repeated overload"_f);
                    return futil::nil;
                }

                l_found = fspace::fnode(l_mark);
                idx << l_found;

                return l_found;
            }

            auto l_car = *params["car"_f];
            auto l_mark = call<GetMark>(l_car);
            auto l_found = idx[l_mark];

            if (!l_found) {
                l_found = fspace::fnode(l_mark);
                idx << l_found;
            }

            return call<SearchOverloadLoc>(l_found, *params["cdr"_f]);
        }
    };

    class MakeOverload : fauCall
    {
    public:
        void operator()(fspace::fnode overload, fspace::fnode obj)
        {
            if (!overload)
                return;

            if (overload == "#!list"_f) {
                call<MakeOverload>(*overload["car"_f], obj);
                call<MakeOverload>(*overload["cdr"_f], obj);

                return;
            }

            // TODO: Ignored return marks.
            auto l_params = *overload["params"_f];
            auto l_size = fspace::fnode_id(std::to_string
                                           (call<ListSize>(l_params)));

            auto l_idx = obj[l_size];

            if (!l_idx) {
                l_idx = fspace::fnode(l_size);
                obj << l_idx;
            }

            if (l_size == "0"_f)
                l_idx << ~overload;
            else {
                auto l_loc = call<SearchOverloadLoc>(l_idx, l_params);

                l_loc << ~overload;
            }
        }
    };

    class ObjectIn : fauCall
    {
    public:
        void operator()(fspace::fnode object)
        {
            auto l_ctx = call<PopContext>();
            fspace::fnode l_obj = fspace::fanon();

            l_obj << ~l_ctx;

            call<MakeOverload>(*object["overloads"_f], l_obj);
            call<XtackPush>(l_obj);

            object.unref();
        }
    };

    class StmntPre : fauCall
    {
    public:
        void operator()(fspace::fnode stmnt)
        {
            call<PcPush>(*stmnt["value"_f]);
        }
    };

    class StmntIn : fauCall
    {
    public:
        void operator()(fspace::fnode stmnt)
        {
            call<Rvalueying>(stmnt);
        }
    };

    class ListPre : fauCall
    {
    public:
        void operator()(fspace::fnode node)
        {
            call<PcPush>(*node["cdr"_f]);
            call<PcPush>(*node["car"_f]);
        }
    };

    class EvalPre : fauCall
    {
    public:
        void operator()(fspace::fnode node)
        {
            node << ~"in"_n;

            if (node == "#!assign"_f)
                call<AssignPre>(node);
            else if (node == "#!symbol"_f)
                call<SymbolPre>(node);
            else if (node == "#!list"_f)
                call<ListPre>(node);
            else if (node == "#!access"_f)
                call<AccessPre>(node);
            else if (node == "#!object"_f)
                call<ObjectPre>(node);
            else if (node == "#!expr_stmnt"_f)
                call<StmntPre>(node);
        }
    };

    class EvalIn : fauCall
    {
    public:
        void operator()(fspace::fnode node)
        {
            if (node == "#!assign"_f)
                call<AssignIn>(node);
            else if (node == "#!access"_f)
                call<AccessIn>(node);
            else if (node == "#!object"_f)
                call<ObjectIn>(node);
            else if (node == "#!expr_stmnt"_f)
                call<StmntIn>(node);
            else
                node.unref();

            if (!*node)
                node << ~"post"_n;
        }
    };

    bool FreeAlgView::operator()()
    {
        fspace::fnode fau = fspace::fcloud("FreeAlgView"_f);

        fau << *this;

        // 'init' controls the execution process.
        fauInit init;
        fau << init;

        return init();
    }

    bool fauInit::operator()()
    {
        // REVIEW: Faupp::class_id? Use of identifiers in general must be
        // changed.
        fspace::fnode faupp("Faupp"_f);

        owner() << faupp;

        // FIXME: Faupp can't be moved and coping it is unsafe. bisonc++ is the
        // culprit, and not me :P.
        // owner() << (faupp << Faupp(...));
        Faupp parser;

        // FIXME: Parser isn't a henfo of faupp!!, but faupp is its owner.
        parser.owner(faupp);

        // Start-up file.
        faupp << ".finit"_fs;

        // TODO: Protocol about informing errors to the user.
        if (not parser()) {
            std::cerr << "Some error did happen during parsing" << std::endl;
            return false;
        }

        // install debugger
        // NOTE: GraphicDebugger has not move constructor.
        Debugger d;

        owner() << d;

        // REVIEW: Debugging.

        // TODO: if fnode returns a copy of henfo, the object after the
        // casting can't be catch by value: `d` should point to the
        // object in memory and not to any temporal value.
        // Debugger& d = owner()[Debugger::class_hid()];

        // main loop
        auto machine = *faupp;
        auto code = *machine["code"_f];

        // Initialize machine.
        Machine::set(machine);

        call<PcPush>(code);
        call<PushContext>();
        call<SemanticRestrictions>(code);

        do {
            // debugging part
            d();

            Debugger::show(false);

        } while (call<Machine>());

        d();
        Debugger::show(false);

        return true;
    }

    bool Machine::operator()()
    {
        if (call<GetError>())
            return false;

        auto l_top = call<PcTop>();

        if (!l_top) {
            call<MakeEnd>();

            return false;
        }

        if (*l_top == "pre"_f)
            call<EvalPre>(l_top);
        else if (*l_top == "in"_f)
            call<EvalIn>(l_top);
        else if (*l_top == "post"_f)
            call<PcPop>();
        else {
            call<MakeError>("Unrecognized control code"_f);
            return false;
        }

        return true;
    }

} // FreeAlgView
