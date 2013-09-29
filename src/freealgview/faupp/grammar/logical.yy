// Types
logical : relational
        | logical OP_AND relational
{
    $$ << fspace::bleacher()
       << "#!and"_t << ($1 << "first"_f) << ($3 << "second"_f);
}
        | logical OP_OR relational
{
    $$ << fspace::bleacher()
       << "#!or"_t << ($1 << "first"_f) << ($3 << "second"_f);
}
;
