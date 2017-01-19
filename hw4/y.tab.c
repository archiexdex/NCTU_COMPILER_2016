/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

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

int QQ[1000] = {0};
int isMain = 0;
const char  types[] = { 'V', 'I', 'F', 'D' ,'Z', 'X'};
const char* reops[] = { "iflt", "ifle", "ifeq", "ifge", "ifgt", "ifne" };
char ConstString[100];
int ConstBool = 0;
int ConstInt = 0;
float ConstFloat = 0.0;
double ConstDouble = 0.0;
int top = -1;
int re0 = -1;
int xd = -1;
char begin[100];
char next[100];
char exixt[100];
char incr[100];
char Lelse[100];
char Lexit[100];
int stk[100];

void log(int a){
	printf(">> %d!!\n",a);
}
void push(int tmp){
	stk[++xd] = tmp;
}
void pop(){
	--xd;
}

//>>>>>>>>>>>>>>>>>>>> load
void gen_load(struct expr_sem *expr ) {
	char tmp = types[expr->pType->type];
	struct SymNode *node = lookupSymbol( symbolTable, expr->varRef->id, scope, __FALSE );
	if ( node->scope ) {
		
		if ( tmp == 'I' || tmp == 'Z' ){
			fprintf(java, "iload %d\n", node->addr);
		}
		else if ( tmp == 'F' || tmp == 'D' ) {
			fprintf(java, "fload %d\n", node->addr);	
		}
	}
	else if( node->category == CONSTANT_t ){
		// printf("!!!!!>>> %d ",node->attribute->constVal->value);
		if ( tmp == 'I' || tmp == 'Z' )
			fprintf(java, "ldc %d\n", node->attribute->constVal->value);
		else if ( tmp == 'F' || tmp == 'D' ) 
			fprintf(java, "ldc %f\n", node->attribute->constVal->value);
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
		
		if ( tmp == 'I' || tmp == 'Z' ){
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
	else if( 1 <= ta && ta <= 3 && 1 <= tb && tb <= 4 ){
		
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
void gen_rel(struct expr_sem *a, struct expr_sem *b, int mode, int t) {
	int ta = a->pType->type, tb = b->pType->type;
	char label1[100], label2[100];
	if ( ( 1 < ta && ta < 4 )  || ( 1 < tb && tb < 4) ) {
		fprintf(java, "fcmpl\n");	
	}
	else {
		fprintf(java, "isub\n");
	}
	sprintf(label1, "L%d", t);
	sprintf(label2, "LL%d", t);
	fprintf(java, "%s %s\n", reops[mode-5], label1);
	fprintf(java, "iconst_0\n");
	fprintf(java, "goto %s\n", label2);
	fprintf(java, "%s:\n", label1);
	fprintf(java, "iconst_1\n");
	fprintf(java, "%s:\n", label2);
}


//>>>>>>>>>>>>>>>>>>>> function 
void gen_func1(char* id) {
	fprintf(java,".method public static %s",id);
}
void gen_funcArg(struct param_sem* params){
	isMain = 0;
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
	isMain = 1;
	fprintf(java,"([Ljava/lang/String;)V\n");
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
void gen_funcEnd(struct PType *pType, char *id){
	
	if ( !strcmp("main", id) || pType == NULL ) {
		
		fprintf(java, "return\n");	
	}
	else if ( pType->type == 1 ){
		fprintf(java, "ireturn\n");
	}
	else if( pType->type == 2 || pType->type == 3 ){
		fprintf(java, "freturn\n");
	}
	
	fprintf(java,".end method\n");
}
//>>>>>>>>>>>>>>>>>>>> invoke function 
int gen_Invoke(char *id, struct expr_sem *exprs){
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
	return node->type->type;
}



#line 389 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LE_OP = 258,
    NE_OP = 259,
    GE_OP = 260,
    EQ_OP = 261,
    AND_OP = 262,
    OR_OP = 263,
    READ = 264,
    BOOLEAN = 265,
    WHILE = 266,
    DO = 267,
    IF = 268,
    ELSE = 269,
    TRUE = 270,
    FALSE = 271,
    FOR = 272,
    INT = 273,
    PRINT = 274,
    BOOL = 275,
    VOID = 276,
    FLOAT = 277,
    DOUBLE = 278,
    STRING = 279,
    CONTINUE = 280,
    BREAK = 281,
    RETURN = 282,
    CONST = 283,
    L_PAREN = 284,
    R_PAREN = 285,
    COMMA = 286,
    SEMICOLON = 287,
    ML_BRACE = 288,
    MR_BRACE = 289,
    L_BRACE = 290,
    R_BRACE = 291,
    ADD_OP = 292,
    SUB_OP = 293,
    MUL_OP = 294,
    DIV_OP = 295,
    MOD_OP = 296,
    ASSIGN_OP = 297,
    LT_OP = 298,
    GT_OP = 299,
    NOT_OP = 300,
    ID = 301,
    INT_CONST = 302,
    FLOAT_CONST = 303,
    SCIENTIFIC = 304,
    STR_CONST = 305
  };
#endif
/* Tokens.  */
#define LE_OP 258
#define NE_OP 259
#define GE_OP 260
#define EQ_OP 261
#define AND_OP 262
#define OR_OP 263
#define READ 264
#define BOOLEAN 265
#define WHILE 266
#define DO 267
#define IF 268
#define ELSE 269
#define TRUE 270
#define FALSE 271
#define FOR 272
#define INT 273
#define PRINT 274
#define BOOL 275
#define VOID 276
#define FLOAT 277
#define DOUBLE 278
#define STRING 279
#define CONTINUE 280
#define BREAK 281
#define RETURN 282
#define CONST 283
#define L_PAREN 284
#define R_PAREN 285
#define COMMA 286
#define SEMICOLON 287
#define ML_BRACE 288
#define MR_BRACE 289
#define L_BRACE 290
#define R_BRACE 291
#define ADD_OP 292
#define SUB_OP 293
#define MUL_OP 294
#define DIV_OP 295
#define MOD_OP 296
#define ASSIGN_OP 297
#define LT_OP 298
#define GT_OP 299
#define NOT_OP 300
#define ID 301
#define INT_CONST 302
#define FLOAT_CONST 303
#define SCIENTIFIC 304
#define STR_CONST 305

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 324 "parser.y" /* yacc.c:355  */

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

#line 543 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 560 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   306

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  60
/* YYNRULES -- Number of rules.  */
#define YYNRULES  152
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  253

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   305

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   360,   360,   369,   370,   371,   372,   376,   377,   378,
     379,   380,   385,   384,   413,   412,   458,   457,   487,   486,
     532,   536,   547,   551,   564,   571,   577,   582,   585,   628,
     637,   652,   661,   669,   675,   680,   687,   695,   698,   705,
     709,   712,   743,   748,   754,   760,   769,   782,   782,   793,
     794,   795,   796,   799,   800,   801,   802,   803,   804,   805,
     808,   824,   824,   829,   841,   848,   848,   867,   875,   884,
     890,   895,   884,   907,   907,   932,   937,   949,   954,   932,
     972,   973,   974,   975,   976,   979,   984,   991,   998,  1003,
    1006,  1007,  1008,  1009,  1010,  1013,  1032,  1037,  1044,  1050,
    1056,  1077,  1081,  1088,  1094,  1100,  1103,  1109,  1112,  1118,
    1121,  1129,  1132,  1133,  1134,  1135,  1136,  1137,  1140,  1146,
    1147,  1150,  1151,  1154,  1165,  1168,  1169,  1170,  1173,  1180,
    1189,  1195,  1203,  1210,  1223,  1229,  1241,  1258,  1265,  1271,
    1272,  1273,  1274,  1275,  1278,  1284,  1290,  1296,  1302,  1308,
    1314,  1319,  1325
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LE_OP", "NE_OP", "GE_OP", "EQ_OP",
  "AND_OP", "OR_OP", "READ", "BOOLEAN", "WHILE", "DO", "IF", "ELSE",
  "TRUE", "FALSE", "FOR", "INT", "PRINT", "BOOL", "VOID", "FLOAT",
  "DOUBLE", "STRING", "CONTINUE", "BREAK", "RETURN", "CONST", "L_PAREN",
  "R_PAREN", "COMMA", "SEMICOLON", "ML_BRACE", "MR_BRACE", "L_BRACE",
  "R_BRACE", "ADD_OP", "SUB_OP", "MUL_OP", "DIV_OP", "MOD_OP", "ASSIGN_OP",
  "LT_OP", "GT_OP", "NOT_OP", "ID", "INT_CONST", "FLOAT_CONST",
  "SCIENTIFIC", "STR_CONST", "$accept", "program", "decl_list",
  "decl_and_def_list", "funct_def", "$@1", "$@2", "$@3", "$@4",
  "funct_decl", "parameter_list", "var_decl", "identifier_list",
  "initial_array", "literal_list", "const_decl", "const_list",
  "array_decl", "dim", "compound_statement", "$@5", "var_const_stmt_list",
  "statement", "simple_statement", "$@6", "conditional_statement", "$@7",
  "R", "conditional_if", "while_statement", "$@8", "$@9", "$@10", "$@11",
  "for_statement", "$@12", "$@13", "$@14", "$@15", "initial_expression",
  "control_expression", "increment_expression", "statement_for",
  "function_invoke_statement", "jump_statement", "variable_reference",
  "dimension", "logical_expression", "logical_term", "logical_factor",
  "relation_expression", "relation_operator", "arithmetic_expression",
  "add_op", "term", "mul_op", "factor", "logical_expression_list",
  "scalar_type", "literal_const", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305
};
# endif

#define YYPACT_NINF -173

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-173)))

#define YYTABLE_NINF -110

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -173,    31,   211,  -173,  -173,  -173,     8,  -173,  -173,  -173,
     165,  -173,  -173,  -173,  -173,    45,    28,    48,   211,    18,
      -3,    33,    58,    35,   100,  -173,  -173,  -173,  -173,   151,
      60,    55,    78,    67,  -173,    81,    90,   110,   115,   108,
     124,  -173,   127,   113,   138,  -173,  -173,    55,    88,    55,
     155,  -173,  -173,  -173,  -173,   153,   184,   190,  -173,    10,
      29,   111,  -173,  -173,   154,   -21,   166,    55,  -173,  -173,
    -173,   178,   165,   203,  -173,   147,  -173,   199,  -173,  -173,
     210,  -173,     0,    55,   214,  -173,  -173,  -173,   153,  -173,
      80,   256,  -173,    55,    55,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,   256,   256,  -173,  -173,  -173,   256,   215,
      55,    81,    83,   184,  -173,   209,  -173,  -173,   204,   108,
    -173,  -173,  -173,  -173,    14,   208,  -173,   184,   117,  -173,
      21,   190,  -173,    29,   111,  -173,  -173,   184,  -173,    55,
    -173,  -173,  -173,   203,  -173,  -173,  -173,  -173,  -173,   135,
    -173,    55,  -173,   184,   194,  -173,   184,   205,   216,   223,
     230,  -173,   228,   231,    55,  -173,   235,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,   253,  -173,  -173,  -173,    50,   220,
    -173,   144,  -173,  -173,    55,    55,  -173,  -173,     6,   232,
    -173,    55,    57,  -173,    55,    55,   168,  -173,    64,   184,
       7,  -173,   236,   195,   258,    11,   184,   237,   184,    55,
    -173,    55,  -173,  -173,   241,   245,  -173,   246,  -173,  -173,
     184,    55,   184,  -173,    55,  -173,   261,   196,  -173,   184,
      15,  -173,   269,    55,  -173,   252,  -173,  -173,  -173,   184,
      55,  -173,  -173,   255,  -173,   184,    55,   257,  -173,   184,
    -173,  -173,  -173
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       6,     0,     0,     1,   139,   142,     0,   143,   140,   141,
       0,    11,     5,     3,     4,     0,     0,     0,     2,    36,
       0,    34,     0,     0,     0,    10,     9,     7,     8,     0,
       0,     0,    44,     0,    28,     0,    16,     0,     0,     0,
       0,    41,    12,     0,     0,   151,   152,     0,     0,     0,
     101,   144,   146,   148,   150,   128,    35,   105,   107,   119,
     111,   120,   124,   136,     0,    29,    32,    40,    33,    22,
      47,    18,     0,    27,    26,     0,    43,     0,    20,    47,
      14,    46,     0,     0,   101,   145,   147,   149,   129,   108,
       0,     0,   102,     0,     0,   113,   117,   115,   114,   121,
     122,   112,   116,     0,     0,   125,   126,   127,     0,     0,
       0,     0,     0,    39,    17,     0,    23,    47,     0,     0,
      13,    21,    47,   130,     0,     0,   134,   138,     0,   119,
       0,   104,   106,   110,   118,   123,    45,    30,    31,     0,
      37,    52,    19,    24,    25,    42,    15,   131,   135,     0,
     132,     0,   103,    38,    47,   133,   137,     0,     0,     0,
       0,    61,     0,     0,     0,    48,   101,    50,    51,    53,
      49,    54,    55,    56,     0,    57,    58,    59,     0,     0,
     101,     0,    69,    67,    84,     0,    98,    99,     0,     0,
      47,     0,    36,    63,     0,     0,     0,    83,   128,    82,
       0,   100,     0,     0,     0,     0,    70,     0,    68,     0,
      75,     0,    62,    97,     0,     0,    60,     0,    47,    80,
      81,    89,    95,    96,     0,    71,    64,     0,    88,    87,
       0,    47,     0,     0,    76,     0,    72,    47,    85,    86,
      94,    74,    66,    77,    93,    92,     0,     0,    90,    91,
      78,    47,    79
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -173,  -173,  -173,  -173,   270,  -173,  -173,  -173,  -173,   271,
     262,   -16,  -173,   179,  -173,   -15,  -173,   -29,  -173,   -69,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -172,  -173,  -173,   -42,  -173,   -31,   200,   -38,
     -41,  -173,    -5,  -173,   188,  -173,   187,  -120,    -9,   -32
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    18,    11,    79,   122,    70,   117,    12,
      37,    13,    20,    68,   112,    14,    24,    21,    32,   114,
     115,   154,   170,   171,   185,   172,   232,   195,   207,   173,
     194,   217,   231,   174,   175,   221,   240,   247,   251,   196,
     227,   243,   197,   176,   177,    55,    92,   127,    57,    58,
      59,   103,    60,   104,    61,   108,    62,   128,    15,    63
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      56,    17,    27,    28,    66,   149,    88,    76,    93,    74,
     120,    89,    30,    38,    93,    93,    82,  -109,  -109,    93,
      38,   110,    93,    93,    95,    96,    97,    98,    33,    34,
     123,     3,    95,    96,    97,    98,   113,   219,   201,   212,
    -109,  -109,  -109,   216,   147,   235,  -109,    29,   142,   228,
     129,    30,   124,   146,    16,   152,   132,    22,    99,   100,
      31,   238,   129,   118,   101,   102,    99,   100,   244,   203,
      45,    46,   101,   102,   248,    35,     4,    39,     5,   137,
       7,     8,     9,    91,    47,   169,   130,   145,    36,   144,
      30,    19,   191,    48,    23,    45,    46,    91,   133,    31,
      49,    50,    51,    52,    53,    54,   211,    44,   153,    47,
     126,    64,   178,    65,   139,   181,    67,    83,    48,   140,
     156,   204,    69,    45,    46,    49,    50,    51,    52,    53,
      54,    40,    41,   188,    84,    85,    86,    87,   167,   168,
      71,    72,   198,    80,    72,   179,    75,   150,   151,   226,
     105,   106,   107,   199,   200,    51,    52,    53,    54,    78,
     205,    73,   236,   206,   208,   155,   151,   198,   242,     4,
      77,     5,    81,     7,     8,     9,   193,    91,   220,   198,
     222,    42,   252,     4,    90,     5,    91,     7,     8,     9,
     229,   198,    93,   230,    85,    86,    87,    94,   198,   209,
     210,   109,   239,   157,   198,   158,   -73,   159,   111,   245,
     116,   160,     4,   161,     5,   249,     7,     8,     9,   162,
     163,   164,    10,    45,    46,   214,   151,   233,   234,     4,
     165,     5,     6,     7,     8,     9,    30,    47,   148,    10,
     166,   119,   121,   125,   141,   182,    48,    45,    46,   136,
     143,   180,   183,    49,    50,    51,    52,    53,    54,   184,
     186,    47,   202,   187,   189,   190,   192,   218,   213,   215,
      48,    45,    46,   223,   224,   -65,   225,    49,    50,    51,
      52,    53,    54,   237,   241,    47,   246,   250,    25,    26,
     138,    43,   134,   131,    48,   135,     0,     0,     0,     0,
       0,     0,    50,    51,    52,    53,    54
};

static const yytype_int16 yycheck[] =
{
      31,    10,    18,    18,    33,   125,    48,    39,     8,    38,
      79,    49,    33,    22,     8,     8,    47,     7,     8,     8,
      29,    42,     8,     8,     3,     4,     5,     6,    31,    32,
      30,     0,     3,     4,     5,     6,    67,   209,    32,    32,
      30,    31,    32,    32,    30,    30,    36,    29,   117,   221,
      91,    33,    83,   122,    46,    34,    94,    29,    37,    38,
      42,   233,   103,    72,    43,    44,    37,    38,   240,   189,
      15,    16,    43,    44,   246,    42,    18,    42,    20,   110,
      22,    23,    24,    33,    29,   154,    91,   119,    30,   118,
      33,    46,    42,    38,    46,    15,    16,    33,   103,    42,
      45,    46,    47,    48,    49,    50,    42,    47,   139,    29,
      30,    33,   154,    46,    31,   157,    35,    29,    38,    36,
     151,   190,    32,    15,    16,    45,    46,    47,    48,    49,
      50,    31,    32,   164,    46,    47,    48,    49,   154,   154,
      30,    31,   184,    30,    31,   154,    38,    30,    31,   218,
      39,    40,    41,   184,   185,    47,    48,    49,    50,    32,
     191,    46,   231,   194,   195,    30,    31,   209,   237,    18,
      46,    20,    34,    22,    23,    24,    32,    33,   209,   221,
     211,    30,   251,    18,    29,    20,    33,    22,    23,    24,
     221,   233,     8,   224,    47,    48,    49,     7,   240,    31,
      32,    47,   233,     9,   246,    11,    12,    13,    42,   240,
      32,    17,    18,    19,    20,   246,    22,    23,    24,    25,
      26,    27,    28,    15,    16,    30,    31,    31,    32,    18,
      36,    20,    21,    22,    23,    24,    33,    29,    30,    28,
      46,    42,    32,    29,    35,    29,    38,    15,    16,    34,
      46,    46,    29,    45,    46,    47,    48,    49,    50,    29,
      32,    29,    30,    32,    29,    12,    46,    30,    32,    11,
      38,    15,    16,    32,    29,    14,    30,    45,    46,    47,
      48,    49,    50,    14,    32,    29,    31,    30,    18,    18,
     111,    29,   104,    93,    38,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    47,    48,    49,    50
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    52,    53,     0,    18,    20,    21,    22,    23,    24,
      28,    55,    60,    62,    66,   109,    46,   109,    54,    46,
      63,    68,    29,    46,    67,    55,    60,    62,    66,    29,
      33,    42,    69,    31,    32,    42,    30,    61,   109,    42,
      31,    32,    30,    61,    47,    15,    16,    29,    38,    45,
      46,    47,    48,    49,    50,    96,    98,    99,   100,   101,
     103,   105,   107,   110,    33,    46,    68,    35,    64,    32,
      58,    30,    31,    46,    68,    38,   110,    46,    32,    56,
      30,    34,    98,    29,    46,    47,    48,    49,    96,   100,
      29,    33,    97,     8,     7,     3,     4,     5,     6,    37,
      38,    43,    44,   102,   104,    39,    40,    41,   106,    47,
      42,    42,    65,    98,    70,    71,    32,    59,   109,    42,
      70,    32,    57,    30,    98,    29,    30,    98,   108,   101,
     103,    99,   100,   103,   105,   107,    34,    98,    64,    31,
      36,    35,    70,    46,    68,   110,    70,    30,    30,   108,
      30,    31,    34,    98,    72,    30,    98,     9,    11,    13,
      17,    19,    25,    26,    27,    36,    46,    62,    66,    70,
      73,    74,    76,    80,    84,    85,    94,    95,    96,   109,
      46,    96,    29,    29,    29,    75,    32,    32,    98,    29,
      12,    42,    46,    32,    81,    78,    90,    93,    96,    98,
      98,    32,    30,   108,    70,    98,    98,    79,    98,    31,
      32,    42,    32,    32,    30,    11,    32,    82,    30,    93,
      98,    86,    98,    32,    29,    30,    70,    91,    93,    98,
      98,    83,    77,    31,    32,    30,    70,    14,    93,    98,
      87,    32,    70,    92,    93,    98,    31,    88,    93,    98,
      30,    89,    70
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    52,    53,    53,    53,    53,    54,    54,    54,
      54,    54,    56,    55,    57,    55,    58,    55,    59,    55,
      60,    60,    60,    60,    61,    61,    61,    61,    62,    63,
      63,    63,    63,    63,    63,    63,    63,    64,    65,    65,
      65,    66,    67,    67,    68,    69,    69,    71,    70,    72,
      72,    72,    72,    73,    73,    73,    73,    73,    73,    73,
      74,    75,    74,    74,    76,    77,    76,    78,    79,    81,
      82,    83,    80,    84,    80,    86,    87,    88,    89,    85,
      90,    90,    90,    90,    90,    91,    91,    91,    91,    91,
      92,    92,    92,    92,    92,    93,    94,    94,    95,    95,
      95,    96,    96,    97,    98,    98,    99,    99,   100,   100,
     101,   101,   102,   102,   102,   102,   102,   102,   103,   103,
     103,   104,   104,   105,   105,   106,   106,   106,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   108,   108,   109,
     109,   109,   109,   109,   110,   110,   110,   110,   110,   110,
     110,   110,   110
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     2,     2,     2,     0,     2,     2,     2,
       2,     0,     0,     6,     0,     7,     0,     6,     0,     7,
       5,     6,     5,     6,     4,     4,     2,     2,     3,     3,
       5,     5,     3,     3,     1,     3,     1,     3,     3,     1,
       0,     4,     5,     3,     2,     4,     3,     0,     4,     2,
       2,     2,     0,     1,     1,     1,     1,     1,     1,     1,
       4,     0,     4,     3,     6,     0,     9,     0,     1,     0,
       0,     0,     8,     0,     8,     0,     0,     0,     0,    13,
       3,     3,     1,     1,     0,     3,     3,     1,     1,     0,
       3,     3,     1,     1,     0,     3,     5,     4,     2,     2,
       3,     1,     2,     3,     3,     1,     3,     1,     2,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     2,
       3,     4,     4,     5,     3,     4,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     2,     1,     2,
       1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 363 "parser.y" /* yacc.c:1646  */
    {
					if(Opt_Symbol == 1)
					printSymTable( symbolTable, scope );	
				}
#line 1847 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 385 "parser.y" /* yacc.c:1646  */
    {
				funcReturn = (yyvsp[-3].ptype); 
				struct SymNode *node;
				node = findFuncDeclaration( symbolTable, (yyvsp[-2].lexeme) );
				
				top = -1;
				if( node != 0 ){
					verifyFuncDeclaration( symbolTable, 0, (yyvsp[-3].ptype), node );
				}
				else{
					insertFuncIntoSymTable( symbolTable, (yyvsp[-2].lexeme), 0, (yyvsp[-3].ptype), scope, __TRUE );
				}
				gen_func1((yyvsp[-2].lexeme));
				if ( !strcmp("main",(yyvsp[-2].lexeme)) ){
					gen_funcMain();
				}
				else {
					gen_funcArg(NULL);
					gen_funcRet((yyvsp[-3].ptype));
				}
				
				gen_funct();
				if ( !strcmp("main",(yyvsp[-2].lexeme)) ) {
					gen_functMain();
				}
			}
#line 1878 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 411 "parser.y" /* yacc.c:1646  */
    { funcReturn = 0; gen_funcEnd((yyvsp[-5].ptype),(yyvsp[-4].lexeme));}
#line 1884 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 413 "parser.y" /* yacc.c:1646  */
    {				
				funcReturn = (yyvsp[-4].ptype);
				
				paramError = checkFuncParam( (yyvsp[-1].par) );
				int *ptr = NULL;
				if( paramError == __TRUE ){
					fprintf( stdout, "########## Error at Line#%d: param(s) with several fault!! ##########\n", linenum );
					semError = __TRUE;
				}
				// check and insert function into symbol table
				else{
					struct SymNode *node;
					node = findFuncDeclaration( symbolTable, (yyvsp[-3].lexeme) );

					top = -1;
					ptr = &top;
					if( node != 0 ){
						if(verifyFuncDeclaration( symbolTable, (yyvsp[-1].par), (yyvsp[-4].ptype), node ) == __TRUE){	
							insertParamIntoSymTable( symbolTable, (yyvsp[-1].par), scope+1 , ptr);
							
						}				
					}
					else{
						insertParamIntoSymTable( symbolTable, (yyvsp[-1].par), scope+1 , ptr);
						insertFuncIntoSymTable( symbolTable, (yyvsp[-3].lexeme), (yyvsp[-1].par), (yyvsp[-4].ptype), scope, __TRUE );
						
					}
					gen_func1((yyvsp[-3].lexeme));
					if ( !strcmp("main",(yyvsp[-3].lexeme)) ){
						gen_funcMain();
					}
					else {
						gen_funcArg((yyvsp[-1].par));
						gen_funcRet((yyvsp[-4].ptype));
					}
					
					gen_funct();
					if ( !strcmp("main",(yyvsp[-3].lexeme)) ) {
						gen_functMain();
					}
					// log(top);
				}
			}
#line 1932 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 456 "parser.y" /* yacc.c:1646  */
    { funcReturn = 0; gen_funcEnd((yyvsp[-6].ptype),(yyvsp[-5].lexeme)); }
#line 1938 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 458 "parser.y" /* yacc.c:1646  */
    {
				funcReturn = createPType(VOID_t); 
				struct SymNode *node;
				node = findFuncDeclaration( symbolTable, (yyvsp[-2].lexeme) );

				top = -1;
				if( node != 0 ){
					verifyFuncDeclaration( symbolTable, 0, createPType( VOID_t ), node );					
				}
				else{
					insertFuncIntoSymTable( symbolTable, (yyvsp[-2].lexeme), 0, createPType( VOID_t ), scope, __TRUE );	
					
				}
				gen_func1((yyvsp[-2].lexeme));
				if ( !strcmp("main",(yyvsp[-2].lexeme)) ){
					gen_funcMain();
				}
				else {
					gen_funcArg(NULL);
					gen_funcRet(funcReturn);
				}
				
				gen_funct();
				if ( !strcmp("main",(yyvsp[-2].lexeme)) ) {
					gen_functMain();
				}
			}
#line 1970 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 485 "parser.y" /* yacc.c:1646  */
    { funcReturn = 0; gen_funcEnd(NULL,(yyvsp[-4].lexeme)); }
#line 1976 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 487 "parser.y" /* yacc.c:1646  */
    {									
				funcReturn = createPType(VOID_t);
				
				paramError = checkFuncParam( (yyvsp[-1].par) );
				if( paramError == __TRUE ){
					fprintf( stdout, "########## Error at Line#%d: param(s) with several fault!! ##########\n", linenum );
					semError = __TRUE;
				}
				// check and insert function into symbol table
				else{
					struct SymNode *node;
					node = findFuncDeclaration( symbolTable, (yyvsp[-3].lexeme) );

					int *ptr = NULL;
					top = -1;
					ptr = &top;
					if( node != 0 ){
						if(verifyFuncDeclaration( symbolTable, (yyvsp[-1].par), createPType( VOID_t ), node ) == __TRUE){	
							insertParamIntoSymTable( symbolTable, (yyvsp[-1].par), scope+1 , ptr);
						}
					}
					else{
						insertParamIntoSymTable( symbolTable, (yyvsp[-1].par), scope+1 , ptr);
						insertFuncIntoSymTable( symbolTable, (yyvsp[-3].lexeme), (yyvsp[-1].par), createPType( VOID_t ), scope, __TRUE );
					}
					
				}
				gen_func1((yyvsp[-3].lexeme));
				if ( !strcmp("main",(yyvsp[-3].lexeme)) ){
					gen_funcMain();
				}
				else {
					gen_funcArg((yyvsp[-1].par));
					gen_funcRet(funcReturn);
				}
				
				gen_funct();
				if ( !strcmp("main",(yyvsp[-3].lexeme)) ) {
					gen_functMain();
				}
				log(top);
			}
#line 2023 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 529 "parser.y" /* yacc.c:1646  */
    { funcReturn = 0; gen_funcEnd(NULL,(yyvsp[-5].lexeme)); }
#line 2029 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 533 "parser.y" /* yacc.c:1646  */
    {
				insertFuncIntoSymTable( symbolTable, (yyvsp[-3].lexeme), 0, (yyvsp[-4].ptype), scope, __FALSE );	
			}
#line 2037 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 537 "parser.y" /* yacc.c:1646  */
    {
				paramError = checkFuncParam( (yyvsp[-2].par) );
				if( paramError == __TRUE ){
					fprintf( stdout, "########## Error at Line#%d: param(s) with several fault!! ##########\n", linenum );
					semError = __TRUE;
				}
				else {
					insertFuncIntoSymTable( symbolTable, (yyvsp[-4].lexeme), (yyvsp[-2].par), (yyvsp[-5].ptype), scope, __FALSE );
				}
			}
#line 2052 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 548 "parser.y" /* yacc.c:1646  */
    {				
				insertFuncIntoSymTable( symbolTable, (yyvsp[-3].lexeme), 0, createPType( VOID_t ), scope, __FALSE );
			}
#line 2060 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 552 "parser.y" /* yacc.c:1646  */
    {
				paramError = checkFuncParam( (yyvsp[-2].par) );
				if( paramError == __TRUE ){
					fprintf( stdout, "########## Error at Line#%d: param(s) with several fault!! ##########\n", linenum );
					semError = __TRUE;	
				}
				else {
					insertFuncIntoSymTable( symbolTable, (yyvsp[-4].lexeme), (yyvsp[-2].par), createPType( VOID_t ), scope, __FALSE );
				}
			}
#line 2075 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 565 "parser.y" /* yacc.c:1646  */
    {
				struct param_sem *ptr;
				ptr = createParam( createIdList( (yyvsp[0].lexeme) ), (yyvsp[-1].ptype) );
				param_sem_addParam( (yyvsp[-3].par), ptr );
				(yyval.par) = (yyvsp[-3].par);
			   }
#line 2086 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 572 "parser.y" /* yacc.c:1646  */
    {
				(yyvsp[0].par)->pType->type= (yyvsp[-1].ptype)->type;
				param_sem_addParam( (yyvsp[-3].par), (yyvsp[0].par) );
				(yyval.par) = (yyvsp[-3].par);
			   }
#line 2096 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 578 "parser.y" /* yacc.c:1646  */
    { 
				(yyvsp[0].par)->pType->type = (yyvsp[-1].ptype)->type;  
				(yyval.par) = (yyvsp[0].par);
			   }
#line 2105 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 582 "parser.y" /* yacc.c:1646  */
    { (yyval.par) = createParam( createIdList( (yyvsp[0].lexeme) ), (yyvsp[-1].ptype) ); }
#line 2111 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 586 "parser.y" /* yacc.c:1646  */
    {
				struct varDeclParam *ptr;
				struct SymNode *newNode;
				int sum = -1;
				memset(QQ,0,sizeof(QQ) );
				for( ptr=(yyvsp[-1].varDeclNode) ; ptr!=0 ; ptr=(ptr->next) ) {						
					if( verifyRedeclaration( symbolTable, ptr->para->idlist->value, scope ) == __FALSE ) { }
					else {
						if( verifyVarInitValue( (yyvsp[-2].ptype), ptr, symbolTable, scope ) ==  __TRUE ){	
							
							
							if ( scope == 0 ) {
								newNode = createVarNode( ptr->para->idlist->value, scope, ptr->para->pType ,top);
								insertTab( symbolTable, newNode );
								fprintf(java, ".field public static %s %c\n", ptr->para->idlist->value, types[(yyvsp[-2].ptype)->type] );
							}
							else {
								newNode = createVarNode( ptr->para->idlist->value, scope, ptr->para->pType ,++top);
								insertTab( symbolTable, newNode );
								if ( ptr->isInit ==  __TRUE) {
									
									struct SymNode *node = lookupSymbol( symbolTable, ptr->para->idlist->value, scope, __FALSE );
									QQ[++sum] = node->addr;
								}
							}
							
						}
					}
				}
				char tmp = types[(yyvsp[-2].ptype)->type];
				int i = 0;
				for ( i = sum ; i >= 0 ; --i ){
					if ( tmp == 'I' || tmp == 'Z' ){
						fprintf(java, "istore %d\n", QQ[i]);
					}
					else {
						fprintf(java, "fstore %d\n", QQ[i]);	
					}
				}
			}
#line 2156 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 629 "parser.y" /* yacc.c:1646  */
    {					
					struct param_sem *ptr;	
					struct varDeclParam *vptr;				
					ptr = createParam( createIdList( (yyvsp[0].lexeme) ), createPType( VOID_t ) );
					vptr = createVarDeclParam( ptr, 0 );	
					addVarDeclParam( (yyvsp[-2].varDeclNode), vptr );
					(yyval.varDeclNode) = (yyvsp[-2].varDeclNode); 					
				}
#line 2169 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 638 "parser.y" /* yacc.c:1646  */
    {
					struct param_sem *ptr;	
					struct varDeclParam *vptr;				
					ptr = createParam( createIdList( (yyvsp[-2].lexeme) ), createPType( VOID_t ) );
					vptr = createVarDeclParam( ptr, (yyvsp[0].exprs) );
					vptr->isArray = __TRUE;
					vptr->isInit = __TRUE;	
					addVarDeclParam( (yyvsp[-4].varDeclNode), vptr );	
					(yyval.varDeclNode) = (yyvsp[-4].varDeclNode);
					if ( scope ){

					}
					
				}
#line 2188 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 653 "parser.y" /* yacc.c:1646  */
    {
					struct varDeclParam *ptr;
					ptr = createVarDeclParam( (yyvsp[-2].par), (yyvsp[0].exprs) );
					ptr->isArray = __TRUE;
					ptr->isInit = __TRUE;
					addVarDeclParam( (yyvsp[-4].varDeclNode), ptr );
					(yyval.varDeclNode) = (yyvsp[-4].varDeclNode);	
				}
#line 2201 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 662 "parser.y" /* yacc.c:1646  */
    {
					struct varDeclParam *ptr;
					ptr = createVarDeclParam( (yyvsp[0].par), 0 );
					ptr->isArray = __TRUE;
					addVarDeclParam( (yyvsp[-2].varDeclNode), ptr );
					(yyval.varDeclNode) = (yyvsp[-2].varDeclNode);
				}
#line 2213 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 670 "parser.y" /* yacc.c:1646  */
    {	
					(yyval.varDeclNode) = createVarDeclParam( (yyvsp[-2].par) , (yyvsp[0].exprs) );
					(yyval.varDeclNode)->isArray = __TRUE;
					(yyval.varDeclNode)->isInit = __TRUE;	
				}
#line 2223 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 676 "parser.y" /* yacc.c:1646  */
    { 
					(yyval.varDeclNode) = createVarDeclParam( (yyvsp[0].par) , 0 ); 
					(yyval.varDeclNode)->isArray = __TRUE;
				}
#line 2232 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 681 "parser.y" /* yacc.c:1646  */
    {
					struct param_sem *ptr;					
					ptr = createParam( createIdList( (yyvsp[-2].lexeme) ), createPType( VOID_t ) );
					(yyval.varDeclNode) = createVarDeclParam( ptr, (yyvsp[0].exprs) );		
					(yyval.varDeclNode)->isInit = __TRUE;
				}
#line 2243 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 688 "parser.y" /* yacc.c:1646  */
    {
					struct param_sem *ptr;					
					ptr = createParam( createIdList( (yyvsp[0].lexeme) ), createPType( VOID_t ) );
					(yyval.varDeclNode) = createVarDeclParam( ptr, 0 );				
				}
#line 2253 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 695 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[-1].exprs); }
#line 2259 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 699 "parser.y" /* yacc.c:1646  */
    {
					struct expr_sem *ptr;
					for( ptr=(yyvsp[-2].exprs); (ptr->next)!=0; ptr=(ptr->next) );				
					ptr->next = (yyvsp[0].exprs);
					(yyval.exprs) = (yyvsp[-2].exprs);
				}
#line 2270 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 706 "parser.y" /* yacc.c:1646  */
    {
					(yyval.exprs) = (yyvsp[0].exprs);
				}
#line 2278 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 713 "parser.y" /* yacc.c:1646  */
    {
				struct SymNode *newNode;				
				struct constParam *ptr;
				for( ptr=(yyvsp[-1].constNode); ptr!=0; ptr=(ptr->next) ){
					if( verifyRedeclaration( symbolTable, ptr->name, scope ) == __TRUE ){//no redeclare
						if( ptr->value->category != (yyvsp[-2].ptype)->type ){//type different
							if( !(((yyvsp[-2].ptype)->type==FLOAT_t || (yyvsp[-2].ptype)->type == DOUBLE_t ) && ptr->value->category==INTEGER_t) ) {
								if(!((yyvsp[-2].ptype)->type==DOUBLE_t && ptr->value->category==FLOAT_t)){	
									fprintf( stdout, "########## Error at Line#%d: const type different!! ##########\n", linenum );
									semError = __TRUE;	
								}
								else{
									newNode = createConstNode( ptr->name, scope, (yyvsp[-2].ptype), ptr->value , ++top);
									insertTab( symbolTable, newNode );
								}
							}							
							else{
								newNode = createConstNode( ptr->name, scope, (yyvsp[-2].ptype), ptr->value , ++top);
								insertTab( symbolTable, newNode );
							}
						}
						else{
							newNode = createConstNode( ptr->name, scope, (yyvsp[-2].ptype), ptr->value, ++top );
							insertTab( symbolTable, newNode );
						}
					}
				}
			}
#line 2311 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 744 "parser.y" /* yacc.c:1646  */
    {				
				addConstParam( (yyvsp[-4].constNode), createConstParam( (yyvsp[0].constVal), (yyvsp[-2].lexeme) ) );
				(yyval.constNode) = (yyvsp[-4].constNode);
			}
#line 2320 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 749 "parser.y" /* yacc.c:1646  */
    {
				(yyval.constNode) = createConstParam( (yyvsp[0].constVal), (yyvsp[-2].lexeme) );	
			}
#line 2328 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 755 "parser.y" /* yacc.c:1646  */
    {
				(yyval.par) = createParam( createIdList( (yyvsp[-1].lexeme) ), (yyvsp[0].ptype) );
			}
#line 2336 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 761 "parser.y" /* yacc.c:1646  */
    {
			if( (yyvsp[-1].intVal) == 0 ){
				fprintf( stdout, "########## Error at Line#%d: array size error!! ##########\n", linenum );
				semError = __TRUE;
			}
			else
				increaseArrayDim( (yyvsp[-3].ptype), 0, (yyvsp[-1].intVal) );			
		}
#line 2349 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 770 "parser.y" /* yacc.c:1646  */
    {
			if( (yyvsp[-1].intVal) == 0 ){
				fprintf( stdout, "########## Error at Line#%d: array size error!! ##########\n", linenum );
				semError = __TRUE;
			}			
			else{
				(yyval.ptype) = createPType( VOID_t ); 			
				increaseArrayDim( (yyval.ptype), 0, (yyvsp[-1].intVal) );
			}		
		}
#line 2364 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 782 "parser.y" /* yacc.c:1646  */
    {scope++;}
#line 2370 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 783 "parser.y" /* yacc.c:1646  */
    { 
						// print contents of current scope
						if( Opt_Symbol == 1 )
							printSymTable( symbolTable, scope );
							
						deleteScope( symbolTable, scope );	// leave this scope, delete...
						scope--; 
					}
#line 2383 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 809 "parser.y" /* yacc.c:1646  */
    {
						// check if LHS exists
						__BOOLEAN flagLHS = verifyExistence( symbolTable, (yyvsp[-3].exprs), scope, __TRUE );
						// id RHS is not dereferenced, check and deference
						__BOOLEAN flagRHS = __TRUE;
						if( (yyvsp[-1].exprs)->isDeref == __FALSE ) {
							flagRHS = verifyExistence( symbolTable, (yyvsp[-1].exprs), scope, __FALSE );
						}
						// if both LHS and RHS are exists, verify their type
						if( flagLHS==__TRUE && flagRHS==__TRUE ){
							verifyAssignmentTypeMatch( (yyvsp[-3].exprs), (yyvsp[-1].exprs) );
							gen_store((yyvsp[-3].exprs));
							puts("!!");
						}
					}
#line 2403 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 824 "parser.y" /* yacc.c:1646  */
    { gen_print1(); }
#line 2409 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 824 "parser.y" /* yacc.c:1646  */
    { 
				 	    verifyScalarExpr( (yyvsp[-1].exprs), "print" ); 
				 	    
				 	    gen_print2((yyvsp[-1].exprs));
				   }
#line 2419 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 830 "parser.y" /* yacc.c:1646  */
    { 
						if( verifyExistence( symbolTable, (yyvsp[-1].exprs), scope, __TRUE ) == __TRUE ) {					
							verifyScalarExpr( (yyvsp[-1].exprs), "read" ); 
							struct SymNode *node = 0;
							node = lookupSymbol( symbolTable, (yyvsp[-1].exprs)->varRef->id, scope, __FALSE );	
							
							gen_read((yyvsp[-1].exprs), node->scope, node->addr);
						}
					}
#line 2433 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 841 "parser.y" /* yacc.c:1646  */
    { 
							char label[100];
							sprintf(label, "Lelse%d",stk[xd]);
							fprintf(java, "%s :\n", label);
							// fprintf(java, "%s :\n", Lelse);
							pop();
						 }
#line 2445 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 848 "parser.y" /* yacc.c:1646  */
    { 
							char label[100], exit[100];
							
							sprintf(label, "Lelse%d",stk[xd]);
							sprintf(exit, "Lexit%d",stk[xd]);
							fprintf(java, "goto %s\n", exit);
							fprintf(java, "%s :\n", label);
							// fprintf(java, "goto %s\n", Lexit);
							// fprintf(java, "%s :\n", Lelse);
						 }
#line 2460 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 858 "parser.y" /* yacc.c:1646  */
    {
							char exit[100];
							sprintf(exit, "Lexit%d",stk[xd]);
							fprintf(java, "%s :\n", exit);
							// fprintf(java, "%s :\n", Lexit);
							pop();
						}
#line 2472 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 867 "parser.y" /* yacc.c:1646  */
    { 
		push(++re0);
		// sprintf(Lelse, "Lelse%d",re0);
		// sprintf(Lexit, "Lexit%d",re0);

	 }
#line 2483 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 875 "parser.y" /* yacc.c:1646  */
    { 
					verifyBooleanExpr( (yyvsp[0].exprs), "if" ); 
					char label[100];
					sprintf(label, "Lelse%d",stk[xd]);
					fprintf(java, "ifeq %s\n", label);
					// fprintf(java, "ifeq %s\n", Lelse);
				}
#line 2495 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 884 "parser.y" /* yacc.c:1646  */
    { 						
						char tmp[100];
						push(++re0);
						sprintf(tmp, "Lbegin%d", stk[xd]);
						fprintf(java, "%s :\n", tmp);

					}
#line 2507 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 890 "parser.y" /* yacc.c:1646  */
    { 
						verifyBooleanExpr( (yyvsp[0].exprs), "while" ); 
						char tmp[100];
						sprintf(tmp, "Lexit%d", stk[xd]);
						fprintf(java, "ifeq %s\n", tmp);
					}
#line 2518 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 895 "parser.y" /* yacc.c:1646  */
    { inloop++; }
#line 2524 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 896 "parser.y" /* yacc.c:1646  */
    { 
						inloop--; 
						char tmp[100];
						sprintf(tmp, "Lbegin%d", stk[xd]);
						fprintf(java, "goto %s\n", tmp);

						sprintf(tmp, "Lexit%d", stk[xd]);
						fprintf(java, "%s :\n", tmp);
						pop();
					}
#line 2539 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 907 "parser.y" /* yacc.c:1646  */
    { 
						inloop++; 
						char tmp[100];
						push(++re0);
						sprintf(tmp, "Lbegin%d", stk[xd]);
						fprintf(java, "%s :\n", tmp);

				}
#line 2552 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 915 "parser.y" /* yacc.c:1646  */
    { 
						verifyBooleanExpr( (yyvsp[-2].exprs), "while" );
						inloop--; 
						char tmp[100];
						sprintf(tmp, "Lexit%d", stk[xd]);
						fprintf(java, "ifeq %s\n", tmp);

						sprintf(tmp, "Lbegin%d", stk[xd]);
						fprintf(java, "goto %s\n", tmp);

						sprintf(tmp, "Lexit%d", stk[xd]);
						fprintf(java, "%s :\n", tmp);
						pop();
					}
#line 2571 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 932 "parser.y" /* yacc.c:1646  */
    {
						char tmp[100];
						push(++re0);
						sprintf(tmp, "Lbegin%d", stk[xd]);
						fprintf(java, "%s :\n", tmp);
					}
#line 2582 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 937 "parser.y" /* yacc.c:1646  */
    {

						char tmp[100];
						sprintf(tmp, "Lexit%d", stk[xd]);
						fprintf(java, "ifeq %s\n", tmp);

						sprintf(tmp, "Lnext%d", stk[xd]);
						fprintf(java, "goto %s\n", tmp);

						sprintf(tmp, "Lincr%d", stk[xd]);
						fprintf(java, "%s :\n", tmp);
					}
#line 2599 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 949 "parser.y" /* yacc.c:1646  */
    {
						char tmp[100];
						sprintf(tmp, "Lbegin%d", stk[xd]);
						fprintf(java, "goto %s\n", tmp);
						
					}
#line 2610 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 954 "parser.y" /* yacc.c:1646  */
    { 
						inloop++; 
						char tmp[100];
						sprintf(tmp, "Lnext%d", stk[xd]);
						fprintf(java, "%s :\n", tmp);
					}
#line 2621 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 960 "parser.y" /* yacc.c:1646  */
    { 
						inloop--; 
						char tmp[100];
						sprintf(tmp, "Lincr%d", stk[xd]);
						fprintf(java, "goto %s\n", tmp);

						sprintf(tmp, "Lexit%d", stk[xd]);
						fprintf(java, "%s :\n", tmp);
						pop();
					}
#line 2636 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 980 "parser.y" /* yacc.c:1646  */
    {
						fprintf( stdout, "########## Error at Line#%d: control_expression is not boolean type ##########\n", linenum );
						semError = __TRUE;	
				   }
#line 2645 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 985 "parser.y" /* yacc.c:1646  */
    {
						if( (yyvsp[0].exprs)->pType->type != BOOLEAN_t ){
							fprintf( stdout, "########## Error at Line#%d: control_expression is not boolean type ##########\n", linenum );
							semError = __TRUE;	
						}
				   }
#line 2656 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 992 "parser.y" /* yacc.c:1646  */
    { 
						if( (yyvsp[0].exprs)->pType->type != BOOLEAN_t ){
							fprintf( stdout, "########## Error at Line#%d: control_expression is not boolean type ##########\n", linenum );
							semError = __TRUE;	
						}
					}
#line 2667 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 999 "parser.y" /* yacc.c:1646  */
    {
						fprintf( stdout, "########## Error at Line#%d: control_expression is not boolean type ##########\n", linenum );
						semError = __TRUE;	
				   }
#line 2676 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1014 "parser.y" /* yacc.c:1646  */
    {
						// check if LHS exists
						__BOOLEAN flagLHS = verifyExistence( symbolTable, (yyvsp[-2].exprs), scope, __TRUE );
						// id RHS is not dereferenced, check and deference
						__BOOLEAN flagRHS = __TRUE;
						if( (yyvsp[0].exprs)->isDeref == __FALSE ) {
							flagRHS = verifyExistence( symbolTable, (yyvsp[0].exprs), scope, __FALSE );
						}
						// if both LHS and RHS are exists, verify their type
						if( flagLHS==__TRUE && flagRHS==__TRUE ){
							verifyAssignmentTypeMatch( (yyvsp[-2].exprs), (yyvsp[0].exprs) );
							gen_store((yyvsp[-2].exprs));
						}
							
					}
#line 2696 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1033 "parser.y" /* yacc.c:1646  */
    {
								verifyFuncInvoke( (yyvsp[-4].lexeme), (yyvsp[-2].exprs), symbolTable, scope );
								gen_Invoke((yyvsp[-4].lexeme),(yyvsp[-2].exprs));
							}
#line 2705 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1038 "parser.y" /* yacc.c:1646  */
    {
								verifyFuncInvoke( (yyvsp[-3].lexeme), 0, symbolTable, scope );
								gen_Invoke((yyvsp[-3].lexeme),NULL);
							}
#line 2714 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1045 "parser.y" /* yacc.c:1646  */
    {
					if( inloop <= 0){
						fprintf( stdout, "########## Error at Line#%d: continue can't appear outside of loop ##########\n", linenum ); semError = __TRUE;
					}
				}
#line 2724 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1051 "parser.y" /* yacc.c:1646  */
    {
					if( inloop <= 0){
						fprintf( stdout, "########## Error at Line#%d: break can't appear outside of loop ##########\n", linenum ); semError = __TRUE;
					}
				}
#line 2734 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1057 "parser.y" /* yacc.c:1646  */
    {
					verifyReturnStatement( (yyvsp[-1].exprs), funcReturn );
					int tmp = funcReturn->type;
					if ( isMain ){
						// puts("1");
						fprintf(java, "return \n");
					}
					else if ( tmp == 1 ){
						fprintf(java, "ireturn \n");	
					}
					else if( tmp == 2 || tmp == 3 ) {
						fprintf(java, "freturn \n");
					}
					else {
						fprintf(java, "return \n");
					}
					
				}
#line 2757 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1078 "parser.y" /* yacc.c:1646  */
    {
						(yyval.exprs) = createExprSem( (yyvsp[0].lexeme) );
					}
#line 2765 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1082 "parser.y" /* yacc.c:1646  */
    {	
						increaseDim( (yyvsp[-1].exprs), (yyvsp[0].intVal) );
						(yyval.exprs) = (yyvsp[-1].exprs);
					}
#line 2774 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1089 "parser.y" /* yacc.c:1646  */
    {
				(yyval.intVal) = verifyArrayIndex( (yyvsp[-1].exprs) );
			}
#line 2782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1095 "parser.y" /* yacc.c:1646  */
    {
						verifyAndOrOp( (yyvsp[-2].exprs), OR_t, (yyvsp[0].exprs) );
						(yyval.exprs) = (yyvsp[-2].exprs);
						gen_expre((yyvsp[-2].exprs),(yyvsp[0].exprs),7);
					}
#line 2792 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1100 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs);  }
#line 2798 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1104 "parser.y" /* yacc.c:1646  */
    {
					verifyAndOrOp( (yyvsp[-2].exprs), AND_t, (yyvsp[0].exprs) );
					(yyval.exprs) = (yyvsp[-2].exprs);
					gen_expre((yyvsp[-2].exprs),(yyvsp[0].exprs),6);
				}
#line 2808 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1109 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 2814 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1113 "parser.y" /* yacc.c:1646  */
    {
					verifyUnaryNOT( (yyvsp[0].exprs) );
					(yyval.exprs) = (yyvsp[0].exprs);
					gen_expre((yyvsp[0].exprs),(yyvsp[0].exprs),8);
				}
#line 2824 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1118 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 2830 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1122 "parser.y" /* yacc.c:1646  */
    {
						verifyRelOp( (yyvsp[-2].exprs), (yyvsp[-1].intVal), (yyvsp[0].exprs) );
						(yyval.exprs) = (yyvsp[-2].exprs);
						push(++re0);
						gen_rel((yyvsp[-2].exprs),(yyvsp[0].exprs),(yyvsp[-1].intVal), stk[xd]);
						pop();
					}
#line 2842 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1129 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 2848 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1132 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = LT_t; }
#line 2854 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1133 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = LE_t; }
#line 2860 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1134 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = EQ_t; }
#line 2866 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1135 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = GE_t; }
#line 2872 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1136 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = GT_t; }
#line 2878 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 1137 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = NE_t; }
#line 2884 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1141 "parser.y" /* yacc.c:1646  */
    {
				verifyArithmeticOp( (yyvsp[-2].exprs), (yyvsp[-1].intVal), (yyvsp[0].exprs) );
				(yyval.exprs) = (yyvsp[-2].exprs);
				gen_expre((yyvsp[-2].exprs),(yyvsp[0].exprs),(yyvsp[-1].intVal));
			}
#line 2894 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1146 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 2900 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1147 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 2906 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1150 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = ADD_t; }
#line 2912 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1151 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = SUB_t; }
#line 2918 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1155 "parser.y" /* yacc.c:1646  */
    {
			if( (yyvsp[-1].intVal) == MOD_t ) {
				verifyModOp( (yyvsp[-2].exprs), (yyvsp[0].exprs) );
			}
			else {
				verifyArithmeticOp( (yyvsp[-2].exprs), (yyvsp[-1].intVal), (yyvsp[0].exprs) );
			}
			(yyval.exprs) = (yyvsp[-2].exprs);
			gen_expre((yyvsp[-2].exprs),(yyvsp[0].exprs),(yyvsp[-1].intVal));
		}
#line 2933 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1165 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 2939 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1168 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = MUL_t; }
#line 2945 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1169 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = DIV_t; }
#line 2951 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1170 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = MOD_t; }
#line 2957 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1174 "parser.y" /* yacc.c:1646  */
    {
			verifyExistence( symbolTable, (yyvsp[0].exprs), scope, __FALSE );
			(yyval.exprs) = (yyvsp[0].exprs);
			(yyval.exprs)->beginningOp = NONE_t;
			gen_load((yyvsp[0].exprs));
		}
#line 2968 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1181 "parser.y" /* yacc.c:1646  */
    {
			if( verifyExistence( symbolTable, (yyvsp[0].exprs), scope, __FALSE ) == __TRUE )
				verifyUnaryMinus( (yyvsp[0].exprs) );
			(yyval.exprs) = (yyvsp[0].exprs);
			(yyval.exprs)->beginningOp = SUB_t;
			gen_load((yyvsp[0].exprs));
			gen_expre((yyvsp[0].exprs),(yyvsp[0].exprs),5);
		}
#line 2981 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1190 "parser.y" /* yacc.c:1646  */
    {
			(yyvsp[-1].exprs)->beginningOp = NONE_t;
			(yyval.exprs) = (yyvsp[-1].exprs); 
			gen_load((yyvsp[-1].exprs));
		}
#line 2991 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 1196 "parser.y" /* yacc.c:1646  */
    {
			verifyUnaryMinus( (yyvsp[-1].exprs) );
			(yyval.exprs) = (yyvsp[-1].exprs);
			(yyval.exprs)->beginningOp = SUB_t;
			gen_load((yyvsp[-1].exprs));
			gen_expre((yyvsp[-1].exprs),(yyvsp[-1].exprs),5);
		}
#line 3003 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 1204 "parser.y" /* yacc.c:1646  */
    {
			(yyval.exprs) = verifyFuncInvoke( (yyvsp[-3].lexeme), (yyvsp[-1].exprs), symbolTable, scope );
			(yyval.exprs)->beginningOp = NONE_t;
			gen_Invoke((yyvsp[-3].lexeme),(yyvsp[-1].exprs));
			
		}
#line 3014 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 1211 "parser.y" /* yacc.c:1646  */
    {
			(yyval.exprs) = verifyFuncInvoke( (yyvsp[-3].lexeme), (yyvsp[-1].exprs), symbolTable, scope );
			(yyval.exprs)->beginningOp = SUB_t;
			int t = gen_Invoke((yyvsp[-3].lexeme),(yyvsp[-1].exprs));
			if ( t == 1 ){
				fprintf(java, "ineg\n");
			}
			else if ( t == 2 || t == 3 ){
				fprintf(java, "fneg\n");
			}

		}
#line 3031 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 1224 "parser.y" /* yacc.c:1646  */
    {
			(yyval.exprs) = verifyFuncInvoke( (yyvsp[-2].lexeme), 0, symbolTable, scope );
			(yyval.exprs)->beginningOp = NONE_t;
			gen_Invoke((yyvsp[-2].lexeme),NULL);
		}
#line 3041 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 1230 "parser.y" /* yacc.c:1646  */
    {
			(yyval.exprs) = verifyFuncInvoke( (yyvsp[-2].lexeme), 0, symbolTable, scope );
			(yyval.exprs)->beginningOp = SUB_OP;
			int t = gen_Invoke((yyvsp[-2].lexeme),NULL);
			if ( t == 1 ){
				fprintf(java, "ineg\n");
			}
			else if ( t == 2 || t == 3 ){
				fprintf(java, "fneg\n");
			}
		}
#line 3057 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 1242 "parser.y" /* yacc.c:1646  */
    {
			  (yyval.exprs) = (struct expr_sem *)malloc(sizeof(struct expr_sem));
			  (yyval.exprs)->isDeref = __TRUE;
			  (yyval.exprs)->varRef = 0;
			  (yyval.exprs)->pType = createPType( (yyvsp[0].constVal)->category );
			  (yyval.exprs)->next = 0;
			  if( (yyvsp[0].constVal)->hasMinus == __TRUE ) {
			  	(yyval.exprs)->beginningOp = SUB_t;
			  }
			  else {
				(yyval.exprs)->beginningOp = NONE_t;
			  }
			  gen_constVal((yyval.exprs));
		}
#line 3076 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 1259 "parser.y" /* yacc.c:1646  */
    {
			  				struct expr_sem *exprPtr;
			  				for( exprPtr=(yyvsp[-2].exprs) ; (exprPtr->next)!=0 ; exprPtr=(exprPtr->next) );
			  				exprPtr->next = (yyvsp[0].exprs);
			  				(yyval.exprs) = (yyvsp[-2].exprs);
						}
#line 3087 "y.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 1265 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 3093 "y.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 1271 "parser.y" /* yacc.c:1646  */
    { (yyval.ptype) = createPType( INTEGER_t ); }
#line 3099 "y.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 1272 "parser.y" /* yacc.c:1646  */
    { (yyval.ptype) = createPType( DOUBLE_t ); }
#line 3105 "y.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 1273 "parser.y" /* yacc.c:1646  */
    { (yyval.ptype) = createPType( STRING_t ); }
#line 3111 "y.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 1274 "parser.y" /* yacc.c:1646  */
    { (yyval.ptype) = createPType( BOOLEAN_t ); }
#line 3117 "y.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 1275 "parser.y" /* yacc.c:1646  */
    { (yyval.ptype) = createPType( FLOAT_t ); }
#line 3123 "y.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 1279 "parser.y" /* yacc.c:1646  */
    {
					int tmp = (yyvsp[0].intVal);
					(yyval.constVal) = createConstAttr( INTEGER_t, &tmp );
					ConstInt = tmp;
				}
#line 3133 "y.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 1285 "parser.y" /* yacc.c:1646  */
    {
					int tmp = -(yyvsp[0].intVal);
					(yyval.constVal) = createConstAttr( INTEGER_t, &tmp );
					ConstInt = tmp;
				}
#line 3143 "y.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 1291 "parser.y" /* yacc.c:1646  */
    {
					float tmp = (yyvsp[0].floatVal);
					(yyval.constVal) = createConstAttr( FLOAT_t, &tmp );
					ConstFloat = tmp;
				}
#line 3153 "y.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 1297 "parser.y" /* yacc.c:1646  */
    {
					float tmp = -(yyvsp[0].floatVal);
					(yyval.constVal) = createConstAttr( FLOAT_t, &tmp );
					ConstFloat = tmp;
				}
#line 3163 "y.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 1303 "parser.y" /* yacc.c:1646  */
    {
					double tmp = (yyvsp[0].floatVal);
					(yyval.constVal) = createConstAttr( DOUBLE_t, &tmp );
					ConstDouble = tmp;
				}
#line 3173 "y.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 1309 "parser.y" /* yacc.c:1646  */
    {
					double tmp = -(yyvsp[0].floatVal);
					(yyval.constVal) = createConstAttr( DOUBLE_t, &tmp );
					ConstDouble = tmp;
				}
#line 3183 "y.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 1315 "parser.y" /* yacc.c:1646  */
    {
					(yyval.constVal) = createConstAttr( STRING_t, (yyvsp[0].lexeme) );
					strcpy(ConstString, (yyval.constVal)->value.stringVal);
				}
#line 3192 "y.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 1320 "parser.y" /* yacc.c:1646  */
    {
					SEMTYPE tmp = __TRUE;
					(yyval.constVal) = createConstAttr( BOOLEAN_t, &tmp );
					ConstBool = 1;
				}
#line 3202 "y.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 1326 "parser.y" /* yacc.c:1646  */
    {
					SEMTYPE tmp = __FALSE;
					(yyval.constVal) = createConstAttr( BOOLEAN_t, &tmp );
					ConstBool = 0;
				}
#line 3212 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3216 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1332 "parser.y" /* yacc.c:1906  */


int yyerror( char *msg )
{
    fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
	fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
	fprintf( stderr, "|\n" );
	fprintf( stderr, "| Unmatched token: %s\n", yytext );
	fprintf( stderr, "|--------------------------------------------------------------------------\n" );
	exit(-1);
}


