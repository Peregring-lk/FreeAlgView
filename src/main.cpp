// main.cpp

#include "fcloud/fspace.hpp"
#include "freealgview/freealgview.hpp"

// REVIEW: file styles: file guards, file comments, namespace comments and file
// names, include directives (useless headers and so on).

// REVIEW: 'using namespace ...' or abusive use of 'std::' 'fspace::'
// and so on?.

// REVIEW: File directory of the project, FreeAlgView modules and CMakeLists
// file.

// TODO: LIST
// 0.- Make parser.
// 1.- Make main
// 2.- Make `debugger`
// 3.- Make freealgview application: it installs the parser in the "freealgview"
//     tree, parses a file, installs the `debugger` visitor in each node of
//     the code, and execute each debugger using the iterator.

// TODO: SECONDARY LIST
// 0.- Revise core (move non-parametric contents).
// 1.- Revise private variable names.
// 2,- Revise include guards.
// 3.- Revise parser and lexer (composition, inheritance, base class, etcetera).
// ..- Revise core (erase useless things).

int main(int argc, char** argv)
{
    fspace::flaunch<freealgview::FreeAlgView>(argc, argv);

    return 0;
}
