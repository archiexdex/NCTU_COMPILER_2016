#!/bin/sh

./compiler example_jfile/assignment.c
./compiler example_jfile/expression.c
./compiler example_jfile/fibonacci.c
./compiler example_jfile/if_stmt.c
./compiler example_jfile/random.c

java -jar jasmin-2.4/jasmin.jar example_jfile/if_stmt.j
java -jar jasmin-2.4/jasmin.jar example_jfile/assignment.j
java -jar jasmin-2.4/jasmin.jar example_jfile/fibonacci.j
java -jar jasmin-2.4/jasmin.jar example_jfile/expression.j
java -jar jasmin-2.4/jasmin.jar example_jfile/random.j

