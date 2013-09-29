// lexer.cpp

#include "lexer.hpp"

using namespace std;

FauppLexer::FauppLexer(std::istream &in, std::ostream &out)
    : FauppLexerBase(in, out)
{}

FauppLexer::FauppLexer(std::string const &infile, std::string const &outfile)
    : FauppLexerBase(infile, outfile)
{}

string const& FauppLexer::matched() const
{
    return FauppLexerBase::matched();
}

int FauppLexer::lex()
{
    return lex__();
}

// Flexc++ functions.
void FauppLexer::preCode()
{}

void FauppLexer::print()
{
    print__();
}
