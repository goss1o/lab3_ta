%{
    #include <stdlib.h>
    #include <iostream>
    #include <fstream>
    #include <string>
    #include <vector>

    #include "ast/ast.h"
    #include "ast/expr.h"
    #include "ast/var.h"
    #include "ast/func.h"
    #include "ast/conditional.h"
    #include "ast/robot_operators.h"

    #include "own_system.h"

    #include "translator/cpptranslator.h"
    #include "ast/system.h"

    int yylex(void); 
    void yyerror(const char*);
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
    extern FILE *yyin;
    ast::statementGrp *PROG;
%}

%token INT_VAL BOOL_VAL STR_VAL CELL_VAL
%token INT_TYPE BOOL_TYPE CELL_TYPE
%token A_CONST A_NAME A_COUNT

%token PROGRAM FUNC CALL
%token VARDECLARATION VAR TYPE
%token DIMENTIONS DIMENSION DIM INDEX
%token VALUES VALUE
%token ASSIGN TO
%token ADD MUL SUB DIV NOT OR AND MAX MIN EQ
%token WHILE SWITCH CHECK CONDITION DO  

%token RIGHT LEFT UP DOWN
%token SENDDRONS GETDRONSCOUNT

%token sys_print
%%
program:
    '<' PROGRAM '>' elements '<''/' PROGRAM '>' {PROG = new ast::statementGrp($4.statements)}
    ;
elements:
    elements vardeclaration {$$.statements = $1.statements; $$.statements.push_back($2.st);}
    | elements funcdeclaring {$$.statements = $1.statements; $$.statements.push_back($2.st);}
    | {$$.statements = {}}
    ;
//-----------------------------------------------------------//
vardeclaration:
    '<' VARDECLARATION '>' vars '<''/' VARDECLARATION '>' {$$.st = new ast::statementGrp($4.statements);}
    ;
vars:
    vars var {$$.statements = $1.statements; $$.statements.push_back($2.st);}
    | var {$$.statements = {$1.st};}
    ;
var:
    '<' VAR '=' STR_VAL '>' type_tag '<''/' VAR '>' {$$.st = new ast::varDecl($4.str_val, $6.vtype);}
    | '<' VAR '=' STR_VAL '>' type_tag values_tag'<''/' VAR '>' {$$.st = new ast::varDecl($4.str_val, $6.vtype, dynamic_cast<ast::values*>($7.st));}

    | '<' VAR '=' STR_VAL '>' type_tag dimentions_tag '<''/' VAR '>' {$$.st = new ast::arrayDecl($4.str_val, $6.vtype, $7.expressions);}
    | '<' VAR '=' STR_VAL '>' type_tag dimentions_tag values_tag'<''/' VAR '>' {$$.st = new ast::arrayDecl($4.str_val, $6.vtype, $7.expressions, dynamic_cast<ast::values*>($8.st));}
    ;
//-----------------------
type_tag:
    '<' TYPE '>' INT_TYPE '<''/' TYPE '>' {$$.vtype = own_types::Types::INT;}
    | '<' TYPE '>' BOOL_TYPE '<''/' TYPE '>' {$$.vtype = own_types::Types::BOOL;}
    | '<' TYPE '>' CELL_TYPE '<''/' TYPE '>' {$$.vtype = own_types::Types::CELL;}
    ;
//-----------------------
dimentions_tag:
    //! @todo Проверка размерностей
    '<' DIMENTIONS A_COUNT '=' INT_VAL '>' dimentions '<''/' DIMENTIONS '>' {$$.expressions = $7.expressions;}
    ;
dimentions:
    dimentions dimension {$$.expressions = $1.expressions; $$.expressions.push_back(dynamic_cast<ast::expr*>($2.st));}
    | dimension {$$.expressions = {dynamic_cast<ast::expr*>($1.st)};}
    ;
dimension:
    '<' DIMENSION '>' expression '<''/' DIMENSION '>' {$$.st = $4.st;/*$$.st = new ast::literal(own_types::value($4.int_val));*/}
    ;
//-----------------------
values_tag:
    '<' VALUES '>' expressions '<''/' VALUES '>' {$$.st = new ast::values($4.expressions);}
    ;
value:
    '<' VALUE '>' literal '<''/' VALUE '>' {$$.st = $4.st;}
    | '<' VALUE '>' expression '<''/' VALUE '>' {$$.st = $4.st;}
    ;
literal:
    INT_VAL {$$.st = new ast::literal(own_types::value($1.int_val));}
    | BOOL_VAL {$$.st = new ast::literal(own_types::value($1.bool_val));}
    | CELL_VAL {$$.st = new ast::literal(own_types::value($1.cell_val));}
    ;
//-----------------------------------------------------------//
var_get:
    '<' VAR A_NAME '=' STR_VAL '>''<''/' VAR '>' {$$.st = new ast::varGet($5.str_val);}
    | '<' VAR A_NAME '=' STR_VAL '>' dim_info '<''/' VAR '>' {$$.st = new ast::arrGet($5.str_val, $7.expressions);}
    ;
dim_info:
    '<' DIM '>' indexes '<''/' DIM '>' {$$.expressions = $4.expressions;}
    ;
indexes:
    indexes index {$$.expressions = $1.expressions; $$.expressions.push_back(dynamic_cast<ast::expr*>($2.st));}
    | index {$$.expressions = {dynamic_cast<ast::expr*>($1.st)};}
    ;
index:
    '<' INDEX '>' expression '<''/' INDEX '>' {$$.st = $4.st;}
    ;
//-----------------------------------------------------------//
expressions:
    expressions expression {$$.expressions = $1.expressions; $$.expressions.push_back(dynamic_cast<ast::expr*>($2.st));}
    | expression {$$.expressions = {dynamic_cast<ast::expr*>($1.st)};}
    ;
expression:
    var_get {$$.st = $1.st;}
    | value {$$.st = $1.st;}
    | '<' ADD '>' expressions '<''/' ADD '>' {$$.st = new ast::addExpr($4.expressions);}
    | '<' MUL '>' expressions '<''/' MUL '>' {$$.st = new ast::mulExpr($4.expressions);}
    | '<' OR '>' expressions '<''/' OR '>'  {$$.st = new ast::orExpr($4.expressions);}
    | '<' AND '>' expressions '<''/' AND '>' {$$.st = new ast::andExpr($4.expressions);}
    | '<' MIN '>' expressions '<''/' MIN '>' {$$.st = new ast::minExpr($4.expressions);}
    | '<' MAX '>' expressions '<''/' MAX '>' {$$.st = new ast::maxExpr($4.expressions);}
    | '<' EQ '>' expressions '<''/' EQ '>' {$$.st = new ast::eqExpr($4.expressions);}

    | '<' NOT '>' expression '<''/' NOT '>' {$$.st = new ast::notExpr(dynamic_cast<ast::expr*>($4.st));}

    | '<' SUB '>'  expression expression '<''/' SUB '>' {$$.st = new ast::subExpr(dynamic_cast<ast::expr*>($4.st), dynamic_cast<ast::expr*>($5.st));}
    | '<' DIV '>' expression expression '<''/' DIV '>' {$$.st = new ast::divExpr(dynamic_cast<ast::expr*>($4.st), dynamic_cast<ast::expr*>($5.st));}
    
    | '<' SENDDRONS '>' expression '<''/' SENDDRONS '>' {$$.st = new ast::Senddrons(dynamic_cast<ast::expr*>($4.st));}
    ;
//-----------------------------------------------------------//
assigning:
    '<' ASSIGN '>' expression  '<' TO '>' many_var_get '<''/' TO '>' '<''/' ASSIGN '>' {$$.st = new ast::assigning(dynamic_cast<ast::expr*>($4.st),$8.statements);}
    ;
many_var_get:
    many_var_get var_get {$$.statements = $1.statements; $$.statements.push_back($2.st);}
    | var_get {$$.statements = {$1.st};}
    ;
//-----------------------------------------------------------//
cycle:
    '<'WHILE'>' '<'CHECK'>' expression '<''/'CHECK'>' '<'DO'>' commands '<''/' WHILE '>' {$$.st = new ast::cycle(dynamic_cast<ast::expr*>($7.st),new ast::statementGrp($15.statements));}
    ;
branching:
    '<'SWITCH'>'branches'<''/'SWITCH'>' {$$.st = new ast::switcher($4.branches);}
    ;
branches:
    branches branch {$$.branches = $1.branches; $$.branches.push_back($2.branch);}
    | branch {$$.branches = {$1.branch};}
    ;
branch:
    '<'CONDITION'>' '<'CHECK'>'expression'<''/'CHECK'>' '<'DO'>'commands'<''/'DO'>' '<''/'CONDITION'>' {$$.branch = {dynamic_cast<ast::expr*>($7.st), new ast::statementGrp($15.statements)}}
    ;
//-----------------------------------------------------------//
funcdeclaring:
    '<' FUNC A_NAME '=' STR_VAL '>' commands '<''/' FUNC '>' {$$.st = new ast::funcDecl($5.str_val, new ast::statementGrp($7.statements));}
    ;
funccall:
    '<'CALL'>'STR_VAL'<''/'CALL'>' {$$.st = new ast::funcCall($4.str_val);}
    ;
commands:
    commands command {$$.statements = $1.statements; $$.statements.push_back($2.st);}
    | command {$$.statements = {$1.st};}
    ;
command:
    vardeclaration {$$.st = $1.st;}
    | assigning {$$.st = $1.st;}
    | expression {$$.st = $1.st;}

    | cycle {$$.st = $1.st;}
    | branching {$$.st = $1.st;}

    | funccall

    | robot_cmd {$$.st = $1.st;}

    | sys_cmd {$$.st = $1.st;}
    ;
//-----------------------------------------------------------//
robot_cmd:
    robot_moving {$$.st = $1.st}
    | '<' GETDRONSCOUNT '>' STR_VAL '<''/' GETDRONSCOUNT '>' {$$.st = new ast::Getdronscount($4.str_val);}
    ;
robot_moving:
    '<' RIGHT '>' expression '<''/' RIGHT '>' {$$.st = new ast::Right(dynamic_cast<ast::expr*>($4.st));}
    | '<' LEFT '>' expression '<''/' LEFT '>' {$$.st = new ast::Left(dynamic_cast<ast::expr*>($4.st));}
    | '<' UP '>' expression '<''/' UP '>' {$$.st = new ast::Up(dynamic_cast<ast::expr*>($4.st));}
    | '<' DOWN '>' expression '<''/' DOWN '>' {$$.st = new ast::Down(dynamic_cast<ast::expr*>($4.st));}
    ;
//-----------------------------------------------------------//
sys_cmd:
    '<' sys_print '>' var_get '<''/'sys_print'>' {$$.st = new sys::print(dynamic_cast<ast::varGet *>($4.st));}
    ;
//-----------------------------------------------------------//
%%

void yyerror(const char* msg) {
    printf("%s\n", msg);
}

int main(int argc, char **argv) {
    if (argc <= 3)
        return 1;
    yyin = fopen(argv[1],"r");
    int err = yyparse();
    fclose(yyin);
    try {
        if (!err) {
            std::cout << "SCANNED" <<std::endl;

            std::ofstream out;          // поток для записи
            out.open(argv[2]); 
            CPPtranslator tr(out, argv[3]);
            PROG->translate(tr);
            out.close();

            std::cout << "TRANSLATED" <<std::endl;
            std::string cmd(" g++ -Wno-return-type -std=c++20 -g robot/labyrinth.cpp ");
            cmd += argv[2];
            cmd += " -o compiled";
            system(cmd.c_str());

            std::cout << "COMPILED" <<std::endl;
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() <<std::endl;
    }
    delete PROG;
    return 0;
}
