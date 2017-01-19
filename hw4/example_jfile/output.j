; output.j
.class public output
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
; Line #1: int fibonacci( int n );

; Line #2: 

.method public static main([Ljava/lang/String;)V
.limit stack 128
.limit locals 128
new java/util/Scanner
dup
getstatic java/lang/System/in Ljava/io/InputStream;
invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
putstatic output/_sc Ljava/util/Scanner;
; Line #3: int main() {

; Line #4: 	int i;

; Line #5: 

ldc 1
istore 0
Lbegin0 :
iload 0
ldc 20
isub
ifle L1
iconst_0
goto LL1
L1:
iconst_1
LL1:
ifeq Lexit0
goto Lnext0
Lincr0 :
iload 0
ldc 1
iadd
istore 0
goto Lbegin0
Lnext0 :
; Line #6: 	for( i = 1 ; i <= 20 ; i = i + 1 ) {

getstatic java/lang/System/out Ljava/io/PrintStream;
iload 0
invokestatic output/fibonacci(I)I
invokevirtual java/io/PrintStream/print(I)V
; Line #7: 		print fibonacci( i );

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "  "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #8: 		print "  ";

goto Lincr0
Lexit0 :
; Line #9: 	}

; Line #10: 	

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #11: 	print "\n";

; Line #12: 	

ldc 0
return 
; Line #13: 	return 0;

return
.end method
; Line #14: }

; Line #15: 

.method public static fibonacci(I)I
.limit stack 128
.limit locals 128
; Line #16: int fibonacci( int n ) {

iload 0
ldc 1
isub
ifeq L3
iconst_0
goto LL3
L3:
iconst_1
LL3:
iload 0
ldc 2
isub
ifeq L4
iconst_0
goto LL4
L4:
iconst_1
LL4:
ior
ifeq Lelse2
; Line #17: 	if( n == 1 || n == 2 ) {

ldc 1
ireturn 
; Line #18: 		return 1;

; Line #19: 	}

; Line #20: 		

goto Lexit2
Lelse2 :
; Line #21: 	else {

iload 0
ldc 1
isub
invokestatic output/fibonacci(I)I
iload 0
ldc 2
isub
invokestatic output/fibonacci(I)I
iadd
ireturn 
; Line #22: 		return fibonacci( n - 1 ) + fibonacci( n - 2 );

Lexit2 :
; Line #23: 	}

; Line #24: 

ldc 0
ireturn 
; Line #25: 	return 0;

ireturn
.end method
; Line #26: }

; Line #27: 

; Line #34: */

