#ifndef _XD_H_
#define _XD_H_

#include <stdbool.h>

#define T_INT    0 
#define T_FLOAT  1
#define T_DOUBLE 2
#define T_BOOL   3
#define T_STRING 4
#define T_VOID   5
#define T_ARRAY  6

#define K_FUNCTION  0
#define K_PARAMETER 1
#define K_VARIABLE  2
#define K_CONSTANT  3



typedef struct _Type {
	int t; // type
	int dim; // dimetion
	int currdim;
	int* array;
} Type;
typedef struct _Value {
	
	int    i;
	float  f;
	double d;
	char*  s;
	bool   b; 
} Value;

typedef struct  _VarList {
	Type     type;
	char*    text;
	struct _Value    val;
	struct _VarList* next;
} VarList;



typedef struct _ConstVal {
	Type  type;
	Value val ;
} ConstVal;

#endif
