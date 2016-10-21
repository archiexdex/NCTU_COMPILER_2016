
%s C_COMMENT CC_COMMENT PRAGMA

%{
#define LIST                { strncat(buf, yytext, sizeof(buf) - strlen(buf) - 1); }
#define token(t)            { LIST; if (Opt_Token) printf("<\"%s\">\n", t); }
#define tokenString(t, s)   { LIST; if (Opt_Token) printf("<%s:%s>\n", t, s); }
#define MAX_LINE_LENGTH 257
#define MAX_ID_LENGTH 257
#define MAX_LENGTH 257
int Opt_Source = 1;
int Opt_Token = 1;
int Opt_Statistic = 1;
int linenum = 1;
char buf[MAX_LINE_LENGTH];
char IDNameBuf[MAX_ID_LENGTH][MAX_LENGTH];
int  IDCountBuf[MAX_ID_LENGTH];
int IDptr = 0;
%}


digit [0-9]
letter [a-zA-Z]

Delimite ","|";"|"("|")"|"["|"]"|"{"|"}"

Operator "+"|"-"|"*"|"/"|"%"|"="|"<"|"<="|"!="|">="|">"|"=="|"&&"|"||"|"!"

Keyword "while"|"do"|"if"|"else"|"true"|"false"|"for"|"int"|"print"|"const"|"read"|"boolean"|"bool"|"void"|"float"|"double"|"string"|"continue"|"break"|"return"

Identifier {letter}({letter}|{digit}){0,255}

Integer (0|[1-9]{digit}*)

Float {Integer}\.{digit}*

Scientific ({Integer}*\.{Integer})[E|e][+-]?{Integer}+

String \"(\\.|[^\\"])*\"

WhiteSpace [\40\t]

Pragma ^#"pragma"

%%

{WhiteSpace} {
	LIST;
}

<INITIAL>{Pragma} {
	LIST;
	BEGIN(PRAGMA);
}
<PRAGMA>"source on" {
	LIST;
	Opt_Source = 1;
	BEGIN(INITIAL);
}
<PRAGMA>"source off" {
	LIST;
	Opt_Source = 0;
	BEGIN(INITIAL);
}
<PRAGMA>"token on" {
	LIST;
	Opt_Token = 1;
	BEGIN(INITIAL);
}
<PRAGMA>"token off" {
	LIST;
	Opt_Token = 0;
	BEGIN(INITIAL);
}
<PRAGMA>"statistic on" {
	LIST;
	Opt_Statistic = 1;
	BEGIN(INITIAL);
}

<PRAGMA>"statistic off" {
	LIST;
	Opt_Statistic = 0;
	BEGIN(INITIAL);
}


<C_COMMENT>.    { LIST; }
<INITIAL>"/*"   { LIST; BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { LIST; BEGIN(INITIAL); }

<CC_COMMENT>. { LIST; }
<CC_COMMENT>\n {
	BEGIN(INITIAL);
	yyless(0);
}
<INITIAL>\/\/ { LIST; BEGIN(CC_COMMENT); }

<INITIAL>{Delimite} {
	tokenString("delim", yytext);
}

<INITIAL>{Operator} {
	token(yytext);
}

<INITIAL>{Keyword} {
	tokenString("KW", yytext);
}

<INITIAL>{Identifier} {
	if (Opt_Statistic){
		int isExist = 0;
		int ptr = 0;
		char now[1024];
		for ( ptr = 0 ; ptr < IDptr ; ++ptr ){
			int i;
			for( i = 0 ; IDNameBuf[ptr][i] != '\0' ; i++)
				now[i] = IDNameBuf[ptr][i];
			now[i] = '\0';
			if ( !strcmp(yytext,now) ){
				isExist = 1;
				break;
			}
		}
		if ( isExist )
			IDCountBuf[ptr]++;
		else {
			char tmp[1024];
			int i;
			for( i = 0 ; yytext[i] != '\0' ; ++i )
				IDNameBuf[IDptr][i] = yytext[i];
			IDNameBuf[IDptr][i] = '\0';
			IDCountBuf[IDptr++] = 1;
		}
	}
	tokenString("id",yytext);
}

<INITIAL>{Integer} {
	tokenString("integer",yytext);
}

<INITIAL>{Float} {
	tokenString("float",yytext);
}

<INITIAL>{Scientific} {
	tokenString("scientific",yytext);
}

<INITIAL>{String} {
	char new[1024];
	int ptr = 0;
	int qtr = 1;
	for ( ; yytext[qtr] != '\"' ; ++qtr, ++ptr ){
		if ( yytext[qtr] == '\\' )
			++qtr;
		new[ptr] = yytext[qtr];
	}
	new[ptr] = '\0';
	tokenString("string",new);
}




\n {
    LIST;
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
}

. { // Unrecognized character
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}

%%

int main( int argc, char **argv )
{
    if (argc >= 2)
    {
        yyin = fopen( argv[1], "r" );
        if ( NULL == yyin ) {
            fprintf( stderr, "Cannot open: %s\n", argv[1] );
            exit(-1);
        }
    }
    else
    {
        yyin = stdin;
    }

    yylex();

    if (Opt_Statistic){
        // Print frequency table
		printf("frequencies of identifiers:\n");
		for( int i = 0 ; i < IDptr ; i++ )
			printf("%s\t%d\n",IDNameBuf[i],IDCountBuf[i]);
	}

    exit(0);
}

int yywrap() {
	return 1;
}
