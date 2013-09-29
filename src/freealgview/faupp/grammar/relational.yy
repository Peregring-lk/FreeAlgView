relational : arithmetical
           | relational '<' arithmetical
{
    $$ << fspace::bleacher()
       << "#!le"_t << ($1 << "first"_f) << ($3 << "second"_f);
}
           | relational OP_LEQ arithmetical
{
    $$ << fspace::bleacher()
       << "#!leq"_t << ($1 << "first"_f) << ($3 << "second"_f);
}
           | relational '>' arithmetical
{
    $$ << fspace::bleacher()
       << "#!ge"_t << ($1 << "first"_f) << ($3 << "second"_f);
}
           | relational OP_GEQ arithmetical
{
    $$ << fspace::bleacher()
       << "#!ge"_t << ($1 << "first"_f) << ($3 << "second"_f);
}
           | relational '=' arithmetical
{
    $$ << fspace::bleacher()
       << "#!eq"_t << ($1 << "first"_f) << ($3 << "second"_f);
}
           | relational OP_NEQ arithmetical
{
    $$ << fspace::bleacher()
       << "#!neq"_t << ($1 << "first"_f) << ($3 << "second"_f);
}
;
