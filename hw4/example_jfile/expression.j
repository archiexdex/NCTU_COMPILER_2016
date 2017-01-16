; output.j
.class public output
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.method public static main([Ljava/lang/String;)V
.limit stack 100
.limit locals 100
; Line #1:	int main() {
; Line #2:		int a, b, c, d, e, f;
; Line #3:		float i, j, k;
; Line #4:		
ldc 4
istore 0
; Line #5:		a = 4;						// 4
ldc 8
ldc 7
iadd
istore 1
; Line #6:		b = 8 + 7;					// 15
ldc 9
ldc 3
imul
istore 2
; Line #7:		c = 9 * 3;					// 27
ldc 7
ldc 2
idiv
istore 3
; Line #8:		d = 7 / 2;					// 3
ldc 19
ldc 4
irem
istore 4
; Line #9:		e = 19 % 4;					// 3
; Line #10:		
iload 0
iload 1
iadd
iload 2
iadd
iload 3
iadd
iload 4
iadd
istore 5
; Line #11:		f = a + b + c + d + e ;		// 52
; Line #12:		
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 5
invokevirtual java/io/PrintStream/print(I)V
; Line #13:		print f;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #14:		print "\n;
; Line #15:		
ldc 2.222200
ldc 2
i2f
fmul
fstore 6
; Line #16:		i = 2.2222 * 2;
ldc 4.444400
ldc 1.111100
fadd
fstore 7
; Line #17:		j = 4.4444 + 1.1111;
fload 6
fload 7
fdiv
fstore 8
; Line #18:		k = i / j;
; Line #19:		
getstatic java/lang/System/out Ljava/io/PrintStream;
fload 8
invokevirtual java/io/PrintStream/print(F)V
; Line #20:		print k;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #21:		print "\n;
; Line #22:	
ldc 0
return
; Line #23:		return 0;
return
.end method
; Line #24:	}
; Line #25:	
; Line #26:	
; Line 27:	/*
; Line 28:	Input:
; Line 29:	No input
; Line 30:	
; Line 31:	Output:
; Line 32:	52
; Line 33:	0.79999995
; Line #34:	*/
