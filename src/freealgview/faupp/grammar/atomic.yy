atomic : symbol
       | NUMBER_CONST
{
    $$ << fspace::bleacher() << "#!number"_t
       << ($1 << "value"_f);
}
       | symbol '(' args ')'
{
    $$ << fspace::bleacher()
       << "#!call"_t << ($1 << "target"_f) << ($3 << "args"_f);
}
       | '(' expr ')'
{
    $$ = $2;
}
;

args : expr
     | expr ',' args
{
    $$ << fspace::bleacher()
       << "#!args"_t << ($1 << "car"_f) << ($3 << "cdr"_f);
}
;

symbol : SYMBOL_NAME
{
    $$ << fspace::bleacher() << "#!symbol"_t
       << ($1 << "value"_f);
}
;

type : SYMBOL_NAME
{
    $$ << fspace::bleacher() << "#!type"_t
       << ($1 << "value"_f);
};
