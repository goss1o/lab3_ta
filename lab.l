%{
#include <stdlib.h>
#include <string>
#include <vector>
    
#include "ast/ast.h"
#include "ast/expr.h"
#include "ast/var.h"
#include "ast/func.h"
#include "ast/conditional.h"

#include "own_system.h"

#define YYSTYPE val
struct val {
    int int_val; 
    bool bool_val;
    own_types::CELL_VALUE cell_val;
    std::string str_val;
    
    own_types::Types vtype;

    ast::statement *st;
    std::vector<ast::statement*> statements;
    std::vector<ast::expr*> expressions;
    
    ast::branch branch;
    std::vector<ast::branch> branches;
};
#include "lab.tab.h"
void yyerror(const char*);
unsigned int strno=1;
unsigned int strerrno=1;
%}
%%

\<|\/|\>|=  {return *yytext;}

INT     {return INT_TYPE;} 
BOOL    {return BOOL_TYPE;}
CELL    {return CELL_TYPE;}

name    {return A_NAME;} 
count   {return A_COUNT;} 
const   {return A_CONST;} 

PROGRAM {return PROGRAM; }
VARDECLARATION {return VARDECLARATION; }
VAR     {return VAR; }
TYPE    {return TYPE; }
DIMENTIONS {return DIMENTIONS; }
DIMENSION {return DIMENSION; }
DIM     {return DIM; }
INDEX   {return INDEX; }
VALUES  {return VALUES; }
VALUE   {return VALUE; }
 
ASSIGN  {return ASSIGN; }
TO      {return TO; }

ADD {return ADD; }
MUL {return MUL; }
SUB {return SUB; }
DIV {return DIV; }
NOT {return NOT; }
OR  {return OR; }
AND {return AND; }
MAX {return MAX; }
MIN {return MIN; }
EQ  {return EQ; }

WHILE  {return WHILE; }
CHECK  {return CHECK; }
DO  {return DO; }
CONDITION {return CONDITION;}
SWITCH {return SWITCH;}

FUNC  {return FUNC; }
CALL  {return CALL; }

RIGHT {return RIGHT; }
LEFT  {return LEFT; }
UP    {return UP; }  
DOWN    {return DOWN; }

SENDDRONS    {return SENDDRONS; }
GETDRONSCOUNT    {return GETDRONSCOUNT; }


[0-9]+ {
	sscanf(yytext, "%d", &yylval.int_val); return INT_VAL;
}
true {yylval.bool_val=true; return BOOL_VAL;}
false {yylval.bool_val=false; return BOOL_VAL;}

UNDEF {yylval.cell_val = own_types::CELL_VALUE::UNDEFINED ;return CELL_VAL;}
EMPTY {yylval.cell_val = own_types::CELL_VALUE::EMPTY ;return CELL_VAL;}
WALL {yylval.cell_val = own_types::CELL_VALUE::WALL ;return CELL_VAL;}
EXIT {yylval.cell_val = own_types::CELL_VALUE::EXIT ;return CELL_VAL;}

sys_print {return sys_print;}

[a-zA-Z_]([a-zA-Z0-9_])* {yylval.str_val = std::string(yytext); return STR_VAL;}

[\n\t\ ] {}
. {yyerror(yytext);}
%% 
int yywrap(void) {
	return 1; 
} 