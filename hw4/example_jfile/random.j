; example_jfile/random.j
.class public example_jfile/random
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.field public static mySeed I
; Line #1: int mySeed;

; Line #2: const int INITIALIZE = 123456789;

; Line #3: 

; Line #4: void srand( int seed );

; Line #5: int rand();

; Line #6: 

.method public static main([Ljava/lang/String;)V
.limit stack 128
.limit locals 128
new java/util/Scanner
dup
getstatic java/lang/System/in Ljava/io/InputStream;
invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
putstatic example_jfile/random/_sc Ljava/util/Scanner;
; Line #7: int main() {

ldc 1
; Line #8: 	int i = 1, grade;

; Line #9: 	

getstatic example_jfile/random/INITIALIZE I
invokestatic example_jfile/random/srand(I)V
; Line #10: 	srand( INITIALIZE );	// print "student (number)    (grade)    pass/fail"

; Line #11: 	

; Line #12: 	// print "student (number)    (grade)    pass/fail"

Lbegin0 :
iload 0
ldc 10
isub
ifle L1
iconst_0
goto LL1
L1:
iconst_1
LL1:
ifeq Lexit0
; Line #13: 	while( i <= 10 ) {

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "student "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #14: 		print "student ";

getstatic java/lang/System/out Ljava/io/PrintStream;
iload 0
invokevirtual java/io/PrintStream/print(I)V
; Line #15: 		print i;

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "	"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #16: 		print "\t";

invokestatic example_jfile/random/rand()I
ldc 71
irem
ldc 30
iadd
istore 1
; Line #17: 		grade = rand() % 71 + 30;

getstatic java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/print(I)V
; Line #18: 		print grade;

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "	"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #19: 		print "\t";

; Line #20: 		

iload 1
ldc 60
isub
ifge L3
iconst_0
goto LL3
L3:
iconst_1
LL3:
ifeq Lelse2
; Line #21: 		if( grade >= 60 ) {

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "pass
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #22: 			print "pass\n";

; Line #23: 		}

; Line #24: 		

goto Lexit2
Lelse2 :
; Line #25: 		else {

getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "fail
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #26: 			print "fail\n";

Lexit2 :
; Line #27: 		}

; Line #28: 		

iload 0
ldc 1
iadd
istore 0
; Line #29: 		i = i + 1;

goto Lbegin0
Lexit0 :
; Line #30: 	}

; Line #31: 

ldc 0
return 
; Line #32: 	return 0;

return
.end method
; Line #33: }

; Line #34: 

.method public static srand(I)V
.limit stack 128
.limit locals 128
; Line #35: void srand( int seed ) {

iload 0
putstatic example_jfile/random/mySeed I
; Line #36: 	mySeed = seed;

return
.end method
; Line #37: }

; Line #38: 

.method public static rand()I
.limit stack 128
.limit locals 128
; Line #39: int rand() {

getstatic example_jfile/random/mySeed I
ldc 234
imul
ldc 123
iadd
putstatic example_jfile/random/mySeed I
; Line #40: 	mySeed = mySeed * 234 + 123;

; Line #41: 	

getstatic example_jfile/random/mySeed I
ldc 0
isub
ifge L5
iconst_0
goto LL5
L5:
iconst_1
LL5:
ifeq Lelse4
; Line #42: 	if( mySeed >= 0 ) {

getstatic example_jfile/random/mySeed I
ireturn 
; Line #43: 		return mySeed;

; Line #44: 	}

; Line #45: 	

goto Lexit4
Lelse4 :
; Line #46: 	else {

getstatic example_jfile/random/mySeed I
ineg
ireturn 
; Line #47: 		return -mySeed;

Lexit4 :
; Line #48: 	}	

ldc 0
ireturn 
; Line #49: 	return 0;

ireturn
.end method
; Line #50: }

; Line #66: */

