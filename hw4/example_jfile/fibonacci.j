; example_jfile/fibonacci.j
.class public example_jfile/fibonacci
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
; Line #1: int fibonacci( int n );

; Line #2: 

.method public static main([Ljava/lang/String;)I
.limit stack 128
.limit locals 128
new java/util/Scanner
dup
getstatic java/lang/System/in Ljava/io/InputStream;
invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
putstatic example_jfile/fibonacci/_sc Ljava/util/Scanner;
; Line #3: int main() {

; Line #4: 	int i;

; Line #5: 

ldc 1
iload 0
ldc 20
iload 0
ldc 1
; Line #6: 	for( i = 1 ; i <= 20 ; i = i + 1 ) {

iload 0
invokestatic example_jfile/fibonacci/fibonacci(I)I
getstatic java/lang/System/out Ljava/io/PrintStream;
invokevirtual java/io/PrintStream/print(I)V
; Line #7: 		print fibonacci( i );

ldc "  "
getstatic java/lang/System/out Ljava/io/PrintStream;
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #8: 		print "  ";

; Line #9: 	}

; Line #10: 	

ldc "
"
getstatic java/lang/System/out Ljava/io/PrintStream;
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #11: 	print "\n";

; Line #12: 	

ldc 0
; Line #13: 	return 0;

.end method
; Line #14: }

; Line #15: 

.method public static fibonacci(I)I
.limit stack 128
.limit locals 128
; Line #16: int fibonacci( int n ) {

iload 0
ldc 1
iload 0
ldc 2
; Line #17: 	if( n == 1 || n == 2 ) {

ldc 1
; Line #18: 		return 1;

; Line #19: 	}

; Line #20: 		

; Line #21: 	else {

iload 0
ldc 1
invokestatic example_jfile/fibonacci/fibonacci(I)I
iload 0
ldc 2
invokestatic example_jfile/fibonacci/fibonacci(I)I
; Line #22: 		return fibonacci( n - 1 ) + fibonacci( n - 2 );

; Line #23: 	}

; Line #24: 

ldc 0
; Line #25: 	return 0;

.end method
; Line #26: }

; Line #27: 

; Line #34: */

