DESTDIR ?= /usr/
BINDIR ?= ${DESTDIR}/bin
LIBDIR ?= ${DESTDIR}/lib
INCDIR ?= ${DESTDIR}/include

all:

install:
	install -m 0755 -d ${BINDIR}
	install -m 0755 -d ${LIBDIR}
	install -m 0755 -d ${INCDIR}
	install -m 0755 scripts/gpio_off ${BINDIR}/gpio_off
	install -m 0755 scripts/gpio_on ${BINDIR}/gpio_on
