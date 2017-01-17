; example_jfile/if_stmt.j
.class public example_jfile/if_stmt
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.method public static main([Ljava/lang/String;)V
.limit stack 128
.limit locals 128
new java/util/Scanner
dup
getstatic java/lang/System/in Ljava/io/InputStream;
invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
putstatic example_jfile/if_stmt/_sc Ljava/util/Scanner;
; Line #1: int main(){ 

; Line #2: 	float a, b;

; Line #3: 	

getstatic example_jfile/if_stmt/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextFloat()F
fstore 0
; Line #4: 	read a;

getstatic example_jfile/if_stmt/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextFloat()F
fstore 1
; Line #5: 	read b;

; Line #6: 	

fload 0
fload 1
fcmpl
ifeq L1
iconst_0
goto LL1
L1:
iconst_1
LL1:
ifeq Lelse0
; Line #7: 	if( a == b ) {

getstatic java/lang/System/out Ljava/io/PrintStream;
fload 0
invokevirtual java/io/PrintStream/print(F)V
; Line #8: 		print a;

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " is equal to "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #9: 		print " is equal to ";

getstatic java/lang/System/out Ljava/io/PrintStream;
fload 1
invokevirtual java/io/PrintStream/print(F)V
; Line #10: 		print b;

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #11: 		print "\n";

; Line #12: 	}

; Line #13: 	

Lelse0 :
fload 0
fload 1
fcmpl
ifgt L3
iconst_0
goto LL3
L3:
iconst_1
LL3:
ifeq Lelse2
; Line #14: 	if( a > b ) {

getstatic java/lang/System/out Ljava/io/PrintStream;
fload 0
invokevirtual java/io/PrintStream/print(F)V
; Line #15: 		print a;

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " is greater than "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #16: 		print " is greater than ";

getstatic java/lang/System/out Ljava/io/PrintStream;
fload 1
invokevirtual java/io/PrintStream/print(F)V
; Line #17: 		print b;

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #18: 		print "\n";

; Line #19: 	}

; Line #20: 	

goto Lexit2
Lelse2 :
; Line #21: 	else {

getstatic java/lang/System/out Ljava/io/PrintStream;
fload 0
invokevirtual java/io/PrintStream/print(F)V
; Line #22: 		print a;

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " is less than or equal to "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #23: 		print " is less than or equal to ";

getstatic java/lang/System/out Ljava/io/PrintStream;
fload 1
invokevirtual java/io/PrintStream/print(F)V
; Line #24: 		print b;

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #25: 		print "\n";

Lexit2 :
; Line #26: 	}

; Line #27: 

ldc 0
return 
; Line #28: 	return 0;

return
.end method
; Line #29: }

; Line #30: 

; Line #31: 

; Line #38: */

; Line #39: 

