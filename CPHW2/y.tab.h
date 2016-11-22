/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
    LE = 258,
    EQ = 259,
    GE = 260,
    NQ = 261,
    AND = 262,
    OR = 263,
    INTEGER = 264,
    REALNUMBER = 265,
    READ = 266,
    BOOLEAN = 267,
    WHILE = 268,
    DO = 269,
    IF = 270,
    ELSE = 271,
    TRUE = 272,
    FALSE = 273,
    FOR = 274,
    CONST = 275,
    INT = 276,
    PRINT = 277,
    BOOL = 278,
    VOID = 279,
    FLOAT = 280,
    DOUBLE = 281,
    STRING = 282,
    CONTINUE = 283,
    BREAK = 284,
    RETURN = 285,
    STRCONTENT = 286,
    ID = 287
  };
#endif
/* Tokens.  */
#define LE 258
#define EQ 259
#define GE 260
#define NQ 261
#define AND 262
#define OR 263
#define INTEGER 264
#define REALNUMBER 265
#define READ 266
#define BOOLEAN 267
#define WHILE 268
#define DO 269
#define IF 270
#define ELSE 271
#define TRUE 272
#define FALSE 273
#define FOR 274
#define CONST 275
#define INT 276
#define PRINT 277
#define BOOL 278
#define VOID 279
#define FLOAT 280
#define DOUBLE 281
#define STRING 282
#define CONTINUE 283
#define BREAK 284
#define RETURN 285
#define STRCONTENT 286
#define ID 287

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
