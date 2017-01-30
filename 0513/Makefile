CC      = gcc
CFLAGS  = -Wall -O -I.

TARGET = libmat.a libmat.so
SRCS   = vector.c matrix.c
OBJS   = $(SRCS:.c=.o)

all: $(TARGET)

libmat.a: $(OBJS)
	ar -ruv $@ $(OBJS)
	-ranlib $@

libmat.so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	-rm $(TARGET) $(OBJS)

vector.o: vecmat.h
matrix.o: vecmat.h
