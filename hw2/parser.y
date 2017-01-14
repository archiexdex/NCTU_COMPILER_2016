%{
#include <stdio.h>
#include <stdlib.h>

extern int linenum;             /* declared in lex.l */
extern FILE *yyin;              /* declared by lex */
extern char *yytext;            /* declared by lex */
extern char buf[256];           /* declared in lex.l */
%}

/*           (            )        ,       ;          [          ]         {         }       */
%token '(' ')' ',' ';' '[' ']' '{' '}'
/*%token LPARENTHESES RPARENTHESES COMMA  SEMICOLON LSBRACKETS RSBRACKETS LBRACKETS RBRACKETS */  

/*        +          -             *           / %      =      < <= == >= > !=  && || ! */
%token '+' '-' '*' '/' '%' '=' '<' LE EQ GE '>' NQ AND OR '!'
/* %token ADDITION SUBSTRACTION MULTIPLICATION DEVISION ASSIGNMENT RELATIONAL     LOGICAL */

/* number */
%token INTEGER REALNUMBER
/* keyword */
%token READ BOOLEAN WHILE DO IF ELSE TRUE FALSE FOR CONST INT PRINT BOOL VOID FLOAT DOUBLE STRING CONTINUE BREAK RETURN   
/* string content */
%token STRCONTENT
/* identification */
%token ID

%left OR
%left AND
%left '!'
%left '<' LE EQ GE '>' NQ
%left '+' '-'
%left '*' '/' '%'

%%

program : program_body
	;

program_body : declaration_list
			 ;

declaration_list : nonEmptyDeclarationList 
				 | /* */
				 ;

nonEmptyDeclarationList : nonEmptyDeclarationList decl
						| decl
						;

decl : func_decl 
	 | var_decl 
	 | const_decl
	 ;

func_decl   : func_set ';'
			| func_set compound
			; 

func_set : type ID '(' argument ')'
		 ;

var_decl : type variables ';'
		 ;

type : VOID
	 | data_type
	 ;

data_type   : INT
			| DOUBLE
			| FLOAT
			| STRING
			| BOOL
			;

variable : ID
		 | ID index
		 ;

index   : '[' expressions ']'
		| index '[' expressions ']'
		;

argument : nonEmptyArgument
		 | /* */
		 ;

nonEmptyArgument : nonEmptyArgument ',' data_type variable
				 | data_type variable
				 ;

var_list : variable
		 | init_var
		 ;

variables : nonEmptyVariables
		  | /* */
		  ;

nonEmptyVariables : nonEmptyVariables ',' var_list
				  | var_list
				  ;

init_var : ID '=' expressions
		 | ID index '=' '{' nonEmptyExpressions '}'
		 ;

initVars : nonEmptyInitVars
		  | /* */
		  ;

nonEmptyInitVars : nonEmptyInitVars ',' init_var
				 | init_var
				 ;

const_decl  : CONST data_type initVars ';'
			;

expression  : func_invoke
			| STRCONTENT
			| INTEGER
			| REALNUMBER
			| TRUE
			| FALSE
			| variable
			;

nonEmptyExpressions : nonEmptyExpressions ',' expression
					| expression
					;

expressions : '-' expressions %prec '*'
			| expressions '+' expressions
			| expressions '-' expressions
			| expressions '*' expressions
			| expressions '/' expressions
			| expressions '%' expressions
			| expressions '<' expressions
			| expressions LE  expressions
			| expressions EQ  expressions
			| expressions GE  expressions
			| expressions '>' expressions
			| expressions NQ  expressions
			| expressions AND expressions
			| expressions OR  expressions
			| '!' expressions 
			| '(' expressions ')'
			| expression
			;

state_list  : compound
			| condition
			| simple
			| while
			| for
			| jump
			| function_call
			;

list  : state_list 
	  | var_decl 
	  | const_decl
	  ;

lists : nonEmptyLists
	  | 
	  ;

nonEmptyLists : nonEmptyLists list
			  | list
			  ;

compound : '{' lists '}' 
		 ; 

simple  : variable '=' expressions ';'
		| PRINT variable ';'
		| PRINT expressions ';'
		| READ variable ';'
		;

condition : IF '(' expressions ')' compound
		  | IF '(' expressions ')' ELSE compound
		  ;

while : WHILE '(' expressions ')' compound
	  | DO compound WHILE '(' expressions ')' ';'
	  ;

for : FOR '(' initexpr ';' ctrexpr ';' increxpr ')' compound
	;

assignment  : ID '=' expressions
			| ID index '=' expressions
			;

assignments : nonEmptyAssignments
			| /* */
			;

nonEmptyAssignments : nonEmptyAssignments ',' assignment
					| assignment
					;


initexpr : data_type assignments
		 | assignments
		 | func_invoke
		 | /**/
		 ;

ctrexpr : expressions
		| 
		;

increxpr : assignments
		 | func_invoke
		 | /**/
		 ;

jump : RETURN expressions ';'
	 | BREAK ';'
	 | CONTINUE ';'
	 ;

expressionList  : nonEmptyExpressionList
				| /* */
				;

nonEmptyExpressionList  : nonEmptyExpressionList ',' expressions
						| expressions
						;

func_invoke : ID '(' expressionList ')'
			;

function_call : func_invoke ';'
			  ;

%%

int yyerror( char *msg )
{
  fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
	fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
	fprintf( stderr, "|\n" );
	fprintf( stderr, "| Unmatched token: %s\n", yytext );
  fprintf( stderr, "|--------------------------------------------------------------------------\n" );
  exit(-1);
}

int  main( int argc, char **argv )
{
	if( argc != 2 ) {
		fprintf(  stdout,  "Usage:  ./parser  [filename]\n"  );
		exit(0);
	}

	FILE *fp = fopen( argv[1], "r" );

	if( fp == NULL )  {
		fprintf( stdout, "Open  file  error\n" );
		exit(-1);
	}

	yyin = fp;
	yyparse();

	fprintf( stdout, "\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	fprintf( stdout, "|  There is no syntactic error!  |\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	exit(0);
}
