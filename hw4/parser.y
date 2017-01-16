%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"
#include "symtab.h"
#include "semcheck.h"

extern int linenum;
extern FILE	*yyin;
extern char	*yytext;
extern char buf[256];
extern int Opt_Symbol;		/* declared in lex.l */
//extern "C" int yylex(void);
extern FILE *java;

int scope = 0;
char fileName[256];
struct SymTable *symbolTable;
__BOOLEAN paramError;
struct PType *funcReturn;
__BOOLEAN semError = __FALSE;
int inloop = 0;

const char types[] = { 'V', 'I', 'F', 'D' ,'Z', 'X'};
char ConstString[100];
int ConstBool = 0;
int ConstInt = 0;
float ConstFloat = 0.0;
double ConstDouble = 0.0;
int top = -1;

void log(int a){
	printf(">> %d!!\n",a);
}

//>>>>>>>>>>>>>>>>>>>> load
void gen_load(struct expr_sem *expr ) {
	char tmp = types[expr->pType->type];
	struct SymNode *node = lookupSymbol( symbolTable, expr->varRef->id, scope, __FALSE );
	if ( node->scope ) {
		
		if ( tmp == 'I' ){
			fprintf(java, "iload %d\n", node->addr);
		}
		else {
			fprintf(java, "fload %d\n", node->addr);	
		}
	}
	else {
		fprintf(java, "getstatic %s/%s %c\n", fileName, expr->varRef->id, tmp);
	}
}

//>>>>>>>>>>>>>>>>>>>> store
void gen_store(struct expr_sem *expr) {
	char tmp = types[expr->pType->type];
	struct SymNode *node = lookupSymbol( symbolTable, expr->varRef->id, scope, __FALSE );
	if (node->scope) {
		
		if ( tmp == 'I' ){
			fprintf(java, "istore %d\n", node->addr);
		}
		else {
			fprintf(java, "fstore %d\n", node->addr);	
		}
	}
	else {
		fprintf(java, "putstatic %s/%s %c\n", fileName, expr->varRef->id, tmp);
	}
	
}
//>>>>>>>>>>>>>>>>>>>> const value
void gen_constVal( struct expr_sem *expr){
	char tmp = types[expr->pType->type];
	if ( tmp == 'Z' ) {
		if ( ConstBool ) {
			fprintf(java, "iconst_1\n");
		}
		else {
			fprintf(java, "iconst_0\n");
		}
	}
	else if ( tmp == 'I' ){
		fprintf(java, "ldc %d\n", ConstInt);
	}
	else if ( tmp == 'F' ){
		fprintf(java, "ldc %f\n", ConstFloat);
	}
	else if ( tmp == 'D' ){
		fprintf(java, "ldc %d\n", ConstDouble);
	}
	else if ( tmp == 'X' ){
		fprintf(java, "ldc \"%s\"\n", ConstString);
	}
}

//>>>>>>>>>>>>>>>>>>>> print & read
void gen_print1(){
	fprintf(java,"getstatic java/lang/System/out Ljava/io/PrintStream;\n");
	
}
void gen_print2( struct expr_sem *expr ){
	char tmp = types[expr->pType->type];
	if( tmp == 'X' ){
		fprintf(java,"invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V\n");
	}
	else {
		fprintf(java,"invokevirtual java/io/PrintStream/print(%c)V\n",tmp);
	}
}
void gen_read( struct expr_sem *expr , int scope,  int top) {
	fprintf(java,"getstatic %s/_sc Ljava/util/Scanner;\n", fileName);
	char tmp = types[expr->pType->type];
	
	if ( tmp == 'I') {
		fprintf(java, "invokevirtual java/util/Scanner/nextInt()I\n");
		if ( scope ) fprintf(java, "istore %d\n", top);
		else {
			fprintf(java, "putstatic %s/%s %c\n", fileName, expr->varRef->id, tmp);
		}
	}
	else if( tmp == 'Z' ) {
		fprintf(java, "invokevirtual java/util/Scanner/nextBoolean()Z\n");	
		if ( scope ) fprintf(java, "istore %d\n", top);
		else {
			fprintf(java, "putstatic %s/%s %c\n", fileName, expr->varRef->id, tmp);
		}
	}
	else if( tmp == 'F' ) {
		fprintf(java, "invokevirtual java/util/Scanner/nextFloat()F\n");	
		if ( scope ) fprintf(java, "fstore %d\n", top);
		else {
			fprintf(java, "putstatic %s/%s %c\n", fileName, expr->varRef->id, tmp);
		}
	}
	else if( tmp == 'D' ) {
		fprintf(java, "invokevirtual java/util/Scanner/nextDouble()D\n");	
		
		if ( scope ) fprintf(java, "fstore %d\n", top);
		else {
			fprintf(java, "putstatic %s/%s %c\n", fileName, expr->varRef->id, tmp);
		}
	}

}
//>>>>>>>>>>>>>>>>>>>> expression
void gen_expre(struct expr_sem *a, struct expr_sem *b, int mode) {
	
	int ta = a->pType->type, tb = b->pType->type;
	// bool
	if ( ta == 4 && tb == 4 ){
		if( mode == 6 ){
			fprintf(java, "iand\n");
		}
		else if( mode == 7 ){
			fprintf(java, "ior\n");
		}
		else if( mode == 8 ) {
			fprintf(java, "ixor\n");
		}
	}
	// int float double 
	else if( 1 <= ta <= 3 && 1 <= tb <= 4 ){
		
		int m = ta, flg = 0;
		if ( tb > ta ) m = tb, flg = 1;
		if ( ta > tb ) m = ta, flg = 1;
		if ( flg ) {
			fprintf(java, "i2f\n");
		}
		if ( m == 1 ){
			if ( mode == 0 ) {
				fprintf(java, "iadd\n");
			}
			else if (mode == 1) {
				fprintf(java, "isub\n");
			}
			else if (mode == 2) {
				fprintf(java, "imul\n");
			}
			else if (mode == 3) {
				fprintf(java, "idiv\n");
			}
			else if(mode == 4) {
				fprintf(java, "irem\n");
			}
			else if(mode == 5) {
				fprintf(java, "ineg\n");
			}
		}
		else {
			if ( mode == 0 ) {
				fprintf(java, "fadd\n");
			}
			else if (mode == 1) {
				fprintf(java, "fsub\n");
			}
			else if (mode == 2) {
				fprintf(java, "fmul\n");
			}
			else if (mode == 3) {
				fprintf(java, "fdiv\n");
			}
			else if(mode == 5) {
				fprintf(java, "fneg\n");
			}
		}
	}


}

//>>>>>>>>>>>>>>>>>>>> function 
void gen_func1(char* id) {
	fprintf(java,".method public static %s",id);
}
void gen_funcArg(struct param_sem* params){
	char param[100];
	int base = 0;
	struct param_sem *p = params;
	for( p = params ; p != NULL ; p = p->next ){
		param[base++] = types[p->pType->type];
	}
	param[base] = '\0';
	fprintf(java, "(%s)", param);
}
void gen_funcMain() {
	fprintf(java,"([Ljava/lang/String;)");
}
void gen_funct(){
	fprintf(java,".limit stack 128\n");
	fprintf(java,".limit locals 128\n");
}
void gen_functMain(){
	fprintf(java,"new java/util/Scanner\n");
	fprintf(java,"dup\n");
	fprintf(java,"getstatic java/lang/System/in Ljava/io/InputStream;\n");
	fprintf(java,"invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V\n");
	fprintf(java, "putstatic %s/_sc Ljava/util/Scanner;\n",fileName);
}
void gen_funcRet(struct PType *ret){
	fprintf(java,"%c\n",types[ret->type]);
}
void gen_funcEnd(){
	fprintf(java,".end method\n");
}
//>>>>>>>>>>>>>>>>>>>> invoke function 
void gen_Invoke(char *id, struct expr_sem *exprs){
	struct SymNode *node = lookupSymbol( symbolTable, id, scope, __FALSE );
	char tmp = types[node->type->type];
	char param[100];
	int base = 0;
	struct expr_sem *p = NULL;
	for( p = exprs ; p != NULL ; p = p->next ){
		param[base++] = types[p->pType->type];
	}
	param[base] = '\0';
	fprintf(java, "invokestatic %s/%s(%s)%c\n", fileName, id, param, tmp);
}


%}

%union {
	int intVal;
	float floatVal;	
	char *lexeme;
	struct idNode_sem *id;
	struct ConstAttr *constVal;
	struct PType *ptype;
	struct param_sem *par;
	struct expr_sem *exprs;
	struct expr_sem_node *exprNode;
	struct constParam *constNode;
	struct varDeclParam* varDeclNode;
};

%token	LE_OP NE_OP GE_OP EQ_OP AND_OP OR_OP
%token	READ BOOLEAN WHILE DO IF ELSE TRUE FALSE FOR INT PRINT BOOL VOID FLOAT DOUBLE STRING CONTINUE BREAK RETURN CONST
%token	L_PAREN R_PAREN COMMA SEMICOLON ML_BRACE MR_BRACE L_BRACE R_BRACE ADD_OP SUB_OP MUL_OP DIV_OP MOD_OP ASSIGN_OP LT_OP GT_OP NOT_OP

%token <lexeme>ID
%token <intVal>INT_CONST 
%token <floatVal>FLOAT_CONST
%token <floatVal>SCIENTIFIC
%token <lexeme>STR_CONST

%type<ptype> scalar_type dim
%type<par> array_decl parameter_list
%type<constVal> literal_const
%type<constNode> const_list 
%type<exprs> variable_reference logical_expression logical_term logical_factor relation_expression arithmetic_expression term factor logical_expression_list literal_list initial_array
%type<intVal> relation_operator add_op mul_op dimension
%type<varDeclNode> identifier_list


%start program
%%

program :		decl_list 
			    funct_def
				decl_and_def_list 
				{
					if(Opt_Symbol == 1)
					printSymTable( symbolTable, scope );	
				}
		;

decl_list : decl_list var_decl
		  | decl_list const_decl
		  | decl_list funct_decl
		  |
		  ;


decl_and_def_list : decl_and_def_list var_decl
				  | decl_and_def_list const_decl
				  | decl_and_def_list funct_decl
				  | decl_and_def_list funct_def
				  | 
				  ;

		  
funct_def : scalar_type ID L_PAREN R_PAREN 
			{
				funcReturn = $1; 
				struct SymNode *node;
				node = findFuncDeclaration( symbolTable, $2 );
				
				top = -1;
				if( node != 0 ){
					verifyFuncDeclaration( symbolTable, 0, $1, node );
				}
				else{
					insertFuncIntoSymTable( symbolTable, $2, 0, $1, scope, __TRUE );
				}
				gen_func1($2);
				if ( !strcmp("main",$2) ){
					gen_funcMain();
				}
				else {
					gen_funcArg(NULL);
				}
				gen_funcRet($1);
				gen_funct();
				if ( !strcmp("main",$2) ) {
					gen_functMain();
				}
			}
			compound_statement { funcReturn = 0; gen_funcEnd();}	
		  | scalar_type ID L_PAREN parameter_list R_PAREN  
			{				
				funcReturn = $1;
				
				paramError = checkFuncParam( $4 );
				int *ptr = NULL;
				if( paramError == __TRUE ){
					fprintf( stdout, "########## Error at Line#%d: param(s) with several fault!! ##########\n", linenum );
					semError = __TRUE;
				}
				// check and insert function into symbol table
				else{
					struct SymNode *node;
					node = findFuncDeclaration( symbolTable, $2 );

					top = -1;
					ptr = &top;
					if( node != 0 ){
						if(verifyFuncDeclaration( symbolTable, $4, $1, node ) == __TRUE){	
							insertParamIntoSymTable( symbolTable, $4, scope+1 , ptr);
							
						}				
					}
					else{
						insertParamIntoSymTable( symbolTable, $4, scope+1 , ptr);
						insertFuncIntoSymTable( symbolTable, $2, $4, $1, scope, __TRUE );
						
					}
					gen_func1($2);
					if ( !strcmp("main",$2) ){
						gen_funcMain();
					}
					else {
						gen_funcArg($4);
					}
					gen_funcRet($1);
					gen_funct();
					if ( !strcmp("main",$2) ) {
						gen_functMain();
					}
					log(top);
				}
			} 	
			compound_statement { funcReturn = 0; gen_funcEnd(); }
		  | VOID ID L_PAREN R_PAREN 
			{
				funcReturn = createPType(VOID_t); 
				struct SymNode *node;
				node = findFuncDeclaration( symbolTable, $2 );

				top = -1;
				if( node != 0 ){
					verifyFuncDeclaration( symbolTable, 0, createPType( VOID_t ), node );					
				}
				else{
					insertFuncIntoSymTable( symbolTable, $2, 0, createPType( VOID_t ), scope, __TRUE );	
					
				}
				gen_func1($2);
				if ( !strcmp("main",$2) ){
					gen_funcMain();
				}
				else {
					gen_funcArg(NULL);
				}
				gen_funcRet(funcReturn);
				gen_funct();
				if ( !strcmp("main",$2) ) {
					gen_functMain();
				}
			}
			compound_statement { funcReturn = 0; gen_funcEnd(); }	
		  | VOID ID L_PAREN parameter_list R_PAREN
			{									
				funcReturn = createPType(VOID_t);
				
				paramError = checkFuncParam( $4 );
				if( paramError == __TRUE ){
					fprintf( stdout, "########## Error at Line#%d: param(s) with several fault!! ##########\n", linenum );
					semError = __TRUE;
				}
				// check and insert function into symbol table
				else{
					struct SymNode *node;
					node = findFuncDeclaration( symbolTable, $2 );

					int *ptr = NULL;
					top = -1;
					ptr = &top;
					if( node != 0 ){
						if(verifyFuncDeclaration( symbolTable, $4, createPType( VOID_t ), node ) == __TRUE){	
							insertParamIntoSymTable( symbolTable, $4, scope+1 , ptr);
						}
					}
					else{
						insertParamIntoSymTable( symbolTable, $4, scope+1 , ptr);
						insertFuncIntoSymTable( symbolTable, $2, $4, createPType( VOID_t ), scope, __TRUE );
					}
					
				}
				gen_func1($2);
				if ( !strcmp("main",$2) ){
					gen_funcMain();
				}
				else {
					gen_funcArg($4);
				}
				gen_funcRet(funcReturn);
				gen_funct();
				if ( !strcmp("main",$2) ) {
					gen_functMain();
				}
				log(top);
			} 
			compound_statement { funcReturn = 0; gen_funcEnd(); }		  
		  ;

funct_decl : scalar_type ID L_PAREN R_PAREN SEMICOLON
			{
				insertFuncIntoSymTable( symbolTable, $2, 0, $1, scope, __FALSE );	
			}
		   | scalar_type ID L_PAREN parameter_list R_PAREN SEMICOLON
		    {
				paramError = checkFuncParam( $4 );
				if( paramError == __TRUE ){
					fprintf( stdout, "########## Error at Line#%d: param(s) with several fault!! ##########\n", linenum );
					semError = __TRUE;
				}
				else {
					insertFuncIntoSymTable( symbolTable, $2, $4, $1, scope, __FALSE );
				}
			}
		   | VOID ID L_PAREN R_PAREN SEMICOLON
			{				
				insertFuncIntoSymTable( symbolTable, $2, 0, createPType( VOID_t ), scope, __FALSE );
			}
		   | VOID ID L_PAREN parameter_list R_PAREN SEMICOLON
			{
				paramError = checkFuncParam( $4 );
				if( paramError == __TRUE ){
					fprintf( stdout, "########## Error at Line#%d: param(s) with several fault!! ##########\n", linenum );
					semError = __TRUE;	
				}
				else {
					insertFuncIntoSymTable( symbolTable, $2, $4, createPType( VOID_t ), scope, __FALSE );
				}
			}
		   ;

parameter_list : parameter_list COMMA scalar_type ID
			   {
				struct param_sem *ptr;
				ptr = createParam( createIdList( $4 ), $3 );
				param_sem_addParam( $1, ptr );
				$$ = $1;
			   }
			   | parameter_list COMMA scalar_type array_decl
			   {
				$4->pType->type= $3->type;
				param_sem_addParam( $1, $4 );
				$$ = $1;
			   }
			   | scalar_type array_decl 
			   { 
				$2->pType->type = $1->type;  
				$$ = $2;
			   }
			   | scalar_type ID { $$ = createParam( createIdList( $2 ), $1 ); }
			   ;

var_decl : scalar_type identifier_list SEMICOLON
			{
				struct varDeclParam *ptr;
				struct SymNode *newNode;
				for( ptr=$2 ; ptr!=0 ; ptr=(ptr->next) ) {						
					if( verifyRedeclaration( symbolTable, ptr->para->idlist->value, scope ) == __FALSE ) { }
					else {
						if( verifyVarInitValue( $1, ptr, symbolTable, scope ) ==  __TRUE ){	
							
							
							if ( scope == 0 ) {
								newNode = createVarNode( ptr->para->idlist->value, scope, ptr->para->pType ,top);
								insertTab( symbolTable, newNode );
								fprintf(java, ".field public static %s %c\n", ptr->para->idlist->value, types[$1->type] );
							}
							else {
								newNode = createVarNode( ptr->para->idlist->value, scope, ptr->para->pType ,++top);
								insertTab( symbolTable, newNode );
							}
							log(top);
						}
					}
				}
			}
			;

identifier_list : identifier_list COMMA ID
				{					
					struct param_sem *ptr;	
					struct varDeclParam *vptr;				
					ptr = createParam( createIdList( $3 ), createPType( VOID_t ) );
					vptr = createVarDeclParam( ptr, 0 );	
					addVarDeclParam( $1, vptr );
					$$ = $1; 					
				}
                | identifier_list COMMA ID ASSIGN_OP logical_expression
				{
					struct param_sem *ptr;	
					struct varDeclParam *vptr;				
					ptr = createParam( createIdList( $3 ), createPType( VOID_t ) );
					vptr = createVarDeclParam( ptr, $5 );
					vptr->isArray = __TRUE;
					vptr->isInit = __TRUE;	
					addVarDeclParam( $1, vptr );	
					$$ = $1;
					
				}
                | identifier_list COMMA array_decl ASSIGN_OP initial_array
				{
					struct varDeclParam *ptr;
					ptr = createVarDeclParam( $3, $5 );
					ptr->isArray = __TRUE;
					ptr->isInit = __TRUE;
					addVarDeclParam( $1, ptr );
					$$ = $1;	
				}
                | identifier_list COMMA array_decl
				{
					struct varDeclParam *ptr;
					ptr = createVarDeclParam( $3, 0 );
					ptr->isArray = __TRUE;
					addVarDeclParam( $1, ptr );
					$$ = $1;
				}
                | array_decl ASSIGN_OP initial_array
				{	
					$$ = createVarDeclParam( $1 , $3 );
					$$->isArray = __TRUE;
					$$->isInit = __TRUE;	
				}
                | array_decl 
				{ 
					$$ = createVarDeclParam( $1 , 0 ); 
					$$->isArray = __TRUE;
				}
                | ID ASSIGN_OP logical_expression
				{
					struct param_sem *ptr;					
					ptr = createParam( createIdList( $1 ), createPType( VOID_t ) );
					$$ = createVarDeclParam( ptr, $3 );		
					$$->isInit = __TRUE;
				}
                | ID 
				{
					struct param_sem *ptr;					
					ptr = createParam( createIdList( $1 ), createPType( VOID_t ) );
					$$ = createVarDeclParam( ptr, 0 );				
				}
                ;
		 
initial_array : L_BRACE literal_list R_BRACE { $$ = $2; }
			  ;

literal_list : literal_list COMMA logical_expression
				{
					struct expr_sem *ptr;
					for( ptr=$1; (ptr->next)!=0; ptr=(ptr->next) );				
					ptr->next = $3;
					$$ = $1;
				}
             | logical_expression
				{
					$$ = $1;
				}
             |
             ;

const_decl 	: CONST scalar_type const_list SEMICOLON
			{
				struct SymNode *newNode;				
				struct constParam *ptr;
				for( ptr=$3; ptr!=0; ptr=(ptr->next) ){
					if( verifyRedeclaration( symbolTable, ptr->name, scope ) == __TRUE ){//no redeclare
						if( ptr->value->category != $2->type ){//type different
							if( !(($2->type==FLOAT_t || $2->type == DOUBLE_t ) && ptr->value->category==INTEGER_t) ) {
								if(!($2->type==DOUBLE_t && ptr->value->category==FLOAT_t)){	
									fprintf( stdout, "########## Error at Line#%d: const type different!! ##########\n", linenum );
									semError = __TRUE;	
								}
								else{
									newNode = createConstNode( ptr->name, scope, $2, ptr->value , ++top);
									insertTab( symbolTable, newNode );
								}
							}							
							else{
								newNode = createConstNode( ptr->name, scope, $2, ptr->value , ++top);
								insertTab( symbolTable, newNode );
							}
						}
						else{
							newNode = createConstNode( ptr->name, scope, $2, ptr->value, ++top );
							insertTab( symbolTable, newNode );
						}
					}
				}
			}
			;

const_list : const_list COMMA ID ASSIGN_OP literal_const
			{				
				addConstParam( $1, createConstParam( $5, $3 ) );
				$$ = $1;
			}
		   | ID ASSIGN_OP literal_const
			{
				$$ = createConstParam( $3, $1 );	
			}
		   ;

array_decl : ID dim 
			{
				$$ = createParam( createIdList( $1 ), $2 );
			}
		   ;

dim : dim ML_BRACE INT_CONST MR_BRACE
		{
			if( $3 == 0 ){
				fprintf( stdout, "########## Error at Line#%d: array size error!! ##########\n", linenum );
				semError = __TRUE;
			}
			else
				increaseArrayDim( $1, 0, $3 );			
		}
	| ML_BRACE INT_CONST MR_BRACE	
		{
			if( $2 == 0 ){
				fprintf( stdout, "########## Error at Line#%d: array size error!! ##########\n", linenum );
				semError = __TRUE;
			}			
			else{
				$$ = createPType( VOID_t ); 			
				increaseArrayDim( $$, 0, $2 );
			}		
		}
	;
	
compound_statement : {scope++;}L_BRACE var_const_stmt_list R_BRACE
					{ 
						// print contents of current scope
						if( Opt_Symbol == 1 )
							printSymTable( symbolTable, scope );
							
						deleteScope( symbolTable, scope );	// leave this scope, delete...
						scope--; 
					}
				   ;

var_const_stmt_list : var_const_stmt_list statement	
				    | var_const_stmt_list var_decl
					| var_const_stmt_list const_decl
				    |
				    ;

statement : compound_statement
		  | simple_statement
		  | conditional_statement
		  | while_statement
		  | for_statement
		  | function_invoke_statement
		  | jump_statement
		  ;		

simple_statement : variable_reference ASSIGN_OP logical_expression SEMICOLON
					{
						// check if LHS exists
						__BOOLEAN flagLHS = verifyExistence( symbolTable, $1, scope, __TRUE );
						// id RHS is not dereferenced, check and deference
						__BOOLEAN flagRHS = __TRUE;
						if( $3->isDeref == __FALSE ) {
							flagRHS = verifyExistence( symbolTable, $3, scope, __FALSE );
						}
						// if both LHS and RHS are exists, verify their type
						if( flagLHS==__TRUE && flagRHS==__TRUE ){
							verifyAssignmentTypeMatch( $1, $3 );
							gen_store($1);
						}
					}
				 | PRINT { gen_print1(); } logical_expression SEMICOLON { 
				 	    verifyScalarExpr( $3, "print" ); 
				 	    
				 	    gen_print2($3);
				   }	
				 | READ variable_reference SEMICOLON 
					{ 
						if( verifyExistence( symbolTable, $2, scope, __TRUE ) == __TRUE ) {					
							verifyScalarExpr( $2, "read" ); 
							struct SymNode *node = 0;
							node = lookupSymbol( symbolTable, $2->varRef->id, scope, __FALSE );	
							
							gen_read($2, node->scope, node->addr);
						}
					}
				 ;

conditional_statement : IF L_PAREN conditional_if  R_PAREN compound_statement
					  | IF L_PAREN conditional_if  R_PAREN compound_statement
						ELSE compound_statement
					  ;
conditional_if : logical_expression { verifyBooleanExpr( $1, "if" ); };;					  

				
while_statement : WHILE L_PAREN logical_expression { verifyBooleanExpr( $3, "while" ); } R_PAREN { inloop++; }
					compound_statement { inloop--; }
				| { inloop++; } DO compound_statement WHILE L_PAREN logical_expression R_PAREN SEMICOLON  
					{ 
						 verifyBooleanExpr( $6, "while" );
						 inloop--; 
						
					}
				;


				
for_statement : FOR L_PAREN initial_expression SEMICOLON control_expression SEMICOLON increment_expression R_PAREN  { inloop++; }
					compound_statement  { inloop--; }
			  ;

initial_expression : initial_expression COMMA statement_for		
				   | initial_expression COMMA logical_expression
				   | logical_expression	
				   | statement_for
				   |
				   ;

control_expression : control_expression COMMA statement_for
				   {
						fprintf( stdout, "########## Error at Line#%d: control_expression is not boolean type ##########\n", linenum );
						semError = __TRUE;	
				   }
				   | control_expression COMMA logical_expression
				   {
						if( $3->pType->type != BOOLEAN_t ){
							fprintf( stdout, "########## Error at Line#%d: control_expression is not boolean type ##########\n", linenum );
							semError = __TRUE;	
						}
				   }
				   | logical_expression 
					{ 
						if( $1->pType->type != BOOLEAN_t ){
							fprintf( stdout, "########## Error at Line#%d: control_expression is not boolean type ##########\n", linenum );
							semError = __TRUE;	
						}
					}
				   | statement_for
				   {
						fprintf( stdout, "########## Error at Line#%d: control_expression is not boolean type ##########\n", linenum );
						semError = __TRUE;	
				   }
				   |
				   ;

increment_expression : increment_expression COMMA statement_for
					 | increment_expression COMMA logical_expression
					 | logical_expression
					 | statement_for
					 |
					 ;

statement_for 	: variable_reference ASSIGN_OP logical_expression
					{
						// check if LHS exists
						__BOOLEAN flagLHS = verifyExistence( symbolTable, $1, scope, __TRUE );
						// id RHS is not dereferenced, check and deference
						__BOOLEAN flagRHS = __TRUE;
						if( $3->isDeref == __FALSE ) {
							flagRHS = verifyExistence( symbolTable, $3, scope, __FALSE );
						}
						// if both LHS and RHS are exists, verify their type
						if( flagLHS==__TRUE && flagRHS==__TRUE )
							verifyAssignmentTypeMatch( $1, $3 );
							
					}
					;
					 
					 
function_invoke_statement : ID L_PAREN logical_expression_list R_PAREN SEMICOLON
							{
								verifyFuncInvoke( $1, $3, symbolTable, scope );
								gen_Invoke($1,$3);
							}
						  | ID L_PAREN R_PAREN SEMICOLON
							{
								verifyFuncInvoke( $1, 0, symbolTable, scope );
								gen_Invoke($1,NULL);
							}
						  ;

jump_statement : CONTINUE SEMICOLON
				{
					if( inloop <= 0){
						fprintf( stdout, "########## Error at Line#%d: continue can't appear outside of loop ##########\n", linenum ); semError = __TRUE;
					}
				}
			   | BREAK SEMICOLON 
				{
					if( inloop <= 0){
						fprintf( stdout, "########## Error at Line#%d: break can't appear outside of loop ##########\n", linenum ); semError = __TRUE;
					}
				}
			   | RETURN logical_expression SEMICOLON
				{
					verifyReturnStatement( $2, funcReturn );
				}
			   ;

variable_reference : ID
					{
						$$ = createExprSem( $1 );
					}
				   | variable_reference dimension
					{	
						increaseDim( $1, $2 );
						$$ = $1;
					}
				   ;

dimension : ML_BRACE arithmetic_expression MR_BRACE
			{
				$$ = verifyArrayIndex( $2 );
			}
		  ;
		  
logical_expression : logical_expression OR_OP logical_term
					{
						verifyAndOrOp( $1, OR_t, $3 );
						$$ = $1;
					}
				   | logical_term { $$ = $1;  }
				   ;

logical_term : logical_term AND_OP logical_factor
				{
					verifyAndOrOp( $1, AND_t, $3 );
					$$ = $1;
				}
			 | logical_factor { $$ = $1; }
			 ;

logical_factor : NOT_OP logical_factor
				{
					verifyUnaryNOT( $2 );
					$$ = $2;
				}
			   | relation_expression { $$ = $1; }
			   ;

relation_expression : arithmetic_expression relation_operator arithmetic_expression
					{
						verifyRelOp( $1, $2, $3 );
						$$ = $1;
					}
					| arithmetic_expression { $$ = $1; }
					;

relation_operator : LT_OP { $$ = LT_t; }
				  | LE_OP { $$ = LE_t; }
				  | EQ_OP { $$ = EQ_t; }
				  | GE_OP { $$ = GE_t; }
				  | GT_OP { $$ = GT_t; }
				  | NE_OP { $$ = NE_t; }
				  ;

arithmetic_expression : arithmetic_expression add_op term
			{
				verifyArithmeticOp( $1, $2, $3 );
				$$ = $1;
				gen_expre($1,$3,0);
			}
                   | relation_expression { $$ = $1; }
		   | term { $$ = $1; }
		   ;

add_op	: ADD_OP { $$ = ADD_t; }
		| SUB_OP { $$ = SUB_t; }
		;
		   
term : term mul_op factor
		{
			if( $2 == MOD_t ) {
				verifyModOp( $1, $3 );
			}
			else {
				verifyArithmeticOp( $1, $2, $3 );
			}
			$$ = $1;
		}
     | factor { $$ = $1; }
	 ;

mul_op 	: MUL_OP { $$ = MUL_t; }
		| DIV_OP { $$ = DIV_t; }
		| MOD_OP { $$ = MOD_t; }
		;
		
factor : variable_reference
		{
			verifyExistence( symbolTable, $1, scope, __FALSE );
			$$ = $1;
			$$->beginningOp = NONE_t;
			gen_load($1);
		}
	   | SUB_OP variable_reference
		{
			if( verifyExistence( symbolTable, $2, scope, __FALSE ) == __TRUE )
				verifyUnaryMinus( $2 );
			$$ = $2;
			$$->beginningOp = SUB_t;
			gen_load($2);
		}		
	   | L_PAREN logical_expression R_PAREN
		{
			$2->beginningOp = NONE_t;
			$$ = $2; 
			gen_load($2);
		}
	   | SUB_OP L_PAREN logical_expression R_PAREN
		{
			verifyUnaryMinus( $3 );
			$$ = $3;
			$$->beginningOp = SUB_t;
			gen_load($3);
		}
	   | ID L_PAREN logical_expression_list R_PAREN
		{
			$$ = verifyFuncInvoke( $1, $3, symbolTable, scope );
			$$->beginningOp = NONE_t;
			gen_Invoke($1,$3);
			
		}
	   | SUB_OP ID L_PAREN logical_expression_list R_PAREN
	    {
			$$ = verifyFuncInvoke( $2, $4, symbolTable, scope );
			$$->beginningOp = SUB_t;
			gen_Invoke($2,$4);
		}
	   | ID L_PAREN R_PAREN
		{
			$$ = verifyFuncInvoke( $1, 0, symbolTable, scope );
			$$->beginningOp = NONE_t;
			gen_Invoke($1,NULL);
		}
	   | SUB_OP ID L_PAREN R_PAREN
		{
			$$ = verifyFuncInvoke( $2, 0, symbolTable, scope );
			$$->beginningOp = SUB_OP;
			gen_Invoke($2,NULL);
		}
	   | literal_const
	    {
			  $$ = (struct expr_sem *)malloc(sizeof(struct expr_sem));
			  $$->isDeref = __TRUE;
			  $$->varRef = 0;
			  $$->pType = createPType( $1->category );
			  $$->next = 0;
			  if( $1->hasMinus == __TRUE ) {
			  	$$->beginningOp = SUB_t;
			  }
			  else {
				$$->beginningOp = NONE_t;
			  }
			  gen_constVal($$);
		}
	   ;

logical_expression_list : logical_expression_list COMMA logical_expression
						{
			  				struct expr_sem *exprPtr;
			  				for( exprPtr=$1 ; (exprPtr->next)!=0 ; exprPtr=(exprPtr->next) );
			  				exprPtr->next = $3;
			  				$$ = $1;
						}
						| logical_expression { $$ = $1; }
						;

		  


scalar_type : INT { $$ = createPType( INTEGER_t ); }
			| DOUBLE { $$ = createPType( DOUBLE_t ); }
			| STRING { $$ = createPType( STRING_t ); }
			| BOOL { $$ = createPType( BOOLEAN_t ); }
			| FLOAT { $$ = createPType( FLOAT_t ); }
			;
 
literal_const : INT_CONST
				{
					int tmp = $1;
					$$ = createConstAttr( INTEGER_t, &tmp );
					ConstInt = tmp;
				}
			  | SUB_OP INT_CONST
				{
					int tmp = -$2;
					$$ = createConstAttr( INTEGER_t, &tmp );
					ConstInt = tmp;
				}
			  | FLOAT_CONST
				{
					float tmp = $1;
					$$ = createConstAttr( FLOAT_t, &tmp );
					ConstFloat = tmp;
				}
			  | SUB_OP FLOAT_CONST
			    {
					float tmp = -$2;
					$$ = createConstAttr( FLOAT_t, &tmp );
					ConstFloat = tmp;
				}
			  | SCIENTIFIC
				{
					double tmp = $1;
					$$ = createConstAttr( DOUBLE_t, &tmp );
					ConstDouble = tmp;
				}
			  | SUB_OP SCIENTIFIC
				{
					double tmp = -$2;
					$$ = createConstAttr( DOUBLE_t, &tmp );
					ConstDouble = tmp;
				}
			  | STR_CONST
				{
					$$ = createConstAttr( STRING_t, $1 );
					strcpy(ConstString, $$->value.stringVal);
				}
			  | TRUE
				{
					SEMTYPE tmp = __TRUE;
					$$ = createConstAttr( BOOLEAN_t, &tmp );
					ConstBool = 1;
				}
			  | FALSE
				{
					SEMTYPE tmp = __FALSE;
					$$ = createConstAttr( BOOLEAN_t, &tmp );
					ConstBool = 0;
				}
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


