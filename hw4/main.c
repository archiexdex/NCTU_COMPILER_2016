#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "symtab.h"

extern int yyparse();
extern FILE* yyin;

extern struct SymTable *symbolTable;
extern struct PType *funcReturn;
extern char fileName[256];

extern __BOOLEAN semError; 

FILE *java;

int  main( int argc, char **argv )
{
	if( argc == 1 )
	{
		yyin = stdin;
	}
	else if( argc == 2 )
	{
		FILE *fp = fopen( argv[1], "r" );
		if( fp == NULL ) {
				fprintf( stderr, "Open file error\n" );
				exit(-1);
		}
		yyin = fp;
		for ( int i = 0 ; i < strlen(argv[1] ); ++i ){
			fileName[i] = argv[1][i];
		}
		// fileName[ strlen(argv[1]) ] = '\0';
		// fileName[ strlen(fileName)-1] = 'j';
		strcpy(fileName, "output.j");
		java = fopen( fileName, "w" );
		fprintf(java,"; %s\n",fileName);
		fileName[ strlen(fileName)-2] = '\0';
		fprintf(java, ".class public %s\n", fileName);
		fprintf(java, ".super java/lang/Object\n");
		fprintf(java, ".field public static _sc Ljava/util/Scanner;\n");
		
	}
	else
	{
	  	fprintf( stderr, "Usage: ./parser [filename]\n" );
   		exit(0);
 	} 

	symbolTable = (struct SymTable *)malloc(sizeof(struct SymTable));
	initSymTab( symbolTable );

	// initial function return recoder

	yyparse();	/* primary procedure of parser */

	if(semError == __TRUE){	
		fprintf( stdout, "\n|--------------------------------|\n" );
		fprintf( stdout, "|  There is no syntactic error!  |\n" );
		fprintf( stdout, "|--------------------------------|\n" );
	}
	else{
		fprintf( stdout, "\n|-------------------------------------------|\n" );
		fprintf( stdout, "| There is no syntactic and semantic error! |\n" );
		fprintf( stdout, "|-------------------------------------------|\n" );
	}

	exit(0);
}

