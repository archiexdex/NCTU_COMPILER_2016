#!/bin/sh

yacc -dv parser.y
gcc scanner.c y.tab.c -ly -lfl
