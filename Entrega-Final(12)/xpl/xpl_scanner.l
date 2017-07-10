%option c++ yyclass="xpl_scanner" outfile="xpl_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
/* $Id: xpl_scanner.l,v 1.12 2017/05/21 20:15:16 ist181151 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "xpl_scanner.h"
#include "xpl_parser.tab.h"

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_STRING_ESPCHAR X_STRING_IGNORE X_COMMENT


IDENTIFIER 						[_a-zA-Z][0-9a-zA-Z_]*
SYMBOL							[-%\]{}()<>!*/~+&|[;?,:=]
SPACE      						[ \r\n\t]+

DIGIT							[0-9]
EXPONENT          				[eE][-+]?{DIGIT}+
REAL							{DIGIT}+{EXPONENT}|({DIGIT}+"."{DIGIT}*|{DIGIT}*"."{DIGIT}+){EXPONENT}?
HEXA             				[0-9a-fA-F]
B10_INT							[1-9]{DIGIT}*|0
B16_INT							0[xX]{HEXA}+

%%

"public"			   			{ return tPUBLIC;}
"use"				   			{ return tUSE;}
"procedure"			   			{ return tPROCEDURE;}

"while"                			{ return tWHILE;} 
"sweep"				   			{ return tSWEEP;}

"next"				            { return tNEXT;}
"stop"			      		    { return tSTOP;}
"return"			   			{ return tRETURN;}
"@"					   			{ return tREAD;} 
"null"				   			{ return tNULL;}

"if"                  			{ return tIF;} 
"elsif"                			{ return tELSEIF;} 
"else"                 			{ return tELSE;}

"int"				   			{ return tTYPEINT;}			   
"real"				   			{ return tTYPEREAL;}
"string"			   			{ return tTYPESTRING;}

">="                   			{ return tGE;} 
"<="                   			{ return tLE; }
"=="                   			{ return tEQ; }
"!="                   			{ return tNE;}
"!!"				   			{ return tPRINTNL;}
"!"                    			{ return tPRINT;}

"/*"                          	{ yy_push_state(X_COMMENT);}
<X_COMMENT>"/*"               	{ yy_push_state(X_COMMENT);}
<X_COMMENT>"*/"               	{ yy_pop_state();}
<X_COMMENT>.|\n               	{ ;}

"//".*$                       	{ ; /* ignore comments */}

{IDENTIFIER}	 	 			{ yylval.s = new std::string(yytext); return tIDENTIFIER;}
{SYMBOL}						{ return *yytext;}
{REAL}               		 	{ yylval.d = strtod(yytext, nullptr);		if(errno == ERANGE) yyerror("Overflow..."); return tDOUBLE;}
{B16_INT}			  		 	{ yylval.i = strtol(yytext, nullptr, 16);	if(errno == ERANGE) yyerror("Overflow..."); return tINTEGER;}
{B10_INT}               		{ yylval.i = strtol(yytext, nullptr, 10);	if(errno == ERANGE) yyerror("Overflow..."); return tINTEGER;}

\"                     		  	{ yy_push_state(X_STRING); yylval.s = new std::string("");}
<X_STRING,X_STRING_IGNORE>\"   	{ yy_pop_state(); 	return tSTRING;}
<X_STRING>\\		   		  	{ yy_push_state(X_STRING_ESPCHAR);}
<X_STRING>.|\n         		  	{ *yylval.s += yytext; }

<X_STRING_ESPCHAR>0		     	{ yy_pop_state(); 	 yy_pop_state(); yy_push_state(X_STRING_IGNORE);}
<X_STRING_ESPCHAR>00		    { yy_pop_state();  	 yy_pop_state(); yy_push_state(X_STRING_IGNORE);}
<X_STRING_ESPCHAR>r		        { *yylval.s += "\r"; yy_pop_state();}
<X_STRING_ESPCHAR>\"         	{ *yylval.s += "\""; yy_pop_state();}
<X_STRING_ESPCHAR>t		        { *yylval.s += "\t"; yy_pop_state();}
<X_STRING_ESPCHAR>\\			{ *yylval.s += "\\"; yy_pop_state();}
<X_STRING_ESPCHAR>n				{ *yylval.s += "\n"; yy_pop_state();}
<X_STRING_ESPCHAR>{HEXA}{HEXA}? { *yylval.s += std::string(1,strtol(yytext, nullptr, 16)); yy_pop_state();}
<X_STRING_ESPCHAR>.				yyerror("Unknown character in string!");

<X_STRING_IGNORE>\\\"        	{ ; /* ignore */}
<X_STRING_IGNORE>\\\\        	{ ; /* ignore */}
<X_STRING_IGNORE>.|\n 	     	{ ; /* ignore */}

{SPACE}		                  	{ ; /* ignore whitespace */}

.                      		 	{ yyerror("Unknown character");}

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int xpl_scanner::yywrap() { return 1; }
