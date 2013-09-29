unary : atomic
      | OP_NOT unary
{
    $$ << fspace::bleacher()
       << "#!not"_t << ($1 << "arg"_f);
}
      | '-' unary
{
    $$ << fspace::bleacher()
       << "#!minus"_t << ($1 << "arg"_f);
}
      | '|' expr '|'
{
    $$ << fspace::bleacher()
       << "#!abs"_t << ($2 << "arg"_f);
}
;
