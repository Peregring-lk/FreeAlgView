if (NOT (DEFINED faupp_l_h_gen))
  set(faupp_l_h_gen ${phantom_dir}/faupp-lexer-phantom.hpp)
endif()

#unset(phantom_content)

if (NOT (DEFINED faupp_l_s_gen))
  set(faupp_l_s_gen ${phantom_dir}/faupp-lexer-phantom.cpp)
endif()

#unset(phantom_content)

file(READ ${faupp_l_s_gen} phantom_content)
string(REPLACE "lexer.hpp" "../freealgview/faupp/lexer/lexer.hpp" phantom_content "${phantom_content}")
file(WRITE ${faupp_l_s_gen} "${phantom_content}")

#unset(phantom_content)

set(SEARCH_EXPR  "    FauppLexerBase(FauppLexerBase const &other)             = delete;")
set(REPLACE_EXPR "    FauppLexerBase(FauppLexerBase const &other) : FauppLexerBase(std::cin, std::cout) {}\n    FauppLexerBase(FauppLexerBase&& other)                  = default;")

file(READ ${faupp_l_h_gen} phantom_content)
string(REPLACE "${SEARCH_EXPR}" "${REPLACE_EXPR}" phantom_content "${phantom_content}")
file(WRITE ${faupp_l_h_gen} "${phantom_content}")