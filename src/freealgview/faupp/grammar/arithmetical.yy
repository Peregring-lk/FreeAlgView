arithmetical : multiplicative
             | arithmetical '+' multiplicative
{
    $$ << fspace::bleacher()
       << "#!add"_t << ($1 << "first"_f) << ($3 << "second"_f);
}
             | arithmetical '-' multiplicative
{
    $$ << fspace::bleacher()
       << "#!sub"_t << ($1 << "first"_f) << ($3 << "second"_f);
}
;

// ...
multiplicative : unary
               | multiplicative '*' unary
{
    $$ << fspace::bleacher()
       << "#!mult"_t << ($1 << "first"_f) << ($3 << "second"_f);
}
               | multiplicative '/' unary
{
    $$ << fspace::bleacher()
       << "#!div"_t << ($1 << "first"_f) << ($3 << "second"_f);
}
;
