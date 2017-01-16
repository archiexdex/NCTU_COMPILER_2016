; output.j
.class public output
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.field public static mySeed I
; Line #1:	int mySeed;
; Line #2:	const int INITIALIZE = 123456789;
; Line #3:	
; Line #4:	void srand( int seed );
; Line #5:	int rand();
; Line #6:	
; Line #7:	
; Line #8:	
.method public static main([Ljava/lang/String;)V
.limit stack 100
.limit locals 100
; Line #9:	int main() {
ldc 1
istore 0
; Line #10:		int i = 1, grade;
; Line #11:		
ldc 123456789
invokestatic output/srand(I)V
; Line #12:		srand( INITIALIZE );	// print "student (number)    (grade)    pass/fail"
; Line #13:		
; Line #14:		// print "student (number)    (grade)    pass/fail"
Lwhile_c1 : 
  iload 0
ldc 10
 isub
 ifle L1
 iconst_0
 goto LL1
L1: 
 iconst_1
LL1: 
ifeq Lwhile_e1
; Line #15:		while( i <= 10 ) {
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "student "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #16:			print "student ;
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 0
invokevirtual java/io/PrintStream/print(I)V
; Line #17:			print i;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "	"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #18:			print "\t;
invokestatic output/rand()I
ldc 71
irem
ldc 30
iadd
istore 1
; Line #19:			grade = rand() % 71 + 30;
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/print(I)V
; Line #20:			print grade;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "	"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #21:			print "\t;
; Line #22:			
iload 1
ldc 60
 isub
 ifge L2
 iconst_0
 goto LL2
L2: 
 iconst_1
LL2: 
ifeq Lelse_1
; Line #23:			if( grade >= 60 ) {
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "pass
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #24:				print "pass\n;
; Line #25:			}
; Line #26:			
 goto Lexit_1
Lelse_1 : 
; Line #27:			else {
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "fail
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #28:				print "fail\n;
Lexit_1 : 
; Line #29:			}
; Line #30:			
iload 0
ldc 1
iadd
istore 0
; Line #31:			i = i + 1;
goto Lwhile_c1
 Lwhile_e1 : 
; Line #32:		}
; Line #33:	
ldc 0
return
; Line #34:		return 0;
return
.end method
; Line #35:	}
; Line #36:	
.method public static srand(I)V
.limit stack 100
.limit locals 100
; Line #37:	void srand( int seed ) {
iload 0
putstatic output/mySeed I
; Line #38:		mySeed = seed;
return
.end method
; Line #39:	}
; Line #40:	
.method public static rand()I
.limit stack 100
.limit locals 100
; Line #41:	int rand() {
getstatic output/mySeed I
ldc 234
imul
ldc 123
iadd
putstatic output/mySeed I
; Line #42:		mySeed = mySeed * 234 + 123;
; Line #43:		
getstatic output/mySeed I
ldc 0
 isub
 ifge L3
 iconst_0
 goto LL3
L3: 
 iconst_1
LL3: 
ifeq Lelse_2
; Line #44:		if( mySeed >= 0 ) {
getstatic output/mySeed I
ireturn
; Line #45:			return mySeed;
; Line #46:		}
; Line #47:		
 goto Lexit_2
Lelse_2 : 
; Line #48:		else {
getstatic output/mySeed I
ineg
ireturn
; Line #49:			return -mySeed;
Lexit_2 : 
; Line #50:		}	
ldc 0
ireturn
; Line #51:		return 0;
ireturn
.end method
; Line #52:	}
; Line 53:	/*
; Line 54:	Input:
; Line 55:	No input
; Line 56:	
; Line 57:	Output:
; Line 58:	student 1       91      pass
; Line 59:	student 2       44      fail
; Line 60:	student 3       66      pass
; Line 61:	student 4       81      pass
; Line 62:	student 5       100     pass
; Line 63:	student 6       72      pass
; Line 64:	student 7       35      fail
; Line 65:	student 8       70      pass
; Line 66:	student 9       33      fail
; Line 67:	student 10      67      pass
; Line #68:	*/
