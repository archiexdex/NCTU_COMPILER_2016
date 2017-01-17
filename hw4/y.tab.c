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

const char  types[] = { 'V', 'I', 'F', 'D' ,'Z', 'X'};
const char* reops[] = { "iflt", "ifle", "ifeq", "ifge", "ifgt", "ifne" };
char ConstString[100];
int ConstBool = 0;
int ConstInt = 0;
float ConstFloat = 0.0;
double ConstDouble = 0.0;
int top = -1;
int re0 = -1;
int lo  = -1;

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
void gen_rel(struct expr_sem *a, struct expr_sem *b, int mode) {
	int ta = a->pType->type, tb = b->pType->type;
	char label1[100], label2[100];
	if ( ( 1 < ta && ta < 4 )  || ( 1 < tb && tb < 4) ) {
		fprintf(java, "fcmpl\n");	
	}
	else {
		fprintf(java, "isub\n");
	}
	sprintf(label1, "L%d", re0);
	sprintf(label2, "LL%d", re0);
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
	fprintf(java, "return\n");
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



#line 354 "y.tab.c" /* yacc.c:339  */

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
#line 289 "parser.y" /* yacc.c:355  */

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

#line 508 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 525 "y.tab.c" /* yacc.c:358  */

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
#define YYLAST   303

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  61
/* YYNRULES -- Number of rules.  */
#define YYNRULES  153
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  257

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
       0,   325,   325,   334,   335,   336,   337,   341,   342,   343,
     344,   345,   350,   349,   377,   376,   421,   420,   449,   448,
     493,   497,   508,   512,   525,   532,   538,   543,   546,   572,
     581,   593,   602,   610,   616,   621,   628,   636,   639,   646,
     650,   653,   684,   689,   695,   701,   710,   723,   723,   734,
     735,   736,   737,   740,   741,   742,   743,   744,   745,   746,
     749,   764,   764,   769,   781,   781,   786,   786,   786,   799,
     807,   811,   816,   807,   827,   827,   851,   855,   867,   872,
     851,   889,   890,   891,   892,   893,   896,   901,   908,   915,
     920,   923,   924,   925,   926,   927,   930,   949,   954,   961,
     967,   973,   985,   989,   996,  1002,  1008,  1011,  1017,  1020,
    1026,  1029,  1035,  1038,  1039,  1040,  1041,  1042,  1043,  1046,
    1052,  1053,  1056,  1057,  1060,  1071,  1074,  1075,  1076,  1079,
    1086,  1095,  1101,  1109,  1116,  1129,  1135,  1147,  1164,  1171,
    1177,  1178,  1179,  1180,  1181,  1184,  1190,  1196,  1202,  1208,
    1214,  1220,  1225,  1231
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
  "$@8", "$@9", "conditional_if", "while_statement", "$@10", "$@11",
  "$@12", "$@13", "for_statement", "$@14", "$@15", "$@16", "$@17",
  "initial_expression", "control_expression", "increment_expression",
  "statement_for", "function_invoke_statement", "jump_statement",
  "variable_reference", "dimension", "logical_expression", "logical_term",
  "logical_factor", "relation_expression", "relation_operator",
  "arithmetic_expression", "add_op", "term", "mul_op", "factor",
  "logical_expression_list", "scalar_type", "literal_const", YY_NULLPTR
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

#define YYPACT_NINF -194

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-194)))

#define YYTABLE_NINF -111

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -194,    12,   180,  -194,  -194,  -194,   -24,  -194,  -194,  -194,
     253,  -194,  -194,  -194,  -194,   -17,     3,    -6,   180,   -12,
      67,     4,   213,    28,    86,  -194,  -194,  -194,  -194,   272,
      31,   236,    49,    39,  -194,    18,    56,    91,    79,    45,
     113,  -194,    70,    93,    98,  -194,  -194,   236,    27,   236,
     123,  -194,  -194,  -194,  -194,   129,   160,   167,  -194,   141,
      63,    64,  -194,  -194,   128,     0,   134,   236,  -194,  -194,
    -194,   151,   253,   158,  -194,    66,  -194,   142,  -194,  -194,
     162,  -194,    11,   236,   166,  -194,  -194,  -194,   129,  -194,
      81,   251,  -194,   236,   236,  -194,  -194,  -194,  -194,  -194,
    -194,  -194,  -194,   251,   251,  -194,  -194,  -194,   251,   163,
     236,    18,    53,   160,  -194,   177,  -194,  -194,   168,    45,
    -194,  -194,  -194,  -194,    15,   140,  -194,   160,   104,  -194,
      21,   167,  -194,    63,    64,  -194,  -194,   160,  -194,   236,
    -194,  -194,  -194,   158,  -194,  -194,  -194,  -194,  -194,   106,
    -194,   236,  -194,   160,   198,  -194,   160,   183,   201,   203,
     209,  -194,   207,   208,   236,  -194,   216,  -194,  -194,  -194,
    -194,  -194,  -194,  -194,   234,  -194,  -194,  -194,    29,   202,
    -194,   109,  -194,  -194,   236,   236,  -194,  -194,     2,   212,
    -194,   236,    44,  -194,   236,   236,   236,   112,  -194,    48,
     160,     6,  -194,   215,   120,   238,     7,   160,   223,   160,
     224,   236,  -194,   236,  -194,  -194,   231,   226,  -194,   239,
    -194,  -194,  -194,   160,   236,   160,  -194,   236,  -194,  -194,
    -194,   126,  -194,   160,    20,  -194,   242,   236,  -194,   232,
    -194,  -194,  -194,   160,   236,  -194,  -194,   237,  -194,   160,
     236,   240,  -194,   160,  -194,  -194,  -194
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       6,     0,     0,     1,   140,   143,     0,   144,   141,   142,
       0,    11,     5,     3,     4,     0,     0,     0,     2,    36,
       0,    34,     0,     0,     0,    10,     9,     7,     8,     0,
       0,     0,    44,     0,    28,     0,    16,     0,     0,     0,
       0,    41,    12,     0,     0,   152,   153,     0,     0,     0,
     102,   145,   147,   149,   151,   129,    35,   106,   108,   120,
     112,   121,   125,   137,     0,    29,    32,    40,    33,    22,
      47,    18,     0,    27,    26,     0,    43,     0,    20,    47,
      14,    46,     0,     0,   102,   146,   148,   150,   130,   109,
       0,     0,   103,     0,     0,   114,   118,   116,   115,   122,
     123,   113,   117,     0,     0,   126,   127,   128,     0,     0,
       0,     0,     0,    39,    17,     0,    23,    47,     0,     0,
      13,    21,    47,   131,     0,     0,   135,   139,     0,   120,
       0,   105,   107,   111,   119,   124,    45,    30,    31,     0,
      37,    52,    19,    24,    25,    42,    15,   132,   136,     0,
     133,     0,   104,    38,    47,   134,   138,     0,     0,     0,
       0,    61,     0,     0,     0,    48,   102,    50,    51,    53,
      49,    54,    55,    56,     0,    57,    58,    59,     0,     0,
     102,     0,    70,    64,    85,     0,    99,   100,     0,     0,
      47,     0,    36,    63,     0,     0,     0,     0,    84,   129,
      83,     0,   101,     0,     0,     0,     0,    71,     0,    69,
       0,     0,    76,     0,    62,    98,     0,     0,    60,     0,
      47,    47,    81,    82,    90,    96,    97,     0,    72,    65,
      67,     0,    89,    88,     0,    47,     0,     0,    77,     0,
      73,    47,    86,    87,    95,    75,    68,    78,    94,    93,
       0,     0,    91,    92,    79,    47,    80
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -194,  -194,  -194,  -194,   254,  -194,  -194,  -194,  -194,   260,
     250,   -16,  -194,   176,  -194,   -14,  -194,   -27,  -194,   -74,
    -194,  -194,  -194,  -194,  -194,  -194,  -194,  -194,  -194,    92,
    -194,  -194,  -194,  -194,  -194,  -194,  -194,  -194,  -194,  -194,
    -194,  -194,  -194,  -193,  -194,  -194,   -45,  -194,   -31,   200,
     -40,   -56,  -194,   -54,  -194,   187,  -194,   195,  -117,    -9,
     -32
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    18,    11,    79,   122,    70,   117,    12,
      37,    13,    20,    68,   112,    14,    24,    21,    32,   114,
     115,   154,   170,   171,   185,   172,   195,   196,   236,   208,
     173,   194,   219,   235,   174,   175,   224,   244,   251,   255,
     197,   231,   247,   198,   176,   177,    55,    92,   127,    57,
      58,    59,   103,    60,   104,    61,   108,    62,   128,    15,
      63
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      56,    17,    27,    88,    28,   120,    66,    76,   149,    89,
      93,    74,     3,    38,    93,    93,    82,    29,   222,    93,
      38,    30,    16,    93,    95,    96,    97,    98,    93,    19,
      31,   232,    22,    30,   202,   129,   113,   130,   214,   218,
      23,   123,   110,   142,   242,   147,    35,   129,   146,   133,
     239,   248,   124,    67,   132,   152,    83,   252,    99,   100,
      45,    46,    91,   118,   101,   102,    95,    96,    97,    98,
      39,   191,   204,    84,    85,    86,    87,    30,    44,   137,
     169,    91,    64,    75,   139,    65,    31,   145,    69,   140,
     213,   144,    51,    52,    53,    54,    45,    46,    33,    34,
      99,   100,    78,   105,   106,   107,   101,   102,   153,   178,
      47,   126,   181,    85,    86,    87,   205,    40,    41,    48,
     156,    71,    72,    80,    72,    73,    49,    50,    51,    52,
      53,    54,    81,   188,   150,   151,   155,   151,   167,   199,
     168,   193,    91,   211,   212,   179,   229,   230,  -110,  -110,
     216,   151,    90,   200,   201,    45,    46,   237,   238,    77,
     206,   240,    91,   207,   209,   209,   199,   246,    93,    47,
     148,  -110,  -110,  -110,    94,   109,   111,  -110,    48,   199,
     223,   256,   225,   116,   119,    49,    50,    51,    52,    53,
      54,    30,   199,   233,   121,   125,   234,   136,     4,   199,
       5,     6,     7,     8,     9,   199,   243,   157,    10,   158,
     -74,   159,   141,   249,   143,   160,     4,   161,     5,   253,
       7,     8,     9,   162,   163,   164,    10,    45,    46,   180,
     182,     4,   183,     5,   165,     7,     8,     9,   184,   186,
     187,    47,   203,    36,   166,   189,   190,   215,   192,   217,
      48,    45,    46,   220,   221,   227,   241,    49,    50,    51,
      52,    53,    54,   226,   245,    47,    45,    46,   250,   228,
     254,     4,    25,     5,    48,     7,     8,     9,    26,    43,
      47,    49,    50,    51,    52,    53,    54,   138,   210,    48,
       4,   134,     5,   131,     7,     8,     9,    50,    51,    52,
      53,    54,    42,   135
};

static const yytype_uint8 yycheck[] =
{
      31,    10,    18,    48,    18,    79,    33,    39,   125,    49,
       8,    38,     0,    22,     8,     8,    47,    29,   211,     8,
      29,    33,    46,     8,     3,     4,     5,     6,     8,    46,
      42,   224,    29,    33,    32,    91,    67,    91,    32,    32,
      46,    30,    42,   117,   237,    30,    42,   103,   122,   103,
      30,   244,    83,    35,    94,    34,    29,   250,    37,    38,
      15,    16,    33,    72,    43,    44,     3,     4,     5,     6,
      42,    42,   189,    46,    47,    48,    49,    33,    47,   110,
     154,    33,    33,    38,    31,    46,    42,   119,    32,    36,
      42,   118,    47,    48,    49,    50,    15,    16,    31,    32,
      37,    38,    32,    39,    40,    41,    43,    44,   139,   154,
      29,    30,   157,    47,    48,    49,   190,    31,    32,    38,
     151,    30,    31,    30,    31,    46,    45,    46,    47,    48,
      49,    50,    34,   164,    30,    31,    30,    31,   154,   184,
     154,    32,    33,    31,    32,   154,   220,   221,     7,     8,
      30,    31,    29,   184,   185,    15,    16,    31,    32,    46,
     191,   235,    33,   194,   195,   196,   211,   241,     8,    29,
      30,    30,    31,    32,     7,    47,    42,    36,    38,   224,
     211,   255,   213,    32,    42,    45,    46,    47,    48,    49,
      50,    33,   237,   224,    32,    29,   227,    34,    18,   244,
      20,    21,    22,    23,    24,   250,   237,     9,    28,    11,
      12,    13,    35,   244,    46,    17,    18,    19,    20,   250,
      22,    23,    24,    25,    26,    27,    28,    15,    16,    46,
      29,    18,    29,    20,    36,    22,    23,    24,    29,    32,
      32,    29,    30,    30,    46,    29,    12,    32,    46,    11,
      38,    15,    16,    30,    30,    29,    14,    45,    46,    47,
      48,    49,    50,    32,    32,    29,    15,    16,    31,    30,
      30,    18,    18,    20,    38,    22,    23,    24,    18,    29,
      29,    45,    46,    47,    48,    49,    50,   111,   196,    38,
      18,   104,    20,    93,    22,    23,    24,    46,    47,    48,
      49,    50,    30,   108
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    52,    53,     0,    18,    20,    21,    22,    23,    24,
      28,    55,    60,    62,    66,   110,    46,   110,    54,    46,
      63,    68,    29,    46,    67,    55,    60,    62,    66,    29,
      33,    42,    69,    31,    32,    42,    30,    61,   110,    42,
      31,    32,    30,    61,    47,    15,    16,    29,    38,    45,
      46,    47,    48,    49,    50,    97,    99,   100,   101,   102,
     104,   106,   108,   111,    33,    46,    68,    35,    64,    32,
      58,    30,    31,    46,    68,    38,   111,    46,    32,    56,
      30,    34,    99,    29,    46,    47,    48,    49,    97,   101,
      29,    33,    98,     8,     7,     3,     4,     5,     6,    37,
      38,    43,    44,   103,   105,    39,    40,    41,   107,    47,
      42,    42,    65,    99,    70,    71,    32,    59,   110,    42,
      70,    32,    57,    30,    99,    29,    30,    99,   109,   102,
     104,   100,   101,   104,   106,   108,    34,    99,    64,    31,
      36,    35,    70,    46,    68,   111,    70,    30,    30,   109,
      30,    31,    34,    99,    72,    30,    99,     9,    11,    13,
      17,    19,    25,    26,    27,    36,    46,    62,    66,    70,
      73,    74,    76,    81,    85,    86,    95,    96,    97,   110,
      46,    97,    29,    29,    29,    75,    32,    32,    99,    29,
      12,    42,    46,    32,    82,    77,    78,    91,    94,    97,
      99,    99,    32,    30,   109,    70,    99,    99,    80,    99,
      80,    31,    32,    42,    32,    32,    30,    11,    32,    83,
      30,    30,    94,    99,    87,    99,    32,    29,    30,    70,
      70,    92,    94,    99,    99,    84,    79,    31,    32,    30,
      70,    14,    94,    99,    88,    32,    70,    93,    94,    99,
      31,    89,    94,    99,    30,    90,    70
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
      74,    75,    74,    74,    77,    76,    78,    79,    76,    80,
      82,    83,    84,    81,    85,    81,    87,    88,    89,    90,
      86,    91,    91,    91,    91,    91,    92,    92,    92,    92,
      92,    93,    93,    93,    93,    93,    94,    95,    95,    96,
      96,    96,    97,    97,    98,    99,    99,   100,   100,   101,
     101,   102,   102,   103,   103,   103,   103,   103,   103,   104,
     104,   104,   105,   105,   106,   106,   107,   107,   107,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   109,   109,
     110,   110,   110,   110,   110,   111,   111,   111,   111,   111,
     111,   111,   111,   111
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
       4,     0,     4,     3,     0,     6,     0,     0,     9,     1,
       0,     0,     0,     8,     0,     8,     0,     0,     0,     0,
      13,     3,     3,     1,     1,     0,     3,     3,     1,     1,
       0,     3,     3,     1,     1,     0,     3,     5,     4,     2,
       2,     3,     1,     2,     3,     3,     1,     3,     1,     2,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       2,     3,     4,     4,     5,     3,     4,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     1,
       2,     1,     1,     1
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
#line 328 "parser.y" /* yacc.c:1646  */
    {
					if(Opt_Symbol == 1)
					printSymTable( symbolTable, scope );	
				}
#line 1814 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 350 "parser.y" /* yacc.c:1646  */
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
				}
				gen_funcRet((yyvsp[-3].ptype));
				gen_funct();
				if ( !strcmp("main",(yyvsp[-2].lexeme)) ) {
					gen_functMain();
				}
			}
#line 1844 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 375 "parser.y" /* yacc.c:1646  */
    { funcReturn = 0; gen_funcEnd();}
#line 1850 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 377 "parser.y" /* yacc.c:1646  */
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
					}
					gen_funcRet((yyvsp[-4].ptype));
					gen_funct();
					if ( !strcmp("main",(yyvsp[-3].lexeme)) ) {
						gen_functMain();
					}
					log(top);
				}
			}
#line 1897 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 419 "parser.y" /* yacc.c:1646  */
    { funcReturn = 0; gen_funcEnd(); }
#line 1903 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 421 "parser.y" /* yacc.c:1646  */
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
				}
				gen_funcRet(funcReturn);
				gen_funct();
				if ( !strcmp("main",(yyvsp[-2].lexeme)) ) {
					gen_functMain();
				}
			}
#line 1934 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 447 "parser.y" /* yacc.c:1646  */
    { funcReturn = 0; gen_funcEnd(); }
#line 1940 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 449 "parser.y" /* yacc.c:1646  */
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
				}
				gen_funcRet(funcReturn);
				gen_funct();
				if ( !strcmp("main",(yyvsp[-3].lexeme)) ) {
					gen_functMain();
				}
				log(top);
			}
#line 1986 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 490 "parser.y" /* yacc.c:1646  */
    { funcReturn = 0; gen_funcEnd(); }
#line 1992 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 494 "parser.y" /* yacc.c:1646  */
    {
				insertFuncIntoSymTable( symbolTable, (yyvsp[-3].lexeme), 0, (yyvsp[-4].ptype), scope, __FALSE );	
			}
#line 2000 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 498 "parser.y" /* yacc.c:1646  */
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
#line 2015 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 509 "parser.y" /* yacc.c:1646  */
    {				
				insertFuncIntoSymTable( symbolTable, (yyvsp[-3].lexeme), 0, createPType( VOID_t ), scope, __FALSE );
			}
#line 2023 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 513 "parser.y" /* yacc.c:1646  */
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
#line 2038 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 526 "parser.y" /* yacc.c:1646  */
    {
				struct param_sem *ptr;
				ptr = createParam( createIdList( (yyvsp[0].lexeme) ), (yyvsp[-1].ptype) );
				param_sem_addParam( (yyvsp[-3].par), ptr );
				(yyval.par) = (yyvsp[-3].par);
			   }
#line 2049 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 533 "parser.y" /* yacc.c:1646  */
    {
				(yyvsp[0].par)->pType->type= (yyvsp[-1].ptype)->type;
				param_sem_addParam( (yyvsp[-3].par), (yyvsp[0].par) );
				(yyval.par) = (yyvsp[-3].par);
			   }
#line 2059 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 539 "parser.y" /* yacc.c:1646  */
    { 
				(yyvsp[0].par)->pType->type = (yyvsp[-1].ptype)->type;  
				(yyval.par) = (yyvsp[0].par);
			   }
#line 2068 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 543 "parser.y" /* yacc.c:1646  */
    { (yyval.par) = createParam( createIdList( (yyvsp[0].lexeme) ), (yyvsp[-1].ptype) ); }
#line 2074 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 547 "parser.y" /* yacc.c:1646  */
    {
				struct varDeclParam *ptr;
				struct SymNode *newNode;
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
							}
							log(top);
						}
					}
				}
			}
#line 2102 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 573 "parser.y" /* yacc.c:1646  */
    {					
					struct param_sem *ptr;	
					struct varDeclParam *vptr;				
					ptr = createParam( createIdList( (yyvsp[0].lexeme) ), createPType( VOID_t ) );
					vptr = createVarDeclParam( ptr, 0 );	
					addVarDeclParam( (yyvsp[-2].varDeclNode), vptr );
					(yyval.varDeclNode) = (yyvsp[-2].varDeclNode); 					
				}
#line 2115 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 582 "parser.y" /* yacc.c:1646  */
    {
					struct param_sem *ptr;	
					struct varDeclParam *vptr;				
					ptr = createParam( createIdList( (yyvsp[-2].lexeme) ), createPType( VOID_t ) );
					vptr = createVarDeclParam( ptr, (yyvsp[0].exprs) );
					vptr->isArray = __TRUE;
					vptr->isInit = __TRUE;	
					addVarDeclParam( (yyvsp[-4].varDeclNode), vptr );	
					(yyval.varDeclNode) = (yyvsp[-4].varDeclNode);
					
				}
#line 2131 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 594 "parser.y" /* yacc.c:1646  */
    {
					struct varDeclParam *ptr;
					ptr = createVarDeclParam( (yyvsp[-2].par), (yyvsp[0].exprs) );
					ptr->isArray = __TRUE;
					ptr->isInit = __TRUE;
					addVarDeclParam( (yyvsp[-4].varDeclNode), ptr );
					(yyval.varDeclNode) = (yyvsp[-4].varDeclNode);	
				}
#line 2144 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 603 "parser.y" /* yacc.c:1646  */
    {
					struct varDeclParam *ptr;
					ptr = createVarDeclParam( (yyvsp[0].par), 0 );
					ptr->isArray = __TRUE;
					addVarDeclParam( (yyvsp[-2].varDeclNode), ptr );
					(yyval.varDeclNode) = (yyvsp[-2].varDeclNode);
				}
#line 2156 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 611 "parser.y" /* yacc.c:1646  */
    {	
					(yyval.varDeclNode) = createVarDeclParam( (yyvsp[-2].par) , (yyvsp[0].exprs) );
					(yyval.varDeclNode)->isArray = __TRUE;
					(yyval.varDeclNode)->isInit = __TRUE;	
				}
#line 2166 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 617 "parser.y" /* yacc.c:1646  */
    { 
					(yyval.varDeclNode) = createVarDeclParam( (yyvsp[0].par) , 0 ); 
					(yyval.varDeclNode)->isArray = __TRUE;
				}
#line 2175 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 622 "parser.y" /* yacc.c:1646  */
    {
					struct param_sem *ptr;					
					ptr = createParam( createIdList( (yyvsp[-2].lexeme) ), createPType( VOID_t ) );
					(yyval.varDeclNode) = createVarDeclParam( ptr, (yyvsp[0].exprs) );		
					(yyval.varDeclNode)->isInit = __TRUE;
				}
#line 2186 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 629 "parser.y" /* yacc.c:1646  */
    {
					struct param_sem *ptr;					
					ptr = createParam( createIdList( (yyvsp[0].lexeme) ), createPType( VOID_t ) );
					(yyval.varDeclNode) = createVarDeclParam( ptr, 0 );				
				}
#line 2196 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 636 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[-1].exprs); }
#line 2202 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 640 "parser.y" /* yacc.c:1646  */
    {
					struct expr_sem *ptr;
					for( ptr=(yyvsp[-2].exprs); (ptr->next)!=0; ptr=(ptr->next) );				
					ptr->next = (yyvsp[0].exprs);
					(yyval.exprs) = (yyvsp[-2].exprs);
				}
#line 2213 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 647 "parser.y" /* yacc.c:1646  */
    {
					(yyval.exprs) = (yyvsp[0].exprs);
				}
#line 2221 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 654 "parser.y" /* yacc.c:1646  */
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
#line 2254 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 685 "parser.y" /* yacc.c:1646  */
    {				
				addConstParam( (yyvsp[-4].constNode), createConstParam( (yyvsp[0].constVal), (yyvsp[-2].lexeme) ) );
				(yyval.constNode) = (yyvsp[-4].constNode);
			}
#line 2263 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 690 "parser.y" /* yacc.c:1646  */
    {
				(yyval.constNode) = createConstParam( (yyvsp[0].constVal), (yyvsp[-2].lexeme) );	
			}
#line 2271 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 696 "parser.y" /* yacc.c:1646  */
    {
				(yyval.par) = createParam( createIdList( (yyvsp[-1].lexeme) ), (yyvsp[0].ptype) );
			}
#line 2279 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 702 "parser.y" /* yacc.c:1646  */
    {
			if( (yyvsp[-1].intVal) == 0 ){
				fprintf( stdout, "########## Error at Line#%d: array size error!! ##########\n", linenum );
				semError = __TRUE;
			}
			else
				increaseArrayDim( (yyvsp[-3].ptype), 0, (yyvsp[-1].intVal) );			
		}
#line 2292 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 711 "parser.y" /* yacc.c:1646  */
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
#line 2307 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 723 "parser.y" /* yacc.c:1646  */
    {scope++;}
#line 2313 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 724 "parser.y" /* yacc.c:1646  */
    { 
						// print contents of current scope
						if( Opt_Symbol == 1 )
							printSymTable( symbolTable, scope );
							
						deleteScope( symbolTable, scope );	// leave this scope, delete...
						scope--; 
					}
#line 2326 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 750 "parser.y" /* yacc.c:1646  */
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
						}
					}
#line 2345 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 764 "parser.y" /* yacc.c:1646  */
    { gen_print1(); }
#line 2351 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 764 "parser.y" /* yacc.c:1646  */
    { 
				 	    verifyScalarExpr( (yyvsp[-1].exprs), "print" ); 
				 	    
				 	    gen_print2((yyvsp[-1].exprs));
				   }
#line 2361 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 770 "parser.y" /* yacc.c:1646  */
    { 
						if( verifyExistence( symbolTable, (yyvsp[-1].exprs), scope, __TRUE ) == __TRUE ) {					
							verifyScalarExpr( (yyvsp[-1].exprs), "read" ); 
							struct SymNode *node = 0;
							node = lookupSymbol( symbolTable, (yyvsp[-1].exprs)->varRef->id, scope, __FALSE );	
							
							gen_read((yyvsp[-1].exprs), node->scope, node->addr);
						}
					}
#line 2375 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 781 "parser.y" /* yacc.c:1646  */
    { ++re0; }
#line 2381 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 781 "parser.y" /* yacc.c:1646  */
    { 
							char label[100];
							sprintf(label, "Lelse%d",re0);
							fprintf(java, "%s :\n", label);
						 }
#line 2391 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 786 "parser.y" /* yacc.c:1646  */
    { ++re0; }
#line 2397 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 786 "parser.y" /* yacc.c:1646  */
    { 
							char label[100], exit[100];
							sprintf(label, "Lelse%d",re0);
							sprintf(exit, "Lexit%d",re0);
							fprintf(java, "goto %s\n", exit);
							fprintf(java, "%s :\n", label);
						 }
#line 2409 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 793 "parser.y" /* yacc.c:1646  */
    {
							char exit[100];
							sprintf(exit, "Lexit%d",re0);
							fprintf(java, "%s :\n", exit);
						}
#line 2419 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 799 "parser.y" /* yacc.c:1646  */
    { 
					verifyBooleanExpr( (yyvsp[0].exprs), "if" ); 
					char label[100];
					sprintf(label, "Lelse%d",re0);
					fprintf(java, "ifeq %s\n", label);
				}
#line 2430 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 807 "parser.y" /* yacc.c:1646  */
    { 						
						char tmp[100];
						sprintf(tmp, "Lbegin%d", ++re0);
						fprintf(java, "%s :\n", tmp);
					}
#line 2440 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 811 "parser.y" /* yacc.c:1646  */
    { 
						verifyBooleanExpr( (yyvsp[0].exprs), "while" ); 
						char tmp[100];
						sprintf(tmp, "Lexit%d", re0);
						fprintf(java, "ifeq %s\n", tmp);
					}
#line 2451 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 816 "parser.y" /* yacc.c:1646  */
    { inloop++; }
#line 2457 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 817 "parser.y" /* yacc.c:1646  */
    { 
						inloop--; 
						char tmp[100];
						sprintf(tmp, "Lbegin%d", re0);
						fprintf(java, "goto %s\n", tmp);

						sprintf(tmp, "Lexit%d", re0);
						fprintf(java, "%s :\n", tmp);
					}
#line 2471 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 827 "parser.y" /* yacc.c:1646  */
    { 
						inloop++; 
						char tmp[100];
						sprintf(tmp, "Lbegin%d", ++re0);
						fprintf(java, "%s :\n", tmp);

				}
#line 2483 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 834 "parser.y" /* yacc.c:1646  */
    { 
						verifyBooleanExpr( (yyvsp[-2].exprs), "while" );
						inloop--; 
						char tmp[100];
						sprintf(tmp, "Lexit%d", re0);
						fprintf(java, "ifeq %s\n", tmp);

						sprintf(tmp, "Lbegin%d", re0);
						fprintf(java, "goto %s\n", tmp);

						sprintf(tmp, "Lexit%d", re0);
						fprintf(java, "%s :\n", tmp);
					}
#line 2501 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 851 "parser.y" /* yacc.c:1646  */
    {
						char tmp[100];
						sprintf(tmp, "Lbegin%d", ++re0);
						fprintf(java, "%s :\n", tmp);
					}
#line 2511 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 855 "parser.y" /* yacc.c:1646  */
    {

						char tmp[100];
						sprintf(tmp, "Lexit%d", re0);
						fprintf(java, "ifeq %s\n", tmp);

						sprintf(tmp, "Lnext%d", re0);
						fprintf(java, "goto %s\n", tmp);

						sprintf(tmp, "Lincr%d", re0);
						fprintf(java, "%s :\n", tmp);
					}
#line 2528 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 867 "parser.y" /* yacc.c:1646  */
    {
						char tmp[100];
						sprintf(tmp, "Lbegin%d", re0);
						fprintf(java, "goto %s\n", tmp);
						
					}
#line 2539 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 872 "parser.y" /* yacc.c:1646  */
    { 
						inloop++; 
						char tmp[100];
						sprintf(tmp, "Lnext%d", re0);
						fprintf(java, "%s :\n", tmp);
					}
#line 2550 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 878 "parser.y" /* yacc.c:1646  */
    { 
						inloop--; 
						char tmp[100];
						sprintf(tmp, "Lincr%d", re0);
						fprintf(java, "goto %s\n", tmp);

						sprintf(tmp, "Lexit%d", re0);
						fprintf(java, "%s :\n", tmp);
					}
#line 2564 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 897 "parser.y" /* yacc.c:1646  */
    {
						fprintf( stdout, "########## Error at Line#%d: control_expression is not boolean type ##########\n", linenum );
						semError = __TRUE;	
				   }
#line 2573 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 902 "parser.y" /* yacc.c:1646  */
    {
						if( (yyvsp[0].exprs)->pType->type != BOOLEAN_t ){
							fprintf( stdout, "########## Error at Line#%d: control_expression is not boolean type ##########\n", linenum );
							semError = __TRUE;	
						}
				   }
#line 2584 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 909 "parser.y" /* yacc.c:1646  */
    { 
						if( (yyvsp[0].exprs)->pType->type != BOOLEAN_t ){
							fprintf( stdout, "########## Error at Line#%d: control_expression is not boolean type ##########\n", linenum );
							semError = __TRUE;	
						}
					}
#line 2595 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 916 "parser.y" /* yacc.c:1646  */
    {
						fprintf( stdout, "########## Error at Line#%d: control_expression is not boolean type ##########\n", linenum );
						semError = __TRUE;	
				   }
#line 2604 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 931 "parser.y" /* yacc.c:1646  */
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
#line 2624 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 950 "parser.y" /* yacc.c:1646  */
    {
								verifyFuncInvoke( (yyvsp[-4].lexeme), (yyvsp[-2].exprs), symbolTable, scope );
								gen_Invoke((yyvsp[-4].lexeme),(yyvsp[-2].exprs));
							}
#line 2633 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 955 "parser.y" /* yacc.c:1646  */
    {
								verifyFuncInvoke( (yyvsp[-3].lexeme), 0, symbolTable, scope );
								gen_Invoke((yyvsp[-3].lexeme),NULL);
							}
#line 2642 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 962 "parser.y" /* yacc.c:1646  */
    {
					if( inloop <= 0){
						fprintf( stdout, "########## Error at Line#%d: continue can't appear outside of loop ##########\n", linenum ); semError = __TRUE;
					}
				}
#line 2652 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 968 "parser.y" /* yacc.c:1646  */
    {
					if( inloop <= 0){
						fprintf( stdout, "########## Error at Line#%d: break can't appear outside of loop ##########\n", linenum ); semError = __TRUE;
					}
				}
#line 2662 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 974 "parser.y" /* yacc.c:1646  */
    {
					verifyReturnStatement( (yyvsp[-1].exprs), funcReturn );
					if ( (yyvsp[-1].exprs)->pType->type == 1 ){
						fprintf(java, "ireturn\n");
					}
					else if( (yyvsp[-1].exprs)->pType->type == 2 || (yyvsp[-1].exprs)->pType->type == 3 ){
						fprintf(java, "freturn\n");
					}
				}
#line 2676 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 986 "parser.y" /* yacc.c:1646  */
    {
						(yyval.exprs) = createExprSem( (yyvsp[0].lexeme) );
					}
#line 2684 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 990 "parser.y" /* yacc.c:1646  */
    {	
						increaseDim( (yyvsp[-1].exprs), (yyvsp[0].intVal) );
						(yyval.exprs) = (yyvsp[-1].exprs);
					}
#line 2693 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 997 "parser.y" /* yacc.c:1646  */
    {
				(yyval.intVal) = verifyArrayIndex( (yyvsp[-1].exprs) );
			}
#line 2701 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1003 "parser.y" /* yacc.c:1646  */
    {
						verifyAndOrOp( (yyvsp[-2].exprs), OR_t, (yyvsp[0].exprs) );
						(yyval.exprs) = (yyvsp[-2].exprs);
						gen_expre((yyvsp[-2].exprs),(yyvsp[0].exprs),7);
					}
#line 2711 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1008 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs);  }
#line 2717 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1012 "parser.y" /* yacc.c:1646  */
    {
					verifyAndOrOp( (yyvsp[-2].exprs), AND_t, (yyvsp[0].exprs) );
					(yyval.exprs) = (yyvsp[-2].exprs);
					gen_expre((yyvsp[-2].exprs),(yyvsp[0].exprs),6);
				}
#line 2727 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1017 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 2733 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1021 "parser.y" /* yacc.c:1646  */
    {
					verifyUnaryNOT( (yyvsp[0].exprs) );
					(yyval.exprs) = (yyvsp[0].exprs);
					gen_expre((yyvsp[0].exprs),(yyvsp[0].exprs),8);
				}
#line 2743 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1026 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 2749 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1030 "parser.y" /* yacc.c:1646  */
    {
						verifyRelOp( (yyvsp[-2].exprs), (yyvsp[-1].intVal), (yyvsp[0].exprs) );
						(yyval.exprs) = (yyvsp[-2].exprs);
						gen_rel((yyvsp[-2].exprs),(yyvsp[0].exprs),(yyvsp[-1].intVal));
					}
#line 2759 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1035 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 2765 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1038 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = LT_t; }
#line 2771 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1039 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = LE_t; }
#line 2777 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1040 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = EQ_t; }
#line 2783 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1041 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = GE_t; }
#line 2789 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 1042 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = GT_t; }
#line 2795 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1043 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = NE_t; }
#line 2801 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1047 "parser.y" /* yacc.c:1646  */
    {
				verifyArithmeticOp( (yyvsp[-2].exprs), (yyvsp[-1].intVal), (yyvsp[0].exprs) );
				(yyval.exprs) = (yyvsp[-2].exprs);
				gen_expre((yyvsp[-2].exprs),(yyvsp[0].exprs),(yyvsp[-1].intVal));
			}
#line 2811 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1052 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 2817 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1053 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 2823 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1056 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = ADD_t; }
#line 2829 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1057 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = SUB_t; }
#line 2835 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1061 "parser.y" /* yacc.c:1646  */
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
#line 2850 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1071 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 2856 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1074 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = MUL_t; }
#line 2862 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1075 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = DIV_t; }
#line 2868 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1076 "parser.y" /* yacc.c:1646  */
    { (yyval.intVal) = MOD_t; }
#line 2874 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1080 "parser.y" /* yacc.c:1646  */
    {
			verifyExistence( symbolTable, (yyvsp[0].exprs), scope, __FALSE );
			(yyval.exprs) = (yyvsp[0].exprs);
			(yyval.exprs)->beginningOp = NONE_t;
			gen_load((yyvsp[0].exprs));
		}
#line 2885 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1087 "parser.y" /* yacc.c:1646  */
    {
			if( verifyExistence( symbolTable, (yyvsp[0].exprs), scope, __FALSE ) == __TRUE )
				verifyUnaryMinus( (yyvsp[0].exprs) );
			(yyval.exprs) = (yyvsp[0].exprs);
			(yyval.exprs)->beginningOp = SUB_t;
			gen_load((yyvsp[0].exprs));
			gen_expre((yyvsp[0].exprs),(yyvsp[0].exprs),5);
		}
#line 2898 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 1096 "parser.y" /* yacc.c:1646  */
    {
			(yyvsp[-1].exprs)->beginningOp = NONE_t;
			(yyval.exprs) = (yyvsp[-1].exprs); 
			gen_load((yyvsp[-1].exprs));
		}
#line 2908 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 1102 "parser.y" /* yacc.c:1646  */
    {
			verifyUnaryMinus( (yyvsp[-1].exprs) );
			(yyval.exprs) = (yyvsp[-1].exprs);
			(yyval.exprs)->beginningOp = SUB_t;
			gen_load((yyvsp[-1].exprs));
			gen_expre((yyvsp[-1].exprs),(yyvsp[-1].exprs),5);
		}
#line 2920 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 1110 "parser.y" /* yacc.c:1646  */
    {
			(yyval.exprs) = verifyFuncInvoke( (yyvsp[-3].lexeme), (yyvsp[-1].exprs), symbolTable, scope );
			(yyval.exprs)->beginningOp = NONE_t;
			gen_Invoke((yyvsp[-3].lexeme),(yyvsp[-1].exprs));
			
		}
#line 2931 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 1117 "parser.y" /* yacc.c:1646  */
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
#line 2948 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 1130 "parser.y" /* yacc.c:1646  */
    {
			(yyval.exprs) = verifyFuncInvoke( (yyvsp[-2].lexeme), 0, symbolTable, scope );
			(yyval.exprs)->beginningOp = NONE_t;
			gen_Invoke((yyvsp[-2].lexeme),NULL);
		}
#line 2958 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 1136 "parser.y" /* yacc.c:1646  */
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
#line 2974 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 1148 "parser.y" /* yacc.c:1646  */
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
#line 2993 "y.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 1165 "parser.y" /* yacc.c:1646  */
    {
			  				struct expr_sem *exprPtr;
			  				for( exprPtr=(yyvsp[-2].exprs) ; (exprPtr->next)!=0 ; exprPtr=(exprPtr->next) );
			  				exprPtr->next = (yyvsp[0].exprs);
			  				(yyval.exprs) = (yyvsp[-2].exprs);
						}
#line 3004 "y.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 1171 "parser.y" /* yacc.c:1646  */
    { (yyval.exprs) = (yyvsp[0].exprs); }
#line 3010 "y.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 1177 "parser.y" /* yacc.c:1646  */
    { (yyval.ptype) = createPType( INTEGER_t ); }
#line 3016 "y.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 1178 "parser.y" /* yacc.c:1646  */
    { (yyval.ptype) = createPType( DOUBLE_t ); }
#line 3022 "y.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 1179 "parser.y" /* yacc.c:1646  */
    { (yyval.ptype) = createPType( STRING_t ); }
#line 3028 "y.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 1180 "parser.y" /* yacc.c:1646  */
    { (yyval.ptype) = createPType( BOOLEAN_t ); }
#line 3034 "y.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 1181 "parser.y" /* yacc.c:1646  */
    { (yyval.ptype) = createPType( FLOAT_t ); }
#line 3040 "y.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 1185 "parser.y" /* yacc.c:1646  */
    {
					int tmp = (yyvsp[0].intVal);
					(yyval.constVal) = createConstAttr( INTEGER_t, &tmp );
					ConstInt = tmp;
				}
#line 3050 "y.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 1191 "parser.y" /* yacc.c:1646  */
    {
					int tmp = -(yyvsp[0].intVal);
					(yyval.constVal) = createConstAttr( INTEGER_t, &tmp );
					ConstInt = tmp;
				}
#line 3060 "y.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 1197 "parser.y" /* yacc.c:1646  */
    {
					float tmp = (yyvsp[0].floatVal);
					(yyval.constVal) = createConstAttr( FLOAT_t, &tmp );
					ConstFloat = tmp;
				}
#line 3070 "y.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 1203 "parser.y" /* yacc.c:1646  */
    {
					float tmp = -(yyvsp[0].floatVal);
					(yyval.constVal) = createConstAttr( FLOAT_t, &tmp );
					ConstFloat = tmp;
				}
#line 3080 "y.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 1209 "parser.y" /* yacc.c:1646  */
    {
					double tmp = (yyvsp[0].floatVal);
					(yyval.constVal) = createConstAttr( DOUBLE_t, &tmp );
					ConstDouble = tmp;
				}
#line 3090 "y.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 1215 "parser.y" /* yacc.c:1646  */
    {
					double tmp = -(yyvsp[0].floatVal);
					(yyval.constVal) = createConstAttr( DOUBLE_t, &tmp );
					ConstDouble = tmp;
				}
#line 3100 "y.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 1221 "parser.y" /* yacc.c:1646  */
    {
					(yyval.constVal) = createConstAttr( STRING_t, (yyvsp[0].lexeme) );
					strcpy(ConstString, (yyval.constVal)->value.stringVal);
				}
#line 3109 "y.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 1226 "parser.y" /* yacc.c:1646  */
    {
					SEMTYPE tmp = __TRUE;
					(yyval.constVal) = createConstAttr( BOOLEAN_t, &tmp );
					ConstBool = 1;
				}
#line 3119 "y.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 1232 "parser.y" /* yacc.c:1646  */
    {
					SEMTYPE tmp = __FALSE;
					(yyval.constVal) = createConstAttr( BOOLEAN_t, &tmp );
					ConstBool = 0;
				}
#line 3129 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3133 "y.tab.c" /* yacc.c:1646  */
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
#line 1238 "parser.y" /* yacc.c:1906  */


int yyerror( char *msg )
{
    fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
	fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
	fprintf( stderr, "|\n" );
	fprintf( stderr, "| Unmatched token: %s\n", yytext );
	fprintf( stderr, "|--------------------------------------------------------------------------\n" );
	exit(-1);
}


