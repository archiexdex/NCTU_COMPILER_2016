#!/bin/sh

make 
./compiler test.c

#java -jar jasmin-2.4/jasmin.jar example_jfile/if_stmt.j
#java -jar jasmin-2.4/jasmin.jar example_jfile/assignment.j
#java -jar jasmin-2.4/jasmin.jar example_jfile/fibonacci.j
#java -jar jasmin-2.4/jasmin.jar example_jfile/expression.j
#java -jar jasmin-2.4/jasmin.jar example_jfile/output.j


java -jar jasmin-2.4/jasmin.jar output.j
