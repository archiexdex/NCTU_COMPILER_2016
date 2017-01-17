; test.j
.class public test
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
; Line #1: // test.c

.field public static a I
.field public static b I
; Line #2: int a, b;

.field public static d Z
; Line #3: bool d;

.method public static foo(I)I
.limit stack 128
.limit locals 128
; Line #4: int foo( int a ) {

; Line #5:     int i, result;

ldc 0
istore 2
; Line #6:     result = 0;

ldc 1
istore 1
; Line #7:     i = 1;

iload 1
iload 0
isub
ifle L0
iconst_0
goto LL0
L0:
iconst_1
LL0:
; Line #8:     while( i <= a ) {

iload 2
iload 1
iadd
istore 2
; Line #9:         result = result + i;

iload 1
ldc 1
iadd
istore 1
; Line #10: i = i + 1; }

iload 2
ireturn
; Line #11:     return result;

return
.end method
; Line #12: }

.method public static main([Ljava/lang/String;)V
.limit stack 128
.limit locals 128
new java/util/Scanner
dup
getstatic java/lang/System/in Ljava/io/InputStream;
invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
putstatic test/_sc Ljava/util/Scanner;
; Line #13: void main() {

; Line #14:     int c ;

getstatic test/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextInt()I
putstatic test/a I
; Line #15:     read a;

getstatic test/a I
invokestatic test/foo(I)I
istore 0
; Line #16:     c = foo( a);

getstatic test/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextInt()I
istore 0
; Line #17:     read c;

getstatic java/lang/System/out Ljava/io/PrintStream;
iload 0
invokevirtual java/io/PrintStream/print(I)V
; Line #18:     print c;

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #19:     print "\n";

iload 0
ldc 100
isub
ifge L1
iconst_0
goto LL1
L1:
iconst_1
LL1:
ifeq Lelse1
; Line #20:     if( c >= 100 ) {

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "c >= 100 
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #21:         print "c >= 100 \n";

; Line #22:     }

goto Lexit1
Lelse1 :
; Line #23:     else {

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "c < 100 
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #24:         print "c < 100 \n";

Lexit1 :
return
.end method
; Line #25: } }

