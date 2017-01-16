; output.j
.class public output
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.field public static intvalue I
; Line #1:	int intvalue;
.field public static floatvalue F
; Line #2:	float floatvalue;
; Line #3:	
.method public static main([Ljava/lang/String;)V
.limit stack 100
.limit locals 100
; Line #4:	int main() {
; Line #5:		int integer_a, integer_b, integer_c;
; Line #6:		bool bool_d;
; Line #7:		float float_e, float_f;
; Line #8:		
; Line #9:		// read 3 integer and 2 float
new java/util/Scanner
dup
getstatic java/lang/System/in Ljava/io/InputStream;
invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
putstatic output/_sc Ljava/util/Scanner;
getstatic output/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextInt()I
istore 0
; Line #10:		read integer_a;
getstatic output/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextInt()I
istore 1
; Line #11:		read integer_b;
getstatic output/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextInt()I
istore 2
; Line #12:		read integer_c;
getstatic output/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextFloat()F
fstore 4
; Line #13:		read float_e;
getstatic output/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextFloat()F
fstore 5
; Line #14:		read float_f;
; Line #15:		
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "--------------------
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #16:		print "--------------------\n;
; Line #17:		
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 2
invokevirtual java/io/PrintStream/print(I)V
; Line #18:		print integer_c;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #19:		print " ;
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 0
invokevirtual java/io/PrintStream/print(I)V
; Line #20:		print integer_a;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #21:		print " ;
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/print(I)V
; Line #22:		print integer_b;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #23:		print " ;
getstatic java/lang/System/out Ljava/io/PrintStream;
fload 4
invokevirtual java/io/PrintStream/print(F)V
; Line #24:		print float_e;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #25:		print " ;
getstatic java/lang/System/out Ljava/io/PrintStream;
fload 5
invokevirtual java/io/PrintStream/print(F)V
; Line #26:		print float_f;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #27:		print "\n;
; Line #28:		
; Line #29:		// swap integer_b and integer_c, take intvalue as temp storage
iload 1
putstatic output/intvalue I
; Line #30:		intvalue = integer_b;
iload 2
istore 1
; Line #31:		integer_b = integer_c;
getstatic output/intvalue I
istore 2
; Line #32:		integer_c = intvalue;
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/print(I)V
; Line #33:		print integer_b;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #34:		print " ;
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 2
invokevirtual java/io/PrintStream/print(I)V
; Line #35:		print integer_c;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #36:		print "\n;
; Line #37:		
; Line #38:		// swap float_e and float_f, take gr as temp storage
fload 5
putstatic output/floatvalue F
; Line #39:		floatvalue = float_f;
fload 4
fstore 5
; Line #40:		float_f = float_e;
getstatic output/floatvalue F
fstore 4
; Line #41:		float_e = floatvalue;
getstatic java/lang/System/out Ljava/io/PrintStream;
fload 4
invokevirtual java/io/PrintStream/print(F)V
; Line #42:		print float_e;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #43:		print " ;
getstatic java/lang/System/out Ljava/io/PrintStream;
fload 5
invokevirtual java/io/PrintStream/print(F)V
; Line #44:		print float_f;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #45:		print "\n;
; Line #46:		
iconst_1
istore 3
; Line #47:		bool_d = true;
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 3
invokevirtual java/io/PrintStream/print(Z)V
; Line #48:		print bool_d;
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "
"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #49:		print "\n;
; Line #50:	
ldc 0
return
; Line #51:		return 0;
return
.end method
; Line #52:	}
; Line #53:	
; Line #54:	
; Line 55:	/*
; Line 56:	Input: 
; Line 57:	6 28 496 1.4142 3.1415
; Line 58:	
; Line 59:	Output: 
; Line 60:	--------------------
; Line 61:	496 6 28 1.4142 3.1415
; Line 62:	496 28
; Line 63:	3.1415 1.4142
; Line 64:	true
; Line #65:	*/
