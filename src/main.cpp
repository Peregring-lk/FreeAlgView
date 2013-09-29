// main.cpp

#include "fcloud/fspace.hpp"
#include "freealgview/freealgview.hpp"

// REVIEW: file guards, comments, namespace comments and file
// names, include directives (useless headers and so on), object structure
// (queries, requests, etc).

int main(int argc, char** argv)
{
    fspace::flaunch<freealgview::FreeAlgView>(argc, argv);

    return 0;
}
