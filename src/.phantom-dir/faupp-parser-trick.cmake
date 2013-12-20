if (NOT (DEFINED faupp_p_h_gen))
  set(faupp_p_h_gen ${phantom_dir}/faupp-parser-phantom.hpp)
endif()

if (NOT (DEFINED faupp_p_s_gen))
  set(faupp_p_s_gen ${phantom_dir}/faupp-parser-phantom.cpp)
endif()

#unset(phantom_content)

file(READ ${faupp_p_h_gen} phantom_content)
string(REPLACE "fcloud/" "../fcloud/" phantom_content "${phantom_content}")
file(WRITE ${faupp_p_h_gen} "${phantom_content}")

file(READ ${faupp_p_s_gen} phantom_content)
string(REPLACE "freealgview/" "../freealgview/" phantom_content "${phantom_content}")
file(WRITE ${faupp_p_s_gen} "${phantom_content}")

#unset(phantom_content)

file(READ ${faupp_p_h_gen} phantom_content)
string(REPLACE "#define Faupp FauppBase" "" phantom_content "${phantom_content}")
file(WRITE ${faupp_p_h_gen} "${phantom_content}")
