SHELL = /bin/sh


subdirs = @subdirs@
top_srcdir = .
srcdir = .
prefix = /usr/local
exec_prefix = ${prefix}
bindir = $(exec_prefix)/bin
infodir = $(prefix)/info
libdir = $(prefix)/lib/gnudl
mandir = $(prefix)/man/man1

CC = g++
CPPFLAGS = 
CFLAGS = $(CPPFLAGS) -g -O2
LDFLAGS = 
LIBS = 
INSTALL = @INSTALL@

PROGRAMS=dumpcalls gensupercheckpartial
DOBJECTS=dumpcalls.o gensupercheckpartial.o

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS) $(DOBJECTS) core *.stackdump

dumpcalls: dumpcalls.o
	$(CC) -o $@ $^

gensupercheckpartial: gensupercheckpartial.o
	$(CC) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $<

# automatic re-running of configure if the configure.in file has changed
${srcdir}/configure: configure.in #aclocal.m4
	cd ${srcdir} && autoconf

 	# autoheader might not change config.h.in, so touch a stamp file
${srcdir}/config.h.in: stamp-h.in
${srcdir}/stamp-h.in: configure.in #aclocal.m4
	cd ${srcdir} && autoheader
	echo timestamp > ${srcdir}/stamp-h.in

config.h: stamp-h
stamp-h: config.h.in config.status
	./config.status
Makefile: Makefile.in config.status
	./config.status
config.status: configure
	./config.status --recheck


