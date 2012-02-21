CC=gcc
CFLAGS=-O -Wall -Wextra -I../parser
EXEC=sexpr_test

${EXEC}: sexpr.o
	${CC} ${CLFAGS} -o $@ $^ ../parser/sexpr_parse.o

clean:
	rm -f ${EXEC} sexpr.o
