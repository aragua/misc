CC=gcc

ifeq ($(DEBUG),1)
CFLAGS=-O0 -g -DDEBUG
else
CFLAGS=-O3
endif

all: ipnotify

ipnotify: ipnotify.c
	${CC} ${CFLAGS} -o $@ $^

indent: ipnotify.c
	indent -linux $^

clean:
	rm -rf ipnotify *.*~
