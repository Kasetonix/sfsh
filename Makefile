CSTD = c99
SRC = sfsh.c
OBJ = ${SRC:.c=.o}
CFLAGS = -std=${CSTD}

all: sfsh

light_asan: CFLAGS += -DLIGHT_MODE
light_asan: asan

light_debug: CFLAGS += -DLIGHT_MODE
light_debug: debug

light: CFLAGS += -DLIGHT_MODE
light: release

asan: LFLAGS += -fsanitize=address
asan: debug

debug: CFLAGS += -Wall -Wextra -g -pedantic
debug: sfsh

release: CFLAGS += -O3
release: sfsh

sfsh: ${OBJ}
	${CC} ${OBJ} ${LIBS} ${LFLAGS} -o $@

clean: sfsh
	rm -f ${OBJ}
