CFLAGS=-g
PROGRAMS=dumpcalls gensupercheckpartial
DOBJECTS=dumpcalls.o gensupercheckpartial.o
CC=g++

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS) $(DOBJECTS) core *.stackdump

dumpcalls: dumpcalls.o
	$(CC) -o $@ $^

gensupercheckpartial: gensupercheckpartial.o
	$(CC) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $<


