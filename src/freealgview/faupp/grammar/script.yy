// Script
translation_unit : script
{
    owner() << ($1 << "#!code"_f);

    // owner().father() << fspace::fnode("#!author"_f, "Aarón Bueno Villares"_t)
    //                  << fspace::fnode("#!special_mention"_f,
    //                                   "Sarah Fee Schirin Kornberger"_t);
}
;

script : instruction NEWLINE script
{
    $$ << fspace::bleacher()
       << "#!script"_t << ($1 << "car"_f) << ($3 << "cdr"_f);
}
       | instruction NEWLINE
       | NEWLINE script
{
    $$ = $2;
}
;

instruction : def
            | stmnt
;

// Definitions
def : vardef
    // fundef:
    | type IMAGE_TYPE symbol '(' params ')' NEWLINE block
{
    $$ << fspace::bleacher()
       << "#!fundef"_t
       << ($1 << "ret"_f)
       << ($3 << "name"_f)
       << ($5 << "params"_f)
       << ($8 << "body"_f);
}
;

vardef : type symbol
{
    $$ << fspace::bleacher()
       << "#!vardef"_t << ($1 << "type"_f) << ($2 << "name"_f);
}
;

// Parameters
params :
{
    $$ << fspace::bleacher() << "#!nil"_t;
}
       | paramlist
;

paramlist : vardef
          | vardef ',' paramlist
{
    $$ << fspace::bleacher()
       << "#!params"_t << ($1 << "car"_f) << ($3 << "cdr"_f);
}
;
