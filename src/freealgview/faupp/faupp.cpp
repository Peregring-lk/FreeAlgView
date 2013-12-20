// faupp.cpp

#include <fstream>

#include "faupp.hpp"

bool Faupp::operator()()
{
    // TODO: Search in a defined path, not in the current work directory.
    fspace::fstring file = owner();
    std::ifstream is(file);

    // TODO: Check for different stream errors.
    // TODO: Protocol to inform about errors to the user.
    if (not is.is_open()) {
        std::cerr << "File not open. Unknown reason" << std::endl;
        return false;
    }

    m_fauppLexer.switchStreams(is);

    bool succ = parse() == 0;

    return succ;
}

void Faupp::error(char const *msg)
{
    std::cerr << msg << std::endl;
}

int Faupp::lex()
{
    // The variable l_search_mark save the state of a TYPE_MARK search.
    // If the last token was a point ';', and it follows two symbols, the first
    // is a TYPE_MARK and the second an ordinary symbol.

    // The values are:
    //    2 -> Detected new sentence (';' or first call).
    //         We test the actual token, and, if it's a symbol, we ask for
    //         the next one and test it. If the second one is also a symbol, the
    //         first token is returned as a TYPE_MARK and the second like a
    //         symbol (in the next call). If not, both as symbols.
    //    1 -> We have a token forward.
    //    0 -> Common behaviour.
    static unsigned l_search_mark = 2;
    static int l_forward_token;
    static std::string l_forward_match;

    if (l_search_mark == 1) {
        d_val__ = fspace::fnode(fspace::fnode_id(l_forward_match));
        l_forward_match.clear();

        if (l_forward_token == ';' or l_forward_token == FauppToken::OP_FORCE)
            l_search_mark = 2;
        else
            l_search_mark = 0;

        return l_forward_token;
    }

    int l_token = m_fauppLexer.lex();

    if (l_token == 0)
        return 0;

    d_val__ =
        fspace::fnode(fspace::fnode_id(m_fauppLexer.matched()));

    if (l_search_mark == 2) {

        if (l_token == FauppToken::SYMBOL) {
            l_forward_token = m_fauppLexer.lex();

            if (l_forward_token == 0)
                return 0;

            l_forward_match = m_fauppLexer.matched();

            l_search_mark = 1;

            // NOTE: It's tested the next token, but returned the previous one.
            if (l_forward_token == FauppToken::SYMBOL)
                return FauppToken::TYPE_MARK;
            else
                return l_token;
        }

        l_search_mark = 0;
    }

    if (l_token == ';' || l_token == FauppToken::OP_FORCE)
        l_search_mark = 2;

    return l_token;
}

// Unimplemented print__().
void Faupp::print()
{
    print__();           // it would display tokens if --print was specified
}
