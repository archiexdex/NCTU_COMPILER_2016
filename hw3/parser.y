%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include "xd.h"

using namespace std;

extern int linenum;
extern FILE	*yyin;
extern char	*yytext;
extern char buf[256];
extern "C" int yylex(void);
extern int Opt_Symbol;
extern bool isError;


int  yyerror(char* ); 
void logs(const char* , ...);
int  checkType(Type, Type, int);
void  addVal(ConstVal& , ConstVal);
void  subVal(ConstVal& , ConstVal);
void  mulVal(ConstVal& , ConstVal);
void  divVal(ConstVal& , ConstVal);
void  modVal(ConstVal& , ConstVal);

const char* typeName[] = {"int", "float", "double", "bool", "string", "void"};
const char* kindName[] = {"function", "parameter", "variable", "constant"};

ConstVal neg(ConstVal a){
	ConstVal tmp = a;
	tmp.val.i = -tmp.val.i;
	tmp.val.f = -tmp.val.f;
	tmp.val.d = -tmp.val.d;
	tmp.val.b = !tmp.val.b;
	return tmp;
}


string getType(Type type) {
	stringstream tmp;
	tmp << typeName[type.t];
	if ( type.dim > 0 ){
		for(int i = 0 ; i < type.dim ; i++){
			tmp << "[" << type.array[i] << "]"; 
		}
	}
	return tmp.str();
}

class Entry {
public:
	Entry() {}
	Entry(string _n, Type _t, int _k): _name(_n), _type(_t), _kind(_k) { }
	virtual ~Entry() { }

	virtual string attr() = 0;
	string text() { return _name; }
	string kind() { return kindName[_kind]; }
	string type() { return getType(_type); }

	string _name;
	int    _kind;
	Type   _type;
};

class Entry_var : public Entry {
public:
	Entry_var() { }
	Entry_var(string _n, Type _t, int _k): Entry(_n,_t,_k) { }
	virtual ~Entry_var() { }
	string attr() { return ""; }
};

class Entry_func : public Entry {
public:
	Entry_func() { }
	Entry_func(string _n, Type _t, int _k, VarList* ptr): Entry(_n,_t,_k) {
		params.clear();
		for( VarList *it = ptr; it; it = it->next ){
			this->params.push_back(it->type);
		}
	}
	virtual ~Entry_func() { }

	string attr() {
		string ans;
		int flg = 1;
		for ( auto ptr: params) {
			if(flg) { 
				flg = 0;
			}
			else {
				ans += ",";
			}
			ans += getType(ptr);
		}
		return ans;
	}

	vector<Type> params;
};

class Entry_const : public Entry {
public:
	Entry_const() { }
	Entry_const(string _n, Type _t, int _k, Value _v): Entry(_n,_t,_k), val(_v) { }
	virtual ~Entry_const() { }
	string attr() {
		string ans;
		stringstream tmp;
		switch(_type.t) {
			case T_INT    : tmp << val.i; break; 
			case T_FLOAT  : tmp << val.f; break;
			case T_DOUBLE : tmp << val.d; break;
			case T_BOOL   : if ( val.b ) 
								tmp << "true" ;
							else 
								tmp << "false"; 
							break;
			case T_STRING : tmp << val.s; break;
		}
		tmp >> ans;
		return ans;
	}
	
	Value val;
};


class Table {
public:
	Table(int _lv = 0): _level(_lv) { }
	~Table() { }
	void clear() {
		for(auto ptr: entry){
			delete ptr.second;
		}
	}

	void insert( string name, Type type, int kind, Value val = {}, VarList* params = NULL ) {
		Entry* ptr = NULL;
		switch(kind) {
			case K_FUNCTION : ptr = new Entry_func (name, type, kind, params);  break;
			case K_PARAMETER: ptr = new Entry_var  (name, type, kind);          break;
			case K_VARIABLE : ptr = new Entry_var  (name, type, kind);          break;
			case K_CONSTANT : ptr = new Entry_const(name, type, kind, val);     break;
		}
		entry[name] = ptr;
		_last = ptr;
	}

	Entry* operator[](string name) {
		auto it = entry.find(name);
		if ( it == entry.end() )
			return NULL;
		return it->second;
	}
	Entry* last() {
		return _last;
	}
	inline int level() const { return _level; }

	void print() {
		for( int i = 0 ; i < 99 ; ++i ) putchar('='); putchar('\n');
		printf("%-33s%-11s%-12s%-19s%-24s\n","Name","Kind","Level","Type","Attribute");
		for( int i = 0 ; i < 99 ; ++i ) putchar('-'); putchar('\n');
		stringstream lv;
		lv << level() << ( level()? "(local)":"(global)" );
		
		for( auto it:entry ){
			auto ptr = it.second;
			printf("%-33s%-11s%-12s%-19s%-24s\n",ptr->text().c_str(), ptr->kind().c_str(), lv.str().c_str(), ptr->type().c_str(), ptr->attr().c_str() );
		}
		for( int i = 0 ; i < 99 ; ++i ) putchar('='); puts("\n");
	}

	int _level;
	map<string, Entry*> entry;
	Entry* _last;
};

vector<Table> table;

Entry* getEntry(string name, int kind = -1) {
	for( int i = table.size()-1 ; i >= 0 ; --i ){
		auto &it = table[i];
		if ( it[name] != NULL && ( kind == -1 || it[name]->_kind == kind ) ) 
			return it[name];
	}
	return NULL;
}

bool checkAttr(VarList* id, VarList* params) {
	auto var = getEntry(id->text,K_FUNCTION);
	auto vec = ((Entry_func*)var)->params;
	bool miss = false;
	VarList* ptr = params;
	for( auto ptr1 = vec.begin() ; ptr1 != vec.end() && ptr ; ptr1++, ptr = ptr->next ){
		if( (*ptr1).dim - (*ptr1).currdim != 0 ){
			miss = true;
			break;
		} 
		if( ptr->type.t > (*ptr1).t ) {
			miss = true;
			break;
		}
		if( (*ptr1).dim > 0 && ptr->type.dim > 0 ){
			for( int i = 0 ; i < vec.size() ; i++ ){
				if( ptr->type.array[i] != (*ptr1).array[i] ){
					miss = true;
					break;
				}
			}
			if ( miss ) break;
		}
	}
	
	return miss;
}

%}

%union {
	char*    text;
	Type     type;
	VarList* varList;
	ConstVal constVal;
}


%token	<varList> ID
%token	<constVal>INT_CONST
%token	<constVal>FLOAT_CONST
%token	<constVal>SCIENTIFIC
%token	<constVal>STR_CONST

%token	<text>LE_OP
%token	<text>NE_OP
%token	<text>GE_OP
%token	<text>EQ_OP
%token	<text>AND_OP
%token	<text>OR_OP

%token	READ
%token	<type>BOOLEAN
%token	WHILE
%token	DO
%token	IF
%token	ELSE
%token	<constVal>TRUE
%token	<constVal>FALSE
%token	FOR
%token	<type>INT
%token	PRINT
%token	<type>BOOL
%token	<type>VOID
%token	<type>FLOAT
%token	<type>DOUBLE
%token	<type>STRING
%token	CONTINUE
%token	BREAK
%token	RETURN
%token  CONST

%token	L_PAREN
%token	R_PAREN
%token	COMMA
%token	SEMICOLON
%token	ML_BRACE
%token	MR_BRACE
%token	L_BRACE
%token	R_BRACE
%token	ADD_OP
%token	SUB_OP
%token	MUL_OP
%token	DIV_OP
%token	MOD_OP
%token	ASSIGN_OP
%token	<text>LT_OP
%token	<text>GT_OP
%token	<text>NOT_OP

%type <type>     scalar_type dimension dim array_list 
%type <type>     jump_statement statement compound_statement var_const_stmt_list
%type <constVal> literal_const logical_expression logical_factor logical_term relation_expression factor term
%type <constVal> arithmetic_expression 
%type <varList>  identifier_list logical_expression_list parameter_list variable_reference 
%type <varList>  array_decl initial_array literal_list const_list 
%type <text>     relation_operator

/*	Program 
	Function 
	Array 
	Const 
	IF 
	ELSE 
	RETURN 
	FOR 
	WHILE
*/

%start program
%%

push_table : { table.push_back(Table(table.size() ) ); }
pop_table  : { if (Opt_Symbol) { table.back().print(); } 
			   table.back().clear();
			   table.pop_back();
			 }


program :  push_table decl_list funct_def decl_and_def_list pop_table
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

funct_def : scalar_type ID L_PAREN R_PAREN compound_statement {  
				if ( $5.t > $1.t ) {
					logs(" return type is not equal");
				}
				table.back().insert($2->text, $1, K_FUNCTION);
				free($2->text);
			}
		  | scalar_type ID L_PAREN parameter_list R_PAREN  compound_statement {
		  	if ( $6.t > $1.t ) {
					logs(" return type is not equal");
				}
				Value tmp;
				table.back().insert($2->text, $1, K_FUNCTION, tmp, $4);
				free($2->text);
		  }
		  | VOID ID L_PAREN R_PAREN compound_statement {
				Type tmp;
				tmp.t = T_VOID;
				table.back().insert($2->text, tmp, K_FUNCTION);
				free($2->text);
		  }
		  | VOID ID L_PAREN parameter_list R_PAREN compound_statement {
		  	 	Value tmp;
				Type tt;
				tt.t = T_VOID;
				table.back().insert($2->text, tt, K_FUNCTION, tmp, $4); ;
				free($2->text);
		  }
		  ;

funct_decl  : scalar_type ID L_PAREN R_PAREN SEMICOLON {
				
				table.back().insert($2->text, $1, K_FUNCTION);
				free($2->text);
			}
			| scalar_type ID L_PAREN parameter_list R_PAREN SEMICOLON {
				Value tmp;
				table.back().insert($2->text, $1, K_FUNCTION, tmp, $4);
				free($2->text);
			}
			| VOID ID L_PAREN R_PAREN SEMICOLON {
				Type tmp;
				tmp.t = T_VOID;
				table.back().insert($2->text, tmp, K_FUNCTION);
				free($2->text);
			}
			| VOID ID L_PAREN parameter_list R_PAREN SEMICOLON {
				Value tmp;
				Type tt;
				tt.t = T_VOID;
				table.back().insert($2->text, tt, K_FUNCTION, tmp, $4); ;
				free($2->text);
			}
			;

parameter_list  : parameter_list COMMA scalar_type ID {
					for(VarList* ptr = $1; ptr; ptr = ptr->next){
						if( !ptr->next ){
							VarList* tmp = $4;
							tmp->text = $4->text;
							tmp->type = $3;
							tmp->next = NULL;
							ptr->next = tmp;
							break;
						}
					}
					$$ = $1;
				}
				| parameter_list COMMA scalar_type array_decl{
					for( VarList* ptr = $1; ptr; ptr = ptr->next ){
						if( !ptr->next ){
							VarList* tmp = $4;
							tmp->type.t = $3.t;
							tmp->next = NULL;
							ptr->next = tmp;
							break;
						}
					}
					$$ = $1;
				}
				| scalar_type array_decl {
					$$ = $2;
					$$->type.t = $1.t;
					$$->text = $2->text;
					$$->next = NULL;
				}
				| scalar_type ID {
					$$ = $2;
					$$->type = $1;
					$$->text = $2->text;
					$$->next = NULL;
				}
				;

var_decl : scalar_type identifier_list SEMICOLON {
			for(VarList* ptr = $2; ptr != NULL ; ptr = ptr->next) {
				if( ptr->type.t != -1 && checkType($1, ptr->type, 1) < 0 ){
					logs(" The assignment is illegal");
				}
				else {
					
					if ( table.back()[ptr->text] != NULL ){
						logs("%s is redeclared", ptr->text);
					}
					else {
						table.back().insert(ptr->text, $1, K_VARIABLE);
					}
				}
			}
		 }
		 ;

identifier_list : identifier_list COMMA ID {
					for( VarList* ptr = $1; ptr; ptr = ptr->next ){
						if( ptr->next == NULL ){
							ptr->next = $3;
							break;
						}
					}
					$$ = $1;
				}
		 		| identifier_list COMMA ID ASSIGN_OP logical_expression {
					for( VarList* ptr = $1; ptr; ptr = ptr->next ){
						if( ptr->next == NULL ){
							$3->type = $5.type;
							ptr->next = $3;
							break;
						}
					}
					$$ = $1;
				}
				| identifier_list COMMA array_decl ASSIGN_OP initial_array {
					int sum = 0;
					for ( int i = 0 ; i < $3->type.dim ; i++ ){
						sum += $3->type.array[i];
					}
					int now = 0;
					for ( VarList* ptr = $5; ptr; ptr = ptr->next ){
						now++;
					}
					if( sum < now ){
						logs(" Too many values");
					}

					for( VarList* ptr = $1; ptr; ptr = ptr->next ){
						if( ptr->next == NULL ){
							$3->type = $5->type;
							ptr->next = $3;
							break;
						}
					}
					$$ = $1;
					}
				| identifier_list COMMA array_decl {
					for( VarList* ptr = $1; ptr; ptr = ptr->next ){
						if( ptr->next == NULL ){
							ptr->next = $3;
							break;
						}
					}
					$$ = $1;
				}
				| array_decl ASSIGN_OP initial_array {
					int sum = 0;
					for ( int i = 0 ; i < $1->type.dim ; i++ ){
						sum += $1->type.array[i];
					}
					int now = 0;
					for ( VarList* ptr = $3; ptr; ptr = ptr->next ){
						now++;
					}
					if( sum < now ){
						logs(" Too many values");
					}
					$$ = $1;
				}
				| array_decl { $$ = $1; }
				| ID ASSIGN_OP logical_expression {
					$$ = $1;
					$$->type = $3.type;
					$$->next = NULL;
				}
				| ID { $$ = $1; }
				;

initial_array   : L_BRACE literal_list R_BRACE{
					$$ = $2;
				}
				;

literal_list : literal_list COMMA logical_expression {
				for( VarList* ptr = $1 ; ptr; ptr = ptr->next ){
					if( !ptr->next ) {
						VarList* tmp = (VarList*)malloc(sizeof(VarList) );
						tmp->type = $3.type;
						tmp->val = $3.val;
						tmp->next = NULL;
						ptr->next = tmp;
						break;
					}
				}
				$$ = $1;
			}
			 | logical_expression {
				$$ = (VarList*)malloc(sizeof(VarList) );
			 	$$->type = $1.type;
				$$->val = $1.val;
				$$->next = NULL;
			 }
			 |  
			 ;

const_decl  : CONST scalar_type const_list SEMICOLON {
				for(VarList* ptr = $3; ptr != NULL ; ptr = ptr->next) {
					if( checkType($2, ptr->type, 1) < 0 ){
						logs(" The assignment is illegal");
					}
					else {
						if ( table.back()[ptr->text] != NULL ){
							logs(" %s is redeclared", ptr->text);
						}
						else {
							table.back().insert(ptr->text, $2, K_CONSTANT, ptr->val );
						}
					}
				}
			}
			;

const_list  : const_list COMMA ID ASSIGN_OP literal_const {
				for( VarList* ptr = $1; ptr; ptr = ptr->next ){
					if( !ptr->next ){
						VarList* tmp = (VarList*)malloc(sizeof(VarList) );
						tmp->text = $3->text;
						tmp->val  = $5.val;
						tmp->type = $5.type;
						tmp->next = NULL;
						ptr->next = tmp;
						break;
					}
				}
				$$ = $1;
			}
			| ID ASSIGN_OP literal_const  {
				//logs(" %d %f %f %s %d\n",$3.val.i,$3.val.f,$3.val.d, $3.val.s, $3.val.b);
				$$ = (VarList*)malloc(sizeof(VarList) ); 
				$$->text = $1->text;
				$$->val  = $3.val; 
				$$->type = $3.type;
				$$->next = NULL;
			}
			;

array_decl  : ID dim {
				$1->type = $2;
				$$ = $1; 
				$$->type.t = -1;
				$$->next = NULL;
			}
			;

dim : dim ML_BRACE INT_CONST MR_BRACE {
		if ( $3.val.i > 0){
			$1.dim++;
			int* tmp = (int*)malloc(sizeof(int)*$1.dim );
			for( int i = 0 ; i < $1.dim-1 ; ++i) {
				tmp[i] = $1.array[i];
			}
			tmp[$1.dim-1] = $3.val.i;
			free($1.array);
			$1.array = tmp;
			$$ = $1;
		}
		else {
			logs(" array size must greater than 0");
		}
	}
	| ML_BRACE INT_CONST MR_BRACE {
		//cout << "array size " << $2.val.i <<"!" <<endl;
		if ( $2.val.i > 0 ){
			$$.array = (int*)malloc(sizeof(int) );
			$$.dim = 1; 
			$$.array[0] = $2.val.i;
			//for( int i = 0 ; i < $$.dim ; i++ ) cout << $$.array[i] << "!!" <<endl;
		}
		else {
			logs("array size must greater than 0");
		}
	}
	;

compound_statement : push_table L_BRACE var_const_stmt_list R_BRACE pop_table { $$ = $3; }
				   ;

var_const_stmt_list : var_const_stmt_list statement { $$ = $2; }
					| var_const_stmt_list var_decl  {}
					| var_const_stmt_list const_decl{}
					|
				    ;

statement : compound_statement {}
		  | simple_statement {}
		  | conditional_statement {}
		  | while_statement {}
		  | for_statement {}
		  | function_invoke_statement {}
		  | jump_statement { $$ = $1; }
		  ;

simple_statement: variable_reference ASSIGN_OP logical_expression SEMICOLON{
					int tmp = checkType($1->type, $3.type,0);
					if( tmp == 1 ){
						logs(" dimention is not equal");
					}
					tmp = checkType($1->type, $3.type, 1);
					if( tmp < 0 ) {
						logs(" Can't assign value to variable");
					}
				}
				| PRINT logical_expression SEMICOLON {
					if( $2.type.t == T_VOID ) {
						logs(" Can't print void type");
					}
				}
				| READ variable_reference SEMICOLON {
					if( $2->type.t == T_VOID ) {
						logs(" Can't read void type");
					}
				}
				;


conditional_statement   : IF L_PAREN logical_expression R_PAREN { 
							if( $3.type.t != T_BOOL ){
								logs(" contidional expression must be boolean");
							}
						} push_table L_BRACE var_const_stmt_list R_BRACE pop_table
						| IF L_PAREN logical_expression R_PAREN {
								if( $3.type.t != T_BOOL ) {
									logs("conditional expression must be boolean");
								}
							} 
							push_table L_BRACE var_const_stmt_list R_BRACE pop_table
						ELSE
							push_table L_BRACE var_const_stmt_list R_BRACE pop_table
						;
while_statement : WHILE L_PAREN logical_expression R_PAREN {
					if( $3.type.t != T_BOOL ){
						logs(" conditional expression must be boolean");
					}
				}
					push_table L_BRACE var_const_stmt_list R_BRACE pop_table
				| DO  push_table L_BRACE
					var_const_stmt_list
				  R_BRACE pop_table WHILE L_PAREN logical_expression R_PAREN SEMICOLON {
				  	if( $9.type.t != T_BOOL ){
						logs(" conditional expression must be boolean");
					}
				  }
				;

for_statement : FOR L_PAREN initial_expression_list SEMICOLON control_expression SEMICOLON increment_expression_list R_PAREN 
					push_table L_BRACE var_const_stmt_list R_BRACE pop_table
				;

initial_expression_list : initial_expression
						|
						;

initial_expression  : initial_expression COMMA variable_reference ASSIGN_OP logical_expression {
						int tmp = checkType($3->type, $5.type,0);
							if( tmp == 1 ){
								logs("dimention is not equal");
							}
							tmp = checkType($3->type, $5.type, 1);
							if( tmp < 0 ) {
								logs(" Can't assign value to variable");
						}
					}
					| initial_expression COMMA logical_expression {
						
					}
					| logical_expression {  }
					| variable_reference ASSIGN_OP logical_expression{
						int tmp = checkType($1->type, $3.type,0);
							if( tmp == 1 ){
								logs(" dimention is not equal");
							}
							tmp = checkType($1->type, $3.type, 1);
							if( tmp < 0 ) {
								logs(" Can't assign value to variable");
						}
					}
					;

control_expression : logical_expression {
						if ( $1.type.t != T_BOOL ){
							logs(" control expression must be type bool");
						}
					}
					;

increment_expression_list : increment_expression 
						  |
						  ;

increment_expression : increment_expression COMMA variable_reference ASSIGN_OP logical_expression {
					 	int tmp = checkType($3->type, $5.type,0);
							if( tmp == 1 ){
								logs(" dimention is not equal");
							}
							tmp = checkType($3->type, $5.type, 1);
							if( tmp < 0 ) {
								logs(" Can't assign value to variable");
						}
					 }
					 | increment_expression COMMA logical_expression { }
					 | logical_expression { }
					 | variable_reference ASSIGN_OP logical_expression {
						int tmp = checkType($1->type, $3.type,0);
							if( tmp == 1 ){
								logs(" dimention is not equal");
							}
							tmp = checkType($1->type, $3.type, 1);
							if( tmp < 0 ) {
								logs(" Can't assign value to variable");
						}
					 }
					 ;

function_invoke_statement   : ID L_PAREN logical_expression_list R_PAREN SEMICOLON {
								Entry * ptr = getEntry($1->text, K_FUNCTION);
								bool miss = checkAttr($1,$3);
								if (miss){
									logs(" dimention is not equal");
								}
							}
							| ID L_PAREN R_PAREN SEMICOLON {
								Entry* ptr = getEntry($1->text, K_FUNCTION);
								bool miss = checkAttr($1, NULL);
								if ( miss ) {
									logs(" dimention is not equal");
								}
							}
							;

jump_statement  : CONTINUE SEMICOLON { }
				| BREAK SEMICOLON { }
				| RETURN logical_expression SEMICOLON {
					$$ = $2.type;
				}
				;

variable_reference  : array_list { $$->type = $1; }
					| ID { 
						Entry* ptr = getEntry($1->text);
						$$->type.t = ptr->_type.t;
						$$->text = (char*)ptr->text().c_str();
						$$->next = NULL;
						//cout << "!" <<  $$->type.t <<endl;
					}
					;


logical_expression  : logical_expression OR_OP logical_term {
						if( $1.type.t != T_BOOL || $3.type.t != T_BOOL ){
							logs(" The operands are not bool");
						}
					}
					| logical_term { $$ = $1; }
					;

logical_term: logical_term AND_OP logical_factor {
				if( $1.type.t != T_BOOL || $3.type.t != T_BOOL ){
					logs(" The operands are not type bool");
				}
			}
			| logical_factor { $$ = $1; }
			;

logical_factor  : NOT_OP logical_factor { 
					if ( $2.type.t != T_BOOL ){
						logs(" %s must be used for type bool",$1);
					}
					else {
						ConstVal tmp = neg($2); 
						$$ = tmp; 
					}
				}
				| relation_expression { $$ = $1; }
				;

relation_expression : arithmetic_expression relation_operator arithmetic_expression {
						int t1 = 0 , t2 = 0;
						switch ($1.type.t) {
							case T_INT   : t1 = T_INT;    break;
							case T_FLOAT : t1 = T_FLOAT;  break;
							case T_DOUBLE: t1 = T_DOUBLE; break;
							case T_BOOL  : t1 = T_BOOL;   break;
							default: t1 = -1;
						}
						switch ($3.type.t) {
							case T_INT   : t1 = T_INT;    break;
							case T_FLOAT : t1 = T_FLOAT;  break;
							case T_DOUBLE: t1 = T_DOUBLE; break;
							case T_BOOL  : t1 = T_BOOL;   break;
							default: t1 = -1;
						}
						if ( strcmp($2, "==" ) || strcmp($2, "!=" ) ){
							logs("The operands must be int, float, double and bool");
						}
						else if ( t1 < 0 || t2 < 0 ){
							logs(" The operands must be int, float and double");
						}
						else{
							$$.type.t = T_BOOL;
						}
					}
					| arithmetic_expression {
						$$ = $1;
					}
					;

relation_operator : LT_OP
				  | LE_OP
				  | EQ_OP
				  | GE_OP
				  | GT_OP
				  | NE_OP
				  ;

arithmetic_expression   : arithmetic_expression ADD_OP term {
							if( checkType($1.type, $3.type, 0) == 1 ){
								logs(" The dimension is not equal");
							}
							if( checkType($1.type, $3.type, 0) == 2 ){
								
								int t1, t2;
								switch($1.type.t){
									case T_INT   : t1 = T_INT; break;
									case T_FLOAT : t1 = T_FLOAT; break;
									case T_DOUBLE: t1 = T_DOUBLE; break;
									default      : t1 = -1; break;
								}
								switch($3.type.t){
									case T_INT   : t2 = T_INT; break;
									case T_FLOAT : t2 = T_FLOAT; break;
									case T_DOUBLE: t2 = T_DOUBLE; break;
									default      : t2 = -1; break;
								}
								
								if( t1 < 0 || t2 < 0 ){
									logs(" The arithmetic is not Int, Float, Double");
								}
								else {
									t1 = max(t1, t2);
									addVal($1,$3);
									$$ = $1;
									$$.type.t = t1;
								}
							}
							
						}
						| arithmetic_expression SUB_OP term {
							if( checkType($1.type, $3.type, 0) == 1 ){
								logs(" The dimension is not equal");
							}
							if( checkType($1.type, $3.type, 0) == 2 ){
								
								int t1, t2;
								switch($1.type.t){
									case T_INT   : t1 = T_INT; break;
									case T_FLOAT : t1 = T_FLOAT; break;
									case T_DOUBLE: t1 = T_DOUBLE; break;
									default      : t1 = -1; break;
								}
								switch($3.type.t){
									case T_INT   : t2 = T_INT; break;
									case T_FLOAT : t2 = T_FLOAT; break;
									case T_DOUBLE: t2 = T_DOUBLE; break;
									default      : t2 = -1; break;
								}
								
								if( t1 < 0 || t2 < 0 ){
									logs(" The arithmetic is not Int, Float, Double");
								}
								else {
									t1 = max(t1, t2);
									subVal($1,$3);
									$$ = $1;
									$$.type.t = t1;
								}
							}
						}
						| relation_expression { $$ = $1; }
						| term { $$ = $1; }
						;

term : term MUL_OP factor{
	 	if( checkType($1.type, $3.type, 0) == 1 ){
	 		logs("The dimension is not equal");
	 	}
	 	if( checkType($1.type, $3.type, 0) == 2 ){
	 	
	 		int t1, t2;
	 		switch($1.type.t){
	 			case T_INT   : t1 = T_INT; break;
	 			case T_FLOAT : t1 = T_FLOAT; break;
	 			case T_DOUBLE: t1 = T_DOUBLE; break;
	 			default      : t1 = -1; break;
	 		}
	 		switch($3.type.t){
	 			case T_INT   : t2 = T_INT; break;
	 			case T_FLOAT : t2 = T_FLOAT; break;
	 			case T_DOUBLE: t2 = T_DOUBLE; break;
	 			default      : t2 = -1; break;
	 		}
	 		
	 		if( t1 < 0 || t2 < 0 ){
	 			logs(" The arithmetic is not Int, Float, Double");
	 		}
	 		else {
	 			t1 = max(t1, t2);
	 			mulVal($1,$3);
	 			$$ = $1;
	 			$$.type.t = t1;
	 		}
	 }
	 }
	 | term DIV_OP factor{
	 	if( checkType($1.type, $3.type, 0) == 1 ){
	 		logs(" The dimension is not equal");
	 	}
	 	if( checkType($1.type, $3.type, 0) == 2 ){
	 	
	 		int t1, t2;
	 		switch($1.type.t){
	 			case T_INT   : t1 = T_INT; break;
	 			case T_FLOAT : t1 = T_FLOAT; break;
	 			case T_DOUBLE: t1 = T_DOUBLE; break;
	 			default      : t1 = -1; break;
	 		}
	 		switch($3.type.t){
	 			case T_INT   : t2 = T_INT; break;
	 			case T_FLOAT : t2 = T_FLOAT; break;
	 			case T_DOUBLE: t2 = T_DOUBLE; break;
	 			default      : t2 = -1; break;
	 		}
	 		
	 		if( t1 < 0 || t2 < 0 ){
	 			logs(" The arithmetic is not Int, Float, Double");
	 		}
	 		else {
	 			t1 = max(t1, t2);
	 			mulVal($1,$3);
	 			$$ = $1;
	 			$$.type.t = t1;
	 		}
		}
	 }
	 | term MOD_OP factor {
	 	if( checkType($1.type, $3.type, 0) == 1 ){
	 		logs(" The dimension is not equal");
	 	}
	 	if( $1.type.t != T_INT || $3.type.t != T_INT ){
	 		logs(" The arithmetic are not int");
	 	}
	 }
	 | factor { $$ = $1; }
	 ;

factor  : variable_reference {
			$$.type = $1->type;
		}
		| SUB_OP factor {
			ConstVal tmp = neg($2);
			$$ = tmp;
			//cout << $$.val.i << "!!" <<endl;
	    }
		| L_PAREN logical_expression R_PAREN { $$ = $2; }
		| SUB_OP L_PAREN logical_expression R_PAREN { ConstVal tmp = neg($3); $$ = tmp; }
		| ID L_PAREN logical_expression_list R_PAREN {// function 
			Entry* ptr = getEntry($1->text, K_FUNCTION);
			
			bool miss = checkAttr($1, $3);
			int tt = ptr->_type.t;
			if ( miss ) {
				logs(" dimention is not equal");
			}
			else if ( tt == T_VOID ){
				logs(" void can't in expression");
			}
			else if ( tt == T_STRING ){
				logs(" string can't in expression");
			}
			else if( tt == T_BOOL ){
				logs(" bool can't in expression");
			}
			else {
				$$.type.t = tt;
			}
		}
		| ID L_PAREN R_PAREN {                        // function
			Entry* ptr = getEntry($1->text, K_FUNCTION);
			bool miss = checkAttr($1, NULL);
			int tt = ptr->_type.t;
			if( miss ) {
				logs(" dimention is not equal");
			}
			else if ( tt == T_VOID ){
				logs(" void can't in expression");
			}
			else if ( tt == T_STRING ){
				logs(" string can't in expression");
			}
			else if( tt == T_BOOL ){
				logs(" bool can't in expression");
			}
			else {
				$$.type.t = tt;
			}
		}
		| literal_const { $$ = $1; }
		| SUB_OP ID L_PAREN logical_expression_list R_PAREN {// function
			Entry* ptr = getEntry($2->text, K_FUNCTION);
			int tt = ptr->_type.t;
			bool miss = checkAttr($2,$4);
			if ( miss ) {
				logs(" dimention is not equal");
			}
			else if ( tt == T_VOID ){
				logs(" void can't in expression");
			}
			else if ( tt == T_STRING ){
				logs(" string can't in expression");
			}
			else if( tt == T_BOOL ){
				logs(" bool can't in expression");
			}
			else {
				$$.type.t = tt;
			}
		}
		| SUB_OP ID L_PAREN R_PAREN {                   // function
			Entry* ptr = getEntry($2->text, K_FUNCTION);
			int tt = ptr->_type.t;
			bool miss = checkAttr($2,NULL);
			if ( miss  ){
				logs(" dimention is not equal");
			}
			else if ( tt == T_VOID ){
				logs(" void can't in expression");
			}
			else if ( tt == T_STRING ){
				logs(" string can't in expression");
			}
			else if( tt == T_BOOL ){
				logs(" bool can't in expression");
			}
			else {
				$$.type.t = tt;
			}
		}
		;

logical_expression_list : logical_expression_list COMMA logical_expression{
							for( VarList* ptr = $1; ptr; ptr = ptr->next ){
								
								if( !ptr->next ){
									VarList* tmp = (VarList*)malloc(sizeof(VarList) );
									tmp->type = $3.type;
									tmp->next = NULL;
									ptr->next = tmp;
									break;
								}
							}
							$$ = $1;
						}
						| logical_expression { 
							$$ = (VarList*)malloc(sizeof(VarList) );
							$$->type = $1.type; 
							$$->next = NULL;
						}
						;

array_list  : ID dimension { 
				Entry* ptr = getEntry($1->text, K_VARIABLE);
				$$ = $2;
				$$.t = ptr->_type.t;
				$$.currdim = $2.currdim;
				$$.dim = ptr->_type.dim;
			}
			;

dimension   : dimension ML_BRACE logical_expression MR_BRACE {
				$1.currdim++;
				if( $3.type.t != T_INT ){
					logs(" Array index must be int");
				}
				else if( $3.val.i <= 0 ){
					logs(" Array index must greater than 0");
				}
				$$ = $1;
			}
			| ML_BRACE logical_expression MR_BRACE {
				$$.currdim = 1;
				$$.array = NULL;
				if( $2.type.t != T_INT ){
					logs(" Array index must be int");
				}
				else if( $2.val.i <= 0 ){
					logs(" Array index must greater than 0");
				}
			}
			;

scalar_type : INT    { $$ = $1; }
			| DOUBLE { $$ = $1; }
			| STRING { $$ = $1; }
			| BOOL   { $$ = $1; }
			| FLOAT  { $$ = $1; }
			;
 
literal_const : INT_CONST          { $$ = $1;   }
			  | SUB_OP INT_CONST   { ConstVal tmp = neg($2); $$ = tmp; }
			  | FLOAT_CONST        { $$ = $1; }
			  | SUB_OP FLOAT_CONST { ConstVal tmp = neg($2); $$ = tmp; }
			  | SCIENTIFIC         { $$ = $1; }
			  | SUB_OP SCIENTIFIC  { ConstVal tmp = neg($2); $$ = tmp; }
			  | STR_CONST          { $$ = $1; cout << $1.val.s << endl; }
			  | TRUE               { $$ = $1; }
			  | FALSE              { $$ = $1; }
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
	//  fprintf( stderr, "%s\t%d\t%s\t%s\n", "Error found in Line ", linenum, "next token: ", yytext );
}

int checkType(Type a, Type b, int mode = 0) {
	if ( mode == 0 ){
		if( a.dim - a.currdim != b.dim - b.currdim ) return 1;
	
		if( a.t != b.t ) return 2;
	}
	else if( mode == 1 ){
		if( a.t == T_VOID ){
			return -1;
		}
		if( (a.t == T_STRING || a.t == T_BOOL ) && a.t != b.t ){
			return -1;
		}
		if( a.t < b.t ){
			return -1;
		}
	}
	return 0;
}

void logs(const char* msg, ...) {
	isError = true;
	va_list args;
	va_start(args, msg);
	fprintf(stderr, "##########Error at Line %d: ",linenum);
	vfprintf(stderr, msg, args);
	fprintf(stderr, "##########\n");
	va_end(args);
}

void addVal(ConstVal& a, ConstVal b){
	a.val.i += b.val.i;
	a.val.f += b.val.f;
	a.val.d += b.val.d;
}
void subVal(ConstVal& a, ConstVal b){
	a.val.i -= b.val.i;
	a.val.f -= b.val.f;
	a.val.d -= b.val.d;
}
void mulVal(ConstVal& a, ConstVal b){
	a.val.i *= b.val.i;
	a.val.f *= b.val.f;
	a.val.d *= b.val.d;
}
void divVal(ConstVal& a, ConstVal b){
	a.val.i /= b.val.i;
	a.val.f /= b.val.f;
	a.val.d /= b.val.d;
}
void modVal(ConstVal& a, ConstVal b){
	a.val.i %= b.val.i;
}
