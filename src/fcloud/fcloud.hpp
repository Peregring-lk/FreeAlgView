// ftreecloud.hpp

#ifndef FSPACE_FCLOUD_HPP_INCLUDED
#define FSPACE_FCLOUD_HPP_INCLUDED

namespace fspace // fcloud
{
    // NOTE: This function is marked inline only to say the compiler that don't
    // redefine this function (this is a header file), but the 'inline' property
    // of this function sure is down by the compiler, because of its size.
    inline fnode i_fcloud(fnode_id const& id)
    {
        static fnode_map l_cloud;

        if (id == "##!clean"_f) {
            l_cloud.clear();
            return futil::nil;
        }

        // NOTE: If 'id' doesn't found, a root with type '##!root' and
        // identifier 'id' is created.
        auto const& l_it = l_cloud.find(id);

        if (l_it == l_cloud.end()) {
            fnode l_new(id);
            l_cloud.insert(fnode_map::value_type(id, l_new));

            return l_new;
        }
        else
            return l_it->second;
    }

    inline fnode fcloud(fnode_id const& id)
    {
        // TODO: Check 'id' content.
        return i_fcloud(id);
    }

    template<typename T>
    inline T& fcloud();

    using argc_tp = int;
    using argv_tp = char**;

    template<>
    inline argc_tp& fcloud<argc_tp>()
    {
        static int s_argc;

        return s_argc;
    }

    template<>
    inline argv_tp& fcloud<argv_tp>()
    {
        static char** s_argv = nullptr;

        return s_argv;
    }

    inline void i_fcloud(int argc)
    {
        fcloud<argc_tp>() = argc;
    }

    inline void i_fcloud(char** argv)
    {
        fcloud<argv_tp>() = argv;
    }

    template<class App>
    inline void flaunch(int argc, char** argv)
    {
        //        try {
            App a;

            i_fcloud(argc);
            i_fcloud(argv);

            a();

            // NOTE: The cloud and the free memory stack (in the core file) are
            // both static local variables. The FMS is deleted before the cloud
            // is, and so, the destruction process of the cloud doesn't work
            // properly. For this reason, with the following trick the cloud is
            // clean before exiting the program.

            // REVIEW: Is FMS cleaned always before the cloud? How is exactly
            // the initialization process performed?

            // REVIEW: Internal private identifiers: ##! ?, #&# ? With whom
            // could be conflicts? Are they necessary?.

            // NOTE: If we don't need finally any private communication to clean
            // anything, the 'i_fcloud'/'fcloud' distinction isn't also
            // neccesary.

            // HACK: Careful! using_pool. If I erase the using_pool variable,
            // the cleaning line shoul be uncommented.
            using_pool = false;
            // i_fcloud("##!clean"_f);
            //}
        /*        catch(...) {
            std::cerr << "Some exception captured." << std::endl;
            }*/
    }

} // fcloud

#endif // FSPACE_FCLOUD_HPP_INCLUDED
