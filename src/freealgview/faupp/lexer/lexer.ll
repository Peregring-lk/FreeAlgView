/*
  Copyright (C) 2011 Aarón Bueno Villares <abv150ci@gmail.com>

  This file is part of FreeAlgView.

  FreeAlgView is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FreeAlgView is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FreeAlgView.  If not, see <http://www.gnu.org/licenses/>.
*/

%class-name = "FauppLexer"

// Own files (not overwritten).
%class-header = "freealgview/faupp/lexer/lexer.hpp"
%implementation-header = "freealgview/faupp/lexer/lexer.hpp"

// Phantoms
%baseclass-header = ".phantom-dir/faupp-lexer-phantom.hpp"
%lex-source = ".phantom-dir/faupp-lexer-phantom.cpp"

DIGIT [0-9]
SYMBOL_NAME [a-zA-Z][a-zA-Z_0-9]*

// Mini-scanner for filtering C comments.
%x commentC

%%

"//"[^\n]*                 ;

"/*"                       begin(StartCondition__::commentC);
<commentC>{
    .|\n                   ;

    "*/"                   begin(StartCondition__::INITIAL);
}

[ \t\n]+  	               ;

if                         return FauppToken::IF;
else                       return FauppToken::ELSE;
do                         return FauppToken::DO;
while                      return FauppToken::WHILE;
return                     return FauppToken::RETURN;

[#.:;,@?(){}[\]<>=+*/|]|'-' return matched().c_str()[0];

"!"                        return FauppToken::OP_NOT;
"<="                       return FauppToken::OP_LEQ;
">="                       return FauppToken::OP_GEQ;
"¬="                       return FauppToken::OP_NEQ;

"&&"                       return FauppToken::OP_AND;
"||"                       return FauppToken::OP_OR;

"<-"                       return FauppToken::OP_ASSIGN;
"&<-"                      return FauppToken::OP_LASSIGN;
"<<-"                      return FauppToken::OP_OASSIGN; // overload assignment
"<:-"                      return FauppToken::OP_BASE_ASSIGN;

"->"                       return FauppToken::OP_EVAL;
":>"                       return FauppToken::OP_BASE;
"=>"                       return FauppToken::OP_FORCE;

[+-]?{DIGIT}+              return FauppToken::SYMBOL;
[+-]?{DIGIT}+"."{DIGIT}+   return FauppToken::SYMBOL;

// See use of token TYPE_MARK in faupp.cpp (Faupp::lex()).
{SYMBOL_NAME}              return FauppToken::SYMBOL;

.                          {
                             std::cerr << "Unrecognized character \""
                                       << matched()
                                       << "\"" << std::endl;

                              exit(EXIT_FAILURE);
                           }
