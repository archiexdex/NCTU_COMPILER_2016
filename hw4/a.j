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

ldc 5
istore 1
; Line #8: 	j = 5;

ldc 1
iload 0
ldc 10
isub
iflt L0
iconst_0
goto LL0
L0:
iconst_1
LL0:
iload 0
ldc 1
iadd
; Line #9: 	for ( i = 1 ; i < 10 ; i = i + 1 ){

ldc 5
istore 1
; Line #10: 		j = 5;

; Line #11: 	}

return
.end method
; Line #12: }

