#Our compiler.
CC = gcc
CFLAGS = -g
LEX = lex
YACC = yacc

#Default rule.

default: spl

spl: lex.yy.o y.tab.o node.o data.o spl.o label.o file.o
	$(CC) $(CFLAGS) -o spl  lex.yy.o y.tab.o node.o data.o spl.o label.o file.o

node.o: node.c node.h
	$(CC) $(CFLAGS) -c node.c

file.o: file.c file.h
	$(CC) $(CFLAGS) -c file.c

label.o: label.c label.h
	$(CC) $(CFLAGS) -c label.c

data.o: data.c data.h
	$(CC) $(CFLAGS) -c data.c

spl.o: spl.c spl.h data.h node.h
	$(CC) $(CFLAGS) -c spl.c

lex.yy.o: lex.yy.c y.tab.c
	$(CC) $(CFLAGS) -c lex.yy.c

y.tab.o: y.tab.c y.tab.h
	$(CC) $(CFLAGS) -c y.tab.c

lex.yy.c: spllexer.l
	$(LEX) spllexer.l

y.tab.c: splparser.y
	$(YACC) -d splparser.y

clean:
	$(RM) spl *.o lex.yy.c y.tab.h y.tab.c
