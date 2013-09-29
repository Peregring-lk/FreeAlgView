// freealgview.cpp

#include "freealgview.hpp"

#include "faupp/faupp.hpp"

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

    class fauInit : fauCall, public fauFunctor<fauInit>
    {
    public:
        bool operator()() override;
    };

    /*
    // Makers
    class MakeField : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode_id id)
        {
            return fspace::fnode(id);
        }

        fspace::fnode operator()(fspace::type_id t)
        {
            return fspace::fnode(t);
        }

        fspace::fnode operator()(fspace::fnode_id id, fspace::type_id t)
        {
            return call<MakeField>(t) << id;
        }

        fspace::fnode operator()(fspace::fnode father,
                                 fspace::fnode_id id, fspace::type_id t)
        {
            auto l_new = call<MakeField>(id, t);

            father << l_new;

            return l_new;
        }

        fspace::fnode operator()(fspace::fnode father, fspace::fnode_id id)
        {
            auto l_new = call<MakeField>(id);

            father << l_new;

            return l_new;
        }
    };

    class MakeNil : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return call<MakeField>("nil"_t);
        }

        fspace::fnode operator()(fspace::fnode_id id)
        {
            return call<MakeNil>() << id;
        }

        fspace::fnode operator()(fspace::fnode father, fspace::fnode_id id)
        {
            auto l_new = call<MakeNil>(id);

            father << l_new;

            return l_new;
        }
    };

    class MakeStack : fauCall
    {
    public:
        fspace::fnode operator()()
        {
            return call<MakeField>("stack"_t);
        }

        fspace::fnode operator()(fspace::fnode_id id)
        {
            return call<MakeStack>() << id;
        }

        fspace::fnode operator()(fspace::fnode father, fspace::fnode_id id)
        {
            auto l_new = call<MakeStack>(id);

            father << l_new;

            return l_new;
        }
    };

    class MakeRef : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode ref)
        {
            auto l_ref = call<MakeField>("ref"_t);

            l_ref << ~ref;

            return l_ref;
        }

        fspace::fnode operator()(fspace::fnode_id id, fspace::fnode ref)
        {
            return call<MakeRef>(ref) << id;
        }

        fspace::fnode operator()(fspace::fnode father,
                                 fspace::fnode_id id, fspace::fnode ref)
        {
            auto l_new = call<MakeRef>(id, ref);

            father << l_new;

            return l_new;
        }
    };

    class MakeScript : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode contents)
        {
            auto l_script = call<MakeField>("script"_t);

            call<MakeRef>(l_script, "contents"_f, contents);
            call<MakeField>(l_script, "pc"_f, "0"_t);

            return l_script;
        }

        fspace::fnode operator()(fspace::fnode_id id, fspace::fnode contents)
        {
            return call<MakeScript>(contents) << id;
        }

        fspace::fnode operator()(fspace::fnode father,
                                 fspace::fnode_id id, fspace::fnode contents)
        {
            auto l_new = call<MakeScript>(id, contents);

            father << l_new;

            return l_new;
        }
    };
    */

    // Stack operations
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

    /*
    // Basic operations

    class Deref : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode node)
        {
            if (node.type() != "ref"_t)
                return futil::nil;

            return *node;
        }
    };
    */

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
            // REVIEW: But completely. Property system too chaotic.

            if (!code)
                return;

            // NOTE: Now, we have restrictions only for assignments.
            // Propagating forced and lvalue marks.

            // NOTE: Pending:
            // #!lassign #!oassign #!base_call #!call #!ref
            // #!return
            // Perhaps innecesary: #!symbol #!mark #!param
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

    class Rvaluiying : fauCall
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
                if (!call<Rvaluiying>(assign))
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
            call<Rvaluiying>(stmnt);
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

    // Implements the 'eval' function for the script type.
    // class ScriptEval : fauCall
    // {
    // public:
    //     void operator()(fspace::fnode script);
    // };

    // class CallEval : fauCall
    // {
    // public:
    //     void operator()(fspace::fnode callable)
    //     {
    //         if (callable == "#!"_f)
    //             call<ScriptEval>(callable);
    //     }
    // };

    /*
    class Return : fauCall
    {
    public:
        void operator()(fspace::fnode owner, fspace::fnode out)
        {
            auto l_out = owner["#!out"_f];

            if (!l_out)
                l_out = call<MakeField>(owner, "#!out"_f);

            out >> l_out;
        }
    };

    class Out : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode node)
        {
            return node["#!out"_f];
        }
    };

    class Pass : fauCall
    {
    public:
        void operator()(fspace::fnode source, fspace::fnode target)
        {
            // NOOP. To define in the future.
        }
    };

    class Translate : fauCall
    {
    public:
        fspace::fnode operator()(fspace::fnode sharp)
        {
            if (sharp.type() == "#!script"_t)
                return call<MakeScript>(sharp);
            else
                return call<MakeNil>();
        }
    };

    class ReturnScript : fauCall
    {
    public:
        void operator()(fspace::fnode script, fspace::fnode_id id)
        {
            if (script.type() != "script"_t)
                return;

            auto l_out =
                call<Translate>(call<Deref>(script["contents"_f])[id]);

            call<Return>(script, l_out);
        }
    };
    */
    // TODO: The general idea to be finally implemented is to create a language
    // for defining grammars and actions that will be transform to function
    // objets. The general process would be as following:
    //
    //    1 The system reads a file wrote in that core grammar with any other
    //    user grammars and associated semantic actions.
    //
    //    2 After grammars, the file describes also the main loop of the
    //    application.
    //
    //    3 The system creates for each grammar a function object that
    //    represents a parser for him, and an additional function object that
    //    represents the main script of the application, called for example the
    //    "init object".
    //
    //    4 The system executes that init object.
    //
    //    5 That init object will mainly contain a serie of instructions saying
    //    that the system should parser any other files wrote in that user
    //    grammars.
    //
    //    6 The system will execute the semantic actions associated with each
    //    grammar node as the user file is read.
    //
    //    7 The init object could indicate upon this a main loop, or perhaps the
    //    "main loop" is propagated among the semantic actions of the grammars,
    //    executed as the user files are read.

    // NOTE: The following lines represents more or less the instructions of
    // that init object.

    bool FreeAlgView::operator()()
    {
        // 'fau' is our node cloud. Here we save, as henfo objects, the
        // differents grammars and the others objects created by the
        // semantic actions of the user grammars. 'fau' is the root node of
        // the cloud.

        // We create the root tree. All parsers will be henfos of that node, and
        // that object as a henfo itself, in order to save all objects in the
        // cloud (if all nodes are in the cloud, that's very comfortable to
        // debugging purposes).
        fspace::fnode fau = fspace::fcloud(class_id());

        fau << *this;

        // The system reads the initial file and creates the initial object,
        // represented here through the class 'fauInit' (the function
        // 'fauInit::operator()()' represents the contents of that file).

        // A functor fauInit is created to add to the tree as a henfo. In the
        // following line that object is extracted and the owner is correctly
        // initialized. From now on the object can to know things about the tree
        // since the tree is its owner.

        fauInit init;
        fau << init;

        return init();
    }

    bool fauInit::operator()()
    {
        // That assumed file could define one or more grammars. We
        // pre-implement here only one grammar (faupp).
        fspace::fnode faupp(Faupp::class_id(), "machine"_t);

        // FIXME: Faupp can't be moved and being copied isn't safe. bisonc++ is
        // the culprit, and not I :P.
        // owner() << (faupp << Faupp());
        owner() << faupp;

        Faupp parser;

        parser.owner(faupp);

        // Our assumed file wants to parser other file wrote in
        // the language faupp. We get the parser and we parser that file
        // against it.
        fauString file_name(
            "/home/peregring-lk/Proyectos/"
            "FreeAlgView/FreeAlgView/"
            "src/.branch3/"
            ".finit");

        faupp << file_name;

        // The semantic actions of that user grammar creates different
        // nodes. The nodes created for each grammar is saved as children of
        // its associated root node. Go to FreeAlgView/faupp to see the
        // grammar's semantic actions.

        // TODO: Protocol about informing errors to the user.
        if (not parser()) {
            std::cerr << "Some error did happen during parsing" << std::endl;
            return false;
        }

        // install debugger
        // GraphicDebugger has not move constructor.
        Debugger d;

        owner() << d;

        /*
        // Initializating the machine.
        auto xtack = call<MakeStack>(faupp, "#!xtack"_f);

        call<StackPush>(xtack, call<MakeRef>(faupp.father()));
        call<StackPush>(xtack, call<MakeRef>(faupp));
        call<StackPush>(xtack, call<Translate>(faupp["#!code"_f]));
        */

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

    /*
    void ScriptEval::operator()(fspace::fnode script)
    {
        auto pc = script["pc"_f];

        if (pc.type() == "0"_t)
            call<ReturnScript>(script, "car"_f);
        else if (pc.type() == "1"_t)
            call<ReturnScript>(script, "cdr"_f);

        call<Increment>(pc);
    }
    */

} // FreeAlgView
