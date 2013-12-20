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

%class-name Faupp

// Own files.
%class-header freealgview/faupp/faupp.hpp
%implementation-header freealgview/faupp/faupp.hpp

// Phantoms
%baseclass-header .phantom-dir/faupp-parser-phantom.hpp
%parsefun-source .phantom-dir/faupp-parser-phantom.cpp

%baseclass-preinclude fcloud/fspace.hpp

%stype fspace::fnode

%token SYMBOL TYPE_MARK // FUN_MARK
%token OP_ASSIGN OP_LASSIGN OP_OASSIGN OP_BASE_ASSIGN
%token OP_EVAL OP_REF OP_BASE OP_FORCE
%token OP_AND OP_OR OP_LEQ OP_GEQ OP_NEQ OP_NOT
%token IF WHILE RETURN DO

%nonassoc else_priority
%nonassoc ELSE ELIF

%start translation_unit

%%

translation_unit : seq
{
    auto machine("machine"_n);

    machine << ("ctx_table"_n << ~"arrack"_n)
            << ("pc"_n << ~"stack"_n)
            << ("xtack"_n << ~"stack"_n);

    /*
    // Creating code.
    // Predefined objects (1) and (2)
    // (1) int <- {}; Constructor of int.
    auto int_assign("#!assign"_n);

    int_assign << ("lvalue"_n << ~("#!symbol"_n << ~"int"_n))
               << ("rvalue"_n << ~"#!object"_n);

    // (2) [+-]?[0-9]+"."[0-9]+" Constructor of constant numbers.
    auto number_assign("#!assign"_n);
    auto number_assign_lvalue("#!symbol"_n);
    auto number_assign_context("#!closure"_n);

    number_assign_lvalue << ~"[+-]?[0-9]+\".\"[0-9]+"_n;
    number_assign_context << ("mark"_n << ~("#!mark"_n << ~"#"_n))
                          << ("body"_n << ~("#!base_mark"_n << ~"int"_n));

    number_assign << ("lvalue"_n << ~number_assign_lvalue)
                  << ("rvalue"_n <<
                      ~("#!object"_n
                        << ("context"_n << ~number_assign_context)));

    // Creating code.
    auto pushed_node("#!list"_n);

    pushed_node << ("car"_n << ~number_assign)
                << ("cdr"_n << ~$1);

    auto code("#!list"_n);

    code << ("car"_n << ~int_assign)
         << ("cdr"_n << ~pushed_node);

    machine << ("code"_n << ~code);
    */

    machine << ("code"_n << ~$1);

    owner() << ~machine;
}
;

// Sentences
seq : stmnt ';' seq
{
    $$ << fspace::bleacher()
       << "#!list"_f
       << ("car"_n << ~$1)
       << ("cdr"_n << ~$3);
}
    | ';' seq
{
    $$ = $2;
}
    | stmnt ';'
;

stmnt : assign_stmnt
      | return_stmnt
      | OP_BASE expr
{
    $$ << fspace::bleacher()
       << "#!base_call"_f
       << ~$2;
}
      | '>' param_mark
{
    $$ = $2;

    $$ << "#!base_mark"_f;
}
      | expr
{
    $$ << fspace::bleacher()
       << "#!expr_stmnt"_f
       << ("value"_n << ~$1);
}
;

/*
      | fun_ret ":=" symbol fun_params body
;

fun_ret : ASSIGN_MARK
        |
;

fun_params : '(' param_list ')'
           | '(' ')'
           |
;
*/

assign_stmnt : type_mark expr op_assign expr
{
    $$ = $3;

    fspace::fnode l_properties("properties"_f);

    l_properties << "lvalue"_n;

    auto l_mark_properties = $1["properties"_f];
    auto l_force = l_mark_properties["forced"_f];

    if (l_force)
        l_properties << l_force;

    if (l_mark_properties.leaf())
        l_mark_properties >> futil::nil;

    if (!$1.leaf())
        $$ << ("mark"_n << ~$1);

    $2 << l_properties;

    $$ << ("lvalue"_n << ~$2)
       << ("rvalue"_n << ~$4);
}
             | type_mark expr
{
    fspace::fnode l_properties("properties"_f);

    l_properties << "lvalue"_n;

    auto l_mark_properties = $1["properties"_f];
    auto l_force = l_mark_properties["forced"_f];

    if (l_force)
        l_properties << l_force;

    if (l_mark_properties.leaf())
        l_mark_properties >> futil::nil;

    $$ << fspace::bleacher()
       << "#!assign"_f;

    if (!$1.leaf())
       $$ << ("mark"_n << ~$1);

    $2 << l_properties;

    $$ << ("lvalue"_n << ~$2);
}
             | expr op_assign expr
{
    $$ = $2;

    $1 << ("properties"_n << "lvalue"_n);

    $$ << ("lvalue"_n << ~$1)
       << ("rvalue"_n << ~$3);
}
;

op_assign : OP_ASSIGN
{
    $$ << fspace::bleacher()
       << "#!assign"_f;
}
          | OP_LASSIGN
{
    $$ << fspace::bleacher()
       << "#!lassign"_f;
}
          | OP_OASSIGN
{
    $$ << fspace::bleacher()
       << "#!oassign"_f;
}
          | OP_BASE_ASSIGN
{
    $$ << fspace::bleacher()
       << "#!base_assign"_f;
}
;

type_mark : type_name '&'
{
    $$ << fspace::bleacher()
       << "#!mark"_f
       << ("value"_n << ~$1)
       << ("properties"_n << "ref"_n);
}
          | type_name
{
    $$ << fspace::bleacher()
       << "#!mark"_f
       << ("value"_n << ~$1);
}
          | '&'
{
    $$ << fspace::bleacher()
       << "#!mark"_f
       << ("properties"_n << "ref"_n);
}
          | OP_FORCE
{
    $$ << fspace::bleacher()
       << "#!mark"_f
       << ("properties"_n << "forced"_n);
}
          | OP_FORCE type_mark
{
    $$ = $2;

    auto l_props = $$["properties"_f];

    if (!l_props) {
        l_props = "properties"_n;

        $$ << l_props;
    }

    l_props << "forced"_n;
}
;

type_name : TYPE_MARK
          | '@'
;

param_mark : param_name '&'
{
    $$ << fspace::bleacher()
       << "#!mark"_f
       << ("value"_n << ~$1)
       << ("properties"_n << "ref"_n);
}
           | param_name
{
    $$ << fspace::bleacher()
       << "#!mark"_f
       << ("value"_n << ~$1);
}
           | '&'
{
    $$ << fspace::bleacher()
       << "#!mark"_f
       << ("properties"_n << "ref"_n);
}
;

param_name : SYMBOL
           | '@'
;

return_stmnt : RETURN expr
{
    $$ << fspace::bleacher()
       << "#!return"_f
       << ("expr"_n << ~$2);
}
             | RETURN
{
    $$ << fspace::bleacher()
       << "#!return"_f;
}
;

arg_list : expr ',' arg_list
{
    $$ << fspace::bleacher()
       << "#!list"_f
       << ("car"_n << ~$1)
       << ("cdr"_n << ~$3);
}
         | expr
;

param_list : param ',' param_list
{
    $$ << fspace::bleacher()
       << "#!list"_f
       << ("car"_n << ~$1)
       << ("cdr"_n << ~$3);
}
           | param
;

param : param_mark symbol
{
    $$ << fspace::bleacher()
       << "#!param"_f
       << ("mark"_n << ~$1)
       << ("symbol"_n << ~$2);
}
      | param_mark
{
    $$ << fspace::bleacher()
       << "#!param"_f
       << ("mark"_n << ~$1);
}
;

// Expressions
%include freealgview/faupp/grammar/exprs.yy

atom : obj
     | '(' expr ')'
{
    $$ = $2;
}
     | atom '(' arg_list ')'
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n << ~$1)
       << ("args"_n << ~$3);
}
     | atom '(' ')'
     | atom '.' symbol
{
    $$ << fspace::bleacher()
       << "#!access"_f
       << ("obj"_n << ~$1)
       << ("field"_n << ~$3);
}
     | symbol
;

symbol : SYMBOL
{
    $$ << fspace::bleacher()
       << "#!symbol"_f
       << ("value"_n << ~$1);
}
;

// Objects
obj : ctx overloads
{
    $$ << fspace::bleacher()
       << "#!object"_f;

    if ($1)
        $$ << ("context"_n << ~$1);

    if ($2)
       $$ << ("overloads"_n << ~$2);
}
    | overloads
{
    $$ << fspace::bleacher()
       << "#!object"_f;

    if ($1)
       $$ << ("overloads"_n << ~$1);
}
    | ctx
{
    $$ << fspace::bleacher()
       << "#!object"_f;

    if ($1)
       $$ << ("context"_n << ~$1);
}
;

overloads : overload overloads
{
    if ($1)
        if ($2)
            $$ << fspace::bleacher()
               << "#!list"_f
               << ("car"_n << ~$1)
               << ("cdr"_n << ~$2);
        else
            $$ = $1;
    else if ($2)
        $$ = $2;
    else
        $$ = futil::nil;
}
          | overload
;

overload : signature body
{
    $$ = futil::nil;

    if ($1 or $2) {
        if ($1)
            $$ = $1;
        else
            $$ << fspace::bleacher();

        $$ << "#!closure"_f;

        if ($2)
            $$ << ("body"_n << ~$2);
    }
}
         | body
{
    if ($1)
        $$ << fspace::bleacher()
           << "#!closure"_f
           << ("body"_n << ~$1);
    else
        $$ = futil::nil;
}
;

signature : return_mark '(' param_list ')'
{
    $$ << fspace::bleacher()
       << ("params"_n << ~$3);

    if ($1)
       $$ << ("mark"_n << ~$1);
}
          | return_mark '(' ')'
{
    $$ = futil::nil;

    if ($1)
        $$ << fspace::bleacher()
           << ("mark"_n << ~$1);
}
          | return_mark
{
    $$ = futil::nil;

    if ($1)
        $$ << fspace::bleacher()
           << ("mark"_n << ~$1);
}
;

return_mark : ':' param_mark
{
    $$ = $2;
}
            | ':'
{
    $$ = futil::nil;
}
;

body : IF '(' expr ')' body else_tail
{
    $$ = futil::nil;

    if ($5 or $6) {
        $$ << fspace::bleacher()
           << "#!if"_f
           << ("clause"_n << ~$3);

        if ($5)
            $$ << ("body"_n << ~$5);

        if ($6)
           $$ << ("else"_n << ~$6);
    }
}
     | IF '(' expr ')' body             %prec else_priority
{
    $$ = futil::nil;

    if ($5)
        $$ << fspace::bleacher()
           << "#!if"_f
           << ("clause"_n << ~$3)
           << ("body"_n << ~$5);
}
     | WHILE '(' expr ')' body
{
    $$ = futil::nil;

    if ($5)
        $$ << fspace::bleacher()
           << "#!while"_f
           << ("clause"_n << ~$3)
           << ("body"_n << ~$5);
}
     | DO body WHILE '(' expr ')'
{
    $$ = futil::nil;

    if ($2)
        $$ << fspace::bleacher()
           << "#!dowhile"_f
           << ("clause"_n << ~$5)
           << ("body"_n << ~$2);
}
     | seq_body
;

else_tail : ELIF '(' expr ')' body else_tail
{
    $$ = futil::nil;

    if ($5 or $6) {
        $$ << fspace::bleacher()
           << "#!if"_f
           << ("clause"_n << ~$3);

        if ($5)
            $$ << ("body"_n << ~$5);

        if ($6)
           $$ << ("else"_n << ~$6);
    }
}
          | ELIF '(' expr ')' body
{
    $$ = futil::nil;

    if ($5)
        $$ << fspace::bleacher()
           << "#!if"_f
           << ("clause"_n << ~$3)
           << ("body"_n << ~$5);
}
          | ELSE body
{
    $$ = $2;
}
;

seq_body : '{' '}'
{
    $$ = futil::nil;
}
         | '?'
{
    $$ = futil::nil;
}
         | '{' seq '}'
{
    $$ = $2;
}
         | OP_EVAL stmnt ';' // REVIEW: Ugly!!
{
    $$ = $2;
}
;

// Context
ctx : '#' param_mark ctx_body
{
    $$ << fspace::bleacher()
       << "#!closure"_f
       << ("mark"_n << ~$2);

    if ($2)
        $$ << ("body"_n << ~$3);
}
    | '#' ctx_body
{
    $$ << fspace::bleacher()
       << "#!closure"_f
       << ("mark"_n << ~("#!mark"_n << ~$1));

    if ($2)
        $$ << ("body"_n << ~$2);
}
    | ctx_body
{
    $$ = futil::nil;

    if ($1)
        $$ << fspace::bleacher()
           << "#!closure"_f
           << ("body"_n << ~$1);
}
    | '#' param_mark
      // TODO: To be implemented.
;

ctx_body : '[' ']'
{
    $$ = futil::nil;
}
         | '[' seq ']'
{
    $$ = $2;
}
;
