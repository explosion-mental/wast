# wast - wall fast!
# See LICENSE file for copyright and license details.

include config.mk

SRC = wast.c
OBJ = ${SRC:.c=.o}

all: options wast

options:
	@echo wast build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

config.h:
	cp config.def.h config.h

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

wast: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f wast ${OBJ} wast-${VERSION}.tar.gz

dist: clean
	mkdir -p wast-${VERSION}
	cp -R LICENSE Makefile config.mk config.def.h ${SRC} wast-${VERSION}
	tar -cf wast-${VERSION}.tar wast-${VERSION}
	gzip wast-${VERSION}.tar
	rm -rf wast-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f wast ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/wast
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < wast.1 > ${DESTDIR}${MANPREFIX}/man1/wast.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/wast.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/wast \
		${DESTDIR}${MANPREFIX}/man1/wast.1

.PHONY: all options clean dist install uninstall
