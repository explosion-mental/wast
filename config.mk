# wast version
VERSION = 0.0

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS =
LIBS = -lm

# flags
CPPFLAGS = -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L -DSTB_IMAGE_IMPLEMENTATION -DVERSION=\"${VERSION}\"
CFLAGS   = -g -std=c99 -pedantic -Wall -O3 ${INCS} ${CPPFLAGS}
LDFLAGS  = -g ${LIBS}
#CFLAGS  = -std=c99 -pedantic -Wall -Os ${INCS} ${CPPFLAGS}
#LDFLAGS = ${LIBS}

# compiler and linker
CC = cc
#CC = tcc
