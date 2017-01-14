; test.j
.class public test
.super java/lang/Object
; Line #1: // test.c

.field public static a I
.field public static b I
; Line #2: int a, b;

.field public static d Z
; Line #3: bool d;

; Line #4: int foo( int a ) {

; Line #5:     int i, result;

; Line #6:     result = 0;

; Line #7:     i = 1;

; Line #8:     while( i <= a ) {

; Line #9:         result = result + i;

; Line #10: i = i + 1; }

; Line #11:     return result;

; Line #12: }

; Line #13: int main() {

; Line #14:     int c ;

; Line #15:     read a;

; Line #16:     c = foo( a );

; Line #17:     print c;

; Line #18:     print "\n;

; Line #19:     if( c >= 100 ) {

; Line #20:         print "c >= 100 \n;

; Line #21:     }

; Line #22:     else {

; Line #23:         print "c < 100 \n;

; Line #24: } }

