stmnt : block
      | return
      | if
      | while
      | dowhile
      | expr
;

// return
return : RETURN expr
{
    $$ << fspace::bleacher()
       << "#!return"_t << ($1 << "expr"_f);
};

// sequence
block : '(' NEWLINE script ')'
{
    // $$ << fspace::bleacher()
    //    << "#!block"_t << ($3 << "script"_f);
    $$ = $3;
};

// selection
if : IF '(' expr ')' stmnt
{
    $$ << fspace::bleacher()
       << "#!if"_t << ($3 << "condition"_f) << ($5 << "true_stmnt"_f);
}
          | IF '(' expr ')' stmnt ELSE stmnt
{
    $$ << fspace::bleacher()
       << "#!if"_t << ($3 << "condition"_f) << ($5 << "true_stmnt"_f)
       << ($7 << "false_stmnt"_f);
};

// loops
while : WHILE '(' expr ')' stmnt
{
    $$ << fspace::bleacher()
       << "#!while"_t << ($3 << "condition"_f) << ($5 << "loop_stmnt"_f);
};

dowhile : DO stmnt WHILE '(' expr ')'
{
    $$ << fspace::bleacher()
      << "#!dowhile"_t << ($2 << "loop_stmnt"_f) << ($5 << "condition"_f);
};
