#!/bin/sh

yacc -dv parser.y
gcc lex.yy.c y.tab.c -ly -lfl
