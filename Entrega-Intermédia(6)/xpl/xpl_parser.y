%{
// $Id: xpl_parser.y,v 1.14 2017/04/21 15:10:37 ist181151 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                     i;	/* integer value */
  double                  d;  /* double value */
  std::string             *s;	/* symbol name or string literal */
  cdk::basic_node         *node;	/* node pointer */
  cdk::sequence_node      *sequence;
  cdk::expression_node    *expression; /* expression nodes */
  cdk::lvalue_node        *lvalue;
  basic_type              *type;
  xpl::block_node         *block;
};

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING
%token tNULL tPROCEDURE tWHILE tSWEEP tIF tTYPEINT tTYPESTRING tTYPEREAL tPRINTNL tPRINT tSTOP tNEXT tPUBLIC tUSE tRETURN tREAD


%nonassoc ")"
%nonassoc tELSE tELSEIF

%right '='
%left '|'
%left '&'
%nonassoc '~'
%left tEQ tNE
%left '<' '>' tLE tGE
%left '+' '-'
%left '*' '/' '%'

%nonassoc tUNARY
%nonassoc "["

%type<node> decl  var  instr param ifs func cicles fvar
%type<block> block
%type<sequence> instrs exprs fvars seqdecl params fparams
%type<expression> literal expr funcall
%type<s> string
%type<type> type
%type<lvalue> leftval

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file   : seqdecl                                                      { compiler->ast($1);}
       |                                                              { compiler->ast(new cdk::sequence_node(LINE));}
       ;


seqdecl: decl                                                         { $$ = new cdk::sequence_node(LINE, $1);}
       | seqdecl decl                                                 { $$ = new cdk::sequence_node(LINE, $2, $1);}
       ;


decl   : var ";"                                                      { $$ = $1;}
       | func                                                         { $$ = $1;}
       ;


var    : type tIDENTIFIER                                             { $$ = new xpl::variable_decl_node(LINE,$1, false,false, $2, nullptr);}
       | type tIDENTIFIER "=" expr                                    { $$ = new xpl::variable_decl_node(LINE,$1, false,false, $2, $4);}
       | tPUBLIC type tIDENTIFIER                                     { $$ = new xpl::variable_decl_node(LINE, $2, true, false, $3, nullptr);}
       | tPUBLIC type tIDENTIFIER "=" expr                            { $$ = new xpl::variable_decl_node(LINE, $2, true, false, $3, $5);}
       | tUSE type tIDENTIFIER                                        { $$ = new xpl::variable_decl_node(LINE, $2, false, true, $3, nullptr);}
       ;


fvar   : param                                                        { $$ = $1;}
       | type tIDENTIFIER "=" expr                                    { $$ = new xpl::variable_decl_node(LINE,$1, false,false, $2, $4);}
       ;


func   : tPUBLIC type tIDENTIFIER "(" fparams ")"                     { $$ = new xpl::function_decl_node(LINE, true, false, $2, $3, $5);}
       | tPUBLIC tPROCEDURE tIDENTIFIER "(" fparams ")"               { $$ = new xpl::function_decl_node(LINE, true, false, new basic_type(0, basic_type::TYPE_VOID), $3, $5);}
       | tPROCEDURE tIDENTIFIER "(" fparams ")"                       { $$ = new xpl::function_decl_node(LINE, false, false, new basic_type(0, basic_type::TYPE_VOID), $2, $4);}
       | tUSE type tIDENTIFIER "(" fparams ")"                        { $$ = new xpl::function_decl_node(LINE, false, true, $2, $3, $5);}
       | tUSE tPROCEDURE tIDENTIFIER "(" fparams ")"                  { $$ = new xpl::function_decl_node(LINE, false, true, new basic_type(0, basic_type::TYPE_VOID), $3, $5);}
       | type tIDENTIFIER "(" fparams ")"                             { $$ = new xpl::function_decl_node(LINE, false, false, $1, $2, $4);}
       | tPUBLIC type tIDENTIFIER "(" fparams ")" block               { $$ = new xpl::function_def_node(LINE, true, $2, $3, $5, nullptr, $7);}
       | tPUBLIC type tIDENTIFIER "(" fparams ")"  "=" literal block  { $$ = new xpl::function_def_node(LINE, true, $2, $3, $5, $8, $9);}
       | tPUBLIC tPROCEDURE tIDENTIFIER "(" fparams ")" block         { $$ = new xpl::function_def_node(LINE, true, new basic_type(0, basic_type::TYPE_VOID), $3, $5, nullptr, $7);}
       | tPROCEDURE tIDENTIFIER "(" fparams ")" block                 { $$ = new xpl::function_def_node(LINE, false, new basic_type(0, basic_type::TYPE_VOID), $2, $4, nullptr, $6);}
       | type tIDENTIFIER "(" fparams ")" block                       { $$ = new xpl::function_def_node(LINE, false, $1, $2, $4, nullptr, $6);}
       | type tIDENTIFIER "(" fparams ")" "=" literal block           { $$ = new xpl::function_def_node(LINE, false, $1, $2, $4, $7, $8);}
       ;


funcall: tIDENTIFIER '('  ')'                                         { $$ = new xpl::function_call_node(LINE, $1,  new cdk::sequence_node(LINE));}
       | tIDENTIFIER '(' exprs ')'                                    { $$ = new xpl::function_call_node(LINE, $1, $3);}
       ;


fvars  : fvar ";"                                                     { $$ = new cdk::sequence_node(LINE, $1);}
       | fvars fvar ";"                                               { $$ = new cdk::sequence_node(LINE, $2, $1);} 
       ;


fparams: params                                                       { $$ = $1; }
       |                                                              { $$ = new cdk::sequence_node(LINE);}
       ;


params : param                                                        { $$ = new cdk::sequence_node(LINE, $1);}
       | params ',' param                                             { $$ = new cdk::sequence_node(LINE, $3, $1);}
       ;


param  : type tIDENTIFIER                                             { $$ = new xpl::variable_decl_node(LINE, $1, false, false, $2, nullptr);}
       ;


type   : tTYPEINT                                                     { $$ = new basic_type(4, basic_type::TYPE_INT);}
       | tTYPEREAL                                                    { $$ = new basic_type(8, basic_type::TYPE_DOUBLE);}
       | tTYPESTRING                                                  { $$ = new basic_type(4, basic_type::TYPE_STRING);}
       | '[' type ']'                                                 { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $2;}
       ;


block  : "{" "}"                                                      { $$ = new xpl::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE));}
       | "{" fvars "}"                                                { $$ = new xpl::block_node(LINE, $2, new cdk::sequence_node(LINE));}
       | "{" instrs "}"                                               { $$ = new xpl::block_node(LINE, new cdk::sequence_node(LINE), $2);}
       | "{" fvars instrs "}"                                         { $$ = new xpl::block_node(LINE, $2, $3);}
       ;


instrs : instr                                                        { $$ = new cdk::sequence_node(LINE, $1);}
       | instrs instr                                                 { $$ = new cdk::sequence_node(LINE, $2, $1);}
       ;


instr  : expr ';'                                                     { $$ = new xpl::evaluation_node(LINE, $1);}
       | expr tPRINT                                                  { $$ = new xpl::print_node(LINE, $1, false);}
       | expr tPRINTNL                                                { $$ = new xpl::print_node(LINE, $1, true);}
       | tNEXT                                                        { $$ = new xpl::next_node(LINE);}
       | tSTOP                                                        { $$ = new xpl::stop_node(LINE);}
       | tRETURN                                                      { $$ = new xpl::return_node(LINE);}
       | tIF ifs                                                      { $$ = $2;}
       | cicles                                                       { $$ = $1;}
       | block                                                        { $$ = $1;}
       ;


ifs    : '(' expr ')' instr                                           { $$ = new xpl::if_node(LINE, $2, $4);}
       | '(' expr ')' instr tELSE instr                               { $$ = new xpl::if_else_node(LINE, $2, $4, $6);}
       | '(' expr ')' instr tELSEIF ifs                               { $$ = new xpl::if_else_node(LINE, $2, $4, $6);}
       ;


cicles : tSWEEP "-" "(" leftval ":" expr ":" expr ")" instr           { $$ = new xpl::sweep_down_node(LINE, $4, $6, $8, new cdk::integer_node(LINE, 1), $10);}
       | tSWEEP "-" "(" leftval ":" expr ":" expr ":" expr ")" instr  { $$ = new xpl::sweep_down_node(LINE, $4, $6, $8, $10, $12);}
       | tSWEEP "+" "(" leftval ":" expr ":" expr ")" instr           { $$ = new xpl::sweep_up_node(LINE, $4, $6, $8, new cdk::integer_node(LINE, 1), $10);}
       | tSWEEP "+" "(" leftval ":" expr ":" expr ":" expr ")" instr  { $$ = new xpl::sweep_up_node(LINE, $4, $6, $8, $10, $12);}
       | tWHILE '(' expr ')' instr                                    { $$ = new xpl::while_node(LINE, $3, $5);}
       ;


expr   : tREAD                                                        { $$ = new xpl::read_node(LINE);}
       | literal                                                      { $$ = $1;}
       | funcall                                                      { $$ = $1;}
       | '-' expr %prec tUNARY                                        { $$ = new cdk::neg_node(LINE, $2);}
       | '+' expr %prec tUNARY                                        { $$ = new xpl::identity_node(LINE, $2);}
       | '~' expr                                                     { $$ = new cdk::not_node(LINE, $2);}
       | expr '+' expr	                                              { $$ = new cdk::add_node(LINE, $1, $3);}
       | expr '-' expr	                                              { $$ = new cdk::sub_node(LINE, $1, $3);}
       | expr '*' expr	                                              { $$ = new cdk::mul_node(LINE, $1, $3);}
       | expr '/' expr	                                              { $$ = new cdk::div_node(LINE, $1, $3);}
       | expr '%' expr	                                              { $$ = new cdk::mod_node(LINE, $1, $3);}
       | expr '<' expr	                                              { $$ = new cdk::lt_node(LINE, $1, $3);}
       | expr '>' expr	                                              { $$ = new cdk::gt_node(LINE, $1, $3);}
       | expr tGE expr	                                              { $$ = new cdk::ge_node(LINE, $1, $3);}
       | expr tLE expr                                                { $$ = new cdk::le_node(LINE, $1, $3);}
       | expr tNE expr	                                              { $$ = new cdk::ne_node(LINE, $1, $3);}
       | expr tEQ expr	                                              { $$ = new cdk::eq_node(LINE, $1, $3);}
       | expr '|' expr                                                { $$ = new cdk::or_node(LINE, $1, $3);}
       | expr '&' expr                                                { $$ = new cdk::and_node(LINE, $1, $3);}
       | leftval                                                      { $$ = new cdk::rvalue_node(LINE, $1);}
       | leftval '?'                                                  { $$ = new xpl::memory_address_node(LINE, $1);}
       | leftval '=' expr                                             { $$ = new cdk::assignment_node(LINE, $1, $3);}
       | '[' expr ']'                                                 { $$ = new xpl::memory_alloc_node(LINE, $2);}
       | '(' expr ')'                                                 { $$ = $2;}
       ;


exprs  : expr                                                         { $$ = new cdk::sequence_node(LINE, $1);}
       | expr ',' exprs                                               { $$ = new cdk::sequence_node(LINE, $1, $3);}
       ;


leftval: expr '[' expr ']'                                            { $$ = new xpl::indexing_node(LINE, $1, $3);}
       | tIDENTIFIER                                                  { $$ = new cdk::identifier_node(LINE, $1); delete $1;}
       ;


literal: tINTEGER                                                     { $$ = new cdk::integer_node(LINE, $1);}
       | tDOUBLE                                                      { $$ = new cdk::double_node(LINE, $1);}
       | string                                                       { $$ = new cdk::string_node(LINE, $1);}
       | tNULL                                                        { $$ = new xpl::null_node(LINE);}
       ;


string : string tSTRING                                               { $$ = new std::string(*$1 + *$2); delete $1; delete $2;}
       | tSTRING                                                      { $$ = $1;}
       ;     


%%
