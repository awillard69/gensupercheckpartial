CFLAGS=-g
PROGRAMS=dumpcalls gensupercheckpartial
DOBJECTS=dumpcalls.o gensupercheckpartial
CC=g++

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS) $(OBJECTS) core *.stackdump

dumpcalls: dumpcalls.o
	$(CC) -o $@ $^

gensupercheckpartial: gensupercheckpartial.o
	$(CC) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $<


