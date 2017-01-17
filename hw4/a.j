; a.j
.class public a
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.field public static a I
.field public static b I
; Line #1: int a , b;

; Line #2: 

.field public static c F
; Line #3: float c;

; Line #4: 

.method public static main([Ljava/lang/String;)I
.limit stack 128
.limit locals 128
new java/util/Scanner
dup
getstatic java/lang/System/in Ljava/io/InputStream;
invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
putstatic a/_sc Ljava/util/Scanner;
; Line #5: int main() {

; Line #6: 	int i;

; Line #7: 	int j;

ldc 1
istore 0
; Line #8: 	i = 1;

ldc 5
istore 1
; Line #9: 	j = 5;

ldc 1
istore 0
Lbegin0 :
iload 0
ldc 5
isub
iflt L0
iconst_0
goto LL0
L0:
iconst_1
LL0:
ifeq Lexit0
goto Lnext0
Lincr0 :
iload 0
ldc 1
iadd
istore 0
goto Lbegin0
Lnext0 :
; Line #10: 	for ( i = 1; i < 5 ; i = i + 1) {

ldc 10
istore 1
; Line #11: 		j = 10;

goto Lincr0
Lexit0 :
; Line #12: 	}

; Line #13: 

Lbegin1 :
iload 0
ldc 10
isub
iflt L1
iconst_0
goto LL1
L1:
iconst_1
LL1:
ifeq Lexit1
; Line #14: 	while ( i < 10 ) {

iload 0
ldc 1
iadd
istore 0
; Line #15: 		i = i + 1;

goto Lbegin1
Lexit1 :
; Line #16: 	}

; Line #17: 

Lbegin2 :
; Line #18: 	do {

iload 0
ldc 1
iadd
istore 0
; Line #19: 		i = i + 1;

iload 0
ldc 15
isub
iflt L2
iconst_0
goto LL2
L2:
iconst_1
LL2:
ifeq Lexit2
goto Lbegin2
Lexit2 :
; Line #20: 	} while( i < 15 );

return
.end method
; Line #21: }

