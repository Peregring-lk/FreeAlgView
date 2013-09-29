expr : relational
     | expr OP_AND relational
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n
           << ~("#!access"_n
                << ("obj"_n << ~$1)
                << ("field"_n << ~("#!symbol"_n << ~"and"_n))))
       << ("args"_n << ~$3);
}
     | expr OP_OR relational
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n
           << ~("#!access"_n
                << ("obj"_n << ~$1)
                << ("field"_n << ~("#!symbol"_n << ~"or"_n))))
       << ("args"_n << ~$3);
}
;

relational : arithmetical
           | relational '<' arithmetical
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n
           << ~("#!access"_n
                << ("obj"_n << ~$1)
                << ("field"_n << ~("#!symbol"_n << ~"le"_n))))
       << ("args"_n << ~$3);
}
           | relational OP_LEQ arithmetical
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n
           << ~("#!access"_n
                << ("obj"_n << ~$1)
                << ("field"_n << ~("#!symbol"_n << ~"leq"_n))))
       << ("args"_n << ~$3);
}
           | relational '>' arithmetical
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n
           << ~("#!access"_n
                << ("obj"_n << ~$1)
                << ("field"_n << ~("#!symbol"_n << ~"ge"_n))))
       << ("args"_n << ~$3);
}
           | relational OP_GEQ arithmetical
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n
           << ~("#!access"_n
                << ("obj"_n << ~$1)
                << ("field"_n << ~("#!symbol"_n << ~"geq"_n))))
       << ("args"_n << ~$3);
}
           | relational '=' arithmetical
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n
           << ~("#!access"_n
                << ("obj"_n << ~$1)
                << ("field"_n << ~("#!symbol"_n << ~"eq"_n))))
       << ("args"_n << ~$3);
}
           | relational OP_NEQ arithmetical
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n
           << ~("#!access"_n
                << ("obj"_n << ~$1)
                << ("field"_n << ~("#!symbol"_n << ~"neq"_n))))
       << ("args"_n << ~$3);
}
;

arithmetical : multiplicative
             | arithmetical '+' multiplicative
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n
           << ~("#!access"_n
                << ("obj"_n << ~$1)
                << ("field"_n << ~("#!symbol"_n << ~"add"_n))))
       << ("args"_n << ~$3);
}
             | arithmetical '-' multiplicative
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n
           << ~("#!access"_n
                << ("obj"_n << ~$1)
                << ("field"_n << ~("#!symbol"_n << ~"sub"_n))))
       << ("args"_n << ~$3);
}
;

multiplicative : unary
               | multiplicative '*' unary
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n
           << ~("#!access"_n
                << ("obj"_n << ~$1)
                << ("field"_n << ~("#!symbol"_n << ~"mult"_n))))
       << ("args"_n << ~$3);
}
               | multiplicative '/' unary
{
    $$ << fspace::bleacher()
       << "#!call"_f
       << ("obj"_n
           << ~("#!access"_n
                << ("obj"_n << ~$1)
                << ("field"_n << ~("#!symbol"_n << ~"div"_n))))
       << ("args"_n << ~$3);
}
;

unary : atom
      | OP_NOT unary
{
    $$ << fspace::bleacher()
       << "#!access"_f
       << ("obj"_n << ~$2)
       << ("field"_n << ~("#!symbol"_n << ~"not"_n));
}
      | OP_REF unary
{
    $$ << fspace::bleacher()
       << "#!ref"_f
       << ("value"_n << ~$2);
}
      | '-' unary
{
    $$ << fspace::bleacher()
       << "#!access"_f
       << ("obj"_n << ~$2)
       << ("field"_n << ~("#!symbol"_n << ~"minus"_n));
}
      | '|' expr '|'
{
    $$ << fspace::bleacher()
       << "#!access"_f
       << ("obj"_n << ~$2)
       << ("field"_n << ~("#!symbol"_n << ~"abs"_n));
}
;
