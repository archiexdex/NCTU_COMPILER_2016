TARGET = parser
OBJECT = lex.yy.c y.tab.c lex.yy.o y.output parser y.tab.h
CC = gcc
CXX = g++
CCFLAGS = -Wall -Wextra -pedantic -g3 -Wno-unused-function
CXXFLAGS = -Wall -Wextra -pedantic -g3 -Wno-unused-function -std=c++11
LEX = flex
LEXXX = flex++
YACC = yacc
YACCFLAG = -d -v -t
LIBS = -lfl

all: $(TARGET)

$(TARGET): y.tab.c lex.yy.o main.cpp
	$(CXX) $^ -o $(TARGET) $(CXXFLAGS) $(LIBS)

y.tab.c: parser.y
	$(YACC) $(YACCFLAG) $<

lex.yy.o: lex.yy.c
	$(CC) -c $< -o $@

lex.yy.c: lex.l
	$(LEX) $<

clean:
	rm -f $(TARGET) $(OBJECT)

