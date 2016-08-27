SRC = $(wildcard src/*.c)
OBJ = ${SRC:.c=.o}

all: baislicka

baislicka: ${OBJ}
	@echo ${OBJ}
	@echo CC -o $@
	@${CC} -o bin/$@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f src/*.o bin/baislicka

.PHONY: all clean
