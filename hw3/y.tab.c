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


#line 295 "y.tab.c" /* yacc.c:339  */

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
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    INT_CONST = 259,
    FLOAT_CONST = 260,
    SCIENTIFIC = 261,
    STR_CONST = 262,
    LE_OP = 263,
    NE_OP = 264,
    GE_OP = 265,
    EQ_OP = 266,
    AND_OP = 267,
    OR_OP = 268,
    READ = 269,
    BOOLEAN = 270,
    WHILE = 271,
    DO = 272,
    IF = 273,
    ELSE = 274,
    TRUE = 275,
    FALSE = 276,
    FOR = 277,
    INT = 278,
    PRINT = 279,
    BOOL = 280,
    VOID = 281,
    FLOAT = 282,
    DOUBLE = 283,
    STRING = 284,
    CONTINUE = 285,
    BREAK = 286,
    RETURN = 287,
    CONST = 288,
    L_PAREN = 289,
    R_PAREN = 290,
    COMMA = 291,
    SEMICOLON = 292,
    ML_BRACE = 293,
    MR_BRACE = 294,
    L_BRACE = 295,
    R_BRACE = 296,
    ADD_OP = 297,
    SUB_OP = 298,
    MUL_OP = 299,
    DIV_OP = 300,
    MOD_OP = 301,
    ASSIGN_OP = 302,
    LT_OP = 303,
    GT_OP = 304,
    NOT_OP = 305
  };
#endif
/* Tokens.  */
#define ID 258
#define INT_CONST 259
#define FLOAT_CONST 260
#define SCIENTIFIC 261
#define STR_CONST 262
#define LE_OP 263
#define NE_OP 264
#define GE_OP 265
#define EQ_OP 266
#define AND_OP 267
#define OR_OP 268
#define READ 269
#define BOOLEAN 270
#define WHILE 271
#define DO 272
#define IF 273
#define ELSE 274
#define TRUE 275
#define FALSE 276
#define FOR 277
#define INT 278
#define PRINT 279
#define BOOL 280
#define VOID 281
#define FLOAT 282
#define DOUBLE 283
#define STRING 284
#define CONTINUE 285
#define BREAK 286
#define RETURN 287
#define CONST 288
#define L_PAREN 289
#define R_PAREN 290
#define COMMA 291
#define SEMICOLON 292
#define ML_BRACE 293
#define MR_BRACE 294
#define L_BRACE 295
#define R_BRACE 296
#define ADD_OP 297
#define SUB_OP 298
#define MUL_OP 299
#define DIV_OP 300
#define MOD_OP 301
#define ASSIGN_OP 302
#define LT_OP 303
#define GT_OP 304
#define NOT_OP 305

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 230 "parser.y" /* yacc.c:355  */

	char*    text;
	Type     type;
	VarList* varList;
	ConstVal constVal;

#line 442 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 459 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   510

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  137
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  279

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
       0,   312,   312,   313,   319,   322,   323,   324,   325,   328,
     329,   330,   331,   332,   335,   342,   350,   356,   365,   370,
     375,   381,   390,   403,   415,   421,   429,   447,   456,   466,
     488,   497,   511,   512,   517,   520,   525,   538,   544,   547,
     564,   578,   588,   596,   612,   626,   629,   630,   631,   632,
     635,   636,   637,   638,   639,   640,   641,   644,   654,   659,
     667,   667,   672,   672,   681,   681,   687,   696,   700,   701,
     704,   714,   717,   718,   730,   737,   738,   741,   751,   752,
     753,   765,   772,   781,   782,   783,   788,   789,   799,   804,
     807,   812,   815,   824,   827,   853,   858,   859,   860,   861,
     862,   863,   866,   898,   929,   930,   933,   964,   995,  1003,
    1006,  1009,  1014,  1015,  1016,  1037,  1057,  1058,  1078,  1100,
    1113,  1120,  1129,  1139,  1151,  1152,  1153,  1154,  1155,  1158,
    1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "INT_CONST", "FLOAT_CONST",
  "SCIENTIFIC", "STR_CONST", "LE_OP", "NE_OP", "GE_OP", "EQ_OP", "AND_OP",
  "OR_OP", "READ", "BOOLEAN", "WHILE", "DO", "IF", "ELSE", "TRUE", "FALSE",
  "FOR", "INT", "PRINT", "BOOL", "VOID", "FLOAT", "DOUBLE", "STRING",
  "CONTINUE", "BREAK", "RETURN", "CONST", "L_PAREN", "R_PAREN", "COMMA",
  "SEMICOLON", "ML_BRACE", "MR_BRACE", "L_BRACE", "R_BRACE", "ADD_OP",
  "SUB_OP", "MUL_OP", "DIV_OP", "MOD_OP", "ASSIGN_OP", "LT_OP", "GT_OP",
  "NOT_OP", "$accept", "push_table", "pop_table", "program", "decl_list",
  "decl_and_def_list", "funct_def", "funct_decl", "parameter_list",
  "var_decl", "identifier_list", "initial_array", "literal_list",
  "const_decl", "const_list", "array_decl", "dim", "compound_statement",
  "var_const_stmt_list", "statement", "simple_statement",
  "conditional_statement", "$@1", "$@2", "while_statement", "$@3",
  "for_statement", "initial_expression_list", "initial_expression",
  "control_expression", "increment_expression_list",
  "increment_expression", "function_invoke_statement", "jump_statement",
  "variable_reference", "logical_expression", "logical_term",
  "logical_factor", "relation_expression", "relation_operator",
  "arithmetic_expression", "term", "factor", "logical_expression_list",
  "array_list", "dimension", "scalar_type", "literal_const", YY_NULLPTR
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

#define YYPACT_NINF -199

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-199)))

#define YYTABLE_NINF -94

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -199,  -199,    35,   477,  -199,  -199,  -199,    10,  -199,  -199,
    -199,    82,  -199,  -199,  -199,  -199,    68,    79,    81,   477,
     -20,    41,    22,   249,    73,    63,  -199,  -199,  -199,  -199,
    -199,   287,    98,   122,    93,   138,  -199,   118,   124,    51,
     161,   250,   168,  -199,   136,    87,   135,    -1,  -199,  -199,
    -199,  -199,  -199,  -199,   122,   207,   122,  -199,   167,   171,
    -199,   457,   190,    50,  -199,  -199,  -199,   180,    -6,   142,
     122,  -199,  -199,   145,  -199,   154,    82,   156,  -199,   134,
    -199,   148,  -199,  -199,   159,  -199,   112,   122,   164,    16,
      45,  -199,  -199,  -199,   122,  -199,  -199,   122,   122,  -199,
    -199,  -199,  -199,   239,   239,  -199,  -199,   239,   239,   239,
     239,   169,   122,   118,    31,   167,  -199,  -199,  -199,   200,
    -199,  -199,  -199,   250,  -199,  -199,  -199,   167,   100,     6,
     122,  -199,   147,    17,   171,  -199,    50,    50,  -199,   190,
    -199,  -199,  -199,  -199,   167,  -199,   122,  -199,   272,   156,
    -199,  -199,  -199,   122,  -199,     8,  -199,   109,  -199,   167,
      59,   206,   183,  -199,   184,   185,   122,   187,   192,   122,
    -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,
    -199,   189,   227,   167,  -199,  -199,   172,   193,   197,   122,
     208,   122,   122,     7,  -199,  -199,    12,  -199,   122,    28,
     203,   113,  -199,    18,  -199,    21,   210,   201,   202,   167,
    -199,  -199,    13,  -199,   215,  -199,   303,  -199,   122,   122,
     122,  -199,  -199,  -199,  -199,  -199,  -199,   216,   167,   211,
     167,   167,   221,   246,   223,   224,   122,   122,  -199,   231,
    -199,  -199,   232,   230,   233,   167,   167,   334,   122,   365,
     396,  -199,   122,   122,  -199,    47,  -199,  -199,   228,   234,
     167,   167,  -199,   242,  -199,   264,  -199,   122,  -199,  -199,
     427,   167,   245,  -199,  -199,  -199,   458,  -199,  -199
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     8,     0,     0,     1,   124,   127,     0,   128,   125,
     126,     0,    13,     7,     5,     6,     0,     0,     0,     3,
      34,     0,    32,     0,     0,     0,     4,    12,    11,     9,
      10,     0,     0,     0,    42,     0,    26,     0,     2,     0,
       0,     0,     0,    39,     2,     0,     0,    87,   129,   131,
     133,   135,   136,   137,     0,     0,     0,   110,    33,    89,
      91,   104,    95,   105,   109,    86,   116,     0,    27,    30,
      38,    31,    20,     0,    16,     2,     0,    25,    24,     0,
      41,     0,    18,    14,     2,    44,     0,     0,   121,     0,
      87,   129,   131,   133,     0,   111,    92,     0,     0,    97,
     101,    99,    98,     0,     0,    96,   100,     0,     0,     0,
       0,     0,     0,     0,     0,    37,    49,    21,    17,     0,
     130,   132,   134,     0,    19,    15,   115,   120,     0,     0,
       0,   112,     0,     0,    88,    90,   102,   103,   104,    94,
     106,   107,   108,    43,    28,    29,     0,    35,     2,    22,
      23,    40,   114,     0,   123,     0,   115,     0,   112,    36,
      87,     0,     0,     2,     0,     0,     0,     0,     0,     0,
       3,    47,    48,    50,    46,    51,    52,    53,    54,    55,
      56,     0,     0,   119,   122,   114,     0,    87,     0,     0,
       0,     0,    69,     0,    83,    84,     0,    45,     0,    34,
       0,     0,    59,     0,    49,     0,     0,    68,   110,    72,
      58,    85,     0,    82,     0,    64,     2,    60,     0,     0,
       0,    57,    81,     2,     3,     2,     2,     0,    74,   110,
      71,    73,     0,     0,     0,     0,    76,     0,    49,     0,
      49,    49,     0,    75,   110,    79,    70,     2,     0,     2,
       2,     2,     0,     0,     3,     0,     3,     3,     0,   110,
      78,    80,    65,     0,    61,     0,    49,     0,    66,     2,
       2,    77,     0,     3,    49,    67,     2,     3,    63
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -199,     0,  -165,  -199,  -199,  -199,   268,   273,   260,    20,
    -199,   194,  -199,    55,  -199,   -31,  -199,   -27,  -198,  -199,
    -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,
    -199,  -199,  -199,  -199,  -146,   -32,   212,   -44,   191,  -199,
     204,    66,   -45,  -125,  -199,  -199,     5,   -38
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    73,    26,     2,     3,    19,    12,    13,    39,   171,
      21,    71,   114,   172,    25,    22,    34,   173,   148,   174,
     175,   176,   225,   226,   177,   223,   178,   206,   207,   227,
     242,   243,   179,   180,    57,   127,    59,    60,    61,   107,
      62,    63,    64,   128,    65,    88,   182,    66
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       1,    58,   181,    80,    69,   197,   216,   157,    16,    78,
      95,    74,    96,    17,    31,   188,    18,    83,    32,    97,
      97,    97,    89,    14,    16,    97,    97,    33,    40,    97,
      97,    97,    32,    86,    97,     4,    40,    87,   115,    29,
     247,   112,   249,   250,   210,   154,   208,   184,   118,   211,
     221,   131,   158,   215,   135,   129,   217,   125,    15,   233,
      97,   201,   133,   140,   141,   142,    32,   146,   270,    37,
     181,    20,   147,   229,    30,    33,   276,    35,    36,   132,
     144,   119,   263,    87,    24,   151,    75,    76,   150,   262,
     244,   264,   265,   186,   108,   109,   110,    87,   155,    42,
      43,   181,    46,   181,   181,     5,   259,     6,   275,     8,
       9,    10,   278,    23,   159,    47,    48,    49,    50,    51,
      41,   183,    84,    76,   181,    47,    48,    49,    50,    51,
     181,    67,    52,    53,   193,   152,   153,   196,   120,   121,
     122,    68,    52,    53,   185,   153,    54,   126,   214,   153,
      47,    48,    49,    50,    51,    55,    54,   203,    70,   205,
     209,    72,    56,   190,    77,    55,   212,    52,    53,   136,
     137,    81,    56,    82,    85,    47,    48,    49,    50,    51,
      97,    54,   156,    98,   111,   116,   228,   230,   231,   113,
      55,   117,    52,    53,    32,   123,   124,    56,    99,   100,
     101,   102,   130,   149,   245,   246,    54,   200,   143,   187,
      90,    91,    92,    93,    51,    55,   255,   189,   191,   192,
     260,   261,    56,   232,   194,   234,   235,    52,    53,   195,
     199,    87,   103,   104,   202,   271,   198,   219,   105,   106,
     213,    94,    47,    48,    49,    50,    51,   218,   204,   220,
      55,   258,   222,   236,    48,    49,    50,    51,   237,    52,
      53,   238,   239,   240,   241,   248,   252,   251,   266,   272,
      52,    53,     5,    54,     6,   160,     8,     9,    10,   268,
     253,   267,    55,   269,    38,   274,   161,    27,   162,   163,
     164,    45,    28,    79,   165,     5,   166,     6,   138,     8,
       9,    10,   167,   168,   169,    11,   160,   145,     0,   134,
       5,   139,     6,   170,     8,     9,    10,   161,     0,   162,
     163,   164,    44,     0,     0,   165,     5,   166,     6,     0,
       8,     9,    10,   167,   168,   169,    11,   160,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,   161,     0,
     162,   163,   164,     0,     0,     0,   165,     5,   166,     6,
       0,     8,     9,    10,   167,   168,   169,    11,   160,     0,
       0,     0,     0,     0,     0,   254,     0,     0,     0,   161,
       0,   162,   163,   164,     0,     0,     0,   165,     5,   166,
       6,     0,     8,     9,    10,   167,   168,   169,    11,   160,
       0,     0,     0,     0,     0,     0,   256,     0,     0,     0,
     161,     0,   162,   163,   164,     0,     0,     0,   165,     5,
     166,     6,     0,     8,     9,    10,   167,   168,   169,    11,
     160,     0,     0,     0,     0,     0,     0,   257,     0,     0,
       0,   161,     0,   162,   163,   164,     0,     0,     0,   165,
       5,   166,     6,     0,     8,     9,    10,   167,   168,   169,
      11,   160,     0,     0,     0,     0,     0,     0,   273,   -93,
     -93,     0,   161,     0,   162,   163,   164,     0,     0,     0,
     165,     5,   166,     6,     0,     8,     9,    10,   167,   168,
     169,    11,   -93,   -93,   -93,     0,   -93,     0,   -93,   277,
       5,     0,     6,     7,     8,     9,    10,     0,     0,     0,
      11
};

static const yytype_int16 yycheck[] =
{
       0,    33,   148,    41,    35,   170,   204,   132,     3,    40,
      55,    38,    56,     3,    34,   161,    11,    44,    38,    13,
      13,    13,    54,     3,    19,    13,    13,    47,    23,    13,
      13,    13,    38,    34,    13,     0,    31,    38,    70,    19,
     238,    47,   240,   241,    37,    39,   192,    39,    75,    37,
      37,    35,    35,    35,    98,    87,    35,    84,     3,   224,
      13,   186,    94,   108,   109,   110,    38,    36,   266,    47,
     216,     3,    41,   219,    19,    47,   274,    36,    37,    34,
     112,    76,    35,    38,     3,   123,    35,    36,   119,   254,
     236,   256,   257,    34,    44,    45,    46,    38,   130,    36,
      37,   247,     4,   249,   250,    23,   252,    25,   273,    27,
      28,    29,   277,    34,   146,     3,     4,     5,     6,     7,
      47,   153,    35,    36,   270,     3,     4,     5,     6,     7,
     276,    38,    20,    21,   166,    35,    36,   169,     4,     5,
       6,     3,    20,    21,    35,    36,    34,    35,    35,    36,
       3,     4,     5,     6,     7,    43,    34,   189,    40,   191,
     192,    37,    50,   163,     3,    43,   198,    20,    21,   103,
     104,     3,    50,    37,    39,     3,     4,     5,     6,     7,
      13,    34,    35,    12,     4,    40,   218,   219,   220,    47,
      43,    37,    20,    21,    38,    47,    37,    50,     8,     9,
      10,    11,    38,     3,   236,   237,    34,    35,    39,     3,
       3,     4,     5,     6,     7,    43,   248,    34,    34,    34,
     252,   253,    50,   223,    37,   225,   226,    20,    21,    37,
       3,    38,    42,    43,    37,   267,    47,    36,    48,    49,
      37,    34,     3,     4,     5,     6,     7,    37,    40,    47,
      43,   251,    37,    37,     4,     5,     6,     7,    47,    20,
      21,    40,    16,    40,    40,    34,    36,    35,    40,   269,
      20,    21,    23,    34,    25,     3,    27,    28,    29,    37,
      47,    47,    43,    19,    35,    40,    14,    19,    16,    17,
      18,    31,    19,    43,    22,    23,    24,    25,   107,    27,
      28,    29,    30,    31,    32,    33,     3,   113,    -1,    97,
      23,   107,    25,    41,    27,    28,    29,    14,    -1,    16,
      17,    18,    35,    -1,    -1,    22,    23,    24,    25,    -1,
      27,    28,    29,    30,    31,    32,    33,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    14,    -1,
      16,    17,    18,    -1,    -1,    -1,    22,    23,    24,    25,
      -1,    27,    28,    29,    30,    31,    32,    33,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    14,
      -1,    16,    17,    18,    -1,    -1,    -1,    22,    23,    24,
      25,    -1,    27,    28,    29,    30,    31,    32,    33,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      14,    -1,    16,    17,    18,    -1,    -1,    -1,    22,    23,
      24,    25,    -1,    27,    28,    29,    30,    31,    32,    33,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    14,    -1,    16,    17,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    -1,    27,    28,    29,    30,    31,    32,
      33,     3,    -1,    -1,    -1,    -1,    -1,    -1,    41,    12,
      13,    -1,    14,    -1,    16,    17,    18,    -1,    -1,    -1,
      22,    23,    24,    25,    -1,    27,    28,    29,    30,    31,
      32,    33,    35,    36,    37,    -1,    39,    -1,    41,    41,
      23,    -1,    25,    26,    27,    28,    29,    -1,    -1,    -1,
      33
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    52,    54,    55,     0,    23,    25,    26,    27,    28,
      29,    33,    57,    58,    60,    64,    97,     3,    97,    56,
       3,    61,    66,    34,     3,    65,    53,    57,    58,    60,
      64,    34,    38,    47,    67,    36,    37,    47,    35,    59,
      97,    47,    36,    37,    35,    59,     4,     3,     4,     5,
       6,     7,    20,    21,    34,    43,    50,    85,    86,    87,
      88,    89,    91,    92,    93,    95,    98,    38,     3,    66,
      40,    62,    37,    52,    68,    35,    36,     3,    66,    43,
      98,     3,    37,    68,    35,    39,    34,    38,    96,    86,
       3,     4,     5,     6,    34,    93,    88,    13,    12,     8,
       9,    10,    11,    42,    43,    48,    49,    90,    44,    45,
      46,     4,    47,    47,    63,    86,    40,    37,    68,    97,
       4,     5,     6,    47,    37,    68,    35,    86,    94,    86,
      38,    35,    34,    86,    87,    88,    92,    92,    89,    91,
      93,    93,    93,    39,    86,    62,    36,    41,    69,     3,
      66,    98,    35,    36,    39,    86,    35,    94,    35,    86,
       3,    14,    16,    17,    18,    22,    24,    30,    31,    32,
      41,    60,    64,    68,    70,    71,    72,    75,    77,    83,
      84,    85,    97,    86,    39,    35,    34,     3,    85,    34,
      52,    34,    34,    86,    37,    37,    86,    53,    47,     3,
      35,    94,    37,    86,    40,    86,    78,    79,    85,    86,
      37,    37,    86,    37,    35,    35,    69,    35,    37,    36,
      47,    37,    37,    76,    41,    73,    74,    80,    86,    85,
      86,    86,    52,    53,    52,    52,    37,    47,    40,    16,
      40,    40,    81,    82,    85,    86,    86,    69,    34,    69,
      69,    35,    36,    47,    41,    86,    41,    41,    52,    85,
      86,    86,    53,    35,    53,    53,    40,    47,    37,    19,
      69,    86,    52,    41,    40,    53,    69,    41,    53
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    52,    53,    54,    55,    55,    55,    55,    56,
      56,    56,    56,    56,    57,    57,    57,    57,    58,    58,
      58,    58,    59,    59,    59,    59,    60,    61,    61,    61,
      61,    61,    61,    61,    61,    62,    63,    63,    63,    64,
      65,    65,    66,    67,    67,    68,    69,    69,    69,    69,
      70,    70,    70,    70,    70,    70,    70,    71,    71,    71,
      73,    72,    74,    72,    76,    75,    75,    77,    78,    78,
      79,    79,    79,    79,    80,    81,    81,    82,    82,    82,
      82,    83,    83,    84,    84,    84,    85,    85,    86,    86,
      87,    87,    88,    88,    89,    89,    90,    90,    90,    90,
      90,    90,    91,    91,    91,    91,    92,    92,    92,    92,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    94,
      94,    95,    96,    96,    97,    97,    97,    97,    97,    98,
      98,    98,    98,    98,    98,    98,    98,    98
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     5,     2,     2,     2,     0,     2,
       2,     2,     2,     0,     5,     6,     5,     6,     5,     6,
       5,     6,     4,     4,     2,     2,     3,     3,     5,     5,
       3,     3,     1,     3,     1,     3,     3,     1,     0,     4,
       5,     3,     2,     4,     3,     5,     2,     2,     2,     0,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     3,
       0,    10,     0,    16,     0,    10,    11,    13,     1,     0,
       5,     3,     1,     3,     1,     1,     0,     5,     3,     1,
       3,     5,     4,     2,     2,     3,     1,     1,     3,     1,
       3,     1,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     1,     1,     3,     3,     3,     1,
       1,     2,     3,     4,     4,     3,     1,     5,     4,     3,
       1,     2,     4,     3,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     1,     2,     1,     1,     1
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
#line 312 "parser.y" /* yacc.c:1646  */
    { table.push_back(Table(table.size() ) ); }
#line 1782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 313 "parser.y" /* yacc.c:1646  */
    { if (Opt_Symbol) { table.back().print(); } 
			   table.back().clear();
			   table.pop_back();
			 }
#line 1791 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 335 "parser.y" /* yacc.c:1646  */
    {  
				if ( (yyvsp[0].type).t > (yyvsp[-4].type).t ) {
					logs(" return type is not equal");
				}
				table.back().insert((yyvsp[-3].varList)->text, (yyvsp[-4].type), K_FUNCTION);
				free((yyvsp[-3].varList)->text);
			}
#line 1803 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 342 "parser.y" /* yacc.c:1646  */
    {
		  	if ( (yyvsp[0].type).t > (yyvsp[-5].type).t ) {
					logs(" return type is not equal");
				}
				Value tmp;
				table.back().insert((yyvsp[-4].varList)->text, (yyvsp[-5].type), K_FUNCTION, tmp, (yyvsp[-2].varList));
				free((yyvsp[-4].varList)->text);
		  }
#line 1816 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 350 "parser.y" /* yacc.c:1646  */
    {
				Type tmp;
				tmp.t = T_VOID;
				table.back().insert((yyvsp[-3].varList)->text, tmp, K_FUNCTION);
				free((yyvsp[-3].varList)->text);
		  }
#line 1827 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 356 "parser.y" /* yacc.c:1646  */
    {
		  	 	Value tmp;
				Type tt;
				tt.t = T_VOID;
				table.back().insert((yyvsp[-4].varList)->text, tt, K_FUNCTION, tmp, (yyvsp[-2].varList)); ;
				free((yyvsp[-4].varList)->text);
		  }
#line 1839 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 365 "parser.y" /* yacc.c:1646  */
    {
				
				table.back().insert((yyvsp[-3].varList)->text, (yyvsp[-4].type), K_FUNCTION);
				free((yyvsp[-3].varList)->text);
			}
#line 1849 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 370 "parser.y" /* yacc.c:1646  */
    {
				Value tmp;
				table.back().insert((yyvsp[-4].varList)->text, (yyvsp[-5].type), K_FUNCTION, tmp, (yyvsp[-2].varList));
				free((yyvsp[-4].varList)->text);
			}
#line 1859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 375 "parser.y" /* yacc.c:1646  */
    {
				Type tmp;
				tmp.t = T_VOID;
				table.back().insert((yyvsp[-3].varList)->text, tmp, K_FUNCTION);
				free((yyvsp[-3].varList)->text);
			}
#line 1870 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 381 "parser.y" /* yacc.c:1646  */
    {
				Value tmp;
				Type tt;
				tt.t = T_VOID;
				table.back().insert((yyvsp[-4].varList)->text, tt, K_FUNCTION, tmp, (yyvsp[-2].varList)); ;
				free((yyvsp[-4].varList)->text);
			}
#line 1882 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 390 "parser.y" /* yacc.c:1646  */
    {
					for(VarList* ptr = (yyvsp[-3].varList); ptr; ptr = ptr->next){
						if( !ptr->next ){
							VarList* tmp = (yyvsp[0].varList);
							tmp->text = (yyvsp[0].varList)->text;
							tmp->type = (yyvsp[-1].type);
							tmp->next = NULL;
							ptr->next = tmp;
							break;
						}
					}
					(yyval.varList) = (yyvsp[-3].varList);
				}
#line 1900 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 403 "parser.y" /* yacc.c:1646  */
    {
					for( VarList* ptr = (yyvsp[-3].varList); ptr; ptr = ptr->next ){
						if( !ptr->next ){
							VarList* tmp = (yyvsp[0].varList);
							tmp->type.t = (yyvsp[-1].type).t;
							tmp->next = NULL;
							ptr->next = tmp;
							break;
						}
					}
					(yyval.varList) = (yyvsp[-3].varList);
				}
#line 1917 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 415 "parser.y" /* yacc.c:1646  */
    {
					(yyval.varList) = (yyvsp[0].varList);
					(yyval.varList)->type.t = (yyvsp[-1].type).t;
					(yyval.varList)->text = (yyvsp[0].varList)->text;
					(yyval.varList)->next = NULL;
				}
#line 1928 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 421 "parser.y" /* yacc.c:1646  */
    {
					(yyval.varList) = (yyvsp[0].varList);
					(yyval.varList)->type = (yyvsp[-1].type);
					(yyval.varList)->text = (yyvsp[0].varList)->text;
					(yyval.varList)->next = NULL;
				}
#line 1939 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 429 "parser.y" /* yacc.c:1646  */
    {
			for(VarList* ptr = (yyvsp[-1].varList); ptr != NULL ; ptr = ptr->next) {
				if( ptr->type.t != -1 && checkType((yyvsp[-2].type), ptr->type, 1) < 0 ){
					logs(" The assignment is illegal");
				}
				else {
					
					if ( table.back()[ptr->text] != NULL ){
						logs("%s is redeclared", ptr->text);
					}
					else {
						table.back().insert(ptr->text, (yyvsp[-2].type), K_VARIABLE);
					}
				}
			}
		 }
#line 1960 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 447 "parser.y" /* yacc.c:1646  */
    {
					for( VarList* ptr = (yyvsp[-2].varList); ptr; ptr = ptr->next ){
						if( ptr->next == NULL ){
							ptr->next = (yyvsp[0].varList);
							break;
						}
					}
					(yyval.varList) = (yyvsp[-2].varList);
				}
#line 1974 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 456 "parser.y" /* yacc.c:1646  */
    {
					for( VarList* ptr = (yyvsp[-4].varList); ptr; ptr = ptr->next ){
						if( ptr->next == NULL ){
							(yyvsp[-2].varList)->type = (yyvsp[0].constVal).type;
							ptr->next = (yyvsp[-2].varList);
							break;
						}
					}
					(yyval.varList) = (yyvsp[-4].varList);
				}
#line 1989 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 466 "parser.y" /* yacc.c:1646  */
    {
					int sum = 0;
					for ( int i = 0 ; i < (yyvsp[-2].varList)->type.dim ; i++ ){
						sum += (yyvsp[-2].varList)->type.array[i];
					}
					int now = 0;
					for ( VarList* ptr = (yyvsp[0].varList); ptr; ptr = ptr->next ){
						now++;
					}
					if( sum < now ){
						logs(" Too many values");
					}

					for( VarList* ptr = (yyvsp[-4].varList); ptr; ptr = ptr->next ){
						if( ptr->next == NULL ){
							(yyvsp[-2].varList)->type = (yyvsp[0].varList)->type;
							ptr->next = (yyvsp[-2].varList);
							break;
						}
					}
					(yyval.varList) = (yyvsp[-4].varList);
					}
#line 2016 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 488 "parser.y" /* yacc.c:1646  */
    {
					for( VarList* ptr = (yyvsp[-2].varList); ptr; ptr = ptr->next ){
						if( ptr->next == NULL ){
							ptr->next = (yyvsp[0].varList);
							break;
						}
					}
					(yyval.varList) = (yyvsp[-2].varList);
				}
#line 2030 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 497 "parser.y" /* yacc.c:1646  */
    {
					int sum = 0;
					for ( int i = 0 ; i < (yyvsp[-2].varList)->type.dim ; i++ ){
						sum += (yyvsp[-2].varList)->type.array[i];
					}
					int now = 0;
					for ( VarList* ptr = (yyvsp[0].varList); ptr; ptr = ptr->next ){
						now++;
					}
					if( sum < now ){
						logs(" Too many values");
					}
					(yyval.varList) = (yyvsp[-2].varList);
				}
#line 2049 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 511 "parser.y" /* yacc.c:1646  */
    { (yyval.varList) = (yyvsp[0].varList); }
#line 2055 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 512 "parser.y" /* yacc.c:1646  */
    {
					(yyval.varList) = (yyvsp[-2].varList);
					(yyval.varList)->type = (yyvsp[0].constVal).type;
					(yyval.varList)->next = NULL;
				}
#line 2065 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 517 "parser.y" /* yacc.c:1646  */
    { (yyval.varList) = (yyvsp[0].varList); }
#line 2071 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 520 "parser.y" /* yacc.c:1646  */
    {
					(yyval.varList) = (yyvsp[-1].varList);
				}
#line 2079 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 525 "parser.y" /* yacc.c:1646  */
    {
				for( VarList* ptr = (yyvsp[-2].varList) ; ptr; ptr = ptr->next ){
					if( !ptr->next ) {
						VarList* tmp = (VarList*)malloc(sizeof(VarList) );
						tmp->type = (yyvsp[0].constVal).type;
						tmp->val = (yyvsp[0].constVal).val;
						tmp->next = NULL;
						ptr->next = tmp;
						break;
					}
				}
				(yyval.varList) = (yyvsp[-2].varList);
			}
#line 2097 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 538 "parser.y" /* yacc.c:1646  */
    {
				(yyval.varList) = (VarList*)malloc(sizeof(VarList) );
			 	(yyval.varList)->type = (yyvsp[0].constVal).type;
				(yyval.varList)->val = (yyvsp[0].constVal).val;
				(yyval.varList)->next = NULL;
			 }
#line 2108 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 547 "parser.y" /* yacc.c:1646  */
    {
				for(VarList* ptr = (yyvsp[-1].varList); ptr != NULL ; ptr = ptr->next) {
					if( checkType((yyvsp[-2].type), ptr->type, 1) < 0 ){
						logs(" The assignment is illegal");
					}
					else {
						if ( table.back()[ptr->text] != NULL ){
							logs(" %s is redeclared", ptr->text);
						}
						else {
							table.back().insert(ptr->text, (yyvsp[-2].type), K_CONSTANT, ptr->val );
						}
					}
				}
			}
#line 2128 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 564 "parser.y" /* yacc.c:1646  */
    {
				for( VarList* ptr = (yyvsp[-4].varList); ptr; ptr = ptr->next ){
					if( !ptr->next ){
						VarList* tmp = (VarList*)malloc(sizeof(VarList) );
						tmp->text = (yyvsp[-2].varList)->text;
						tmp->val  = (yyvsp[0].constVal).val;
						tmp->type = (yyvsp[0].constVal).type;
						tmp->next = NULL;
						ptr->next = tmp;
						break;
					}
				}
				(yyval.varList) = (yyvsp[-4].varList);
			}
#line 2147 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 578 "parser.y" /* yacc.c:1646  */
    {
				//logs(" %d %f %f %s %d\n",$3.val.i,$3.val.f,$3.val.d, $3.val.s, $3.val.b);
				(yyval.varList) = (VarList*)malloc(sizeof(VarList) ); 
				(yyval.varList)->text = (yyvsp[-2].varList)->text;
				(yyval.varList)->val  = (yyvsp[0].constVal).val; 
				(yyval.varList)->type = (yyvsp[0].constVal).type;
				(yyval.varList)->next = NULL;
			}
#line 2160 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 588 "parser.y" /* yacc.c:1646  */
    {
				(yyvsp[-1].varList)->type = (yyvsp[0].type);
				(yyval.varList) = (yyvsp[-1].varList); 
				(yyval.varList)->type.t = -1;
				(yyval.varList)->next = NULL;
			}
#line 2171 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 596 "parser.y" /* yacc.c:1646  */
    {
		if ( (yyvsp[-1].constVal).val.i > 0){
			(yyvsp[-3].type).dim++;
			int* tmp = (int*)malloc(sizeof(int)*(yyvsp[-3].type).dim );
			for( int i = 0 ; i < (yyvsp[-3].type).dim-1 ; ++i) {
				tmp[i] = (yyvsp[-3].type).array[i];
			}
			tmp[(yyvsp[-3].type).dim-1] = (yyvsp[-1].constVal).val.i;
			free((yyvsp[-3].type).array);
			(yyvsp[-3].type).array = tmp;
			(yyval.type) = (yyvsp[-3].type);
		}
		else {
			logs(" array size must greater than 0");
		}
	}
#line 2192 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 612 "parser.y" /* yacc.c:1646  */
    {
		//cout << "array size " << $2.val.i <<"!" <<endl;
		if ( (yyvsp[-1].constVal).val.i > 0 ){
			(yyval.type).array = (int*)malloc(sizeof(int) );
			(yyval.type).dim = 1; 
			(yyval.type).array[0] = (yyvsp[-1].constVal).val.i;
			//for( int i = 0 ; i < $$.dim ; i++ ) cout << $$.array[i] << "!!" <<endl;
		}
		else {
			logs("array size must greater than 0");
		}
	}
#line 2209 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 626 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = (yyvsp[-2].type); }
#line 2215 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 629 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = (yyvsp[0].type); }
#line 2221 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 630 "parser.y" /* yacc.c:1646  */
    {}
#line 2227 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 631 "parser.y" /* yacc.c:1646  */
    {}
#line 2233 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 635 "parser.y" /* yacc.c:1646  */
    {}
#line 2239 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 636 "parser.y" /* yacc.c:1646  */
    {}
#line 2245 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 637 "parser.y" /* yacc.c:1646  */
    {}
#line 2251 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 638 "parser.y" /* yacc.c:1646  */
    {}
#line 2257 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 639 "parser.y" /* yacc.c:1646  */
    {}
#line 2263 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 640 "parser.y" /* yacc.c:1646  */
    {}
#line 2269 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 641 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = (yyvsp[0].type); }
#line 2275 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 644 "parser.y" /* yacc.c:1646  */
    {
					int tmp = checkType((yyvsp[-3].varList)->type, (yyvsp[-1].constVal).type,0);
					if( tmp == 1 ){
						logs(" dimention is not equal");
					}
					tmp = checkType((yyvsp[-3].varList)->type, (yyvsp[-1].constVal).type, 1);
					if( tmp < 0 ) {
						logs(" Can't assign value to variable");
					}
				}
#line 2290 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 654 "parser.y" /* yacc.c:1646  */
    {
					if( (yyvsp[-1].constVal).type.t == T_VOID ) {
						logs(" Can't print void type");
					}
				}
#line 2300 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 659 "parser.y" /* yacc.c:1646  */
    {
					if( (yyvsp[-1].varList)->type.t == T_VOID ) {
						logs(" Can't read void type");
					}
				}
#line 2310 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 667 "parser.y" /* yacc.c:1646  */
    { 
							if( (yyvsp[-1].constVal).type.t != T_BOOL ){
								logs(" contidional expression must be boolean");
							}
						}
#line 2320 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 672 "parser.y" /* yacc.c:1646  */
    {
								if( (yyvsp[-1].constVal).type.t != T_BOOL ) {
									logs("conditional expression must be boolean");
								}
							}
#line 2330 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 681 "parser.y" /* yacc.c:1646  */
    {
					if( (yyvsp[-1].constVal).type.t != T_BOOL ){
						logs(" conditional expression must be boolean");
					}
				}
#line 2340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 689 "parser.y" /* yacc.c:1646  */
    {
				  	if( (yyvsp[-2].constVal).type.t != T_BOOL ){
						logs(" conditional expression must be boolean");
					}
				  }
#line 2350 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 704 "parser.y" /* yacc.c:1646  */
    {
						int tmp = checkType((yyvsp[-2].varList)->type, (yyvsp[0].constVal).type,0);
							if( tmp == 1 ){
								logs("dimention is not equal");
							}
							tmp = checkType((yyvsp[-2].varList)->type, (yyvsp[0].constVal).type, 1);
							if( tmp < 0 ) {
								logs(" Can't assign value to variable");
						}
					}
#line 2365 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 714 "parser.y" /* yacc.c:1646  */
    {
						
					}
#line 2373 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 717 "parser.y" /* yacc.c:1646  */
    {  }
#line 2379 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 718 "parser.y" /* yacc.c:1646  */
    {
						int tmp = checkType((yyvsp[-2].varList)->type, (yyvsp[0].constVal).type,0);
							if( tmp == 1 ){
								logs(" dimention is not equal");
							}
							tmp = checkType((yyvsp[-2].varList)->type, (yyvsp[0].constVal).type, 1);
							if( tmp < 0 ) {
								logs(" Can't assign value to variable");
						}
					}
#line 2394 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 730 "parser.y" /* yacc.c:1646  */
    {
						if ( (yyvsp[0].constVal).type.t != T_BOOL ){
							logs(" control expression must be type bool");
						}
					}
#line 2404 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 741 "parser.y" /* yacc.c:1646  */
    {
					 	int tmp = checkType((yyvsp[-2].varList)->type, (yyvsp[0].constVal).type,0);
							if( tmp == 1 ){
								logs(" dimention is not equal");
							}
							tmp = checkType((yyvsp[-2].varList)->type, (yyvsp[0].constVal).type, 1);
							if( tmp < 0 ) {
								logs(" Can't assign value to variable");
						}
					 }
#line 2419 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 751 "parser.y" /* yacc.c:1646  */
    { }
#line 2425 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 752 "parser.y" /* yacc.c:1646  */
    { }
#line 2431 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 753 "parser.y" /* yacc.c:1646  */
    {
						int tmp = checkType((yyvsp[-2].varList)->type, (yyvsp[0].constVal).type,0);
							if( tmp == 1 ){
								logs(" dimention is not equal");
							}
							tmp = checkType((yyvsp[-2].varList)->type, (yyvsp[0].constVal).type, 1);
							if( tmp < 0 ) {
								logs(" Can't assign value to variable");
						}
					 }
#line 2446 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 765 "parser.y" /* yacc.c:1646  */
    {
								Entry * ptr = getEntry((yyvsp[-4].varList)->text, K_FUNCTION);
								bool miss = checkAttr((yyvsp[-4].varList),(yyvsp[-2].varList));
								if (miss){
									logs(" dimention is not equal");
								}
							}
#line 2458 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 772 "parser.y" /* yacc.c:1646  */
    {
								Entry* ptr = getEntry((yyvsp[-3].varList)->text, K_FUNCTION);
								bool miss = checkAttr((yyvsp[-3].varList), NULL);
								if ( miss ) {
									logs(" dimention is not equal");
								}
							}
#line 2470 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 781 "parser.y" /* yacc.c:1646  */
    { }
#line 2476 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 782 "parser.y" /* yacc.c:1646  */
    { }
#line 2482 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 783 "parser.y" /* yacc.c:1646  */
    {
					(yyval.type) = (yyvsp[-1].constVal).type;
				}
#line 2490 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 788 "parser.y" /* yacc.c:1646  */
    { (yyval.varList)->type = (yyvsp[0].type); }
#line 2496 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 789 "parser.y" /* yacc.c:1646  */
    { 
						Entry* ptr = getEntry((yyvsp[0].varList)->text);
						(yyval.varList)->type.t = ptr->_type.t;
						(yyval.varList)->text = (char*)ptr->text().c_str();
						(yyval.varList)->next = NULL;
						//cout << "!" <<  $$->type.t <<endl;
					}
#line 2508 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 799 "parser.y" /* yacc.c:1646  */
    {
						if( (yyvsp[-2].constVal).type.t != T_BOOL || (yyvsp[0].constVal).type.t != T_BOOL ){
							logs(" The operands are not bool");
						}
					}
#line 2518 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 804 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal); }
#line 2524 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 807 "parser.y" /* yacc.c:1646  */
    {
				if( (yyvsp[-2].constVal).type.t != T_BOOL || (yyvsp[0].constVal).type.t != T_BOOL ){
					logs(" The operands are not type bool");
				}
			}
#line 2534 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 812 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal); }
#line 2540 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 815 "parser.y" /* yacc.c:1646  */
    { 
					if ( (yyvsp[0].constVal).type.t != T_BOOL ){
						logs(" %s must be used for type bool",(yyvsp[-1].text));
					}
					else {
						ConstVal tmp = neg((yyvsp[0].constVal)); 
						(yyval.constVal) = tmp; 
					}
				}
#line 2554 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 824 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal); }
#line 2560 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 827 "parser.y" /* yacc.c:1646  */
    {
						int t1 = 0 , t2 = 0;
						switch ((yyvsp[-2].constVal).type.t) {
							case T_INT   : t1 = T_INT;    break;
							case T_FLOAT : t1 = T_FLOAT;  break;
							case T_DOUBLE: t1 = T_DOUBLE; break;
							case T_BOOL  : t1 = T_BOOL;   break;
							default: t1 = -1;
						}
						switch ((yyvsp[0].constVal).type.t) {
							case T_INT   : t1 = T_INT;    break;
							case T_FLOAT : t1 = T_FLOAT;  break;
							case T_DOUBLE: t1 = T_DOUBLE; break;
							case T_BOOL  : t1 = T_BOOL;   break;
							default: t1 = -1;
						}
						if ( strcmp((yyvsp[-1].text), "==" ) || strcmp((yyvsp[-1].text), "!=" ) ){
							logs("The operands must be int, float, double and bool");
						}
						else if ( t1 < 0 || t2 < 0 ){
							logs(" The operands must be int, float and double");
						}
						else{
							(yyval.constVal).type.t = T_BOOL;
						}
					}
#line 2591 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 853 "parser.y" /* yacc.c:1646  */
    {
						(yyval.constVal) = (yyvsp[0].constVal);
					}
#line 2599 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 866 "parser.y" /* yacc.c:1646  */
    {
							if( checkType((yyvsp[-2].constVal).type, (yyvsp[0].constVal).type, 0) == 1 ){
								logs(" The dimension is not equal");
							}
							if( checkType((yyvsp[-2].constVal).type, (yyvsp[0].constVal).type, 0) == 2 ){
								
								int t1, t2;
								switch((yyvsp[-2].constVal).type.t){
									case T_INT   : t1 = T_INT; break;
									case T_FLOAT : t1 = T_FLOAT; break;
									case T_DOUBLE: t1 = T_DOUBLE; break;
									default      : t1 = -1; break;
								}
								switch((yyvsp[0].constVal).type.t){
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
									addVal((yyvsp[-2].constVal),(yyvsp[0].constVal));
									(yyval.constVal) = (yyvsp[-2].constVal);
									(yyval.constVal).type.t = t1;
								}
							}
							
						}
#line 2636 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 898 "parser.y" /* yacc.c:1646  */
    {
							if( checkType((yyvsp[-2].constVal).type, (yyvsp[0].constVal).type, 0) == 1 ){
								logs(" The dimension is not equal");
							}
							if( checkType((yyvsp[-2].constVal).type, (yyvsp[0].constVal).type, 0) == 2 ){
								
								int t1, t2;
								switch((yyvsp[-2].constVal).type.t){
									case T_INT   : t1 = T_INT; break;
									case T_FLOAT : t1 = T_FLOAT; break;
									case T_DOUBLE: t1 = T_DOUBLE; break;
									default      : t1 = -1; break;
								}
								switch((yyvsp[0].constVal).type.t){
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
									subVal((yyvsp[-2].constVal),(yyvsp[0].constVal));
									(yyval.constVal) = (yyvsp[-2].constVal);
									(yyval.constVal).type.t = t1;
								}
							}
						}
#line 2672 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 929 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal); }
#line 2678 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 930 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal); }
#line 2684 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 933 "parser.y" /* yacc.c:1646  */
    {
	 	if( checkType((yyvsp[-2].constVal).type, (yyvsp[0].constVal).type, 0) == 1 ){
	 		logs("The dimension is not equal");
	 	}
	 	if( checkType((yyvsp[-2].constVal).type, (yyvsp[0].constVal).type, 0) == 2 ){
	 	
	 		int t1, t2;
	 		switch((yyvsp[-2].constVal).type.t){
	 			case T_INT   : t1 = T_INT; break;
	 			case T_FLOAT : t1 = T_FLOAT; break;
	 			case T_DOUBLE: t1 = T_DOUBLE; break;
	 			default      : t1 = -1; break;
	 		}
	 		switch((yyvsp[0].constVal).type.t){
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
	 			mulVal((yyvsp[-2].constVal),(yyvsp[0].constVal));
	 			(yyval.constVal) = (yyvsp[-2].constVal);
	 			(yyval.constVal).type.t = t1;
	 		}
	 }
	 }
#line 2720 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 964 "parser.y" /* yacc.c:1646  */
    {
	 	if( checkType((yyvsp[-2].constVal).type, (yyvsp[0].constVal).type, 0) == 1 ){
	 		logs(" The dimension is not equal");
	 	}
	 	if( checkType((yyvsp[-2].constVal).type, (yyvsp[0].constVal).type, 0) == 2 ){
	 	
	 		int t1, t2;
	 		switch((yyvsp[-2].constVal).type.t){
	 			case T_INT   : t1 = T_INT; break;
	 			case T_FLOAT : t1 = T_FLOAT; break;
	 			case T_DOUBLE: t1 = T_DOUBLE; break;
	 			default      : t1 = -1; break;
	 		}
	 		switch((yyvsp[0].constVal).type.t){
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
	 			mulVal((yyvsp[-2].constVal),(yyvsp[0].constVal));
	 			(yyval.constVal) = (yyvsp[-2].constVal);
	 			(yyval.constVal).type.t = t1;
	 		}
		}
	 }
#line 2756 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 995 "parser.y" /* yacc.c:1646  */
    {
	 	if( checkType((yyvsp[-2].constVal).type, (yyvsp[0].constVal).type, 0) == 1 ){
	 		logs(" The dimension is not equal");
	 	}
	 	if( (yyvsp[-2].constVal).type.t != T_INT || (yyvsp[0].constVal).type.t != T_INT ){
	 		logs(" The arithmetic are not int");
	 	}
	 }
#line 2769 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1003 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal); }
#line 2775 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1006 "parser.y" /* yacc.c:1646  */
    {
			(yyval.constVal).type = (yyvsp[0].varList)->type;
		}
#line 2783 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1009 "parser.y" /* yacc.c:1646  */
    {
			ConstVal tmp = neg((yyvsp[0].constVal));
			(yyval.constVal) = tmp;
			//cout << $$.val.i << "!!" <<endl;
	    }
#line 2793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1014 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[-1].constVal); }
#line 2799 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1015 "parser.y" /* yacc.c:1646  */
    { ConstVal tmp = neg((yyvsp[-1].constVal)); (yyval.constVal) = tmp; }
#line 2805 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1016 "parser.y" /* yacc.c:1646  */
    {// function 
			Entry* ptr = getEntry((yyvsp[-3].varList)->text, K_FUNCTION);
			
			bool miss = checkAttr((yyvsp[-3].varList), (yyvsp[-1].varList));
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
				(yyval.constVal).type.t = tt;
			}
		}
#line 2831 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1037 "parser.y" /* yacc.c:1646  */
    {                        // function
			Entry* ptr = getEntry((yyvsp[-2].varList)->text, K_FUNCTION);
			bool miss = checkAttr((yyvsp[-2].varList), NULL);
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
				(yyval.constVal).type.t = tt;
			}
		}
#line 2856 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1057 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal); }
#line 2862 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 1058 "parser.y" /* yacc.c:1646  */
    {// function
			Entry* ptr = getEntry((yyvsp[-3].varList)->text, K_FUNCTION);
			int tt = ptr->_type.t;
			bool miss = checkAttr((yyvsp[-3].varList),(yyvsp[-1].varList));
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
				(yyval.constVal).type.t = tt;
			}
		}
#line 2887 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1078 "parser.y" /* yacc.c:1646  */
    {                   // function
			Entry* ptr = getEntry((yyvsp[-2].varList)->text, K_FUNCTION);
			int tt = ptr->_type.t;
			bool miss = checkAttr((yyvsp[-2].varList),NULL);
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
				(yyval.constVal).type.t = tt;
			}
		}
#line 2912 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1100 "parser.y" /* yacc.c:1646  */
    {
							for( VarList* ptr = (yyvsp[-2].varList); ptr; ptr = ptr->next ){
								
								if( !ptr->next ){
									VarList* tmp = (VarList*)malloc(sizeof(VarList) );
									tmp->type = (yyvsp[0].constVal).type;
									tmp->next = NULL;
									ptr->next = tmp;
									break;
								}
							}
							(yyval.varList) = (yyvsp[-2].varList);
						}
#line 2930 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1113 "parser.y" /* yacc.c:1646  */
    { 
							(yyval.varList) = (VarList*)malloc(sizeof(VarList) );
							(yyval.varList)->type = (yyvsp[0].constVal).type; 
							(yyval.varList)->next = NULL;
						}
#line 2940 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1120 "parser.y" /* yacc.c:1646  */
    { 
				Entry* ptr = getEntry((yyvsp[-1].varList)->text, K_VARIABLE);
				(yyval.type) = (yyvsp[0].type);
				(yyval.type).t = ptr->_type.t;
				(yyval.type).currdim = (yyvsp[0].type).currdim;
				(yyval.type).dim = ptr->_type.dim;
			}
#line 2952 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1129 "parser.y" /* yacc.c:1646  */
    {
				(yyvsp[-3].type).currdim++;
				if( (yyvsp[-1].constVal).type.t != T_INT ){
					logs(" Array index must be int");
				}
				else if( (yyvsp[-1].constVal).val.i <= 0 ){
					logs(" Array index must greater than 0");
				}
				(yyval.type) = (yyvsp[-3].type);
			}
#line 2967 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1139 "parser.y" /* yacc.c:1646  */
    {
				(yyval.type).currdim = 1;
				(yyval.type).array = NULL;
				if( (yyvsp[-1].constVal).type.t != T_INT ){
					logs(" Array index must be int");
				}
				else if( (yyvsp[-1].constVal).val.i <= 0 ){
					logs(" Array index must greater than 0");
				}
			}
#line 2982 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1151 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = (yyvsp[0].type); }
#line 2988 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1152 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = (yyvsp[0].type); }
#line 2994 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1153 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = (yyvsp[0].type); }
#line 3000 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1154 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = (yyvsp[0].type); }
#line 3006 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1155 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = (yyvsp[0].type); }
#line 3012 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1158 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal);   }
#line 3018 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1159 "parser.y" /* yacc.c:1646  */
    { ConstVal tmp = neg((yyvsp[0].constVal)); (yyval.constVal) = tmp; }
#line 3024 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 1160 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal); }
#line 3030 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 1161 "parser.y" /* yacc.c:1646  */
    { ConstVal tmp = neg((yyvsp[0].constVal)); (yyval.constVal) = tmp; }
#line 3036 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 1162 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal); }
#line 3042 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 1163 "parser.y" /* yacc.c:1646  */
    { ConstVal tmp = neg((yyvsp[0].constVal)); (yyval.constVal) = tmp; }
#line 3048 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 1164 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal); cout << (yyvsp[0].constVal).val.s << endl; }
#line 3054 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 1165 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal); }
#line 3060 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 1166 "parser.y" /* yacc.c:1646  */
    { (yyval.constVal) = (yyvsp[0].constVal); }
#line 3066 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3070 "y.tab.c" /* yacc.c:1646  */
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
#line 1168 "parser.y" /* yacc.c:1906  */


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
