CC=gcc
CFLAGS=-I.
DEPS = circular_buff.h
OBJ = Client_app.o circular_buff.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Circular_Buffer: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
