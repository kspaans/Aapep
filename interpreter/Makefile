CC=gcc
CFLAGS=-O -Wall -Wextra -MMD -I../parser
EXEC=Aapep
OBJS=interpreter.o hash.o
DEPS=${OBJS:.o=.d}

${EXEC}: ${OBJS}
	${CC} ${CFLAGS} -o $@ $^  ../parser/sexpr_parse.o

clean:
	rm ${EXEC} ${OBJS} ${DEPS}

valgrind: ${EXEC}
	valgrind -v --leak-check=full ./${EXEC}

-include ${DEPS}
